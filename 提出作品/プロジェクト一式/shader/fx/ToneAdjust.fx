//=====================================
// 
// 色調変化 シェーダー
// Auther 木田威吹
// 
//=====================================


// -----------------------------
// 色調調整用シェーダー（背景色向け）
// -----------------------------

// 背景のベース色（元の色）
float3 g_BaseColor = float3(1.0f, 1.0f, 1.0f); // 初期は白

// 色調補正用のパラメータ
float3 g_Tint = float3(1.0f, 1.0f, 1.0f);      // RGBごとの乗算係数（色味）
float g_Brightness = 0.0f;                    // 明るさ補正（-1.0 ～ 1.0）
float g_Contrast = 1.0f;                      // コントラスト（1.0が標準）
float g_Alpha = 1.0f;

// -----------------------------
// 頂点入出力構造体
// -----------------------------
struct VS_OUTPUT {
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

// -----------------------------
// 頂点シェーダ
// -----------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD0) {
    VS_OUTPUT output;
    output.Pos = Pos;
    output.Tex = Tex;
    return output;
}

// -----------------------------
// ピクセルシェーダ
// -----------------------------
float4 PS(VS_OUTPUT input) : COLOR{
    float3 color = g_BaseColor;

    // 色味調整
    color *= g_Tint;

    // 明るさ調整
    color += g_Brightness;

    // コントラスト調整
    color = ((color - 0.5) * g_Contrast) + 0.5;

    return float4(saturate(color), g_Alpha); // Alphaは常に1.0（不透明）

}

// -----------------------------
// テクニック定義
// -----------------------------
technique ToneAdjust {
    pass P0 {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
    }
}