//=============================================================================
//
// ���f���I�u�W�F�N�g�N���X(model_obj.cpp)
// Author : �������l
// �T�v : ���f���I�u�W�F�N�g�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "model_obj.h"
#include "model3D.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// �C���X�^���X����
// Author : �������l
// �T�v : ���[�V�����L�����N�^�[3D�𐶐�����
//=============================================================================
CModelObj * CModelObj::Create()
{
	// �I�u�W�F�N�g�C���X�^���X
	CModelObj *pModelObj = nullptr;

	// �������̉��
	pModelObj = new CModelObj;

	// �������̊m�ۂ��ł��Ȃ�����
	assert(pModelObj != nullptr);

	// ���l�̏�����
	pModelObj->Init();

	// �C���X�^���X��Ԃ�
	return pModelObj;
}

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CModelObj::CModelObj() : m_pModel(nullptr),					// ���f�����
m_mtxWorld(D3DXMATRIX()),									// ���[���h�}�g���b�N�X
m_pos(D3DXVECTOR3()),										// �ʒu
m_posOld(D3DXVECTOR3()),									// �ߋ��ʒu
m_rot(D3DXVECTOR3()),										// ����
m_size(D3DXVECTOR3()),										// �傫��
m_nType(-1)													// ���f���̃^�C�v
{
	
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CModelObj::~CModelObj()
{

}

//=============================================================================
// ������
// Author : �������l
// �T�v : ���_�o�b�t�@�𐶐����A�����o�ϐ��̏����l��ݒ�
//=============================================================================
HRESULT CModelObj::Init()
{
	// �ϐ��̏�����
	m_mtxWorld = {};										// ���[���h�}�g���b�N�X
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ߋ��ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ����
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);					// �傫��
	m_nType = -1;											// ���f���̃^�C�v

	// ���f���N���X�̃������m��
	m_pModel = CModel3D::Create();
	assert(m_pModel != nullptr);
	m_pModel->SetModelID(m_nType);

	return E_NOTIMPL;
}

//=============================================================================
// �I��
// Author : �������l
// �T�v : �e�N�X�`���̃|�C���^�ƒ��_�o�b�t�@�̉��
//=============================================================================
void CModelObj::Uninit()
{
	if (m_pModel != nullptr)
	{// �I������
		m_pModel->Uninit();

		// �������̉��
		delete m_pModel;
		m_pModel = nullptr;
	}

	// �I�u�W�F�N�g�̉��
	Release();
}

//=============================================================================
// �X�V
// Author : �������l
// �T�v : �X�V���s��
//=============================================================================
void CModelObj::Update()
{
	// ���f���̍X�V
	m_pModel->Update();
}

//=============================================================================
// �`��
// Author : �������l
// �T�v : �`����s��
//=============================================================================
void CModelObj::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);											// �s�񏉊����֐�

	// �����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);			// �s���]�֐�
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);						// �s��|���Z�֐� 

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);				// �s��ړ��֐�
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);					// �s��|���Z�֐�

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	m_pModel->Draw(m_mtxWorld);
}

//=============================================================================
// ���f���̃^�C�v�̐ݒ�
// Author : �������l
// �T�v : ���f���̃^�C�v�̐ݒ�
//=============================================================================
void CModelObj::SetType(const int nType)
{
	// ���f���̃^�C�v
	m_nType = nType;

	// ���f���N���X��ID�ݒ�
	m_pModel->SetModelID(m_nType);
}

