//=================================
// 
//レンダラークラス(DirectX描画)renderer.h
//Auther kida ibuki 
// 
//==================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "pch.h"
//レンダラークラスの定義
class CRenderer
{
public:
	CRenderer();		//コンストラクタ
	~CRenderer();		//デストラクタ
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	void DrawFPS();
	bool GetDebugOn() { return isDebugOn; }
	void SetDebugOn(bool isDebug) { isDebugOn = isDebug; }
	void DrawDebugText();
	LPDIRECT3DDEVICE9 GetDevice();	//3Dデバイスの取得
private:
	LPDIRECT3D9 m_pD3D;		//Direct3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;		//Direct3Dデバイス
	LPD3DXFONT m_pFont;

	bool isDebugOn;
};

#endif