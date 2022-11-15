//**************************************************
// 
// obstacle_manager.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _OBSTACLE_MANAGER_H_	// ���̃}�N����`������ĂȂ�������
#define _OBSTACLE_MANAGER_H_	// ��d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include "object.h"
#include <d3dx9.h>

//==================================================
// �N���X
//==================================================
class CObstacleManager : public CObject
{
	/* ����`�� */
private:
	
	/* ���ÓI�����o�֐��� */
public:
	static CObstacleManager* Create();	// ����

	/* �������o�֐��� */
public:
	CObstacleManager();				// �f�t�H���g�R���X�g���N�^
	~CObstacleManager() override;	// �f�X�g���N�^

public:
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update() override;		// �X�V
	void Draw() override;		// �`��
	void SetPos(const D3DXVECTOR3 &pos) override { m_pos = pos; }				// �ʒu�̐ݒ�
	D3DXVECTOR3 GetPos() override { return m_pos; }								// �ʒu�̎擾
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }	// �ߋ��̈ʒu�̐ݒ�
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }						// �ߋ��̈ʒu�̎擾
	void SetRot(const D3DXVECTOR3 &rot) override { m_rot = rot; }				// �����̐ݒ�
	D3DXVECTOR3 GetRot()  override { return m_rot; }							// �����̎擾
	void SetSize(const D3DXVECTOR3 &size) override { m_size = size; };			// �傫���̐ݒ�
	D3DXVECTOR3 GetSize()  override { return m_size; }							// �傫���̎擾

public:
	void SetGame(bool game) { m_game = game; }	// �Q�[�������ǂ����ݒ�
	void SetStop(bool stop) { m_stop = stop; }	// �o�����~�߂邩�ǂ����̐ݒ�

private:
	void Game();	// �Q�[��
	void BG();		// �w�i

	/* �������o�ϐ��� */
private:
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �ߋ��̈ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_size;		// �傫��
	int m_time;		// ����
	int m_popIdxBG;	// �w�i�̏o�������ԍ�
	int m_pop;		// �o����
	int m_interval;	// �Ԋu
	float m_move;	// �ړ���
	bool m_game;	// �Q�[�������ǂ���
	bool m_stop;	// �o�����~�߂邩�ǂ���
};

#endif // !_OBSTACLE_MANAGER_H_
