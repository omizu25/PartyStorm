//=============================================================================
//
// �v���C���[�N���X(player.cpp)
// Author : �������l
// �T�v : �v���C���[�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
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
// �C���X�^���X����
// Author : �������l
// �T�v : ���[�V�����L�����N�^�[3D�𐶐�����
//=============================================================================
CPlayer * CPlayer::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CPlayer *pPlayer = nullptr;

	// �������̉��
	pPlayer = new CPlayer;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pPlayer != nullptr);

	// ���l�̏�����
	pPlayer->Init();

	// �C���X�^���X��Ԃ�
	return pPlayer;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CPlayer::CPlayer() : m_pMove(nullptr),
m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_fSpeed(0.0f),
m_nNumMotion(0),
m_bDead(false)
{
#ifdef _DEBUG
	// ���C�����
	m_pLine = nullptr;
#endif // _DEBUG
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CPlayer::Init()
{
	// ������
	CMotionModel3D::Init();

	// �ړ��N���X�̃������m��
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// �����蔻��̐ݒ�
	SetColisonPos(D3DXVECTOR3(0.0f, 25.0f, 0.0f));
	SetColisonSize(D3DXVECTOR3(20.0f, 50.0f, 20.0f));

	// �J�����̒Ǐ]�ݒ�(�ڕW : �v���C���[)
	CApplication::GetCamera()->SetFollowTarget(this, D3DXVECTOR3(0.0f, 50.0f, 0.0f), 300.0f, 0.5f);

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetObjType(OBJTYPE_3DPLAYER);

#ifdef _DEBUG
	// ���C�����
	m_pLine = new CLine*[12];

	for (int nCntLine = 0; nCntLine < 12; nCntLine++)
	{
		m_pLine[nCntLine] = CLine::Create();
	}

	// ���C���̐ݒ�
	SetLine();
#endif // _DEBUG

	return E_NOTIMPL;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CPlayer::Uninit()
{
	if (m_pMove != nullptr)
	{// �I������
	 // �������̉��
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

	// �I��
	CMotionModel3D::Uninit();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CPlayer::Update()
{
	if (!m_bDead)
	{// �L�[�{�[�h�̎擾
		CKeyboard *pKeyboard = CApplication::GetKeyboard();

		// ���[�V�������̎擾
		CMotion *pMotion = CMotionModel3D::GetMotion();

		// �ʒu�̎擾
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();
		SetPosOld(pos);

		// �U��
		if (pKeyboard->GetTrigger(DIK_RETURN)
			&& pMotion != nullptr)
		{
			pMotion->SetNumMotion(ATTACK_ACTION);
			m_EAction = ATTACK_ACTION;
		}

		// �ړ�
		pos += Move();

		//pos.y -= CCalculation::Gravity();

		// ��]
		Rotate();

		// �j���[�g�������[�V�����̐ݒ�
		if (pMotion != nullptr
			&& !pMotion->GetMotion())
		{
			pMotion->SetNumMotion(NEUTRAL_ACTION);
			m_EAction = NEUTRAL_ACTION;
		}

		// �ʒu�̐ݒ�
		SetPos(pos);

		// �����蔻��
		Collison();

		// ���b�V���̓����蔻��
		CMesh3D *pMesh = CGame::GetMesh();

		if (pMesh != nullptr)
		{
			pMesh->Collison(this);
		}

		// �ʒu�̎擾
		pos = GetPos();

		// �X�V
		CMotionModel3D::Update();

#ifdef _DEBUG
		// �f�o�b�N�\��
		CDebugProc::Print("�v���C���[�̈ʒu | X : %.3f | Y : %.3f | Z : %.3f |\n", pos.x, pos.y, pos.z);

		// ���C���̍X�V
		SetLine();
#endif // _DEBUG
	}
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CPlayer::Draw()
{
	if (!m_bDead)
	{// �`��
		CMotionModel3D::Draw();
	}
}

//=============================================================================
// �ړ�
// Author : �������l
// �T�v : �L�[���͂ŕ��������߂āA�ړ��x�N�g�����Z�o����
//=============================================================================
D3DXVECTOR3 CPlayer::Move()
{
	// �ϐ��錾
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// �L�[�{�[�h�̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	//�R���g���[���[
	CJoypad *pJoy = CApplication::GetJoy();

	// �ړ������̎擾
	if (pJoy->Stick(CJoypad::JOYKEY_LEFT_STICK, m_nNum, 0.5f))
	{// �����̌v�Z
		m_rotDest.y = pJoy->GetStickAngle(CJoypad::JOYKEY_LEFT_STICK, m_nNum);

		// �J�������̎擾
		CCamera *pCamera = CApplication::GetCamera();

		// �ړ������̎Z�o
		m_rotDest.y += pCamera->GetRot().y;

		// �ړ������̐��K��
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		// �ړ��ʂ̌v�Z
		move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

		// �p�x�̐��K��
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
	{// �ړ��L�[�������ꂽ
		if (pJoy->GetPress(CJoypad::JOYKEY_UP, m_nNum))
		{// [W]�L�[�������ꂽ��
			if (pJoy->GetPress(CJoypad::JOYKEY_LEFT, m_nNum))
			{// [A]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * -0.25f;
			}
			else if (pJoy->GetPress(CJoypad::JOYKEY_DOWN, m_nNum))
			{// [D]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{// �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.0f;
			}
		}
		else if (pJoy->GetPress(CJoypad::JOYKEY_DOWN, m_nNum))
		{// [S]�L�[�������ꂽ��
			if (pJoy->GetPress(CJoypad::JOYKEY_LEFT, m_nNum))
			{// [A]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * -0.75f;
			}
			else if (pJoy->GetPress(CJoypad::JOYKEY_RIGHT, m_nNum))
			{// [D]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.75f;
			}
			else
			{// �ړ������̍X�Vq
				m_rotDest.y = D3DX_PI;
			}
		}
		else if (pJoy->GetPress(CJoypad::JOYKEY_LEFT, m_nNum))
		{// [A]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotDest.y = D3DX_PI * -0.5f;
		}
		else if (pJoy->GetPress(CJoypad::JOYKEY_RIGHT, m_nNum))
		{// [D]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotDest.y = D3DX_PI * 0.5f;
		}

		// �J�������̎擾
		CCamera *pCamera = CApplication::GetCamera();

		// �ړ������̎Z�o
		m_rotDest.y += pCamera->GetRot().y;

		// �ړ������̐��K��
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		// �ړ��ʂ̌v�Z
		move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

		// �p�x�̐��K��
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


	//�L�[�{�[�h
	if (pKeyboard->GetPress(DIK_W)
		|| pKeyboard->GetPress(DIK_A)
		|| pKeyboard->GetPress(DIK_D)
		|| pKeyboard->GetPress(DIK_S))
	{// �ړ��L�[�������ꂽ
		if (pKeyboard->GetPress(DIK_W))
		{// [W]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A))
			{// [A]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [D]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.25f;
			}
			else
			{// �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S))
		{// [S]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A))
			{// [A]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// [D]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotDest.y = D3DX_PI * 0.75f;
			}
			else
			{// �ړ������̍X�Vq
				m_rotDest.y = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A))
		{// [A]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotDest.y = D3DX_PI * -0.5f;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{// [D]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotDest.y = D3DX_PI * 0.5f;
		}

		// �J�������̎擾
		CCamera *pCamera = CApplication::GetCamera();

		// �ړ������̎Z�o
		m_rotDest.y += pCamera->GetRot().y;

		// �ړ������̐��K��
		m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);

		// �ړ��ʂ̌v�Z
		move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));

		// �p�x�̐��K��
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

	// �ړ����̌v�Z
	m_pMove->Moving(move);

	// �ړ����̎擾
	D3DXVECTOR3 moveing = m_pMove->GetMove();

	if (sqrtf((moveing.x * moveing.x) + (moveing.z * moveing.z)) <= 0.0f
		&& pMotion != nullptr
		&& m_EAction == MOVE_ACTION)
	{
		m_EAction = NEUTRAL_ACTION;
		pMotion->SetNumMotion(NEUTRAL_ACTION);
	}

	// �f�o�b�N�\��
	CDebugProc::Print("�ړ��x�N�g�� : %.3f\n", m_pMove->GetMoveLength());

	// �����̎擾
	D3DXVECTOR3 rot = GetRot();

	// �ړI�̌����̕␳
	if (m_rotDest.y - rot.y >= D3DX_PI)
	{// �ړ������̐��K��
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - rot.y <= -D3DX_PI)
	{// �ړ������̐��K��
		m_rotDest.y += D3DX_PI * 2;
	}

	return moveing;
}

//=============================================================================
// ��]
// Author : �������l
// �T�v : �ړI�̌����܂ŉ�]����
//=============================================================================
void CPlayer::Rotate()
{
	// �����̎擾
	D3DXVECTOR3 rot = GetRot();

	// �����̍X�V
	rot.y += (m_rotDest.y - rot.y) * 0.5f;

	// �����̐��K��
	rot.y = CCalculation::RotNormalization(rot.y);

	// �����̐ݒ�
	SetRot(rot);
}

//=============================================================================
// �����蔻��
// Author : �������l
// �T�v : �����蔻����s��
//=============================================================================
void CPlayer::Collison()
{
	// �v���C���[���̎擾
	D3DXVECTOR3 pos = GetPos() + GetColisonPos();
	D3DXVECTOR3 size = GetColisonSize();

	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		CObject *pTop = CObject::GetTop(nCntPriority);

		if (pTop != nullptr)
		{// �ϐ��錾
			CObject *pObject = pTop;

			while (pObject)
			{// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ۊ�
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
						{// �v���C���[�̈ړ�
							CPlayer *pPlayer = dynamic_cast<CPlayer*>(pObject);
							CMove *pMove = GetMove();
							CMove *pMoveTarget = pPlayer->GetMove();
							D3DXVECTOR3 pos = pPlayer->GetPos();
							pos += pMove->GetMove() - pMoveTarget->GetMove();
							pPlayer->SetPos(pos);
						}	
						if (pObject->GetObjType() == OBJTYPE_3DENEMY)
						{// �G�Ƃ̓����蔻��
							m_bDead = true;
						}
					}
				}

				// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���X�V
				pObject = pObjectNext;
			}
		}
	}
}

#ifdef _DEBUG
//=============================================================================
// ���C���̐ݒu
// Author : �������l
// �T�v : ���C������`��ɐݒu
//=============================================================================
void CPlayer::SetLine()
{
	// �ϐ��錾
	const D3DXVECTOR3 pos = GetPos() + GetColisonPos();
	const D3DXVECTOR3 rot = GetRot();
	const D3DXVECTOR3 size = GetColisonSize() / 2.0f;
	const D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// ���C���̐ݒ�
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


