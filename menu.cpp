//**************************************************
// 
// menu.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
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
// ��`
//==================================================
const float CMenu::CURSOR_INTERVAL = 0.0f;

//--------------------------------------------------
// ����
//--------------------------------------------------
CMenu* CMenu::Create()
{
	CMenu* pMenu = nullptr;

	pMenu = new CMenu;

	if (pMenu != nullptr)
	{// null�`�F�b�N
		// ������
		pMenu->Init();
	}

	return pMenu;
}

//--------------------------------------------------
// �f�t�H���g�R���X�g���N�^
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
// �f�X�g���N�^
//--------------------------------------------------
CMenu::~CMenu()
{
}

//--------------------------------------------------
// ������
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
// �I��
//--------------------------------------------------
void CMenu::Uninit()
{
	for (int i = 0; i < m_numUse; i++)
	{
		if (m_pOption[i] != nullptr)
		{// null�`�F�b�N
			// �I��
			m_pOption[i]->Uninit();
			m_pOption[i] = nullptr;
		}
	}

	delete[] m_pOption;
	m_pOption = nullptr;

	if (m_pFrame != nullptr)
	{// null�`�F�b�N
		// �I��
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}

	if (m_pCursor != nullptr)
	{// null�`�F�b�N
		// �I��
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CMenu::Update()
{
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CMenu::Draw()
{
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
int CMenu::Select()
{
	// ���͏��̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	CJoypad *pJoypad = CApplication::GetJoy();
	bool bJoypad = false;

	if (pJoypad->GetUseJoyPad() > 0)
	{
		bJoypad = true;
	}

	if (m_sort)
	{// �c
		if (!bJoypad)
		{
			if (pKeyboard->GetTrigger(DIK_W))
			{// ��L�[�������ꂽ
				Add(-1);
			}
			if (pKeyboard->GetTrigger(DIK_S))
			{// ���L�[�������ꂽ
				Add(1);
			}
		}
		else
		{
			for (int nCntPlayer = 0; nCntPlayer < pJoypad->GetUseJoyPad(); nCntPlayer++)
			{
				if (pJoypad->GetTrigger(CJoypad::JOYKEY_UP, nCntPlayer) ||
					pJoypad->GetStickTrigger(CJoypad::JOYKEY_UP, false, nCntPlayer))
				{// ��L�[�������ꂽ
					Add(-1);
				}
				if (pJoypad->GetTrigger(CJoypad::JOYKEY_DOWN, nCntPlayer) ||
					pJoypad->GetStickTrigger(CJoypad::JOYKEY_DOWN, false, nCntPlayer))
				{// ���L�[�������ꂽ
					Add(1);
				}
			}
		}
	}
	else
	{// ��
		if (!bJoypad)
		{
			if (pKeyboard->GetTrigger(DIK_A))
			{// ���L�[�������ꂽ
				Add(-1);
			}
			if (pKeyboard->GetTrigger(DIK_D))
			{// �E�L�[�������ꂽ
				Add(1);
			}
		}
		else
		{
			for (int nCntPlayer = 0; nCntPlayer < pJoypad->GetUseJoyPad(); nCntPlayer++)
			{
				if (pJoypad->GetTrigger(CJoypad::JOYKEY_LEFT, nCntPlayer) ||
					pJoypad->GetStickTrigger(CJoypad::JOYKEY_LEFT, false, nCntPlayer))
				{// ���L�[�������ꂽ
					Add(-1);
				}
				if (pJoypad->GetTrigger(CJoypad::JOYKEY_RIGHT, nCntPlayer) ||
					pJoypad->GetStickTrigger(CJoypad::JOYKEY_RIGHT, false, nCntPlayer))
				{// �E�L�[�������ꂽ
					Add(1);
				}
			}
		}
	}

	// �F�̕ύX
	ColorChange();

	if (!bJoypad)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// ����L�[�������ꂽ
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
			{// ����L�[�������ꂽ
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
// �F�̕ύX
//--------------------------------------------------
void CMenu::ColorChange()
{
	m_time++;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	float sinCurve = (sinf((m_time * m_cycle) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
	col.a = 1.0f - (sinCurve * 0.7f);

	// �F�̐ݒ�
	m_pOption[m_selectIdx]->SetCol(col);
}

//--------------------------------------------------
// �t���[���̐ݒ�
//--------------------------------------------------
void CMenu::SetFrame(const D3DXVECTOR3& size, const D3DXCOLOR& col)
{
	// �T�C�Y�̐ݒ�
	m_pFrame->SetSize(size);

	// �F�̐ݒ�
	m_pFrame->SetCol(col);
}

//--------------------------------------------------
// �e�N�X�`���̐ݒ�
//--------------------------------------------------
void CMenu::SetTexture(int idx, int texIdx)
{
	if (idx == -1)
	{// �w��̒l
		m_pFrame->LoadTex(texIdx);
		return;
	}

	assert(idx >= 0 && idx < m_numUse);

	if (m_pOption[idx] != nullptr)
	{// null�`�F�b�N
		m_pOption[idx]->LoadTex(texIdx);
	}
}

//--------------------------------------------------
// ���Z
//--------------------------------------------------
void CMenu::Add(int add)
{
	// �F�̐ݒ�
	m_pOption[m_selectIdx]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_selectIdx = ((m_selectIdx + add) + m_numUse) % m_numUse;

	m_time = 0;

	// SE
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);

	if (m_pCursor != nullptr)
	{// null�`�F�b�N
		D3DXVECTOR3 pos = m_pOption[m_selectIdx]->GetPos();
		D3DXVECTOR3 optionSize = m_pOption[m_selectIdx]->GetSize();
		D3DXVECTOR3 cursorSize = m_pCursor->GetSize();
		pos.x -= ((optionSize.x * 0.5f) + (cursorSize.x * 0.5f) + CURSOR_INTERVAL);

		// �ʒu�̐ݒ�
		m_pCursor->SetPos(pos);
	}
}

//--------------------------------------------------
// �I������Ă���ԍ��̎擾
//--------------------------------------------------
int CMenu::GetSelectIdx()
{
	return m_selectIdx;
}

//--------------------------------------------------
// ������
//--------------------------------------------------
void CMenu::Set(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, int numUse, float interval, bool sort, bool cursor)
{
	m_numUse = numUse;
	m_sort = sort;
	m_selectIdx = 0;
	m_time = 0;

	// �g�̍쐬
	m_pFrame = CObject2D::Create();

	// �ʒu�̐ݒ�
	m_pFrame->SetPos(pos);

	D3DXVECTOR3 optionPos = pos;

	if (m_sort)
	{// �c
		optionPos.y -= ((m_numUse - 1) * ((size.y * 0.5f) + (interval * 0.5f)));
	}
	else
	{// ��
		optionPos.x -= ((m_numUse - 1) * ((size.x * 0.5f) + (interval * 0.5f)));
	}

	if (cursor)
	{// �J�[�\�����g�p����
	 // �J�[�\���̍쐬
		m_pCursor = CObject2D::Create();

		float cursorSize = size.y * 0.8f;
		D3DXVECTOR3 cursorPos = optionPos;
		cursorPos.x -= ((cursorSize * 0.5f) + (size.x * 0.5f) + CURSOR_INTERVAL);

		// �ʒu�̐ݒ�
		m_pCursor->SetPos(cursorPos);

		// �T�C�Y�̐ݒ�
		m_pCursor->SetSize(D3DXVECTOR3(cursorSize, cursorSize, 0.0f));

		// �e�N�X�`���̐ݒ�
		m_pCursor->LoadTex(29);
	}
	else
	{
		m_pCursor = nullptr;
	}

	m_pOption = new CObject2D*[m_numUse];

	for (int i = 0; i < m_numUse; i++)
	{
		// �I�����̍쐬
		m_pOption[i] = CObject2D::Create();

		// �ʒu�̐ݒ�
		m_pOption[i]->SetPos(optionPos);

		// �T�C�Y�̐ݒ�
		m_pOption[i]->SetSize(size);

		if (m_sort)
		{// �c
			optionPos.y += size.y + interval;
		}
		else
		{// ��
			optionPos.x += size.x + interval;
		}
	}
}

//--------------------------------------------------
// �ʒu�̐ݒ�
//--------------------------------------------------
void CMenu::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//--------------------------------------------------
// �����̐ݒ�
//--------------------------------------------------
void CMenu::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//--------------------------------------------------
// �T�C�Y�̐ݒ�
//--------------------------------------------------
void CMenu::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;
}