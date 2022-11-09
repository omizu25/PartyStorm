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
#include "bullet3D.h"
#include "debug_proc.h"
#include "line.h"
#include "sound.h"
#include "follow_model.h"

//--------------------------------------------------------------------
// �萔��`
//--------------------------------------------------------------------
const D3DXVECTOR2 CEnemyShark::MAX_VIB_RAND = D3DXVECTOR2(20.0f, 50.0f);		// �U���̕�
const float CEnemyShark::VIB_SPEED = 5.0f;										// �U���̑��x
const float CEnemyShark::VIB_COEFFICIENT = 0.4f;								// �U���̌����l

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
	// ������
	CMotionModel3D::Init();

	// �ړ��N���X�̃������m��
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// �����蔻��̐ݒ�
	SetColisonPos(D3DXVECTOR3(0.0f, 250.0f, 300.0f));
	SetColisonSize(D3DXVECTOR3(500.0f, 500.0f, 2500.0f));

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetObjType(OBJTYPE_3DENEMY);

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

	return S_OK;
}

void CEnemyShark::Uninit()
{
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
	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// �j���[�g�������[�V�����̐ݒ�
	if (pMotion != nullptr
		&& !pMotion->GetMotion())
	{
		m_EAction = ATTACK_ACTION;
		pMotion->SetNumMotion(m_EAction);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_BITING);

		if (CApplication::GetMode() != CApplication::MODE_GAME)
		{
			return;
		}

		CFollowModel *pCameraTarget = CGame::GetCameraTarget();

		if (pCameraTarget != nullptr)
		{// �J�����^�[�Q�b�g���
			D3DXVECTOR3 pos = pCameraTarget->GetPos();
			D3DXVECTOR3 posDest = D3DXVECTOR3(pos.x + MAX_VIB_RAND.x - (float)(rand() % (int)(MAX_VIB_RAND.x * 2.0f)),
				pos.y + MAX_VIB_RAND.y - (float)(rand() % (int)(MAX_VIB_RAND.y * 2.0f)), 0.0f);
			pCameraTarget->SetPos(posDest);
			pCameraTarget->SetFollow(CGame::CAMERA_POSR);
			pCameraTarget->SetSpeed(VIB_SPEED);
			pCameraTarget->SetCoefficient(VIB_COEFFICIENT);
		}
	}

	// �X�V
	CMotionModel3D::Update();

#ifdef _DEBUG
	// ���C���̍X�V
	SetLine();
#endif // _DEBUG
}

void CEnemyShark::Draw()
{
	// �`��
	CMotionModel3D::Draw();
}

//D3DXVECTOR3 CEnemyShark::Move()
//{
//	// �ϐ��錾
//	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	// �L�[�{�[�h�̎擾
//	CKeyboard *pKeyboard = CApplication::GetKeyboard();
//
//	// ���[�V�������̎擾
//	CMotion *pMotion = CMotionModel3D::GetMotion();
//
//	//if (pKeyboard->GetPress(DIK_W)
//	//	|| pKeyboard->GetPress(DIK_A)
//	//	|| pKeyboard->GetPress(DIK_D)
//	//	|| pKeyboard->GetPress(DIK_S))
//	//{// �ړ��L�[�������ꂽ
//	//	if (pKeyboard->GetPress(DIK_W))
//	//	{// [W]�L�[�������ꂽ��
//	//		if (pKeyboard->GetPress(DIK_A))
//	//		{// [A]�L�[�������ꂽ��
//	//		 // �ړ������̍X�V
//	//			m_rotDest.y = D3DX_PI * -0.25f;
//	//		}
//	//		else if (pKeyboard->GetPress(DIK_D))
//	//		{// [D]�L�[�������ꂽ��
//	//		 // �ړ������̍X�V
//	//			m_rotDest.y = D3DX_PI * 0.25f;
//	//		}
//	//		else
//	//		{// �ړ������̍X�V
//	//			m_rotDest.y = D3DX_PI * 0.0f;
//	//		}
//	//	}
//	//	else if (pKeyboard->GetPress(DIK_S))
//	//	{// [S]�L�[�������ꂽ��
//	//		if (pKeyboard->GetPress(DIK_A))
//	//		{// [A]�L�[�������ꂽ��
//	//		 // �ړ������̍X�V
//	//			m_rotDest.y = D3DX_PI * -0.75f;
//	//		}
//	//		else if (pKeyboard->GetPress(DIK_D))
//	//		{// [D]�L�[�������ꂽ��
//	//		 // �ړ������̍X�V
//	//			m_rotDest.y = D3DX_PI * 0.75f;
//	//		}
//	//		else
//	//		{// �ړ������̍X�Vq
//	//			m_rotDest.y = D3DX_PI;
//	//		}
//	//	}
//	//	else if (pKeyboard->GetPress(DIK_A))
//	//	{// [A]�L�[�������ꂽ��
//	//	 // �ړ������̍X�V
//	//		m_rotDest.y = D3DX_PI * -0.5f;
//	//	}
//	//	else if (pKeyboard->GetPress(DIK_D))
//	//	{// [D]�L�[�������ꂽ��
//	//	 // �ړ������̍X�V
//	//		m_rotDest.y = D3DX_PI * 0.5f;
//	//	}
//
//	//	// �J�������̎擾
//	//	CCamera *pCamera = CApplication::GetCamera();
//
//	//	// �ړ������̎Z�o
//	//	m_rotDest.y += pCamera->GetRot().y;
//
//	//	// �ړ������̐��K��
//	//	m_rotDest.y = CCalculation::RotNormalization(m_rotDest.y);
//
//	//	// �ړ��ʂ̌v�Z
//	//	move = D3DXVECTOR3(sinf(m_rotDest.y), 0.0f, cosf(m_rotDest.y));
//
//	//	// �p�x�̐��K��
//	//	m_rotDest.y -= D3DX_PI;
//
//	//	if (m_EAction == NEUTRAL_ACTION)
//	//	{
//	//		m_EAction = MOVE_ACTION;
//
//	//		if (pMotion != nullptr)
//	//		{
//	//			pMotion->SetNumMotion(MOVE_ACTION);
//	//		}
//	//	}
//	//}
//
//	// �ړ����̌v�Z
//	m_pMove->Moving(move);
//
//	// �ړ����̎擾
//	D3DXVECTOR3 moveing = m_pMove->GetMove();
//
//	if (sqrtf((moveing.x * moveing.x) + (moveing.z * moveing.z)) <= 0.0f
//		&& pMotion != nullptr
//		&& m_EAction == MOVE_ACTION)
//	{
//		m_EAction = NEUTRAL_ACTION;
//		pMotion->SetNumMotion(NEUTRAL_ACTION);
//	}
//
//	// �����̎擾
//	D3DXVECTOR3 rot = GetRot();
//
//	// �ړI�̌����̕␳
//	if (m_rotDest.y - rot.y >= D3DX_PI)
//	{// �ړ������̐��K��
//		m_rotDest.y -= D3DX_PI * 2;
//	}
//	else if (m_rotDest.y - rot.y <= -D3DX_PI)
//	{// �ړ������̐��K��
//		m_rotDest.y += D3DX_PI * 2;
//	}
//
//	return moveing;
//}
//
//void CEnemyShark::Rotate()
//{
//	// �����̎擾
//	D3DXVECTOR3 rot = GetRot();
//
//	// �����̍X�V
//	rot.y += (m_rotDest.y - rot.y) * 0.5f;
//
//	// �����̐��K��
//	rot.y = CCalculation::RotNormalization(rot.y);
//
//	// �����̐ݒ�
//	SetRot(rot);
//}

#ifdef _DEBUG
//=============================================================================
// ���C���̐ݒu
// Author : �������l
// �T�v : ���C������`��ɐݒu
//=============================================================================
void CEnemyShark::SetLine()
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