//================================
//
//xファイルでブロック(blockX.h)
//Auther kida ibuki
//================================
#include "blockX.h"
#include "manager.h"

//======================================
//コンストラクタ
//======================================
CBlockX::CBlockX()
{
	m_DiffuseColor = D3DXVECTOR4(1, 1, 1, 1);
	m_SpecularColor = D3DXVECTOR4(1, 1, 1, 1);
	m_Shininess = 32.0f;

	m_LightDir = D3DXVECTOR3(0, -1, -1);
	m_CameraPos = D3DXVECTOR3(0, 5, -10);
	m_RimPowor = 0.0f;
	m_RimIntensity = 0.0f;

	vecX = 0.0f;
	vecY = 0.0f;
	vecZ = 0.0f;

}
//======================================
//デストラクタ
//======================================
CBlockX::~CBlockX()
{
}
//======================================
//初期設定
//======================================
HRESULT CBlockX::Init()
{
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}
	CManager* pManager = CManager::GetInstance();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	// 種類をブロックに設定
	SetType(CObject::TYPE::BLOCK);

	return S_OK;
}
//======================================
//終了処理
//======================================
void CBlockX::Uninit()
{
	CObjectX::Uninit();
}
//======================================
//更新処理
//======================================
void CBlockX::Update()
{
	CManager* pManager = CManager::GetInstance();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();
	D3DXMATRIX mtxViewProj = pManager->GetCamera()->GetViewProj();
	D3DXMATRIX mtxView = pManager->GetCamera()->GetView();
	D3DXMATRIX mtxProj = pManager->GetCamera()->GetProj();
	D3DXMATRIX worldViewProj = GetWorldMatrix();

	// カメラ情報を取得（自作カメラクラス）
	D3DXVECTOR3 cameraPos = pManager->GetCamera()->GetPosV();
	UpdateToonShader(pDevice);  // トゥーンシェーダー
	//移動量の更新
	m_move += Move();

	//posを動かす
	m_pos += m_move;



	m_pos.y += m_move.y;

	//移動量を減衰
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;
	m_move.y += (0.0f - m_move.y) * 0.1f;

}
//======================================
//描画処理
//======================================
void CBlockX::Draw()
{
	CManager* pManager = CManager::GetInstance();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld, mtxParent;	// 計算用マトリクス	
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;						// マテリアルデータへのポインタ

	D3DXMATRIX mtxViewProj = pManager->GetCamera()->GetViewProj();
	D3DXMATRIX mtxView = pManager->GetCamera()->GetView();
	D3DXMATRIX mtxProj = pManager->GetCamera()->GetProj();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//※ブロック以外の向きや位置の反映↓
	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);  // シェーダーの描画この上か下のどちらかかも※これ消したらおかしくなるぜ

	RenderToonShader(pDevice);  // トゥーンシェーダー

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
//======================================
//ブロックの生成
//======================================
CBlockX* CBlockX::Create(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CBlockX* pBlockX = new CBlockX();
	
	pBlockX->SetPos(pos);
	pBlockX->SetRot(rot);

	pBlockX->Init();
	//pBlockX->LoadEffect(device, "shader/ToonShader2.fx");
	pBlockX->LoadEffect(device, "shader/ToonShader.fxo");   // fxo読み込み(外部変更できない)


	pBlockX->LoadXModel(device, "data/MODEL/ballD.x");


	return pBlockX;
}
//=====================================
// モデルの移動
//=====================================
D3DXVECTOR3 CBlockX::Move()
{
	CManager* pManager = CManager::GetInstance();  //マネージャーのインスタンス取得

	CInputKeyboard* pKeyboard = pManager->GetKeyboard();    //キーボード取得


	D3DXVECTOR3 move = { 0,0,0 };
	//**========カメラの情報==========**

	if (pKeyboard->GetPress(DIK_A))
	{//Aキーが押された場合
		move.x -= 0.2f;

	}
	if (pKeyboard->GetPress(DIK_D))
	{//Dキーが押された場合

		move.x += 0.2f;
	}
	if (pKeyboard->GetPress(DIK_W))
	{//Aキーが押された場合
		move.z += 0.2f;

	}
	if (pKeyboard->GetPress(DIK_S))
	{//Dキーが押された場合

		move.z -= 0.2f;
	}
	if (pKeyboard->GetPress(DIK_N))
	{//Dキーが押された場合

		move.y += 0.2f;
	}
	if (pKeyboard->GetPress(DIK_M))
	{//Dキーが押された場合

		move.y -= 0.2f;
	}
	if (pKeyboard->GetPress(DIK_B))
	{//Dキーが押された場合

		m_rot.y -= 0.02f;
	}
	if (pKeyboard->GetPress(DIK_V))
	{//Dキーが押された場合

		m_rot.x -= 0.02f;
	}
	if (pKeyboard->GetPress(DIK_Q))
	{//Dキーが押された場合

		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}


	return move;
}
//=====================================
// シェーダーの更新
//=====================================
void CBlockX::UpdateToonShader(LPDIRECT3DDEVICE9 device)
{
	CManager* pManager = CManager::GetInstance();  //マネージャーのインスタンス取得

	D3DXMATRIX world, view, proj, wvp;
	world = GetWorldMatrix();
	// ワールド行列：回転など加える
	//D3DXMatrixRotationY(&world, timeGetTime() / 1000.0f);
	D3DXMATRIX rotY;
	D3DXMatrixRotationY(&rotY, timeGetTime() / 1000.0f);
	world = rotY * world; // 回転を適用

	// ビュー行列
	D3DXVECTOR3 eye(0, 2, -5), at(0, 0, 0), up(0, 1, 0);
	D3DXMatrixLookAtLH(&view, &eye, &at, &up);

	// 射影行列
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, 800.0f / 600.0f, 0.1f, 100.0f);

	// ワールドビュー射影合成
	wvp = world * view * proj;

	// シェーダへ設定
	m_pEffect->SetMatrix("World", &world);
	m_pEffect->SetMatrix("View", &view);
	m_pEffect->SetMatrix("Projection", &proj);
	m_pEffect->SetMatrix("WorldViewProj", &wvp);

	// ライトの向き
	D3DXVECTOR3 lightDir = pManager->GetLight()->GetLightDir();

	// 正規化（必要なら）
	D3DXVec3Normalize(&lightDir, &lightDir);

	// エフェクトに渡す（gLightDir は fx ファイルの float3 名と一致させる）
	m_pEffect->SetVector("LightDirection", (D3DXVECTOR4*)&lightDir);

	// 法線変換用の逆転置行列を作成
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, NULL, &world);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	m_pEffect->SetMatrix("gWorldInverseTranspose", &worldInvTrans);
}
//=====================================
// シェーダー描画
//=====================================
void CBlockX::RenderToonShader(LPDIRECT3DDEVICE9 device)
{
	CManager* pManager = CManager::GetInstance();  //マネージャーのインスタンス取得

	D3DXMATRIX view, proj, wvp;
	D3DXMATRIX mtxWorld = GetWorldMatrix();
	D3DXMATRIX matWorldNoScale, matWorldInvTrans;

	device->GetTransform(D3DTS_VIEW, &view);
	device->GetTransform(D3DTS_PROJECTION, &proj);
	wvp = mtxWorld * view * proj;

	// エフェクトに変数を渡す(この4つの位置を変えるとおかしくなるよ)
	m_pEffect->SetMatrix("World", &mtxWorld);
	m_pEffect->SetMatrix("View", &view);
	m_pEffect->SetMatrix("Projection", &proj);
	m_pEffect->SetMatrix("WorldViewProj", &wvp);

	// エフェクト設定用スケール・回転・平行移動
	D3DXVECTOR3 scale, translation;
	D3DXQUATERNION rotation;

	// モデル行列（World）
	D3DXMatrixIdentity(&mtxWorld); // 回転・移動が入る  ※これ消すと黒いところ出る

	D3DXVECTOR3 eye(0, 2, -5), at(0, 0, 0), up(0, 1, 0);

	// ビュー・プロジェクション行列（固定 or カメラから生成）
	D3DXMatrixLookAtLH(&view,
		&eye,  // Eye
		&at,   // LookAt
		&up);  // Up

	// 射影行列
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, 640.0f / 480.0f, 1.0f, 100.0f);

	D3DXVECTOR3 Scaling = D3DXVECTOR3(1, 1, 1);  // スケール

	D3DXMatrixDecompose(&scale, &rotation, &translation, &mtxWorld);
	D3DXMatrixTransformation(&matWorldNoScale, nullptr, nullptr, &Scaling, nullptr, &rotation, &translation);

	// 法線変換用：Worldのスケーリングを除外して逆転置行列を作成
	D3DXMatrixInverse(&matWorldInvTrans, NULL, &mtxWorld);
	D3DXMatrixTranspose(&matWorldInvTrans, &matWorldInvTrans);

	m_pEffect->SetMatrix("gWorldInverseTranspose", &matWorldInvTrans);  // 除外したスケーリングをエフェクトに入れる


		// ライトの向き
	D3DXVECTOR3 lightDir = pManager->GetLight()->GetLightDir();

	// 正規化（必要なら）
	D3DXVec3Normalize(&lightDir, &lightDir);

	// エフェクトに渡す（gLightDir は fx ファイルの float3 名と一致させる）
	m_pEffect->SetVector("LightDirection", (D3DXVECTOR4*)&lightDir);

	UINT numPasses = 0;

	// --- アウトライン描画（先に） ---
	m_pEffect->SetTechnique("Outline");
	m_pEffect->Begin(&numPasses, 0);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		for (int n = 0; n < m_dwNumMat; ++n)
		{
			m_pEffect->CommitChanges();
			m_pMesh->DrawSubset(n);
		}
		m_pEffect->EndPass();
	}
	m_pEffect->End();

	// --- トゥーン本体描画（後に） ---
	m_pEffect->SetTechnique("ToonTechnique"); 
	m_pEffect->Begin(&numPasses, 0);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		for (int n = 0; n < m_dwNumMat; ++n)
		{
			//m_pEffect->SetTexture("DiffuseTexture", m_pTexture[n]);
			m_pEffect->SetTexture("DiffuseTexture", m_pToonMap);   // どっちかがいいと思う

			m_pEffect->CommitChanges();
			m_pMesh->DrawSubset(n);
		}
		m_pEffect->EndPass();
	}
	m_pEffect->End();

}
