//=============================================================================
//
// �Ǐ]���f���N���X(follow_model.cpp)
// Author : �������l
// �T�v : �Ǐ]���f���������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>
#include <stdio.h>

#include "follow_model.h"
#include "calculation.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : �Ǐ]���f���𐶐�����
//=============================================================================
CFollowModel * CFollowModel::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CFollowModel *pFollowModel = nullptr;

	// �������̉��
	pFollowModel = new CFollowModel;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pFollowModel != nullptr);

	// ���l�̏�����
	pFollowModel->Init();

	// �C���X�^���X��Ԃ�
	return pFollowModel;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CFollowModel::CFollowModel() : m_posDest(D3DXVECTOR3(0.0f,0.0f,0.0f)),		// �ړI�̈ʒu
m_moveVec(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),									// �ړ�����
m_fSpeed(0.0f),																// �ړ����x
m_fCoefficient(0.0f),														// �����W��
m_bFollow(false)															// �Ǐ]����
{

}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CFollowModel::~CFollowModel()
{

}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CFollowModel::Update()
{
	// �X�V
	CModelObj::Update();

	// �Ǐ]
	Follow();
}

//=============================================================================
// �ړI�̈ʒu�̐ݒ�
// Author : �������l
// �T�v : �ړI�̈ʒu�̐ݒ肵�A�Ǐ]�J�n
//=============================================================================
void CFollowModel::SetFollow(const D3DXVECTOR3 posDest)
{
	// ����(ture)
	m_bFollow = true;

	// �ړI�̈ʒu�̐ݒ�
	m_posDest = posDest;
}

//=============================================================================
// �Ǐ]
// Author : �������l
// �T�v : �Ǐ]
//=============================================================================
void CFollowModel::Follow()
{
	if (m_bFollow)
	{// �Ǐ]
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 moveVecDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 distance = m_posDest - pos;

		moveVecDest.y = atan2f(distance.x, distance.z);
		moveVecDest.x = atan2f(sqrtf((distance.x * distance.x) + (distance.z * distance.z)), distance.y);
		moveVecDest.z = m_moveVec.z;

		// �ړ������̐ݒ�
		D3DXVECTOR3 add = (moveVecDest - m_moveVec);

		// ���K��
		add.x = CCalculation::RotNormalization(add.x);
		add.y = CCalculation::RotNormalization(add.y);
		add.z = CCalculation::RotNormalization(add.z);

		// �ړ������̎Z�o
		m_moveVec += add * m_fCoefficient;

		// ���K��
		m_moveVec.x = CCalculation::RotNormalization(m_moveVec.x);
		m_moveVec.y = CCalculation::RotNormalization(m_moveVec.y);
		m_moveVec.z = CCalculation::RotNormalization(m_moveVec.z);

		// �ړ��ʂ̐ݒ�
		move.z = sinf(m_moveVec.x) * cosf(m_moveVec.y) * m_fSpeed;
		move.x = sinf(m_moveVec.x) * sinf(m_moveVec.y) * m_fSpeed;
		move.y = cosf(m_moveVec.x) * m_fSpeed;

		SetPos(pos + move);
	}
}
