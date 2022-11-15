///=============================================================================
//
// �^�C���N���X(time.cpp)
// Author : �������l
// �T�v : �^�C���̐ݒ���s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>
#include <math.h>

#include "time.h"
#include "object.h"
#include "object2D.h"
#include "score.h"
#include "renderer.h"
#include "application.h"
#include "texture.h"
#include "game.h"
#include "player.h"
#include "obstacle_manager.h"
#include "pause.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : �X�R�A�𐶐�����
//=============================================================================
CTime *CTime::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CTime *pTime = nullptr;

	pTime = new CTime;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pTime != nullptr);

	// ���l�̏�����
	pTime->Init();

	// �C���X�^���X��Ԃ�
	return pTime;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CTime::CTime(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_pScore = nullptr;									// �X�R�A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ߋ��̈ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �傫��
	m_nTime = 0;										// ����
	m_nCntFrame = 0;									// �t���[���J�E���g
	m_bStop = false;									// ��~����
	m_bCountDown = false;								// �J�E���g�_�E��
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CTime::~CTime()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CTime::Init()
{
	// �X�R�A�̐���
	m_pScore = CScore::Create(10, true);
	assert(m_pScore != nullptr);
	m_pScore->SetScore(0);

	// �傫���̐ݒ�
	m_pScore->SetWholeSize(D3DXVECTOR3(1000.0f, 50.0f, 0.0f));
	m_pScore->SetSize(D3DXVECTOR3(50.0f, 75.0f, 0.0f));

	// �ʒu�̐ݒ�
	SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	// �����̐ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CTime::Uninit()
{
	m_pScore->Uninit();

	// �C���X�^���X�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CTime::Update()
{
	if (m_bCountDown)
	{
		m_nCntFrame++;

		if (m_nCntFrame % 60 == 0)
		{// ���Ԃ̐ݒ�

			m_pScore->AddScore(-1);

			// ���Ԃ̎擾
			m_nTime--;

			if (m_nTime <= 0)
			{// �������Ԃ�����
				Uninit();
				CPlayer** pPlayer = CGame::GetPlayer();

				int max = CApplication::GetPersonCount();

				for (int i = 0; i < max; i++)
				{
					pPlayer[i]->SetAction(true);
				}

				// ���Ԃ��i��
				CGame::GetTime()->StopTime(false);

				// �o������
				CGame::GetObstacle()->SetStop(false);

				// �|�[�Y�ł���
				CApplication::GetPause()->SetSelect(true);

				return;
			}
		}

		return;
	}

	if (m_nTime >= 0 && !m_bStop)
	{// �t���[���J�E���g
		int nPlayer = CApplication::GetPersonCount();

		if (nPlayer > 1)
		{// �}���`�v���C
			m_nCntFrame++;

			if (m_nCntFrame % 60 == 0)
			{// ���Ԃ̐ݒ�

				m_pScore->AddScore(-1);

				// ���Ԃ̎擾
				m_nTime--;

				if (m_nTime <= 0)
				{// �������Ԃ�����
					m_bStop = true;
					m_nTime = 0;
					m_pScore->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CGame::SetGame(false);

					// �o������
					CGame::GetObstacle()->SetStop(true);

					// �|�[�Y�ł��Ȃ�����
					CApplication::GetPause()->SetSelect(false);
				}

			}

			if (m_nTime <= 5)
			{// �}���`�v���C�Ő������Ԃ܂Ō㏭��
				float sinCurve = (sinf((m_nCntFrame * 0.05f) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
				float alpha = (sinCurve * 0.75f) + 0.25f;

				// �F�̐ݒ�
				m_pScore->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, alpha));
			}
		}
		else
		{// �V���O���v���C
			m_pScore->AddScore(13);
			
			// ���Ԃ̎擾
			m_nTime += 13;
		}
	}	
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CTime::Draw()
{
	
}

//=============================================================================
// �ʒu�̃Z�b�^�[
// Author : �������l
// �T�v : �ʒu�̃����o�ϐ��Ɉ�������
//=============================================================================
void CTime::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	m_pScore->SetPos(m_pos);
}

//=============================================================================
// �����̃Z�b�^�[
// Author : �������l
// �T�v : �����̃����o�ϐ��Ɉ�������
//=============================================================================
void CTime::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;

	m_pScore->SetRot(m_rot);
}

//=============================================================================
// �傫���̃Z�b�^�[
// Author : �������l
// �T�v : �傫���̃����o�ϐ��Ɉ�������
//=============================================================================
void CTime::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	m_pScore->SetWholeSize(D3DXVECTOR3(m_size.x, size.y / 2.0f, size.z));
	m_pScore->SetSize(D3DXVECTOR3(m_size.x / 2.5f, size.y / 2.5f, size.z));
}

//=============================================================================
// �i���o�[�̃Z�b�^�[
// Author : �������l
// �T�v : �i���o�[�̃����o�ϐ��Ɉ�������
//=============================================================================
void CTime::SetTime(const int nTime)
{// ���Ԃ̐ݒ�
	m_nTime = nTime;

	// �X�R�A�̐ݒ�
	m_pScore->SetScore(m_nTime);
	m_pScore->SetDestScore(m_nTime);
	m_pScore->AddDigit();
}

//=============================================================================
// �J�E���g�_�E���̃Z�b�^�[
// Author : �������P
// �T�v : �J�E���g�_�E��������
//=============================================================================
void CTime::SetCountDown()
{
	m_bCountDown = true;

	m_nTime = 3;

	m_pScore->SetScore(3);
	m_pScore->SetDestScore(3);
	m_pScore->AddDigit();

	// �傫���̐ݒ�
	m_pScore->SetWholeSize(D3DXVECTOR3(1000.0f, 50.0f, 0.0f));
	m_pScore->SetSize(D3DXVECTOR3(200.0f, 400.0f, 0.0f));

	// �ʒu�̐ݒ�
	SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	// �e�N�X�`���̐ݒ�
	m_pScore->SetTexture(16);

	CPlayer** pPlayer = CGame::GetPlayer();

	int max = CApplication::GetPersonCount();

	for (int i = 0; i < max; i++)
	{
		pPlayer[i]->SetAction(false);
	}

	// ���Ԃ̒�~
	CGame::GetTime()->StopTime(true);

	// �o���̒�~
	CGame::GetObstacle()->SetStop(true);

	// �|�[�Y�ł��Ȃ�����
	CApplication::GetPause()->SetSelect(false);
}
