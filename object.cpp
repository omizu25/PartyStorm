//=============================================================================
//
// オブジェクトクラス(object.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "object.h"
#include "debug_proc.h"
#include "renderer.h"
#include "application.h"
#include "camera.h"
#include "instancing.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CObject *CObject::m_pTop[PRIORITY_MAX] = {};				// 先頭オブジェクトへのポインタ
CObject *CObject::m_pCurrent[PRIORITY_MAX] = {};			// 現在の(一番後ろ)オブジェクトへのポインタ
int CObject::m_nMaxObject = 0;								// 使用数
int CObject::m_nPriorityMaxObj[PRIORITY_MAX] = {};			// プライオリティごとのオブジェクト数
bool CObject::m_bPause = false;								// ポーズを使用しているかどうか

//=============================================================================
// インスタンスの解放
// Author : 唐﨑結斗
// 概要 : すべてのインスタンスを解放する処理
//=============================================================================
void CObject::ReleaseAll(bool bPermanent)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CObject *pObjectNext = pObject->m_pNext;
				if (bPermanent)
				{
					if (!pObject->m_bDeath)
					{// オブジェクトの解放
						pObject->Release();
					}
				}
				else
				{
					if (!pObject->m_bDeath
						&& pObject->m_objectType != CObject::OBJTYPE_FADE
						&& pObject->m_objectType != OBJTYPE_PAUSE)
					{// オブジェクトの解放
						pObject->Release();
					}
				}

				// 現在のオブジェクトの次のオブジェクトを更新
				pObject = pObjectNext;
			}
		}
	}

	// すべてのリスト解除
	ReleaseListAll();
}

//=============================================================================
// すべてのインスタンスの更新
// Author : 唐﨑結斗
// 概要 : 使用されてるインスタンスの更新処理を呼び出す
//=============================================================================
void CObject::UpdateAll(void)
{
	// オブジェクトの使用数の表示
	CDebugProc::Print("オブジェクトの使用数 : %d\n", m_nMaxObject);

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CObject *pObjectNext = pObject->m_pNext;

				if (!pObject->m_bDeath
					&& !m_bPause)
				{// オブジェクトの更新
					pObject->Update();
				}
				else if (!pObject->m_bDeath
					&& m_bPause
					&& pObject->m_objectType == OBJTYPE_PAUSE)
				{// オブジェクト更新処理
					pObject->Update();
				}
				
				// 現在のオブジェクトの次のオブジェクトを更新
				pObject = pObjectNext;
			}
		}
	}
}

//=============================================================================
// すべてのインスタンスの描画
// Author : 唐﨑結斗
// 概要 : 使用されてるインスタンスの描画処理を呼び出す
//=============================================================================
void CObject::DrawAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (nCntPriority == PRIORITY_EFFECT)
		{// エフェクト
			CApplication::GetInstancing()->Draw();
			continue;
		}

		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CObject *pObjectNext = pObject->m_pNext;

				if (!pObject->m_bDeath)
				{
					// オブジェクトの描画
					pObject->Draw();
				}

				// 現在のオブジェクトの次のオブジェクトを更新
				pObject = pObjectNext;
			}
		}
	}

	// すべてのリスト解除
	ReleaseListAll();
}

//=============================================================================
// すべてのオブジェクトのリスト解除
// Author : 唐﨑結斗
// 概要 : すべてのオブジェクトのリスト解除を呼び出す
//=============================================================================
void CObject::ReleaseListAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CObject *pObjectNext = pObject->m_pNext;

				if (pObject->m_bDeath)
				{// オブジェクトのリスト解除
					pObject->ReleaseList();
				}

				// 現在のオブジェクトの次のオブジェクトを更新
				pObject = pObjectNext;
			}
		}
	}
}

#ifdef _DEBUG
void CObject::ReleaseTop(int nPriority)
{
	if (m_pTop[nPriority] != nullptr)
	{
		m_pTop[nPriority]->Release();
	}
}
void CObject::ReleaseCurrent(int nPriority)
{
	if (m_pCurrent[nPriority] != nullptr)
	{
		m_pCurrent[nPriority]->Release();
	}
}
void CObject::RandomRelease(int nPriority)
{
	if (m_pTop[nPriority] != nullptr)
	{// 変数宣言
		CObject *pObject = m_pTop[nPriority];
		int nDestObject = rand() % m_nPriorityMaxObj[nPriority];

		for(int nCntObject = 0; nCntObject < nDestObject; nCntObject++)
		{// オブジェクトの更新
			pObject = pObject->m_pNext;
		}

		pObject->Release();
	}
}
#endif // DEBUG

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CObject::CObject(int nPriority) : m_pPrev(nullptr),		// 前のオブジェクトへのポインタ
m_pNext(nullptr),										// 次のオブジェクトへのポインタ
m_nLevPriority(nPriority),								// プライオリティのレベル
m_bDeath(false)											// 死亡フラグ
{
	// 使用数のインクリメント
	m_nMaxObject++;
	m_nPriorityMaxObj[m_nLevPriority]++;

	if (m_pTop[m_nLevPriority] == nullptr)
	{// 先頭ポインターの設定
		m_pTop[m_nLevPriority] = this;
	}
	else
	{// 過去の最後尾オブジェクトの次のオブジェクトを設定
		m_pCurrent[m_nLevPriority]->m_pNext = this;

		// 自分の前のオブジェクトの設定
		this->m_pPrev = m_pCurrent[m_nLevPriority];
	}
	
	// 自分を現在のオブジェクトに設定
	m_pCurrent[m_nLevPriority] = this;
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CObject::~CObject()
{
	
}

//=============================================================================
// インスタンスの解放
// Author : 唐﨑結斗
// 概要 : すべてのインスタンスを解放する処理
//=============================================================================
void CObject::Release(void)
{
	// 使用数のデクリメント
	m_nMaxObject--;
	m_nPriorityMaxObj[m_nLevPriority]--;

	// 死亡フラグを立てる
	m_bDeath = true;
}

//=============================================================================
// リストの解除と破棄
// Author : 唐﨑結斗
// 概要 : リストの解除と破棄をする処理
//=============================================================================
void CObject::ReleaseList(void)
{
	if (m_pTop[m_nLevPriority] == this)
	{// 先頭オブジェクトを自分の次のオブジェクトに設定
		m_pTop[m_nLevPriority] = m_pNext;
	}
	if (m_pCurrent[m_nLevPriority] == this)
	{// 最後尾オブジェクトを自分の前のオブジェクトに設定
		m_pCurrent[m_nLevPriority] = m_pPrev;
	}

	if (m_pPrev != nullptr)
	{// 前のオブジェクトの前のオブジェクトに自分の次のオブジェクトを設定
		m_pPrev->m_pNext = m_pNext;
	}
	if (m_pNext != nullptr)
	{// 自分の次のオブジェクトに自分の前のオブジェクトを設定
		m_pNext->m_pPrev = m_pPrev;
	}

	// オブジェクトの解放
	delete this;
}

//=============================================================================
// 矩形の判定
// Author : 唐﨑結斗
// 概要 : ターゲットとの矩形判定
//=============================================================================
bool CObject::ColisonRectangle3D(CObject * target, bool bExtrude)
{// 返り値用の変数
	bool bColision = false;

	// 自分の情報を取得する
	D3DXVECTOR3 pos = GetPos() + m_colisonPos;
	D3DXVECTOR3 posOld = GetPosOld() + m_colisonPos;
	D3DXVECTOR3 size = m_colisonSize / 2.0f;

	// 目標の情報取得
	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
	D3DXVECTOR3 sizeTarget = target->GetColisonSize() / 2.0f;

	if ((pos.z - size.z) < (posTarget.z + sizeTarget.z)
		&& (pos.z + size.z) > (posTarget.z - sizeTarget.z)
		&& (pos.x - size.x) < (posTarget.x + sizeTarget.x)
		&& (pos.x + size.x) > (posTarget.x - sizeTarget.x))
	{// モデル内にいる(XZ軸)
		if ((posOld.y + size.y) <= (posTarget.y - sizeTarget.y)
			&& (pos.y + size.y) > (posTarget.y - sizeTarget.y))
		{
			bColision = true;

			if (bExtrude)
			{
				pos.y = posTarget.y - sizeTarget.y - size.y;
			}
		}
		if ((posOld.y - size.y) >= (posTarget.y + sizeTarget.y)
			&& (pos.y - size.y) < (posTarget.y + sizeTarget.y))
		{
			bColision = true;

			if (bExtrude)
			{
				pos.y = posTarget.y + sizeTarget.y + size.y;
			}
		}
	}
	if ((pos.y - size.y) < (posTarget.y + sizeTarget.y)
		&& (pos.y + size.y) > (posTarget.y - sizeTarget.y))
	{// モデル内にいる(Y軸)
		bool bColisionX = false;
		bool bColisionZ = false;

		if ((pos.z - size.z) < (posTarget.z + sizeTarget.z)
			&& (pos.z + size.z) > (posTarget.z - sizeTarget.z))
		{// モデル内にいる(Z軸)
			bColisionZ = true;
			if ((posOld.x + size.z) <= (posTarget.x - sizeTarget.x)
				&& (pos.x + size.z) > (posTarget.x - sizeTarget.x))
			{
				if (bExtrude)
				{
					pos.x = posTarget.x - sizeTarget.x - size.z;
				}
			}
			if ((posOld.x - size.z) >= (posTarget.x + sizeTarget.x)
				&& (pos.x - size.z) < (posTarget.x + sizeTarget.x))
			{
				if (bExtrude)
				{
					pos.x = posTarget.x + sizeTarget.x + size.z;
				}
			}
		}
		if ((pos.x - size.x) < (posTarget.x + sizeTarget.x)
			&& (pos.x + size.x) > (posTarget.x - sizeTarget.x))
		{// モデル内にいる(X軸)
			bColisionX = true;
			if ((posOld.z + size.z) <= (posTarget.z - sizeTarget.z)
				&& (pos.z + size.z) > (posTarget.z - sizeTarget.z))
			{
				if (bExtrude)
				{
					pos.z = posTarget.z - sizeTarget.z - size.z;
				}
			}
			if ((posOld.z - size.z) >= (posTarget.z + sizeTarget.z)
				&& (pos.z - size.z) < (posTarget.z + sizeTarget.z))
			{
				if (bExtrude)
				{
					pos.z = posTarget.z + sizeTarget.z + size.z;
				}
			}
		}

		if (bColisionX
			&& bColisionZ)
		{
			bColision = true;
		}
	}

	// 位置の設定
	SetPos(pos - m_colisonPos);
	return bColision;
}

//=============================================================================
// 球の判定
// Author : 唐﨑結斗
// 概要 : ターゲットとの球判定
//=============================================================================
bool CObject::ColisonSphere3D(CObject *target, bool bExtrude)
{
	// 変数宣言
	bool bCollision = false;

	// 自分の情報を取得する
	D3DXVECTOR3 pos = GetPos() + m_colisonPos;
	D3DXVECTOR3 size = m_colisonSize / 2.0f;

	// 目標の情報取得
	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
	D3DXVECTOR3 sizeTarget = target->GetColisonSize() / 2.0f;

	// 判定を行う距離を算出
	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y) + (size.z * size.z));
	fJudgeDistance += sqrtf((sizeTarget.x * sizeTarget.x) + (sizeTarget.y * sizeTarget.y) + (sizeTarget.z * sizeTarget.z));

	// お互いの位置の差を算出
	D3DXVECTOR3 distance = posTarget - pos;
	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));

	if (fDistance <= fJudgeDistance)
	{// 位置の差が判定を行う距離より短い場合
		bCollision = true;

		if (bExtrude)
		{
			// 角度の算出
			D3DXVECTOR3 rotDiff;
			rotDiff.y = atan2f(distance.x, distance.z);
			rotDiff.x = atan2f(sqrtf((distance.x * distance.x) + (distance.z * distance.z)), distance.y);
			rotDiff.z = 0.0f;

			// 位置の算出
			pos.z = posTarget.z - sinf(rotDiff.x) * cosf(rotDiff.y) * fJudgeDistance;
			pos.x = posTarget.x - sinf(rotDiff.x) * sinf(rotDiff.y) * fJudgeDistance;
			pos.y = posTarget.y - cosf(rotDiff.x) * fJudgeDistance;

			// 位置の設定
			SetPos(pos - m_colisonPos);
		}
	}

	return bCollision;
}
