//=================================
// 
//���C�g�N���X�@�@light.cpp
//Auther kida ibuki 
// 
//==================================
#include "light.h"   // ���C�g
#include "manager.h"  // �}�l�[�W���[

//==============================
// �R���X�g���N�^
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
// �f�X�g���N�^
//==============================
CLight::~CLight()
{
}
//==============================
// �����ݒ�
//==============================
HRESULT CLight::Init()
{
#if 1
	CManager* pManager = CManager::GetInstance();

	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir;		//�ݒ�p�����x�N�g��

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);   // �g�U��
	light.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);  // ���ʔ���
	light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);   // ����
	D3DXVECTOR3 dir = D3DXVECTOR3(vecX, vecY, vecZ);    // ���̕���
	D3DXVec3Normalize(&dir, &dir);
	light.Direction = dir;    // ���̕���
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);


	return S_OK;

#endif
}
//==============================
// �I������
//==============================
void CLight::Uninit()
{
	if (m_mesh) {
		m_mesh->Release();
		m_mesh = nullptr;
	}
}
//==============================
// �X�V����
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
	D3DXVECTOR3 dir = D3DXVECTOR3(vecX, vecY, vecZ);    // ���̕���
	D3DXVec3Normalize(&dir, &dir);
	light.Direction = dir;    // ���̕���
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//==========================================
// ���C�g�̕`��
//==========================================
void CLight::Draw()
{
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	m_directionalLight->DrawObjectWithLighting(pDevice, m_mesh, position);
}


//==========================================
// �R���X�g���N�g
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
// �f�X�g���N�^
//==========================================
Light::~Light()
{
}
//==========================================
// �w�������C�g
//==========================================
void Light::InitDirectional(const D3DXVECTOR3& direction, const D3DXCOLOR& color)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Diffuse = color;   // �F
	m_light.Direction = direction;   // ����
}
//==========================================
// �|�C���g���C�g
//==========================================
void Light::InitPoint(const D3DXVECTOR3& position, const D3DXCOLOR& color, float range)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = D3DLIGHT_POINT;
	m_light.Diffuse = color;   // �F
	m_light.Position = position;  // �ʒu
	m_light.Range = range;   // �͈�
	m_light.Attenuation0 = 1.0f;  // ����������L���ɂ���

}
//==========================================
// �X�|�b�g���C�g
//==========================================
void Light::InitSpot(const D3DXVECTOR3& position, const D3DXVECTOR3& direction, const D3DXCOLOR& color, float range, float theta, float phi)
{
	ZeroMemory(&m_light, sizeof(m_light));
	m_light.Type = D3DLIGHT_SPOT;
	m_light.Diffuse = color;   // �F
	m_light.Position = position;   // �ʒu
	m_light.Direction = direction;   // ����
	m_light.Range = range;    // �͈�
	m_light.Theta = theta;  // �V�[�^
	m_light.Phi = phi;  // �t�@�C
	m_light.Falloff = 1.0f;
	m_light.Attenuation0 = 1.0f;  // ��������
}
//==========================================
// ���C�g�̗L����
//==========================================
void Light::Enable(LPDIRECT3DDEVICE9 device, DWORD index)
{
	device->SetLight(index, &m_light);
	device->LightEnable(index, TRUE);
}
//==========================================
// ���C�g�̖�����
//==========================================
void Light::Disable(LPDIRECT3DDEVICE9 device, DWORD index)
{
	device->LightEnable(index, FALSE);
}

//==========================================
// ���C�g�̈ʒu�ݒ�
//==========================================
void Light::SetPosition(const D3DXVECTOR3& pos)
{
	m_light.Position = pos;
}
//==========================================
// ���C�g�̕����ݒ�
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
		anglef -= D3DX_PI * 2; // �p�x�����[�v
	}
	float x = center.x + radius * cosf(anglef);
	float z = center.z + radius * sinf(anglef);
	float y = 10.0f; // �������Œ肷��Ȃ�

	D3DXVECTOR3 pos = D3DXVECTOR3(x, y, z);

	// ���[���h�s��̍쐬
	D3DXMatrixTranslation(&world, pos.x, pos.y, pos.z);
	pDevice->SetTransform(D3DTS_WORLD, &world);
	D3DXVECTOR3 lightPos = D3DXVECTOR3(world._41, world._42, world._43);  // ���s�����ȊO
	m_light.Position = lightPos;

}

//==========================================
// ���C�g�I�u�W�F�N�g�`��
//==========================================
void Light::DrawObjectWithLighting(LPDIRECT3DDEVICE9 device, LPD3DXMESH mesh, const D3DXVECTOR3& position)
{
	//D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	device->SetTransform(D3DTS_WORLD, &world);
	D3DXVECTOR3 lightPos = D3DXVECTOR3(world._41, world._42, world._43);  // ���s�����ȊO
	m_light.Position = lightPos;

	// ���C�e�B���O�L��
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	//device->SetLight(0, &m_light);
	//device->LightEnable(0, TRUE);

	// �}�e���A���ݒ�i�F���t���ĂȂ��ƌ��������Ȃ��j
	D3DMATERIAL9 mtrl = {};
	mtrl.Diffuse = D3DXCOLOR(1, 1, 1, 1);
	mtrl.Ambient = D3DXCOLOR(1, 1, 1, 1);
	device->SetMaterial(&mtrl);

	//// �J�����O�����i���ʂ��`��j
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (mesh) {
		// �`��
		mesh->DrawSubset(0);
	}
}