//==============================================
// 
//オブジェクト(xファイルの読み込み)　objectX.h
//Auther kida ibuki 
// 
//==============================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "object.h"
#include "pch.h"
#include "main.h"

#define MAX_CHAR (256)
#define NUM_MODEL (21) //モデルの種類
#define MAX_TEX_X (36) // 1モデルのテクスチャの最大数

class CCamera;

//オブジェクトXクラスの定義
class CObjectX :public CObject
{
public:
	//**==================基本処理====================**
	CObjectX();		//コンストラクタ
	CObjectX(int nPriority);		//コンストラクタ
	~CObjectX() override;		//デストラクタ
	HRESULT Init() override;	//初期設定
	void Uninit() override;		//終了処理
	void Update() override;		//更新処理
	void Draw() override;		//描画処理

	void InitEffect(LPDIRECT3DDEVICE9 device);
	void UpdateEffect(LPDIRECT3DDEVICE9 device);
	void DrawWithEffect(LPDIRECT3DDEVICE9 device);
	bool LoadEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void LoadXModel(LPDIRECT3DDEVICE9 device, const char* filename);
	//void DrawModelWithEffect(LPD3DXEFFECT pEffect);
	bool LoadEffect_fxo(LPDIRECT3DDEVICE9 device, const char* fxoFile);


	//ID3DXMesh* LoadMeshFromX(LPDIRECT3DDEVICE9 device, const char* filename);
	//**=================各生成の処理==================**
	static CObjectX* Create();		//オブジェクトの生成
	void SetModel(std::string sPath);		//モデルのセット
	void SizeVtx();						// オブジェクトのサイズの最大最小を求める
	//**===============各設定の取得======================**
	D3DXVECTOR3 GetPos() { return m_pos; }			//位置の取得
	D3DXVECTOR3 GetSize() { return m_size; }		//サイズの取得
	D3DXVECTOR3 GetRot() { return m_rot; }

	void SetPos(D3DXVECTOR3 pos);			//各オブジェクトの位置設定
	void SetRot(D3DXVECTOR3 rot);
	void SetSize(D3DXVECTOR3 size);	//各オブジェクトのサイズ設定

	D3DXMATRIX GetWorldMatrix() { return m_mtxWorld; }  //ワールドマトリックスを取得(階層構造に必要)
	void SetWorldMatrix(D3DXMATRIX mat) { m_mtxWorld = mat; }
	LPD3DXMESH GetMesh() { return m_pMesh; }

	D3DXVECTOR3 m_pos;							//位置
	D3DXVECTOR3 m_rot;							//向き
	D3DXVECTOR3 m_move;							//移動量
	D3DXVECTOR3 m_size;							//サイズ
	LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEX_X];				//テクスチャ

protected:
	//**==============描画の情報=================**
	D3DXMATRIX m_mtxWorld;		//ワールド変換行列
	LPD3DXMESH m_pMesh;			//メッシュへのポインタ
	LPD3DXBUFFER m_pBuffMat;    //マテリアルへのポインタ
	DWORD m_dwNumMat;           //マテリアルの数

	// メッシュの描画用変数
	ID3DXMesh* mesh;
	ID3DXBuffer* adjacencyBuffer;
	ID3DXBuffer* materialBuffer;
	DWORD numMaterials;
	//モデルファイルの読み込み構造体
	typedef struct
	{
		int nType;
		char cFile[MAX_CHAR];		//ファイルを読み込む
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		int nIndexFile;
		int Parent;			//パーツの親子関係
	}MODEL_FILENAME;

	MODEL_FILENAME m_gModelData[NUM_MODEL];

	//**==========メンバ変数================**
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファ
	//LPDIRECT3DTEXTURE9 m_pTexture;				//テクスチャ

	std::string m_fxFilePath;             // 読み込む.fxファイルのパス
	LPDIRECT3DTEXTURE9 m_pToonMap;  // トゥーンマップ

	ID3DXMesh* m_pMesh1;
	D3DMATERIAL9* m_pMaterials;
	LPDIRECT3DTEXTURE9* m_pTextures1;
	DWORD m_dwNumMaterials;
	LPD3DXEFFECT m_pEffect;
	LPD3DXBUFFER m_pMaterialBuffer;
private:
	//**=============このオブジェクトXでしか使えない関数===============**




	//**===========モデルのメッシュ情報取得=============**
	D3DXVECTOR3 m_vtxMinModel;		//モデルの頂点の最小値
	D3DXVECTOR3 m_vtxMaxModel;		//モデルの頂点の最大値
	int m_nNumVtx;					//頂点数
	DWORD m_sizeFVF;				//頂点フォーマットのサイズ
	BYTE* m_bpVtxBuff;				//頂点バッファへのポインタ
	D3DXVECTOR3 m_nMaxVtx;					//モデルの最大値
	D3DXVECTOR3 m_nMinVtx;					//モデルの最小値


		// シャドウマップ用シェーダー変数
	LPDIRECT3DTEXTURE9 m_pShadowMap;
	LPDIRECT3DSURFACE9 m_pShadowSurf;
	static const int SHADOWMAP_SIZE;
	std::vector<CObjectX*> objects;
	LPDIRECT3DSURFACE9 m_pMainBackBuffer;
	D3DXMATRIX m_matLightView, m_matLightProj;
	LPDIRECT3DSURFACE9 m_pShadowDepth;
	LPDIRECT3DSURFACE9 m_pMainDepthBuffer;
	LPDIRECT3DSURFACE9 m_pShadowDepthBuffer;
	D3DXMATRIX m_LightView, m_LightProj, m_world;



};

//class Terrain
//{
//public:
//	Terrain(LPD3DXMESH mesh);
//	
//
//	struct Triangle {
//		D3DXVECTOR3 v0, v1, v2;  // 三角形の頂点
//		D3DXVECTOR3 normal;      // 法線ベクトル
//	};
//
//	const std::vector<Triangle>& GetTriangles() const { return triangles; }
//
//private:
//	std::vector<Triangle> triangles;
//
//	void ExtractPolygons(LPD3DXMESH mesh);
//	
//};

#endif
