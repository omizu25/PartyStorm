//=============================================================================
//
// �e�N���X(shadow.cpp)
// Author : �������l
// �T�v : �e�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "shadow.h"
#include "renderer.h"
#include "application.h"
#include "calculation.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : 3D�G�t�F�N�g�𐶐�����
//=============================================================================
CShadow * CShadow::Create(CObject *pParent)
{
	// �I�u�W�F�N�g�C���X�^���X
	CShadow *pShadow = nullptr;

	// �������̉��
	pShadow = new CShadow;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pShadow != nullptr);

	// ���l�̏�����
	pShadow->Init();

	// �e�̐ݒ�
	pShadow->SetParent(pParent);

	// �C���X�^���X��Ԃ�
	return pShadow;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CShadow::CShadow() : m_pParent(nullptr)			// �e
{

}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CShadow::Init()
{
	// ������
	CObject3D::Init();

	// �e�N�X�`���̐ݒ�
	LoadTex(-1);

	// Z�e�X�g�̗D��x�̐ݒ�
	SetZFunc(D3DCMP_LESSEQUAL);

	// �A���t�@�e�X�g�̓��ߗ��̐ݒ�
	SetAlphaValue(1);

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CShadow::Uninit()
{
	// �I��
	CObject3D::Uninit();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CShadow::Update()
{
	// �e�̈ʒu�̎擾
	D3DXVECTOR3 posParent = m_pParent->GetPos();
	posParent.y = 0.5f;

	// �e�̌����̎擾
	D3DXVECTOR3 rotParent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotParent.x += m_pParent->GetRot().x + D3DX_PI * 0.5f;
	rotParent.x = CCalculation::RotNormalization(rotParent.x);

	// �ʒu�̐ݒ�
	SetPos(posParent);

	// �����̐ݒ�
	SetRot(rotParent);

	// �X�V
	CObject3D::Update();
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CShadow::Draw()
{// �����_���[�̃Q�b�g
	CRenderer *pRenderer = CApplication::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �`��
	CObject3D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
