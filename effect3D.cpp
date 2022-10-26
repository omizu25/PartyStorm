//=============================================================================
//
// 3D�G�t�F�N�g�N���X(effect3D.cpp)
// Author : �������l
// �T�v : 3D�G�t�F�N�g�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "effect3D.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : 3D�G�t�F�N�g�𐶐�����
//=============================================================================
CEffect3D * CEffect3D::Create(void)
{
	// �I�u�W�F�N�g�C���X�^���X
	CEffect3D *pEffect3D = nullptr;

	// �������̉��
	pEffect3D = new CEffect3D;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pEffect3D != nullptr);

	// ���l�̏�����
	pEffect3D->Init();

	// �C���X�^���X��Ԃ�
	return pEffect3D;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CEffect3D::CEffect3D()
{
	m_renderMode = MODE_NORMAL;								// �����_�[�X�e�[�g�̌v�Z���@
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ړ���
	m_moveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ�����
	m_subSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �傫���̌�����
	m_fSpeed = 0.0f;										// ���x
	m_fSubAlpha = 0.0f;										// �A���t�@�l�̌�����
	m_nLife = 0;											// ����
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CEffect3D::Init()
{
	// ������
	CObject3D::Init();

	// �e�N�X�`���̐ݒ�
	LoadTex(-1);

	// Z�o�b�t�@�̐ݒ�
	SetZFunc(D3DCMP_ALWAYS);

	// ���e�X�g�̓��ߗ��̐ݒ�
	SetAlphaValue(0);

	// �r���{�[�h�I��
	SetBillboard(true);

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CEffect3D::Uninit()
{
	// �I��
	CObject3D::Uninit();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CEffect3D::Update()
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	// �ړ��ʂ̎Z�o
	m_move.z = sinf(m_moveVec.x) * cosf(m_moveVec.y) * m_fSpeed;
	m_move.x = sinf(m_moveVec.x) * sinf(m_moveVec.y) * m_fSpeed;
	m_move.y = cosf(m_moveVec.x) * m_fSpeed;

	// �ʒu�̍X�V
	pos += m_move;

	// �ʒu�̍X�V
	SetPos(pos);

	// ���C�t�̌���
	DecreaseLife();

	if (m_nLife == 0)
	{// �I��
		Uninit();
	}
	else
	{// �X�V
		CObject3D::Update();
	}
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CEffect3D::Draw()
{// �����_���[�̃Q�b�g
	CRenderer *pRenderer = CApplication::GetRenderer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	switch (m_renderMode)
	{
	case CEffect3D::MODE_NORMAL:
		break;

	case CEffect3D::MODE_ADD:
		// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	case CEffect3D::MODE_SUB:
		// ���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	default:
		break;
	}

	// �`��
	CObject3D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// �����̐ݒ�
// Author : �������l
// �T�v : �����̐ݒ�
//=============================================================================
void CEffect3D::SetLife(const int nLife)
{
	m_nLife = nLife;
	m_subSize = GetSize() / (float)m_nLife;
	m_fSubAlpha = GetColor().a / m_nLife;
}

//=============================================================================
// ���C�t�̌�������
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CEffect3D::DecreaseLife()
{
	if (m_nLife > 0)
	{
		D3DXVECTOR3 size = GetSize();
		D3DXCOLOR color = GetColor();

		// ���C�t�̌���
		m_nLife--;
		SetSize(size - m_subSize);
		SetColor(D3DXCOLOR(color.r, color.g, color.b, color.a - m_fSubAlpha));

		if (m_nLife <= 0)
		{
			m_nLife = 0;
		}
	}
}

