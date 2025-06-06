//================================
//
//xファイルでブロック(blockX.h)
//Auther kida ibuki
//================================
#ifndef _BLOCKX_H_
#define _BLOCKX_H_

#include "objectX.h"
#include "object.h"

//ブロックのクラス定義
class CBlockX : public CObjectX
{
public:
	CBlockX();																				//コンストラクタ
	~CBlockX();																		//デストラクタ
	HRESULT Init();																//初期設定
	void Uninit();																	//終了処理
	void Update();																	//更新処理
	void Draw();																	//描画処理
	static CBlockX* Create(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 rot);					//ブロックの生成
	D3DXVECTOR3 Move();		//オブジェクト(仮プレイヤー)の移動処設定
	static CBlockX* Create2(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat);


protected:

private:

	// 材質変換シェーダーの変数
	D3DXVECTOR4 m_DiffuseColor;
	D3DXVECTOR4 m_SpecularColor;
	float m_Shininess;

	D3DXVECTOR3 m_LightDir;
	D3DXVECTOR3 m_CameraPos;
	bool m_bUseShader;  // シェーダーの切り替え

	//LPD3DXEFFECT m_pEffect;     // エフェクト
	std::string m_fxFilePath;             // 読み込む.fxファイルのパス
	D3DXMATRIX m_matWorld, m_matView, m_matProj;
	float m_RimPowor;
	float m_RimIntensity;

	// ImGui用ライトの方向変更変数
	float vecX, vecY, vecZ;

	// テクスチャの材質を変更シェーダー関数
	bool LoadTextureEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UninitTextureEffect();
	void UpdateTextureEffect(LPD3DXEFFECT effect, const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& proj, const D3DXVECTOR3& cameraPos, const D3DXVECTOR3& lightDir);
	void DrawTextureEffect(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, LPD3DXMESH mesh);

	// ToonShader2
	void UpdateToonShader(LPDIRECT3DDEVICE9 device);
	void RenderToonShader(LPDIRECT3DDEVICE9 device);

	// liltoon(失敗)
	void UpdateShaderParameters(LPD3DXEFFECT effect, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);
	void RenderMeshWithEffect(LPDIRECT3DDEVICE9 pDevice);


	// アウトライン、リムライト、セルルック(失敗)
	bool InitShader(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UpdateShader(LPDIRECT3DDEVICE9 device/*, const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& proj*/);
	void RenderWithToonShader(LPD3DXMESH mesh, LPDIRECT3DDEVICE9 device);

	// 距離歪み
	bool InitDistanceShader(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UpdateDistanceShader(LPDIRECT3DDEVICE9 device, const D3DXVECTOR3& cameraPos);
	void RenderDistanceShader(LPDIRECT3DDEVICE9 device);

};
#endif
