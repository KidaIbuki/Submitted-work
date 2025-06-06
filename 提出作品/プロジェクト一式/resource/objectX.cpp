//==============================================
// 
//オブジェクト(xファイルの読み込み)　objectX.cpp
//Auther kida ibuki 
// 
//==============================================
#include "objectX.h"
#include "manager.h"
#include <fstream>  // バイナリ化に必要
const int CObjectX::SHADOWMAP_SIZE = 1024;   // シャドウマップの大きさ

//==================================================
//コンストラクタ
//==================================================
CObjectX::CObjectX()
{
	// 変数の初期化
	m_pMesh = nullptr;
	m_pBuffMat = nullptr;
	m_dwNumMat = 0;
	m_pVtxBuff = nullptr;				// 頂点バッファ
	for (int i = 0; i < MAX_TEX_X; i++)
	{
		m_pTexture[i] = nullptr;			// テクスチャ
	}
	m_pos = { 0.0f, 0.0f, 0.0f };		// 位置
	m_rot = { 0.0f, 0.0f, 0.0f };		// 向き
	//m_move = { 0.0f, 0.0f, 0.0f };		//移動量
	m_size = { 0.0f,0.0f,0.0f };
	m_nMaxVtx = { 0.0f,0.0f,0.0f };
	m_nMinVtx = { 0.0f,0.0f,0.0f };


	// メッシュの描画用変数初期化
	mesh = nullptr;
	adjacencyBuffer = nullptr;
	materialBuffer = nullptr;
	numMaterials = 0;

	m_pMesh1 = nullptr;
	m_pMaterials = nullptr;
	m_pTextures1 = nullptr;
	m_dwNumMaterials = 0;
	m_pEffect = nullptr;

	// シェーダー変数初期化
	m_pEffect = nullptr;
	//m_pShadowMap = nullptr;
	//m_pShadowSurf = nullptr;
	//m_pMainBackBuffer = nullptr;
	//m_pShadowDepth = nullptr;
	//m_pMainDepthBuffer = nullptr;
	//m_pShadowDepthBuffer = nullptr;

}
CObjectX::CObjectX(int nPrioroty) :CObject(nPrioroty)
{
	// 変数の初期化
	m_pMesh = nullptr;
	m_pBuffMat = nullptr;
	m_dwNumMat = 0;
	m_pVtxBuff = nullptr;				// 頂点バッファ
	for (int i = 0; i < MAX_TEX_X; i++)
	{
		m_pTexture[i] = nullptr;			// テクスチャ
	}
	m_pos = { 0.0f, 0.0f, 0.0f };		// 位置
	m_rot = { 0.0f, 0.0f, 0.0f };		// 向き
	//m_move = { 0.0f, 0.0f, 0.0f };		//移動量
	m_size = { 0.0f,0.0f,0.0f };
	m_nMaxVtx = { 0.0f,0.0f,0.0f };
	m_nMinVtx = { 0.0f,0.0f,0.0f };

	m_pMesh1 = nullptr;
	m_pMaterials = nullptr;
	m_pTextures1 = nullptr;
	m_dwNumMaterials = 0;
	m_pEffect = nullptr;
	m_pMaterialBuffer = nullptr;

	// シェーダー変数初期化
	m_pEffect = nullptr;
	//m_pShadowMap = nullptr;
	//m_pShadowSurf = nullptr;
	//m_pMainBackBuffer = nullptr;
	//m_pShadowDepth = nullptr;
	//m_pMainDepthBuffer = nullptr;
	//m_pShadowDepthBuffer = nullptr;

}

//==================================================
//デストラクタ
//==================================================
CObjectX::~CObjectX()
{
}
//==================================================
//初期設定
//==================================================
HRESULT CObjectX::Init()
{
	CManager* pManager = CManager::GetInstance();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	VERTEX_3D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	

	return S_OK;


}

//===============================================
//モデルの頂点情報を元にサイズの最大値最小値を求める
//===============================================
void CObjectX::SizeVtx()
{
	//**========================メッシュの情報を取得================**

	if (m_pMesh != nullptr)
	{
		//頂点数の取得
		m_nNumVtx = m_pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		m_sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		//頂点バッファのロック
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_bpVtxBuff);

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
		{
			//頂点座標を代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_bpVtxBuff;

			//頂点座標を比較してモデルの最大値、最小値を取得
			//xの最大値
			if (vtx.x > m_nMaxVtx.x)
			{
				m_nMaxVtx.x = vtx.x;
			}
			//xの最小値
			else if (vtx.x < m_nMinVtx.x)
			{
				m_nMinVtx.x = vtx.x;
			}
			//yの最大値
			if (vtx.y > m_nMaxVtx.y)
			{
				m_nMaxVtx.y = vtx.y;
			}
			//yの最小値
			else if (vtx.y < m_nMinVtx.y)
			{
				m_nMinVtx.y = vtx.y;
			}
			//zの最大値
			if (vtx.z > m_nMaxVtx.z)
			{
				m_nMaxVtx.z = vtx.z;
			}
			//zの最小値
			else if (vtx.z < m_nMinVtx.z)
			{
				m_nMinVtx.z = vtx.z;
			}

			//頂点フォーマットのサイズ分ポインタを進める
			m_bpVtxBuff += m_sizeFVF;
		}
		//頂点バッファのアンロック
		m_pMesh->UnlockVertexBuffer();

		//**=========================================================**
		m_size = m_nMaxVtx - m_nMinVtx;
	}
}

//==================================================
//終了処理
//==================================================
void CObjectX::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	for (int i = 0; i < MAX_TEX_X; i++)
	{
		// テクスチャの破棄
		if (m_pTexture[i] != nullptr)
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = nullptr;
		}
	}

	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	if (m_pMaterialBuffer)
	{
		m_pMaterialBuffer->Release();
		m_pMaterialBuffer = nullptr;
	}

	if (m_pEffect) {
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
	if (m_pToonMap) {
		m_pToonMap->Release();
		m_pToonMap = nullptr;
	}

}
//==================================================
//更新処理
//==================================================
void CObjectX::Update()
{
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();		// デバイスを取得


}
//==================================================
//描画処理
//==================================================
void CObjectX::Draw()
{
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtxRot, mtxTrans;											// 計算用マトリックス
	D3DMATERIAL9 matDef;				// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;					// マテリアルデータへのポインタ


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//※ブロック以外の向きや位置の反映↓
	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//==================================================================================
	// オブジェクトの描画[blockX]
	//==================================================================================
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// モデルの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルに戻す
	pDevice->SetMaterial(&matDef);

}
bool CObjectX::LoadEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
	m_fxFilePath = fxFile; // パス保存

	LPD3DXBUFFER pErr = nullptr;
#ifdef _DEBUG
	// デバック時
	HRESULT hr = D3DXCreateEffectFromFileA(
		device, fxFile, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &pErr);
#else	
	// リリース時
	HRESULT hr = D3DXCreateEffectFromFileA(
		device, fxFile, nullptr, nullptr, D3DXSHADER_OPTIMIZATION_LEVEL3, nullptr, &m_pEffect, &pErr);
#endif	

	// 別プロジェクトで使うやつ
	//HRESULT hr = D3DXCreateEffectFromFileA(
	//	device,
	//	"Shader/MyShader.fxo", // .fx ではなく .fxo を指定
	//	nullptr,
	//	nullptr,
	//	D3DXFX_NOT_CLONEABLE,  // バイナリから読み込むときのフラグ
	//	nullptr,
	//	&m_pEffect,
	//	&pErr);

	if (FAILED(hr))
	{
		if (pErr)
		{
			MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
			pErr->Release();
		}
		return false;
	}

	return true;
}
//================================================
// fxoファイル読み込み(手動コンパイルをしろ！)
//================================================
bool CObjectX::LoadEffect_fxo(LPDIRECT3DDEVICE9 device, const char* fxoFile)
{
	m_fxFilePath = fxoFile; // パス保存（拡張子 .fxo）

	// .fxo バイナリファイルを読み込む
	std::ifstream ifs(fxoFile, std::ios::binary | std::ios::ate);
	if (!ifs.is_open()) {
		MessageBoxA(nullptr, "FXO ファイルの読み込みに失敗しました", "Error", MB_OK);
		return false;
	}

	std::streamsize size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<char> buffer(static_cast<size_t>(size));
	if (!ifs.read(buffer.data(), size)) {
		MessageBoxA(nullptr, "FXO バッファ読み込みに失敗しました", "Error", MB_OK);
		return false;
	}

	HRESULT hr = D3DXCreateEffect(
		device,
		buffer.data(),
		static_cast<UINT>(buffer.size()),
		nullptr,
		nullptr,
#ifdef _DEBUG
		D3DXSHADER_DEBUG,
#else
		D3DXSHADER_OPTIMIZATION_LEVEL3,
#endif
		nullptr,
		&m_pEffect,
		nullptr
	);

	if (FAILED(hr) || m_pEffect == nullptr) {
		MessageBoxA(nullptr, "D3DXCreateEffect に失敗しました", "FX Error", MB_OK);
		return false;
	}

	// トゥーンマップ読み込み
	if (FAILED(D3DXCreateTextureFromFileA(device, "data/TEXTURE/ToonMap_Test.png", &m_pToonMap))) {
		MessageBoxA(nullptr, "ToonMap.png の読み込みに失敗しました", "Error", MB_OK);
		return false;
	}

	return true;
}

void CObjectX::LoadXModel(LPDIRECT3DDEVICE9 device, const char* filename)
{
	// 隣接情報を受け取るバッファ
	LPD3DXBUFFER pAdjacency = nullptr;

	// マテリアルバッファが存在する場合は解放）
	if (m_pBuffMat) {
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	// Xファイルからメッシュを読み込む
	HRESULT hr = D3DXLoadMeshFromX(
		filename,             // 読み込むXファイル名
		D3DXMESH_MANAGED,     // メッシュのメモリ管理方法
		device,               // Direct3Dデバイス
		&pAdjacency,          // 隣接情報
		&m_pBuffMat,          // マテリアル情報バッファ）
		nullptr,              // エフェクト情報
		&m_dwNumMat,          // 読み込まれたマテリアル数
		&m_pMesh              // メッシュオブジェクト
	);

	// メッシュの読み込みに失敗した場合、エラーメッセージを出して終了
	if (FAILED(hr)) {
		MessageBoxA(nullptr, "モデル読み込み失敗", "Error", MB_OK);
		return;
	}

	// テクスチャ数の上限を超えないように制限
	if (m_dwNumMat > MAX_TEX_X) m_dwNumMat = MAX_TEX_X;

	// 読み込んだマテリアルバッファをD3DXMATERIAL構造体の配列として扱う
	D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 独自のマテリアル配列にコピー用の配列を確保
	m_pMaterials = new D3DMATERIAL9[m_dwNumMat];

	// テクスチャ配列のポインタを事前にすべてnullptrで初期化
	for (DWORD i = 0; i < MAX_TEX_X; i++) {
		m_pTexture[i] = nullptr;
	}

	// マテリアルとテクスチャの設定
	for (DWORD i = 0; i < m_dwNumMat; i++) {
		m_pMaterials[i] = pMaterials[i].MatD3D;               // マテリアルのコピー
		m_pMaterials[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_pMaterials[i].Ambient = m_pMaterials[i].Diffuse;    // 環境光を拡散光と同じに設定

		// テクスチャファイル名が設定されている場合は読み込む
		if (pMaterials[i].pTextureFilename) {
			HRESULT texHr = D3DXCreateTextureFromFileA(
				device,
				pMaterials[i].pTextureFilename,
				&m_pTexture[i]
			);
			// テクスチャの読み込みに失敗した場合はエラー表示
			if (FAILED(texHr)) {
				char buf[256];
				sprintf_s(buf, "テクスチャ読み込み失敗: %s", pMaterials[i].pTextureFilename);
				MessageBoxA(NULL, buf, "ERROR", MB_OK);
			}
		}
	}
	// 法線が無い場合に備えて、再計算しておく
	if (!(m_pMesh->GetFVF() & D3DFVF_NORMAL)) {
		HRESULT normHr = m_pMesh->CloneMeshFVF(
			m_pMesh->GetOptions(),
			m_pMesh->GetFVF() | D3DFVF_NORMAL,
			device,
			&m_pMesh
		);
		if (SUCCEEDED(normHr)) {
			D3DXComputeNormals(m_pMesh, nullptr);
		}
	}
	// 隣接情報バッファの解放
	if (pAdjacency) pAdjacency->Release();
}
//==================================================
//オブジェクトの生成(xファイルの)
//==================================================
CObjectX* CObjectX::Create()
{
	CObjectX* pObjectX = new CObjectX;
	pObjectX->Init();
	return pObjectX;

}
//==================================================
//モデルの設定(引数にパスを書く)
//==================================================
void CObjectX::SetModel(std::string sPath)
{
	CManager* pManager = CManager::GetInstance();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	// モデルの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	// Xファイルの読み込み
	D3DXLoadMeshFromX(sPath.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);


	//SizeVtx();	// pMeshの情報を取得
}
//======================================
//位置の設定
//======================================
void CObjectX::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//======================================
//向きの設定
//======================================
void CObjectX::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//======================================
//サイズの設定
//======================================
void CObjectX::SetSize(D3DXVECTOR3 size)
{
	m_size =size;
}

