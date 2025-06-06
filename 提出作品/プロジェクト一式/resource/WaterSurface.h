//================================
//
// フレネル効果付きの波シェーダー(WaterSurface.h)
// Auther kida ibuki
// 
//================================
#ifndef _WATERSURFACE_H_
#define _WATERSURFACE_H_

#include "pch.h"
#include "object3D.h"
#include "objectX.h"
class Wave : public CObjectX
{
public:
	Wave();	// コンストラクタ
	~Wave();	// デストラクタ
	HRESULT Init();			// 初期設定
	void Uninit();			// 終了処理
	void Update();			// 更新処理
	void Draw();			// 描画処理
	static Wave* Create();	// 生成
private:

	bool InitWaterEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void ShutdownWaterEffect();  // 変数の破棄
	void UpdateWaterEffect(float deltaTime, D3DXVECTOR3 camPos);  // シェーダーの更新
	void RenderWaterEffect(LPDIRECT3DDEVICE9 device, const D3DXMATRIX& viewProj);  // シェーダーの描画
	void RenderReflectionTexture(LPDIRECT3DDEVICE9 device);
	void RenderSceneForReflection(LPDIRECT3DDEVICE9 device);
	std::string m_fxFilePath;             // 読み込む.fxファイルのパス

	static constexpr int NUM_TEX = 3;
	LPD3DXEFFECT m_pWaterEffect;
	LPD3DXMESH m_pWaterMesh;
	LPDIRECT3DCUBETEXTURE9 m_pEnvMap;
	D3DXHANDLE m_hTech, m_hWorld, m_hView, m_hProj, m_hTime, m_hEnvMap;
	float m_time;
	float m_animPow;
	LPDIRECT3DTEXTURE9 m_normalMap[NUM_TEX];
	bool m_bChange;
	int m_currentTextureIndex;
	LPDIRECT3DTEXTURE9 m_pReflectionTex;
	LPDIRECT3DSURFACE9 m_pReflectionSurf;
	D3DXMATRIX m_reflectionViewProj;
};

#endif