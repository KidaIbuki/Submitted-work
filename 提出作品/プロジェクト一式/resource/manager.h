//=================================
// 
// マネージャ　manager.h
// Auther kida ibuki 
// 
//==================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "pch.h"   // プリコンパイル済みヘッダー
#include "renderer.h"      // レンダラー
#include "camera.h"        // カメラ
#include "light.h"         // ライト
#include "input.h"         // 入力
#include "scene.h"         // シーン
#include "fade.h"          // フェード

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../ImGui/imgui_impl_win32.h"

class CManager;

// マネージャクラスの定義
class CManager
{
private:
	CManager();															//コンストラクタ
public:
	~CManager();														//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);			//初期設定
	void Uninit();														//終了処理
	void Update();														//更新処理
	void Draw();	                                                    //描画処理

	//シングルトン
	static CManager* GetInstance()
	{
		static CManager instance; // 静的インスタンス
		return &instance;
	}
	
	CRenderer* GetRenderer();			// レンダラーの取得
	CCamera* GetCamera();				// カメラの取得
	CLight* GetLight();					// ライトの取得


	CInputKeyboard* GetKeyboard();	    // キーボード入力の取得
	CInputJoypad* GetJoypad();		    // コントローラー入力の取得
	CInputMouse* GetMouse();		    // マウス入力の取得
										   
	void SetMode(CScene::MODE mode);	// シーンの取得
	void SetPause(bool bPause) { m_bPause = bPause; }   // ポーズを設定

	CScene* GetScene();

	CFade* GetFade();					// フェードの取得
	bool GetPause() { return m_bPause; }   //ポーズ中か取得

private:
	CRenderer* m_pRenderer;				// レンダラー情報
	CCamera* m_pCamera;					// ビュー情報
	CLight* m_pLight;                   // ライト情報
	CInputKeyboard* m_pKeyboard;		// キーボード情報
	CInputJoypad* m_pKeyJoypad;		    // コントローラー情報
	CInputMouse* m_pKeyMouse;		    // マウス情報
										   
	CScene* m_pScene;					// シーン情報

	CFade* m_pFade;						// フェードの情報
	CScene::MODE m_mode;
	bool m_bPause;

	LPD3DXEFFECT m_pEffect;     // エフェクト

};

#endif 
