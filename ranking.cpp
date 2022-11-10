//**************************************************
// 
// rankingUI.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "ranking.h"
#include "score.h"
#include "number.h"
#include "object2D.h"

#include <assert.h>
#include <stdio.h>

//==================================================
// ��`
//==================================================
const float CRanking::STD_WIDTH = 40.0f;
const float CRanking::STD_HEIGHT = 50.0f;
const char* CRanking::FILE_NAME = "data/FILE/Ranking.txt";

namespace
{
const int texIdx[CRanking::MAX_RANKING]
{
	24,
	25,
	26,
	27,
	28,
};
}

//==================================================
// �ÓI�����o�ϐ�
//==================================================
int CRanking::m_score = 0;
int CRanking::m_ranking[MAX_RANKING] = {};

//--------------------------------------------------
// ����
//--------------------------------------------------
CRanking* CRanking::Create()
{
	CRanking* pRanking = nullptr;

	pRanking = new CRanking;

	if (pRanking != nullptr)
	{// null�`�F�b�N
		// ������
		pRanking->Init();
	}

	return pRanking;
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void CRanking::Set(int score)
{
	m_score = score;

	// �ǂݍ���
	Load();

	// �ύX
	Change();

	// �ۑ�
	Save();
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
int CRanking::Get(int rank)
{
	if (rank == -1)
	{// �w��̒l
		return m_score;
	}

	assert(rank >= 0 && rank < MAX_RANKING);

	// �ǂݍ���
	Load();

	return m_ranking[rank];
}

//--------------------------------------------------
// �ǂݍ���
//--------------------------------------------------
void CRanking::Load()
{
	FILE *pFile = nullptr;

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ

		for (int i = 0; i < MAX_RANKING; i++)
		{
			fscanf(pFile, "%d", &m_ranking[i]);
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// �ۑ�
//--------------------------------------------------
void CRanking::Save()
{
	FILE *pFile = nullptr;

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "w");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ

		for (int i = 0; i < MAX_RANKING; i++)
		{
			fprintf(pFile, "%d\n\n", m_ranking[i]);
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		assert(false);
	}
}

//--------------------------------------------------
// �ύX
//--------------------------------------------------
void CRanking::Change()
{
	int ranking[MAX_RANKING + 1] = {};

	for (int i = 0; i < MAX_RANKING; i++)
	{
		ranking[i] = m_ranking[i];
	}

	ranking[MAX_RANKING] = m_score;

	int save = 0;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		save = ranking[i];

		for (int j = i + 1; j < MAX_RANKING + 1; j++)
		{
			if (save < ranking[j])
			{// ���肪�傫����������
				save = ranking[j];
			}
		}

		for (int j = i + 1; j < MAX_RANKING + 1; j++)
		{
			if (save == ranking[j])
			{// �ő�l��T���Č��
				ranking[j] = ranking[i];
				ranking[i] = save;
				break;
			}
		}
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{
		m_ranking[i] = ranking[i];
	}
}

//--------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//--------------------------------------------------
CRanking::CRanking() :
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_newRank(0),
	m_time(0)
{
	for (int i = 0; i < MAX_RANKING; i++)
	{
		m_pRanking[i] = nullptr;
		m_pRank[i] = nullptr;
	}
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CRanking::~CRanking()
{
}

//--------------------------------------------------
// ������
//--------------------------------------------------
HRESULT CRanking::Init()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_newRank = 0;
	m_time = 0;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		m_pRanking[i] = nullptr;
		m_pRank[i] = nullptr;
	}

	return S_OK;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CRanking::Uninit()
{
	for (int i = 0; i < MAX_RANKING; i++)
	{
		if (m_pRanking[i] != nullptr)
		{// null�`�F�b�N
			// �I��
			m_pRanking[i]->Uninit();
			delete m_pRanking[i];
			m_pRanking[i] = nullptr;
		}

		if (m_pRank[i] != nullptr)
		{// null�`�F�b�N
			// �I��
			m_pRank[i]->Uninit();
			m_pRank[i] = nullptr;
		}
	}
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CRanking::Update()
{
	if (m_newRank == -1)
	{// �w��̒l
		return;
	}

	m_time++;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	col.a = 1.0f - ((cosf((m_time * 0.01f) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f) * 0.9f;

	// �F�̐ݒ�
	m_pRanking[m_newRank]->SetColor(col);
}

//--------------------------------------------------
// �`��
//--------------------------------------------------
void CRanking::Draw()
{
}

//--------------------------------------------------
// �ݒ�
//--------------------------------------------------
void CRanking::Set(const D3DXVECTOR3& pos, float length)
{
	m_time = 0;

	m_pos = pos;
	m_size = D3DXVECTOR3(STD_WIDTH, STD_HEIGHT, 0.0f);

	float height = STD_HEIGHT;

	if (length <= 0.0f)
	{// �l���}�C�i�X
		height *= -1.0f;
	}

	float posY = 0.0f;
	float interval = 3 * (m_size.x * 0.5f);
	float maxWidth = 10 * m_size.x;
	float rankPosX = 0.0f;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		posY = pos.y + (i * (length + height));

		// �X�R�A�̐���
		m_pRanking[i] = CScore::Create(Digit(m_ranking[i]), false);

		// �傫���̐ݒ�
		m_pRanking[i]->SetWholeSize(D3DXVECTOR3(1000.0f, 50.0f, 0.0f));
		m_pRanking[i]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));

		// �ʒu�̐ݒ�
		m_pRanking[i]->SetPos(D3DXVECTOR3(pos.x, posY, 0.0f));

		// �����̐ݒ�
		m_pRanking[i]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// �X�R�A�̐ݒ�
		m_pRanking[i]->SetScore(0);
		m_pRanking[i]->SetDestScore(m_ranking[i]);

		rankPosX = pos.x - maxWidth - interval - (STD_HEIGHT * 0.5f);

		// �ʂ̐���
		m_pRank[i] = CObject2D::Create();
		m_pRank[i]->SetPos(D3DXVECTOR3(rankPosX - 100.0f, posY, 0.0f));
		m_pRank[i]->SetSize(D3DXVECTOR3(100.0f, 50.0f, 0.0f));
		m_pRank[i]->LoadTex(texIdx[i]);
	}

	m_newRank = -1;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		if (m_ranking[i] != m_score)
		{// �w��̒l�ł͂Ȃ�
			continue;
		}

		m_newRank = i;

		break;
	}
}

//--------------------------------------------------
// �ʒu�̐ݒ�
//--------------------------------------------------
void CRanking::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	float posY = 0.0f;
	float interval = 3 * (m_size.x * 0.5f);
	float maxWidth = 10 * m_size.x;
	float rankPosX = 0.0f;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		posY = pos.y + (i * (STD_HEIGHT));

		rankPosX = pos.x - maxWidth - interval - (STD_HEIGHT * 0.5f);

		// �ʒu�̐ݒ�
		m_pRanking[i]->SetPos(D3DXVECTOR3(820.0f, posY, 0.0f));
		m_pRank[i]->SetPos(D3DXVECTOR3(rankPosX - 20.0f, posY, 0.0f));
	}
}

//--------------------------------------------------
// �����̐ݒ�
//--------------------------------------------------
void CRanking::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		// �����̐ݒ�
		m_pRanking[i]->SetRot(rot);
		m_pRank[i]->SetRot(rot);
	}
}

//--------------------------------------------------
// �T�C�Y�̐ݒ�
//--------------------------------------------------
void CRanking::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		// �T�C�Y�̐ݒ�
		m_pRanking[i]->SetSize(size);
		m_pRank[i]->SetSize(size);
	}
}

//--------------------------------------------------
// ����
//--------------------------------------------------
int CRanking::Digit(int number)
{
	if (number == 0)
	{
		return 1;
	}

	return (int)log10f((float)number) + 1;
}
