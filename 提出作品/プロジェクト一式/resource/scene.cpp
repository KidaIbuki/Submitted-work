//=================================
// 
// シーンクラス　scene.cpp
// Auther kida ibuki 
// 
//==================================
#include "scene.h"  // シーン
#include "title.h"  // タイトル
#include "manager.h"  // マネージャー
#include "object.h"  // オブジェクト

//====================================
// コンストラクタ
//====================================
CScene::CScene()
{
	m_Mode = MODE_TITLE;
	m_pos = { 0.0f,0.0f };
	m_size = { 0.0f,0.0f };
	m_nTimerCnt = 0;
	Init();
}
//====================================
// デストラクタ
//====================================
CScene::~CScene()
{
}
//====================================
// 初期設定
//====================================
HRESULT CScene::Init()
{
	CManager* pManager = CManager::GetInstance();

	LPDIRECT3DDEVICE9 pDevice;

	pDevice = pManager->GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D* pVtx;	//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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
//====================================
// 終了処理
//====================================
void CScene::Uninit()
{
	// すべてのオブジェクトを解放する
	CObject::ReleaseAll();   

	//テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

}
//====================================
// 更新処理
//====================================
void CScene::Update()
{
	UpdateVertex();
}
//====================================
// 描画処理
//====================================
void CScene::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	CManager* pManager = CManager::GetInstance();

	pDevice = pManager->GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Zの比較方法を変更
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//Zバッファに書き込まない(奥に表示するか、手前に表示するか)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Zの比較方法変更
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
//====================================
// テクスチャの設定
//====================================
void CScene::SetTexture(std::string sPath)
{
	CManager* pManager = CManager::GetInstance();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	// テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, sPath.c_str(), &m_pTexture);

}

void CScene::UpdateVertex()
{
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	D3DXMATRIX mtx;
	D3DXMatrixIdentity(&mtx);

	D3DXVECTOR3 sizePos[4] =
	{
		{-m_size.x * VECTOR_SIZE, -m_size.y * VECTOR_SIZE, 0.0f},
		{m_size.x * VECTOR_SIZE, -m_size.y * VECTOR_SIZE, 0.0f},
		{-m_size.x * VECTOR_SIZE, m_size.y * VECTOR_SIZE, 0.0f},
		{m_size.x * VECTOR_SIZE, m_size.y * VECTOR_SIZE, 0.0f}
	};
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// サイズと回転
	D3DXVec3TransformCoord(&pVtx[0].pos, &sizePos[0], &mtx);
	D3DXVec3TransformCoord(&pVtx[1].pos, &sizePos[1], &mtx);
	D3DXVec3TransformCoord(&pVtx[2].pos, &sizePos[2], &mtx);
	D3DXVec3TransformCoord(&pVtx[3].pos, &sizePos[3], &mtx);

	// 位置
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCnt].pos.x += m_pos.x;
		pVtx[nCnt].pos.y += m_pos.y;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//====================================
// シーンの生成
//====================================
CScene* CScene::Create(MODE mode)
{
	// シーンの終了処理
	// ↓
	// シーンの変更
	// ↓
	// シーンの初期化

	CScene* pScene = nullptr;
	switch (mode)
	{
	case MODE_TITLE:
		CManager::GetInstance()->GetCamera()->ResetCamera();
		pScene = CTitle::Create();
		break;
	}
	pScene->m_Mode = mode;
	return pScene;
}
//====================================
// サイズの設定
//====================================
void CScene::SetSize(float fWidth, float fHeight)
{
	m_size = D3DXVECTOR2(fWidth, fHeight);
}
//====================================
// 位置の設定
//====================================
void CScene::SetPos(float x, float y)
{
	m_pos = D3DXVECTOR2(x, y);
}
