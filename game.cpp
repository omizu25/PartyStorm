//=============================================================================
//
// �Q�[���N���X(game.cpp)
// Author : �������l
// Author : �������P
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
#include "object2D.h"

//���f��
#include "motion_model3D.h"
#include "player.h"
#include "Shark.h"
#include "obstacle.h"
#include "obstacle_manager.h"
#include "move.h"

#include "effect.h"
#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "model_obj.h"
#include "score.h"
#include "time.h"
#include "follow_model.h"
#include "sound.h"
#include "pause.h"
#include "result.h"

//--------------------------------------------------------------------
// �萔��`
//--------------------------------------------------------------------
const D3DXVECTOR3 CGame::CAMERA_POSV = D3DXVECTOR3(0.0f, 300.0f, -1600.0f);		// ���_
const D3DXVECTOR3 CGame::CAMERA_POSR = D3DXVECTOR3(0.0f, 90.0f, 0.0f);			// �����_

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer **CGame::m_pPlayer = nullptr;					// �v���C���[�N���X
CEnemyShark *CGame::m_pEnemyShark = nullptr;			// Enemy
CScore *CGame::m_pScore = nullptr;						// �X�R�A�N���X
CTime *CGame::m_pTime = nullptr;						// �^�C���N���X
CMesh3D *CGame::m_pMesh3D;								// ���b�V���N���X
CFollowModel *CGame::m_pCameraTarget = nullptr;			// �J�����^�[�Q�b�g
CObstacleManager *CGame::m_pObstacle = nullptr;			// ��Q���̃}�l�[�W���[
bool CGame::m_bGame = false;							// �Q�[���̏�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame() : m_nCntFrame(0)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// ������
//=============================================================================
HRESULT CGame::Init()
{// �}�E�X�̎擾
	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// �Q�[��BGM�̍Đ�
	pSound->PlaySound(CSound::SOUND_LABEL_GAMEBGM);

	// �d�͂̒l��ݒ�
	CCalculation::SetGravity(4.0f);

	// �J�����̈ʒu�ύX
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetPosV(CAMERA_POSV);
	pCamera->SetPosR(CAMERA_POSR);
	pCamera->SetViewing(20.0f);

	// �J�����^�[�Q�b�g�̐ݒ�
	m_pCameraTarget = CFollowModel::Create();
	m_pCameraTarget->SetPos(CAMERA_POSR);
	pCamera->SetFollowTarget(m_pCameraTarget, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f);
	pCamera->SetPosV(CAMERA_POSV);
	pCamera->SetLockPosV(true);

	// �n�ʂ̐ݒ�
	m_pMesh3D = CMesh3D::Create();
	m_pMesh3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	m_pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	m_pMesh3D->SetSplitTex(true);
	m_pMesh3D->SetScrollTex(D3DXVECTOR2(0.0f, 0.01f), true);
	m_pMesh3D->LoadTex(0);

	// ���b�V���̐���
	CMesh3D *pMesh3D = CMesh3D::Create();
	pMesh3D->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
	pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
	pMesh3D->SetSplitTex(false);
	pMesh3D->SetWave(7.0f, 10.0f);
	pMesh3D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	pMesh3D->LoadTex(1);

	// �X�J�C�{�b�N�X�̐ݒ�
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(5000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(2);

	// �v���C���[�̐ݒ�
	int nMaxPlayer = CApplication::GetPersonCount();
	m_pPlayer = new CPlayer*[nMaxPlayer];
	assert(m_pPlayer != nullptr);

	float posX = 500.0f / (nMaxPlayer + 1);

	for (int nCntPlayer = 0; nCntPlayer < nMaxPlayer; nCntPlayer++)
	{// �v���C���[�̐���
		m_pPlayer[nCntPlayer] = CPlayer::Create();
		m_pPlayer[nCntPlayer]->SetMotion("data/MOTION/motion.txt");
		m_pPlayer[nCntPlayer]->SetPos(D3DXVECTOR3(-250.0f + posX + (posX * nCntPlayer), 0.0f, -300.0f));
		m_pPlayer[nCntPlayer]->SetNum(nCntPlayer);

		// �ړ����̐ݒ�
		CMove *pMove = m_pPlayer[nCntPlayer]->GetMove();
		pMove->SetMoving(1.0f, 8.5f, 0.5f, 0.1f);
	}

	// ��Q��
	m_pObstacle = CObstacleManager::Create();
	m_pObstacle->SetGame(true);

	// �^�C���̐ݒ�
	m_pTime = CTime::Create();

	if (nMaxPlayer > 1)
	{// �}���`�v���C
		m_pTime->SetTime(60);
		m_pTime->SetPos(D3DXVECTOR3(640.0f, 75.0f, 0.0f));
		m_pTime->SetSize(D3DXVECTOR3(200.0f, 300.0f, 0.0f));
	}
	else
	{// �V���O���v���C
		m_pTime->SetTime(0);
		m_pTime->SetPos(D3DXVECTOR3(640.0f, 50.0f, 0.0f));
	}
	
	//�T���ݒ�
	m_pEnemyShark = CEnemyShark::Create();
	m_pEnemyShark->SetMotion("data/MOTION/motionShark.txt", 2);
	m_pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
	m_pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));

	{// �`���[�g���A��
		m_pTutorial = CObject2D::Create();
		m_pTutorial->SetPos(D3DXVECTOR3(200.0f, 100.0f, 0.0f));
		m_pTutorial->SetSize(D3DXVECTOR3(400.0f, 200.0f, 0.0f));
		int numJoy = CApplication::GetJoy()->GetUseJoyPad();

		if (numJoy >= 1)
		{// �R���g���[���[���q���ł���
			m_pTutorial->LoadTex(31);
		}
		else
		{// �R���g���[���[���q���łȂ�
			m_pTutorial->LoadTex(32);
		}
	}

	{// �ړI
		CObject2D* pObj = CObject2D::Create();
		pObj->SetPos(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH - 200.0f, 100.0f, 0.0f));
		pObj->SetSize(D3DXVECTOR3(250.0f, 100.0f, 0.0f));
		pObj->LoadTex(30);
	}

	// �J�E���g�_�E���̐ݒ�
	CTime* pCountDown = CTime::Create();
	pCountDown->SetCountDown();

	// ���f���̐ݒu
	CModelObj::LoadFile("data/FILE/setModel.txt");

	// ���S�̏�����
	CResult::InitDead();

	m_bGame = true;

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CGame::Uninit()
{// �}�E�X�̎擾
	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// �T�E���h�I��
	pSound->StopSound();

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

		delete[] m_pPlayer;
		m_pPlayer = nullptr;
	}

	// �X�R�A�̉��
	Release();

	// �J�����̈ʒu�ύX
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetFollowTarget(nullptr, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f);

	// �G�t�F�N�g�̏I��
	CEffect::ReleaseAll();
}

//=============================================================================
// �X�V
//=============================================================================
void CGame::Update()
{
	// �G�t�F�N�g�̍X�V
	CEffect::UpdateAll();

	// �I�u�W�F�N�g�̕ύX
	ObjChange();

	// ���S����
	Dead();

	if (!m_bGame)
	{// �Q�[�����ł͂Ȃ�
		m_nCntFrame++;

		if (m_nCntFrame >= 120)
		{// ��莞�Ԍ�
			CApplication::SetNextMode(CApplication::MODE_RESULT);
		}
	}
}

//=============================================================================
// �`��
//=============================================================================
void CGame::Draw()
{
}

//=============================================================================
// �I�u�W�F�N�g�̕ύX
//=============================================================================
void CGame::ObjChange()
{
	if (CApplication::GetJoy()->GetUseJoyPad() >= 1)
	{// �R���g���[���[���q���ł���
		m_pTutorial->LoadTex(31);
	}
	else
	{// �R���g���[���[���q���łȂ�
		m_pTutorial->LoadTex(32);
	}
}

//=============================================================================
// ���S����
//=============================================================================
void CGame::Dead()
{
	int numPlayer = CApplication::GetPersonCount();
	bool dead = true;

	for (int i = 0; i < numPlayer; i++)
	{
		if (!m_pPlayer[i]->GetDead())
		{// ����ł��Ȃ�
			dead = false;
			break;
		}
	}

	if (dead)
	{// �S������
		m_bGame = false;

		// ���Ԃ��~�߂�
		m_pTime->StopTime(true);

		// �|�[�Y�ł��Ȃ�����
		CApplication::GetPause()->SetSelect(false);
	}
}
