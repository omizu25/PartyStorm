//=============================================================================
//
// ��Q���N���X�@(obstacle.cpp)
// Author : �������P
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "obstacle.h"
#include "model3D.h"
#include "player.h"
#include "move.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
namespace
{
const int MODEL_TYPE = 23;				// ���f���̎��
const int POP_INTERVAL = 120;			// �o���̊Ԋu
const float MOVE_SPEED = 5.0f;			// �ړ����x
const float POP_POS_Z = -1500.0f;		// �o����Z�̈ʒu
const float RELEASE_POS_Z = 1500.0f;	// �����Z�̈ʒu
const D3DXVECTOR2 POP_POS_X[] =			// �o����X�̈ʒu
{
	D3DXVECTOR2(-60.0f, -30.0f),
	D3DXVECTOR2(-60.0f, 0.0f),
	D3DXVECTOR2(-60.0f, 30.0f),
	D3DXVECTOR2(-60.0f, 60.0f),
	D3DXVECTOR2(-30.0f, 0.0f),
	D3DXVECTOR2(-30.0f, 30.0f),
	D3DXVECTOR2(-30.0f, 60.0f),
	D3DXVECTOR2(0.0f, 30.0f),
	D3DXVECTOR2(0.0f, 60.0f),
	D3DXVECTOR2(30.0f, 60.0f),
};
const int POP_MAX = sizeof(POP_POS_X) / sizeof(POP_POS_X[0]);	// �o���ʒu�̍ő吔
}

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CObstacle::m_time = 0;

//=============================================================================
// �o��
//=============================================================================
void CObstacle::Pop()
{
	m_time++;

	if (m_time % POP_INTERVAL == 0)
	{
		int randam = rand() % POP_MAX;

		Create(POP_POS_X[randam].x);
		Create(POP_POS_X[randam].y);
	}
}

//=============================================================================
// ����
//=============================================================================
CObstacle* CObstacle::Create(float posX)
{
	// �I�u�W�F�N�g�C���X�^���X
	CObstacle* pObstacle = new CObstacle;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pObstacle != nullptr);

	// ������
	pObstacle->Init();

	// �ʒu�̐ݒ�
	pObstacle->SetPos(D3DXVECTOR3(posX, 0.0f, POP_POS_Z));

	// �C���X�^���X��Ԃ�
	return pObstacle;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObstacle::CObstacle()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObstacle::~CObstacle()
{
}

//=============================================================================
// ������
//=============================================================================
HRESULT CObstacle::Init()
{
	// ������
	CModelObj::Init();

	// ��ނ̐ݒ�
	CModelObj::SetType(MODEL_TYPE);

	// �����蔻��̐ݒ�
	CModel3D::MODEL_MATERIAL *material = GetModel()->GetMaterial();
	D3DXVECTOR3 size = material[GetModel()->GetModelID()].size;
	SetColisonSize(size);
	SetColisonPos(D3DXVECTOR3(0.0f, size.y / 2.0f, 0.0f));

	return E_NOTIMPL;
}

//=============================================================================
// �I��
//=============================================================================
void CObstacle::Uninit()
{
	// �I��
	CModelObj::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CObstacle::Update()
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = CModelObj::GetPos();

	pos.z += MOVE_SPEED;

	// �ʒu�̐ݒ�
	CModelObj::SetPos(pos);

	if (pos.z >= RELEASE_POS_Z)
	{// �������ʒu���z����
		Uninit();
		return;
	}

	// �����蔻��
	Collison();

	// �X�V
	CModelObj::Update();
}

//=============================================================================
// �`��
//=============================================================================
void CObstacle::Draw()
{
	// �`��
	CModelObj::Draw();
}

//=============================================================================
// �����蔻��
// Author : �������l
// �T�v : �����蔻����s��
//=============================================================================
void CObstacle::Collison()
{
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
	{
		// �ϐ��錾
		CObject *pObject = CObject::GetTop(nCntPriority);

		while (pObject != nullptr)
		{// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ۊ�
			CObject *pObjectNext = pObject->GetNext();

			if (pObject->GetFlagDeath() ||
				(pObject->GetObjType() != OBJTYPE_3DPLAYER) ||
				!pObject->ColisonRectangle3D(this, true))
			{
				// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���X�V
				pObject = pObjectNext;
				continue;
			}

			// �v���C���[�̈ړ�
			CPlayer *pPlayer = dynamic_cast<CPlayer*>(pObject);
			CMove *pMove = pPlayer->GetMove();
			D3DXVECTOR3 pos = pPlayer->GetPos();
			pos.z += MOVE_SPEED - pMove->GetMove().z;
			pPlayer->SetPos(pos);

			// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���X�V
			pObject = pObjectNext;
		}
	}
}


