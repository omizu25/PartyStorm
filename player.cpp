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
#include "effect.h"
#include "sound.h"
#include "object3D.h"
#include "result.h"
#include "time.h"
#include "ranking.h"
#include "follow_model.h"

namespace
{
const int texIdx[4]
{
	12,
	13,
	14,
	15
};
}

//--------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------
const D3DXVECTOR2 CPlayer::MAX_VIB_RAND = D3DXVECTOR2(40.0f, 70.0f);		// 振動の幅
const float CPlayer::VIB_SPEED = 20.0f;										// 振動の速度
const float CPlayer::VIB_COEFFICIENT = 0.2f;								// 振動の減衰値

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
m_pIdx(nullptr),
m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_fSpeed(0.0f),
m_nNumMotion(0),
m_nVibCount(0),
m_bVib(false),
m_bDead(false),
m_bAction(false),
m_bMove(false)
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

	m_pIdx =CObject3D::Create();
	m_pIdx->SetSize(D3DXVECTOR3(15.0f, 15.0f, 0.0f));
	m_pIdx->SetBillboard(true);

	// 移動クラスのメモリ確保
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// 当たり判定の設定
	SetColisonPos(D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	SetColisonSize(D3DXVECTOR3(40.0f, 50.0f, 40.0f));

	// オブジェクトタイプの設定
	SetObjType(OBJTYPE_3DPLAYER);

	m_bAction = true;
	m_bMove = true;

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

	if (m_pIdx != nullptr)
	{// 終了処理
		m_pIdx->Uninit();
		m_pIdx = nullptr;
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
	CApplication::SCENE_MODE scene = CApplication::GetMode();

	if (!m_bDead)
	{// キーボードの取得
		// モーション情報の取得
		CMotion *pMotion = CMotionModel3D::GetMotion();

		if (m_bAction)
		{// 行動していい
			// 位置の取得
			D3DXVECTOR3 pos = GetPos();
			D3DXVECTOR3 rot = GetRot();
			SetPosOld(pos);

			if (CApplication::GetMode() == CApplication::MODE_GAME &&
				CGame::GetTime()->GetTime() <= 0)
			{// ゲーム中で制限時間切れ
				pos.z += -20.0f;
				m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			else
			{
				// 移動
				pos.z += CCalculation::Gravity();
				pos += Move();
			}
			
			// 回転
			Rotate();

			// 位置の設定
			SetPos(pos);

			// 当たり判定
			Collison();
		}

		// ニュートラルモーションの設定
		if (pMotion != nullptr
			&& !pMotion->GetMotion())
		{
			m_EAction = NEUTRAL_ACTION;
			pMotion->SetNumMotion(m_EAction);
		}

		if (scene == CApplication::MODE_GAME)
		{// ゲーム中
			// メッシュの当たり判定
			CMesh3D *pMesh = CGame::GetMesh();

			if (pMesh != nullptr)
			{
				pMesh->Collison(this);
			}
		}

		// 位置の取得
		D3DXVECTOR3 pos = GetPos();

		if (m_pIdx != nullptr)
		{// nullチェック
			// 位置の更新
			m_pIdx->SetPos(D3DXVECTOR3(pos.x, pos.y + 100.0f, pos.z));
		}

		// 更新
		CMotionModel3D::Update();

#ifdef _DEBUG
		// デバック表示
		CDebugProc::Print("プレイヤーの位置 | X : %.3f | Y : %.3f | Z : %.3f |\n", pos.x, pos.y, pos.z);

		// ラインの更新
		SetLine();
#endif // _DEBUG
	}

	if (m_bVib)
	{
		m_nVibCount++;

		if (m_nVibCount >= 60)
		{
			CFollowModel *pCameraTarget = CGame::GetCameraTarget();
			pCameraTarget->SetFollow(false);
			pCameraTarget->SetPos(CGame::CAMERA_POSR);
			m_nVibCount = 0;
			m_bVib = false;
		}
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
	if (!m_bMove)
	{// 移動しない
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

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

	if (pJoy->GetUseJoyPad() == 0)
	{//キーボード
		if ((pKeyboard->GetPress(DIK_W)
			|| pKeyboard->GetPress(DIK_A)
			|| pKeyboard->GetPress(DIK_D)
			|| pKeyboard->GetPress(DIK_S))
			|| (pKeyboard->GetPress(DIK_UP)
			|| pKeyboard->GetPress(DIK_LEFT)
			|| pKeyboard->GetPress(DIK_RIGHT)
			|| pKeyboard->GetPress(DIK_DOWN)))
		{// 移動キーが押された
			if (pKeyboard->GetPress(DIK_W)
				|| pKeyboard->GetPress(DIK_UP))
			{// [W]キーが押された時
				if (pKeyboard->GetPress(DIK_A)
					|| pKeyboard->GetPress(DIK_LEFT))
				{// [A]キーが押された時
				 // 移動方向の更新
					m_rotDest.y = D3DX_PI * -0.25f;
				}
				else if (pKeyboard->GetPress(DIK_D)
					|| pKeyboard->GetPress(DIK_RIGHT))
				{// [D]キーが押された時
				 // 移動方向の更新
					m_rotDest.y = D3DX_PI * 0.25f;
				}
				else
				{// 移動方向の更新
					m_rotDest.y = D3DX_PI * 0.0f;
				}
			}
			else if (pKeyboard->GetPress(DIK_S)
				|| pKeyboard->GetPress(DIK_DOWN))
			{// [S]キーが押された時
				if (pKeyboard->GetPress(DIK_A)
					|| pKeyboard->GetPress(DIK_LEFT))
				{// [A]キーが押された時
				 // 移動方向の更新
					m_rotDest.y = D3DX_PI * -0.75f;
				}
				else if (pKeyboard->GetPress(DIK_D)
					|| pKeyboard->GetPress(DIK_RIGHT))
				{// [D]キーが押された時
				 // 移動方向の更新
					m_rotDest.y = D3DX_PI * 0.75f;
				}
				else
				{// 移動方向の更新q
					m_rotDest.y = D3DX_PI;
				}
			}
			else if (pKeyboard->GetPress(DIK_A)
				|| pKeyboard->GetPress(DIK_LEFT))
			{// [A]キーが押された時
			 // 移動方向の更新
				m_rotDest.y = D3DX_PI * -0.5f;
			}
			else if (pKeyboard->GetPress(DIK_D)
				|| pKeyboard->GetPress(DIK_RIGHT))
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

	moveing.x *= 0.5f;

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
	if (CApplication::GetMode() == CApplication::MODE_GAME &&
		CGame::GetTime()->GetTime() <= 0)
	{// ゲーム中で制限時間切れ
		return;
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject *pTop = CObject::GetTop(nCntPriority);

		if (pTop == nullptr)
		{// 変数宣言
			continue;
		}

		CObject *pObject = pTop;

		while (true)
		{
			if (pObject == nullptr)
			{// nullチェック
				break;
			}

			// 現在のオブジェクトの次のオブジェクトを保管
			CObject *pObjectNext = pObject->GetNext();

			if (pObject->GetFlagDeath() ||
				pObject == this)
			{// 破棄する予定、自分自身
				// 現在のオブジェクトの次のオブジェクトを更新
				pObject = pObjectNext;
				continue;
			}
			
			// 種類の取得
			CObject::EObjectType type = pObject->GetObjType();

			if (type != OBJTYPE_3DPLAYER &&
				type != OBJTYPE_3DENEMY)
			{// 指定のタイプではない
				// 現在のオブジェクトの次のオブジェクトを更新
				pObject = pObjectNext;
				continue;
			}

			if (!ColisonRectangle3D(pObject, true))
			{// 当たっていない
				// 現在のオブジェクトの次のオブジェクトを更新
				pObject = pObjectNext;
				continue;
			}

			if (type == OBJTYPE_3DPLAYER)
			{// プレイヤー
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(pObject);
				CMove *pMove = GetMove();
				CMove *pMoveTarget = pPlayer->GetMove();
				D3DXVECTOR3 pos = pPlayer->GetPos();
				pos += pMove->GetMove() - pMoveTarget->GetMove();
				pPlayer->SetPos(pos);
			}
			else if (type == OBJTYPE_3DENEMY)
			{// 敵
				m_bDead = true;
				CEffect::Explosion(GetPos());
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_EAT);

				if (m_pIdx != nullptr)
				{// 終了処理
					m_pIdx->Uninit();
					m_pIdx = nullptr;
				}

				if (CApplication::GetMode() == CApplication::MODE_GAME)
				{// ゲーム中
					// 死亡した
					CResult::SetDead(m_nNum);

					// スコアの設定
					CRanking::Set(CGame::GetTime()->GetTime());

					CFollowModel *pCameraTarget = CGame::GetCameraTarget();

					if (pCameraTarget != nullptr)
					{// カメラターゲット情報
						D3DXVECTOR3 pos = pCameraTarget->GetPos();
						D3DXVECTOR3 posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						posDest.x = pos.x + MAX_VIB_RAND.x - (float)(rand() % (int)(MAX_VIB_RAND.x * 2.0f));
						posDest.y = pos.y + MAX_VIB_RAND.y - (float)(rand() % (int)(MAX_VIB_RAND.y * 2.0f));
						posDest.z = 0.0f;
						pCameraTarget->SetPos(posDest);
						pCameraTarget->SetFollow(CGame::CAMERA_POSR);
						pCameraTarget->SetSpeed(VIB_SPEED);
						pCameraTarget->SetCoefficient(VIB_COEFFICIENT);
						if (!m_bVib)
						{
							m_bVib = true;
						}
					}
				}
			}

			// 現在のオブジェクトの次のオブジェクトを更新
			pObject = pObjectNext;
		}
	}

	// プレイヤー情報の取得
	D3DXVECTOR3 pos = GetPos() + GetColisonPos();
	D3DXVECTOR3 size = GetColisonSize();

	if (pos.x - size.x < -250.0f)
	{
		pos.x = -250.0f + size.x;
	}
	else if (pos.x + size.x > 250.0f)
	{
		pos.x = 250.0f - size.x;
	}

	if (pos.z - size.z < -700.0f)
	{
		pos.z = -700.0f + size.z;
	}

	// 位置の設定
	SetPos(pos - GetColisonPos());
}

//=============================================================================
// 識別番号の設定
// Author : 香月瑞輝
// 概要 : 識別番号の設定
//=============================================================================
void CPlayer::SetNum(const int nNum)
{
	m_nNum = nNum;

	m_pIdx->LoadTex(texIdx[nNum]);
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


