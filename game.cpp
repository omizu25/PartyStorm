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
#include "inputcontroller.h"

#include "application.h"
#include "camera_manager.h"
#include "renderer.h"
#include "object.h"
#include "object3D.h"

//���f��
#include "player.h"
#include "Shark.h"
#include "obstacle.h"

#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "model_obj.h"
#include "score.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CPlayer *CGame::m_pPlayer = nullptr;					// �v���C���[�N���X
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
	CCalculation::SetGravity(10.0f);

	// �n�ʂ̐ݒ�
	m_pMesh3D = CMesh3D::Create();
	m_pMesh3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pMesh3D->SetSize(D3DXVECTOR3(5000.0f, 0, 5000.0f));
	m_pMesh3D->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	m_pMesh3D->SetSplitTex(true);
	/*m_pMesh3D->SetWave(1.0f, 300.0f);*/
	m_pMesh3D->LoadTex(-1);

	// �X�J�C�{�b�N�X�̐ݒ�
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(50000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(-1);

	// �v���C���[�̐ݒ�
	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetMotion("data/MOTION/motion.txt");
	m_pPlayer->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�T���ݒ�
	m_pEnemyShark = CEnemyShark::Create();
	m_pEnemyShark->SetMotion("data/MOTION/motionShark.txt");
	m_pEnemyShark->SetPos(D3DXVECTOR3(0.0f, 0.0f, 200.0f));

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
	CInputController *pjoy = CApplication::GetJoy();
	if (pjoy->GetJoypadTrigger(CInputController::JOYKEY_A, 0))
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
