//=================================
// 
// �}�l�[�W���@manager.h
// Auther kida ibuki 
// 
//==================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "pch.h"   // �v���R���p�C���ς݃w�b�_�[
#include "renderer.h"      // �����_���[
#include "camera.h"        // �J����
#include "light.h"         // ���C�g
#include "input.h"         // ����
#include "scene.h"         // �V�[��
#include "fade.h"          // �t�F�[�h

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../ImGui/imgui_impl_win32.h"

class CManager;

// �}�l�[�W���N���X�̒�`
class CManager
{
private:
	CManager();															//�R���X�g���N�^
public:
	~CManager();														//�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);			//�����ݒ�
	void Uninit();														//�I������
	void Update();														//�X�V����
	void Draw();	                                                    //�`�揈��

	//�V���O���g��
	static CManager* GetInstance()
	{
		static CManager instance; // �ÓI�C���X�^���X
		return &instance;
	}
	
	CRenderer* GetRenderer();			// �����_���[�̎擾
	CCamera* GetCamera();				// �J�����̎擾
	CLight* GetLight();					// ���C�g�̎擾


	CInputKeyboard* GetKeyboard();	    // �L�[�{�[�h���͂̎擾
	CInputJoypad* GetJoypad();		    // �R���g���[���[���͂̎擾
	CInputMouse* GetMouse();		    // �}�E�X���͂̎擾
										   
	void SetMode(CScene::MODE mode);	// �V�[���̎擾
	void SetPause(bool bPause) { m_bPause = bPause; }   // �|�[�Y��ݒ�

	CScene* GetScene();

	CFade* GetFade();					// �t�F�[�h�̎擾
	bool GetPause() { return m_bPause; }   //�|�[�Y�����擾

private:
	CRenderer* m_pRenderer;				// �����_���[���
	CCamera* m_pCamera;					// �r���[���
	CLight* m_pLight;                   // ���C�g���
	CInputKeyboard* m_pKeyboard;		// �L�[�{�[�h���
	CInputJoypad* m_pKeyJoypad;		    // �R���g���[���[���
	CInputMouse* m_pKeyMouse;		    // �}�E�X���
										   
	CScene* m_pScene;					// �V�[�����

	CFade* m_pFade;						// �t�F�[�h�̏��
	CScene::MODE m_mode;
	bool m_bPause;

	LPD3DXEFFECT m_pEffect;     // �G�t�F�N�g

};

#endif 
