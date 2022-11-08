//=============================================================================
//
// �^�C�g���N���X(title.cpp)
// Author : �������l
// �T�v : �^�C�g���N���X�̊Ǘ����s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "title.h"

#include "application.h"
#include "keyboard.h"
#include "mouse.h"
#include "debug_proc.h"
#include "joypad.h"
#include "sound.h"
#include "object2D.h"

#include "camera_manager.h"
#include "renderer.h"
#include "object.h"
#include "object3D.h"

//���f��
#include "player.h"
#include "Shark.h"
#include "model3D.h"
#include "mesh.h"
#include "sphere.h"
#include "model_obj.h"

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CTitle::CTitle()
{
	m_nextMode = MODE_NONE;		// ���ɐݒ肷�郂�[�h
	m_pTitleLogo = nullptr;		// �^�C�g�����S�I�u�W�F�N�g
	m_pNewGame = nullptr;		// �j���[�Q�[���I�u�W�F�N�g
	m_pExit = nullptr;			// �I���I�u�W�F�N�g
	m_fAddAlpha = 0.0f;			// �t���[�����̃J�E���g
	m_fAddSize = 0.0f;			// �傫���̎Q�ƒl
	m_nCntFrame = 0;			// �t���[���J�E���g
	m_nNumPlayer = 0;			// �v���C���[��
	m_bPressEnter = true;		// �G���^�[�L�[�������邩
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CTitle::Init()
{// �}�E�X�̎擾
	CMouse *pMouse = CApplication::GetMouse();

	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// �^�C�g��BGM�̍Đ�
	pSound->PlaySound(CSound::SOUND_LABEL_TITELBGM);

	// �^�C�g�����S
	m_pTitleLogo = CObject2D::Create();
	m_pTitleLogo->SetPos(D3DXVECTOR3(640.0f, 280.0f, 0.0f));
	m_pTitleLogo->SetSize(D3DXVECTOR3(800.0f, 300.0f, 0.0f));
	m_pTitleLogo->LoadTex(20);

	// �J�����̈ʒu�ύX
	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetPosV(D3DXVECTOR3(0.0f, 300.0f, -1600.0f));
	pCamera->SetPosR(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
	pCamera->SetViewing(20.0f);

	// �n�ʂ̐ݒ�
	CMesh3D *pMesh3D[2];
	pMesh3D[0] = CMesh3D::Create();
	pMesh3D[0]->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D[0]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D[0]->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	pMesh3D[0]->SetBlock(CMesh3D::DOUBLE_INT(10, 10));
	pMesh3D[0]->SetSplitTex(true);
	pMesh3D[0]->SetScrollTex(D3DXVECTOR2(0.0f, 0.01f), true);
	pMesh3D[0]->LoadTex(2);

	// ���b�V���̐���
	pMesh3D[1] = CMesh3D::Create();
	pMesh3D[1]->SetPos(D3DXVECTOR3(0.0f, 11.0f, 0.0f));
	pMesh3D[1]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pMesh3D[1]->SetSize(D3DXVECTOR3(10000.0f, 0, 10000.0f));
	pMesh3D[1]->SetBlock(CMesh3D::DOUBLE_INT(1, 3000));
	pMesh3D[1]->SetSplitTex(false);
	pMesh3D[1]->SetWave(7.0f, 10.0f);
	pMesh3D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
	pMesh3D[1]->LoadTex(3);

	// �X�J�C�{�b�N�X�̐ݒ�
	CSphere *pSphere = CSphere::Create();
	pSphere->SetRot(D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));
	pSphere->SetSize(D3DXVECTOR3(100.0f, 0, 100.0f));
	pSphere->SetBlock(CMesh3D::DOUBLE_INT(100, 100));
	pSphere->SetRadius(5000.0f);
	pSphere->SetSphereRange(D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * -0.5f));
	pSphere->LoadTex(4);

	//�T���ݒ�
	CEnemyShark *pEnemyShark = CEnemyShark::Create();
	pEnemyShark->SetMotion("data/MOTION/motionShark.txt");
	pEnemyShark->SetPos(D3DXVECTOR3(0.0f, -200.0f, 1500.0f));
	pEnemyShark->SetRot(D3DXVECTOR3(D3DX_PI * 0.05f, 0.0f, 0.0f));

	// ���f���̐ݒu
	CModelObj::LoadFile("data/FILE/setModel.txt");

	// �}�E�X�J�[�\��������
	pMouse->SetShowCursor(false);

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CTitle::Uninit()
{
	// �T�E���h���̎擾
	CSound *pSound = CApplication::GetSound();

	// �T�E���h�I��
	pSound->StopSound();

	// �X�R�A�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CTitle::Update()
{
	// ���͏��̎擾
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	// �W���C�p�b�h ���̎擾
	CJoypad *pJoy = CApplication::GetJoy();

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		CApplication::SetNextMode(CApplication::MODE_GAME);
	}

	m_fAddSize += 0.05f;
	D3DXVECTOR3 sizeTitleLogo = m_pTitleLogo->GetSize();
	m_pTitleLogo->SetSize(D3DXVECTOR3(sizeTitleLogo.x + sinf(m_fAddSize) * 3.0f,
		sizeTitleLogo.y + sinf(m_fAddSize) * 2.0f, 0.0f));

	//�Q�b�g
	CDebugProc::Print("PersonCount Q or E: %d\n", m_nNumPlayer);

	int nMaxPlayer = pJoy->GetUseJoyPad();

	if (nMaxPlayer >= 0)
	{
		nMaxPlayer = 1;
	}

	if (pKeyboard->GetTrigger(DIK_Q))
	{
		m_nNumPlayer++;
	}
	if (pKeyboard->GetTrigger(DIK_E))
	{
		m_nNumPlayer--;
	}

	if (m_nNumPlayer > nMaxPlayer)	// 4���z���Ȃ��悤
	{
		m_nNumPlayer = 1;
	}
	if (m_nNumPlayer < 1)	// �}�C�i�X�ݒ薳��
	{
		m_nNumPlayer = nMaxPlayer;
	}

	CApplication::SetPersonCount(m_nNumPlayer);
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CTitle::Draw()
{

}