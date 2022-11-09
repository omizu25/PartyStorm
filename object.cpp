//=============================================================================
//
// �I�u�W�F�N�g�N���X(object.h)
// Author : �������l
// �T�v : �I�u�W�F�N�g�������s��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <assert.h>

#include "object.h"
#include "debug_proc.h"
#include "renderer.h"
#include "application.h"
#include "camera.h"
#include "instancing.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CObject *CObject::m_pTop[PRIORITY_MAX] = {};				// �擪�I�u�W�F�N�g�ւ̃|�C���^
CObject *CObject::m_pCurrent[PRIORITY_MAX] = {};			// ���݂�(��Ԍ��)�I�u�W�F�N�g�ւ̃|�C���^
int CObject::m_nMaxObject = 0;								// �g�p��
int CObject::m_nPriorityMaxObj[PRIORITY_MAX] = {};			// �v���C�I���e�B���Ƃ̃I�u�W�F�N�g��
bool CObject::m_bPause = false;								// �|�[�Y���g�p���Ă��邩�ǂ���

//=============================================================================
// �C���X�^���X�̉��
// Author : �������l
// �T�v : ���ׂẴC���X�^���X��������鏈��
//=============================================================================
void CObject::ReleaseAll(bool bPermanent)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// �ϐ��錾
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ۊ�
				CObject *pObjectNext = pObject->m_pNext;
				if (bPermanent)
				{
					if (!pObject->m_bDeath)
					{// �I�u�W�F�N�g�̉��
						pObject->Release();
					}
				}
				else
				{
					if (!pObject->m_bDeath
						&& pObject->m_objectType != CObject::OBJTYPE_FADE
						&& pObject->m_objectType != OBJTYPE_PAUSE)
					{// �I�u�W�F�N�g�̉��
						pObject->Release();
					}
				}

				// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���X�V
				pObject = pObjectNext;
			}
		}
	}

	// ���ׂẴ��X�g����
	ReleaseListAll();
}

//=============================================================================
// ���ׂẴC���X�^���X�̍X�V
// Author : �������l
// �T�v : �g�p����Ă�C���X�^���X�̍X�V�������Ăяo��
//=============================================================================
void CObject::UpdateAll(void)
{
	// �I�u�W�F�N�g�̎g�p���̕\��
	CDebugProc::Print("�I�u�W�F�N�g�̎g�p�� : %d\n", m_nMaxObject);

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// �ϐ��錾
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ۊ�
				CObject *pObjectNext = pObject->m_pNext;

				if (!pObject->m_bDeath
					&& !m_bPause)
				{// �I�u�W�F�N�g�̍X�V
					pObject->Update();
				}
				else if (!pObject->m_bDeath
					&& m_bPause
					&& pObject->m_objectType == OBJTYPE_PAUSE)
				{// �I�u�W�F�N�g�X�V����
					pObject->Update();
				}
				
				// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���X�V
				pObject = pObjectNext;
			}
		}
	}
}

//=============================================================================
// ���ׂẴC���X�^���X�̕`��
// Author : �������l
// �T�v : �g�p����Ă�C���X�^���X�̕`�揈�����Ăяo��
//=============================================================================
void CObject::DrawAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (nCntPriority == PRIORITY_EFFECT)
		{// �G�t�F�N�g
			CApplication::GetInstancing()->Draw();
			continue;
		}

		if (m_pTop[nCntPriority] != nullptr)
		{// �ϐ��錾
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ۊ�
				CObject *pObjectNext = pObject->m_pNext;

				if (!pObject->m_bDeath)
				{
					// �I�u�W�F�N�g�̕`��
					pObject->Draw();
				}

				// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���X�V
				pObject = pObjectNext;
			}
		}
	}

	// ���ׂẴ��X�g����
	ReleaseListAll();
}

//=============================================================================
// ���ׂẴI�u�W�F�N�g�̃��X�g����
// Author : �������l
// �T�v : ���ׂẴI�u�W�F�N�g�̃��X�g�������Ăяo��
//=============================================================================
void CObject::ReleaseListAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{// �ϐ��錾
			CObject *pObject = m_pTop[nCntPriority];

			while (pObject)
			{// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ۊ�
				CObject *pObjectNext = pObject->m_pNext;

				if (pObject->m_bDeath)
				{// �I�u�W�F�N�g�̃��X�g����
					pObject->ReleaseList();
				}

				// ���݂̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g���X�V
				pObject = pObjectNext;
			}
		}
	}
}

#ifdef _DEBUG
void CObject::ReleaseTop(int nPriority)
{
	if (m_pTop[nPriority] != nullptr)
	{
		m_pTop[nPriority]->Release();
	}
}
void CObject::ReleaseCurrent(int nPriority)
{
	if (m_pCurrent[nPriority] != nullptr)
	{
		m_pCurrent[nPriority]->Release();
	}
}
void CObject::RandomRelease(int nPriority)
{
	if (m_pTop[nPriority] != nullptr)
	{// �ϐ��錾
		CObject *pObject = m_pTop[nPriority];
		int nDestObject = rand() % m_nPriorityMaxObj[nPriority];

		for(int nCntObject = 0; nCntObject < nDestObject; nCntObject++)
		{// �I�u�W�F�N�g�̍X�V
			pObject = pObject->m_pNext;
		}

		pObject->Release();
	}
}
#endif // DEBUG

//=============================================================================
// �R���X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�������ɍs������
//=============================================================================
CObject::CObject(int nPriority) : m_pPrev(nullptr),		// �O�̃I�u�W�F�N�g�ւ̃|�C���^
m_pNext(nullptr),										// ���̃I�u�W�F�N�g�ւ̃|�C���^
m_nLevPriority(nPriority),								// �v���C�I���e�B�̃��x��
m_bDeath(false)											// ���S�t���O
{
	// �g�p���̃C���N�������g
	m_nMaxObject++;
	m_nPriorityMaxObj[m_nLevPriority]++;

	if (m_pTop[m_nLevPriority] == nullptr)
	{// �擪�|�C���^�[�̐ݒ�
		m_pTop[m_nLevPriority] = this;
	}
	else
	{// �ߋ��̍Ō���I�u�W�F�N�g�̎��̃I�u�W�F�N�g��ݒ�
		m_pCurrent[m_nLevPriority]->m_pNext = this;

		// �����̑O�̃I�u�W�F�N�g�̐ݒ�
		this->m_pPrev = m_pCurrent[m_nLevPriority];
	}
	
	// ���������݂̃I�u�W�F�N�g�ɐݒ�
	m_pCurrent[m_nLevPriority] = this;
}

//=============================================================================
// �f�X�g���N�^
// Author : �������l
// �T�v : �C���X�^���X�I�����ɍs������
//=============================================================================
CObject::~CObject()
{
	
}

//=============================================================================
// �C���X�^���X�̉��
// Author : �������l
// �T�v : ���ׂẴC���X�^���X��������鏈��
//=============================================================================
void CObject::Release(void)
{
	// �g�p���̃f�N�������g
	m_nMaxObject--;
	m_nPriorityMaxObj[m_nLevPriority]--;

	// ���S�t���O�𗧂Ă�
	m_bDeath = true;
}

//=============================================================================
// ���X�g�̉����Ɣj��
// Author : �������l
// �T�v : ���X�g�̉����Ɣj�������鏈��
//=============================================================================
void CObject::ReleaseList(void)
{
	if (m_pTop[m_nLevPriority] == this)
	{// �擪�I�u�W�F�N�g�������̎��̃I�u�W�F�N�g�ɐݒ�
		m_pTop[m_nLevPriority] = m_pNext;
	}
	if (m_pCurrent[m_nLevPriority] == this)
	{// �Ō���I�u�W�F�N�g�������̑O�̃I�u�W�F�N�g�ɐݒ�
		m_pCurrent[m_nLevPriority] = m_pPrev;
	}

	if (m_pPrev != nullptr)
	{// �O�̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g�Ɏ����̎��̃I�u�W�F�N�g��ݒ�
		m_pPrev->m_pNext = m_pNext;
	}
	if (m_pNext != nullptr)
	{// �����̎��̃I�u�W�F�N�g�Ɏ����̑O�̃I�u�W�F�N�g��ݒ�
		m_pNext->m_pPrev = m_pPrev;
	}

	// �I�u�W�F�N�g�̉��
	delete this;
}

//=============================================================================
// ��`�̔���
// Author : �������l
// �T�v : �^�[�Q�b�g�Ƃ̋�`����
//=============================================================================
bool CObject::ColisonRectangle3D(CObject * target, bool bExtrude)
{// �Ԃ�l�p�̕ϐ�
	bool bColision = false;

	// �����̏����擾����
	D3DXVECTOR3 pos = GetPos() + m_colisonPos;
	D3DXVECTOR3 posOld = GetPosOld() + m_colisonPos;
	D3DXVECTOR3 size = m_colisonSize / 2.0f;

	// �ڕW�̏��擾
	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
	D3DXVECTOR3 sizeTarget = target->GetColisonSize() / 2.0f;

	if ((pos.z - size.z) < (posTarget.z + sizeTarget.z)
		&& (pos.z + size.z) > (posTarget.z - sizeTarget.z)
		&& (pos.x - size.x) < (posTarget.x + sizeTarget.x)
		&& (pos.x + size.x) > (posTarget.x - sizeTarget.x))
	{// ���f�����ɂ���(XZ��)
		if ((posOld.y + size.y) <= (posTarget.y - sizeTarget.y)
			&& (pos.y + size.y) > (posTarget.y - sizeTarget.y))
		{
			bColision = true;

			if (bExtrude)
			{
				pos.y = posTarget.y - sizeTarget.y - size.y;
			}
		}
		if ((posOld.y - size.y) >= (posTarget.y + sizeTarget.y)
			&& (pos.y - size.y) < (posTarget.y + sizeTarget.y))
		{
			bColision = true;

			if (bExtrude)
			{
				pos.y = posTarget.y + sizeTarget.y + size.y;
			}
		}
	}
	if ((pos.y - size.y) < (posTarget.y + sizeTarget.y)
		&& (pos.y + size.y) > (posTarget.y - sizeTarget.y))
	{// ���f�����ɂ���(Y��)
		bool bColisionX = false;
		bool bColisionZ = false;

		if ((pos.z - size.z) < (posTarget.z + sizeTarget.z)
			&& (pos.z + size.z) > (posTarget.z - sizeTarget.z))
		{// ���f�����ɂ���(Z��)
			bColisionZ = true;
			if ((posOld.x + size.z) <= (posTarget.x - sizeTarget.x)
				&& (pos.x + size.z) > (posTarget.x - sizeTarget.x))
			{
				if (bExtrude)
				{
					pos.x = posTarget.x - sizeTarget.x - size.z;
				}
			}
			if ((posOld.x - size.z) >= (posTarget.x + sizeTarget.x)
				&& (pos.x - size.z) < (posTarget.x + sizeTarget.x))
			{
				if (bExtrude)
				{
					pos.x = posTarget.x + sizeTarget.x + size.z;
				}
			}
		}
		if ((pos.x - size.x) < (posTarget.x + sizeTarget.x)
			&& (pos.x + size.x) > (posTarget.x - sizeTarget.x))
		{// ���f�����ɂ���(X��)
			bColisionX = true;
			if ((posOld.z + size.z) <= (posTarget.z - sizeTarget.z)
				&& (pos.z + size.z) > (posTarget.z - sizeTarget.z))
			{
				if (bExtrude)
				{
					pos.z = posTarget.z - sizeTarget.z - size.z;
				}
			}
			if ((posOld.z - size.z) >= (posTarget.z + sizeTarget.z)
				&& (pos.z - size.z) < (posTarget.z + sizeTarget.z))
			{
				if (bExtrude)
				{
					pos.z = posTarget.z + sizeTarget.z + size.z;
				}
			}
		}

		if (bColisionX
			&& bColisionZ)
		{
			bColision = true;
		}
	}

	// �ʒu�̐ݒ�
	SetPos(pos - m_colisonPos);
	return bColision;
}

//=============================================================================
// ���̔���
// Author : �������l
// �T�v : �^�[�Q�b�g�Ƃ̋�����
//=============================================================================
bool CObject::ColisonSphere3D(CObject *target, bool bExtrude)
{
	// �ϐ��錾
	bool bCollision = false;

	// �����̏����擾����
	D3DXVECTOR3 pos = GetPos() + m_colisonPos;
	D3DXVECTOR3 size = m_colisonSize / 2.0f;

	// �ڕW�̏��擾
	D3DXVECTOR3 posTarget = target->GetPos() + target->GetColisonPos();
	D3DXVECTOR3 sizeTarget = target->GetColisonSize() / 2.0f;

	// ������s���������Z�o
	float fJudgeDistance = sqrtf((size.x * size.x) + (size.y * size.y) + (size.z * size.z));
	fJudgeDistance += sqrtf((sizeTarget.x * sizeTarget.x) + (sizeTarget.y * sizeTarget.y) + (sizeTarget.z * sizeTarget.z));

	// ���݂��̈ʒu�̍����Z�o
	D3DXVECTOR3 distance = posTarget - pos;
	float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));

	if (fDistance <= fJudgeDistance)
	{// �ʒu�̍���������s���������Z���ꍇ
		bCollision = true;

		if (bExtrude)
		{
			// �p�x�̎Z�o
			D3DXVECTOR3 rotDiff;
			rotDiff.y = atan2f(distance.x, distance.z);
			rotDiff.x = atan2f(sqrtf((distance.x * distance.x) + (distance.z * distance.z)), distance.y);
			rotDiff.z = 0.0f;

			// �ʒu�̎Z�o
			pos.z = posTarget.z - sinf(rotDiff.x) * cosf(rotDiff.y) * fJudgeDistance;
			pos.x = posTarget.x - sinf(rotDiff.x) * sinf(rotDiff.y) * fJudgeDistance;
			pos.y = posTarget.y - cosf(rotDiff.x) * fJudgeDistance;

			// �ʒu�̐ݒ�
			SetPos(pos - m_colisonPos);
		}
	}

	return bCollision;
}
