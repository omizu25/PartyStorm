//**************************************************
// 
// menu.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "menu.h"
#include "object2D.h"
#include "input.h"
#include "keyboard.h"
#include "application.h"
#include "sound.h"
#include "joypad.h"
#include <assert.h>

//==================================================
// 定義
//==================================================
const float CMenu::CURSOR_INTERVAL = 0.0f;

//--------------------------------------------------
// 生成
//--------------------------------------------------
CMenu* CMenu::Create()
{
	CMenu* pMenu = nullptr;

	pMenu = new CMenu;

	if (pMenu != nullptr)
	{// nullチェック
		// 初期化
		pMenu->Init();
	}

	return pMenu;
}

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CMenu::CMenu() :
	m_pOption(nullptr),
	m_pFrame(nullptr),
	m_pCursor(nullptr),
	m_selectIdx(0),
	m_numUse(0),
	m_time(0),
	m_cycle(0.0f),
	m_sort(false)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CMenu::~CMenu()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CMenu::Init()
{
	m_pOption = nullptr;
	m_pFrame = nullptr;
	m_pCursor = nullptr;
	m_numUse = 0;
	m_selectIdx = 0;
	m_time = 0;
	m_cycle = 0.025f;
	m_sort = false;

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CMenu::Uninit()
{
	for (int i = 0; i < m_numUse; i++)
	{
		if (m_pOption[i] != nullptr)
		{// nullチェック
			// 終了
			m_pOption[i]->Uninit();
			m_pOption[i] = nullptr;
		}
	}

	delete[] m_pOption;
	m_pOption = nullptr;

	if (m_pFrame != nullptr)
	{// nullチェック
		// 終了
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}

	if (m_pCursor != nullptr)
	{// nullチェック
		// 終了
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CMenu::Update()
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CMenu::Draw()
{
}

//--------------------------------------------------
// 選択
//--------------------------------------------------
int CMenu::Select()
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	CJoypad *pJoypad = CApplication::GetJoy();
	bool bJoypad = false;

	if (pJoypad->GetUseJoyPad() > 0)
	{
		bJoypad = true;
	}

	if (m_sort)
	{// 縦
		if (!bJoypad)
		{
			if (pKeyboard->GetTrigger(DIK_W))
			{// 上キーが押された
				Add(-1);
			}
			if (pKeyboard->GetTrigger(DIK_S))
			{// 下キーが押された
				Add(1);
			}
		}
		else
		{
			for (int nCntPlayer = 0; nCntPlayer < pJoypad->GetUseJoyPad(); nCntPlayer++)
			{
				if (pJoypad->GetTrigger(CJoypad::JOYKEY_UP, nCntPlayer) ||
					pJoypad->GetStickTrigger(CJoypad::JOYKEY_UP, false, nCntPlayer))
				{// 上キーが押された
					Add(-1);
				}
				if (pJoypad->GetTrigger(CJoypad::JOYKEY_DOWN, nCntPlayer) ||
					pJoypad->GetStickTrigger(CJoypad::JOYKEY_DOWN, false, nCntPlayer))
				{// 下キーが押された
					Add(1);
				}
			}
		}
	}
	else
	{// 横
		if (!bJoypad)
		{
			if (pKeyboard->GetTrigger(DIK_A))
			{// 左キーが押された
				Add(-1);
			}
			if (pKeyboard->GetTrigger(DIK_D))
			{// 右キーが押された
				Add(1);
			}
		}
		else
		{
			for (int nCntPlayer = 0; nCntPlayer < pJoypad->GetUseJoyPad(); nCntPlayer++)
			{
				if (pJoypad->GetTrigger(CJoypad::JOYKEY_LEFT, nCntPlayer) ||
					pJoypad->GetStickTrigger(CJoypad::JOYKEY_LEFT, false, nCntPlayer))
				{// 左キーが押された
					Add(-1);
				}
				if (pJoypad->GetTrigger(CJoypad::JOYKEY_RIGHT, nCntPlayer) ||
					pJoypad->GetStickTrigger(CJoypad::JOYKEY_RIGHT, false, nCntPlayer))
				{// 右キーが押された
					Add(1);
				}
			}
		}
	}

	// 色の変更
	ColorChange();

	if (!bJoypad)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// 決定キーが押された
			// SE
			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
			m_cycle = 0.1f;

			return m_selectIdx;
		}
	}
	else
	{
		for (int nCntPlayer = 0; nCntPlayer < pJoypad->GetUseJoyPad(); nCntPlayer++)
		{
			if (pJoypad->GetTrigger(CJoypad::JOYKEY_B, nCntPlayer)
				|| pJoypad->GetTrigger(CJoypad::JOYKEY_A, nCntPlayer))
			{// 決定キーが押された
			 // SE
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
				m_cycle = 0.1f;
				return m_selectIdx;
			}
		}
	}

	return -1;
}

//--------------------------------------------------
// 色の変更
//--------------------------------------------------
void CMenu::ColorChange()
{
	m_time++;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	float sinCurve = (sinf((m_time * m_cycle) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
	col.a = 1.0f - (sinCurve * 0.7f);

	// 色の設定
	m_pOption[m_selectIdx]->SetCol(col);
}

//--------------------------------------------------
// フレームの設定
//--------------------------------------------------
void CMenu::SetFrame(const D3DXVECTOR3& size, const D3DXCOLOR& col)
{
	// サイズの設定
	m_pFrame->SetSize(size);

	// 色の設定
	m_pFrame->SetCol(col);
}

//--------------------------------------------------
// テクスチャの設定
//--------------------------------------------------
void CMenu::SetTexture(int idx, int texIdx)
{
	if (idx == -1)
	{// 指定の値
		m_pFrame->LoadTex(texIdx);
		return;
	}

	assert(idx >= 0 && idx < m_numUse);

	if (m_pOption[idx] != nullptr)
	{// nullチェック
		m_pOption[idx]->LoadTex(texIdx);
	}
}

//--------------------------------------------------
// 加算
//--------------------------------------------------
void CMenu::Add(int add)
{
	// 色の設定
	m_pOption[m_selectIdx]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_selectIdx = ((m_selectIdx + add) + m_numUse) % m_numUse;

	m_time = 0;

	// SE
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);

	if (m_pCursor != nullptr)
	{// nullチェック
		D3DXVECTOR3 pos = m_pOption[m_selectIdx]->GetPos();
		D3DXVECTOR3 optionSize = m_pOption[m_selectIdx]->GetSize();
		D3DXVECTOR3 cursorSize = m_pCursor->GetSize();
		pos.x -= ((optionSize.x * 0.5f) + (cursorSize.x * 0.5f) + CURSOR_INTERVAL);

		// 位置の設定
		m_pCursor->SetPos(pos);
	}
}

//--------------------------------------------------
// 選択されている番号の取得
//--------------------------------------------------
int CMenu::GetSelectIdx()
{
	return m_selectIdx;
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void CMenu::Set(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, int numUse, float interval, bool sort, bool cursor)
{
	m_numUse = numUse;
	m_sort = sort;
	m_selectIdx = 0;
	m_time = 0;

	// 枠の作成
	m_pFrame = CObject2D::Create();

	// 位置の設定
	m_pFrame->SetPos(pos);

	D3DXVECTOR3 optionPos = pos;

	if (m_sort)
	{// 縦
		optionPos.y -= ((m_numUse - 1) * ((size.y * 0.5f) + (interval * 0.5f)));
	}
	else
	{// 横
		optionPos.x -= ((m_numUse - 1) * ((size.x * 0.5f) + (interval * 0.5f)));
	}

	if (cursor)
	{// カーソルを使用する
	 // カーソルの作成
		m_pCursor = CObject2D::Create();

		float cursorSize = size.y * 0.8f;
		D3DXVECTOR3 cursorPos = optionPos;
		cursorPos.x -= ((cursorSize * 0.5f) + (size.x * 0.5f) + CURSOR_INTERVAL);

		// 位置の設定
		m_pCursor->SetPos(cursorPos);

		// サイズの設定
		m_pCursor->SetSize(D3DXVECTOR3(cursorSize, cursorSize, 0.0f));

		// テクスチャの設定
		m_pCursor->LoadTex(29);
	}
	else
	{
		m_pCursor = nullptr;
	}

	m_pOption = new CObject2D*[m_numUse];

	for (int i = 0; i < m_numUse; i++)
	{
		// 選択肢の作成
		m_pOption[i] = CObject2D::Create();

		// 位置の設定
		m_pOption[i]->SetPos(optionPos);

		// サイズの設定
		m_pOption[i]->SetSize(size);

		if (m_sort)
		{// 縦
			optionPos.y += size.y + interval;
		}
		else
		{// 横
			optionPos.x += size.x + interval;
		}
	}
}

//--------------------------------------------------
// 位置の設定
//--------------------------------------------------
void CMenu::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//--------------------------------------------------
// 向きの設定
//--------------------------------------------------
void CMenu::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//--------------------------------------------------
// サイズの設定
//--------------------------------------------------
void CMenu::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;
}