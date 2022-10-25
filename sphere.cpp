//=============================================================================
//
// �X�t�B�A�N���X(sphere.cpp)
// Author : �������l
// �T�v : �X�t�B�A�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "sphere.h"
#include "calculation.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : �X�t�B�A�𐶐�����
//=============================================================================
CSphere * CSphere::Create(void)
{
	// �X�t�B�A�C���X�^���X
	CSphere *pSphere = nullptr;

	// �������̉��
	pSphere = new CSphere;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pSphere != nullptr);

	// ���l�̏�����
	pSphere->Init();

	// �C���X�^���X��Ԃ�
	return pSphere;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CSphere::CSphere()
{
	m_sphereRange = D3DXVECTOR2(0.0f,0.0f);
	m_fRadius = 0.0f;
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CSphere::~CSphere()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CSphere::Init()
{
	// ������
	CMesh3D::Init();

	// �����l�̐ݒ�
	m_sphereRange = D3DXVECTOR2(D3DX_PI * 2.0f, D3DX_PI * 0.5f);
	m_fRadius = 50.0f;

	// �e�N�X�`���̐ݒ�
	LoadTex(-1);

	return S_OK;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CSphere::Uninit()
{// �I��
	CMesh3D::Uninit();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : 2D�X�V���s��
//=============================================================================
void CSphere::Update()
{// �X�V
	CMesh3D::Update();
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : 2D�`����s��
//=============================================================================
void CSphere::Draw()
{// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ���C�g�𖳌�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �`��
	CMesh3D::Draw();

	// ���C�g��L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���_���W�Ȃǂ̐ݒ�
// Author : �������l
// �T�v : 3D���_���W�Anor�A���_�J���[��ݒ肷��
//=============================================================================
void CSphere::SetVtx()
{
	// ���̎擾
	DOUBLE_INT block = GetBlock();
	DOUBLE_INT line = GetLine();

	// ���_���̎擾
	VERTEX_3D *pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < line.y; nCntZ++)
	{
		for (int nCntX = 0; nCntX < line.x; nCntX++)
		{// �ϐ��錾
			int nCntVtx = nCntX + (nCntZ *  line.x);
			float fRot = (m_sphereRange.x / block.x) * nCntX;					// Y���̊p�x�̐ݒ�
			float fHalfRot = (m_sphereRange.y / block.y) * nCntZ;				// ������Z���̊p�x�̔���

			// �����Ɣ��a�̐ݒ�
			D3DXVECTOR2 radius = D3DXVECTOR2(sinf(fHalfRot) * m_fRadius, cosf(fHalfRot) * m_fRadius);

			// ���_���W�̐ݒ�
			pVtx[nCntVtx].pos.x = sinf(fRot) * radius.y;
			pVtx[nCntVtx].pos.z = cosf(fRot) * radius.y;
			pVtx[nCntVtx].pos.y = radius.x;

			/*pVtx[nCntVtx].pos.z = sinf(fHalfRot) * cosf(fRot) * m_fRadius;
			pVtx[nCntVtx].pos.x = sinf(fHalfRot) * sinf(fRot) * m_fRadius;
			pVtx[nCntVtx].pos.y = cosf(fHalfRot) * m_fRadius;*/

			// ���[���h���W�ɃL���X�g
			CCalculation::WorldCastVtx(pVtx[nCntVtx].pos, GetPos(), GetRot());

			// �e���_�̖@���̐ݒ�(*�x�N�g���̑傫����1�ɂ���K�v������)
			pVtx[nCntVtx].nor.x = pVtx[nCntVtx].pos.x;
			pVtx[nCntVtx].nor.z = pVtx[nCntVtx].pos.z;
			pVtx[nCntVtx].nor.y = 0.0f;
			D3DXVec3Normalize(&pVtx[nCntVtx].nor, &pVtx[nCntVtx].nor);

			// ���_�J���[�̐ݒ�
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
// Author : �������l
// �T�v : 3D���b�V���̃e�N�X�`�����W��ݒ肷��
//=============================================================================
void CSphere::SetTex()
{
	// ���̎擾
	DOUBLE_INT block = GetBlock();
	DOUBLE_INT line = GetLine();
	D3DXVECTOR2 tex = GetTex();

	// ���_���̎擾
	VERTEX_3D *pVtx = NULL;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < line.y; nCntZ++)
	{
		for (int nCntX = 0; nCntX < line.x; nCntX++)
		{// �ϐ��錾
			int nCntVtx = nCntX + (nCntZ *  line.x);

			if (GetSplitTex())
			{// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntX + tex.x, 1.0f * (line.y - nCntZ) + tex.y);
			}
			else
			{// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f / block.x * nCntX + tex.x, 1.0f / block.y * (line.y - nCntZ) + tex.y);
			}
		}
	}

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���̔��a�̐ݒ�
// Author : �������l
// �T�v : ���̔��a��ݒ肷��
//=============================================================================
void CSphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;

	// ���_���W�̐ݒ�
	SetVtx();
}

//=============================================================================
// ���̕`��͈͂̐ݒ�
// Author : �������l
// �T�v : ���̕`��͈͂�ݒ肷��
//=============================================================================
void CSphere::SetSphereRange(D3DXVECTOR2 sphereRange)
{
	m_sphereRange = sphereRange;

	// ���_���W�̐ݒ�
	SetVtx();
}
