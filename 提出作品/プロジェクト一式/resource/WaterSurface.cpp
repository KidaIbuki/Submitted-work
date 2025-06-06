//================================
//
// �t���l�����ʕt���̔g�V�F�[�_�[(WaterSurface.cpp)
// Auther kida ibuki
// 
//================================

#include "WaterSurface.h"
#include "manager.h"

//=====================================
// �R���X�g���N�^
//=====================================
Wave::Wave()
{
	m_pWaterEffect = nullptr;  // �G�t�F�N�g
	m_pWaterMesh = nullptr;  // ���b�V��
	m_pEnvMap = nullptr;  // �L���[�u�}�b�v
	m_time = 0.0f;  // �A�j���[�V�����^�C��
	m_animPow = 0.05f;  // �A�j���[�V�����̋���
	for (int i = 0; i < NUM_TEX; i++) {
		m_normalMap[i] = nullptr;  // �����e�N�X�`��
	}
	m_bChange = true;  // ImGui��on off
	m_currentTextureIndex = 0;  // �e�N�X�`���̃C���f�b�N�X
	m_pReflectionTex = nullptr;
	m_pReflectionSurf = nullptr;
}
//=====================================
// �f�X�g���N�^
//=====================================
Wave::~Wave()
{
}
//=====================================
// ����������
//=====================================
HRESULT Wave::Init()
{
	CManager* pManager = CManager::GetInstance();
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	CObjectX::Init();

	InitWaterEffect(pDevice, "shader/Wave.fx");

	return S_OK;
}
//=====================================
// �I������
//=====================================
void Wave::Uninit()
{
	CObjectX::Uninit();

	ShutdownWaterEffect();
}
//=====================================
// �X�V����
//=====================================
void Wave::Update()
{
	CManager* pManager = CManager::GetInstance();

	D3DXVECTOR3 cameraPos = pManager->GetCamera()->GetPosV();
	D3DXVECTOR3 cameraTarget = pManager->GetCamera()->GetPosR();
	//CObject3D::Update();
// ImGui�̂��
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin(u8"Wave Change"))
	{
		ImGui::Text("NormalMap");
		// �e�N�X�`���I���{�^���� ImGui ��ɔz�u
		if (ImGui::Button("Texture 1")) m_currentTextureIndex = 0;
		ImGui::SameLine();
		if (ImGui::Button("Texture 2")) m_currentTextureIndex = 1;
		ImGui::SameLine();
		if (ImGui::Button("Texture 3")) m_currentTextureIndex = 2;

		// �e�N�X�`���̕\��
		if (m_normalMap[m_currentTextureIndex]) {
			ImTextureID texID = (ImTextureID)m_normalMap[m_currentTextureIndex];
			ImGui::Image(texID, ImVec2(128, 128)); // �T�C�Y�͓K�X�ύX
		}

		ImGui::Text("Animation");
		ImGui::SliderFloat("AnimPowor", &m_animPow, 0.0f, 0.1f);
		ImGui::End();
	}

	UpdateWaterEffect(m_animPow, cameraPos);
}
//=====================================
// �`�揈��
//=====================================
void Wave::Draw()
{


	CManager* pManager = CManager::GetInstance();
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	D3DXMATRIX mtxViewProj = pManager->GetCamera()->GetViewProj();
	D3DXMATRIX mtxView = pManager->GetCamera()->GetView();
	D3DXMATRIX mtxProj = pManager->GetCamera()->GetProj();

	RenderReflectionTexture(pDevice);
	//CObjectX::Draw();
	RenderWaterEffect(pDevice, mtxViewProj);

}
//=====================================
// ��������
//=====================================
Wave* Wave::Create()
{
	Wave* pWave = new Wave();
	pWave->Init();
	return pWave;
}
//=====================================
// �V�F�[�_�[�̏�����
//=====================================
bool Wave::InitWaterEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
	m_fxFilePath = fxFile; // �p�X�ۑ�

	LPD3DXBUFFER pErr = nullptr;
#ifdef _DEBUG
	// �f�o�b�N��
	HRESULT hr = D3DXCreateEffectFromFileA(
		device, fxFile, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &pErr);
#else	
	// �����[�X��
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

	LoadXModel(device, "data/MODEL/block00.x");  // �����ς���Ɛ��ʂ��ς�邺

	// �m�[�}���}�b�v�̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(device, "data/TEXTURE/water_normalMap.png", &m_normalMap[0]))) {
		MessageBox(nullptr, "�m�[�}���}�b�v�ǂݍ��ݎ��s", "Error", MB_OK);
		return false;
	}
	// �m�[�}���}�b�v�̓ǂݍ���2
	if (FAILED(D3DXCreateTextureFromFile(device, "data/TEXTURE/water_normalMap01.png", &m_normalMap[1]))) {
		MessageBox(nullptr, "�m�[�}���}�b�v�ǂݍ��ݎ��s", "Error", MB_OK);
		return false;
	}
	// �m�[�}���}�b�v�̓ǂݍ���3
	if (FAILED(D3DXCreateTextureFromFile(device, "data/TEXTURE/water_normalMap04.png", &m_normalMap[2]))) {
		MessageBox(nullptr, "�m�[�}���}�b�v�ǂݍ��ݎ��s", "Error", MB_OK);
		return false;
	}

	// �L���[�u���}�b�v�̓ǂݍ���
	if (FAILED(D3DXCreateCubeTextureFromFile(device, "data/TEXTURE/WaterCubeMap01.dds", &m_pEnvMap))) {
		MessageBox(nullptr, "�L���[�u�}�b�v�ǂݍ��ݎ��s", "Error", MB_OK);
		return false;
	}

	device->CreateTexture(300, 300, 1, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pReflectionTex, nullptr);
	m_pReflectionTex->GetSurfaceLevel(0, &m_pReflectionSurf);

	// ���ʃ��b�V���i���ʁj
	D3DXCreatePolygon(device, 100, 1, &m_pMesh, nullptr);

	return true;
}
//=====================================
// �V�F�[�_�[�̔j��
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
// �V�F�[�_�[�̍X�V
//=====================================
void Wave::UpdateWaterEffect(float deltaTime, D3DXVECTOR3 camPos)
{
	CManager* pManager = CManager::GetInstance();
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	m_time += deltaTime;

	m_pEffect->SetFloat("time", m_time);

	m_pEffect->SetFloat("waveStrength", 0.05f);
	m_pEffect->SetValue("cameraPos", &camPos, sizeof(D3DXVECTOR3));

}
//=====================================
// �V�F�[�_�[�̕`��
//=====================================
void Wave::RenderWaterEffect(LPDIRECT3DDEVICE9 device, const D3DXMATRIX& viewProj)
{
	m_pEffect->SetMatrix("worldViewProj", &viewProj);

	// �m�[�}���}�b�v�̐ݒ�
	m_pEffect->SetTexture("NormalMapTex", m_normalMap[m_currentTextureIndex]);

	m_pEffect->SetTexture("ReflectionTex", m_pReflectionTex);


	m_pEffect->SetTexture("EnvMapTex", m_pEnvMap);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);        // �A���t�@�u�����hON
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);   // �\�[�X�̃u�����h�W��
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // �f�X�e�B�l�[�V�����̃u�����h�W��

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
// ���˂̕`��
//=====================================
void Wave::RenderSceneForReflection(LPDIRECT3DDEVICE9 device)
{
	CManager* pManager = CManager::GetInstance();

	D3DXVECTOR3 cameraPos = pManager->GetCamera()->GetPosV();
	D3DXVECTOR3 cameraTarget = pManager->GetCamera()->GetPosR();
	D3DXMATRIX mtxView = pManager->GetCamera()->GetProj();
	// ���ʂ̍���
	float waterHeight = 0.0f;

	// �r���[�s����擾���Ĕ��]�r���[�s����쐬
	D3DXMATRIX viewMatrix, reflectedView;
	D3DXVECTOR3 eyePos, lookAt, upVec;

	// ���݂̃r���[�s��̃p�����[�^
	eyePos = cameraPos;   // �J�����ʒu
	lookAt = cameraTarget; // �����_
	upVec = D3DXVECTOR3(0, 1, 0); // �����

	// Y�����]
	eyePos.y = waterHeight - (eyePos.y - waterHeight);
	lookAt.y = waterHeight - (lookAt.y - waterHeight);

	D3DXMatrixLookAtLH(&reflectedView, &eyePos, &lookAt, &upVec);

	// �v���W�F�N�V�����s��
	D3DXMATRIX projMatrix = mtxView;

	// ���[���h�E�r���[�E�v���W�F�N�V���������s����쐬
	D3DXMATRIX viewProj = reflectedView * projMatrix;

	// �e�I�u�W�F�N�g�̕`��
	CObjectX::Draw();
}
//=====================================
// ���˃e�N�X�`���̕`��
//=====================================
void Wave::RenderReflectionTexture(LPDIRECT3DDEVICE9 device)
{
	// �o�b�N�o�b�t�@�ۑ�
	LPDIRECT3DSURFACE9 pOldRT = nullptr;
	device->GetRenderTarget(0, &pOldRT);

	// ���˃T�[�t�F�X�֐؂�ւ�
	device->SetRenderTarget(0, m_pReflectionSurf);
	device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	RenderSceneForReflection(device);
	// �߂�
	device->SetRenderTarget(0, pOldRT);
	if (pOldRT) {
		pOldRT->Release();
		pOldRT = nullptr;
	}
}

