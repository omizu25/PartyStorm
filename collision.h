//=============================================================================
//
// �����蔻��N���X(collision.h)
// Author : �������l
// �T�v : �����蔻�萶�����s��
//
//=============================================================================
#ifndef _COLLISION_H_			// ���̃}�N����`������ĂȂ�������
#define _COLLISION_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"
#include "main.h"

//=============================================================================
// �����蔻��N���X
// Author : �������l
// �T�v : �����蔻�萶�����s���N���X
//=============================================================================
class CCollision : public CObject
{
public:
	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CCollision();
	~CCollision();

	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	HRESULT Init() override;																			// ������
	void Uninit() override;																				// �I��
	void Update() override;																				// �X�V
	void Draw() override;																				// �`��
	void SetParent(CObject *pParent);																	// �e�̃Z�b�^�[
	void SetPos(const D3DXVECTOR3 &pos) override;														// �ʒu�̃Z�b�^�[
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }							// �ߋ��ʒu�̃Z�b�^�[
	void SetRot(const D3DXVECTOR3 &rot) override;														// �����̃Z�b�^�[
	void SetSize(const D3DXVECTOR3 &size) override;														// �傫���̃Z�b�^�[
	bool ColisonRange2D(CObject *target);																// �͈͂̔���(2D)
	bool ColisonRectangle2D(CObject *target, bool bExtrude);											// ��`�̔���(2D)
	bool ColisonCircle2D(CObject *target, bool bExtrude);												// �~�̔���(2D)
	bool ColisonRectangle3D(CObject *target, bool bExtrude);											// ��`�̔���(3D)
	bool ColisonSphere3D(CObject *target, D3DXVECTOR3 size, D3DXVECTOR3 targetSize, bool bExtrude);		// ���̔���(3D)
	CObject	*GetParent() { return m_pParent; }															// �e�̃Q�b�^�[
	D3DXVECTOR3 GetPos() override { return m_pos; }														// �ʒu�̃Q�b�^�[
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }												// �ߋ��ʒu�̃Q�b�^�[
	D3DXVECTOR3 GetRot()  override { return m_rot; }													// �����̃Q�b�^�[
	D3DXVECTOR3 GetSize()  override { return m_size; }													// �傫���̃Q�b�^�[

private:
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	CObject				*m_pParent;			// �e
	D3DXVECTOR3			m_pos;				// �ʒu
	D3DXVECTOR3			m_posOld;			// �ߋ��ʒu
	D3DXVECTOR3			m_posAdd;			// �ʒu�̒ǉ���
	D3DXVECTOR3			m_rot;				// ����
	D3DXVECTOR3			m_size;				// �傫��
};

#endif
