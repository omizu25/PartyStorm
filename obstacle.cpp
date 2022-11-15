//**************************************************
// 
// obstacle.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "obstacle.h"
#include "model3D.h"
#include "player.h"
#include "move.h"
#include "game.h"
#include "application.h"
#include "time.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int MODEL_TYPE = 17;		// ���f���̎��
const float MOVE_SPEED = 5.0f;	// �ړ����x
const float POS_Y = -25.0f;		// Y�̈ʒu
}

//==================================================
// ��`
//==================================================
float CObstacle::m_move = 0.0f;

//--------------------------------------------------
// ����
//--------------------------------------------------
CObstacle* CObstacle::Create(float posX, float posZ, float inverse, float diePosZ)
{
	// �I�u�W�F�N�g�C���X�^���X
	CObstacle* pObstacle = new CObstacle;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pObstacle != nullptr);

	// ������
	pObstacle->Init();

	// �ʒu�̐ݒ�
	pObstacle->SetPos(D3DXVECTOR3(posX, 0.0f, posZ));

	pObstacle->m_inverse = inverse;
	pObstacle->m_diePosZ = diePosZ;

	// �C���X�^���X��Ԃ�
	return pObstacle;
}

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CObstacle::CObstacle() :
	m_waveTime(0),
	m_inverse(0.0f),
	m_diePosZ(0.0f)
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CObstacle::~CObstacle()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
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
	m_inverse = 0.0f;
	m_diePosZ = 0.0f;

	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CObstacle::Uninit()
{
	// �I��
	CModelObj::Uninit();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CObstacle::Update()
{
	m_waveTime++;

	// �ړ�
	Move();

	// ��]
	Rot();

	if (CModelObj::GetPos().z >= m_diePosZ)
	{// �������ʒu���z����
		Uninit();
		return;
	}

	// �����蔻��
	Collison();

	// �X�V
	CModelObj::Update();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CObstacle::Draw()
{
	// �`��
	CModelObj::Draw();
}

//--------------------------------------------------
// �ړ�
//--------------------------------------------------
void CObstacle::Move()
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = CModelObj::GetPos();

	// �ړ�
	pos.z += (MOVE_SPEED * m_move);

	if (CApplication::GetMode() == CApplication::MODE_GAME &&
		CGame::GetTime()->GetTime() <= 0)
	{// �Q�[�����Ő������Ԑ؂�
		pos.y += -1.0f;
	}
	else if (pos.z >= 4500.0f)
	{// �w��̒l�ȏ�
		pos.y += -1.0f;
	}
	else
	{
		// �㉺�ړ�
		pos.y = sinf((m_waveTime * 0.01f) * (D3DX_PI * 2.0f)) * 20.0f;
		pos.y += POS_Y;
	}

	// �ʒu�̐ݒ�
	CModelObj::SetPos(pos);
}

//--------------------------------------------------
// ��]
//--------------------------------------------------
void CObstacle::Rot()
{
	// �����̎擾
	D3DXVECTOR3 rot = CModelObj::GetRot();

	rot.z = sinf((m_waveTime * 0.01f) * (D3DX_PI * 2.0f)) * (D3DX_PI * 0.03f * m_inverse);

	// �����̐ݒ�
	CModelObj::SetRot(rot);
}

//--------------------------------------------------
// �����蔻��
// Author : �������l
//--------------------------------------------------
void CObstacle::Collison()
{
	if (CApplication::GetMode() == CApplication::MODE_GAME &&
		CGame::GetTime()->GetTime() <= 0)
	{// �Q�[�����Ő������Ԑ؂�
		return;
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// �ϐ��錾
		CObject *pObject = CObject::GetTop(nCntPriority);

		while (pObject != nullptr)
		{
			// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ۊ�
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
