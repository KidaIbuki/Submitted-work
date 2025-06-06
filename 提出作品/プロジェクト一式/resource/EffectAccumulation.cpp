//================================
//
//�u�C�v�̒~��(EffectAccumulation.cpp)
// Auther kida ibuki
// 
//================================
#include "manager.h"
#include "EffectAccumulation.h"

//======================================
// �R���X�g���N�^
//======================================
EffectAccumulation::EffectAccumulation()
{
	m_sceneRenderTarget = nullptr;   // �����_���[�^�[�Q�b�g
	m_fireTexture = nullptr;   // �e�N�X�`��
	m_fireEffect = nullptr;   // �G�t�F�N�g
    m_fireTime = 0.0f;  // �A�j���[�V�����̑��x
    m_sceneSurface = nullptr;   // �T�[�t�F�X

    // �e�N�X�`���̐F
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;

    // �~�Ɛ��̋���
    m_circlePowor = 0.4f;
    m_sharpnessPowor = 0.1f;

    m_circleSize = 4.0f;  // �G�t�F�N�g�̃T�C�Y(Update���ŉ��Z�����珙�X�ɑ�����B�L�[���������Ƃ��Ȃ�)

    m_enableEffect = false;  // �G�t�F�N�g�̕\�����
    m_bActive = true;
}
//======================================
// �f�X�g���N�^
//======================================
EffectAccumulation::~EffectAccumulation()
{
}
//======================================
// ����������
//======================================
HRESULT EffectAccumulation::Init()
{
    CManager* pManager = CManager::GetInstance();
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

    if (FAILED(CObject2D::Init()))
    {
        return E_FAIL;
    }

    InitPostEffect(pDevice, "shader/EffectAccumulation.fx");  // �V�F�[�_�[�̏����� & �Ăяo��

	return S_OK;
}
//======================================
// �I������
//======================================
void EffectAccumulation::Uninit()
{
    ShutdownPostEffect();   // �V�F�[�_�[�̕ϐ��j��

    CObject2D::Uninit();
}
//======================================
// �X�V����
//======================================
void EffectAccumulation::Update()
{
    CObject2D::Update();

    // ImGui�̂��
    ImGui::SetNextWindowSize({ -1, -1 });
    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

    if (ImGui::Begin(u8"EffectAccumulation"))
    {
        ImGui::Checkbox("OpenImGui", &m_bActive);
        if (!m_bActive) {


            ImGui::Text("Effect ColorRGB");
            ImGui::SliderFloat("Color R", &m_colorR, 0.0f, 1.0f);
            ImGui::SliderFloat("Color G", &m_colorG, 0.0f, 1.0f);
            ImGui::SliderFloat("Color B", &m_colorB, 0.0f, 1.0f);

            ImGui::Text("Effect Powor");
            ImGui::SliderFloat("circlePowor", &m_circlePowor, 0.0f, 0.5f);
            ImGui::SliderFloat("sharpnessPowor", &m_sharpnessPowor, 0.0f, 0.5f);

            ImGui::Text("Effect Size");
            ImGui::SliderFloat("circleSize", &m_circleSize, 2.0f, 10.0f);
            ImGui::Checkbox("EffectActive", &m_enableEffect);
        }
        ImGui::End();
    }


    UpdatePostEffect(0.05f);  // �V�F�[�_�[�̍X�V(����:�A�j���[�V�����̑��x)
}
//======================================
// �`�揈��
//======================================
void EffectAccumulation::Draw()
{
    CManager* pManager = CManager::GetInstance();
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

    RenderEffect(pDevice);  // �V�F�[�_�[�̕`��
}
//======================================
// ��������
//======================================
EffectAccumulation* EffectAccumulation::Create()
{
	EffectAccumulation* pEffect = new EffectAccumulation();
	pEffect->Init();
	return pEffect;
}
//======================================
// �V�F�[�_�[�̏����� & �Ăяo��
//======================================
void EffectAccumulation::InitPostEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
    m_fxFilePath = fxFile; // �p�X�ۑ�

    // �e�N�X�`���̍쐬
    device->CreateTexture(
        1280, 720, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
        &m_sceneRenderTarget, nullptr
    );
    m_sceneRenderTarget->GetSurfaceLevel(0, &m_sceneSurface);  // �T�[�t�F�X���x���̎擾

    // ���e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(device, "data/TEXTURE/fireNoice.png", &m_fireTexture);

    // �G�t�F�N�g�̓ǂݍ���
    D3DXCreateEffectFromFile(device, fxFile, nullptr, nullptr, 0, nullptr, &m_fireEffect, nullptr);

    // �t���X�N���[���l�p�`�̍쐬
    device->CreateVertexBuffer(4 * sizeof(ScreenVertex), 0, FVF_SCREENVERTEX, D3DPOOL_MANAGED, &m_fullscreenVB, nullptr);

    // �e�N�X�`���̃T�C�Y���w�� + �e�N�X�`�����W��
    ScreenVertex* vertices;
    m_fullscreenVB->Lock(0, 0, (void**)&vertices, 0);
    vertices[0] = { { 0.0f,       0.0f,    0.0f, 1.0f }, { 0.0f, 0.0f } };
    vertices[1] = { { TEX_SIZE_X, 0.0f ,   0.0f, 1.0f }, { 1.0f, 0.0f } };
    vertices[2] = { { 0.0f,       TEX_SIZE_Y,  0.0f, 1.0f }, { 0.0f, 1.0f } };
    vertices[3] = { { TEX_SIZE_X, TEX_SIZE_Y,  0.0f, 1.0f }, { 1.0f, 1.0f } };
    m_fullscreenVB->Unlock();
}
//======================================
// �V�F�[�_�[�Ɋւ���ϐ��̔j��
//======================================
void EffectAccumulation::ShutdownPostEffect()
{
    // �V�[���̃����_���[�^�[�Q�b�g�̔j��
    if (m_sceneRenderTarget)
    {
        m_sceneRenderTarget->Release();
        m_sceneRenderTarget = nullptr;
    }
    // �e�N�X�`���̔j��
    if (m_fireTexture)
    {
       m_fireTexture->Release();
       m_fireTexture = nullptr;
    }
    // �G�t�F�N�g�̔j��
    if (m_fireEffect) 
    {
        m_fireEffect->Release();
        m_fireEffect = nullptr;
    }
    // �|���S���̃o�b�t�@�̔j��
    if (m_fullscreenVB)
    {
        m_fullscreenVB->Release();
        m_fullscreenVB = nullptr;
    }
    // �T�[�t�F�X�̔j��
    if (m_sceneSurface)
    {
        m_sceneSurface->Release();
        m_sceneSurface = nullptr;
    }

}
//======================================
// �V�F�[�_�[�̍X�V
//======================================
void EffectAccumulation::UpdatePostEffect(float deltaTime)
{
    m_fireTime += deltaTime;   // �A�j���[�V�����̑��x

    D3DXVECTOR4 color = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);

    m_fireEffect->SetFloat("circlePowor", m_circlePowor);   // �~�̋����imgui�ŕύX��
    m_fireEffect->SetFloat("sharpnessPowor", m_sharpnessPowor);   // ���̋���

    m_fireEffect->SetVector("flameColor", &color);  // �e�N�X�`���̐F
    m_fireEffect->SetFloat("size", m_circleSize);  // �|�X�g�G�t�F�N�g�̃T�C�Y(������ς����)


}
//======================================
// �V�F�[�_�[�̕`��
//======================================
void EffectAccumulation::RenderEffect(LPDIRECT3DDEVICE9 device)
{
    if (!m_enableEffect)
    {// �G�t�F�N�g�̕\�����
        return;  // false�̎��͕`�悵�Ȃ�
    }
    // �o�b�N�o�b�t�@�擾
    LPDIRECT3DSURFACE9 backBuffer = nullptr;
    device->GetRenderTarget(0, &backBuffer);

    device->SetRenderTarget(0, m_sceneSurface);  // �V�[���T�[�t�F�X���Z�b�g

    // ��ʂ̃N���A
    device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

    CObject2D::Draw();  // 2D�`��

    // �A���t�@�u�����h�ݒ�
    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // �e�N�X�`���̃A���t�@���K��
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

    // �o�b�N�o�b�t�@�̐ݒ�
    device->SetRenderTarget(0, backBuffer);


    D3DXVECTOR4 color = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    // fireEffect �ɒl���Z�b�g
    m_fireEffect->SetTexture("screenTex", m_sceneRenderTarget);  // �V�[���̃����_���[�^�[�Q�b�g
    m_fireEffect->SetTexture("fireTex", m_fireTexture);   // �e�N�X�`��
    m_fireEffect->SetFloat("time", m_fireTime);    // �A�j���[�V�����^�C��
    m_fireEffect->SetFloat("circlePowor", m_circlePowor);   // �~�̋���
    m_fireEffect->SetFloat("sharpnessPowor", m_sharpnessPowor);  // ���̋���
    m_fireEffect->SetVector("flameColor", &color);   // �e�N�X�`���̐F
    m_fireEffect->SetFloat("size", m_circleSize);    // �G�t�F�N�g�̃T�C�Y


    m_fireEffect->SetTechnique("TechMain");  // �e�N�j�b�N�錾
    UINT passCount = 0;
    m_fireEffect->Begin(&passCount, 0);
    for (UINT i = 0; i < passCount; ++i)
    {
        m_fireEffect->BeginPass(i);

        // �|���S���`��
        device->SetFVF(FVF_SCREENVERTEX);
        device->SetStreamSource(0, m_fullscreenVB, 0, sizeof(ScreenVertex));
        device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

        m_fireEffect->EndPass();
    }
    m_fireEffect->End();

    // �o�b�N�o�b�t�@�̃��Z�b�g
    if (backBuffer)
    {
        backBuffer->Release();
        backBuffer = nullptr;
    }
}
