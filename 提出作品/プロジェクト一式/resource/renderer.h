//=================================
// 
//�����_���[�N���X(DirectX�`��)renderer.h
//Auther kida ibuki 
// 
//==================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "pch.h"
//�����_���[�N���X�̒�`
class CRenderer
{
public:
	CRenderer();		//�R���X�g���N�^
	~CRenderer();		//�f�X�g���N�^
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	void DrawFPS();
	bool GetDebugOn() { return isDebugOn; }
	void SetDebugOn(bool isDebug) { isDebugOn = isDebug; }
	void DrawDebugText();
	LPDIRECT3DDEVICE9 GetDevice();	//3D�f�o�C�X�̎擾
private:
	LPDIRECT3D9 m_pD3D;		//Direct3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;		//Direct3D�f�o�C�X
	LPD3DXFONT m_pFont;

	bool isDebugOn;
};

#endif