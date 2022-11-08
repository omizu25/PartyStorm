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
	m_nextMode = MODE_NONE;		// 次に設定するモード
	m_pTitleLogo = nullptr;		// タイトルロゴオブジェクト
	m_pNewGame = nullptr;		// ニューゲームオブジェクト
	m_pExit = nullptr;			// 終了オブジェクト
	m_fAddAlpha = 0.0f;			// フレーム数のカウント
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
{// マウスの取得
	CMouse *pMouse = CApplication::GetMouse();

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// タイトルBGMの再生
	pSound->PlaySound(CSound::SOUND_LABEL_TITELBGM);

	// タイトルロゴ
	m_pTitleLogo = CObject2D::Create();
	m_pTitleLogo->SetPos(D3DXVECTOR3(640.0f, 280.0f, 0.0f));
	m_pTitleLogo->SetSize(D3DXVECTOR3(800.0f, 300.0f, 0.0f));
	m_pTitleLogo->LoadTex(20);

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
	pMesh3D[0]->LoadTex(2);

	// メッシュの生成
	pMesh3D[1] = CMesh3D::Create();
	pMesh3D[1]->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
	pMesh3D[1]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D[1]->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	pMesh3D[1]->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
	pMesh3D[1]->SetSplitTex(false);
	pMesh3D[1]->SetWave(7.0f, 10.0f);
	pMesh3D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	pMesh3D[1]->LoadTex(3);

	// スカイボックスの設定
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(5000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(4);

	//サメ設定
	CEnemyShark *pEnemyShark = CEnemyShark::Create();
	pEnemyShark->SetMotion("data/MOTION/motionShark.txt");
	pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
	pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));

	// モデルの設置
	CModelObj::LoadFile("data/FILE/setModel.txt");

	// マウスカーソルを消す
	pMouse->SetShowCursor(false);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CTitle::Uninit()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// サウンド終了
	pSound->StopSound();

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

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		CApplication::SetNextMode(CApplication::MODE_GAME);
	}

	m_fAddSize += 0.05f;
	D3DXVECTOR3 sizeTitleLogo = m_pTitleLogo->GetSize();
	m_pTitleLogo->SetSize(D3DXVECTOR3(sizeTitleLogo.x + sinf(m_fAddSize) * 3.0f,
		sizeTitleLogo.y + sinf(m_fAddSize) * 2.0f, 0.0f));

	//ゲット
	CDebugProc::Print("PersonCount Q or E: %d\n", m_nNumPlayer);

	int nMaxPlayer = pJoy->GetUseJoyPad();

	if (nMaxPlayer >= 0)
	{
		nMaxPlayer = 1;
	}

	if (pKeyboard->GetTrigger(DIK_Q))
	{
		m_nNumPlayer++;
	}
	if (pKeyboard->GetTrigger(DIK_E))
	{
		m_nNumPlayer--;
	}

	if (m_nNumPlayer > nMaxPlayer)	// 4を越えないよう
	{
		m_nNumPlayer = 1;
	}
	if (m_nNumPlayer < 1)	// マイナス設定無し
	{
		m_nNumPlayer = nMaxPlayer;
	}

	CApplication::SetPersonCount(m_nNumPlayer);
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CTitle::Draw()
{

}