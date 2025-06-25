//=====================================
// 
// フレネル効果付き波シェーダーシェーダー
// Auther 木田威吹
// 
// ====================================

//=======================
// 定数・パラメータ
//=======================
float4x4 worldViewProj; // ワールド → ビュー → プロジェクション行列
float3 cameraPos;       // カメラ位置
float time;             // 時間（C++側から渡す）

// テクスチャ
texture NormalMapTex;   // 水面用ノーマルマップ
texture EnvMapTex;      // 環境マップ（反射用キューブマップ）
sampler2D ReflectionTex  : register(s2);

// 波アニメーションパラメータ
float waveStrength = 0.05;
float waveSpeed = 2.0;
float waveFrequency = 4.0;

//=======================
// サンプラ設定
//=======================
sampler2D NormalSampler = sampler_state {
    Texture = <NormalMapTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

samplerCUBE EnvSampler = sampler_state {
    Texture = <EnvMapTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

//=======================
// 頂点シェーダ用構造体
//=======================
struct VS_IN {
    float4 pos : POSITION;   // 頂点位置
    float2 uv  : TEXCOORD0;  // UV座標
};

struct VS_OUT {
    float4 pos       : POSITION;
    float2 uv        : TEXCOORD0;
    float3 worldPos  : TEXCOORD1;
    float3 viewDir   : TEXCOORD2;
    float4 screenPos : TEXCOORD4;
};

//=======================
// 頂点シェーダ
//=======================
VS_OUT VS_main(VS_IN IN) {
    VS_OUT OUT;

    float4 worldPosition = IN.pos; // 変換不要（ローカル＝ワールド想定）
    OUT.worldPos = worldPosition.xyz;

    OUT.pos = mul(worldPosition, worldViewProj); // 最終スクリーン位置
    OUT.uv = IN.uv;
    OUT.screenPos = OUT.pos;
    OUT.viewDir = normalize(cameraPos - OUT.worldPos); // 視線ベクトル

    return OUT;
}

//=======================
// ピクセルシェーダ
//=======================
float4 PS_main(VS_OUT IN) : COLOR{
    // ノーマルマップスクロール
    float2 uv1 = IN.uv + time * float2(0.05, 0.03);
    float2 uv2 = IN.uv + time * float2(-0.02, 0.04);

    // 法線取得と合成
    float3 n1 = tex2D(NormalSampler, uv1).xyz * 2 - 1;
    float3 n2 = tex2D(NormalSampler, uv2).xyz * 2 - 1;
    float3 normal = normalize(n1 + n2);

    // ビュー方向と反射ベクトル
    float3 I = normalize(IN.viewDir);
    float3 R = reflect(I, normal);

    // 環境マップから反射色
    float4 envColor = texCUBE(EnvSampler, R);

    // フレネル項
    float fresnel = pow(1.0 - saturate(dot(normal, I)), 3.0);

    // ベースカラー
    float4 waterBase = float4(0.5, 0.8, 0.9, 0.3);

    // 簡易ライティング
    float3 lightDir = normalize(float3(0.3, -1, 0.2));
    float NdotL = saturate(dot(normal, -lightDir));
    float3 litColor = waterBase.rgb * (0.3 + 0.7 * NdotL);

    // ---------- スクリーン座標から反射テクスチャを参照 ----------
    float2 screenUV = IN.screenPos.xy / IN.screenPos.w;
    screenUV = screenUV * 0.5 + 0.5; // -1~1 → 0~1 に変換
    screenUV.y = 1.0 - screenUV.y;  // Y軸反転（DirectX）

    float4 reflectionColor = tex2D(ReflectionTex, screenUV);
    // -----------------------------------------------------------------------

    // フレネルで環境マップと反射テクスチャを合成
    float4 reflectionMixed = lerp(reflectionColor, envColor, 0.5);

    // ライティングカラーと反射をフレネルで混ぜる
    float4 finalColor = lerp(float4(litColor, waterBase.a), reflectionMixed, fresnel);

    // アルファ
    finalColor.a = 0.6;

    return finalColor;
}
//=======================
// テクニック
//=======================
technique WaterTech {
    pass P0 {
        VertexShader = compile vs_2_0 VS_main();
        PixelShader = compile ps_3_0 PS_main();
    }
}