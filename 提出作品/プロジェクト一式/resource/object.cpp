//=================================
// 
// �I�u�W�F�N�g�N���X�@object.cpp
// Auther kida ibuki 
// 
//==================================
#include "object.h"   // �I�u�W�F�N�g
#include "manager.h"

CObject* CObject::m_pTop[NUM_MAX_PRIORITY] = {};
CObject* CObject::m_pCur[NUM_MAX_PRIORITY] = {};
bool CObject::m_bReleaseAll = false;

//====================================================
// �R���X�g���N�^(�������g�̓o�^)
//====================================================
CObject::CObject()
{

	m_pNext = nullptr;	// �I�u�W�F�N�g�̎��̃|�C���^��������
	m_pPrev = nullptr;	// �I�u�W�F�N�g�̑O�̃|�C���^��������
	m_bDeath = false;	// ���S�t���O��������
	m_bDeathAll = false;  // �V�[���ȍ~�Ńt���O�𗧂Ă�
	m_type = NONE;      // �^�C�v�̏�����

	m_bNormalUpdate = true;   // �ʏ�̍X�V
	m_bPauseUpdate = false;   // �|�[�Y���̍X�V
	m_bNormalDraw = false;    // �ʏ�̕`��
	m_bPauseDraw = true;      // �|�[�Y���̕`��

	if (m_pCur[3] != nullptr)
	{ // �Ō�����󂶂�Ȃ�����
		m_pPrev = m_pCur[3];			// �������g��1�O�̃I�u�W�F�N�g�͌��݂̍Ō��
		m_pPrev->m_pNext = this;	// �������g��1�O�̃I�u�W�F�N�g�̎��̃|�C���^�Ɏ������g����
	}

	if (m_pTop[3] == nullptr)
	{ // �擪���󂾂���
		m_pTop[3] = this;	// �擪�Ɏ������g����
		m_pCur[3] = this;
	}

	m_pCur[3] = this;	// �Ō���Ɏ������g����

}
CObject::CObject(int nPriority)
{

	m_pNext = nullptr;	// �I�u�W�F�N�g�̎��̃|�C���^��������
	m_pPrev = nullptr;	// �I�u�W�F�N�g�̑O�̃|�C���^��������
	m_bDeath = false;	// ���S�t���O��������
	m_bDeathAll = false;  // �V�[���ȍ~�Ńt���O�𗧂Ă�
	m_type = NONE;      // �^�C�v�̏�����

	m_bNormalUpdate = true;   // �ʏ�̍X�V
	m_bPauseUpdate = false;   // �|�[�Y���̍X�V
	m_bNormalDraw = false;    // �ʏ�̕`��
	m_bPauseDraw = true;      // �|�[�Y���̕`��


	if (m_pCur[nPriority] != nullptr)
	{ // �Ō�����󂶂�Ȃ�����
		m_pPrev = m_pCur[nPriority];			// �������g��1�O�̃I�u�W�F�N�g�͌��݂̍Ō��
		m_pPrev->m_pNext = this;	// �������g��1�O�̃I�u�W�F�N�g�̎��̃|�C���^�Ɏ������g����
	}

	if (m_pTop[nPriority] == nullptr)
	{ // �擪���󂾂���
		m_pTop[nPriority] = this;	// �擪�Ɏ������g����
		m_pCur[nPriority] = this;
	}

	m_pCur[nPriority] = this;	// �Ō���Ɏ������g����

}

//====================================================
// �f�X�g���N�^
//====================================================
CObject::~CObject()
{

}

//====================================================
// �����ݒ�
//====================================================
HRESULT CObject::Init()
{
	return S_OK;
}
//====================================================
// �S�I�u�W�F�N�g���
//====================================================
void CObject::ReleaseAll()
{
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		CObject* pObj = nullptr;
		pObj = m_pTop[nPriority];   // �擪�擾
		// �ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;
			pObj->m_bDeath = true;
			pObj = pNext;
		}
	}
}

//====================================================
//�S�I�u�W�F�N�g�X�V
//====================================================
void CObject::UpdateAll()
{
	CManager* pManager = CManager::GetInstance();
	bool bPause = pManager->GetPause();

	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		CObject* pObj = nullptr;
		pObj = m_pTop[nPriority];   // �擪�擾

		// �ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;

			if (bPause)
			{
				bool a = pObj->IsPauseUpdate();
				if (a)
				{
					pObj->Update();
				}
			}
			else
			{
				if (pObj->IsNormalUpdate())
				{
					pObj->Update();
				}
			}
			pObj = pNext;
		}


	}
}
//====================================================
//�S�I�u�W�F�N�g�`��
//====================================================
void CObject::DrawAll()
{
	CManager* pManager = CManager::GetInstance();
	bool bPause = pManager->GetPause();

	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		CObject* pObj = nullptr;
		pObj = m_pTop[nPriority];   // �擪�擾
		// �ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;
			pObj->Draw();
			if (bPause)
			{
				if (pObj->IsPauseUpdate())
				{
					pObj->Draw();
				}
			}
			else
			{
				if (pObj->IsNormalUpdate())
				{
					pObj->Draw();
				}
			}

			pObj = pNext;
		}
	}
}
//====================================================
// �S�I�u�W�F�N�g��j��
//====================================================
void CObject::DeathAll()
{
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		CObject* pObj = nullptr;
		pObj = m_pTop[nPriority];   // �擪�擾

				// �ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;
			if (pObj->m_bDeath == true)  // TODO: ���܂ŃI�u�W�F�N�g���g�� Relese ���Ă�ł����Ƃ�����A"m_bDeath = true" �ɂ���
			{
				pObj->Release();
			}

			pObj = pNext;
		}
	}

}
//====================================================
// �I�u�W�F�N�g�̎擾
//====================================================
CObject* CObject::GetObj(int nPriority)
{
	return m_pTop[nPriority];
}
void CObject::GetObj(CObject* pObject[NUM_MAX_PRIORITY])
{
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		pObject[nPriority] = m_pTop[nPriority];
	}
}
//====================================================
//�^�C�v�ݒ�
//====================================================
void CObject::SetType(TYPE type)
{
	assert(type >= NONE && type < MAX);
	m_type = type;
}

//====================================================
//�������g�̉��(�ς��Ȃ���OK)
//====================================================
void CObject::Release()
{
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		if (m_pTop[nPriority] == this) // || m_pPrev == nullptr �ł��ł�����
		{ // �������g���擪������
			m_pTop[nPriority] = m_pNext;	// �����̎��̃I�u�W�F�N�g��擪�ɂ���
		}
		if (m_pCur[nPriority] == this) // || m_pNext == nullptr �ł��ł�����
		{ // �������g���Ō��������
			m_pCur[nPriority] = m_pPrev;	// �����̑O�̃I�u�W�F�N�g��擪�ɂ���
		}
	}
	if (m_pPrev != nullptr)
	{
		m_pPrev->m_pNext = m_pNext;   // ���������I�u�W�F�N�g����������A�������I�u�W�F�N�g�̑O����q����
	}
	if (m_pNext != nullptr)
	{
		m_pNext->m_pPrev = m_pPrev;   // ���������I�u�W�F�N�g����������A�������I�u�W�F�N�g�̑O����q����
	}
	delete this;
}
//====================================================
// ���S�t���O�𗧂Ă�
//====================================================
void CObject::DeathFlag()
{
	m_bDeath = true;
}
