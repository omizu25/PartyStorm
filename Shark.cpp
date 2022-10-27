//=============================================================================
//
// エネミークラス(Enemy.h)
// Author : 磯江寿希亜
// 概要 : プレイヤーに襲い掛かる
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "Shark.h"
#include "game.h"
#include "mesh.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"
#include "camera.h"
#include "keyboard.h"
#include "calculation.h"
#include "move.h"
#include "shadow.h"
#include "bullet3D.h"
#include "debug_proc.h"

CEnemyShark * CEnemyShark::Create()
{
	// オブジェクトインスタンス
	CEnemyShark *pEnemyShark = nullptr;

	// メモリの解放
	pEnemyShark = new CEnemyShark;

	// メモリの確保ができなかった
	assert(pEnemyShark != nullptr);

	// 数値の初期化
	pEnemyShark->Init();

	// インスタンスを返す
	return pEnemyShark;
}

CEnemyShark::CEnemyShark() : m_pMove(nullptr),m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),m_fSpeed(0.0f),
m_nNumMotion(0)
{

}

CEnemyShark::~CEnemyShark()
{
}

HRESULT CEnemyShark::Init()
{
	// 初期化
	CMotionModel3D::Init();

	// 移動クラスのメモリ確保
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// 影の設定
	m_pShadow = CShadow::Create(this);
	m_pShadow->SetPos(GetPos());
	m_pShadow->SetSize(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

	return S_OK;
}

void CEnemyShark::Uninit()
{
	// 影の終了
	m_pShadow->Uninit();

	if (m_pMove != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_pMove;
		m_pMove = nullptr;
	}

	// 終了
	CMotionModel3D::Uninit();

}

void CEnemyShark::Update()
{
	// キーボードの取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// 位置の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// 攻撃
	if (pKeyboard->GetTrigger(DIK_RETURN)
		&& pMotion != nullptr)
	{
		pMotion->SetNumMotion(ATTACK_ACTION);
		m_EAction = ATTACK_ACTION;
	}

	// 移動
	pos += Move();

	pos.y -= CCalculation::Gravity();

	// 回転
	Rotate();

	// ニュートラルモーションの設定
	if (pMotion != nullptr
		&& !pMotion->GetMotion())
	{
		pMotion->SetNumMotion(NEUTRAL_ACTION);
		m_EAction = NEUTRAL_ACTION;
	}

	// 位置の設定
	SetPos(pos);

	// メッシュの当たり判定
	CMesh3D *pMesh = CGame::GetMesh();
	pMesh->Collison(this);

	// 位置の取得
	pos = GetPos();

	// 更新
	CMotionModel3D::Update();
}

void CEnemyShark::Draw()
{
	// 描画
	CMotionModel3D::Draw();
}

D3DXVECTOR3 CEnemyShark::Move()
{
	// 変数宣言
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// キーボードの取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	if (pKeyboard->GetPress(DIK_W)
		|| pKeyboard->GetPress(DIK_A)
		|| pKeyboard->GetPress(DIK_D)
		|| pKeyboard->GetPress(DIK_S))
	{// 移動キーが押された
		if (pKeyboard->GetPress(DIK_W))
		{// [W]キーが押された時
			if (pKeyboard->GetPress(DIK_A))
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{// 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S))
		{// [S]キーが押された時
			if (pKeyboard->GetPress(DIK_A))
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.75f;
			}
			else
			{// 移動方向の更新q
				m_rotDest.y = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A))
		{// [A]キーが押された時
		 // 移動方向の更新
			m_rotDest.y = D3DX_PI * -0.5f;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{// [D]キーが押された時
		 // 移動方向の更新
			m_rotDest.y = D3DX_PI * 0.5f;
		}

		// カメラ情報の取得
		CCamera *pCamera = CApplication::GetCamera();

		// 移動方向の算出
		m_rotDest.y += pCamera->GetRot().y;

		// 移動方向の正規化
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		// 移動量の計算
		move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

		// 角度の正規化
		m_rotDest.y -= D3DX_PI;

		if (m_EAction == NEUTRAL_ACTION)
		{
			m_EAction = MOVE_ACTION;

			if (pMotion != nullptr)
			{
				pMotion->SetNumMotion(MOVE_ACTION);
			}
		}
	}

	// 移動情報の計算
	m_pMove->Moving(move);

	// 移動情報の取得
	D3DXVECTOR3 moveing = m_pMove->GetMove();

	if (sqrtf((moveing.x * moveing.x) + (moveing.z * moveing.z)) <= 0.0f
		&& pMotion != nullptr
		&& m_EAction == MOVE_ACTION)
	{
		m_EAction = NEUTRAL_ACTION;
		pMotion->SetNumMotion(NEUTRAL_ACTION);
	}

	// デバック表示
	//CDebugProc::Print("Enemy 移動ベクトル : %.3f\n", m_pMove->GetMoveLength());

	// 向きの取得
	D3DXVECTOR3 rot = GetRot();

	// 目的の向きの補正
	if (m_rotDest.y - rot.y >= D3DX_PI)
	{// 移動方向の正規化
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - rot.y <= -D3DX_PI)
	{// 移動方向の正規化
		m_rotDest.y += D3DX_PI * 2;
	}

	return moveing;
}

void CEnemyShark::Rotate()
{
	// 向きの取得
	D3DXVECTOR3 rot = GetRot();

	// 向きの更新
	rot.y += (m_rotDest.y - rot.y) * 0.5f;

	// 向きの正規化
	rot.y = CCalculation::RotNormalization(rot.y);

	// 向きの設定
	SetRot(rot);

}
