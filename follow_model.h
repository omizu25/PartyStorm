//=============================================================================
//
// �Ǐ]���f���N���X(follow_model.h)
// Author : �������l
// �T�v : �Ǐ]���f���������s��
//
//=============================================================================
#ifndef _FOLLOW_MODEL_H_			// ���̃}�N����`������ĂȂ�������
#define _FOLLOW_MODEL_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "model_obj.h"
#include "main.h"

//=============================================================================
// ���f���I�u�W�F�N�g�N���X
// Author : �������l
// �T�v : ���f���I�u�W�F�N�g�������s���N���X
//=============================================================================
class CFollowModel : public CModelObj
{
public:
	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CFollowModel *Create();	// �Ǐ]���f���̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CFollowModel();
	~CFollowModel();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void Update() override;																	// �X�V
	void SetFollow(const D3DXVECTOR3 posDest);												// �ړI�̈ʒu���w��
	void SetFollow(bool bFollow) { m_bFollow = bFollow; }									// �Ǐ]����̐ݒ�
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }								// ���x�̐ݒ�
	void SetCoefficient(const float fCoefficient) { m_fCoefficient = fCoefficient; }		// �����W���̐ݒ�

private:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void Follow();

	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	D3DXVECTOR3 m_posDest;		// �ړI�̈ʒu
	D3DXVECTOR3 m_moveVec;		// �ړ�����
	float m_fSpeed;				// �ړ����x
	float m_fCoefficient;		// �����W��
	bool m_bFollow;				// �Ǐ]����
};

#endif
