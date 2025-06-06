//================================
//
//「気」の蓄積(EffectAccumulation.cpp)
// Auther kida ibuki
// 
//================================
#include "manager.h"
#include "EffectAccumulation.h"

//======================================
// コンストラクタ
//======================================
EffectAccumulation::EffectAccumulation()
{
	m_sceneRenderTarget = nullptr;   // レンダラーターゲット
	m_fireTexture = nullptr;   // テクスチャ
	m_fireEffect = nullptr;   // エフェクト
    m_fireTime = 0.0f;  // アニメーションの速度
    m_sceneSurface = nullptr;   // サーフェス

    // テクスチャの色
    m_colorR = 1.0f;
    m_colorG = 1.0f;
    m_colorB = 1.0f;

    // 円と尖りの強弱
    m_circlePowor = 0.4f;
    m_sharpnessPowor = 0.1f;

    m_circleSize = 4.0f;  // エフェクトのサイズ(Update内で加算したら徐々に増える。キーを押したときなど)

    m_enableEffect = false;  // エフェクトの表示状態
    m_bActive = true;
}
//======================================
// デストラクタ
//======================================
EffectAccumulation::~EffectAccumulation()
{
}
//======================================
// 初期化処理
//======================================
HRESULT EffectAccumulation::Init()
{
    CManager* pManager = CManager::GetInstance();
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

    if (FAILED(CObject2D::Init()))
    {
        return E_FAIL;
    }

    InitPostEffect(pDevice, "shader/EffectAccumulation.fx");  // シェーダーの初期化 & 呼び出し

	return S_OK;
}
//======================================
// 終了処理
//======================================
void EffectAccumulation::Uninit()
{
    ShutdownPostEffect();   // シェーダーの変数破棄

    CObject2D::Uninit();
}
//======================================
// 更新処理
//======================================
void EffectAccumulation::Update()
{
    CObject2D::Update();

    // ImGuiのやつ
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


    UpdatePostEffect(0.05f);  // シェーダーの更新(引数:アニメーションの速度)
}
//======================================
// 描画処理
//======================================
void EffectAccumulation::Draw()
{
    CManager* pManager = CManager::GetInstance();
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

    RenderEffect(pDevice);  // シェーダーの描画
}
//======================================
// 生成処理
//======================================
EffectAccumulation* EffectAccumulation::Create()
{
	EffectAccumulation* pEffect = new EffectAccumulation();
	pEffect->Init();
	return pEffect;
}
//======================================
// シェーダーの初期化 & 呼び出し
//======================================
void EffectAccumulation::InitPostEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
    m_fxFilePath = fxFile; // パス保存

    // テクスチャの作成
    device->CreateTexture(
        1280, 720, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
        &m_sceneRenderTarget, nullptr
    );
    m_sceneRenderTarget->GetSurfaceLevel(0, &m_sceneSurface);  // サーフェスレベルの取得

    // 炎テクスチャの読み込み
    D3DXCreateTextureFromFile(device, "data/TEXTURE/fireNoice.png", &m_fireTexture);

    // エフェクトの読み込み
    D3DXCreateEffectFromFile(device, fxFile, nullptr, nullptr, 0, nullptr, &m_fireEffect, nullptr);

    // フルスクリーン四角形の作成
    device->CreateVertexBuffer(4 * sizeof(ScreenVertex), 0, FVF_SCREENVERTEX, D3DPOOL_MANAGED, &m_fullscreenVB, nullptr);

    // テクスチャのサイズを指定 + テクスチャ座標も
    ScreenVertex* vertices;
    m_fullscreenVB->Lock(0, 0, (void**)&vertices, 0);
    vertices[0] = { { 0.0f,       0.0f,    0.0f, 1.0f }, { 0.0f, 0.0f } };
    vertices[1] = { { TEX_SIZE_X, 0.0f ,   0.0f, 1.0f }, { 1.0f, 0.0f } };
    vertices[2] = { { 0.0f,       TEX_SIZE_Y,  0.0f, 1.0f }, { 0.0f, 1.0f } };
    vertices[3] = { { TEX_SIZE_X, TEX_SIZE_Y,  0.0f, 1.0f }, { 1.0f, 1.0f } };
    m_fullscreenVB->Unlock();
}
//======================================
// シェーダーに関する変数の破棄
//======================================
void EffectAccumulation::ShutdownPostEffect()
{
    // シーンのレンダラーターゲットの破棄
    if (m_sceneRenderTarget)
    {
        m_sceneRenderTarget->Release();
        m_sceneRenderTarget = nullptr;
    }
    // テクスチャの破棄
    if (m_fireTexture)
    {
       m_fireTexture->Release();
       m_fireTexture = nullptr;
    }
    // エフェクトの破棄
    if (m_fireEffect) 
    {
        m_fireEffect->Release();
        m_fireEffect = nullptr;
    }
    // ポリゴンのバッファの破棄
    if (m_fullscreenVB)
    {
        m_fullscreenVB->Release();
        m_fullscreenVB = nullptr;
    }
    // サーフェスの破棄
    if (m_sceneSurface)
    {
        m_sceneSurface->Release();
        m_sceneSurface = nullptr;
    }

}
//======================================
// シェーダーの更新
//======================================
void EffectAccumulation::UpdatePostEffect(float deltaTime)
{
    m_fireTime += deltaTime;   // アニメーションの速度

    D3DXVECTOR4 color = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);

    m_fireEffect->SetFloat("circlePowor", m_circlePowor);   // 円の強弱をimguiで変更可
    m_fireEffect->SetFloat("sharpnessPowor", m_sharpnessPowor);   // 尖りの強弱

    m_fireEffect->SetVector("flameColor", &color);  // テクスチャの色
    m_fireEffect->SetFloat("size", m_circleSize);  // ポストエフェクトのサイズ(増減を変えれる)


}
//======================================
// シェーダーの描画
//======================================
void EffectAccumulation::RenderEffect(LPDIRECT3DDEVICE9 device)
{
    if (!m_enableEffect)
    {// エフェクトの表示状態
        return;  // falseの時は描画しない
    }
    // バックバッファ取得
    LPDIRECT3DSURFACE9 backBuffer = nullptr;
    device->GetRenderTarget(0, &backBuffer);

    device->SetRenderTarget(0, m_sceneSurface);  // シーンサーフェスをセット

    // 画面のクリア
    device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

    CObject2D::Draw();  // 2D描画

    // アルファブレンド設定
    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    // テクスチャのアルファも適応
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

    // バックバッファの設定
    device->SetRenderTarget(0, backBuffer);


    D3DXVECTOR4 color = D3DXVECTOR4(m_colorR, m_colorG, m_colorB, 1.0f);
    // fireEffect に値をセット
    m_fireEffect->SetTexture("screenTex", m_sceneRenderTarget);  // シーンのレンダラーターゲット
    m_fireEffect->SetTexture("fireTex", m_fireTexture);   // テクスチャ
    m_fireEffect->SetFloat("time", m_fireTime);    // アニメーションタイム
    m_fireEffect->SetFloat("circlePowor", m_circlePowor);   // 円の強弱
    m_fireEffect->SetFloat("sharpnessPowor", m_sharpnessPowor);  // 尖りの強弱
    m_fireEffect->SetVector("flameColor", &color);   // テクスチャの色
    m_fireEffect->SetFloat("size", m_circleSize);    // エフェクトのサイズ


    m_fireEffect->SetTechnique("TechMain");  // テクニック宣言
    UINT passCount = 0;
    m_fireEffect->Begin(&passCount, 0);
    for (UINT i = 0; i < passCount; ++i)
    {
        m_fireEffect->BeginPass(i);

        // ポリゴン描画
        device->SetFVF(FVF_SCREENVERTEX);
        device->SetStreamSource(0, m_fullscreenVB, 0, sizeof(ScreenVertex));
        device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

        m_fireEffect->EndPass();
    }
    m_fireEffect->End();

    // バックバッファのリセット
    if (backBuffer)
    {
        backBuffer->Release();
        backBuffer = nullptr;
    }
}
