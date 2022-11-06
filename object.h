//=============================================================================
//
// �I�u�W�F�N�g�N���X(object.h)
// Author : �������l
// �T�v : �I�u�W�F�N�g�������s��
//
//=============================================================================
#ifndef _OBJECT_H_		// ���̃}�N����`������ĂȂ�������
#define _OBJECT_H_		// ��d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//=============================================================================
// �I�u�W�F�N�g�N���X
// Author : �������l
// �T�v : �I�u�W�F�N�g�������s���N���X
//=============================================================================
class CObject
{
public:
	//--------------------------------------------------------------------
	// �v���C�I���e�B�̃��x��
	// Author : �������l
	// �T�v : �v���C�I���e�B�̃��x����ݒ�
	//--------------------------------------------------------------------
	enum PRIORITY_LEVEL
	{
		PRIORITY_NONE = 0,	// ���̑�
		PRIORITY_3D,		// 3D
		PRIORITY_EFFECT,	// �G�t�F�N�g
		PRIORITY_2D,		// 2D
		PRIORITY_FADE,		// �t�F�[�h
		PRIORITY_MAX		// �v���C�I���e�B�̍ő�
	};

	//--------------------------------------------------------------------
	// �I�u�W�F�N�g�̎�ʂ̗񋓌^
	// Author : �������l
	// �T�v : �I�u�W�F�N�g�̎�ʏ����i�[
	//--------------------------------------------------------------------
	enum EObjectType
	{
		OBJTYPE_NONE = 0,			// �^�C�v����
		OBJTYPE_3DPLAYER,			// 3D�v���C���[
		OBJTYPE_3DENEMY,			// 3D�G�l�~�[
		OBJTYPE_3DMODEL,			// 3D���f��
		OBJTYPE_FADE,				// �t�F�[�h
		OBJTYPE_PAUSE,				// �|�[�Y
		MAX_OBJTYPE,				// ��ʂ̍ő吔
	};

	//--------------------------------------------------------------------
	// �ÓI�����o�֐�
	//--------------------------------------------------------------------
	static void ReleaseAll(bool bPermanent);										// ���ׂẴI�u�W�F�N�g�̉��
	static void UpdateAll();														// ���ׂẴI�u�W�F�N�g�̍X�V
	static void DrawAll();															// ���ׂẴI�u�W�F�N�g�̕`��
	static void ReleaseListAll();													// ���ׂẴI�u�W�F�N�g�̃��X�g����
	static CObject *GetTop(int nPriority) { return m_pTop[nPriority]; }				// �擪�I�u�W�F�N�g�ւ̃|�C���^�̃Q�b�^�[
	static CObject *GetCurrent(int nPriority) { return m_pCurrent[nPriority]; }		// ���݂�(��Ԍ��)�I�u�W�F�N�g�ւ̃|�C���^�̃Q�b�^�[
	static void SetPause(const bool bPause) { m_bPause = bPause; }					// �|�[�Y��Ԃ̃Z�b�^�[

#ifdef _DEBUG
	static void ReleaseTop(int nPriority);
	static void ReleaseCurrent(int nPriority);
	static void RandomRelease(int nPriority);
#endif // DEBUG

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	explicit CObject(int nPriority = PRIORITY_NONE);
	virtual ~CObject();

	//--------------------------------------------------------------------
	// �������z�֐�
	//--------------------------------------------------------------------
	virtual HRESULT Init() = 0;																// ������
	virtual void Uninit() = 0;																// �I��
	virtual void Update() = 0;																// �X�V
	virtual void Draw() = 0;																// �`��
	virtual void SetPos(const D3DXVECTOR3 &pos) = 0;										// �ʒu�̃Z�b�^�[
	virtual void SetPosOld(const D3DXVECTOR3 &posOld) = 0;									// �ߋ��ʒu�̃Z�b�^
	virtual void SetRot(const D3DXVECTOR3 &rot) = 0;										// �����̃Z�b�^�[
	virtual void SetSize(const D3DXVECTOR3 &size) = 0;										// �傫���̃Z�b�^�[
	virtual D3DXVECTOR3 GetPos() = 0;														// �ʒu�̃Q�b�^�[
	virtual D3DXVECTOR3 GetPosOld() = 0;													// �ߋ��ʒu�̃Q�b�^�[
	virtual D3DXVECTOR3 GetRot() = 0;														// �����̃Q�b�^�[
	virtual D3DXVECTOR3 GetSize() = 0;														// �傫���̃Q�b�^�[
	void SetObjType(EObjectType objectType) { m_objectType = objectType; }					// �I�u�W�F�N�g�̎�ʐݒ�
	EObjectType GetObjType() { return m_objectType; }										// �I�u�W�F�N�g�̎�ʐݒ�
	void SetColisonPos(const D3DXVECTOR3 &colisonPos) { m_colisonPos = colisonPos; }		// �����蔻��̈ʒu�̃Z�b�^�[
	D3DXVECTOR3 GetColisonPos() { return m_colisonPos; }									// �����蔻��̈ʒu�̃Q�b�^�[
	void SetColisonSize(const D3DXVECTOR3 &colisonSize) { m_colisonSize = colisonSize; }	// �����蔻��̑傫���̃Z�b�^�[
	D3DXVECTOR3 GetColisonSize() { return m_colisonSize; }									// �����蔻��̑傫���̃Q�b�^�[
	CObject *GetPrev() { return m_pPrev; }													// �O�̃I�u�W�F�N�g�ւ̃|�C���^�̃Q�b�^�[
	CObject *GetNext() { return m_pNext; }													// ���̃I�u�W�F�N�g�ւ̃|�C���^�̃Q�b�^�[
	bool GetFlagDeath() { return m_bDeath; }												// ���S�t���O�̎擾
	bool ColisonRectangle3D(CObject *target, bool bExtrude);								// ��`�̔���(3D)
	bool ColisonSphere3D(CObject *target, bool bExtrude);									// ���̔���(3D)

protected:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	void Release(void);												// �I�u�W�F�N�g�̉��
	void ReleaseList(void);											// �I�u�W�F�N�g�̃��X�g����

private:
	//--------------------------------------------------------------------
	// �ÓI�����o�ϐ�
	//--------------------------------------------------------------------
	static CObject *m_pTop[PRIORITY_MAX];			// �擪�I�u�W�F�N�g�ւ̃|�C���^
	static CObject *m_pCurrent[PRIORITY_MAX];		// ���݂�(��Ԍ��)�I�u�W�F�N�g�ւ̃|�C���^
	static int m_nMaxObject;						// �g�p��
	static int m_nPriorityMaxObj[PRIORITY_MAX];		// �v���C�I���e�B���Ƃ̃I�u�W�F�N�g��
	static bool m_bPause;							// �|�[�Y���g�p���Ă��邩�ǂ���

	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	CObject *m_pPrev;				// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pNext;				// ���̃I�u�W�F�N�g�ւ̃|�C���^
	EObjectType m_objectType;		// �I�u�W�F�N�g�̎��
	D3DXVECTOR3 m_colisonPos;		// �����蔻��̈ʒu
	D3DXVECTOR3 m_colisonSize;		// �����蔻��̑傫��
	int	 m_nLevPriority;			// �v���C�I���e�B�̃��x��
	bool m_bDeath;					// ���S�t���O
};

#endif

