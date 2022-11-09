//=============================================================================
//
// 追従モデルクラス(follow_model.cpp)
// Author : 唐﨑結斗
// 概要 : 追従モデル生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <stdio.h>

#include "follow_model.h"
#include "calculation.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 追従モデルを生成する
//=============================================================================
CFollowModel * CFollowModel::Create()
{
	// オブジェクトインスタンス
	CFollowModel *pFollowModel = nullptr;

	// メモリの解放
	pFollowModel = new CFollowModel;

	// メモリの確保ができなかった
	assert(pFollowModel != nullptr);

	// 数値の初期化
	pFollowModel->Init();

	// インスタンスを返す
	return pFollowModel;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CFollowModel::CFollowModel() : m_posDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),		// 目的の位置
m_moveVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),									// 移動方向
m_fSpeed(0.0f),																// 移動速度
m_fCoefficient(0.0f),														// 減衰係数
m_bFollow(false)															// 追従判定
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CFollowModel::~CFollowModel()
{

}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CFollowModel::Update()
{
	// 更新
	CModelObj::Update();

	// 追従
	Follow();
}

//=============================================================================
// 目的の位置の設定
// Author : 唐﨑結斗
// 概要 : 目的の位置の設定し、追従開始
//=============================================================================
void CFollowModel::SetFollow(const D3DXVECTOR3 posDest)
{
	// 判定(ture)
	m_bFollow = true;

	// 目的の位置の設定
	m_posDest = posDest;
}

//=============================================================================
// 追従
// Author : 唐﨑結斗
// 概要 : 追従
//=============================================================================
void CFollowModel::Follow()
{
	if (m_bFollow)
	{// 追従
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 moveVecDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 distance = m_posDest - pos;

		moveVecDest.y = atan2f(distance.x, distance.z);
		moveVecDest.x = atan2f(sqrtf((distance.x * distance.x) + (distance.z * distance.z)), distance.y);
		moveVecDest.z = m_moveVec.z;

		// 移動方向の設定
		D3DXVECTOR3 add = (moveVecDest - m_moveVec);

		// 正規化
		add.x = CCalculation::RotNormalization(add.x);
		add.y = CCalculation::RotNormalization(add.y);
		add.z = CCalculation::RotNormalization(add.z);

		// 移動方向の算出
		m_moveVec += add * m_fCoefficient;

		// 正規化
		m_moveVec.x = CCalculation::RotNormalization(m_moveVec.x);
		m_moveVec.y = CCalculation::RotNormalization(m_moveVec.y);
		m_moveVec.z = CCalculation::RotNormalization(m_moveVec.z);

		// 移動量の設定
		move.z = sinf(m_moveVec.x) * cosf(m_moveVec.y) * m_fSpeed;
		move.x = sinf(m_moveVec.x) * sinf(m_moveVec.y) * m_fSpeed;
		move.y = cosf(m_moveVec.x) * m_fSpeed;

		SetPos(pos + move);
	}
}
