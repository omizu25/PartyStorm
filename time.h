//=============================================================================
//
// �^�C���N���X(time.h)
// Author : �������l
// �T�v : �^�C���̐ݒ���s��
//
//=============================================================================
#ifndef _TIME_H_		// ���̃}�N����`������ĂȂ�������
#define _TIME_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject2D;
class CScore;

//=============================================================================
// �^�C���N���X
// Author : �������l
// �T�v : �^�C���������s���N���X
//=============================================================================
class CTime : public CObject
{
public:
	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static CTime *Create();			// �G�l���M�[�Q�[�W�̐���

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CTime(int nPriority = CObject::PRIORITY_2D);
	~CTime() override;

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init() override;														// ������
	void Uninit() override;															// �I��
	void Update() override;															// �X�V
	void Draw() override;															// �`��
	void SetPos(const D3DXVECTOR3 &pos) override;									// �ʒu�̃Z�b�^�[
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// �ߋ��ʒu�̃Z�b�^�[
	void SetRot(const D3DXVECTOR3 &rot) override;									// �����̃Z�b�^�[
	void SetSize(const D3DXVECTOR3 &size) override;									// �傫���̃Z�b�^�[
	void SetTime(const int nTime);													// ���Ԃ̃Z�b�^�[
	void SetCountDown();															// �J�E���g�_�E���̃Z�b�^�[
	D3DXVECTOR3 GetPos() override { return m_pos; }									// �ʒu�̃Q�b�^�[
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// �ߋ��ʒu�̃Q�b�^�[
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// �����̃Q�b�^�[
	D3DXVECTOR3 GetSize()  override { return m_size; }								// �傫���̃Q�b�^�[
	int GetTime() { return m_nTime; }												// ���Ԃ̃Q�b�^�[
	CScore *GetScore() { return m_pScore; }											// �X�R�A�̃Q�b�^�[
	void StopTime(bool bStop) { m_bStop = bStop; }									// ���Ԓ�~

private:
	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	CScore				*m_pScore;			// �X�R�A
	D3DXVECTOR3			m_pos;				// �ʒu
	D3DXVECTOR3			m_posOld;			// �ߋ��̈ʒu
	D3DXVECTOR3			m_rot;				// ����
	D3DXVECTOR3			m_size;				// �傫��
	int					m_nTime;			// ����
	int					m_nCntFrame;		// �t���[���J�E���g
	bool				m_bStop;			// ��~����
	bool				m_bCountDown;		// �J�E���g�_�E��
};

#endif




