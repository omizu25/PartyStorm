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

		if (CApplication::GetMode() != CApplication::MODE_GAME)
		{
			return;
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