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
#include "inputcontroller.h"

#include "application.h"
#include "camera_manager.h"
#include "renderer.h"
#include "object.h"
#include "object3D.h"

//モデル
#include "player.h"
#include "Shark.h"
#include "obstacle.h"

#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "model_obj.h"
#include "score.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPlayer *CGame::m_pPlayer = nullptr;					// プレイヤークラス
CEnemyShark *CGame::m_pEnemyShark = nullptr;			// Enemy
CScore *CGame::m_pScore = nullptr;						// スコアクラス
CMesh3D *CGame::m_pMesh3D;								// メッシュクラス
bool CGame::m_bGame = false;							// ゲームの状況

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CGame::CGame()
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

	// 重力の値を設定
	CCalculation::SetGravity(10.0f);

	// 地面の設定
	m_pMesh3D = CMesh3D::Create();
	m_pMesh3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetSize(D3DXVECTOR3(5000.0f, 0, 5000.0f));
	m_pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	m_pMesh3D->SetSplitTex(true);
	/*m_pMesh3D->SetWave(1.0f, 300.0f);*/
	m_pMesh3D->LoadTex(-1);

	// スカイボックスの設定
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(50000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(-1);

	// プレイヤーの設定
	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetMotion("data/MOTION/motion.txt");
	m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//サメ設定
	m_pEnemyShark = CEnemyShark::Create();
	m_pEnemyShark->SetMotion("data/MOTION/motionShark.txt");
	m_pEnemyShark->SetPos(D3DXVECTOR3(0.0f, 0.0f, 200.0f));

	// モデルの設置
	CModelObj::LoadFile("data/FILE/setModel.txt");

	// スコアの生成
	m_pScore = CScore::Create(10, false);
	m_pScore->SetScore(0);
	m_pScore->SetPos(D3DXVECTOR3(1280.0f, m_pScore->GetSize().y / 2.0f, 0.0f));

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

	// スコアの解放
	Release();
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

	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	if (pKeyboard->GetTrigger(DIK_F3))
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}

	//コントローラー使用の確認
	CInputController *pjoy = CApplication::GetJoy();
	if (pjoy->GetJoypadTrigger(CInputController::JOYKEY_A, 0))
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
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
