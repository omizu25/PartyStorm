//**************************************************
// 
// effect.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// �C���N���[�h
//==================================================
#include "effect.h"
#include "application.h"
#include "calculation.h"
#include <assert.h>

//==================================================
// ��`
//==================================================
const int CEffect::MAX_EXPLOSION = 3000;
const int CEffect::MAX_LIFE = 100;
const int CEffect::STD_MOVE = 75;
const float CEffect::STD_SIZE = 12.0f;

//==================================================
// �ÓI�����o�ϐ�
//==================================================
CEffect* CEffect::m_pEffect[MAX_EFFECT] = {};
int CEffect::m_numAll = 0;

namespace
{
float FloatRandom(float min, float max);	// �����̃����_��
}// namespace�͂����܂�

//--------------------------------------------------
// ����
//--------------------------------------------------
CEffect* CEffect::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXCOLOR& col)
{
	if (m_numAll >= MAX_EFFECT)
	{// �ő吔���z����
		return nullptr;
	}

	CEffect* pEffect = nullptr;

	pEffect = new CEffect;
	
	if (pEffect != nullptr)
	{// null�`�F�b�N
		pEffect->Init();
		pEffect->m_pos = pos;
		pEffect->m_move = move;
		pEffect->m_col = col;
	}

	return pEffect;
}

//--------------------------------------------------
// �S�Ẳ��
//--------------------------------------------------
void CEffect::ReleaseAll()
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (m_pEffect[i] != nullptr)
		{// NULL�`�F�b�N
			m_pEffect[i]->Uninit();
			m_pEffect[i] = nullptr;
		}
	}
}

//--------------------------------------------------
// �S�Ă̍X�V
//--------------------------------------------------
void CEffect::UpdateAll()
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (m_pEffect[i] != nullptr)
		{// NULL�`�F�b�N
			m_pEffect[i]->Update();
		}
	}
}

//--------------------------------------------------
// �����̎擾
//--------------------------------------------------
int CEffect::GetNumAll()
{
	return m_numAll;
}

//--------------------------------------------------
// �G�t�F�N�g�̏��̎擾
//--------------------------------------------------
CEffect** CEffect::GetEffect()
{
	assert(m_numAll >= 0 && m_numAll < MAX_EFFECT);

	return m_pEffect;
}

//--------------------------------------------------
// ����
//--------------------------------------------------
void CEffect::Explosion(const D3DXVECTOR3& pos)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	int max = STD_MOVE * 2;

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		move.x = ((float)(rand() % max) - STD_MOVE);
		move.y = ((float)(rand() % max) - STD_MOVE);
		move.z = ((float)(rand() % max) - STD_MOVE);

		// ����
		CEffect::Create(pos, move, col);
	}
}

//--------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//--------------------------------------------------
CEffect::CEffect() :
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR2(0.0f, 0.0f)),
	m_col(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)),
	m_life(0)
{
	assert(m_numAll >= 0 && m_numAll < MAX_EFFECT);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (m_pEffect[i] == nullptr)
		{// NULL�`�F�b�N
			m_pEffect[i] = this;
			m_index = i;
			m_numAll++;
			break;
		}
	}
}

//--------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------
CEffect::~CEffect()
{
	m_numAll--;
}

//--------------------------------------------------
// ������
//--------------------------------------------------
void CEffect::Init()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(STD_SIZE, STD_SIZE);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_life = MAX_LIFE;
}

//--------------------------------------------------
// �I��
//--------------------------------------------------
void CEffect::Uninit()
{
	// ���
	Release();
}

//--------------------------------------------------
// �X�V
//--------------------------------------------------
void CEffect::Update()
{
	m_life--;

	if (m_life <= 0)
	{
		Uninit();
		return;
	}

	m_pos += m_move;

	//�����E�ړ��ʂ��X�V (����������)
	m_move.x += (0.0f - m_move.x) * 0.05f;
	m_move.y += (0.0f - m_move.y) * 0.05f;
	m_move.z += (0.0f - m_move.z) * 0.05f;

	{// �F�̌��Z
		float ratio = ((float)(MAX_LIFE - m_life) / MAX_LIFE);
		m_col.a = 1.0f - (ratio * ratio);
	}
}

//--------------------------------------------------
// ���
//--------------------------------------------------
void CEffect::Release()
{
	if (m_pEffect[m_index] != nullptr)
	{// NULL�`�F�b�N
		int index = m_index;
		delete m_pEffect[index];
		m_pEffect[index] = nullptr;
	}
}

//--------------------------------------------------
// �ʒu�̎擾
//--------------------------------------------------
const D3DXVECTOR3& CEffect::GetPos() const
{
	return m_pos;
}

//--------------------------------------------------
// �F�̎擾
//--------------------------------------------------
const D3DXCOLOR& CEffect::GetCol() const
{
	return m_col;
}

//--------------------------------------------------
// �ړ��ʂ̎擾
//--------------------------------------------------
const D3DXVECTOR3& CEffect::GetMove() const
{
	return m_move;
}

//--------------------------------------------------
// �T�C�Y�̎擾
//--------------------------------------------------
const D3DXVECTOR2& CEffect::GetSize() const
{
	return m_size;
}

namespace
{
//--------------------------------------------------
// �����_�̃����_��
//--------------------------------------------------
float FloatRandom(float min, float max)
{
	return ((rand() / (float)RAND_MAX) * (max - min)) + min;
}
}
