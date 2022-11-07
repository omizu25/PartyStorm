//=============================================================================
//
// ゲームクラス(game.cpp)
// Author : 唐﨑結斗
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

//モデル
#include "player.h"
#include "Shark.h"
#include "obstacle.h"
#include "move.h"

#include "effect.h"
#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "model_obj.h"
#include "score.h"
#include "time.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPlayer **CGame::m_pPlayer = nullptr;					// プレイヤークラス
CEnemyShark *CGame::m_pEnemyShark = nullptr;			// Enemy
CScore *CGame::m_pScore = nullptr;						// スコアクラス
CTime *CGame::m_pTime = nullptr;						// タイムクラス
CMesh3D *CGame::m_pMesh3D;								// メッシュクラス
bool CGame::m_bGame = false;							// ゲームの状況

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CGame::CGame() : m_nCntFrame(0)
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CGame::Init()
{// マウスの取得
	CMouse *pMouse = CApplication::GetMouse();

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// ゲームBGMの再生
	pSound->PlaySound(CSound::SOUND_LABEL_GAMEBGM);

	// 重力の値を設定
	CCalculation::SetGravity(4.0f);

	// カメラの位置変更
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetPosV(D3DXVECTOR3(0.0f, 300.0f, -1600.0f));
	pCamera->SetPosR(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
	pCamera->SetViewing(20.0f);

	// タイムの設定
	CTime *pTime = CTime::Create();
	pTime->SetPos(D3DXVECTOR3(640.0f, 50.0f, 0.0f));
	pTime->SetTime(60);

	// 地面の設定
	m_pMesh3D = CMesh3D::Create();
	m_pMesh3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	m_pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	m_pMesh3D->SetSplitTex(true);
	m_pMesh3D->SetScrollTex(D3DXVECTOR2(0.0f, 0.01f), true);
	m_pMesh3D->LoadTex(2);

	// メッシュの生成
	CMesh3D *pMesh3D = CMesh3D::Create();
	pMesh3D->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
	pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
	pMesh3D->SetSplitTex(false);
	pMesh3D->SetWave(7.0f, 10.0f);
	pMesh3D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	pMesh3D->LoadTex(3);

	// スカイボックスの設定
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(5000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(4);

	// プレイヤーの設定
	int nMaxPlayer = CApplication::GetPersonCount();
	m_pPlayer = new CPlayer*[nMaxPlayer];
	assert(m_pPlayer != nullptr);

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// プレイヤーの生成
		m_pPlayer[nCntPlayer] = CPlayer::Create();
		m_pPlayer[nCntPlayer]->SetMotion("data/MOTION/motion.txt");
		m_pPlayer[nCntPlayer]->SetPos(D3DXVECTOR3(-100.0f + 100.0f * nCntPlayer, 0.0f, -300.0f));
		m_pPlayer[nCntPlayer]->SetNum(nCntPlayer);

		// 移動情報の設定
		CMove *pMove = m_pPlayer[nCntPlayer]->GetMove();
		pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);
	}

	//サメ設定
	m_pEnemyShark = CEnemyShark::Create();
	m_pEnemyShark->SetMotion("data/MOTION/motionShark.txt", 2);
	m_pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
	m_pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));

	// モデルの設置
	CModelObj::LoadFile("data/FILE/setModel.txt");

	//// スコアの生成
	//m_pScore = CScore::Create(10, false);
	//m_pScore->SetScore(0);
	//m_pScore->SetPos(D3DXVECTOR3(1280.0f, m_pScore->GetSize().y / 2.0f, 0.0f));

	// マウスカーソルを消す
	pMouse->SetShowCursor(false);

	m_bGame = true;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CGame::Uninit()
{// マウスの取得
	CMouse *pMouse = CApplication::GetMouse();

	// マウスカーソルを出す
	pMouse->SetShowCursor(true);

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

		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

	// スコアの解放
	Release();

	// エフェクトの終了
	CEffect::ReleaseAll();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CGame::Update()
{// キーボードの取得
	// 障害物
	CObstacle::Pop();

	// エフェクトの更新
	CEffect::UpdateAll();

	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	if (pKeyboard->GetTrigger(DIK_F3))
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}

	//コントローラー使用の確認
	CJoypad *pJoy = CApplication::GetJoy();
	if (pJoy->GetTrigger(CJoypad::JOYKEY_A, 1))
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		pJoy->Vibration(120, 50000, 0);
		pJoy->Vibration(120, 50000, 1);
	}

	int nMaxPlayer = CApplication::GetPersonCount();
	int nCntDead = 0;

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer]->GetDead())
		{
			nCntDead++;
		}
	}

	if (nCntDead >= nMaxPlayer)
	{// ゲームを終わる
		m_bGame = false;
	}

	if (!m_bGame)
	{
		m_nCntFrame++;

		if (m_nCntFrame % 120 == 0)
		{// リザルト画面へ
			CApplication::SetNextMode(CApplication::MODE_RESULT);
		}
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CGame::Draw()
{

}
