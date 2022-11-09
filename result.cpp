//=============================================================================
//
// ���U���g�N���X(result.cpp)
// Author : �������l
// �T�v : ���U���g�N���X�̊Ǘ����s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "result.h"
#include "keyboard.h"
#include "object2D.h"
#include "sound.h"
#include "camera.h"
#include "mesh.h"
#include "sphere.h"
#include "player.h"
#include "Shark.h"
#include "model_obj.h"
#include "move.h"
#include "effect.h"
#include "renderer.h"
#include "score.h"
#include "ranking.h"

//*****************************************************************************
// �ϐ���`
//*****************************************************************************
namespace
{
const int POP_TIME = 240;	// �o�������鎞��
const int texIdx[4]
{
	16,
	17,
	18,
	19
};
}

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer **CResult::m_pPlayer = nullptr;	// �v���C���[�N���X
bool CResult::m_dead[4] = {};			// ���S�������ǂ���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult() :
	m_nextMode(CApplication::MODE_NONE),
	m_time(0),
	m_pop(false)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// ���S�̏�����
//=============================================================================
void CResult::InitDead()
{
	for (int i = 0; i < 4; i++)
	{
		m_dead[i] = false;
	}
}

//=============================================================================
// ���S�̐ݒ�
//=============================================================================
void CResult::SetDead(int numPlayer)
{
	m_dead[numPlayer] = true;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CResult::Init()
{
	// ���ɍs�����[�h�̐ݒ�
	m_nextMode = CApplication::MODE_TITLE;

	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// ���U���gBGM�̍Đ�
	pSound->PlaySound(CSound::SOUND_LABEL_RESULTBGM);

	{// �J�����̈ʒu�ύX
		CCamera *pCamera = CApplication::GetCamera();
		pCamera->SetPosV(D3DXVECTOR3(0.0f, 300.0f, -1600.0f));
		pCamera->SetPosR(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
		pCamera->SetViewing(20.0f);
	}

	{// �n�ʂ̐ݒ�
		CMesh3D *pMesh3D = CMesh3D::Create();
		pMesh3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
		pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
		pMesh3D->SetSplitTex(true);
		pMesh3D->SetScrollTex(D3DXVECTOR2(0.0f, 0.01f), true);
		pMesh3D->LoadTex(2);
	}

	{// ���b�V���̐���
		CMesh3D *pMesh3D = CMesh3D::Create();
		pMesh3D->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
		pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pMesh3D->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
		pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
		pMesh3D->SetSplitTex(false);
		pMesh3D->SetWave(7.0f, 10.0f);
		pMesh3D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		pMesh3D->LoadTex(3);
	}

	{// �X�J�C�{�b�N�X�̐ݒ�
		CSphere *pSphere = CSphere::Create();
		pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
		pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
		pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
		pSphere->SetRadius(5000.0f);
		pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
		pSphere->LoadTex(4);
	}

	{// ���[�V�������f���̐ݒ�
		CMotionModel3D *pSnake = CMotionModel3D::Create();
		pSnake->SetMotion("data/MOTION/snake.txt");
		pSnake->SetPos(D3DXVECTOR3(400.0f, 0.0f, 1000.0f));
		pSnake->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		pSnake = CMotionModel3D::Create();
		pSnake->SetMotion("data/MOTION/snake.txt");
		pSnake->SetPos(D3DXVECTOR3(-400.0f, 0.0f, 1000.0f));
		pSnake->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

		pSnake = CMotionModel3D::Create();
		pSnake->SetMotion("data/MOTION/snake.txt");
		pSnake->SetPos(D3DXVECTOR3(600.0f, 0.0f, 1000.0f));
		pSnake->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		pSnake = CMotionModel3D::Create();
		pSnake->SetMotion("data/MOTION/snake.txt");
		pSnake->SetPos(D3DXVECTOR3(-600.0f, 0.0f, 1000.0f));
		pSnake->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	}

	// �v���C���[�̐ݒ�
	int maxPlayer = CApplication::GetPersonCount();
	m_pPlayer = new CPlayer*[maxPlayer];
	assert(m_pPlayer != nullptr);

	float posX = 500.0f / (maxPlayer + 1);

	for (int i = 0; i < maxPlayer; i++)
	{// �v���C���[�̐���
		m_pPlayer[i] = CPlayer::Create();
		m_pPlayer[i]->SetMotion("data/MOTION/motion.txt");
		m_pPlayer[i]->SetPos(D3DXVECTOR3(-250.0f + posX + (posX * i), 0.0f, -300.0f));
		m_pPlayer[i]->SetNum(i);

		if (maxPlayer > 1)
		{// �}���`�v���C
			if (m_dead[i])
			{// ���񂾃v���C���[
				m_pPlayer[i]->SetAction(true);
				m_pPlayer[i]->SetMove(false);
			}
			else
			{// �����Ă�v���C���[
				m_pPlayer[i]->SetAction(false);
			}
		}
		else
		{// �V���O���v���C
			m_pPlayer[i]->SetAction(false);
		}
	}

	// ���f���̐ݒu
	CModelObj::LoadFile("data/FILE/setModel.txt");

	m_time = 0;
	m_pop = false;

	bool gameclear = true;
	
	if (maxPlayer > 1)
	{// �}���`�v���C
		for (int i = 0; i < maxPlayer; i++)
		{
			if (m_dead[i])
			{// ���S����
				gameclear = false;
			}
		}

		if (gameclear)
		{// �Q�[���N���A
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.5f, CRenderer::SCREEN_HEIGHT * 0.5f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(1000.0f, 300.0f, 0.0f));
			pObj->LoadTex(24);

			m_pop = true;
		}
	}
	else
	{// �V���O���v���C
		{// �w�i
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.75f, CRenderer::SCREEN_HEIGHT * 0.5f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(650.0f, CRenderer::SCREEN_HEIGHT, 0.0f));
			pObj->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
			pObj->LoadTex(-1);
		}

		{// �����L���O�̐���
			CRanking *pRanking = CRanking::Create();
			assert(pRanking != nullptr);
			float width = (float)CRenderer::SCREEN_WIDTH;
			float height = (float)CRenderer::SCREEN_HEIGHT * 0.35f;
			pRanking->Set(D3DXVECTOR3(width, height, 0.0f), 40.0f);
		}

		{// �X�R�A�̐���
			CScore *pScore = CScore::Create(10, true);
			assert(pScore != nullptr);
			pScore->SetScore(0);

			// �傫���̐ݒ�
			pScore->SetWholeSize(D3DXVECTOR3(1000.0f, 50.0f, 0.0f));
			pScore->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));

			// �ʒu�̐ݒ�
			pScore->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.25f, 360.0f, 0.0f));

			// �����̐ݒ�
			pScore->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			pScore->SetDestScore(CRanking::Get(-1));
		}

		{// �����L���O�̕���
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.75f, CRenderer::SCREEN_HEIGHT * 0.15f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(600.0f, 150.0f, 0.0f));
			pObj->LoadTex(26);
		}

		{// �����̃X�R�A�̕���
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.25f, CRenderer::SCREEN_HEIGHT * 0.3f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(500.0f, 150.0f, 0.0f));
			pObj->LoadTex(27);
		}
	}

	{//�T���ݒ�
		CEnemyShark *pEnemyShark = CEnemyShark::Create();

		if (gameclear)
		{// �Q�[���N���A�A�V���O���v���C
			pEnemyShark->SetMotion("data/MOTION/motionShark.txt");
		}
		else
		{// �Q�[���I�[�o�[
			pEnemyShark->SetMotion("data/MOTION/motionShark.txt", 2);
		}

		pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
		pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));
	}

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CResult::Uninit()
{
	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// �T�E���h�I��
	pSound->StopSound();

	// �G�t�F�N�g�̏I��
	CEffect::ReleaseAll();

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
//=============================================================================
void CResult::Update()
{
	int maxPlayer = CApplication::GetPersonCount();
	
	if (maxPlayer > 1)
	{// �}���`�v���C
		Multi();
	}
	else
	{// �V���O���v���C
		Single();
	}
}

//=============================================================================
// �`��
//=============================================================================
void CResult::Draw()
{
}

//=============================================================================
// �V���O��
//=============================================================================
void CResult::Single()
{
	// ���͏��̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{// ENTER�������ꂽ
		CApplication::SetNextMode(m_nextMode);
	}

	// �G�t�F�N�g�̍X�V
	CEffect::UpdateAll();
}

//=============================================================================
// �}���`
//=============================================================================
void CResult::Multi()
{
	// �G�t�F�N�g�̍X�V
	CEffect::UpdateAll();

	if (m_pop)
	{// �o������
		// ���͏��̎擾
		CKeyboard *pKeyboard = CApplication::GetKeyboard();

		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// ENTER�������ꂽ
			CApplication::SetNextMode(m_nextMode);
		}

		return;
	}

	m_time++;

	if (m_time >= POP_TIME)
	{// �o�����鎞�Ԃ��z����
		int gameover = 0;
		int maxPlayer = CApplication::GetPersonCount();
		maxPlayer = 4;
		for (int i = 0; i < maxPlayer; i++)
		{
			if (m_dead[i])
			{// ���S����
				gameover++;
			}
		}

		if (gameover == maxPlayer)
		{// �S�����S
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.5f, CRenderer::SCREEN_HEIGHT * 0.5f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(1000.0f, 300.0f, 0.0f));
			pObj->LoadTex(25);
		}
		else
		{// �����c�肪����
			float posX = CRenderer::SCREEN_WIDTH / ((maxPlayer - gameover) + 1);
			int count = 0;

			for (int i = 0; i < maxPlayer; i++)
			{// �v���C���[�̐���
				if (!m_dead[i])
				{// ���S���Ă��Ȃ�
					CObject2D *pObj = CObject2D::Create();
					pObj->SetPos(D3DXVECTOR3(posX + (posX * count), 200.0f, 0.0f));
					pObj->SetSize(D3DXVECTOR3(150.0f, 150.0f, 0.0f));
					pObj->LoadTex(texIdx[i]);
					count++;
				}
			}

			assert(count == (maxPlayer - gameover));

			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.5f, CRenderer::SCREEN_HEIGHT * 0.65f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(600.0f, 300.0f, 0.0f));
			pObj->LoadTex(23);
		}

		m_pop = true;
	}
}
