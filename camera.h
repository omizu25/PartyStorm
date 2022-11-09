//**************************************************************************************************
//
// �J��������(camera.h)
// Auther�F�������l
// �T�v : �J�����̐ݒ菈��
//
//**************************************************************************************************
#ifndef _CAMERA_H_			// ���̃}�N����`������ĂȂ�������
#define _CAMERA_H_			// ��d�C���N���[�h�h�~�̃}�N����`

//***************************************************************************
// �C���N���[�h
//***************************************************************************
#include"main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CMove;
class CObject;

//=============================================================================
// �J�����N���X
// Author : �������l
// �T�v : �J�����ݒ���s���N���X
//=============================================================================
class CCamera
{
private:
	//--------------------------------------------------------------------
	// �萔��`
	//--------------------------------------------------------------------
	static const float CAMERA_NEAR;			// �j�A�̍ő�l
	static const float CAMERA_FUR;			// �t�@�[�̍ő�l

public:
	//=============================================================================
	// ���e���@�̃^�C�v�񋓌^
	// Author : �������l
	// �T�v : ���e���@�̎��
	//=============================================================================
	enum VIEW_TYPE
	{
		TYPE_CLAIRVOYANCE = 0,		// �������e
		TYPE_PARALLEL,				// ���s���e
		MAX_VIEW_TYPE,				// �^�C�v�̍ő吔
	};

	//=============================================================================
	// �J�����̃A�N�V�����̊Ǘ�
	// Author : �������l
	// �T�v : �J�����̃A�N�V�������Ǘ�����ϐ�
	//=============================================================================
	struct CAMERA_ACTION
	{
		D3DXVECTOR3			posVDest;				// �ړI�̎��_�ʒu
		D3DXVECTOR3			posRDest;				// �ړI�̒����_�ʒu
		int					nFrame;					// �s���ɗv����t���[����
	};

	//=============================================================================
	// �J�����̃��[�V�����Ǘ�
	// Author : �������l
	// �T�v : �J�����̃��[�V�������Ǘ�����ϐ�
	//=============================================================================
	struct CAMERA_MOTION
	{
		CAMERA_ACTION	*pCameraAction;			// �J�����̃A�N�V����
		int				nMaxKey;				// �A�N�V�����̃L�[��
		bool			bLoop;					// ���[�v���邩�ǂ���
	};

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CCamera();
	~CCamera();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init(void);							// ������
	void Uninit(void);							// �I��
	void Update(void);							// �X�V
	void Set(void);								// �ݒ�
	void SetViewType(VIEW_TYPE type);			// �^�C�v�̐ݒ�
	D3DXVECTOR3 GetPosV() { return m_posV; }
	D3DXVECTOR3 GetPosR() { return m_posR; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	D3DXMATRIX GetMtxView() { return m_mtxView; }
	D3DXMATRIX GetMtxProj() { return m_mtxProj; }
	float GetDistance() { return m_fDistance; }
	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosR(const D3DXVECTOR3 posR);
	void SetRot(const D3DXVECTOR3 rot, const int nShaft);
	void SetCamera(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR, const D3DXVECTOR3 rot);
	void SetFollowTarget(CObject *pTarget, D3DXVECTOR3 posRAdd, float fDistance, float fCoeffFllow);
	void SetViewing(const float fViewing) { m_fViewing = fViewing; }
	void MotionReset(void);
	void SetNumMotion(const int nNumMotion);
	void SetLockPosV(bool bLockPosV) { m_bLockPosV = bLockPosV; }	

private:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void Rotate(void);				// ��]
	void Move(void);				// �ړ�
	void Follow(void);				// �Ǐ]
	void Action();					// �J�����̃A�N�V����
	void Zoom();					// �J�����̊g�k
	void SetPosV();					// ���_�̐ݒ�
	void SetPosR();					// �����_�̐ݒ�
	void SetRot();					// �����̐ݒ�
	void SetDistance();				// ���_���璍���_�܂ł̋�����ݒ肷��

	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	CAMERA_MOTION		*m_pMotion;			// ���[�V����
	CMove				*m_pMove;			// �ړ��N���X�̃C���X�^���X
	CMove				*m_pRoll;			// �ړ��N���X�̃C���X�^���X(�p�x)
	CObject				*m_pTarget;			// �^�[�Q�b�g
	D3DXMATRIX			m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxProj;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			m_mtxView;			// �r���[�}�g���b�N�X
	D3DXVECTOR3			m_posV;				// ���_
	D3DXVECTOR3			m_posR;				// �����_
	D3DXVECTOR3			m_vecU;				// ������x�N�g��
	D3DXVECTOR3			m_rot;				// ����
	D3DXVECTOR3			m_rotMove;			// �ړ�����
	D3DXVECTOR3			m_posVDest;			// ���_�̖ړI�̈ʒu
	D3DXVECTOR3			m_posRDest;			// �����_�̖ړI�̈ʒu
	D3DXVECTOR3			m_posRAdd;			// �����_�̒ǉ��ʒu
	VIEW_TYPE			m_viewType;			// ���e�̎��
	float				m_fDistance;		// ���_���璍���_�܂ł̋���
	float				m_fViewing;			// ����p
	float				m_fRotMove;			// �ړ�����
	float				m_fCoeffFllow;		// �Ǐ]�̌����W��
	int					m_nCntFrame;		// �t���[���J�E���g
	int					m_nCntKey;			// �L�[�J�E���g
	int					m_nMaxMotion;		// ���[�V�����̍ő吔
	int					m_nNumMotion;		// ���[�V�����J�E���g
	bool				m_bAutoMove;		// �����ړ�
	bool				m_bAction;			// �A�N�V�������s����
	bool				m_bFllow;			// �Ǐ]���s����
	bool				m_bLockPosV;		// ���_�̃��b�N
};

#endif
