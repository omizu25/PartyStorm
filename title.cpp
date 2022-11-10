//=============================================================================
//
// タイトルクラス(title.cpp)
// Author : 唐﨑結斗
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
#include "mouse.h"
#include "debug_proc.h"
#include "joypad.h"
#include "sound.h"
#include "object2D.h"

#include "camera_manager.h"
#include "renderer.h"
#include "object.h"
#include "object3D.h"

//モデル
#include "player.h"
#include "Shark.h"
#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "model_obj.h"

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTitle::CTitle()
{
	m_pTitleLogo = nullptr;		// タイトルロゴオブジェクト
	m_pPressAny = nullptr;		// プレスオブジェクト
	m_pMode = nullptr;			// モードオブジェクト
	m_pNum = nullptr;			// 人数オブジェクト
	m_fCycle = 0.0f;			// フレーム数のカウント
	m_fAddSize = 0.0f;			// 大きさの参照値
	m_nCntFrame = 0;			// フレームカウント
	m_nNumPlayer = 0;			// プレイヤー数
	m_bPressEnter = true;		// エンターキーを押せるか
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CTitle::Init()
{
	// タイトルBGMの再生
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_TITELBGM);

	// タイトルロゴ
	m_pTitleLogo = CObject2D::Create();
	m_pTitleLogo->SetPos(D3DXVECTOR3(640.0f, 230.0f, 0.0f));
	m_pTitleLogo->SetSize(D3DXVECTOR3(800.0f, 300.0f, 0.0f));
	m_pTitleLogo->LoadTex(18);

	// プレスオブジェクト
	m_pPressAny = CObject2D::Create();
	m_pPressAny->SetPos(D3DXVECTOR3(640.0f, 600.0f, 0.0f));
	m_pPressAny->SetSize(D3DXVECTOR3(450.0f, 180.0f, 0.0f));

	int nMaxPlayer = CApplication::GetJoy()->GetUseJoyPad();

	if (nMaxPlayer <= 0)
	{// パッドの接続が一つも無い
		nMaxPlayer = 1;
		m_pPressAny->LoadTex(34);
		m_pPressAny->SetSize(D3DXVECTOR3(550.0f, 180.0f, 0.0f));
	}
	else
	{
		m_pPressAny->LoadTex(33);
		m_pPressAny->SetSize(D3DXVECTOR3(600.0f, 180.0f, 0.0f));
	}

	CApplication::SetPersonCount(nMaxPlayer);

	if (nMaxPlayer <= 1)
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

		switch (nMaxPlayer)
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

	// カメラの位置変更
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetPosV(D3DXVECTOR3(0.0f, 300.0f, -1600.0f));
	pCamera->SetPosR(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
	pCamera->SetViewing(20.0f);

	// 地面の設定
	CMesh3D *pMesh3D[2];
	pMesh3D[0] = CMesh3D::Create();
	pMesh3D[0]->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D[0]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D[0]->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	pMesh3D[0]->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	pMesh3D[0]->SetSplitTex(true);
	pMesh3D[0]->SetScrollTex(D3DXVECTOR2(0.0f, 0.01f), true);
	pMesh3D[0]->LoadTex(0);

	// メッシュの生成
	pMesh3D[1] = CMesh3D::Create();
	pMesh3D[1]->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
	pMesh3D[1]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D[1]->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	pMesh3D[1]->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
	pMesh3D[1]->SetSplitTex(false);
	pMesh3D[1]->SetWave(7.0f, 10.0f);
	pMesh3D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	pMesh3D[1]->LoadTex(1);

	// スカイボックスの設定
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(5000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(2);

	//サメ設定
	CEnemyShark *pEnemyShark = CEnemyShark::Create();
	pEnemyShark->SetMotion("data/MOTION/motionShark.txt");
	pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
	pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));

	// モデルの設置
	CModelObj::LoadFile("data/FILE/setModel.txt");

	{// モーションモデルの設定
		CMotionModel3D *pSnake = CMotionModel3D::Create();
		pSnake->SetMotion("data/MOTION/snake.txt");
		pSnake->SetPos(D3DXVECTOR3(400.0f, 0.0f, 1000.0f));
		pSnake->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		pSnake = CMotionModel3D::Create();
		pSnake->SetMotion("data/MOTION/snake.txt");
		pSnake->SetPos(D3DXVECTOR3(-400.0f, 0.0f, 1000.0f));
		pSnake->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

		pSnake = CMotionModel3D::Create();
		pSnake->SetMotion("data/MOTION/snake.txt");
		pSnake->SetPos(D3DXVECTOR3(600.0f, 0.0f, 1000.0f));
		pSnake->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		pSnake = CMotionModel3D::Create();
		pSnake->SetMotion("data/MOTION/snake.txt");
		pSnake->SetPos(D3DXVECTOR3(-600.0f, 0.0f, 1000.0f));
		pSnake->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	}

	m_fCycle = 0.01f;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
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
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CTitle::Update()
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// ジョイパッド 情報の取得
	CJoypad *pJoy = CApplication::GetJoy();

	{// タイトルロゴのサイズ変更
		m_fAddSize += 0.05f;

		D3DXVECTOR3 sizeTitleLogo = m_pTitleLogo->GetSize();
		sizeTitleLogo.x += sinf(m_fAddSize) * 3.0f;
		sizeTitleLogo.y += sinf(m_fAddSize) * 2.0f;

		m_pTitleLogo->SetSize(sizeTitleLogo);
	}

	// 点滅
	FlashObj();

	int nMaxPlayer = pJoy->GetUseJoyPad();

	if (nMaxPlayer <= 0)
	{
		nMaxPlayer = 1;
		m_pPressAny->LoadTex(34);
		m_pPressAny->SetSize(D3DXVECTOR3(550.0f, 180.0f, 0.0f));
	}
	else
	{
		m_pPressAny->LoadTex(33);
		m_pPressAny->SetSize(D3DXVECTOR3(600.0f, 180.0f, 0.0f));
	}

	CApplication::SetPersonCount(nMaxPlayer);

	if (nMaxPlayer <= 1)
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
		
		switch (nMaxPlayer)
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

	if (m_bPressEnter)
	{
		if (pJoy->GetUseJoyPad() > 0)
		{// パッドが使用されている
			for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
			{
				if (pJoy->AnyButton(nCntPlayer))
				{
					m_bPressEnter = false;
					m_nCntFrame = 0;
					m_fCycle = 0.1f;
					break;
				}
			}
		}
		else
		{// されていない
			if (pKeyboard->GetUseAnyKey())
			{
				m_bPressEnter = false;
				m_nCntFrame = 0;
				m_fCycle = 0.1f;
			}
		}
	}
	else
	{
		if (m_nCntFrame >= 60)
		{
			CApplication::SetNextMode(CApplication::MODE_GAME);
		}
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CTitle::Draw()
{
}

//=============================================================================
// オブジェクトの点滅
// Author : 香月瑞輝
// 概要 : 指定のオブジェクトを点滅させる
//=============================================================================
void CTitle::FlashObj()
{
	m_nCntFrame++;

	float sinCurve = (sinf((m_nCntFrame * m_fCycle) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
	float alpha = (sinCurve * 0.75f) + 0.25f;

	// 色の設定
	m_pPressAny->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha));
}
