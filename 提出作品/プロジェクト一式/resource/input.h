//��������������������������
//
//���͏����iinput.h)
//Author ibuki kida
// 
//��������������������������
#ifndef _INPUT_H_	//���̃}�N����`������ĂȂ�������
#define _INPUT_H_	//��d�C���N���[�h�h�~�̃}�N����`

#include "main.h"   // ���C��
#include "pch.h"  // �v���R���p�C���ς݃w�b�_�[

#define MAX_KEY (256)          // �L�[�̍ő吔


//���͊��N���X(DirectInput�̊Ǘ�)
class CInput
{
public:
	CInput();		//�R���X�g���N�^
	virtual ~CInput();		//�f�X�g���N�^
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;	//�������z�֐��̍X�V����

protected:
	//�S���͏����ŋ��L
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

//�L�[�{�[�h����(�h���N���X)
class CInputKeyboard :public CInput
{
public:
	CInputKeyboard();		//�R���X�g���N�^
	~CInputKeyboard() override;		//�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;
	bool GetPress(int nKey);		//�v���X��Ԏ擾
	bool GetTrigger(int nKey);		//�g���K�[��Ԏ擾

private:
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];
};
//�p�b�h����(�h���N���X)��
class CInputJoypad :public CInput
{
public:
	//�L�[�̎��
	typedef enum
	{
		JOYKEY_UP = 0,			// �\���L�[&�X�e�B�b�N(��)
		JOYKEY_DOWN,			// �\���L�[&�X�e�B�b�N(��)
		JOYKEY_LEFT,			// �\���L�[&�X�e�B�b�N(��)					���̏��Ԃ���Ȃ��ƃL�[���킩��Ȃ��Ȃ�I
		JOYKEY_RIGHT,			// �\���L�[&�X�e�B�b�N(�E)
		JOYKEY_START,			// START�{�^��
		JOYKEY_BACK,			// BACK�{�^��
		JOYKEY_LEFT_THUMB,		// ���X�e�B�b�N��������
		JOYKEY_RIGHT_THUMB,		// �E�X�e�B�b�N��������
		JOYKEY_LEFT_SHOULDER,	// LB�{�^��
		JOYKEY_RIGHT_SHOULDER,	// RB�{�^��
		JOYKEY_UNCOMN1,			// ��L�[
		JOYKEY_UNCOMN2,			   
		JOYKEY_A,				// A�{�^��
		JOYKEY_B,				// B�{�^��
		JOYKEY_X,				// X�{�^��
		JOYKEY_Y,				// Y�{�^��
		JOYKEY_MAX
	}JOYKEY;

	CInputJoypad();		//�R���X�g���N�^
	~CInputJoypad() override;		//�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;
	bool GetJoypadPress(JOYKEY key);
	bool GetJoypadTrigger(JOYKEY key);
	WORD GetJoypadStick(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone);
	bool GetJoyR();
	XINPUT_STATE GetStickR();  //�E�X�e�B�b�N�����擾
private:
	XINPUT_STATE m_joyKeyState;		//�W���C�p�b�h�̃v���X���
	XINPUT_STATE m_joyKeyStateTrigger;		//�g���K�[���
	XINPUT_VIBRATION m_JoypadMotor;				//�R���g���[���̃��[�^�[���			
};

#define ON_VALUE (0x80)  // �I����Ԃ̒l

class CInputMouse :public CInput
{
public:
	// �}�E�X�̃{�^�����
	typedef enum
	{
		MOUSEBUTTON_LEFT = 0,   // ���N���b�N
		MOUSEBUTTON_RIGHT,      // �E�N���b�N
		MOUSEBUTTON_CENTER,     // ���N���b�N
		MOUSEBUTTON_BACK,       // �߂�
		MOUSEBUTTON_MAX
	}MouseButton;
	CInputMouse();		//�R���X�g���N�^
	~CInputMouse() override;		//�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;
	bool GetPress(MouseButton button_type);  // �v���X����
	bool GetTrigger(MouseButton button_type); // �g���K�[����
	bool GetRepeat(MouseButton button_type); // ���s�[�g����
	bool GetRelease(MouseButton button_type); // �����[�X����
	D3DXVECTOR2 GetPos() { return m_MousePos; }
	D3DXVECTOR2 GetMove() { return m_MouseMove; }
	int GetMouseHweel() { return m_lHoweel; }
private:
	DIMOUSESTATE m_PrevMouseState; // �}�E�X�̂P�t���[���O�̓��͏��
	DIMOUSESTATE m_CurrentMouseState; // �}�E�X�̌��݂̓��͏��
	D3DXVECTOR2 m_MousePos;       // �}�E�X�̈ʒu
	D3DXVECTOR2 m_MouseMove;       // �}�E�X�̓���
	long m_lHoweel;
	
	// �}�E�X�������Ƃ����������Ȃ��^^

};

#endif
