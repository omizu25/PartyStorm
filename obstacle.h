//**************************************************
// 
// obstacle.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _OBSTACLE_H_	// ���̃}�N����`������ĂȂ�������
#define _OBSTACLE_H_	// ��d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include "model_obj.h"
#include "main.h"

//==================================================
// �N���X
//==================================================
class CObstacle : public CModelObj
{
	/* ���ÓI�����o�֐��� */
public:
	static void SetMove(float move) { m_move = move; }	// �ړ��ʂ̐ݒ�
	static CObstacle* Create(float posX, float posZ, float inverse, float diePosZ);	// ����

	/* ���ÓI�����o�ϐ��� */
private:
	static float m_move;	// �ړ���

	/* �������o�֐��� */
public:
	CObstacle();			// �f�t�H���g�R���X�g���N�^
	~CObstacle() override;	// �f�X�g���N�^

public:
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update() override;		// �X�V
	void Draw() override;		// �`��

private:
	void Move();		// �ړ�
	void Rot();			// ��]
	void Collison();	// �����蔻��

	/* �������o�ϐ��� */
private:
	int m_waveTime;		// �E�F�[�u�p�̎���
	float m_inverse;	// �t�̓������邩�ǂ���
	float m_diePosZ;	// �j������Z�̈ʒu
};

#endif // !_OBSTACLE_H_
