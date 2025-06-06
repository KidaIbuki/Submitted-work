//================================
//
// フレネル効果付きの波シェーダー(WaterSurface.cpp)
// Auther kida ibuki
// 
//================================

#include "WaterSurface.h"
#include "manager.h"

//=====================================
// コンストラクタ
//=====================================
Wave::Wave()
{
	m_pWaterEffect = nullptr;  // エフェクト
	m_pWaterMesh = nullptr;  // メッシュ
	m_pEnvMap = nullptr;  // キューブマップ
	m_time = 0.0f;  // アニメーションタイム
	m_animPow = 0.05f;  // アニメーションの強さ
	for (int i = 0; i < NUM_TEX; i++) {
		m_normalMap[i] = nullptr;  // 複数テクスチャ
	}
	m_bChange = true;  // ImGuiのon off
	m_currentTextureIndex = 0;  // テクスチャのインデックス
	m_pReflectionTex = nullptr;
	m_pReflectionSurf = nullptr;
}
//=====================================
// デストラクタ
//=====================================
Wave::~Wave()
{
}
//=====================================
// 初期化処理
//=====================================
HRESULT Wave::Init()
{
	CManager* pManager = CManager::GetInstance();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	CObjectX::Init();

	InitWaterEffect(pDevice, "shader/Wave.fx");

	return S_OK;
}
//=====================================
// 終了処理
//=====================================
void Wave::Uninit()
{
	CObjectX::Uninit();

	ShutdownWaterEffect();
}
//=====================================
// 更新処理
//=====================================
void Wave::Update()
{
	CManager* pManager = CManager::GetInstance();

	D3DXVECTOR3 cameraPos = pManager->GetCamera()->GetPosV();
	D3DXVECTOR3 cameraTarget = pManager->GetCamera()->GetPosR();
	//CObject3D::Update();
// ImGuiのやつ
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin(u8"Wave Change"))
	{
		ImGui::Text("NormalMap");
		// テクスチャ選択ボタンを ImGui 上に配置
		if (ImGui::Button("Texture 1")) m_currentTextureIndex = 0;
		ImGui::SameLine();
		if (ImGui::Button("Texture 2")) m_currentTextureIndex = 1;
		ImGui::SameLine();
		if (ImGui::Button("Texture 3")) m_currentTextureIndex = 2;

		// テクスチャの表示
		if (m_normalMap[m_currentTextureIndex]) {
			ImTextureID texID = (ImTextureID)m_normalMap[m_currentTextureIndex];
			ImGui::Image(texID, ImVec2(128, 128)); // サイズは適宜変更
		}

		ImGui::Text("Animation");
		ImGui::SliderFloat("AnimPowor", &m_animPow, 0.0f, 0.1f);
		ImGui::End();
	}

	UpdateWaterEffect(m_animPow, cameraPos);
}
//=====================================
// 描画処理
//=====================================
void Wave::Draw()
{


	CManager* pManager = CManager::GetInstance();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	D3DXMATRIX mtxViewProj = pManager->GetCamera()->GetViewProj();
	D3DXMATRIX mtxView = pManager->GetCamera()->GetView();
	D3DXMATRIX mtxProj = pManager->GetCamera()->GetProj();

	RenderReflectionTexture(pDevice);
	//CObjectX::Draw();
	RenderWaterEffect(pDevice, mtxViewProj);

}
//=====================================
// 生成処理
//=====================================
Wave* Wave::Create()
{
	Wave* pWave = new Wave();
	pWave->Init();
	return pWave;
}
//=====================================
// シェーダーの初期化
//=====================================
bool Wave::InitWaterEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
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

	if (FAILED(hr))
	{
		if (pErr)
		{
			MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
			pErr->Release();
		}
		return false;
	}

	LoadXModel(device, "data/MODEL/block00.x");  // これを変えると水面も変わるぜ

	// ノーマルマップの読み込み
	if (FAILED(D3DXCreateTextureFromFile(device, "data/TEXTURE/water_normalMap.png", &m_normalMap[0]))) {
		MessageBox(nullptr, "ノーマルマップ読み込み失敗", "Error", MB_OK);
		return false;
	}
	// ノーマルマップの読み込み2
	if (FAILED(D3DXCreateTextureFromFile(device, "data/TEXTURE/water_normalMap01.png", &m_normalMap[1]))) {
		MessageBox(nullptr, "ノーマルマップ読み込み失敗", "Error", MB_OK);
		return false;
	}
	// ノーマルマップの読み込み3
	if (FAILED(D3DXCreateTextureFromFile(device, "data/TEXTURE/water_normalMap04.png", &m_normalMap[2]))) {
		MessageBox(nullptr, "ノーマルマップ読み込み失敗", "Error", MB_OK);
		return false;
	}

	// キューブ環境マップの読み込み
	if (FAILED(D3DXCreateCubeTextureFromFile(device, "data/TEXTURE/WaterCubeMap01.dds", &m_pEnvMap))) {
		MessageBox(nullptr, "キューブマップ読み込み失敗", "Error", MB_OK);
		return false;
	}

	device->CreateTexture(300, 300, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pReflectionTex, nullptr);
	m_pReflectionTex->GetSurfaceLevel(0, &m_pReflectionSurf);

	// 水面メッシュ（平面）
	D3DXCreatePolygon(device, 100, 1, &m_pMesh, nullptr);

	return true;
}
//=====================================
// シェーダーの破棄
//=====================================
void Wave::ShutdownWaterEffect()
{
	if (m_pEnvMap) {
		m_pEnvMap->Release();
		m_pEnvMap = nullptr;
	}
	for (int i = 0; i < NUM_TEX; i++) {
		if (m_normalMap[i]) {
			m_normalMap[i]->Release();
			m_normalMap[i] = nullptr;
		}
	}

	if (m_pWaterEffect) {
		m_pWaterEffect->Release();
		m_pWaterEffect = nullptr;
	}
}
//=====================================
// シェーダーの更新
//=====================================
void Wave::UpdateWaterEffect(float deltaTime, D3DXVECTOR3 camPos)
{
	CManager* pManager = CManager::GetInstance();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	m_time += deltaTime;

	m_pEffect->SetFloat("time", m_time);

	m_pEffect->SetFloat("waveStrength", 0.05f);
	m_pEffect->SetValue("cameraPos", &camPos, sizeof(D3DXVECTOR3));

}
//=====================================
// シェーダーの描画
//=====================================
void Wave::RenderWaterEffect(LPDIRECT3DDEVICE9 device, const D3DXMATRIX& viewProj)
{
	m_pEffect->SetMatrix("worldViewProj", &viewProj);

	// ノーマルマップの設定
	m_pEffect->SetTexture("NormalMapTex", m_normalMap[m_currentTextureIndex]);

	m_pEffect->SetTexture("ReflectionTex", m_pReflectionTex);


	m_pEffect->SetTexture("EnvMapTex", m_pEnvMap);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);        // アルファブレンドON
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);   // ソースのブレンド係数
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // デスティネーションのブレンド係数

	UINT passCount = 0;
	m_pEffect->Begin(&passCount, 0);
	for (UINT i = 0; i < passCount; ++i)
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

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//=====================================
// 反射の描画
//=====================================
void Wave::RenderSceneForReflection(LPDIRECT3DDEVICE9 device)
{
	CManager* pManager = CManager::GetInstance();

	D3DXVECTOR3 cameraPos = pManager->GetCamera()->GetPosV();
	D3DXVECTOR3 cameraTarget = pManager->GetCamera()->GetPosR();
	D3DXMATRIX mtxView = pManager->GetCamera()->GetProj();
	// 水面の高さ
	float waterHeight = 0.0f;

	// ビュー行列を取得して反転ビュー行列を作成
	D3DXMATRIX viewMatrix, reflectedView;
	D3DXVECTOR3 eyePos, lookAt, upVec;

	// 現在のビュー行列のパラメータ
	eyePos = cameraPos;   // カメラ位置
	lookAt = cameraTarget; // 注視点
	upVec = D3DXVECTOR3(0, 1, 0); // 上方向

	// Y軸反転
	eyePos.y = waterHeight - (eyePos.y - waterHeight);
	lookAt.y = waterHeight - (lookAt.y - waterHeight);

	D3DXMatrixLookAtLH(&reflectedView, &eyePos, &lookAt, &upVec);

	// プロジェクション行列
	D3DXMATRIX projMatrix = mtxView;

	// ワールド・ビュー・プロジェクション合成行列を作成
	D3DXMATRIX viewProj = reflectedView * projMatrix;

	// 各オブジェクトの描画
	CObjectX::Draw();
}
//=====================================
// 反射テクスチャの描画
//=====================================
void Wave::RenderReflectionTexture(LPDIRECT3DDEVICE9 device)
{
	// バックバッファ保存
	LPDIRECT3DSURFACE9 pOldRT = nullptr;
	device->GetRenderTarget(0, &pOldRT);

	// 反射サーフェスへ切り替え
	device->SetRenderTarget(0, m_pReflectionSurf);
	device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	RenderSceneForReflection(device);
	// 戻す
	device->SetRenderTarget(0, pOldRT);
	if (pOldRT) {
		pOldRT->Release();
		pOldRT = nullptr;
	}
}

