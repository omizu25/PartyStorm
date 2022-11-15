//**************************************************
// 
// obstacle_manager.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "obstacle_manager.h"
#include "obstacle.h"

#include <assert.h>

//==================================================
// ��`
//==================================================
namespace
{
const int MIN_INTERVAL = 60;			// �Ԋu�̍ŏ��l
const int MAX_INTERVAL = 120;			// �Ԋu�̍ő�l
const int POP_INCREASE = 1200;			// �o����������
const int MIN_POP = 2;					// �o���̍ŏ���
const int MAX_POP = 4;					// �o���̍ő吔
const float POP_POS_Z = -1000.0f;		// �o����Z�̈ʒu
const float RELEASE_POS_Z = 1250.0f;	// �����Z�̈ʒu
const float POP_POS_X[] =
{// �o����X�̈ʒu
	-200.0f,
	-100.0f,
	0.0f,
	100.0f,
	200.0f,
};
const int POP_MAX = sizeof(POP_POS_X) / sizeof(POP_POS_X[0]);	// �o���ʒu�̍ő吔
const int BG_INTERVAL = 180;			// �w�i�̊Ԋu
const float RELEASE_POS_Z_BG = 5000.0f;	// �w�i�̉����Z�̈ʒu
const float POP_POS_X_BG[] =
{// �w�i�̏o����X�̈ʒu
	-1300.0f,
	-1050.0f,
	-650.0f,
	550.0f,
	1200.0f,
};
}

//--------------------------------------------------
// ����
//--------------------------------------------------
CObstacleManager* CObstacleManager::Create()
{
	CObstacleManager* pManager = nullptr;

	pManager = new CObstacleManager;

	if (pManager != nullptr)
	{// null�`�F�b�N
		// ������
		pManager->Init();
	}

	return pManager;
}

//--------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------
CObstacleManager::CObstacleManager() :
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_time(0),
	m_popIdxBG(0),
	m_pop(0),
	m_interval(0),
	m_move(0.0f),
	m_game(false),
	m_stop(false)
{
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CObstacleManager::~CObstacleManager()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CObstacleManager::Init()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_time = 0;
	m_popIdxBG = -1;
	m_pop = MIN_POP;
	m_interval = MAX_INTERVAL;
	m_move = 1.0f;
	m_game = false;
	m_stop = false;

	// �ړ��ʂ̐ݒ�
	CObstacle::SetMove(m_move);

	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CObstacleManager::Uninit()
{
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CObstacleManager::Update()
{
	if (m_stop)
	{// �o�����~�߂�
		return;
	}

	m_time++;

	// �w�i
	BG();

	// �Q�[��
	Game();
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CObstacleManager::Draw()
{
}

//--------------------------------------------------
// �Q�[��
//--------------------------------------------------
void CObstacleManager::Game()
{
	if (!m_game)
	{// �Q�[�������ǂ���
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
			{// �o�������̃����_�����I�����
				break;
			}
		}

		float inverse = 1.0f;

		for (int i = 0; i < m_pop; i++)
		{
			// ����
			CObstacle::Create(POP_POS_X[random[i]], POP_POS_Z, inverse, RELEASE_POS_Z);

			inverse *= -1.0f;
		}

		if (m_pop >= MAX_POP)
		{// �ő吔���z����
			m_move += 0.05f;

			// �ړ��ʂ̐ݒ�
			CObstacle::SetMove(m_move);

			m_interval--;

			if (m_interval <= MIN_INTERVAL)
			{// �ŏ��l�ȉ�
				m_interval = MIN_INTERVAL;
			}
		}
	}

	if (m_time % POP_INCREASE == 0)
	{// ���Ԋu
		m_pop++;

		if (m_pop >= MAX_POP)
		{// �ő吔���z����
			m_pop = MAX_POP;
		}
	}
}
//--------------------------------------------------
// �w�i
//--------------------------------------------------
void CObstacleManager::BG()
{
	if (m_time % BG_INTERVAL == 0)
	{// ���Ԋu
		int random = 0;

		while (true)
		{
			random = rand() % POP_MAX;

			if (random != m_popIdxBG)
			{// �O��̏o�������ԍ��ł͂Ȃ�
				m_popIdxBG = random;
				break;
			}
		}

		// ����
		CObstacle::Create(POP_POS_X_BG[random], 0.0f, 1.0f, RELEASE_POS_Z_BG);
	}
}
