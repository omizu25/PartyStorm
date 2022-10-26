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

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CObject *CObject::m_pTop[MAX_LEVEL] = { nullptr };				// 先頭オブジェクトへのポインタ
CObject *CObject::m_pCurrent[MAX_LEVEL] = { nullptr };			// 現在の(一番後ろ)オブジェクトへのポインタ
int CObject::m_nMaxObject = 0;									// 使用数
int CObject::m_nPriorityMaxObj[MAX_LEVEL] = { 0 };				// プライオリティごとのオブジェクト数

//=============================================================================
// インスタンスの解放
// Author : 唐﨑結斗
// 概要 : すべてのインスタンスを解放する処理
//=============================================================================
void CObject::ReleaseAll(bool bPermanent)
{
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
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
						&& pObject->m_objectType != CObject::OBJTYPE_FADE)
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

	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CObject *pObjectNext = pObject->m_pNext;

				if (!pObject->m_bDeath)
				{// オブジェクトの更新
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
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// 変数宣言
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CObject *pObjectNext = pObject->m_pNext;

				if (!pObject->m_bDeath)
				{// レンダラーのゲット
					//CRenderer *pRenderer = CApplication::GetRenderer();

					// ステンシルの設定
					//pRenderer->SetStencil(1, D3DCMP_EQUAL);

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
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
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
CObject::CObject(int nPriority/* = PRIORITY_LEVEL0*/) : m_pPrev(nullptr),		// 前のオブジェクトへのポインタ
m_pNext(nullptr),																// 次のオブジェクトへのポインタ
m_nLevPriority(nPriority),														// プライオリティのレベル
m_bDeath(false)																	// 死亡フラグ
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
