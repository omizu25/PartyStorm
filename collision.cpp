//=============================================================================
//
// �����蔻��N���X(collision.cpp)
// Author : �������l
// �T�v : �����蔻�萶�����s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "collision.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CCollision::CCollision() : m_pParent(nullptr),
m_pos(D3DXVECTOR3(0.0f,0.0f,0.0f)),
m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_size(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
{

}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CCollision::~CCollision()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CCollision::Init()
{
	// �ϐ��̏�����
	m_pParent = nullptr;							// �e
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ߋ��ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �傫��

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CCollision::Uninit()
{
	// �I�u�W�F�N�g3D�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CCollision::Update()
{
	
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CCollision::Draw()
{

}

////=============================================================================
//// �͈͓��̔���
//// Author : �������l
//// �T�v : �^�[�Q�b�g���͈͓��ɓ��������̔���
////=============================================================================
//bool CCollision::ColisonRange2D(CObject * target)
//{// �����̏����擾����
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	D3DXVECTOR3 size = GetSize();
//
//	// �ڕW�̏��擾
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	D3DXVECTOR3 sizeTarget = target->GetSize();
//
//	if (pos.x - size.x >= posTarget.x - sizeTarget.x
//		&& pos.x + size.x <= posTarget.x + sizeTarget.x
//		&& pos.y - size.y >= posTarget.y - sizeTarget.y
//		&& pos.y + size.y <= posTarget.y + sizeTarget.y)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
////=============================================================================
//// ��`�̔���
//// Author : �������l
//// �T�v : �^�[�Q�b�g�Ƃ̋�`����
////=============================================================================
//bool CCollision::ColisonRectangle2D(CObject *target, bool bExtrude)
//{// �Ԃ�l�p�̕ϐ�
//	bool bColision = false;
//
//	// �����̏����擾����
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	D3DXVECTOR3 posOld = GetPosOld();
//	D3DXVECTOR3 size = GetSize() / 2.0f;
//
//	// �ڕW�̏��擾
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;
//
//	if (pos.y - size.y  < posTarget.y + sizeTarget.y
//		&& pos.y + size.y  > posTarget.y - sizeTarget.y)
//	{// ���f�����ɂ���(Y��)
//		if (posOld.x + size.y <= posTarget.x - sizeTarget.x
//			&& pos.x + size.y  > posTarget.x - sizeTarget.x)
//		{
//			if (bExtrude)
//			{// �����o�����g�p����
//				pos.x = posTarget.x - sizeTarget.x - size.y;
//			}
//
//			bColision = true;
//		}
//		if (posOld.x - size.y >= posTarget.x + sizeTarget.x
//			&& pos.x - size.y  < posTarget.x + sizeTarget.x)
//		{
//			if (bExtrude)
//			{// �����o�����g�p����
//				pos.x = posTarget.x + sizeTarget.x + size.y;
//			}
//
//			bColision = true;
//		}
//	}
//	if (pos.x - size.x  < posTarget.x + sizeTarget.x
//		&& pos.x + size.x  > posTarget.x - sizeTarget.x)
//	{// ���f�����ɂ���(X��)
//		if (posOld.y + size.y <= posTarget.y - sizeTarget.y
//			&& pos.y + size.y  > posTarget.y - sizeTarget.y)
//		{
//			if (bExtrude)
//			{// �����o�����g�p����
//				pos.y = posTarget.y - sizeTarget.y - size.y;
//			}
//
//			bColision = true;
//		}
//		if (posOld.y - size.y >= posTarget.y + sizeTarget.y
//			&& pos.y - size.y  < posTarget.y + sizeTarget.y)
//		{
//			if (bExtrude)
//			{// �����o�����g�p����
//				pos.y = posTarget.y + sizeTarget.y + size.y;
//			}
//
//			bColision = true;
//		}
//	}
//
//	// �ʒu�̐ݒ�
//	SetPos(pos);
//	return bColision;
//}
//
////=============================================================================
//// �~�̔���
//// Author : �������l
//// �T�v : �^�[�Q�b�g�Ƃ̉~����
////=============================================================================
//bool CCollision::ColisonCircle2D(CObject * target, bool bExtrude)
//{
//	// �ϐ��錾
//	bool bCollision = false;
//
//	// �����̏����擾����
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	D3DXVECTOR3 size = GetSize() / 2.0f;
//
//	// �ڕW�̏��擾
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;
//
//	// ������s���������Z�o
//	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y));
//	fJudgeDistance += sqrtf((sizeTarget.x * sizeTarget.x) + (sizeTarget.y * sizeTarget.y));
//
//	// ���݂��̈ʒu�̍����Z�o
//	D3DXVECTOR3 distance = posTarget - pos;
//	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));
//
//	if (fDistance <= fJudgeDistance)
//	{// �ʒu�̍���������s���������Z���ꍇ
//		bCollision = true;
//
//		if (bExtrude)
//		{
//			float fRotDif = atan2f(distance.x, distance.y);
//			pos.x = posTarget.x - (sinf(fRotDif) * fJudgeDistance);
//			pos.y = posTarget.y - (cosf(fRotDif) * fJudgeDistance);
//		}
//	}
//
//	// �ʒu�̐ݒ�
//	SetPos(pos);
//	return bCollision;
//}
//
////=============================================================================
//// ��`�̔���
//// Author : �������l
//// �T�v : �^�[�Q�b�g�Ƃ̋�`����
////=============================================================================
//bool CCollision::ColisonRectangle3D(CObject * target, bool bExtrude)
//{// �Ԃ�l�p�̕ϐ�
//	bool bColision = false;
//
//	// �����̏����擾����
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	D3DXVECTOR3 posOld = GetPosOld();
//	D3DXVECTOR3 size = GetSize() / 2.0f;
//
//	// �ڕW�̏��擾
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	D3DXVECTOR3 sizeTarget = target->GetSize() / 2.0f;
//
//	if ((pos.z - size.z / 2) < (posTarget.z + sizeTarget.z)
//		&& (pos.z + size.z / 2) > (posTarget.z - sizeTarget.z)
//		&& (pos.x - size.x / 2) < (posTarget.x + sizeTarget.x)
//		&& (pos.x + size.x / 2) > (posTarget.x - sizeTarget.x))
//	{// ���f�����ɂ���(XZ��)
//		if ((posOld.y + size.y) <= (posTarget.y - sizeTarget.y)
//			&& (pos.y + size.y) >(posTarget.y - sizeTarget.y))
//		{
//			bColision = true;
//
//			if (bExtrude)
//			{
//				pos.y = posTarget.y - sizeTarget.y - size.y;
//			}
//		}
//		if ((posOld.y) >= (posTarget.y + sizeTarget.y)
//			&& (pos.y) < (posTarget.y + sizeTarget.y))
//		{
//			bColision = true;
//
//			if (bExtrude)
//			{
//				pos.y = posTarget.y + sizeTarget.y;
//			}
//		}
//	}
//	if ((pos.y) < (posTarget.y + sizeTarget.y)
//		&& (pos.y + size.y) > (posTarget.y - sizeTarget.y))
//	{// ���f�����ɂ���(Y��)
//		if ((pos.z - size.z / 2) < (posTarget.z + sizeTarget.z)
//			&& (pos.z + size.z / 2) > (posTarget.z - sizeTarget.z))
//		{// ���f�����ɂ���(Z��)
//			if ((posOld.x + size.z / 2) <= (posTarget.x - sizeTarget.x)
//				&& (pos.x + size.z / 2) > (posTarget.x - sizeTarget.x))
//			{
//				bColision = true;
//
//				if (bExtrude)
//				{
//					pos.x = posTarget.x - sizeTarget.x - size.z / 2;
//				}
//			}
//			if ((posOld.x - size.z / 2) >= (posTarget.x + sizeTarget.x)
//				&& (pos.x - size.z / 2) < (posTarget.x + sizeTarget.x))
//			{
//				bColision = true;
//
//				if (bExtrude)
//				{
//					pos.x = posTarget.x + sizeTarget.x + size.z / 2;
//				}
//			}
//		}
//		if ((pos.x - size.x / 2) < (posTarget.x + sizeTarget.x)
//			&& (pos.x + size.x / 2) > (posTarget.x - sizeTarget.x))
//		{// ���f�����ɂ���(X��)
//			if ((posOld.z + size.z / 2) <= (posTarget.z - sizeTarget.z)
//				&& (pos.z + size.z / 2) > (posTarget.z - sizeTarget.z))
//			{
//				bColision = true;
//
//				if (bExtrude)
//				{
//					pos.z = posTarget.z - sizeTarget.z - size.z / 2;
//				}
//			}
//			if ((posOld.z - size.z / 2) >= (posTarget.z + sizeTarget.z)
//				&& (pos.z - size.z / 2) < (posTarget.z + sizeTarget.z))
//			{
//				bColision = true;
//
//				if (bExtrude)
//				{
//					pos.z = posTarget.z + sizeTarget.z + size.z / 2;
//				}
//			}
//		}
//	}
//
//	// �ʒu�̐ݒ�
//	SetPos(pos);
//	return bColision;
//}
//
////=============================================================================
//// ���̔���
//// Author : �������l
//// �T�v : �^�[�Q�b�g�Ƃ̋�����
////=============================================================================
//bool CCollision::ColisonSphere3D(CObject *target, D3DXVECTOR3 size, D3DXVECTOR3 targetSize, bool bExtrude)
//{
//	// �ϐ��錾
//	bool bCollision = false;
//
//	// �����̏����擾����
//	D3DXVECTOR3 pos = GetPos() + m_posAdd;
//	size /= 2.0f;
//
//	// �ڕW�̏��擾
//	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
//	targetSize /= 2.0f;
//
//	// ������s���������Z�o
//	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y) + (size.z * size.z));
//	fJudgeDistance += sqrtf((targetSize.x * targetSize.x) + (targetSize.y * targetSize.y) + (targetSize.z * targetSize.z));
//
//	// ���݂��̈ʒu�̍����Z�o
//	D3DXVECTOR3 distance = posTarget - pos;
//	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));
//
//	if (fDistance <= fJudgeDistance)
//	{// �ʒu�̍���������s���������Z���ꍇ
//		bCollision = true;
//
//		if (bExtrude)
//		{
//			// �p�x�̎Z�o
//			D3DXVECTOR3 rotDiff;
//			rotDiff.y = atan2f(distance.x, distance.z);
//			rotDiff.x = atan2f(sqrtf((distance.x * distance.x) + (distance.z * distance.z)), distance.y);
//			rotDiff.z = 0.0f;
//
//			// �ʒu�̎Z�o
//			pos.z = posTarget.z - sinf(rotDiff.x) * cosf(rotDiff.y) * fJudgeDistance;
//			pos.x = posTarget.x - sinf(rotDiff.x) * sinf(rotDiff.y) * fJudgeDistance;
//			pos.y = posTarget.y - cosf(rotDiff.x) * fJudgeDistance;
//
//			// �ʒu�̐ݒ�
//			SetPos(pos);
//		}
//	}
//
//	return bCollision;
//}

//=============================================================================
// �e�̃Z�b�^�[
// Author : �������l
// �T�v : �e�̃����o�ϐ��Ɉ�������
//=============================================================================
void CCollision::SetParent(CObject * pParent)
{
	 // �e�I�u�W�F�N�g�̐ݒ�
	m_pParent = pParent;
}

//=============================================================================
// �ʒu�̃Z�b�^�[
// Author : �������l
// �T�v : �ʒu�̃����o�ϐ��Ɉ�������
//=============================================================================
void CCollision::SetPos(const D3DXVECTOR3 &pos)
{
	// �ʒu�̐ݒ�
	m_pos = pos;
}

//=============================================================================
// �����̃Z�b�^�[
// Author : �������l
// �T�v : �����̃����o�ϐ��Ɉ�������
//=============================================================================
void CCollision::SetRot(const D3DXVECTOR3 &rot)
{
	// �ʒu�̐ݒ�
	m_rot = rot;
}

//=============================================================================
// �傫���̃Z�b�^�[
// Author : �������l
// �T�v : �傫���̃����o�ϐ��Ɉ�������
//=============================================================================
void CCollision::SetSize(const D3DXVECTOR3 & size)
{
	// �傫���̐ݒ�
	m_size = size;
}
