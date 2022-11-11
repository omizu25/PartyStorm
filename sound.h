//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
// ���C�u�����[�����N
//*****************************************************************************
#pragma comment(lib,"dxguid.lib")		// DirectX�̃R���|�l�[�g(���i)�̎g�p�ɕK�v

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "xaudio2.h"			// �T�E���h�����ɕK�v

//=============================================================================
// �e�N�X�`���N���X
// Author : �������l
// �T�v : �e�N�X�`���ݒ���s���N���X
//=============================================================================
class CSound
{
public:
	//*****************************************************************************
	// �p�����[�^�\���̒�`
	// Author : �������l
	// �T�v : �t�@�C�����ƃ��[�v�̎g�p�󋵂��i�[
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	//*****************************************************************************
	// �T�E���h�t�@�C���̗񋓌^
	// Author : �������l
	// �T�v : �T�E���h�t�@�C���̎��
	//*****************************************************************************
	enum SOUND_LABEL
	{
		SOUND_LABEL_TITELBGM,
		SOUND_LABEL_GAMEBGM,
		SOUND_LABEL_RESULTBGM,
		SOUND_LABEL_GAMEOVERBGM,
		SOUND_LABEL_SE_BITING,
		SOUND_LABEL_SE_EAT,
		SOUND_LABEL_SE_DECIDE,
		SOUND_LABEL_SE_SELECT,
		SOUND_LABEL_SE_PAUSE,
		SOUND_LABEL_MAX,
	};

	//--------------------------------------------------------------------
	// �R���X�g���N�^�ƃf�X�g���N�^
	//--------------------------------------------------------------------
	CSound();
	~CSound();

	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	HRESULT Init(HWND hWnd);																			// ������
	void Uninit(void);																					// �I��
	HRESULT PlaySound(SOUND_LABEL label);																// �Z�O�����g�Đ�(�Đ����Ȃ��~)
	void StopSound(SOUND_LABEL label);																	// �Z�O�����g��~(���x���w��)
	void StopSound(void);																				// �Z�O�����g��~(�S��)
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);		// �`�����N�̃`�F�b�N
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);		// �`�����N�f�[�^�̓ǂݍ���

private:
	//--------------------------------------------------------------------
	// �����o�֐�
	//--------------------------------------------------------------------
	SOUNDPARAM SetSoundParam(char *pFileName, int nCntLoop);			// �T�E���h�p�����[�^�̓o�^

	//--------------------------------------------------------------------
	// �����o�ϐ�
	//--------------------------------------------------------------------
	IXAudio2					*m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice		*m_pMasteringVoice;							// �}�X�^�[�{�C�X
	IXAudio2SourceVoice			*m_apSourceVoice[SOUND_LABEL_MAX];			// �\�[�X�{�C�X
	BYTE						*m_apDataAudio[SOUND_LABEL_MAX];			// �I�[�f�B�I�f�[�^
	DWORD						m_aSizeAudio[SOUND_LABEL_MAX];				// �I�[�f�B�I�f�[�^�T�C�Y
	SOUNDPARAM					m_aParam[SOUND_LABEL_MAX];					// �e���f�ނ̃p�����[�^
};

#endif
