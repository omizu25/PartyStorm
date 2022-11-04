//=============================================================================
//
// �Q�[���N���X(game.cpp)
// Author : �������l
// �T�v : �Q�[���N���X�̊Ǘ����s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "game.h"
#include "calculation.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"

#include "application.h"
#include "camera_manager.h"
#include "renderer.h"
#include "object.h"
#include "object3D.h"

//���f��
#include "player.h"
#include "Shark.h"
#include "obstacle.h"
#include "move.h"

#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "model_obj.h"
#include "score.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer **CGame::m_pPlayer = nullptr;					// �v���C���[�N���X
CEnemyShark *CGame::m_pEnemyShark = nullptr;			// Enemy
CScore *CGame::m_pScore = nullptr;						// �X�R�A�N���X
CMesh3D *CGame::m_pMesh3D;								// ���b�V���N���X
bool CGame::m_bGame = false;							// �Q�[���̏�

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CGame::Init()
{// �}�E�X�̎擾
	CMouse *pMouse = CApplication::GetMouse();

	// �d�͂̒l��ݒ�
	CCalculation::SetGravity(4.0f);

	// �J�����̈ʒu�ύX
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetPosV(D3DXVECTOR3(0.0f, 300.0f, -1600.0f));
	pCamera->SetPosR(D3DXVECTOR3(0.0f, 90.0f, 0.0f));

	// �n�ʂ̐ݒ�
	m_pMesh3D = CMesh3D::Create();
	m_pMesh3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	m_pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	m_pMesh3D->SetSplitTex(true);
	m_pMesh3D->SetScrollTex(D3DXVECTOR2(0.0f, 0.01f), true);
	m_pMesh3D->LoadTex(2);

	// ���b�V���̐���
	CMesh3D *pMesh3D = CMesh3D::Create();
	pMesh3D->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
	pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
	pMesh3D->SetSplitTex(false);
	pMesh3D->SetWave(7.0f, 10.0f);
	pMesh3D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	pMesh3D->LoadTex(3);

	// �X�J�C�{�b�N�X�̐ݒ�
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(5000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(4);

	// �v���C���[�̐ݒ�
	int nMaxPlayer = CApplication::GetPersonCount();
	m_pPlayer = new CPlayer*[nMaxPlayer];
	assert(m_pPlayer != nullptr);

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// �v���C���[�̐���
		m_pPlayer[nCntPlayer] = CPlayer::Create();
		m_pPlayer[nCntPlayer]->SetMotion("data/MOTION/motion.txt");
		m_pPlayer[nCntPlayer]->SetPos(D3DXVECTOR3(-100.0f + 100.0f * nCntPlayer, 0.0f, -300.0f));
		m_pPlayer[nCntPlayer]->SetNum(nCntPlayer);

		// �ړ����̐ݒ�
		CMove *pMove = m_pPlayer[nCntPlayer]->GetMove();
		pMove->SetMoving(1.0f, 5.0f, 0.5f, 0.1f);
	}

	//�T���ݒ�
	m_pEnemyShark = CEnemyShark::Create();
	m_pEnemyShark->SetMotion("data/MOTION/motionShark.txt");
	m_pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
	m_pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));

	// ���f���̐ݒu
	CModelObj::LoadFile("data/FILE/setModel.txt");

	// �X�R�A�̐���
	m_pScore = CScore::Create(10, false);
	m_pScore->SetScore(0);
	m_pScore->SetPos(D3DXVECTOR3(1280.0f, m_pScore->GetSize().y / 2.0f, 0.0f));

	// �}�E�X�J�[�\��������
	pMouse->SetShowCursor(false);

	m_bGame = true;

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CGame::Uninit()
{// �}�E�X�̎擾
	CMouse *pMouse = CApplication::GetMouse();

	// �}�E�X�J�[�\�����o��
	pMouse->SetShowCursor(true);

	if (m_pPlayer != nullptr)
	{
		int nMaxPlayer = CApplication::GetPersonCount();

		for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
		{// �v���C���[�̐���
			if (m_pPlayer[nCntPlayer] != nullptr
				|| !m_pPlayer[nCntPlayer]->GetDead())
			{
				m_pPlayer[nCntPlayer]->Uninit();
			}	
		}

		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

	// �X�R�A�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CGame::Update()
{// �L�[�{�[�h�̎擾
	// ��Q��
	CObstacle::Pop();

	CKeyboard *pKeyboard = CApplication::GetKeyboard();
	if (pKeyboard->GetTrigger(DIK_F3))
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}

	//�R���g���[���[�g�p�̊m�F
	CJoypad *pJoy = CApplication::GetJoy();
	if (pJoy->GetTrigger(CJoypad::JOYKEY_A, 1))
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		pJoy->Vibration(120, 50000, 0);
		pJoy->Vibration(120, 50000, 1);
	}

	int nMaxPlayer = CApplication::GetPersonCount();
	int nCntDead = 0;

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer]->GetDead())
		{
			nCntDead++;
		}
	}

	if (nCntDead >= nMaxPlayer)
	{
		CApplication::SetNextMode(CApplication::MODE_RESULT);
	}
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CGame::Draw()
{

}
