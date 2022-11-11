//=============================================================================
//
// ポーズクラス(pause.cpp)
// Author : 唐﨑結斗
// 概要 : ポーズ生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "pause.h"
#include "object2D.h"
#include "renderer.h"
#include "application.h"
#include "keyboard.h"
#include "sound.h"
#include "fade.h"
#include "joypad.h"
#include "debug_proc.h"

//--------------------------------------------------------------------
// 定数定義
//--------------------------------------------------------------------
const D3DXCOLOR CPause::BASE_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// ベース色
const D3DXCOLOR CPause::SELECT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// セレクト色
const D3DXCOLOR CPause::BACK_COLOR = D3DXCOLOR(0.7f, 0.2f, 0.2f, 0.5f);		// 背景色

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スコアを生成する
//=============================================================================
CPause *CPause::Create()
{
	// オブジェクトインスタンス
	CPause *pPause = nullptr;

	pPause = new CPause;

	// メモリの確保ができなかった
	assert(pPause != nullptr);

	// 数値の初期化
	pPause->Init();

	// インスタンスを返す
	return pPause;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CPause::CPause(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_nextMode = MODE_RETURN;							// 次のモード
	m_pBGObj = nullptr;									// 背景オブジェクト
	m_pPauseBGObj = nullptr;							// ポーズ背景オブジェクト
	m_pReturnObj = nullptr;								// リターンオブジェクト
	m_pNewGameObj = nullptr;							// ニューゲームオブジェクト
	m_pTitleObj = nullptr;								// タイトルオブジェクト
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
	m_fCycle = 0.0f;									// カーブの周期
	m_nCntFrame = 0;									// フレームカウント
	m_bPressEnter = true;								// エンターキーを押せるかさ
	m_bPause = false;									// ポーズしているか
	m_bSelect = true;									// 選択していいか
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CPause::Init()
{
	m_pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(1200.0f, 350.0f, 0.0f);
	SetObjType(CObject::OBJTYPE_PAUSE);

	m_fCycle = 0.025f;

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CPause::Uninit()
{
	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CPause::Update()
{
	if (!m_bSelect)
	{// 選択してはいけない
		return;
	}

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	CJoypad *pJoypad = CApplication::GetJoy();
	bool bJoypad = false;
	bool bPause = false;

	if (pJoypad->GetUseJoyPad() > 0)
	{
		bJoypad = true;
	}

	if (CApplication::GetMode() == CApplication::MODE_GAME
		&& !CApplication::GetFade()->GetFadeSituation())
	{
		if (m_bPause)
		{
			if (m_bPressEnter)
			{
				SelectMode();
			}

			FlashObj();

			if (m_bPressEnter)
			{
				if (!bJoypad)
				{
					bPause = pKeyboard->GetTrigger(DIK_RETURN);
				}
				else
				{
					for (int nCntJoy = 0; nCntJoy < pJoypad->GetUseJoyPad(); nCntJoy++)
					{
						bPause = pJoypad->GetTrigger(CJoypad::JOYKEY_A, nCntJoy);

						if (bPause)
						{
							break;
						}

						bPause = pJoypad->GetTrigger(CJoypad::JOYKEY_B, nCntJoy);

						if (bPause)
						{
							break;
						}
					}
				}

				if (bPause)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
					m_bPressEnter = false;
					m_fCycle = 0.1f;
					m_nCntFrame = 0;
				}
			}

			if (m_bPressEnter)
			{
				if (!bJoypad)
				{
					bPause = pKeyboard->GetTrigger(DIK_P);
				}
				else
				{
					for (int nCntJoy = 0; nCntJoy < pJoypad->GetUseJoyPad(); nCntJoy++)
					{
						bPause = pJoypad->GetTrigger(CJoypad::JOYKEY_START, nCntJoy);

						if (bPause)
						{
							break;
						}
					}
				}

				if (bPause)
				{
					SetPause(false);
				}
			}

			if (!m_bPressEnter
				&& m_nCntFrame >= 60)
			{
				m_bPressEnter = true;
				m_nCntFrame = 0;

				switch (m_nextMode)
				{
				case MODE_RETURN:
					SetPause(false);
					break;

				case MODE_GAME:
					SetPause(false);
					CApplication::SetNextMode(CApplication::MODE_GAME);
					break;

				case MODE_TITLE:
					SetPause(false);
					CApplication::SetNextMode(CApplication::MODE_TITLE);
					break;

				default:
					assert(false);
					break;
				}
			}
		}
		else if (!m_bPause)
		{
			if (!bJoypad)
			{
				bPause = pKeyboard->GetTrigger(DIK_P);
			}
			else
			{
				for (int nCntJoy = 0; nCntJoy < pJoypad->GetUseJoyPad(); nCntJoy++)
				{
					bPause = pJoypad->GetTrigger(CJoypad::JOYKEY_START, nCntJoy);

					if (bPause)
					{
						break;
					}
				}
			}

			if (bPause)
			{
				SetPause(true);
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE);
			}
		}
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CPause::Draw()
{

}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CPause::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CPause::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CPause::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;
}

//=============================================================================
// ポーズのセッター
// Author : 唐﨑結斗
// 概要 : ポーズの設定を行う
//=============================================================================
void CPause::SetPause(const bool bPause)
{
	m_bPause = bPause;

	if (m_bPause)
	{
		// 背景オブジェクト
		m_pBGObj = CObject2D::Create();
		D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vec.x = (float)CRenderer::SCREEN_WIDTH;
		vec.y = (float)CRenderer::SCREEN_HEIGHT;

		m_pBGObj->SetPos(vec * 0.5f);
		m_pBGObj->SetSize(vec);
		m_pBGObj->LoadTex(-1);
		m_pBGObj->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
		m_pBGObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// ポーズ背景オブジェクト
		m_pPauseBGObj = CObject2D::Create();
		m_pPauseBGObj->SetSize(m_size);
		m_pPauseBGObj->SetPos(m_pos);
		m_pPauseBGObj->LoadTex(7);
		m_pPauseBGObj->SetCol(BACK_COLOR);
		m_pPauseBGObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// リターンオブジェクト
		m_pReturnObj = CObject2D::Create();
		m_pReturnObj->SetSize(D3DXVECTOR3(450.0f, 125.0f, 0.0f));
		m_pReturnObj->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y - m_pReturnObj->GetSize().y - 50.0f, 0.0f));
		m_pReturnObj->LoadTex(9);
		m_pReturnObj->SetCol(SELECT_COLOR);
		m_pReturnObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// ニューゲームオブジェクト
		m_pNewGameObj = CObject2D::Create();
		m_pNewGameObj->SetPos(m_pos);
		m_pNewGameObj->SetSize(D3DXVECTOR3(450.0f, 125.0f, 0.0f));
		m_pNewGameObj->LoadTex(10);
		m_pNewGameObj->SetCol(SELECT_COLOR);
		m_pNewGameObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// タイトルオブジェクト
		m_pTitleObj = CObject2D::Create();
		m_pTitleObj->SetSize(D3DXVECTOR3(450.0f, 125.0f, 0.0f));
		m_pTitleObj->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + m_pTitleObj->GetSize().y + 50.0f, 0.0f));
		m_pTitleObj->LoadTex(11);
		m_pTitleObj->SetCol(SELECT_COLOR);
		m_pTitleObj->SetObjType(CObject::OBJTYPE_PAUSE);
	}
	else
	{
		// 背景オブジェクト
		m_pBGObj->Uninit();

		// ポーズ背景オブジェクト
		m_pPauseBGObj->Uninit();

		// リターンオブジェクト
		m_pReturnObj->Uninit();

		// ニューゲームオブジェクト
		m_pNewGameObj->Uninit();

		// タイトルオブジェクト
		m_pTitleObj->Uninit();
	}

	m_nextMode = MODE_RETURN;
	m_fCycle = 0.025f;
	CObject::SetPause(m_bPause);
}

//=============================================================================
// オブジェクトの点滅
// Author : 唐﨑結斗
// 概要 : 指定のオブジェクトを点滅させる
//=============================================================================
void CPause::FlashObj()
{
	CObject2D *pObj = nullptr;

	switch (m_nextMode)
	{
	case MODE_RETURN:
		pObj = m_pReturnObj;
		m_pNewGameObj->SetCol(BASE_COLOR);
		m_pTitleObj->SetCol(BASE_COLOR);
		break;

	case MODE_GAME:
		pObj = m_pNewGameObj;
		m_pReturnObj->SetCol(BASE_COLOR);
		m_pTitleObj->SetCol(BASE_COLOR);
		break;

	case MODE_TITLE:
		pObj = m_pTitleObj;
		m_pReturnObj->SetCol(BASE_COLOR);
		m_pNewGameObj->SetCol(BASE_COLOR);
		break;

	default:
		assert(false);
		break;
	}

	m_nCntFrame++;
	float sinCurve = (sinf((m_nCntFrame * m_fCycle) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
	float alpha = 1.0f - (sinCurve * 0.7f);

	pObj->SetCol(D3DXCOLOR(SELECT_COLOR.r, SELECT_COLOR.g, SELECT_COLOR.b, alpha));
}

//=============================================================================
// モードの選択
// Author : 唐﨑結斗
// 概要 : モードの選択する
//=============================================================================
void CPause::SelectMode()
{
	int nMode = (int)m_nextMode;

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	CJoypad *pJoypad = CApplication::GetJoy();
	bool bJoypad = false;

	if (pJoypad->GetUseJoyPad() > 0)
	{
		bJoypad = true;
	}

	if (!bJoypad)
	{
		if (pKeyboard->GetTrigger(DIK_W))
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
			nMode--;

			if (nMode < 0)
			{
				nMode = 2;
			}
		}
		else if (pKeyboard->GetTrigger(DIK_S))
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
			nMode++;

			if (nMode > 2)
			{
				nMode = 0;
			}
		}
	}
	else
	{
		for (int nCntJoy = 0; nCntJoy < pJoypad->GetUseJoyPad(); nCntJoy++)
		{
			if (pJoypad->GetTrigger(CJoypad::JOYKEY_UP, nCntJoy) ||
				pJoypad->GetStickTrigger(CJoypad::JOYKEY_UP, false, nCntJoy))
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
				nMode--;

				if (nMode < 0)
				{
					nMode = 2;
				}
			}
			else if (pJoypad->GetTrigger(CJoypad::JOYKEY_DOWN, nCntJoy) ||
				pJoypad->GetStickTrigger(CJoypad::JOYKEY_DOWN, false, nCntJoy))
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
				nMode++;

				if (nMode > 2)
				{
					nMode = 0;
				}
			}
		}
	}
	
	m_nextMode = (NEXT_MODE)nMode;
}

