//**************************************************
// 
// ranking.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _RANKING_H_	//���̃}�N����`������ĂȂ�������
#define _RANKING_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==================================================
// �C���N���[�h
//==================================================
#include <d3dx9.h>
#include "object.h"

//==================================================
// �O���錾
//==================================================
class CScore;
class CObject2D;

//==================================================
// ��`
//==================================================
class CRanking : public CObject
{
	/* ����`�� */
public:
	static const int MAX_RANKING = 5;	// �����L���O�̍ő吔
	static const float STD_WIDTH;		// ���̕W���l
	static const float STD_HEIGHT;		// �����̕W���l

private:
	static const char* FILE_NAME;	// �t�@�C���p�X

	/* ���ÓI�����o�֐��� */
public:
	static CRanking* Create();	// ����
	static void Set(int score);	// �ݒ�
	static int Get(int rank);	// �擾

private:
	static void Load();		// �ǂݍ���
	static void Save();		// �ۑ�
	static void Change();	// �ύX

	/* ���ÓI�����o�ϐ��� */
private:
	static int m_score;	// ����̃X�R�A
	static int m_ranking[MAX_RANKING];	// �����L���O

	/* �������o�֐��� */
public:
	CRanking();				// �f�t�H���g�R���X�g���N�^
	~CRanking() override;	// �f�X�g���N�^

public:
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update() override;		// �X�V
	void Draw() override;		// �`��
	void Set(const D3DXVECTOR3& pos, float length);			// �ݒ�
	void SetPos(const D3DXVECTOR3 &pos) override;			// �ʒu�̐ݒ�
	D3DXVECTOR3 GetPos() override { return m_pos; }			// �ʒu�̎擾
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }	// �ߋ��ʒu�̎擾
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }	// �ߋ��ʒu�̐ݒ�
	void SetRot(const D3DXVECTOR3 &rot) override;			// �����̐ݒ�
	D3DXVECTOR3 GetRot()  override { return m_rot; }		// �����̎擾
	void SetSize(const D3DXVECTOR3 &size) override;			// �傫���̐ݒ�
	D3DXVECTOR3 GetSize()  override { return m_size; }		// �傫���̎擾

private:
	int Digit(int number);	// ����

	/* �������o�ϐ��� */
private:
	CScore* m_pRanking[MAX_RANKING];	// �����L���O
	CObject2D* m_pRank[MAX_RANKING];	// ��
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �ߋ��̈ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_size;		// �傫��
	int m_newRank;			// �V�����X�R�A
	int m_time;				// �^�C��
};

#endif // !_RANKING_UI_H_
