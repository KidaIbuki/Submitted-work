//=================================
// 
// �V�[���N���X�@scene.cpp
// Auther kida ibuki 
// 
//==================================
#include "title.h"     // �^�C�g��
#include "fade.h"      // �t�F�[�h
#include "manager.h"    // �}�l�[�W���[
#include "blockX.h"   // �u���b�N

#include "ToneAdjust.h"
#include "EffectAccumulation.h"
#include "WaterSurface.h"

//====================================
// �R���X�g���N�^
//====================================
CTitle::CTitle()
{
}
//====================================
// �f�X�g���N�^
//====================================
CTitle::~CTitle()
{
}
//====================================
// �����ݒ�
//====================================
HRESULT CTitle::Init()
{
	//CManager* pManager = CManager::GetInstance();

	CManager* pManager = CManager::GetInstance();
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	CScene::Init();
	
	CBlockX::Create(pDevice, D3DXVECTOR3(60.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));  // �g�D�[��
	ToneAdjust::Create(D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));  // �F���ω�
	EffectAccumulation::Create();  // ���V�F�[�_�[



	Wave::Create();  // �t���l�����ʕt���̔g�V�F�[�_�[

	return S_OK;
}
//====================================
// �I������
//====================================
void CTitle::Uninit()
{

	CScene::Uninit();
	CObject* pTopObject[NUM_MAX_PRIORITY];
	CObject::GetObj(pTopObject);
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		//�I�u�W�F�N�g�擾
		CObject* pObj = pTopObject[nPriority];
		// �ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();

			pObj->DeathFlag();

			pObj = pNext;

		}
	}

}
//====================================
// �X�V����
//====================================
void CTitle::Update()
{
	CScene::Update();

	CObject* pTopObject[NUM_MAX_PRIORITY];
	CObject::GetObj(pTopObject);
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		//�I�u�W�F�N�g�擾
		CObject* pObj = pTopObject[nPriority];
		// �ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();

			pObj->Update();

			pObj = pNext;

		}
	}

}

//====================================
// �`�揈��
//====================================
void CTitle::Draw()
{
	CObject* pTopObject[NUM_MAX_PRIORITY];
	CObject::GetObj(pTopObject);
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		//�I�u�W�F�N�g�擾
		CObject* pObj = pTopObject[nPriority];
		// �ő吔���s���Ȃ̂�while���g�p
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();

			pObj->Draw();

			pObj = pNext;

		}
	}


}

//======================================
// �^�C�g���̐���
//======================================
CTitle* CTitle::Create()
{
	CTitle* pTitle = new CTitle();
	pTitle->Init();

	return pTitle;
}