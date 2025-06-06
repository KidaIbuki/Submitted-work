//==============================================
// 
//�|�X�g�G�t�F�N�g[�F���ω�]�@ToneAdjust.cpp
//Auther kida ibuki 
// 
//==============================================


#include "ToneAdjust.h"
#include "manager.h"

//=====================================
// �R���X�g���N�^
//=====================================
ToneAdjust::ToneAdjust()
{
    m_pEffect = nullptr;

    // �e�N�X�`���̐F
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;

    m_enableEffect = false;  // �G�t�F�N�g�̕\�����
    m_bActive = true;

}
//=====================================
// �f�X�g���N�^
//=====================================
ToneAdjust::~ToneAdjust()
{
}
//=====================================
// ����������
//=====================================
HRESULT ToneAdjust::Init()
{
    CManager* pManager = CManager::GetInstance();
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

    CObject2D::Init();
    InitToneAdjust(pDevice, "shader/ToneAdjust.fx");

	return S_OK;
}
//=====================================
// �I������
//=====================================
void ToneAdjust::Uninit()
{
    if (m_pEffect)
    {
        m_pEffect->Release();
        m_pEffect = nullptr;
    }

    CObject2D::Uninit();
}

//=====================================
// �X�V����
//=====================================
void ToneAdjust::Update()
{
    CObject2D::Update();

    // ImGui�̂��
    ImGui::SetNextWindowSize({ -1, -1 });
    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

    if (ImGui::Begin(u8"ToneAdjust"))
    {
        ImGui::Checkbox("OpenImGui", &m_bActive);
        if (!m_bActive) {


            ImGui::Text("Effect ColorRGB");
            ImGui::SliderFloat("Color R", &m_colorR, 0.0f, 1.0f);
            ImGui::SliderFloat("Color G", &m_colorG, 0.0f, 1.0f);
            ImGui::SliderFloat("Color B", &m_colorB, 0.0f, 1.0f);

            ImGui::Checkbox("EffectActive", &m_enableEffect);
        }
        ImGui::End();
    }

}
//=====================================
// �`�揈��
//=====================================
void ToneAdjust::Draw()
{
    CManager* pManager = CManager::GetInstance();
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

    //CObject2D::Draw();
    RenderToneAdjust(pDevice);
}

//=====================================
// ��������
//=====================================
ToneAdjust* ToneAdjust::Create(D3DXVECTOR2 pos)
{
    ToneAdjust* pToneAdjust = new ToneAdjust();
    pToneAdjust->Init();
    pToneAdjust->SetPos(pos);

    return pToneAdjust;
}
//=====================================
// �V�F�[�_�[�̏�����
//=====================================
bool ToneAdjust::InitToneAdjust(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
    m_fxFilePath = fxFile; // �p�X�ۑ�
    LPD3DXBUFFER pErr = nullptr;

    // �G�t�F�N�g�ǂݍ���
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
    {// �V�F�[�_�[���ǂݍ��܂�Ȃ�������
        if (pErr)
        {// �G���[�\�����o��
            MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
            pErr->Release();
        }
        return false;
    }

    return true;
}
//=====================================
// �V�F�[�_�[�̔j��
//=====================================
void ToneAdjust::UninitToneAdjust()
{
    // ����Ȃ�
}
//=====================================
// �V�F�[�_�[�̍X�V
//=====================================
void ToneAdjust::UpdateToneAdjust()
{
    D3DXVECTOR4 baseColor = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);

    m_pEffect->SetVector("g_BaseColor", &baseColor);  // �x�[�X�̐F

}
//=====================================
// �V�F�[�_�[�̕`��
//=====================================
void ToneAdjust::RenderToneAdjust(LPDIRECT3DDEVICE9 device)
{
    if (!m_enableEffect)
    {// �G�t�F�N�g�̕\�����
        return;  // false�̎��͕`�悵�Ȃ�
    }

    // --- �u�����h�L���� ---
    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // --- �F�p�����[�^�ݒ� ---
    D3DXVECTOR4 baseColor = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    D3DXVECTOR4 tint(1.0f, 1.0f, 1.0f, 1.0f);      // ���ύX
    m_pEffect->SetVector("g_BaseColor", &baseColor);  // �x�[�X�̐F
    m_pEffect->SetVector("g_Tint", &tint);    // �F��
    m_pEffect->SetFloat("g_Brightness", 0.0f);  // ���邳
    m_pEffect->SetFloat("g_Contrast", 1.0f);  // �R���g���N�^
    m_pEffect->SetFloat("g_Alpha", 0.5f); // ������

// --- �`�� ---
    UINT numPasses = 0;
    m_pEffect->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        m_pEffect->BeginPass(i);
        CreateFullScreenQuad(device);   // �S��ʂɃ|���S����`��
        m_pEffect->EndPass();
    }
    m_pEffect->End();
}
//=====================================
// �|���S���쐬
//=====================================
void ToneAdjust::CreateFullScreenQuad(LPDIRECT3DDEVICE9 device)
{

    // �w�i�T�C�Y�ݒ�
    SCREEN_VERTEX quad[] = {
    { D3DXVECTOR4(0, 0, 0.0f, 1.0f), D3DXVECTOR2(0, 0), 0xFFFFFFFF },
    { D3DXVECTOR4(1280.0f, 0, 0.0f, 1.0f), D3DXVECTOR2(1, 0), 0xFFFFFFFF },
    { D3DXVECTOR4(0, 720.0f, 0.0f, 1.0f), D3DXVECTOR2(0, 1), 0xFFFFFFFF },
    { D3DXVECTOR4(1280.0f, 720.0f, 0.0f, 1.0f), D3DXVECTOR2(1, 1), 0xFFFFFFFF },
    };

    device->SetFVF(FVF_SCREEN_VERTEX);
    device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(SCREEN_VERTEX));

}

