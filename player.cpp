//=============================================================================
//
// プレイヤークラス(player.cpp)
// Author : 唐﨑結斗
// 概要 : プレイヤー生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "player.h"
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
#include "line.h"
#include "joypad.h"
#include "title.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : モーションキャラクター3Dを生成する
//=============================================================================
CPlayer * CPlayer::Create()
{
	// オブジェクトインスタンス
	CPlayer *pPlayer = nullptr;

	// メモリの解放
	pPlayer = new CPlayer;

	// メモリの確保ができなかった
	assert(pPlayer != nullptr);

	// 数値の初期化
	pPlayer->Init();

	// インスタンスを返す
	return pPlayer;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CPlayer::CPlayer() : m_pMove(nullptr),
m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_fSpeed(0.0f),
m_nNumMotion(0),
m_bDead(false)
{
#ifdef _DEBUG
	// ライン情報
	m_pLine = nullptr;
#endif // _DEBUG
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CPlayer::Init()
{
	// 初期化
	CMotionModel3D::Init();

	// 移動クラスのメモリ確保
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// 当たり判定の設定
	SetColisonPos(D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	SetColisonSize(D3DXVECTOR3(20.0f, 50.0f, 20.0f));

	// カメラの追従設定(目標 : プレイヤー)
	CApplication::GetCamera()->SetFollowTarget(this, D3DXVECTOR3(0.0f, 50.0f, 0.0f), 300.0f, 0.5f);

	// オブジェクトタイプの設定
	SetObjType(OBJTYPE_3DPLAYER);

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

	return E_NOTIMPL;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CPlayer::Uninit()
{
	if (m_pMove != nullptr)
	{// 終了処理
	 // メモリの解放
		delete m_pMove;
		m_pMove = nullptr;
	}

#ifdef _DEBUG
	for (int nCntLine = 0; nCntLine < 12; nCntLine++)
	{
		m_pLine[nCntLine]->Uninit();
	}

	delete m_pLine;
#endif // _DEBUG

	// 終了
	CMotionModel3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CPlayer::Update()
{
	if (!m_bDead)
	{// キーボードの取得
		CKeyboard *pKeyboard = CApplication::GetKeyboard();

		// モーション情報の取得
		CMotion *pMotion = CMotionModel3D::GetMotion();

		// 位置の取得
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();
		SetPosOld(pos);

		// 攻撃
		if (pKeyboard->GetTrigger(DIK_RETURN)
			&& pMotion != nullptr)
		{
			pMotion->SetNumMotion(ATTACK_ACTION);
			m_EAction = ATTACK_ACTION;
		}

		// 移動
		pos += Move();

		//pos.y -= CCalculation::Gravity();

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

		// 当たり判定
		Collison();

		// メッシュの当たり判定
		CMesh3D *pMesh = CGame::GetMesh();

		if (pMesh != nullptr)
		{
			pMesh->Collison(this);
		}

		// 位置の取得
		pos = GetPos();

		// 更新
		CMotionModel3D::Update();

#ifdef _DEBUG
		// デバック表示
		CDebugProc::Print("プレイヤーの位置 | X : %.3f | Y : %.3f | Z : %.3f |\n", pos.x, pos.y, pos.z);

		// ラインの更新
		SetLine();
#endif // _DEBUG
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CPlayer::Draw()
{
	if (!m_bDead)
	{// 描画
		CMotionModel3D::Draw();
	}
}

//=============================================================================
// 移動
// Author : 唐﨑結斗
// 概要 : キー入力で方向を決めて、移動ベクトルを算出する
//=============================================================================
D3DXVECTOR3 CPlayer::Move()
{
	// 変数宣言
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// モーション情報の取得
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// キーボードの取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	//コントローラー
	CJoypad *pJoy = CApplication::GetJoy();

	// 移動方向の取得
	if (pJoy->Stick(CJoypad::JOYKEY_LEFT_STICK, m_nNum, 0.5f))
	{// 向きの計算
		m_rotDest.y = pJoy->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, m_nNum);

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

	if (pJoy->GetPress(CJoypad::JOYKEY_UP, m_nNum)		//w
		|| pJoy->GetPress(CJoypad::JOYKEY_LEFT, m_nNum)	//a
		|| pJoy->GetPress(CJoypad::JOYKEY_RIGHT, m_nNum)	//s 
		|| pJoy->GetPress(CJoypad::JOYKEY_DOWN, m_nNum))	//d 
	{// 移動キーが押された
		if (pJoy->GetPress(CJoypad::JOYKEY_UP, m_nNum))
		{// [W]キーが押された時
			if (pJoy->GetPress(CJoypad::JOYKEY_LEFT, m_nNum))
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * -0.25f;
			}
			else if (pJoy->GetPress(CJoypad::JOYKEY_DOWN, m_nNum))
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{// 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.0f;
			}
		}
		else if (pJoy->GetPress(CJoypad::JOYKEY_DOWN, m_nNum))
		{// [S]キーが押された時
			if (pJoy->GetPress(CJoypad::JOYKEY_LEFT, m_nNum))
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * -0.75f;
			}
			else if (pJoy->GetPress(CJoypad::JOYKEY_RIGHT, m_nNum))
			{// [D]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * 0.75f;
			}
			else
			{// 移動方向の更新q
				m_rotDest.y = D3DX_PI;
			}
		}
		else if (pJoy->GetPress(CJoypad::JOYKEY_LEFT, m_nNum))
		{// [A]キーが押された時
		 // 移動方向の更新
			m_rotDest.y = D3DX_PI * -0.5f;
		}
		else if (pJoy->GetPress(CJoypad::JOYKEY_RIGHT, m_nNum))
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

	pJoy->GetStick(CJoypad::JOYKEY_LEFT_STICK,m_nNum);


	//キーボード
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
	CDebugProc::Print("移動ベクトル : %.3f\n", m_pMove->GetMoveLength());

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

//=============================================================================
// 回転
// Author : 唐﨑結斗
// 概要 : 目的の向きまで回転する
//=============================================================================
void CPlayer::Rotate()
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

//=============================================================================
// 当たり判定
// Author : 唐﨑結斗
// 概要 : 当たり判定を行う
//=============================================================================
void CPlayer::Collison()
{
	// プレイヤー情報の取得
	D3DXVECTOR3 pos = GetPos() + GetColisonPos();
	D3DXVECTOR3 size = GetColisonSize();

	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		CObject *pTop = CObject::GetTop(nCntPriority);

		if (pTop != nullptr)
		{// 変数宣言
			CObject *pObject = pTop;

			while (pObject)
			{// 現在のオブジェクトの次のオブジェクトを保管
				CObject *pObjectNext = pObject->GetNext();

				if (!pObject->GetFlagDeath()
					&& (pObject->GetObjType() == OBJTYPE_3DMODEL
					|| pObject->GetObjType() == OBJTYPE_3DPLAYER
					|| pObject->GetObjType() == OBJTYPE_3DENEMY)
					&& pObject != this)
				{
					if (ColisonRectangle3D(pObject, true))
					{
						if (pObject->GetObjType() == OBJTYPE_3DPLAYER)
						{// プレイヤーの移動
							CPlayer *pPlayer = dynamic_cast<CPlayer*>(pObject);
							CMove *pMove = GetMove();
							CMove *pMoveTarget = pPlayer->GetMove();
							D3DXVECTOR3 pos = pPlayer->GetPos();
							pos += pMove->GetMove() - pMoveTarget->GetMove();
							pPlayer->SetPos(pos);
						}	
						if (pObject->GetObjType() == OBJTYPE_3DENEMY)
						{// 敵との当たり判定
							m_bDead = true;
						}
					}
				}

				// 現在のオブジェクトの次のオブジェクトを更新
				pObject = pObjectNext;
			}
		}
	}
}

#ifdef _DEBUG
//=============================================================================
// ラインの設置
// Author : 唐﨑結斗
// 概要 : ラインを矩形状に設置
//=============================================================================
void CPlayer::SetLine()
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


