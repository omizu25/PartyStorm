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
#include "bullet3D.h"
#include "debug_proc.h"
#include "line.h"
#include "sound.h"
#include "follow_model.h"

//--------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------
const D3DXVECTOR2 CEnemyShark::MAX_VIB_RAND = D3DXVECTOR2(20.0f, 50.0f);		// 振動の幅
const float CEnemyShark::VIB_SPEED = 5.0f;										// 振動の速度
const float CEnemyShark::VIB_COEFFICIENT = 0.4f;								// 振動の減衰値

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
m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_fSpeed(0.0f),
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

	// 当たり判定の設定
	SetColisonPos(D3DXVECTOR3(0.0f, 250.0f, 300.0f));
	SetColisonSize(D3DXVECTOR3(500.0f, 500.0f, 2500.0f));

	// オブジェクトタイプの設定
	SetObjType(OBJTYPE_3DENEMY);

#ifdef _DEBUG
	// ライン情報
	m_pLine = new CLine*[12];

	for (int nCntLine = 0; nCntLine < 12; nCntLine++)
	{
		m_pLine[nCntLine] = CLine::Create();
	}

	// ラインの設定
	SetLine();
#endif // _DEBUG

	return S_OK;
}

void CEnemyShark::Uninit()
{
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
	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// ニュートラルモーションの設定
	if (pMotion != nullptr
		&& !pMotion->GetMotion())
	{
		m_EAction = ATTACK_ACTION;
		pMotion->SetNumMotion(m_EAction);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_BITING);

		if (CApplication::GetMode() != CApplication::MODE_GAME)
		{
			return;
		}

		CFollowModel *pCameraTarget = CGame::GetCameraTarget();

		if (pCameraTarget != nullptr)
		{// カメラターゲット情報
			D3DXVECTOR3 pos = pCameraTarget->GetPos();
			D3DXVECTOR3 posDest = D3DXVECTOR3(pos.x + MAX_VIB_RAND.x - (float)(rand() % (int)(MAX_VIB_RAND.x * 2.0f)),
				pos.y + MAX_VIB_RAND.y - (float)(rand() % (int)(MAX_VIB_RAND.y * 2.0f)), 0.0f);
			pCameraTarget->SetPos(posDest);
			pCameraTarget->SetFollow(CGame::CAMERA_POSR);
			pCameraTarget->SetSpeed(VIB_SPEED);
			pCameraTarget->SetCoefficient(VIB_COEFFICIENT);
		}
	}

	// 更新
	CMotionModel3D::Update();

#ifdef _DEBUG
	// ラインの更新
	SetLine();
#endif // _DEBUG
}

void CEnemyShark::Draw()
{
	// 描画
	CMotionModel3D::Draw();
}

//D3DXVECTOR3 CEnemyShark::Move()
//{
//	// 変数宣言
//	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	// キーボードの取得
//	CKeyboard *pKeyboard = CApplication::GetKeyboard();
//
//	// モーション情報の取得
//	CMotion *pMotion = CMotionModel3D::GetMotion();
//
//	//if (pKeyboard->GetPress(DIK_W)
//	//	|| pKeyboard->GetPress(DIK_A)
//	//	|| pKeyboard->GetPress(DIK_D)
//	//	|| pKeyboard->GetPress(DIK_S))
//	//{// 移動キーが押された
//	//	if (pKeyboard->GetPress(DIK_W))
//	//	{// [W]キーが押された時
//	//		if (pKeyboard->GetPress(DIK_A))
//	//		{// [A]キーが押された時
//	//		 // 移動方向の更新
//	//			m_rotDest.y = D3DX_PI * -0.25f;
//	//		}
//	//		else if (pKeyboard->GetPress(DIK_D))
//	//		{// [D]キーが押された時
//	//		 // 移動方向の更新
//	//			m_rotDest.y = D3DX_PI * 0.25f;
//	//		}
//	//		else
//	//		{// 移動方向の更新
//	//			m_rotDest.y = D3DX_PI * 0.0f;
//	//		}
//	//	}
//	//	else if (pKeyboard->GetPress(DIK_S))
//	//	{// [S]キーが押された時
//	//		if (pKeyboard->GetPress(DIK_A))
//	//		{// [A]キーが押された時
//	//		 // 移動方向の更新
//	//			m_rotDest.y = D3DX_PI * -0.75f;
//	//		}
//	//		else if (pKeyboard->GetPress(DIK_D))
//	//		{// [D]キーが押された時
//	//		 // 移動方向の更新
//	//			m_rotDest.y = D3DX_PI * 0.75f;
//	//		}
//	//		else
//	//		{// 移動方向の更新q
//	//			m_rotDest.y = D3DX_PI;
//	//		}
//	//	}
//	//	else if (pKeyboard->GetPress(DIK_A))
//	//	{// [A]キーが押された時
//	//	 // 移動方向の更新
//	//		m_rotDest.y = D3DX_PI * -0.5f;
//	//	}
//	//	else if (pKeyboard->GetPress(DIK_D))
//	//	{// [D]キーが押された時
//	//	 // 移動方向の更新
//	//		m_rotDest.y = D3DX_PI * 0.5f;
//	//	}
//
//	//	// カメラ情報の取得
//	//	CCamera *pCamera = CApplication::GetCamera();
//
//	//	// 移動方向の算出
//	//	m_rotDest.y += pCamera->GetRot().y;
//
//	//	// 移動方向の正規化
//	//	m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);
//
//	//	// 移動量の計算
//	//	move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));
//
//	//	// 角度の正規化
//	//	m_rotDest.y -= D3DX_PI;
//
//	//	if (m_EAction == NEUTRAL_ACTION)
//	//	{
//	//		m_EAction = MOVE_ACTION;
//
//	//		if (pMotion != nullptr)
//	//		{
//	//			pMotion->SetNumMotion(MOVE_ACTION);
//	//		}
//	//	}
//	//}
//
//	// 移動情報の計算
//	m_pMove->Moving(move);
//
//	// 移動情報の取得
//	D3DXVECTOR3 moveing = m_pMove->GetMove();
//
//	if (sqrtf((moveing.x * moveing.x) + (moveing.z * moveing.z)) <= 0.0f
//		&& pMotion != nullptr
//		&& m_EAction == MOVE_ACTION)
//	{
//		m_EAction = NEUTRAL_ACTION;
//		pMotion->SetNumMotion(NEUTRAL_ACTION);
//	}
//
//	// 向きの取得
//	D3DXVECTOR3 rot = GetRot();
//
//	// 目的の向きの補正
//	if (m_rotDest.y - rot.y >= D3DX_PI)
//	{// 移動方向の正規化
//		m_rotDest.y -= D3DX_PI * 2;
//	}
//	else if (m_rotDest.y - rot.y <= -D3DX_PI)
//	{// 移動方向の正規化
//		m_rotDest.y += D3DX_PI * 2;
//	}
//
//	return moveing;
//}
//
//void CEnemyShark::Rotate()
//{
//	// 向きの取得
//	D3DXVECTOR3 rot = GetRot();
//
//	// 向きの更新
//	rot.y += (m_rotDest.y - rot.y) * 0.5f;
//
//	// 向きの正規化
//	rot.y = CCalculation::RotNormalization(rot.y);
//
//	// 向きの設定
//	SetRot(rot);
//}

#ifdef _DEBUG
//=============================================================================
// ラインの設置
// Author : 唐﨑結斗
// 概要 : ラインを矩形状に設置
//=============================================================================
void CEnemyShark::SetLine()
{
	// 変数宣言
	const D3DXVECTOR3 pos = GetPos() + GetColisonPos();
	const D3DXVECTOR3 rot = GetRot();
	const D3DXVECTOR3 size = GetColisonSize() / 2.0f;
	const D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// ラインの設定
	m_pLine[0]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, size.z), D3DXVECTOR3(size.x, -size.y, size.z), col);
	m_pLine[1]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, -size.z), D3DXVECTOR3(-size.x, -size.y, size.z), col);
	m_pLine[2]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, -size.z), D3DXVECTOR3(size.x, -size.y, -size.z), col);
	m_pLine[3]->SetLine(pos, rot, D3DXVECTOR3(size.x, -size.y, -size.z), D3DXVECTOR3(size.x, -size.y, size.z), col);
	m_pLine[4]->SetLine(pos, rot, D3DXVECTOR3(-size.x, size.y, size.z), D3DXVECTOR3(size.x, size.y, size.z), col);
	m_pLine[5]->SetLine(pos, rot, D3DXVECTOR3(-size.x, size.y, -size.z), D3DXVECTOR3(-size.x, size.y, size.z), col);
	m_pLine[6]->SetLine(pos, rot, D3DXVECTOR3(-size.x, size.y, -size.z), D3DXVECTOR3(size.x, size.y, -size.z), col);
	m_pLine[7]->SetLine(pos, rot, D3DXVECTOR3(size.x, size.y, -size.z), D3DXVECTOR3(size.x, size.y, size.z), col);
	m_pLine[8]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, size.z), D3DXVECTOR3(-size.x, size.y, size.z), col);
	m_pLine[9]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, -size.z), D3DXVECTOR3(-size.x, size.y, -size.z), col);
	m_pLine[10]->SetLine(pos, rot, D3DXVECTOR3(size.x, -size.y, -size.z), D3DXVECTOR3(size.x, size.y, -size.z), col);
	m_pLine[11]->SetLine(pos, rot, D3DXVECTOR3(size.x, -size.y, size.z), D3DXVECTOR3(size.x, size.y, size.z), col);
}
#endif // DEBUG