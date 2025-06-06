//��������������������������
//
//���͏����iinput.cpp)
//Author ibuki kida
// 
//��������������������������
#include "input.h"   // ����
#include "main.h"    // ���C��

//�ÓI�����o������
LPDIRECTINPUT8 CInput::m_pInput = nullptr;


//==============================
//�R���X�g���N�^
//==============================
CInput::CInput()
{
	 m_pDevice = nullptr;
}
//===============================
//�f�X�g���N�^
//===============================
CInput::~CInput()
{

}

//===================================
//�����ݒ�
//===================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{

	if (m_pInput == nullptr)
	{
		//DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, nullptr)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}
//=================================
//�I������(�j��)
//=================================
void CInput::Uninit()
{
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}

}
//====================================================================
//�L�[�{�[�h�̓���
//====================================================================

//==============================
//�R���X�g���N�^
//==============================
CInputKeyboard::CInputKeyboard()
{

}
//===============================
//�f�X�g���N�^
//===============================
CInputKeyboard::~CInputKeyboard()
{

}
//===================================
//�����ݒ�
//===================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X: �L�[�{�[�h�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;


}
//=================================
//�I������(�j��)
//=================================
void CInputKeyboard::Uninit()
{
	CInput::Uninit();
}
//===================================
//�X�V����
//===================================
void CInputKeyboard::Update()
{
	BYTE aKeyState[MAX_KEY]; //�L�[�{�[�h�̓��͏��
	int nCntKey;

	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //�L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyState[nCntKey] = aKeyState[nCntKey]; //�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire(); //�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}

}
//===================================================================================
//�R���g���[���[�̏���
//===================================================================================

//==================================
//�R���X�g���N�^
//==================================
CInputJoypad::CInputJoypad()
{
}
//==================================
//�f�X�g���N�^
//==================================
CInputJoypad::~CInputJoypad()
{
}
//==================================
//�����ݒ�
//==================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	memset(&m_joyKeyState, 0, sizeof(XINPUT_STATE));

	//�������̃N���A
	memset(&m_JoypadMotor, 0, sizeof(XINPUT_VIBRATION));


	//XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	return S_OK;
}
//==================================
//�I������
//==================================
void CInputJoypad::Uninit()
{
	//XInput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);

}
//==================================
//�X�V����
//==================================
void CInputJoypad::Update()
{
	XINPUT_STATE joykeyState;		//�W���C�p�b�h�̓��͏��
	//int nCntJoykey;

	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{

		//���X�e�B�b�N����̓��͂�����{�^���ɕϊ�
		joykeyState.Gamepad.wButtons |= GetJoypadStick(m_joyKeyState.Gamepad.sThumbLX, m_joyKeyState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		//g_joyKeyStateTrigger[nCntJoykey] = (g_joyKeyState[nCntJoykey] ^ joykeyState[nCntJoykey]) & joykeyState[nCntJoykey];
		m_joyKeyStateTrigger.Gamepad.wButtons = (m_joyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons;

		m_joyKeyState = joykeyState;		//�W���C�p�b�h�̃v���X����ۑ�


	}

}
//==================================
//�E�X�e�B�b�N�̏��
//==================================
bool CInputJoypad::GetJoyR()
{
	bool bGetInput = false;

	if ((m_joyKeyState.Gamepad.sThumbRX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_joyKeyState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_joyKeyState.Gamepad.sThumbRY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			m_joyKeyState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		m_joyKeyState.Gamepad.sThumbRX = 0;
		m_joyKeyState.Gamepad.sThumbRY = 0;
	}
	else
	{
		bGetInput = true;

	}

	return bGetInput;
}
//==================================
//�E�X�e�B�b�N�̏����擾
//==================================
XINPUT_STATE CInputJoypad::GetStickR()
{
	return m_joyKeyState;
}

//==================================
//�R���g���[���[�̓��͏��
//==================================
WORD CInputJoypad::GetJoypadStick(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone)
{
	WORD wButtons = 0;

	//���
	if (sThumbY >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_UP;
	}
	//����
	else if (sThumbY <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
	}

	//��
	if (sThumbX <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
	}
	//�E
	else if (sThumbX >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
	}


	return wButtons;
}


//==============================
//�R���X�g���N�^
//==============================
CInputMouse::CInputMouse()
{

}
//===============================
//�f�X�g���N�^
//===============================
CInputMouse::~CInputMouse()
{

}

//===================================
//�����ݒ�
//===================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	// ���̓f�o�C�X(�}�E�X)���쐬
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		return E_FAIL;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}
	// �f�[�^���[�h�ݒ�(�t�H�A�O���E���h����r�����[�h)
	if (FAILED(m_pDevice->SetCooperativeLevel(FindWindowA(CLASS_NAME, nullptr), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;  // ���Βl���[�h�Őݒ�(��Βl��DIPROPAXISMODE_ABS)

	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	// ���͐���J�n
	m_pDevice->Acquire();

	return S_OK;
}
//=================================
//�I������(�j��)
//=================================
void CInputMouse::Uninit()
{
	CInput::Uninit();

}
//=================================
//�X�V����
//=================================
void CInputMouse::Update()
{
	D3DXVECTOR2 PosOld = m_MousePos;  // 1�t���[���O�̈ʒu
	POINT p;   // ���݂̈ʒu�ۑ��p
	GetCursorPos(&p);
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &p);

	m_MousePos.x = (float)p.x;
	m_MousePos.y = (float)p.y;

	m_MouseMove.x = m_MousePos.x - PosOld.x;
	m_MouseMove.y = m_MousePos.y - PosOld.y;

	m_PrevMouseState = m_CurrentMouseState;

	// �}�E�X�̏�Ԃ��擾
	HRESULT hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
	if (FAILED(hr))
	{
		m_pDevice->Acquire();
		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState);
		m_lHoweel = m_CurrentMouseState.lZ;
	}

}
//==================================
// �}�E�X�̃v���X���
//==================================
bool CInputMouse::GetPress(MouseButton button_type)
{
	if (m_CurrentMouseState.rgbButtons[button_type] != 0)
	{
		return true;
	}

	return false;
}
//==================================
// �}�E�X�̃g���K�[���
//==================================
bool CInputMouse::GetTrigger(MouseButton button_type)
{
	if (!(m_PrevMouseState.rgbButtons[button_type] & ON_VALUE) &&
		m_CurrentMouseState.rgbButtons[button_type] & ON_VALUE)
	{
		return true;
	}
	return false;
}
//==================================
// �}�E�X�̃��s�[�g���
//==================================
bool CInputMouse::GetRepeat(MouseButton button_type)
{
	if (m_PrevMouseState.rgbButtons[button_type] & ON_VALUE &&
		m_CurrentMouseState.rgbButtons[button_type] & ON_VALUE)
	{
		return true;
	}

	return false;
}
//==================================
// �}�E�X�̃����[�X���
//==================================
bool CInputMouse::GetRelease(MouseButton button_type)
{
	if (m_PrevMouseState.rgbButtons[button_type] & ON_VALUE &&
		!(m_CurrentMouseState.rgbButtons[button_type] & ON_VALUE))
	{
		return true;
	}

	return false;
}


//=====================================
//�e���͏��擾
//=====================================

//==================================
//�L�[�{�[�h�̃v���X���擾
//==================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) != 0;
}
//==================================
//�L�[�{�[�h�̃g���K�[�����擾
//==================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) != 0;
}

//==================================
//�R���g���[���[�̃v���X���
//==================================
bool CInputJoypad::GetJoypadPress(JOYKEY key)
{
	return (m_joyKeyState.Gamepad.wButtons & (0x01 << key)) != 0;
}
//==================================
//�R���g���[���[�̃g���K�[���
//==================================
bool CInputJoypad::GetJoypadTrigger(JOYKEY key)
{
	return (m_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) != 0;
}
