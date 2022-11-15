//=============================================================================
//
// タイトルクラス(title.cpp)
// Author : 唐﨑結斗
// Author : 香月瑞輝
// 概要 : タイトルクラスの管理を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "title.h"
#include "application.h"
#include "keyboard.h"
#include "joypad.h"
#include "sound.h"
#include "object2D.h"
#include "camera_manager.h"
#include "renderer.h"
#include "Shark.h"
#include "sphere.h"
#include "model_obj.h"
#include "obstacle_manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle() :
	m_pTitleLogo(nullptr),
	m_pPressAny(nullptr),
	m_pMode(nullptr),
	m_pNum(nullptr),
	m_nCntFrame(0),
	m_nNumPlayer(0),
	m_fCycle(0.0f),
	m_fAddSize(0.0f),
	m_bPressEnter(false)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CTitle::Init()
{
	// タイトルBGMの再生
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_TITELBGM);

	int numPlayer = CApplication::GetJoy()->GetUseJoyPad();

	if (numPlayer <= 0)
	{// パッドの接続が一つも無い
		numPlayer = 1;
	}

	CApplication::SetPersonCount(numPlayer);

	{// タイトルロゴ
		m_pTitleLogo = CObject2D::Create();
		m_pTitleLogo->SetPos(D3DXVECTOR3(640.0f, 230.0f, 0.0f));
		m_pTitleLogo->SetSize(D3DXVECTOR3(800.0f, 300.0f, 0.0f));
		m_pTitleLogo->LoadTex(18);
	}

	{// プレスオブジェクト
		m_pPressAny = CObject2D::Create();
		m_pPressAny->SetPos(D3DXVECTOR3(640.0f, 550.0f, 0.0f));
		m_pPressAny->SetSize(D3DXVECTOR3(450.0f, 180.0f, 0.0f));

		if (CApplication::GetJoy()->GetUseJoyPad() <= 0)
		{// パッドの接続が一つも無い
			m_pPressAny->LoadTex(34);
			m_pPressAny->SetSize(D3DXVECTOR3(550.0f, 180.0f, 0.0f));
		}
		else
		{
			m_pPressAny->LoadTex(33);
			m_pPressAny->SetSize(D3DXVECTOR3(600.0f, 180.0f, 0.0f));
		}
	}

	if (numPlayer <= 1)
	{// シングルプレイ
		m_pMode = CObject2D::Create();
		m_pMode->SetPos(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH - 100.0f, 50.0f, 0.0f));
		m_pMode->SetSize(D3DXVECTOR3(200.0f, 100.0f, 0.0f));
		m_pMode->LoadTex(35);

		m_pNum = nullptr;
	}
	else
	{// マルチプレイ
		m_pMode = CObject2D::Create();
		m_pMode->SetPos(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH - 100.0f, 50.0f, 0.0f));
		m_pMode->SetSize(D3DXVECTOR3(200.0f, 100.0f, 0.0f));
		m_pMode->LoadTex(36);

		m_pNum = CObject2D::Create();
		m_pNum->SetPos(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH - 50.0f, 150.0f, 0.0f));
		m_pNum->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));

		switch (numPlayer)
		{
		case 2:
			m_pNum->LoadTex(37);
			break;

		case 3:
			m_pNum->LoadTex(38);
			break;

		case 4:
			m_pNum->LoadTex(39);
			break;

		default:
			assert(false);
			break;
		}
	}

	{// カメラの位置変更
		CCamera *pCamera = CApplication::GetCamera();
		pCamera->SetPosV(D3DXVECTOR3(0.0f, 300.0f, -1600.0f));
		pCamera->SetPosR(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
		pCamera->SetViewing(20.0f);
	}

	{// 地面の設定
		CMesh3D *pMesh = CMesh3D::Create();
		pMesh->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMesh->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMesh->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
		pMesh->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
		pMesh->SetSplitTex(true);
		pMesh->SetScrollTex(D3DXVECTOR2(0.0f, 0.01f), true);
		pMesh->LoadTex(0);
	}

	{// 地面の設定
		CMesh3D *pMesh = CMesh3D::Create();
		pMesh->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
		pMesh->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMesh->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
		pMesh->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
		pMesh->SetSplitTex(false);
		pMesh->SetWave(7.0f, 10.0f);
		pMesh->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		pMesh->LoadTex(1);
	}

	{// スカイボックスの設定
		CSphere *pSphere = CSphere::Create();
		pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
		pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
		pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
		pSphere->SetRadius(5000.0f);
		pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
		pSphere->LoadTex(2);
	}

	{// サメ設定
		CEnemyShark *pEnemyShark = CEnemyShark::Create();
		pEnemyShark->SetMotion("data/MOTION/motionShark.txt");
		pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
		pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));
	}

	// モデルの設置
	CModelObj::LoadFile("data/FILE/setModel.txt");

	// 障害物の背景
	CObstacleManager::Create();

	m_fCycle = 0.01f;
	m_bPressEnter = false;

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CTitle::Uninit()
{
	// サウンド終了
	CApplication::GetSound()->StopSound();

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
//=============================================================================
void CTitle::Update()
{
	m_nCntFrame++;

	// パッドの接続数の取得
	int numPlayer = CApplication::GetJoy()->GetUseJoyPad();

	if (numPlayer <= 0)
	{// 接続されていない
		numPlayer = 1;
	}
	
	// プレイヤー数の設定
	CApplication::SetPersonCount(numPlayer);

	// オブジェクトの変更
	ObjChange();

	// 点滅
	FlashObj();

	if (!m_bPressEnter)
	{// まだエンターが押されていない
		// 入力
		Input();
	}
	else
	{
		if (m_nCntFrame >= 60)
		{// 一定時間後
			CApplication::SetNextMode(CApplication::MODE_GAME);
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void CTitle::Draw()
{
}

//=============================================================================
// オブジェクトの変更
//=============================================================================
void CTitle::ObjChange()
{
	{// タイトルロゴのサイズ変更
		m_fAddSize += 0.05f;

		D3DXVECTOR3 sizeTitleLogo = m_pTitleLogo->GetSize();
		sizeTitleLogo.x += sinf(m_fAddSize) * 3.0f;
		sizeTitleLogo.y += sinf(m_fAddSize) * 2.0f;

		m_pTitleLogo->SetSize(sizeTitleLogo);
	}

	if (CApplication::GetJoy()->GetUseJoyPad() <= 0)
	{// パッドが一つも接続されていない
		m_pPressAny->LoadTex(34);
		m_pPressAny->SetSize(D3DXVECTOR3(550.0f, 180.0f, 0.0f));
	}
	else
	{
		m_pPressAny->LoadTex(33);
		m_pPressAny->SetSize(D3DXVECTOR3(600.0f, 180.0f, 0.0f));
	}

	int numPlayer = CApplication::GetPersonCount();

	if (numPlayer <= 1)
	{// シングルプレイ
		m_pMode->LoadTex(35);

		if (m_pNum != nullptr)
		{// nullチェック
			m_pNum->Uninit();
			m_pNum = nullptr;
		}
	}
	else
	{// マルチプレイ
		m_pMode->LoadTex(36);

		if (m_pNum == nullptr)
		{// nullチェック
			m_pNum = CObject2D::Create();
			m_pNum->SetPos(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH - 50.0f, 150.0f, 0.0f));
			m_pNum->SetSize(D3DXVECTOR3(100.0f, 100.0f, 0.0f));
		}

		switch (numPlayer)
		{
		case 2:
			m_pNum->LoadTex(37);
			break;

		case 3:
			m_pNum->LoadTex(38);
			break;

		case 4:
			m_pNum->LoadTex(39);
			break;

		default:
			assert(false);
			break;
		}
	}
}

//=============================================================================
// 入力
//=============================================================================
void CTitle::Input()
{
	// ジョイパッドの情報の取得
	CJoypad *pJoy = CApplication::GetJoy();
	int nMaxPlayer = pJoy->GetUseJoyPad();

	if (nMaxPlayer > 0)
	{// コントローラー接続がない
		for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
		{
			if (!pJoy->AnyButton(nCntPlayer))
			{// 全てのボタン
				continue;
			}

			m_bPressEnter = true;
			m_nCntFrame = 0;
			m_fCycle = 0.1f;

			// SE
			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
			break;
		}
	}
	else
	{
		if (CApplication::GetKeyboard()->GetUseAnyKey())
		{// 全てのキー
			m_bPressEnter = true;
			m_nCntFrame = 0;
			m_fCycle = 0.1f;

			// SE
			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//=============================================================================
// オブジェクトの点滅
//=============================================================================
void CTitle::FlashObj()
{
	m_nCntFrame++;

	float sinCurve = (sinf((m_nCntFrame * m_fCycle) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
	float alpha = (sinCurve * 0.75f) + 0.25f;

	// 色の設定
	m_pPressAny->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha));
}
