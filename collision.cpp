//=============================================================================
//
// 当たり判定クラス(collision.cpp)
// Author : 唐﨑結斗
// 概要 : 当たり判定生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "collision.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CCollision::CCollision() : m_pParent(nullptr),
m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_size(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CCollision::~CCollision()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CCollision::Init()
{
	// 変数の初期化
	m_pParent = nullptr;							// 親
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 過去位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 大きさ

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CCollision::Uninit()
{
	// オブジェクト3Dの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CCollision::Update()
{
	
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CCollision::Draw()
{

}

////=============================================================================
//// 範囲内の判定
//// Author : 唐﨑結斗
//// 概要 : ターゲットが範囲内に入ったかの判定
////=============================================================================
//bool CCollision::ColisonRange2D(CObject * target)
//{// 自分の情報を取得する
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	D3DXVECTOR3 size = GetSize();
//
//	// 目標の情報取得
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	D3DXVECTOR3 sizeTarget = target->GetSize();
//
//	if (pos.x - size.x >= posTarget.x - sizeTarget.x
//		&& pos.x + size.x <= posTarget.x + sizeTarget.x
//		&& pos.y - size.y >= posTarget.y - sizeTarget.y
//		&& pos.y + size.y <= posTarget.y + sizeTarget.y)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
////=============================================================================
//// 矩形の判定
//// Author : 唐﨑結斗
//// 概要 : ターゲットとの矩形判定
////=============================================================================
//bool CCollision::ColisonRectangle2D(CObject *target, bool bExtrude)
//{// 返り値用の変数
//	bool bColision = false;
//
//	// 自分の情報を取得する
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	D3DXVECTOR3 posOld = GetPosOld();
//	D3DXVECTOR3 size = GetSize() / 2.0f;
//
//	// 目標の情報取得
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;
//
//	if (pos.y - size.y  < posTarget.y + sizeTarget.y
//		&& pos.y + size.y  > posTarget.y - sizeTarget.y)
//	{// モデル内にいる(Y軸)
//		if (posOld.x + size.y <= posTarget.x - sizeTarget.x
//			&& pos.x + size.y  > posTarget.x - sizeTarget.x)
//		{
//			if (bExtrude)
//			{// 押し出しを使用する
//				pos.x = posTarget.x - sizeTarget.x - size.y;
//			}
//
//			bColision = true;
//		}
//		if (posOld.x - size.y >= posTarget.x + sizeTarget.x
//			&& pos.x - size.y  < posTarget.x + sizeTarget.x)
//		{
//			if (bExtrude)
//			{// 押し出しを使用する
//				pos.x = posTarget.x + sizeTarget.x + size.y;
//			}
//
//			bColision = true;
//		}
//	}
//	if (pos.x - size.x  < posTarget.x + sizeTarget.x
//		&& pos.x + size.x  > posTarget.x - sizeTarget.x)
//	{// モデル内にいる(X軸)
//		if (posOld.y + size.y <= posTarget.y - sizeTarget.y
//			&& pos.y + size.y  > posTarget.y - sizeTarget.y)
//		{
//			if (bExtrude)
//			{// 押し出しを使用する
//				pos.y = posTarget.y - sizeTarget.y - size.y;
//			}
//
//			bColision = true;
//		}
//		if (posOld.y - size.y >= posTarget.y + sizeTarget.y
//			&& pos.y - size.y  < posTarget.y + sizeTarget.y)
//		{
//			if (bExtrude)
//			{// 押し出しを使用する
//				pos.y = posTarget.y + sizeTarget.y + size.y;
//			}
//
//			bColision = true;
//		}
//	}
//
//	// 位置の設定
//	SetPos(pos);
//	return bColision;
//}
//
////=============================================================================
//// 円の判定
//// Author : 唐﨑結斗
//// 概要 : ターゲットとの円判定
////=============================================================================
//bool CCollision::ColisonCircle2D(CObject * target, bool bExtrude)
//{
//	// 変数宣言
//	bool bCollision = false;
//
//	// 自分の情報を取得する
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	D3DXVECTOR3 size = GetSize() / 2.0f;
//
//	// 目標の情報取得
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;
//
//	// 判定を行う距離を算出
//	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y));
//	fJudgeDistance += sqrtf((sizeTarget.x * sizeTarget.x) + (sizeTarget.y * sizeTarget.y));
//
//	// お互いの位置の差を算出
//	D3DXVECTOR3 distance = posTarget - pos;
//	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));
//
//	if (fDistance <= fJudgeDistance)
//	{// 位置の差が判定を行う距離より短い場合
//		bCollision = true;
//
//		if (bExtrude)
//		{
//			float fRotDif = atan2f(distance.x, distance.y);
//			pos.x = posTarget.x - (sinf(fRotDif) * fJudgeDistance);
//			pos.y = posTarget.y - (cosf(fRotDif) * fJudgeDistance);
//		}
//	}
//
//	// 位置の設定
//	SetPos(pos);
//	return bCollision;
//}
//
////=============================================================================
//// 矩形の判定
//// Author : 唐﨑結斗
//// 概要 : ターゲットとの矩形判定
////=============================================================================
//bool CCollision::ColisonRectangle3D(CObject * target, bool bExtrude)
//{// 返り値用の変数
//	bool bColision = false;
//
//	// 自分の情報を取得する
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	D3DXVECTOR3 posOld = GetPosOld();
//	D3DXVECTOR3 size = GetSize() / 2.0f;
//
//	// 目標の情報取得
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;
//
//	if ((pos.z - size.z / 2) < (posTarget.z + sizeTarget.z)
//		&& (pos.z + size.z / 2) > (posTarget.z - sizeTarget.z)
//		&& (pos.x - size.x / 2) < (posTarget.x + sizeTarget.x)
//		&& (pos.x + size.x / 2) > (posTarget.x - sizeTarget.x))
//	{// モデル内にいる(XZ軸)
//		if ((posOld.y + size.y) <= (posTarget.y - sizeTarget.y)
//			&& (pos.y + size.y) >(posTarget.y - sizeTarget.y))
//		{
//			bColision = true;
//
//			if (bExtrude)
//			{
//				pos.y = posTarget.y - sizeTarget.y - size.y;
//			}
//		}
//		if ((posOld.y) >= (posTarget.y + sizeTarget.y)
//			&& (pos.y) < (posTarget.y + sizeTarget.y))
//		{
//			bColision = true;
//
//			if (bExtrude)
//			{
//				pos.y = posTarget.y + sizeTarget.y;
//			}
//		}
//	}
//	if ((pos.y) < (posTarget.y + sizeTarget.y)
//		&& (pos.y + size.y) > (posTarget.y - sizeTarget.y))
//	{// モデル内にいる(Y軸)
//		if ((pos.z - size.z / 2) < (posTarget.z + sizeTarget.z)
//			&& (pos.z + size.z / 2) > (posTarget.z - sizeTarget.z))
//		{// モデル内にいる(Z軸)
//			if ((posOld.x + size.z / 2) <= (posTarget.x - sizeTarget.x)
//				&& (pos.x + size.z / 2) > (posTarget.x - sizeTarget.x))
//			{
//				bColision = true;
//
//				if (bExtrude)
//				{
//					pos.x = posTarget.x - sizeTarget.x - size.z / 2;
//				}
//			}
//			if ((posOld.x - size.z / 2) >= (posTarget.x + sizeTarget.x)
//				&& (pos.x - size.z / 2) < (posTarget.x + sizeTarget.x))
//			{
//				bColision = true;
//
//				if (bExtrude)
//				{
//					pos.x = posTarget.x + sizeTarget.x + size.z / 2;
//				}
//			}
//		}
//		if ((pos.x - size.x / 2) < (posTarget.x + sizeTarget.x)
//			&& (pos.x + size.x / 2) > (posTarget.x - sizeTarget.x))
//		{// モデル内にいる(X軸)
//			if ((posOld.z + size.z / 2) <= (posTarget.z - sizeTarget.z)
//				&& (pos.z + size.z / 2) > (posTarget.z - sizeTarget.z))
//			{
//				bColision = true;
//
//				if (bExtrude)
//				{
//					pos.z = posTarget.z - sizeTarget.z - size.z / 2;
//				}
//			}
//			if ((posOld.z - size.z / 2) >= (posTarget.z + sizeTarget.z)
//				&& (pos.z - size.z / 2) < (posTarget.z + sizeTarget.z))
//			{
//				bColision = true;
//
//				if (bExtrude)
//				{
//					pos.z = posTarget.z + sizeTarget.z + size.z / 2;
//				}
//			}
//		}
//	}
//
//	// 位置の設定
//	SetPos(pos);
//	return bColision;
//}
//
////=============================================================================
//// 球の判定
//// Author : 唐﨑結斗
//// 概要 : ターゲットとの球判定
////=============================================================================
//bool CCollision::ColisonSphere3D(CObject *target, D3DXVECTOR3 size, D3DXVECTOR3 targetSize, bool bExtrude)
//{
//	// 変数宣言
//	bool bCollision = false;
//
//	// 自分の情報を取得する
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	size /= 2.0f;
//
//	// 目標の情報取得
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	targetSize /= 2.0f;
//
//	// 判定を行う距離を算出
//	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y) + (size.z * size.z));
//	fJudgeDistance += sqrtf((targetSize.x * targetSize.x) + (targetSize.y * targetSize.y) + (targetSize.z * targetSize.z));
//
//	// お互いの位置の差を算出
//	D3DXVECTOR3 distance = posTarget - pos;
//	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));
//
//	if (fDistance <= fJudgeDistance)
//	{// 位置の差が判定を行う距離より短い場合
//		bCollision = true;
//
//		if (bExtrude)
//		{
//			// 角度の算出
//			D3DXVECTOR3 rotDiff;
//			rotDiff.y = atan2f(distance.x, distance.z);
//			rotDiff.x = atan2f(sqrtf((distance.x * distance.x) + (distance.z * distance.z)), distance.y);
//			rotDiff.z = 0.0f;
//
//			// 位置の算出
//			pos.z = posTarget.z - sinf(rotDiff.x) * cosf(rotDiff.y) * fJudgeDistance;
//			pos.x = posTarget.x - sinf(rotDiff.x) * sinf(rotDiff.y) * fJudgeDistance;
//			pos.y = posTarget.y - cosf(rotDiff.x) * fJudgeDistance;
//
//			// 位置の設定
//			SetPos(pos);
//		}
//	}
//
//	return bCollision;
//}

//=============================================================================
// 親のセッター
// Author : 唐﨑結斗
// 概要 : 親のメンバ変数に引数を代入
//=============================================================================
void CCollision::SetParent(CObject * pParent)
{
	 // 親オブジェクトの設定
	m_pParent = pParent;
}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CCollision::SetPos(const D3DXVECTOR3 &pos)
{
	// 位置の設定
	m_pos = pos;
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CCollision::SetRot(const D3DXVECTOR3 &rot)
{
	// 位置の設定
	m_rot = rot;
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CCollision::SetSize(const D3DXVECTOR3 & size)
{
	// 大きさの設定
	m_size = size;
}
