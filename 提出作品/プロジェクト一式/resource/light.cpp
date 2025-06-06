//=================================
// 
//ライトクラス　　light.cpp
//Auther kida ibuki 
// 
//==================================
#include "light.h"   // ライト
#include "manager.h"  // マネージャー

//==============================
// コンストラクタ
//==============================
CLight::CLight()
{
	ZeroMemory(&light, sizeof(light));

	vecX = 0.0f;
	vecY = -1.0f;
	vecZ = 1.0f;

	//position = { 0.0f,10.0f,0.0f };
	//m_directionalLight = nullptr;
	//m_mesh = nullptr;

}
//==============================
// デストラクタ
//==============================
CLight::~CLight()
{
}
//==============================
// 初期設定
//==============================
HRESULT CLight::Init()
{
#if 1
	CManager* pManager = CManager::GetInstance();

	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir;		//設定用方向ベクトル

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   // 拡散光
	light.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);  // 鏡面反射
	light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);   // 環境光
	D3DXVECTOR3 dir = D3DXVECTOR3(vecX, vecY, vecZ);    // 光の方向
	D3DXVec3Normalize(&dir, &dir);
	light.Direction = dir;    // 光の方向
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	return S_OK;

#endif
}
//==============================
// 終了処理
//==============================
void CLight::Uninit()
{
	if (m_mesh) {
		m_mesh->Release();
		m_mesh = nullptr;
	}
}
//==============================
// 更新処理
//==============================
void CLight::Update()
{
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();


	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin(u8"light Dir"))
	{
		ImGui::Text("LightDir Move");               // Display some text (you can use a format strings too)
		ImGui::SliderFloat("lightDir X", &vecX, -10.0f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("lightDir Y", &vecY, -10.0f, 30.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("lightDir Z", &vecZ, -10.0f, 20.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		ImGui::End();
	}
	D3DXVECTOR3 dir = D3DXVECTOR3(vecX, vecY, vecZ);    // 光の方向
	D3DXVec3Normalize(&dir, &dir);
	light.Direction = dir;    // 光の方向
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//==========================================
// ライトの描画
//==========================================
void CLight::Draw()
{
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	m_directionalLight->DrawObjectWithLighting(pDevice, m_mesh, position);
}


//==========================================
// コンストラクト
//==========================================
Light::Light()
{
	ZeroMemory(&m_light, sizeof(m_light));

	anglef = 0.0f;
	radius = 30.0f;
	speed = 1.0f;
	//center = { 0, 0, 0 };

}

//==========================================
// デストラクタ
//==========================================
Light::~Light()
{
}
//==========================================
// 指向性ライト
//==========================================
void Light::InitDirectional(const D3DXVECTOR3& direction, const D3DXCOLOR& color)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Diffuse = color;   // 色
	m_light.Direction = direction;   // 方向
}
//==========================================
// ポイントライト
//==========================================
void Light::InitPoint(const D3DXVECTOR3& position, const D3DXCOLOR& color, float range)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = D3DLIGHT_POINT;
	m_light.Diffuse = color;   // 色
	m_light.Position = position;  // 位置
	m_light.Range = range;   // 範囲
	m_light.Attenuation0 = 1.0f;  // 距離減衰を有効にする

}
//==========================================
// スポットライト
//==========================================
void Light::InitSpot(const D3DXVECTOR3& position, const D3DXVECTOR3& direction, const D3DXCOLOR& color, float range, float theta, float phi)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = D3DLIGHT_SPOT;
	m_light.Diffuse = color;   // 色
	m_light.Position = position;   // 位置
	m_light.Direction = direction;   // 方向
	m_light.Range = range;    // 範囲
	m_light.Theta = theta;  // シータ
	m_light.Phi = phi;  // ファイ
	m_light.Falloff = 1.0f;
	m_light.Attenuation0 = 1.0f;  // 距離減衰
}
//==========================================
// ライトの有効化
//==========================================
void Light::Enable(LPDIRECT3DDEVICE9 device, DWORD index)
{
	device->SetLight(index, &m_light);
	device->LightEnable(index, TRUE);
}
//==========================================
// ライトの無効化
//==========================================
void Light::Disable(LPDIRECT3DDEVICE9 device, DWORD index)
{
	device->LightEnable(index, FALSE);
}

//==========================================
// ライトの位置設定
//==========================================
void Light::SetPosition(const D3DXVECTOR3& pos)
{
	m_light.Position = pos;
}
//==========================================
// ライトの方向設定
//==========================================
void Light::SetDirection(const D3DXVECTOR3& dir)
{
	m_light.Direction = dir;
}
void Light::UpdateObj(float deltaTime, D3DXVECTOR3 center)
{
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();


	anglef += speed * deltaTime;
	if (anglef > D3DX_PI * 2) {
		anglef -= D3DX_PI * 2; // 角度をループ
	}
	float x = center.x + radius * cosf(anglef);
	float z = center.z + radius * sinf(anglef);
	float y = 10.0f; // 高さを固定するなら

	D3DXVECTOR3 pos = D3DXVECTOR3(x, y, z);

	// ワールド行列の作成
	D3DXMatrixTranslation(&world, pos.x, pos.y, pos.z);
	pDevice->SetTransform(D3DTS_WORLD, &world);
	D3DXVECTOR3 lightPos = D3DXVECTOR3(world._41, world._42, world._43);  // 平行光源以外
	m_light.Position = lightPos;

}

//==========================================
// ライトオブジェクト描画
//==========================================
void Light::DrawObjectWithLighting(LPDIRECT3DDEVICE9 device, LPD3DXMESH mesh, const D3DXVECTOR3& position)
{
	//D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	device->SetTransform(D3DTS_WORLD, &world);
	D3DXVECTOR3 lightPos = D3DXVECTOR3(world._41, world._42, world._43);  // 平行光源以外
	m_light.Position = lightPos;

	// ライティング有効
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	//device->SetLight(0, &m_light);
	//device->LightEnable(0, TRUE);

	// マテリアル設定（色が付いてないと光が見えない）
	D3DMATERIAL9 mtrl = {};
	mtrl.Diffuse = D3DXCOLOR(1, 1, 1, 1);
	mtrl.Ambient = D3DXCOLOR(1, 1, 1, 1);
	device->SetMaterial(&mtrl);

	//// カリング解除（裏面も描画）
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (mesh) {
		// 描画
		mesh->DrawSubset(0);
	}
}