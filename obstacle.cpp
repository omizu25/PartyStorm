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
const int MODEL_TYPE = 24;				// ���f���̎��
const int POP_INTERVAL = 120;			// �o���̊Ԋu
const float MOVE_SPEED = 5.0f;			// �ړ����x
const float POP_POS_Z = -1000.0f;		// �o����Z�̈ʒu
const float POS_Y = -30.0f;				// Y�̈ʒu
const float RELEASE_POS_Z = 1250.0f;	// �����Z�̈ʒu
const D3DXVECTOR2 POP_POS_X[] =			// �o����X�̈ʒu
{
	D3DXVECTOR2(-200.0f, -100.0f),
	D3DXVECTOR2(-200.0f, 0.0f),
	D3DXVECTOR2(-200.0f, 100.0f),
	D3DXVECTOR2(-200.0f, 200.0f),
	D3DXVECTOR2(-100.0f, 0.0f),
	D3DXVECTOR2(-100.0f, 100.0f),
	D3DXVECTOR2(-100.0f, 200.0f),
	D3DXVECTOR2(0.0f, 100.0f),
	D3DXVECTOR2(0.0f, 200.0f),
	D3DXVECTOR2(100.0f, 200.0f),
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
	if (m_time % POP_INTERVAL == 0)
	{// ���Ԋu
		int randam = rand() % POP_MAX;

		Create(POP_POS_X[randam].x, -1.0f);
		Create(POP_POS_X[randam].y, 1.0f);
	}

	m_time++;
}

//=============================================================================
// ����
//=============================================================================
CObstacle* CObstacle::Create(float posX, float inverse)
{
	// �I�u�W�F�N�g�C���X�^���X
	CObstacle* pObstacle = new CObstacle;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pObstacle != nullptr);

	// ������
	pObstacle->Init();

	// �ʒu�̐ݒ�
	pObstacle->SetPos(D3DXVECTOR3(posX, 0.0f, POP_POS_Z));

	pObstacle->m_inverse = inverse;

	// �C���X�^���X��Ԃ�
	return pObstacle;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObstacle::CObstacle() :
	m_waveTime(0)
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

	m_waveTime = 0;

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

	m_waveTime++;

	pos.y = sinf((m_waveTime * 0.01f) * (D3DX_PI * 2.0f)) * 25.0f;

	pos.y += POS_Y;

	// �ʒu�̐ݒ�
	CModelObj::SetPos(pos);

	// �����̎擾
	D3DXVECTOR3 rot = CModelObj::GetRot();

	rot.z = sinf((m_waveTime * 0.01f) * (D3DX_PI * 2.0f)) * (D3DX_PI * 0.03f * m_inverse);

	// �����̐ݒ�
	CModelObj::SetRot(rot);

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
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
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
