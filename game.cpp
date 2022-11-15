//=============================================================================
//
// ゲームクラス(game.cpp)
// Author : 唐﨑結斗
// Author : 香月瑞輝
// 概要 : ゲームクラスの管理を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "game.h"
#include "calculation.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"

#include "application.h"
#include "camera_manager.h"
#include "renderer.h"
#include "object.h"
#include "object3D.h"
#include "object2D.h"

//モデル
#include "motion_model3D.h"
#include "player.h"
#include "Shark.h"
#include "obstacle.h"
#include "obstacle_manager.h"
#include "move.h"

#include "effect.h"
#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "model_obj.h"
#include "score.h"
#include "time.h"
#include "follow_model.h"
#include "sound.h"
#include "pause.h"
#include "result.h"

//--------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------
const D3DXVECTOR3 CGame::CAMERA_POSV = D3DXVECTOR3(0.0f, 300.0f, -1600.0f);		// 視点
const D3DXVECTOR3 CGame::CAMERA_POSR = D3DXVECTOR3(0.0f, 90.0f, 0.0f);			// 注視点

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPlayer **CGame::m_pPlayer = nullptr;					// プレイヤークラス
CEnemyShark *CGame::m_pEnemyShark = nullptr;			// Enemy
CScore *CGame::m_pScore = nullptr;						// スコアクラス
CTime *CGame::m_pTime = nullptr;						// タイムクラス
CMesh3D *CGame::m_pMesh3D;								// メッシュクラス
CFollowModel *CGame::m_pCameraTarget = nullptr;			// カメラターゲット
CObstacleManager *CGame::m_pObstacle = nullptr;			// 障害物のマネージャー
bool CGame::m_bGame = false;							// ゲームの状況

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame() : m_nCntFrame(0)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGame::Init()
{// マウスの取得
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// ゲームBGMの再生
	pSound->PlaySound(CSound::SOUND_LABEL_GAMEBGM);

	// 重力の値を設定
	CCalculation::SetGravity(4.0f);

	// カメラの位置変更
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetPosV(CAMERA_POSV);
	pCamera->SetPosR(CAMERA_POSR);
	pCamera->SetViewing(20.0f);

	// カメラターゲットの設定
	m_pCameraTarget = CFollowModel::Create();
	m_pCameraTarget->SetPos(CAMERA_POSR);
	pCamera->SetFollowTarget(m_pCameraTarget, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f);
	pCamera->SetPosV(CAMERA_POSV);
	pCamera->SetLockPosV(true);

	// 地面の設定
	m_pMesh3D = CMesh3D::Create();
	m_pMesh3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	m_pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	m_pMesh3D->SetSplitTex(true);
	m_pMesh3D->SetScrollTex(D3DXVECTOR2(0.0f, 0.01f), true);
	m_pMesh3D->LoadTex(0);

	// メッシュの生成
	CMesh3D *pMesh3D = CMesh3D::Create();
	pMesh3D->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
	pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
	pMesh3D->SetSplitTex(false);
	pMesh3D->SetWave(7.0f, 10.0f);
	pMesh3D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	pMesh3D->LoadTex(1);

	// スカイボックスの設定
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(5000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(2);

	// プレイヤーの設定
	int nMaxPlayer = CApplication::GetPersonCount();
	m_pPlayer = new CPlayer*[nMaxPlayer];
	assert(m_pPlayer != nullptr);

	float posX = 500.0f / (nMaxPlayer + 1);

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// プレイヤーの生成
		m_pPlayer[nCntPlayer] = CPlayer::Create();
		m_pPlayer[nCntPlayer]->SetMotion("data/MOTION/motion.txt");
		m_pPlayer[nCntPlayer]->SetPos(D3DXVECTOR3(-250.0f + posX + (posX * nCntPlayer), 0.0f, -300.0f));
		m_pPlayer[nCntPlayer]->SetNum(nCntPlayer);

		// 移動情報の設定
		CMove *pMove = m_pPlayer[nCntPlayer]->GetMove();
		pMove->SetMoving(1.0f, 8.5f, 0.5f, 0.1f);
	}

	// 障害物
	m_pObstacle = CObstacleManager::Create();
	m_pObstacle->SetGame(true);

	// タイムの設定
	m_pTime = CTime::Create();

	if (nMaxPlayer > 1)
	{// マルチプレイ
		m_pTime->SetTime(60);
		m_pTime->SetPos(D3DXVECTOR3(640.0f, 75.0f, 0.0f));
		m_pTime->SetSize(D3DXVECTOR3(200.0f, 300.0f, 0.0f));
	}
	else
	{// シングルプレイ
		m_pTime->SetTime(0);
		m_pTime->SetPos(D3DXVECTOR3(640.0f, 50.0f, 0.0f));
	}
	
	//サメ設定
	m_pEnemyShark = CEnemyShark::Create();
	m_pEnemyShark->SetMotion("data/MOTION/motionShark.txt", 2);
	m_pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
	m_pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));

	{// チュートリアル
		m_pTutorial = CObject2D::Create();
		m_pTutorial->SetPos(D3DXVECTOR3(200.0f, 100.0f, 0.0f));
		m_pTutorial->SetSize(D3DXVECTOR3(400.0f, 200.0f, 0.0f));
		int numJoy = CApplication::GetJoy()->GetUseJoyPad();

		if (numJoy >= 1)
		{// コントローラーが繋いである
			m_pTutorial->LoadTex(31);
		}
		else
		{// コントローラーが繋いでない
			m_pTutorial->LoadTex(32);
		}
	}

	{// 目的
		CObject2D* pObj = CObject2D::Create();
		pObj->SetPos(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH - 200.0f, 100.0f, 0.0f));
		pObj->SetSize(D3DXVECTOR3(250.0f, 100.0f, 0.0f));
		pObj->LoadTex(30);
	}

	// カウントダウンの設定
	CTime* pCountDown = CTime::Create();
	pCountDown->SetCountDown();

	// モデルの設置
	CModelObj::LoadFile("data/FILE/setModel.txt");

	// 死亡の初期化
	CResult::InitDead();

	m_bGame = true;

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CGame::Uninit()
{// マウスの取得
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// サウンド終了
	pSound->StopSound();

	if (m_pPlayer != nullptr)
	{
		int nMaxPlayer = CApplication::GetPersonCount();

		for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
		{// プレイヤーの生成
			if (m_pPlayer[nCntPlayer] != nullptr
				|| !m_pPlayer[nCntPlayer]->GetDead())
			{
				m_pPlayer[nCntPlayer]->Uninit();
			}	
		}

		delete[] m_pPlayer;
		m_pPlayer = nullptr;
	}

	// スコアの解放
	Release();

	// カメラの位置変更
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(nullptr, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f);

	// エフェクトの終了
	CEffect::ReleaseAll();
}

//=============================================================================
// 更新
//=============================================================================
void CGame::Update()
{
	// エフェクトの更新
	CEffect::UpdateAll();

	// オブジェクトの変更
	ObjChange();

	// 死亡判定
	Dead();

	if (!m_bGame)
	{// ゲーム中ではない
		m_nCntFrame++;

		if (m_nCntFrame >= 120)
		{// 一定時間後
			CApplication::SetNextMode(CApplication::MODE_RESULT);
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void CGame::Draw()
{
}

//=============================================================================
// オブジェクトの変更
//=============================================================================
void CGame::ObjChange()
{
	if (CApplication::GetJoy()->GetUseJoyPad() >= 1)
	{// コントローラーが繋いである
		m_pTutorial->LoadTex(31);
	}
	else
	{// コントローラーが繋いでない
		m_pTutorial->LoadTex(32);
	}
}

//=============================================================================
// 死亡判定
//=============================================================================
void CGame::Dead()
{
	int numPlayer = CApplication::GetPersonCount();
	bool dead = true;

	for (int i = 0; i < numPlayer; i++)
	{
		if (!m_pPlayer[i]->GetDead())
		{// 死んでいない
			dead = false;
			break;
		}
	}

	if (dead)
	{// 全員死んだ
		m_bGame = false;

		// 時間を止める
		m_pTime->StopTime(true);

		// ポーズできなくする
		CApplication::GetPause()->SetSelect(false);
	}
}
