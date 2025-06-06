//=================================
// 
// �I�u�W�F�N�g�N���X�@object.h
// Auther kida ibuki 
// 
//==================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"   // ���C��
#include "pch.h"    // �v���R���p�C���ς݃w�b�_�[

#define NUM_MAX_PRIORITY (8) // �v���C�I���e�B�̍ő吔


//�S�I�u�W�F�N�g�̊��N���X
class CObject
{
public:
	// �I�u�W�F�N�g�^�C�v
	typedef enum
	{
		NONE = 0,	         // ����
		BLOCK,		         // �z�u�I�u�W�F�N�g
		BLOCK2,		         // �z�u�I�u�W�F�N�g
		CAMERA,              // �J����
		PAUSE,               // �|�[�Y
		PLAYER,
		LIGHT,
		MAX
	}TYPE;

	CObject();		// �R���X�g���N�^
	CObject(int nPriority);		// �R���X�g���N�^ �I�[�o�[���C�h

	virtual ~CObject();		// �f�X�g���N�^
	virtual HRESULT Init() = 0;		// �������z�֐��̏����ݒ�
	virtual void Uninit() = 0;		// �������z�֐��̏I������
	virtual void Update() = 0;		// �������z�֐��̍X�V����
	virtual void Draw() = 0;		// �������z�֐��̕`�揈��
	static void ReleaseAll();		// �S�I�u�W�F�N�g�̉��
	static void UpdateAll();		// �S�I�u�W�F�N�g�̍X�V
	static void DrawAll();			// �S�I�u�W�F�N�g�̕`��
	static void DeathAll();         // �S�I�u�W�F�N�g�j��

	void SetNormalUpdate(bool bNormalUpdate) { m_bNormalUpdate = bNormalUpdate; }   // �S�̂ōX�V���邩�ݒ�
	void SetPauseUpdate(bool bPauseUpdate) { m_bPauseUpdate = bPauseUpdate; }       // �|�[�Y���ōX�V���邩�ݒ�
	void SetNormalDraw(bool bNormalDraw) { m_bNormalDraw = bNormalDraw; }   // �S�̂ŕ`�悷�邩�ݒ�
	void SetPauseDraw(bool bPauseDraw) { m_bPauseDraw = bPauseDraw; }   // �|�[�Y���ŕ`�悷�邩�ݒ�


	bool IsNormalUpdate() { return m_bNormalUpdate; }   // �S�̂ōX�V
	bool IsPauseUpdate() { return m_bPauseUpdate; }  // �|�[�Y���ōX�V
	bool IsNormalDraw() { return m_bNormalDraw; }     // �S�̂ŕ`��
	bool isPauseDraw() { return m_bPauseDraw; }    // �S�̂ŕ`��

	static CObject* GetObj(int nPriority);						// �I�u�W�F�N�g�擾
	static void GetObj(CObject* pObject[NUM_MAX_PRIORITY]);						// �I�u�W�F�N�g�擾

	TYPE GetType() { return m_type; }		// �^�C�v�擾
	CObject* GetNext() { return m_pNext; }  // ���̃I�u�W�F�N�g���擾
	void SetType(TYPE type);				// �^�C�v�ݒ�

	void Release();					// �������g�̉��
	bool m_bDeath;

	virtual void DeathFlag();
private:
	TYPE m_type;

	//==========���X�g�\��==============================================

	static CObject* m_pTop[NUM_MAX_PRIORITY];  // �擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCur[NUM_MAX_PRIORITY];  // ����(�Ō��)�̃|�C���^

	CObject* m_pPrev;  // �O�̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pNext;  // ���̃I�u�W�F�N�g�̃|�C���^

	static bool m_bReleaseAll;	// TODO: �����߂�Bupdate �ŗ�O�X���[����闝�R => update ���� releaseAll ����āApNext �����łɔj������Ă��邩��B

	bool m_bNormalUpdate;   // �S�̂ŃA�b�v�f�[�g
	bool m_bPauseUpdate;    // �|�[�Y���ɃA�b�v�f�[�g
	bool m_bNormalDraw;     // �S�̂ŕ`��
	bool m_bPauseDraw;      // �|�[�Y���ɕ`��

	bool m_bDeathAll;
};

#endif
