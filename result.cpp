//=============================================================================
//
// リザルトクラス(result.cpp)
// Author : 唐﨑結斗
// Author : 香月瑞輝
// 概要 : リザルトクラスの管理を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "result.h"
#include "object2D.h"
#include "sound.h"
#include "camera.h"
#include "sphere.h"
#include "player.h"
#include "Shark.h"
#include "model_obj.h"
#include "effect.h"
#include "renderer.h"
#include "score.h"
#include "ranking.h"
#include "menu.h"
#include "obstacle_manager.h"

//*****************************************************************************
// 変数定義
//*****************************************************************************
namespace
{
const int POP_TIME = 240;	// 出現させる時間
const int texIdx[4]
{
	12,
	13,
	14,
	15
};
}

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPlayer **CResult::m_pPlayer = nullptr;	// プレイヤークラス
CMenu *CResult::m_pMenu = nullptr;		// メニュークラス
bool CResult::m_dead[4] = {};			// 死亡したかどうか

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult() :
	m_mode(CApplication::MODE_GAME),
	m_time(0),
	m_pop(false),
	m_pressEnter(false)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// 死亡の初期化
//=============================================================================
void CResult::InitDead()
{
	for (int i = 0; i < 4; i++)
	{
		m_dead[i] = false;
	}
}

//=============================================================================
// 死亡の設定
//=============================================================================
void CResult::SetDead(int numPlayer)
{
	m_dead[numPlayer] = true;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CResult::Init()
{
	int maxPlayer = CApplication::GetPersonCount();

	{// サウンドの設定
		int gameover = 0;

		for (int i = 0; i < maxPlayer; i++)
		{
			if (m_dead[i])
			{// 死亡した
				gameover++;
			}
		}

		if (gameover == maxPlayer &&
			maxPlayer > 1)
		{// 全員死亡
			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GAMEOVERBGM);
		}
		else
		{
			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_RESULTBGM);
		}
	}

	{// カメラの位置変更
		CCamera *pCamera = CApplication::GetCamera();
		pCamera->SetPosV(D3DXVECTOR3(0.0f, 300.0f, -1600.0f));
		pCamera->SetPosR(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
		pCamera->SetViewing(20.0f);
	}

	{// 地面の設定
		CMesh3D *pMesh3D = CMesh3D::Create();
		pMesh3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
		pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
		pMesh3D->SetSplitTex(true);
		pMesh3D->SetScrollTex(D3DXVECTOR2(0.0f, 0.01f), true);
		pMesh3D->LoadTex(0);
	}

	{// メッシュの生成
		CMesh3D *pMesh3D = CMesh3D::Create();
		pMesh3D->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
		pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
		pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
		pMesh3D->SetSplitTex(false);
		pMesh3D->SetWave(7.0f, 10.0f);
		pMesh3D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		pMesh3D->LoadTex(1);
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

	{// プレイヤーの設定
		m_pPlayer = new CPlayer*[maxPlayer];
		assert(m_pPlayer != nullptr);

		float posX = 500.0f / (maxPlayer + 1);

		for (int i = 0; i < maxPlayer; i++)
		{// プレイヤーの生成
			m_pPlayer[i] = CPlayer::Create();
			m_pPlayer[i]->SetMotion("data/MOTION/motion.txt");
			m_pPlayer[i]->SetPos(D3DXVECTOR3(-250.0f + posX + (posX * i), 0.0f, -300.0f));
			m_pPlayer[i]->SetNum(i);

			if (maxPlayer > 1)
			{// マルチプレイ
				if (m_dead[i])
				{// 死んだプレイヤー
					m_pPlayer[i]->SetAction(true);
					m_pPlayer[i]->SetMove(false);
				}
				else
				{// 生きてるプレイヤー
					m_pPlayer[i]->SetAction(false);
				}
			}
			else
			{// シングルプレイ
				m_pPlayer[i]->SetAction(false);
			}
		}
	}

	// モデルの設置
	CModelObj::LoadFile("data/FILE/setModel.txt");

	m_time = 0;
	m_pop = false;
	m_mode = CApplication::MODE_GAME;
	m_pressEnter = false;

	bool gameclear = true;
	
	if (maxPlayer > 1)
	{// マルチプレイ
		for (int i = 0; i < maxPlayer; i++)
		{
			if (m_dead[i])
			{// 死亡した
				gameclear = false;
			}
		}

		m_pMenu = nullptr;

		if (gameclear)
		{// ゲームクリア
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.5f, CRenderer::SCREEN_HEIGHT * 0.25f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(1000.0f, 300.0f, 0.0f));
			pObj->LoadTex(20);

			m_pop = true;

			{// メニューの生成
				D3DXVECTOR3 pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.5f, (float)CRenderer::SCREEN_HEIGHT * 0.75f, 0.0f);
				D3DXVECTOR3 size = D3DXVECTOR3(350.0f, 100.0f, 0.0f);

				// メニューの生成
				m_pMenu = CMenu::Create();

				// メニューの設定
				m_pMenu->Set(pos, size, 2, 50.0f, true, true);

				// 枠の設定
				m_pMenu->SetFrame(D3DXVECTOR3(600.0f, 360.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));

				// テクスチャの設定
				m_pMenu->SetTexture(0, 8);
				m_pMenu->SetTexture(1, 11);
			}
		}
	}
	else
	{// シングルプレイ
		{// 背景
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.75f, (float)CRenderer::SCREEN_HEIGHT * 0.5f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(650.0f, (float)CRenderer::SCREEN_HEIGHT, 0.0f));
			pObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
			pObj->LoadTex(-1);
		}

		{// ランキングの生成
			CRanking *pRanking = CRanking::Create();
			assert(pRanking != nullptr);
			float width = (float)CRenderer::SCREEN_WIDTH;
			float height = (float)CRenderer::SCREEN_HEIGHT * 0.35f;
			pRanking->Set(D3DXVECTOR3(width, height, 0.0f), 40.0f);
		}

		{// スコアの生成
			CScore *pScore = CScore::Create(10, true);
			assert(pScore != nullptr);
			pScore->SetScore(0);

			// 大きさの設定
			pScore->SetWholeSize(D3DXVECTOR3(1000.0f, 50.0f, 0.0f));
			pScore->SetSize(D3DXVECTOR3(60.0f, 60.0f, 0.0f));

			// 位置の設定
			pScore->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.25f, CRenderer::SCREEN_HEIGHT * 0.35f, 0.0f));

			// 向きの設定
			pScore->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			pScore->SetDestScore(CRanking::Get(-1));
		}

		{// ランキングの文字
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.75f, CRenderer::SCREEN_HEIGHT * 0.15f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(600.0f, 150.0f, 0.0f));
			pObj->LoadTex(22);
		}

		{// 自分のスコアの文字
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.25f, CRenderer::SCREEN_HEIGHT * 0.15f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(600.0f, 175.0f, 0.0f));
			pObj->LoadTex(23);
		}

		{// メニューの生成
			D3DXVECTOR3 pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.25f, (float)CRenderer::SCREEN_HEIGHT * 0.75f, 0.0f);
			D3DXVECTOR3 size = D3DXVECTOR3(350.0f, 100.0f, 0.0f);

			// メニューの生成
			m_pMenu = CMenu::Create();

			// メニューの設定
			m_pMenu->Set(pos, size, 2, 50.0f, true, true);

			// 枠の設定
			m_pMenu->SetFrame(D3DXVECTOR3(600.0f, 360.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));

			// テクスチャの設定
			m_pMenu->SetTexture(0, 8);
			m_pMenu->SetTexture(1, 11);
		}
	}

	{//サメ設定
		CEnemyShark *pEnemyShark = CEnemyShark::Create();

		if (gameclear)
		{// ゲームクリア、シングルプレイ
			pEnemyShark->SetMotion("data/MOTION/motionShark.txt");
		}
		else
		{// ゲームオーバー
			pEnemyShark->SetMotion("data/MOTION/motionShark.txt", 2);
		}

		pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
		pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));
	}

	// 障害物の背景
	CObstacleManager::Create();

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CResult::Uninit()
{
	// サウンド終了
	CApplication::GetSound()->StopSound();

	// エフェクトの終了
	CEffect::ReleaseAll();

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
}

//=============================================================================
// 更新
//=============================================================================
void CResult::Update()
{
	m_time++;

	if (CApplication::GetPersonCount() > 1)
	{// マルチプレイ
		Multi();
	}

	// エフェクトの更新
	CEffect::UpdateAll();

	if (!m_pressEnter)
	{// エンターが押されていない
		// 選択
		Select();
	}
	else
	{// エンターが押された

		if (m_pMenu != nullptr)
		{// 選択
			// 色の変更
			m_pMenu->ColorChange();
		}

		if (m_time >= 60)
		{// 一定時間後
			CApplication::SetNextMode(m_mode);
		}
	}
}

//=============================================================================
// 描画
//=============================================================================
void CResult::Draw()
{
}

//=============================================================================+
// 選択
//=============================================================================
void CResult::Select()
{
	if (m_pMenu == nullptr)
	{// 選択
		return;
	}

	int mode = m_pMenu->Select();

	switch (mode)
	{
	case -1:
		break;

	case 0:
		m_mode = CApplication::MODE_GAME;
		m_pressEnter = true;
		m_time = 0;
		break;

	case 1:
		m_mode = CApplication::MODE_TITLE;
		m_pressEnter = true;
		m_time = 0;
		break;

	default:
		assert(false);
		break;
	}
}

//=============================================================================
// マルチ
//=============================================================================
void CResult::Multi()
{
	if (m_pop)
	{// 出現した
		return;
	}

	if (m_time >= POP_TIME)
	{// 出現する時間を越した
		int gameover = 0;
		int maxPlayer = CApplication::GetPersonCount();
		
		for (int i = 0; i < maxPlayer; i++)
		{
			if (m_dead[i])
			{// 死亡した
				gameover++;
			}
		}

		if (gameover == maxPlayer)
		{// 全員死亡
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.5f, CRenderer::SCREEN_HEIGHT * 0.25f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(1000.0f, 300.0f, 0.0f));
			pObj->LoadTex(21);

			{// メニューの生成
				D3DXVECTOR3 pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.5f, (float)CRenderer::SCREEN_HEIGHT * 0.75f, 0.0f);
				D3DXVECTOR3 size = D3DXVECTOR3(350.0f, 100.0f, 0.0f);

				// メニューの生成
				m_pMenu = CMenu::Create();

				// メニューの設定
				m_pMenu->Set(pos, size, 2, 50.0f, true, true);

				// 枠の設定
				m_pMenu->SetFrame(D3DXVECTOR3(600.0f, 360.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));

				// テクスチャの設定
				m_pMenu->SetTexture(0, 8);
				m_pMenu->SetTexture(1, 11);
			}
		}
		else
		{// 生き残りがいる
			float posX = (float)(CRenderer::SCREEN_WIDTH * 0.5f) / ((maxPlayer - gameover) + 1);
			int count = 0;

			for (int i = 0; i < maxPlayer; i++)
			{// プレイヤーの生成
				if (!m_dead[i])
				{// 死亡していない
					CObject2D *pObj = CObject2D::Create();
					pObj->SetPos(D3DXVECTOR3(((float)(CRenderer::SCREEN_WIDTH * 0.5f)) + posX + (posX * count), 200.0f, 0.0f));
					pObj->SetSize(D3DXVECTOR3(150.0f, 150.0f, 0.0f));
					pObj->LoadTex(texIdx[i]);
					count++;
				}
			}

			assert(count == (maxPlayer - gameover));

			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.74f, CRenderer::SCREEN_HEIGHT * 0.65f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(600.0f, 300.0f, 0.0f));
			pObj->LoadTex(19);

			{// メニューの生成
				D3DXVECTOR3 pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.25f, (float)CRenderer::SCREEN_HEIGHT * 0.5f, 0.0f);
				D3DXVECTOR3 size = D3DXVECTOR3(350.0f, 100.0f, 0.0f);

				// メニューの生成
				m_pMenu = CMenu::Create();

				// メニューの設定
				m_pMenu->Set(pos, size, 2, 100.0f, true, true);

				// 枠の設定
				m_pMenu->SetFrame(D3DXVECTOR3(550.0f, (float)CRenderer::SCREEN_HEIGHT, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));

				// テクスチャの設定
				m_pMenu->SetTexture(0, 8);
				m_pMenu->SetTexture(1, 11);
			}
		}

		m_pop = true;
	}
}
