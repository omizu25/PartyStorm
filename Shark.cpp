//=============================================================================
//
// �G�l�~�[�N���X(Enemy.h)
// Author : ��]����
// �T�v : �v���C���[�ɏP���|����
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
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
#include "shadow.h"
#include "bullet3D.h"
#include "debug_proc.h"

CEnemyShark * CEnemyShark::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CEnemyShark *pEnemyShark = nullptr;

	// �������̉��
	pEnemyShark = new CEnemyShark;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pEnemyShark != nullptr);

	// ���l�̏�����
	pEnemyShark->Init();

	// �C���X�^���X��Ԃ�
	return pEnemyShark;
}

CEnemyShark::CEnemyShark() : m_pMove(nullptr),m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),m_fSpeed(0.0f),
m_nNumMotion(0)
{

}

CEnemyShark::~CEnemyShark()
{
}

HRESULT CEnemyShark::Init()
{
	// ������
	CMotionModel3D::Init();

	// �ړ��N���X�̃������m��
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// �e�̐ݒ�
	m_pShadow = CShadow::Create(this);
	m_pShadow->SetPos(GetPos());
	m_pShadow->SetSize(D3DXVECTOR3(20.0f, 20.0f, 0.0f));

	return S_OK;
}

void CEnemyShark::Uninit()
{
	// �e�̏I��
	m_pShadow->Uninit();

	if (m_pMove != nullptr)
	{// �I������
	 // �������̉��
		delete m_pMove;
		m_pMove = nullptr;
	}

	// �I��
	CMotionModel3D::Uninit();

}

void CEnemyShark::Update()
{
	// �L�[�{�[�h�̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// �U��
	if (pKeyboard->GetTrigger(DIK_RETURN)
		&& pMotion != nullptr)
	{
		pMotion->SetNumMotion(ATTACK_ACTION);
		m_EAction = ATTACK_ACTION;
	}

	// �ړ�
	pos += Move();

	pos.y -= CCalculation::Gravity();

	// ��]
	Rotate();

	if (pKeyboard->GetPress(DIK_SPACE))
	{// �e�̐���
		CBullet3D *pBullet3D = CBullet3D::Create();
		pBullet3D->SetPos(D3DXVECTOR3(pos.x, pos.y + 50.0f, pos.z));
		pBullet3D->SetSize(D3DXVECTOR3(10.0f, 10.0f, 0.0f));
		pBullet3D->SetMoveVec(D3DXVECTOR3(rot.x + D3DX_PI * -0.5f, rot.y, 0.0f));
		pBullet3D->SetSpeed(10.0f);
		pBullet3D->SetLife(120);
	}

	// �j���[�g�������[�V�����̐ݒ�
	if (pMotion != nullptr
		&& !pMotion->GetMotion())
	{
		pMotion->SetNumMotion(NEUTRAL_ACTION);
		m_EAction = NEUTRAL_ACTION;
	}

	// �ʒu�̐ݒ�
	SetPos(pos);

	// ���b�V���̓����蔻��
	CMesh3D *pMesh = CGame::GetMesh();
	pMesh->Collison(this);

	// �ʒu�̎擾
	pos = GetPos();

	// �f�o�b�N�\��
	CDebugProc::Print("Enemy�̈ʒu | X : %.3f | Y : %.3f | Z : %.3f |\n", pos.x, pos.y, pos.z);


	// �X�V
	CMotionModel3D::Update();
}

void CEnemyShark::Draw()
{
	// �`��
	CMotionModel3D::Draw();
}

D3DXVECTOR3 CEnemyShark::Move()
{
	// �ϐ��錾
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �L�[�{�[�h�̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

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
	//CDebugProc::Print("Enemy �ړ��x�N�g�� : %.3f\n", m_pMove->GetMoveLength());

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

void CEnemyShark::Rotate()
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
