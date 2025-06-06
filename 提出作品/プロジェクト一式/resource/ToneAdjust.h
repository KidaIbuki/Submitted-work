//==============================================
// 
//ポストエフェクト[色調変化]　ToneAdjust.h
//Auther kida ibuki 
// 
//==============================================
#ifndef _TONEADJUST_H_
#define _TONEADJUST_H_


#include "pch.h"
#include "object2D.h"
class ToneAdjust :public CObject2D
{
public:
	ToneAdjust();		//コンストラクタ
	~ToneAdjust()override;		//デストラクタ
	HRESULT Init()override;	//初期設定
	void Uninit()override;		//終了処理
	void Update()override;		//更新処理
	void Draw()override;		//描画処理

	static ToneAdjust* Create(D3DXVECTOR2 pos);

	bool InitToneAdjust(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UninitToneAdjust();
	void UpdateToneAdjust();
	void RenderToneAdjust(LPDIRECT3DDEVICE9 device);
	void CreateFullScreenQuad(LPDIRECT3DDEVICE9 device);
private:
	LPD3DXEFFECT m_pEffect;    // エフェクト
	std::string m_fxFilePath;             // 読み込む.fxファイルのパス

	struct SCREEN_VERTEX {
		D3DXVECTOR4 pos;
		D3DXVECTOR2 tex;
		D3DCOLOR color; 
	};
#define FVF_SCREEN_VERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE)

	bool m_enableEffect;  // エフェクトの表示状態
	bool m_bActive;
	// カラー保持変数
	float m_colorR;
	float m_colorG;
	float m_colorB;

};

#endif