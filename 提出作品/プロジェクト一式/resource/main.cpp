//==============================
//
//�|���S���`��c++��
// Aurther kida ibuki
//==============================
#include "manager.h"  // �}�l�[�W���[
#include "main.h"     // ���C��
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../ImGui/imgui_impl_win32.h"

//=========================
//�O���[�o���錾
//=========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int g_nCountFPS = 0;
//=========================
//���C���֐�
//=========================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_  LPSTR lpCmdLine, _In_  int nCmdShow)
{
	DWORD dwCurrentTime;				//���ݎ���
	DWORD dwExecLastTime;				//�Ō�ɏ�����������
	DWORD dwFormeCount;					//�t���[���J�E���g
	DWORD dwFPSLastTime;				//�Ō��FPS���v����������


	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//windowclass�̃������T�C�Y
		CS_CLASSDC,						//�E�B���h�E�̃X�^�C��
		WindowProc,						//�E�B���h�E�v���V�[�W��
		0,								//�O�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,								//�O�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,						//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),		//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 23),		//�N���C�A���g�̈�̔w�i�F
		NULL,							//���j���[�o�[
		CLASS_NAME,						//�E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),	//�t�@�C���̃A�C�R��
	};

	HWND hWnd;							//�E�B���h�E�n���h��(���ʎq)
	MSG msg;							//���b�Z�[�W���i�[����ϐ�

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�E�B���h�E�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,			//�g���E�B���h�E�X�^�C��
		CLASS_NAME,						//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,					//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,			//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,					//�E�B���h�E�̍���x���W
		CW_USEDEFAULT,					//�E�B���h�E�̍���y���W
		(rect.right - rect.left),			//�E�B���h�E�̕�
		(rect.bottom - rect.top),			//�E�B���h�E�̍���
		NULL,							//�e�E�B���h�E�̃n���h��
		NULL,							//���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,						//�C���X�^���X�n���h��
		NULL);							//�E�B���h�E�쐬�f�[�^

	CManager* pManager = CManager::GetInstance();
	

	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{
		return-1;
	}
#ifndef _DEBUG
	//// �t���X�N���[����
	//ShowWindow(hWnd, SW_SHOW);
	//SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	//SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
#endif
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);			//�E�B���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);					//�N���C�A���g�̈���X�V

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);


	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;					//����������
	dwExecLastTime = timeGetTime();		//���ݎ������擾�i�ۑ��j

	dwFormeCount = 0;
	dwFPSLastTime = timeGetTime();

	//ShowCursor(FALSE);  // �}�E�X���\��
	

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				TranslateMessage(&msg);//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);//�E�B���h�E�v���V�[�W���փ��b�Z�[�W�֑��o
			}
		}
		else
		{//DirectX�̏���

			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				//FPS�̌v��
				g_nCountFPS = (dwFormeCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPS�𑪒肵��������ۑ�
				dwFormeCount = 0;				//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;//�����J�n�̎�����ۑ�

				pManager->Update();
				pManager->Draw();

				dwFormeCount++;
			}
		}

	}

	//�S�I�u�W�F�N�g���
	pManager->Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=========================
//�E�B���h�E�v���V�[�W��
//=========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	//�Ԃ�l���i�[
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}


	switch (uMsg)
	{
	case WM_DESTROY:				//�E�B���h�E�j�����b�Z�[�W
	//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:				//[ESC]�L�[�������ꂽ
			//�E�B���h�E��j������iWM_DESTROY���b�Z�[�W�𑗂�j
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}

			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{//�E�B���h�E��j������
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;				//�O��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);//����̏�����Ԃ�
}

//===================================
// FPS�̎擾
//===================================
int GetFPS()
{
	return g_nCountFPS;
}
