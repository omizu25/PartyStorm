//=============================================================================
//
// ���U���g�N���X(result.cpp)
// Author : �������l
// Author : �������P
// �T�v : ���U���g�N���X�̊Ǘ����s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "result.h"
#include "object2D.h"
#include "sound.h"
#include "camera.h"
#include "sphere.h"
#include "player.h"
#include "Shark.h"
#include "model_obj.h"
#include "effect.h"
#include "renderer.h"
#include "score.h"
#include "ranking.h"
#include "menu.h"
#include "obstacle_manager.h"

//*****************************************************************************
// �ϐ���`
//*****************************************************************************
namespace
{
const int POP_TIME = 240;	// �o�������鎞��
const int texIdx[4]
{
	12,
	13,
	14,
	15
};
}

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer **CResult::m_pPlayer = nullptr;	// �v���C���[�N���X
CMenu *CResult::m_pMenu = nullptr;		// ���j���[�N���X
bool CResult::m_dead[4] = {};			// ���S�������ǂ���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult() :
	m_mode(CApplication::MODE_GAME),
	m_time(0),
	m_pop(false),
	m_pressEnter(false)
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
	int maxPlayer = CApplication::GetPersonCount();

	{// �T�E���h�̐ݒ�
		int gameover = 0;

		for (int i = 0; i < maxPlayer; i++)
		{
			if (m_dead[i])
			{// ���S����
				gameover++;
			}
		}

		if (gameover == maxPlayer &&
			maxPlayer > 1)
		{// �S�����S
			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GAMEOVERBGM);
		}
		else
		{
			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_RESULTBGM);
		}
	}

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
		pMesh3D->LoadTex(0);
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
		pMesh3D->LoadTex(1);
	}

	{// �X�J�C�{�b�N�X�̐ݒ�
		CSphere *pSphere = CSphere::Create();
		pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
		pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
		pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
		pSphere->SetRadius(5000.0f);
		pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
		pSphere->LoadTex(2);
	}

	{// �v���C���[�̐ݒ�
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
	}

	// ���f���̐ݒu
	CModelObj::LoadFile("data/FILE/setModel.txt");

	m_time = 0;
	m_pop = false;
	m_mode = CApplication::MODE_GAME;
	m_pressEnter = false;

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

		m_pMenu = nullptr;

		if (gameclear)
		{// �Q�[���N���A
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.5f, CRenderer::SCREEN_HEIGHT * 0.25f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(1000.0f, 300.0f, 0.0f));
			pObj->LoadTex(20);

			m_pop = true;

			{// ���j���[�̐���
				D3DXVECTOR3 pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.5f, (float)CRenderer::SCREEN_HEIGHT * 0.75f, 0.0f);
				D3DXVECTOR3 size = D3DXVECTOR3(350.0f, 100.0f, 0.0f);

				// ���j���[�̐���
				m_pMenu = CMenu::Create();

				// ���j���[�̐ݒ�
				m_pMenu->Set(pos, size, 2, 50.0f, true, true);

				// �g�̐ݒ�
				m_pMenu->SetFrame(D3DXVECTOR3(600.0f, 360.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));

				// �e�N�X�`���̐ݒ�
				m_pMenu->SetTexture(0, 8);
				m_pMenu->SetTexture(1, 11);
			}
		}
	}
	else
	{// �V���O���v���C
		{// �w�i
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.75f, (float)CRenderer::SCREEN_HEIGHT * 0.5f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(650.0f, (float)CRenderer::SCREEN_HEIGHT, 0.0f));
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
			pScore->SetSize(D3DXVECTOR3(60.0f, 60.0f, 0.0f));

			// �ʒu�̐ݒ�
			pScore->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.25f, CRenderer::SCREEN_HEIGHT * 0.35f, 0.0f));

			// �����̐ݒ�
			pScore->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			pScore->SetDestScore(CRanking::Get(-1));
		}

		{// �����L���O�̕���
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.75f, CRenderer::SCREEN_HEIGHT * 0.15f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(600.0f, 150.0f, 0.0f));
			pObj->LoadTex(22);
		}

		{// �����̃X�R�A�̕���
			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.25f, CRenderer::SCREEN_HEIGHT * 0.15f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(600.0f, 175.0f, 0.0f));
			pObj->LoadTex(23);
		}

		{// ���j���[�̐���
			D3DXVECTOR3 pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.25f, (float)CRenderer::SCREEN_HEIGHT * 0.75f, 0.0f);
			D3DXVECTOR3 size = D3DXVECTOR3(350.0f, 100.0f, 0.0f);

			// ���j���[�̐���
			m_pMenu = CMenu::Create();

			// ���j���[�̐ݒ�
			m_pMenu->Set(pos, size, 2, 50.0f, true, true);

			// �g�̐ݒ�
			m_pMenu->SetFrame(D3DXVECTOR3(600.0f, 360.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));

			// �e�N�X�`���̐ݒ�
			m_pMenu->SetTexture(0, 8);
			m_pMenu->SetTexture(1, 11);
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

	// ��Q���̔w�i
	CObstacleManager::Create();

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CResult::Uninit()
{
	// �T�E���h�I��
	CApplication::GetSound()->StopSound();

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

		delete[] m_pPlayer;
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
	m_time++;

	if (CApplication::GetPersonCount() > 1)
	{// �}���`�v���C
		Multi();
	}

	// �G�t�F�N�g�̍X�V
	CEffect::UpdateAll();

	if (!m_pressEnter)
	{// �G���^�[��������Ă��Ȃ�
		// �I��
		Select();
	}
	else
	{// �G���^�[�������ꂽ

		if (m_pMenu != nullptr)
		{// �I��
			// �F�̕ύX
			m_pMenu->ColorChange();
		}

		if (m_time >= 60)
		{// ��莞�Ԍ�
			CApplication::SetNextMode(m_mode);
		}
	}
}

//=============================================================================
// �`��
//=============================================================================
void CResult::Draw()
{
}

//=============================================================================+
// �I��
//=============================================================================
void CResult::Select()
{
	if (m_pMenu == nullptr)
	{// �I��
		return;
	}

	int mode = m_pMenu->Select();

	switch (mode)
	{
	case -1:
		break;

	case 0:
		m_mode = CApplication::MODE_GAME;
		m_pressEnter = true;
		m_time = 0;
		break;

	case 1:
		m_mode = CApplication::MODE_TITLE;
		m_pressEnter = true;
		m_time = 0;
		break;

	default:
		assert(false);
		break;
	}
}

//=============================================================================
// �}���`
//=============================================================================
void CResult::Multi()
{
	if (m_pop)
	{// �o������
		return;
	}

	if (m_time >= POP_TIME)
	{// �o�����鎞�Ԃ��z����
		int gameover = 0;
		int maxPlayer = CApplication::GetPersonCount();
		
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
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.5f, CRenderer::SCREEN_HEIGHT * 0.25f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(1000.0f, 300.0f, 0.0f));
			pObj->LoadTex(21);

			{// ���j���[�̐���
				D3DXVECTOR3 pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.5f, (float)CRenderer::SCREEN_HEIGHT * 0.75f, 0.0f);
				D3DXVECTOR3 size = D3DXVECTOR3(350.0f, 100.0f, 0.0f);

				// ���j���[�̐���
				m_pMenu = CMenu::Create();

				// ���j���[�̐ݒ�
				m_pMenu->Set(pos, size, 2, 50.0f, true, true);

				// �g�̐ݒ�
				m_pMenu->SetFrame(D3DXVECTOR3(600.0f, 360.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));

				// �e�N�X�`���̐ݒ�
				m_pMenu->SetTexture(0, 8);
				m_pMenu->SetTexture(1, 11);
			}
		}
		else
		{// �����c�肪����
			float posX = (float)(CRenderer::SCREEN_WIDTH * 0.5f) / ((maxPlayer - gameover) + 1);
			int count = 0;

			for (int i = 0; i < maxPlayer; i++)
			{// �v���C���[�̐���
				if (!m_dead[i])
				{// ���S���Ă��Ȃ�
					CObject2D *pObj = CObject2D::Create();
					pObj->SetPos(D3DXVECTOR3(((float)(CRenderer::SCREEN_WIDTH * 0.5f)) + posX + (posX * count), 200.0f, 0.0f));
					pObj->SetSize(D3DXVECTOR3(150.0f, 150.0f, 0.0f));
					pObj->LoadTex(texIdx[i]);
					count++;
				}
			}

			assert(count == (maxPlayer - gameover));

			CObject2D *pObj = CObject2D::Create();
			pObj->SetPos(D3DXVECTOR3(CRenderer::SCREEN_WIDTH * 0.74f, CRenderer::SCREEN_HEIGHT * 0.65f, 0.0f));
			pObj->SetSize(D3DXVECTOR3(600.0f, 300.0f, 0.0f));
			pObj->LoadTex(19);

			{// ���j���[�̐���
				D3DXVECTOR3 pos = D3DXVECTOR3((float)CRenderer::SCREEN_WIDTH * 0.25f, (float)CRenderer::SCREEN_HEIGHT * 0.5f, 0.0f);
				D3DXVECTOR3 size = D3DXVECTOR3(350.0f, 100.0f, 0.0f);

				// ���j���[�̐���
				m_pMenu = CMenu::Create();

				// ���j���[�̐ݒ�
				m_pMenu->Set(pos, size, 2, 100.0f, true, true);

				// �g�̐ݒ�
				m_pMenu->SetFrame(D3DXVECTOR3(550.0f, (float)CRenderer::SCREEN_HEIGHT, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));

				// �e�N�X�`���̐ݒ�
				m_pMenu->SetTexture(0, 8);
				m_pMenu->SetTexture(1, 11);
			}
		}

		m_pop = true;
	}
}
