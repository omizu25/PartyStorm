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
#include "game.h"
#include "application.h"
#include "time.h"

//*****************************************************************************
// �萔��`
//*****************************************************************************
namespace
{
const int MODEL_TYPE = 16;				// ���f���̎��
const int MIN_INTERVAL = 60;			// �Ԋu�̍ŏ��l
const int MAX_INTERVAL = 120;			// �Ԋu�̍ŏ��l
const int POP_INCREASE = 1200;			// �o����������
const int MIN_POP = 2;					// �o���̍ŏ���
const int MAX_POP = 4;					// �o���̍ő吔
const float MOVE_SPEED = 5.0f;			// �ړ����x
const float POP_POS_Z = -1000.0f;		// �o����Z�̈ʒu
const float POS_Y = -25.0f;				// Y�̈ʒu
const float RELEASE_POS_Z = 1250.0f;	// �����Z�̈ʒu
const float POP_POS_X[] =				// �o����X�̈ʒu
{
	-200.0f,
	-100.0f,
	0.0f,
	100.0f,
	200.0f,
};
const int POP_MAX = sizeof(POP_POS_X) / sizeof(POP_POS_X[0]);	// �o���ʒu�̍ő吔
}

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
int CObstacle::m_time = 0;
int CObstacle::m_pop = 0;
int CObstacle::m_interval = 0;
float CObstacle::m_move = 0.0f;
bool CObstacle::m_stop = false;

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
void CObstacle::InitStatic()
{
	m_time = 0;
	m_pop = MIN_POP;
	m_interval = MAX_INTERVAL;
	m_move = 1.0f;
}

//=============================================================================
// �o��
//=============================================================================
void CObstacle::Pop()
{
	if (m_stop)
	{// �o�����~�߂�
		return;
	}

	if (m_time % m_interval == 0)
	{// ���Ԋu
		int random[MAX_POP];

		int count = 0;

		while (true)
		{
			random[count] = rand() % POP_MAX;
			bool same = false;

			for (int i = 0; i < count; i++)
			{
				if (random[i] == random[count])
				{// �����l������
					same = true;
					break;
				}
			}

			if (!same)
			{// �����l���Ȃ�����
				count++;
			}

			if (count >= m_pop)
			{// �o�����̃����_�����I�����
				break;
			}
		}

		float inverse = 1.0f;

		for (int i = 0; i < m_pop; i++)
		{
			// ����
			Create(POP_POS_X[random[i]], inverse);

			inverse *= -1.0f;
		}

		if (m_pop >= MAX_POP)
		{// �ő吔���z����
			m_move += 0.05f;
			m_interval--;

			if (m_interval <= MIN_INTERVAL)
			{// �ŏ��l�ȉ�
				m_interval = MIN_INTERVAL;
			}
		}
	}

	m_time++;

	if (m_time % POP_INCREASE == 0)
	{// ���Ԋu
		m_pop++;

		if (m_pop >= MAX_POP)
		{// �ő吔���z����
			m_pop = MAX_POP;
		}
	}
}

//=============================================================================
// �o�����~�߂邩�ǂ���
//=============================================================================
void CObstacle::Stop(bool stop)
{
	m_stop = stop;
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
	m_waveTime(0),
	m_inverse(0.0f)
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
	m_inverse = 0.0f;

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

	pos.z += (MOVE_SPEED * m_move);

	m_waveTime++;

	if (CApplication::GetMode() == CApplication::MODE_GAME &&
		CGame::GetTime()->GetTime() <= 0)
	{// �Q�[�����Ő������Ԑ؂�
		pos.y += -1.0f;
	}
	else
	{
		pos.y = sinf((m_waveTime * 0.01f) * (D3DX_PI * 2.0f)) * 20.0f;

		pos.y += POS_Y;
	}

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
