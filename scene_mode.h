//=============================================================================
//
// �V�[�����[�h�N���X(scene_mode.h)
// Author : �������l
// �T�v : �V�[�����[�h�̔h�����s��
//
//=============================================================================
#ifndef _SCENE_MODE_H_		// ���̃}�N����`������ĂȂ�������
#define _SCENE_MODE_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//=============================================================================
// �V�[�����[�h�N���X
// Author : �������l
// �T�v : �V�[�����[�h�������s���N���X
//=============================================================================
class CSceneMode : public CObject
{
public:
	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CSceneMode();
	~CSceneMode() override;

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init() override;														// ������
	void Uninit() override;															// �I��
	void Update() override;															// �X�V
	void Draw() override;															// �`��
	void SetPos(const D3DXVECTOR3 &pos) override { pos; }							// �ʒu�̃Z�b�^�[
	void SetPosOld(const D3DXVECTOR3 &posOld) override { posOld; }					// �ߋ��ʒu�̃Z�b�^�[
	void SetRot(const D3DXVECTOR3 &rot) override { rot; }							// �����̃Z�b�^�[
	void SetSize(const D3DXVECTOR3 &size) override { size; }						// �傫���̃Z�b�^�[
	D3DXVECTOR3 GetPos() override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			// �ʒu�̃Q�b�^�[
	D3DXVECTOR3 GetPosOld()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �ߋ��ʒu�̃Q�b�^�[
	D3DXVECTOR3 GetRot()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �����̃Q�b�^�[
	D3DXVECTOR3 GetSize()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// �傫���̃Q�b�^�[
};

#endif

