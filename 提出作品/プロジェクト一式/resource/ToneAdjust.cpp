//==============================================
// 
//ポストエフェクト[色調変化]　ToneAdjust.cpp
//Auther kida ibuki 
// 
//==============================================


#include "ToneAdjust.h"
#include "manager.h"

//=====================================
// コンストラクタ
//=====================================
ToneAdjust::ToneAdjust()
{
    m_pEffect = nullptr;

    // テクスチャの色
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;

    m_enableEffect = false;  // エフェクトの表示状態
    m_bActive = true;

}
//=====================================
// デストラクタ
//=====================================
ToneAdjust::~ToneAdjust()
{
}
//=====================================
// 初期化処理
//=====================================
HRESULT ToneAdjust::Init()
{
    CManager* pManager = CManager::GetInstance();
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

    CObject2D::Init();
    InitToneAdjust(pDevice, "shader/ToneAdjust.fx");

	return S_OK;
}
//=====================================
// 終了処理
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
// 更新処理
//=====================================
void ToneAdjust::Update()
{
    CObject2D::Update();

    // ImGuiのやつ
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
// 描画処理
//=====================================
void ToneAdjust::Draw()
{
    CManager* pManager = CManager::GetInstance();
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

    //CObject2D::Draw();
    RenderToneAdjust(pDevice);
}

//=====================================
// 生成処理
//=====================================
ToneAdjust* ToneAdjust::Create(D3DXVECTOR2 pos)
{
    ToneAdjust* pToneAdjust = new ToneAdjust();
    pToneAdjust->Init();
    pToneAdjust->SetPos(pos);

    return pToneAdjust;
}
//=====================================
// シェーダーの初期化
//=====================================
bool ToneAdjust::InitToneAdjust(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
    m_fxFilePath = fxFile; // パス保存
    LPD3DXBUFFER pErr = nullptr;

    // エフェクト読み込み
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
    {// シェーダーが読み込まれなかったら
        if (pErr)
        {// エラー表示を出す
            MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
            pErr->Release();
        }
        return false;
    }

    return true;
}
//=====================================
// シェーダーの破棄
//=====================================
void ToneAdjust::UninitToneAdjust()
{
    // 今回なし
}
//=====================================
// シェーダーの更新
//=====================================
void ToneAdjust::UpdateToneAdjust()
{
    D3DXVECTOR4 baseColor = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);

    m_pEffect->SetVector("g_BaseColor", &baseColor);  // ベースの色

}
//=====================================
// シェーダーの描画
//=====================================
void ToneAdjust::RenderToneAdjust(LPDIRECT3DDEVICE9 device)
{
    if (!m_enableEffect)
    {// エフェクトの表示状態
        return;  // falseの時は描画しない
    }

    // --- ブレンド有効化 ---
    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // --- 色パラメータ設定 ---
    D3DXVECTOR4 baseColor = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    D3DXVECTOR4 tint(1.0f, 1.0f, 1.0f, 1.0f);      // 無変更
    m_pEffect->SetVector("g_BaseColor", &baseColor);  // ベースの色
    m_pEffect->SetVector("g_Tint", &tint);    // 色味
    m_pEffect->SetFloat("g_Brightness", 0.0f);  // 明るさ
    m_pEffect->SetFloat("g_Contrast", 1.0f);  // コントラクタ
    m_pEffect->SetFloat("g_Alpha", 0.5f); // 半透明

// --- 描画 ---
    UINT numPasses = 0;
    m_pEffect->Begin(&numPasses, 0);
    for (UINT i = 0; i < numPasses; ++i)
    {
        m_pEffect->BeginPass(i);
        CreateFullScreenQuad(device);   // 全画面にポリゴンを描画
        m_pEffect->EndPass();
    }
    m_pEffect->End();
}
//=====================================
// ポリゴン作成
//=====================================
void ToneAdjust::CreateFullScreenQuad(LPDIRECT3DDEVICE9 device)
{

    // 背景サイズ設定
    SCREEN_VERTEX quad[] = {
    { D3DXVECTOR4(0, 0, 0.0f, 1.0f), D3DXVECTOR2(0, 0), 0xFFFFFFFF },
    { D3DXVECTOR4(1280.0f, 0, 0.0f, 1.0f), D3DXVECTOR2(1, 0), 0xFFFFFFFF },
    { D3DXVECTOR4(0, 720.0f, 0.0f, 1.0f), D3DXVECTOR2(0, 1), 0xFFFFFFFF },
    { D3DXVECTOR4(1280.0f, 720.0f, 0.0f, 1.0f), D3DXVECTOR2(1, 1), 0xFFFFFFFF },
    };

    device->SetFVF(FVF_SCREEN_VERTEX);
    device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(SCREEN_VERTEX));

}

