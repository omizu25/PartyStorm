//=============================================================================
//
// �G�l�~�[�N���X(Enemy.h)
// Author : ��]����
// �T�v : �v���C���[�ɏP���|����
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "Shark.h"
#include "game.h"
#include "mesh.h"
#include "motion.h"
#include "renderer.h"
#include "application.h"
#include "camera.h"
#include "keyboard.h"
#include "calculation.h"
#include "move.h"
#include "bullet3D.h"
#include "debug_proc.h"
#include "line.h"
#include "sound.h"

CEnemyShark * CEnemyShark::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CEnemyShark *pEnemyShark = nullptr;

	// �������̉��
	pEnemyShark = new CEnemyShark;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pEnemyShark != nullptr);

	// ���l�̏�����
	pEnemyShark->Init();

	// �C���X�^���X��Ԃ�
	return pEnemyShark;
}

CEnemyShark::CEnemyShark() : m_pMove(nullptr),m_EAction(NEUTRAL_ACTION),
m_rotDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
m_fSpeed(0.0f),
m_nNumMotion(0)
{

}

CEnemyShark::~CEnemyShark()
{
}

HRESULT CEnemyShark::Init()
{
	// ������
	CMotionModel3D::Init();

	// �ړ��N���X�̃������m��
	m_pMove = new CMove;
	assert(m_pMove != nullptr);
	m_pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);

	// �����蔻��̐ݒ�
	SetColisonPos(D3DXVECTOR3(0.0f, 250.0f, 300.0f));
	SetColisonSize(D3DXVECTOR3(500.0f, 500.0f, 2500.0f));

	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetObjType(OBJTYPE_3DENEMY);

#ifdef _DEBUG
	// ���C�����
	m_pLine = new CLine*[12];

	for (int nCntLine = 0; nCntLine < 12; nCntLine++)
	{
		m_pLine[nCntLine] = CLine::Create();
	}

	// ���C���̐ݒ�
	SetLine();
#endif // _DEBUG

	return S_OK;
}

void CEnemyShark::Uninit()
{
	if (m_pMove != nullptr)
	{// �I������
	 // �������̉��
		delete m_pMove;
		m_pMove = nullptr;
	}

	// �I��
	CMotionModel3D::Uninit();
}

void CEnemyShark::Update()
{
	// ���[�V�������̎擾
	CMotion *pMotion = CMotionModel3D::GetMotion();

	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// �j���[�g�������[�V�����̐ݒ�
	if (pMotion != nullptr
		&& !pMotion->GetMotion())
	{
		m_EAction = ATTACK_ACTION;
		pMotion->SetNumMotion(m_EAction);

		if (CApplication::GetMode() != CApplication::MODE_GAME)
		{
			return;
		}
	}

	// �X�V
	CMotionModel3D::Update();

#ifdef _DEBUG
	// ���C���̍X�V
	SetLine();
#endif // _DEBUG
}

void CEnemyShark::Draw()
{
	// �`��
	CMotionModel3D::Draw();
}

#ifdef _DEBUG
//=============================================================================
// ���C���̐ݒu
// Author : �������l
// �T�v : ���C������`��ɐݒu
//=============================================================================
void CEnemyShark::SetLine()
{
	// �ϐ��錾
	const D3DXVECTOR3 pos = GetPos() + GetColisonPos();
	const D3DXVECTOR3 rot = GetRot();
	const D3DXVECTOR3 size = GetColisonSize() / 2.0f;
	const D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// ���C���̐ݒ�
	m_pLine[0]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, size.z), D3DXVECTOR3(size.x, -size.y, size.z), col);
	m_pLine[1]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, -size.z), D3DXVECTOR3(-size.x, -size.y, size.z), col);
	m_pLine[2]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, -size.z), D3DXVECTOR3(size.x, -size.y, -size.z), col);
	m_pLine[3]->SetLine(pos, rot, D3DXVECTOR3(size.x, -size.y, -size.z), D3DXVECTOR3(size.x, -size.y, size.z), col);
	m_pLine[4]->SetLine(pos, rot, D3DXVECTOR3(-size.x, size.y, size.z), D3DXVECTOR3(size.x, size.y, size.z), col);
	m_pLine[5]->SetLine(pos, rot, D3DXVECTOR3(-size.x, size.y, -size.z), D3DXVECTOR3(-size.x, size.y, size.z), col);
	m_pLine[6]->SetLine(pos, rot, D3DXVECTOR3(-size.x, size.y, -size.z), D3DXVECTOR3(size.x, size.y, -size.z), col);
	m_pLine[7]->SetLine(pos, rot, D3DXVECTOR3(size.x, size.y, -size.z), D3DXVECTOR3(size.x, size.y, size.z), col);
	m_pLine[8]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, size.z), D3DXVECTOR3(-size.x, size.y, size.z), col);
	m_pLine[9]->SetLine(pos, rot, D3DXVECTOR3(-size.x, -size.y, -size.z), D3DXVECTOR3(-size.x, size.y, -size.z), col);
	m_pLine[10]->SetLine(pos, rot, D3DXVECTOR3(size.x, -size.y, -size.z), D3DXVECTOR3(size.x, size.y, -size.z), col);
	m_pLine[11]->SetLine(pos, rot, D3DXVECTOR3(size.x, -size.y, size.z), D3DXVECTOR3(size.x, size.y, size.z), col);
}
#endif // DEBUG