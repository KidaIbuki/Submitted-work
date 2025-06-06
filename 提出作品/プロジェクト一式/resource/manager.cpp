//=================================
// 
// �}�l�[�W���@manager.cpp
// Auther kida ibuki 
// 
//==================================
#include "manager.h"  // �}�l�[�W���[
#include "object.h"   // �I�u�W�F�N�g
//#include "objectX.h"  // �I�u�W�F�N�gX
//#include "blockX.h"               // �u���b�N
//#include "enemyX.h"               // �G�l�~�[
////=====�V�[��============
#include "title.h"     // �^�C�g��


//======================================
//�R���X�g���N�^
//======================================
CManager::CManager()
{
	//srand((float)time(NULL));

	m_pRenderer = nullptr;
	m_pCamera = nullptr;
	m_pLight = nullptr;
	m_pKeyboard = nullptr;
	m_pKeyJoypad = nullptr;
	m_pScene = nullptr;
	//m_pFade = nullptr;
	m_pKeyMouse = nullptr;
}

//=======================================
//�f�X�g���N�^
//=======================================
CManager::~CManager()
{

}

//=====================================
//�����ݒ�
//=====================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer == nullptr)
	{
		// �����_���[�̐��� �����_���[�͈�ԏ�I
		m_pRenderer = new CRenderer();
		m_pRenderer->Init(hWnd, bWindow);
	}
	if (m_pCamera == nullptr)
	{
		//�J�����̐���
		m_pCamera = new CCamera();
		m_pCamera->Init();
	}
	if (m_pLight == nullptr)
	{
		//���C�g�̐���
		m_pLight = new CLight();
		m_pLight->Init();
	}

	if (m_pKeyboard == nullptr)
	{
		//�L�[�{�[�h�̐���
		m_pKeyboard = new CInputKeyboard();
		m_pKeyboard->Init(hInstance, hWnd);
	}
	if (m_pKeyJoypad == nullptr)
	{
		//�R���g���[���[�̐���
		m_pKeyJoypad = new CInputJoypad();
		m_pKeyJoypad->Init(hInstance, hWnd);
	}
	if (m_pKeyMouse == nullptr)
	{
		//�}�E�X�̐���
		m_pKeyMouse = new CInputMouse();
		m_pKeyMouse->Init(hInstance, hWnd);
	}
	//if (m_pFade == nullptr)
	//{
	//	//�t�F�[�h�̐���
	//	m_pFade = new CFade();
	//	m_pFade->Init();
	//}
	if (m_pScene == nullptr)
	{
		// �V�[���̐���
		m_pScene = new CTitle();
	}


	//�ŏ��̃V�[���ݒ�
	//SetMode(CScene::MODE::MODE_TITLE);
	m_pFade->SetFade(CScene::MODE::MODE_TITLE);

	return S_OK;
}

//=====================================
// �I��
//=====================================
void CManager::Uninit()
{	
	// Cleanup
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	// �����_���[��j������
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
	//�J�����̔j��
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	//���C�g�̔j��
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}
	// �L�[�{�[�h���͔j��
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}
	// �R���g���[���[���͔j��
	if (m_pKeyJoypad != nullptr)
	{
		m_pKeyJoypad->Uninit();
		delete m_pKeyJoypad;
		m_pKeyJoypad = nullptr;
	}
	// �}�E�X���͔j��
	if (m_pKeyMouse != nullptr)
	{
		m_pKeyMouse->Uninit();
		delete m_pKeyMouse;
		m_pKeyMouse = nullptr;
	}

	//// �V�[���̔j��
	//if (m_pScene != nullptr)
	//{
	//	m_pScene->Uninit();
	//	delete m_pScene;
	//	m_pScene = nullptr;
	//}

	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}


	if (m_pEffect) {
		m_pEffect->Release();
		m_pEffect = nullptr;
	}

}

//=====================================
// �X�V
//=====================================
void CManager::Update()
{

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();



	// �V�[���̍X�V
	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}
	// �����_���[�̍X�V
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}
	// �L�[�{�[�h�̍X�V
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Update();
	}
	// �R���g���[���[�̍X�V
	if (m_pKeyJoypad != nullptr)
	{
		m_pKeyJoypad->Update();
	}
	// �}�E�X�̍X�V
	if (m_pKeyMouse != nullptr)
	{
		m_pKeyMouse->Update();
	}
	// �J�����̍X�V
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}
	// ���C�g�̍X�V
	if (m_pLight != nullptr)
	{
		m_pLight->Update();
	}
	if (m_pFade != nullptr)
	{
		m_pFade->Update();
	}

	
	// Rendering
	ImGui::EndFrame();

}

//======================================
//�`��
//======================================
void CManager::Draw()
{
	// �����_���[�̕`��
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}
	//�V�[���̕`��
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}

	//// �f�o�C�X�擾
	//LPDIRECT3DDEVICE9 pDevice = m_pRenderer->GetDevice();

	////// ��ʁ�Z�o�b�t�@���N���A�i���ꂪ�Ȃ��Ǝc�����o��j
	////pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
	////	D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//// �`��J�n
	//if (SUCCEEDED(pDevice->BeginScene()))
	//{
	//	// �r���[�E�v���W�F�N�V�����Ȃǂ̐ݒ�
	//	// ���f���`��i���t���[���K�v�I�j
	//	if (m_pLight != nullptr)
	//	{
	//		m_pLight->Draw();
	//	}

	//	pDevice->EndScene();
	//}

	//// �\��
	//pDevice->Present(NULL, NULL, NULL, NULL);

	if (m_pFade != nullptr)
	{
		m_pFade->Draw();
	}
}



//=======================================
// �����_���[�̎擾
//=======================================
CRenderer* CManager::GetRenderer()
{
	assert(m_pRenderer != nullptr);
	return m_pRenderer;
}
//======================================
//�J�����̎擾
//=====================================
CCamera* CManager::GetCamera()
{
	assert(m_pCamera != nullptr);
	return m_pCamera;
}
//======================================
//���C�g�̎擾
//=====================================
CLight* CManager::GetLight()
{
	assert(m_pLight != nullptr);
	return m_pLight;
}
//======================================
//�L�[�{�[�h�̎擾
//=====================================
CInputKeyboard* CManager::GetKeyboard()
{
	assert(m_pKeyboard != nullptr);
	return m_pKeyboard;
}
//======================================
//�R���g���[���[�̎擾
//=====================================
CInputJoypad* CManager::GetJoypad()
{
	assert(m_pKeyJoypad != nullptr);
	return m_pKeyJoypad;
}
//======================================
//�}�E�X�̎擾
//=====================================
CInputMouse* CManager::GetMouse()
{
	assert(m_pKeyMouse != nullptr);
	return m_pKeyMouse;
}

//======================================
//�V�[���̐ݒ�
//======================================
void CManager::SetMode(CScene::MODE mode)
{
	// �V�[���̔j��
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}
	//����
	m_pScene = CScene::Create(mode);

}

//======================================
//�V�[���̎擾
//=====================================
CScene* CManager::GetScene()
{
	assert(m_pScene != nullptr);
	return m_pScene;
}

//======================================
//�t�F�[�h�̎擾
//=====================================
CFade* CManager::GetFade()
{
	assert(m_pFade != nullptr);
	return m_pFade;
}
