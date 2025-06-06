//＝＝＝＝＝＝＝＝＝＝＝＝＝
//
//入力処理（input.h)
//Author ibuki kida
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _INPUT_H_	//このマクロ定義がされてなかったら
#define _INPUT_H_	//二重インクルード防止のマクロ定義

#include "main.h"   // メイン
#include "pch.h"  // プリコンパイル済みヘッダー

#define MAX_KEY (256)          // キーの最大数


//入力基底クラス(DirectInputの管理)
class CInput
{
public:
	CInput();		//コンストラクタ
	virtual ~CInput();		//デストラクタ
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;	//純粋仮想関数の更新処理

protected:
	//全入力処理で共有
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

//キーボード入力(派生クラス)
class CInputKeyboard :public CInput
{
public:
	CInputKeyboard();		//コンストラクタ
	~CInputKeyboard() override;		//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;
	bool GetPress(int nKey);		//プレス状態取得
	bool GetTrigger(int nKey);		//トリガー状態取得

private:
	BYTE m_aKeyState[MAX_KEY];
	BYTE m_aKeyStateTrigger[MAX_KEY];
};
//パッド入力(派生クラス)↓
class CInputJoypad :public CInput
{
public:
	//キーの種類
	typedef enum
	{
		JOYKEY_UP = 0,			// 十字キー&スティック(上)
		JOYKEY_DOWN,			// 十字キー&スティック(下)
		JOYKEY_LEFT,			// 十字キー&スティック(左)					この順番じゃないとキーがわからなくなる！
		JOYKEY_RIGHT,			// 十字キー&スティック(右)
		JOYKEY_START,			// STARTボタン
		JOYKEY_BACK,			// BACKボタン
		JOYKEY_LEFT_THUMB,		// 左スティック押し込み
		JOYKEY_RIGHT_THUMB,		// 右スティック押し込み
		JOYKEY_LEFT_SHOULDER,	// LBボタン
		JOYKEY_RIGHT_SHOULDER,	// RBボタン
		JOYKEY_UNCOMN1,			// 謎キー
		JOYKEY_UNCOMN2,			   
		JOYKEY_A,				// Aボタン
		JOYKEY_B,				// Bボタン
		JOYKEY_X,				// Xボタン
		JOYKEY_Y,				// Yボタン
		JOYKEY_MAX
	}JOYKEY;

	CInputJoypad();		//コンストラクタ
	~CInputJoypad() override;		//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;
	bool GetJoypadPress(JOYKEY key);
	bool GetJoypadTrigger(JOYKEY key);
	WORD GetJoypadStick(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone);
	bool GetJoyR();
	XINPUT_STATE GetStickR();  //右スティック情報を取得
private:
	XINPUT_STATE m_joyKeyState;		//ジョイパッドのプレス情報
	XINPUT_STATE m_joyKeyStateTrigger;		//トリガー情報
	XINPUT_VIBRATION m_JoypadMotor;				//コントローラのモーター情報			
};

#define ON_VALUE (0x80)  // オン状態の値

class CInputMouse :public CInput
{
public:
	// マウスのボタン種類
	typedef enum
	{
		MOUSEBUTTON_LEFT = 0,   // 左クリック
		MOUSEBUTTON_RIGHT,      // 右クリック
		MOUSEBUTTON_CENTER,     // 中クリック
		MOUSEBUTTON_BACK,       // 戻る
		MOUSEBUTTON_MAX
	}MouseButton;
	CInputMouse();		//コンストラクタ
	~CInputMouse() override;		//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;
	bool GetPress(MouseButton button_type);  // プレス入力
	bool GetTrigger(MouseButton button_type); // トリガー入力
	bool GetRepeat(MouseButton button_type); // リピート入力
	bool GetRelease(MouseButton button_type); // リリース入力
	D3DXVECTOR2 GetPos() { return m_MousePos; }
	D3DXVECTOR2 GetMove() { return m_MouseMove; }
	int GetMouseHweel() { return m_lHoweel; }
private:
	DIMOUSESTATE m_PrevMouseState; // マウスの１フレーム前の入力情報
	DIMOUSESTATE m_CurrentMouseState; // マウスの現在の入力情報
	D3DXVECTOR2 m_MousePos;       // マウスの位置
	D3DXVECTOR2 m_MouseMove;       // マウスの動き
	long m_lHoweel;
	
	// マウス抜いたときおかしくなるよ^^

};

#endif
