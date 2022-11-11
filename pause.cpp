//=============================================================================
//
// �|�[�Y�N���X(pause.cpp)
// Author : �������l
// �T�v : �|�[�Y�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
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
// �萔��`
//--------------------------------------------------------------------
const D3DXCOLOR CPause::BASE_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �x�[�X�F
const D3DXCOLOR CPause::SELECT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �Z���N�g�F
const D3DXCOLOR CPause::BACK_COLOR = D3DXCOLOR(0.7f, 0.2f, 0.2f, 0.5f);		// �w�i�F

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : �X�R�A�𐶐�����
//=============================================================================
CPause *CPause::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CPause *pPause = nullptr;

	pPause = new CPause;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pPause != nullptr);

	// ���l�̏�����
	pPause->Init();

	// �C���X�^���X��Ԃ�
	return pPause;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CPause::CPause(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_nextMode = MODE_RETURN;							// ���̃��[�h
	m_pBGObj = nullptr;									// �w�i�I�u�W�F�N�g
	m_pPauseBGObj = nullptr;							// �|�[�Y�w�i�I�u�W�F�N�g
	m_pReturnObj = nullptr;								// ���^�[���I�u�W�F�N�g
	m_pNewGameObj = nullptr;							// �j���[�Q�[���I�u�W�F�N�g
	m_pTitleObj = nullptr;								// �^�C�g���I�u�W�F�N�g
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ߋ��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �傫��
	m_fCycle = 0.0f;									// �J�[�u�̎���
	m_nCntFrame = 0;									// �t���[���J�E���g
	m_bPressEnter = true;								// �G���^�[�L�[�������邩��
	m_bPause = false;									// �|�[�Y���Ă��邩
	m_bSelect = true;									// �I�����Ă�����
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
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
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CPause::Uninit()
{
	// �X�R�A�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CPause::Update()
{
	if (!m_bSelect)
	{// �I�����Ă͂����Ȃ�
		return;
	}

	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// ���͏��̎擾
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
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CPause::Draw()
{

}

//=============================================================================
// �ʒu�̃Z�b�^�[
// Author : �������l
// �T�v : �ʒu�̃����o�ϐ��Ɉ�������
//=============================================================================
void CPause::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//=============================================================================
// �����̃Z�b�^�[
// Author : �������l
// �T�v : �����̃����o�ϐ��Ɉ�������
//=============================================================================
void CPause::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;
}

//=============================================================================
// �傫���̃Z�b�^�[
// Author : �������l
// �T�v : �傫���̃����o�ϐ��Ɉ�������
//=============================================================================
void CPause::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;
}

//=============================================================================
// �|�[�Y�̃Z�b�^�[
// Author : �������l
// �T�v : �|�[�Y�̐ݒ���s��
//=============================================================================
void CPause::SetPause(const bool bPause)
{
	m_bPause = bPause;

	if (m_bPause)
	{
		// �w�i�I�u�W�F�N�g
		m_pBGObj = CObject2D::Create();
		D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vec.x = (float)CRenderer::SCREEN_WIDTH;
		vec.y = (float)CRenderer::SCREEN_HEIGHT;

		m_pBGObj->SetPos(vec * 0.5f);
		m_pBGObj->SetSize(vec);
		m_pBGObj->LoadTex(-1);
		m_pBGObj->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
		m_pBGObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// �|�[�Y�w�i�I�u�W�F�N�g
		m_pPauseBGObj = CObject2D::Create();
		m_pPauseBGObj->SetSize(m_size);
		m_pPauseBGObj->SetPos(m_pos);
		m_pPauseBGObj->LoadTex(7);
		m_pPauseBGObj->SetCol(BACK_COLOR);
		m_pPauseBGObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// ���^�[���I�u�W�F�N�g
		m_pReturnObj = CObject2D::Create();
		m_pReturnObj->SetSize(D3DXVECTOR3(450.0f, 125.0f, 0.0f));
		m_pReturnObj->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y - m_pReturnObj->GetSize().y - 50.0f, 0.0f));
		m_pReturnObj->LoadTex(9);
		m_pReturnObj->SetCol(SELECT_COLOR);
		m_pReturnObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// �j���[�Q�[���I�u�W�F�N�g
		m_pNewGameObj = CObject2D::Create();
		m_pNewGameObj->SetPos(m_pos);
		m_pNewGameObj->SetSize(D3DXVECTOR3(450.0f, 125.0f, 0.0f));
		m_pNewGameObj->LoadTex(10);
		m_pNewGameObj->SetCol(SELECT_COLOR);
		m_pNewGameObj->SetObjType(CObject::OBJTYPE_PAUSE);

		// �^�C�g���I�u�W�F�N�g
		m_pTitleObj = CObject2D::Create();
		m_pTitleObj->SetSize(D3DXVECTOR3(450.0f, 125.0f, 0.0f));
		m_pTitleObj->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + m_pTitleObj->GetSize().y + 50.0f, 0.0f));
		m_pTitleObj->LoadTex(11);
		m_pTitleObj->SetCol(SELECT_COLOR);
		m_pTitleObj->SetObjType(CObject::OBJTYPE_PAUSE);
	}
	else
	{
		// �w�i�I�u�W�F�N�g
		m_pBGObj->Uninit();

		// �|�[�Y�w�i�I�u�W�F�N�g
		m_pPauseBGObj->Uninit();

		// ���^�[���I�u�W�F�N�g
		m_pReturnObj->Uninit();

		// �j���[�Q�[���I�u�W�F�N�g
		m_pNewGameObj->Uninit();

		// �^�C�g���I�u�W�F�N�g
		m_pTitleObj->Uninit();
	}

	m_nextMode = MODE_RETURN;
	m_fCycle = 0.025f;
	CObject::SetPause(m_bPause);
}

//=============================================================================
// �I�u�W�F�N�g�̓_��
// Author : �������l
// �T�v : �w��̃I�u�W�F�N�g��_�ł�����
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
// ���[�h�̑I��
// Author : �������l
// �T�v : ���[�h�̑I������
//=============================================================================
void CPause::SelectMode()
{
	int nMode = (int)m_nextMode;

	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// ���͏��̎擾
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

