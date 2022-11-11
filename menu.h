//**************************************************
// 
// menu.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _MENU_H_	//���̃}�N����`������ĂȂ�������
#define _MENU_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include <d3dx9.h>
#include "object.h"

//==================================================
// �O���錾
//==================================================
class CObject2D;

//==================================================
// ��`
//==================================================
class CMenu : public CObject
{
	/* ����`�� */
private:
	static const float CURSOR_INTERVAL;	// �J�[�\���̊Ԋu

	/* ���ÓI�����o�֐��� */
public:
	static CMenu* Create();	// ����

	/* �������o�֐��� */
public:
	CMenu();			// �f�t�H���g�R���X�g���N�^
	~CMenu() override;	// �f�X�g���N�^

public:
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Draw() override;		// ���
	void Update() override;		// �X�V
	void SetPos(const D3DXVECTOR3 &pos) override;									// �ʒu�̃Z�b�^�[
	D3DXVECTOR3 GetPos() override { return m_pos; }									// �ʒu�̃Q�b�^�[
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// �ߋ��ʒu�̃Z�b�^�[
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// �ߋ��ʒu�̃Q�b�^�[
	void SetRot(const D3DXVECTOR3 &rot) override;									// �����̃Z�b�^�[
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// �����̃Q�b�^�[
	void SetSize(const D3DXVECTOR3 &size) override;									// �傫���̃Z�b�^�[
	D3DXVECTOR3 GetSize()  override { return m_size; }								// �傫���̃Q�b�^�[
	int Select();		// �I��
	void ColorChange();	// �F�̕ύX
	void CMenu::SetTexture(int idx, int texIdx);	// �e�N�X�`���̐ݒ�
	void SetFrame(const D3DXVECTOR3& size, const D3DXCOLOR& col);	// �g�̐ݒ�
	int GetSelectIdx();	// �I������Ă���ԍ��̎擾

	//--------------------------------------------------
	// ����
	// pos / �ʒu
	// size / �T�C�Y
	// numUse / �I�����̎g�p��
	// interval / �Ԋu
	// sort / ���ו� [ true : �c false : �� ]
	// cursor / �J�[�\�����g�����ǂ���
	//--------------------------------------------------
	void Set(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, int numUse, float interval, bool sort, bool cursor);	// �ݒ�

private:
	void Add(int add);

	/* �������o�ϐ��� */
private:
	CObject2D** m_pOption;	// �I����
	CObject2D* m_pFrame;	// �g
	CObject2D* m_pCursor;	// �J�[�\��
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �ߋ��̈ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_size;		// �傫��
	int m_selectIdx;		// �I������Ă���ԍ�
	int m_numUse;			// �g�p��
	int m_time;				// �^�C��
	float m_cycle;			// �J�[�u�̎���
	bool m_sort;			// ���ו� [ true : �c false : �� ]
};

#endif // !_MENU_H_
