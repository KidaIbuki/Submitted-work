//================================
//
//「気」の蓄積(EffectAccumulation.h)
// Auther kida ibuki
// 
//================================
#ifndef _EFFECTACCUMULATION_H_
#define _EFFECTACCUMULATION_H_

#include "pch.h"

class EffectAccumulation :public CObject2D
{
public:
	EffectAccumulation();	// コンストラクタ
	~EffectAccumulation();	// デストラクタ
	HRESULT Init();			// 初期設定
	void Uninit();			// 終了処理
	void Update();			// 更新処理
	void Draw();			// 描画処理
	static EffectAccumulation* Create();	// 生成

private:
	LPDIRECT3DTEXTURE9 m_sceneRenderTarget;
	LPDIRECT3DTEXTURE9 m_fireTexture;
	LPD3DXEFFECT m_fireEffect;
	LPDIRECT3DSURFACE9 m_sceneSurface;
	LPDIRECT3DVERTEXBUFFER9 m_fullscreenVB; // フルスクリーンクアッド用
	float m_fireTime;
	std::string m_fxFilePath;             // 読み込む.fxファイルのパス

	// カラー保持変数
	float m_colorR;
	float m_colorG;
	float m_colorB;
	// 円と尖りの強弱変数
	float m_circlePowor;
	float m_sharpnessPowor;
	float m_circleSize;
	// 間をあける変数
	//float m_holeMin;
	//float m_holeMax;
	bool m_enableEffect;  // エフェクトの表示状態
	bool m_bActive;
	void InitPostEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);  // シェーダーと各処理の初期化
	void ShutdownPostEffect();  // 変数の破棄
	void UpdatePostEffect(float deltaTime);  // シェーダーの更新
	void RenderEffect(LPDIRECT3DDEVICE9 device);  // シェーダーの描画

	// 頂点の構造体
	struct ScreenVertex {
		D3DXVECTOR4 pos;
		D3DXVECTOR2 uv;
	};
	// 頂点フォーマット(このシェーダー専用)
#define FVF_SCREENVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

	static constexpr float TEX_SIZE_X = 1280.0f;  // テクスチャ座標 X
	static constexpr float TEX_SIZE_Y = 720.0f;   // テクスチャ座標 Y

};


#endif