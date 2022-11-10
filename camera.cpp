//**************************************************************************************************
//
// �J��������(camera.h)
// Auther�F�������l
// �T�v : �J�����̐ݒ菈��
//
//**************************************************************************************************

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "camera.h"
#include "camera_manager.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "mouse.h"
#include "keyboard.h"
#include "move.h"
#include "calculation.h"
#include "object.h"
#include "debug_proc.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
const float CCamera::CAMERA_NEAR = 10.0f;				// �j�A
const float CCamera::CAMERA_FUR = 100000.0f;			// �t�@�[

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CCamera::CCamera()
{
	m_pMotion = nullptr;							// �J�����̃��[�V�������
	m_pMove = nullptr;								// �ړ��N���X�̃C���X�^���X
	m_pRoll = nullptr;								// �ړ��N���X�̃C���X�^���X(�p�x)
	m_pTarget = nullptr;							// �^�[�Q�b�g
	m_mtxWorld = {};								// ���[���h�}�g���b�N�X
	m_mtxProj = {};									// �v���W�F�N�V�����}�g���b�N�X
	m_mtxView = {};									// �r���[�}�g���b�N�X
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �����_
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ������x�N�g��
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ�����
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���_�̖ړI�̈ʒu
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_�̖ړI�̈ʒu
	m_posRAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����_�̒ǉ��ʒu
	m_viewType = TYPE_CLAIRVOYANCE;					// ���e�̎��
	m_fDistance = 0.0f;								// ���_���璍���_�܂ł̋���
	m_fViewing = 0.0f;								// ����p
	m_fRotMove = 0.0f;								// �ړ�����
	m_fCoeffFllow = 0.0f;							// �Ǐ]�̌����W��
	m_nCntFrame = 0;								// �t���[���J�E���g
	m_nCntKey = 0;									// �L�[�J�E���g
	m_nNumMotion = 0;								// ���[�V�����J�E���g
	m_nMaxMotion = 0;								// ���[�V�����̍ő吔
	m_bAutoMove = false;							// �����ړ�
	m_bAction = false;								// �A�N�V�������s�����ۂ�
	m_bFllow = false;								// �Ǐ]���s����
	m_bLockPosV = false;							// ���_�̃��b�N
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�ƒ����_�̊Ԃ̋������Z�o����
//=============================================================================
HRESULT CCamera::Init()
{
	m_pMotion = CApplication::GetCameraManager()->GetCameraMotion();
	m_nMaxMotion = CApplication::GetCameraManager()->GetMaxMotion();

	m_posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �Œ�
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_�ƒ����_�̋���
	SetDistance();

	// �����̐ݒ�
	SetRot();

	// �ړ��N���X�̃������m��
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(10.0f, 100.0f, 0.0f, 0.3f);

	// �ړ��N���X(�p�x)�̃������m��
	m_pRoll = new CMove;
	assert(m_pRoll != nullptr);
	m_pRoll->SetMoving(0.5f, 5.0f, 0.0f, 0.1f);

	// �����_�̎Z�o
	SetPosR();

	// ���_�̎Z�o
	SetPosV();

	// ����p�̏����l�̑��
	m_fViewing = 45.0f;

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �I��
//=============================================================================
void CCamera::Uninit(void)
{
	if (m_pMove != nullptr)
	{// �I������
		// �������̉��
		delete m_pMove;
		m_pMove = nullptr;
	}

	if (m_pRoll != nullptr)
	{// �I������
		// �������̉��
		delete m_pRoll;
		m_pRoll = nullptr;
	}
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V
//=============================================================================
void CCamera::Update(void)
{
	if(m_viewType == TYPE_CLAIRVOYANCE)
	{
		//Rotate();

		if (m_bFllow)
		{
			Follow();	// �Ǐ]
		}
		else
		{
			Action();	// �A�N�V����
			//Move();		// �ړ�
		}

		//Zoom();
	}

#ifdef _DEBUG
	CDebugProc::Print("�J�����̎��_ | X : %.3f | Y : %.3f | Z : %.3f |\n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("�J�����̒����_ | X : %.3f | Y : %.3f | Z : %.3f |\n", m_posR.x, m_posR.y, m_posR.z);
	CDebugProc::Print("�J�����̌��� | X : %.3f | Y : %.3f | Z : %.3f |\n", m_rot.x, m_rot.y, m_rot.z);
#endif // _DEBUG

}

//=============================================================================
// �J�����̐ݒ�
// Author : �������l
// �T�v : �r���[�}�g���b�N�X�̐ݒ�
//=============================================================================
void CCamera::Set()
{// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);			// �s�񏉊����֐�

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProj);			// �s�񏉊����֐�

	switch (m_viewType)
	{
	case TYPE_CLAIRVOYANCE:
		// �v���W�F�N�V�����}�g���b�N�X�̍쐬(�������e)
		D3DXMatrixPerspectiveFovLH(&m_mtxProj,											// �v���W�F�N�V�����}�g���b�N�X
			D3DXToRadian(m_fViewing),													// ����p
			(float)CRenderer::SCREEN_WIDTH / (float)CRenderer::SCREEN_HEIGHT,			// �A�X�y�N�g��
			CAMERA_NEAR,																// �j�A
			CAMERA_FUR);																// �t�@�[
		break;

	case TYPE_PARALLEL:
		// �v���W�F�N�V�����}�g���b�N�X�̍쐬(���s���e)
		D3DXMatrixOrthoLH(&m_mtxProj,							// �v���W�F�N�V�����}�g���b�N�X
			(float)CRenderer::SCREEN_WIDTH,						// ��
			(float)CRenderer::SCREEN_HEIGHT,					// ����
			CAMERA_NEAR,										// �j�A
			CAMERA_FUR);										// �t�@�[
		break;

	default:
		assert(false);
		break;
	}

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);
}

//=============================================================================
// �J�����̓��e���@�̐ݒ�
// Author : �������l
// �T�v : 
//=============================================================================
void CCamera::SetViewType(VIEW_TYPE type)
{
	m_viewType = type;
}

//=============================================================================
// �J�����̎��_�̐ݒ�
// Author : �������l
// �T�v : �������王�_��ݒ肷��
//=============================================================================
void CCamera::SetPosV(const D3DXVECTOR3 posV)
{
	// ���_�̐ݒ�
	m_posV = posV;

	// ���_�ƒ����_�̋���
	SetDistance();

	// �����̐ݒ�
	SetRot();

	// ���_�̎Z�o
	SetPosV();
}

//=============================================================================
// �J�����̒����_�̐ݒ�
// Author : �������l
// �T�v : �������璍���_��ݒ肷��
//=============================================================================
void CCamera::SetPosR(const D3DXVECTOR3 posR)
{
	// �����_�̐ݒ�
	m_posR = posR;

	// ���_�ƒ����_�̋���
	SetDistance();

	// �����̐ݒ�
	SetRot();

	// �����_�̎Z�o
	SetPosR();
}

//=============================================================================
// �J�����̎��_���璍���_�܂ł̋�����ݒ肷��
// Author : �������l
// �T�v : ���_���璍���_�܂ł̋������Z�o����
//=============================================================================
void CCamera::SetRot(const D3DXVECTOR3 rot, const int nShaft)
{
	// �����̎Z�o
	m_rot = rot;

	if (m_rot.y < -D3DX_PI)
	{// ������-D3DX_PI�����̎�
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.x < 0.0f + 0.1f)
	{// ������0�ȉ��̎�
		m_rot.x = 0.0f + 0.1f;
	}
	if (m_rot.x > D3DX_PI - 0.1f)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.x = D3DX_PI - 0.1f;
	}

	if (nShaft == 0)
	{// �����_�̎Z�o
		SetPosR();
	}
	else
	{// ���_�̎Z�o
		SetPosV();
	}
}

//=============================================================================
// �J�����̐ݒ�
// Author : �������l
// �T�v : 
//=============================================================================
void CCamera::SetCamera(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR, const D3DXVECTOR3 rot)
{
	m_posV = posV;
	m_posR = posR;
	m_rot = rot;

	// ���_�ƒ����_�̋���
	SetDistance();

	// �����̐ݒ�
	SetRot();

	// �����_�̎Z�o
	SetPosR();

	// ���_�̎Z�o
	SetPosV();
}

//=============================================================================
// �Ǐ]����̐ݒ�
// Author : �������l
// �T�v : �Ǐ]����̐ݒ�
//=============================================================================
void CCamera::SetFollowTarget(CObject *pTarget, D3DXVECTOR3 posRAdd, float fDistance, float fCoeffFllow)
{
	m_pTarget = pTarget;
	m_posRAdd = posRAdd;
	m_fDistance = fDistance;
	m_fCoeffFllow = fCoeffFllow;
	m_bFllow = true;
}

//=============================================================================
// ���[�V�����֌W�̃J�E���g�̏�����
// Author : �������l
// �T�v : 
//=============================================================================
void CCamera::MotionReset(void)
{
	m_nCntFrame = 0;								// �t���[���J�E���g
	m_nCntKey = 0;									// �L�[�J�E���g
	m_nNumMotion = 0;								// ���[�V�����J�E���g
}

//=============================================================================
// ���[�V�����ԍ��̐ݒ�
// Author : �������l
// �T�v : 
//=============================================================================
void CCamera::SetNumMotion(const int nNumMotion)
{
	MotionReset();
	m_nNumMotion = nNumMotion;
	m_bAction = true;
}

//=============================================================================
// �J�����̉�]
// Author : �������l
// �T�v : 
//=============================================================================
void CCamera::Rotate(void)
{
	// ���͏��̎擾
	const float MIN_MOUSE_MOVED = 1.0f;
	const float ROTATE_MOUSE_MOVED = 0.45f;

	CMouse *pMouse = CApplication::GetMouse();
	D3DXVECTOR3 rollDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nRotateType = -1;

	if (pMouse->GetPress(CMouse::MOUSE_KEY_LEFT)
		|| pMouse->GetPress(CMouse::MOUSE_KEY_RIGHT))
	{// �}�E�X�̈ړ��ʂ̎擾
		D3DXVECTOR3 mouseMove = D3DXVECTOR3(pMouse->GetMouseMove().y, pMouse->GetMouseMove().x, pMouse->GetMouseMove().z);

		if (D3DXVec3Length(&mouseMove) > MIN_MOUSE_MOVED || D3DXVec3Length(&mouseMove) < -MIN_MOUSE_MOVED)
		{// �}�E�X�̈ړ������̃m�[�}���C�Y
			D3DXVec3Normalize(&mouseMove, &mouseMove);

			// �ړ������̎Z�o
			rollDir = mouseMove * (D3DX_PI / 180.0f) * ROTATE_MOUSE_MOVED;
		}

		if (pMouse->GetPress(CMouse::MOUSE_KEY_LEFT))
		{// ��]�^�C�v�̍X�V
			nRotateType = 1;
		}
		else
		{// ��]�^�C�v�̍X�V
			nRotateType = 0;
		}
	}

	// ���C�W���̌v�Z
	m_pRoll->Moving(rollDir);

	// �J�����̌����̐��K��
	CCalculation::RotNormalization(rollDir.x);
	CCalculation::RotNormalization(rollDir.y);
	CCalculation::RotNormalization(rollDir.z);

	// ��]
	m_rot += m_pRoll->GetMove();

	if (m_rot.y < -D3DX_PI)
	{// ������-D3DX_PI�����̎�
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.x < 0.0f + 0.1f)
	{// ������0�ȉ��̎�
		m_rot.x = 0.0f + 0.1f;
	}
	if (m_rot.x > D3DX_PI - 0.1f)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.x = D3DX_PI - 0.1f;
	}

	if (nRotateType == 0)
	{// �����_�̎Z�o
		m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
		m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
	}
	else
	{// ���_�̎Z�o
		m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
		m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
		m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
	}
}

//=============================================================================
// �J�����̈ړ�
// Author : �������l
// �T�v : �J�����̈ړ�
//=============================================================================
void CCamera::Move(void)
{
	const float CAMERA_MOVE_SPEED = 5.0f;
	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (pKeyboard->GetPress(DIK_W) == true
		|| pKeyboard->GetPress(DIK_A) == true
		|| pKeyboard->GetPress(DIK_D) == true
		|| pKeyboard->GetPress(DIK_S) == true)
	{// �ړ��L�[�������ꂽ
	 // �O�㍶�E�ړ�
		if (pKeyboard->GetPress(DIK_W) == true)
		{// [W]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A) == true)
			{// [A]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotMove.y = D3DX_PI * -0.25f;
				m_rotMove.x = D3DX_PI * -0.25f;
			}
			else if (pKeyboard->GetPress(DIK_D) == true)
			{// [D]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotMove.y = D3DX_PI * 0.25f;
				m_rotMove.x = D3DX_PI * 0.25f;
			}
			else
			{// �ړ������̍X�V
				m_rotMove.y = D3DX_PI * 0.0f;
				m_rotMove.x = D3DX_PI * 0.0f;
			}
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// [S]�L�[�������ꂽ��
			if (pKeyboard->GetPress(DIK_A) == true)
			{// [A]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotMove.y = D3DX_PI * -0.75f;
				m_rotMove.x = D3DX_PI * -0.75f;
			}
			else if (pKeyboard->GetPress(DIK_D) == true)
			{// [D]�L�[�������ꂽ��
			 // �ړ������̍X�V
				m_rotMove.y = D3DX_PI * 0.75f;
				m_rotMove.x = D3DX_PI * 0.75f;
			}
			else
			{// �ړ������̍X�V
				m_rotMove.y = 0.0f;
				m_rotMove.x = D3DX_PI;
			}
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// [A]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotMove.y = D3DX_PI * -0.5f;
			m_rotMove.x = D3DX_PI * 0.0f;
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// [D]�L�[�������ꂽ��
		 // �ړ������̍X�V
			m_rotMove.y = D3DX_PI * 0.5f;
			m_rotMove.x = D3DX_PI * 0.0f;
		}

		// �ړ������̎Z�o
		m_rotMove.x += m_rot.x;

		if (m_rotMove.x > D3DX_PI)
		{// �ړ������̐��K��
			m_rotMove.x -= D3DX_PI * 2;
		}
		else if (m_rotMove.x < -D3DX_PI)
		{// �ړ������̐��K��
			m_rotMove.x += D3DX_PI * 2;
		}

		m_rotMove.y += m_rot.y;

		if (m_rotMove.y > D3DX_PI)
		{// �ړ������̐��K��
			m_rotMove.y -= D3DX_PI * 2;
		}
		else if (m_rotMove.y < -D3DX_PI)
		{// �ړ������̐��K��
			m_rotMove.y += D3DX_PI * 2;
		}

		// ���_�ʒu�̍X�V
		move.z = sinf(m_rotMove.x) * cosf(m_rotMove.y);
		move.x = sinf(m_rotMove.x) * sinf(m_rotMove.y);
		move.y = cosf(m_rotMove.x);
	}

	// ���C�W���̌v�Z
	m_pMove->Moving(move);
	m_posV += m_pMove->GetMove();

	// �����_�̎Z�o
	m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
}

//=============================================================================
// �J�����̒Ǐ]
// Author : �������l
// �T�v : �J�����̒Ǐ]
//=============================================================================
void CCamera::Follow(void)
{
	if (m_pTarget != nullptr
		&& !m_pTarget->GetFlagDeath())
	{// �ϐ��錾
		D3DXVECTOR3 targetPos = m_pTarget->GetPos() + m_posRAdd;
		D3DXVECTOR3 targetRot = m_pTarget->GetRot();

		m_posRDest.z = targetPos.z + cosf(targetRot.y);
		m_posRDest.x = targetPos.x + sinf(targetRot.y);
		m_posRDest.y = targetPos.y;

		// �����_�̈ړ�
		m_posR += (m_posRDest - m_posR) * m_fCoeffFllow;

		if (!m_bLockPosV)
		{
			m_posVDest.z = targetPos.z - cosf(m_rot.y) * m_fDistance;
			m_posVDest.x = targetPos.x - sinf(m_rot.y) * m_fDistance;
			m_posVDest.y = m_posV.y;

			// ���_�̈ړ�
			m_posV += (m_posVDest - m_posV) * m_fCoeffFllow;
		}
	}
	else
	{
		m_pTarget = nullptr;
		m_bFllow = false;
	}
}

//=============================================================================
// �J�����̃A�N�V����
// Author : �������l
// �T�v : �J�����̃A�N�V�����ݒ�
//=============================================================================
void CCamera::Action()
{
	if (m_pMotion != nullptr
		&& m_bAction)
	{
		if (m_nCntFrame == 0)
		{// �ǉ����鐔�l�̎Z�o
			m_posVDest = m_pMotion[m_nNumMotion].pCameraAction[m_nCntKey].posVDest - m_posV;
			m_posRDest = m_pMotion[m_nNumMotion].pCameraAction[m_nCntKey].posRDest - m_posR;
		}

		m_nCntFrame++;

		D3DXVECTOR3 addPosV = m_posVDest / (float)m_pMotion[m_nNumMotion].pCameraAction[m_nCntKey].nFrame;
		D3DXVECTOR3 addPosR = m_posRDest / (float)m_pMotion[m_nNumMotion].pCameraAction[m_nCntKey].nFrame;

		// ���_�̈ړ�
		D3DXVECTOR3 posV = GetPosV() + addPosV;

		// ���_�̐ݒ�
		SetPosV(posV);

		// �����_�̈ړ�
		D3DXVECTOR3 posR = GetPosR() + addPosR;

		// �����_�̐ݒ�
		SetPosR(posR);

		if (m_nCntFrame >= m_pMotion[m_nNumMotion].pCameraAction[m_nCntKey].nFrame)
		{
			m_nCntKey++;
			m_nCntFrame = 0;

			if (m_nCntKey >= m_pMotion[m_nNumMotion].nMaxKey)
			{
				if (m_pMotion[m_nNumMotion].bLoop)
				{
					m_nCntKey = 0;
				}
				else if(!m_pMotion[m_nNumMotion].bLoop)
				{
					m_bAction = false;
				}
			}
		}
	}
}

//=============================================================================
// �J�����̊g�k����
// Author : �������l
// �T�v : �}�E�X�z�C�[���ŃJ�����̊g�k���s��
//=============================================================================
void CCamera::Zoom()
{// �}�E�X���̎擾
	CMouse *pMouse = CApplication::GetMouse();

	m_fDistance += (float)pMouse->GetMouseWheel() * 1.0f;

	if (m_fDistance >= CAMERA_FUR / 2.0f)
	{
		m_fDistance = CAMERA_FUR / 2.0f;
	}
	else if (m_fDistance <= CAMERA_NEAR * 2.0f)
	{
		m_fDistance = CAMERA_NEAR * 2.0f;
	}

	SetPosV();
}

//=============================================================================
// �J�����̎��_�̐ݒ�
// Author : �������l
// �T�v : ���_��ݒ肷��
//=============================================================================
void CCamera::SetPosV()
{
	// ���_�̎Z�o
	m_posV.z = m_posR.z - sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	m_posV.x = m_posR.x - sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x) * m_fDistance;
}

//=============================================================================
// �J�����̒����_�̐ݒ�
// Author : �������l
// �T�v : �����_��ݒ肷��
//=============================================================================
void CCamera::SetPosR()
{
	// �����_�̎Z�o
	m_posR.z = m_posV.z + sinf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
	m_posR.x = m_posV.x + sinf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posR.y = m_posV.y + cosf(m_rot.x) * m_fDistance;
}

//=============================================================================
// �J�����̊p�x�̐ݒ�
// Author : �������l
// �T�v : �J�����̊p�x
//=============================================================================
void CCamera::SetRot()
{
	// ���_�ƒ����_�̋���
	D3DXVECTOR3 posDiss = m_posR - m_posV;

	// �p�x�̎Z�o
	m_rot.y = atan2f(posDiss.x, posDiss.z);
	m_rot.x = atan2f(sqrtf((posDiss.x * posDiss.x) + (posDiss.z * posDiss.z)), posDiss.y);
	m_rot.z = 0.0f;

	if (m_rot.y < -D3DX_PI)
	{// ������-D3DX_PI�����̎�
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.y -= D3DX_PI * 2;
	}
	if (m_rot.x < 0.0f + 0.1f)
	{// ������0�ȉ��̎�
		m_rot.x = 0.0f + 0.1f;
	}
	if (m_rot.x > D3DX_PI - 0.1f)
	{// ������D3DX_PI�ȏ�̎�
		m_rot.x = D3DX_PI - 0.1f;
	}
}

//=============================================================================
// �J�����̎��_���璍���_�܂ł̋�����ݒ肷��
// Author : �������l
// �T�v : ���_���璍���_�܂ł̋������Z�o����
//=============================================================================
void CCamera::SetDistance()
{
	// ���_�ƒ����_�̋���
	D3DXVECTOR3 posDiss = m_posR - m_posV;
	m_fDistance = sqrtf((posDiss.y * posDiss.y) + (posDiss.x * posDiss.x) + (posDiss.z * posDiss.z));
}


