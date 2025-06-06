//==============================
//
//ポリゴン描画c++版
// Aurther kida ibuki
//==============================
#include "manager.h"  // マネージャー
#include "main.h"     // メイン
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../ImGui/imgui_impl_win32.h"

//=========================
//グローバル宣言
//=========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int g_nCountFPS = 0;
//=========================
//メイン関数
//=========================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_  LPSTR lpCmdLine, _In_  int nCmdShow)
{
	DWORD dwCurrentTime;				//現在時間
	DWORD dwExecLastTime;				//最後に処理した時刻
	DWORD dwFormeCount;					//フレームカウント
	DWORD dwFPSLastTime;				//最後にFPSを計測した時刻


	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//windowclassのメモリサイズ
		CS_CLASSDC,						//ウィンドウのスタイル
		WindowProc,						//ウィンドウプロシージャ
		0,								//０にする(通常は使用しない)
		0,								//０にする(通常は使用しない)
		hInstance,						//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 23),		//クライアント領域の背景色
		NULL,							//メニューバー
		CLASS_NAME,						//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),	//ファイルのアイコン
	};

	HWND hWnd;							//ウィンドウハンドル(識別子)
	MSG msg;							//メッセージを格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウィンドウの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定サイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウの作成
	hWnd = CreateWindowEx(0,			//拡張ウィンドウスタイル
		CLASS_NAME,						//ウィンドウクラスの名前
		WINDOW_NAME,					//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,			//ウィンドウスタイル
		CW_USEDEFAULT,					//ウィンドウの左上x座標
		CW_USEDEFAULT,					//ウィンドウの左上y座標
		(rect.right - rect.left),			//ウィンドウの幅
		(rect.bottom - rect.top),			//ウィンドウの高さ
		NULL,							//親ウィンドウのハンドル
		NULL,							//メニューハンドルまたは子ウィンドウID
		hInstance,						//インスタンスハンドル
		NULL);							//ウィンドウ作成データ

	CManager* pManager = CManager::GetInstance();
	

	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{
		return-1;
	}
#ifndef _DEBUG
	//// フルスクリーン化
	//ShowWindow(hWnd, SW_SHOW);
	//SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	//SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
#endif
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);			//ウィンドウの表示状態を設定
	UpdateWindow(hWnd);					//クライアント領域を更新

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);


	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;					//初期化する
	dwExecLastTime = timeGetTime();		//現在時刻を取得（保存）

	dwFormeCount = 0;
	dwFPSLastTime = timeGetTime();

	//ShowCursor(FALSE);  // マウスを非表示
	

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				TranslateMessage(&msg);//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);//ウィンドウプロシージャへメッセージへ送出
			}
		}
		else
		{//DirectXの処理

			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSの計測
				g_nCountFPS = (dwFormeCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	//FPSを測定した時刻を保存
				dwFormeCount = 0;				//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;//処理開始の時刻を保存

				pManager->Update();
				pManager->Draw();

				dwFormeCount++;
			}
		}

	}

	//全オブジェクト解放
	pManager->Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=========================
//ウィンドウプロシージャ
//=========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	//返り値を格納
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}


	switch (uMsg)
	{
	case WM_DESTROY:				//ウィンドウ破棄メッセージ
	//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:				//[ESC]キーが押された
			//ウィンドウを破棄する（WM_DESTROYメッセージを送る）
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}

			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{//ウィンドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;				//０を返さないと終了してしまう
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);//既定の処理を返す
}

//===================================
// FPSの取得
//===================================
int GetFPS()
{
	return g_nCountFPS;
}
