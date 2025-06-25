//=====================================
// 
// アウトラインシェーダー＆トゥーンシェーダー
// Auther 木田威吹
// 
//=====================================

// -----------------------------
// グローバル変数宣言
// -----------------------------
float4x4 World;     // ワールド行列
float4x4 View;      // ビュー行列
float4x4 Projection;        // プロジェクション行列
float4x4 WorldViewProj;     // ワールドビュープロジェクション一括変換用行列
float3x3 gWorldInverseTranspose;    // 法線変換用の逆転置行列

float3 LightDirection; // 平行光源の方向

float4 LightColor = float4(1, 1, 1, 1);                       // 光の色（白）
float4 ShadowColor = float4(0.4, 0.3, 0.3, 1);                // 影色（やや肌色寄りの暗色）
float4 MainColor = float4(1, 1, 1, 1);                        // テクスチャに乗算する基本色

// -----------------------------
// テクスチャサンプラブロック
// -----------------------------
texture DiffuseTexture;
sampler2D DiffuseSampler = sampler_state {
    Texture = <DiffuseTexture>;    // 使用するテクスチャを指定
    MinFilter = LINEAR;            // ミップマップ以下での補間
    MagFilter = LINEAR;            // 拡大時の補間
    MipFilter = LINEAR;            // ミップマップ間の補間
};

// -----------------------------
// 頂点入出力構造体
// -----------------------------

// 頂点シェーダー入力構造体
struct VS_INPUT {
    float4 Position : POSITION;     // 頂点の位置（ローカル座標）
    float3 Normal : NORMAL;         // 法線ベクトル（ローカル空間）
    float2 TexCoord : TEXCOORD0;    // テクスチャ座標
};

// 通常描画用の頂点シェーダー出力
struct VS_OUTPUT {
    float4 Position : POSITION;          // 出力位置（スクリーン空間座標）
    float2 TexCoord : TEXCOORD0;         // テクスチャ座標（ピクセルシェーダーに渡す）
    float  LightIntensity : TEXCOORD1;   // ライトとの角度（明暗判定用）
};

// アウトライン描画用の頂点シェーダー出力
struct VS_OUTPUT_OUTLINE {
    float4 Pos : POSITION;    // 位置
    float2 Tex : TEXCOORD0;   // テクスチャ座標
    float3 LightIntensity : COLOR0;      // ライティング結果
};

// -----------------------------
// トゥーン用頂点シェーダー
// -----------------------------
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 頂点座標をワールド空間へ変換
    float4 worldPos = mul(input.Position, World);

    // 法線をワールド空間に変換
    float3 worldNormal = normalize(mul(float4(input.Normal, 0.0), World).xyz);

    output.Position = mul(input.Position, World);
    // ビュー→プロジェクション変換
    output.Position = mul(worldPos, View);
    output.Position = mul(output.Position, Projection);

    output.Position = mul(input.Position, WorldViewProj);
    // UV座標はそのまま
    output.TexCoord = input.TexCoord;

    output.LightIntensity = saturate(dot(worldNormal, -normalize(LightDirection)));

    return output;
}

// -----------------------------
// アウトライン用 頂点シェーダー
// -----------------------------
VS_OUTPUT_OUTLINE VS_Outline(VS_INPUT input)
{
    VS_OUTPUT_OUTLINE output;

    // ワールド空間の法線ベクトルに変換
    float3 normalWS = normalize(mul(input.Normal, gWorldInverseTranspose)); // ワールド空間の法線

    // 法線方向に少し押し出して輪郭線を作る
    float3 offset = normalWS * 0.3;

    // 元の頂点位置にオフセットを加える
    float4 localPos = input.Position;
    localPos.xyz += offset; // ローカル空間でオフセット

    // ローカル座標から最終スクリーン座標へ変換
    output.Pos = mul(localPos, WorldViewProj);

    output.LightIntensity = float3(0.0, 0.0, 0.0);
    output.Tex = input.TexCoord;
    return output;
}

// -----------------------------
// トゥーン ピクセルシェーダー
// -----------------------------
float4 PS_Main(VS_OUTPUT input) : COLOR
{
    // テクスチャから色を取得
    float4 texColor = tex2D(DiffuseSampler, input.TexCoord);

    // トゥーン風に明暗を2段階で切り替える（0.5が境界)
    float lighting = smoothstep(0.4, 0.6, input.LightIntensity);

    // 明暗に応じて色を補間
    float4 shadeColor = lerp(ShadowColor, LightColor, lighting);

    // テクスチャ色 × 基本色 × 光の影響
    return texColor * MainColor * shadeColor;
}

// -----------------------------
// アウトライン ピクセルシェーダー
// -----------------------------
float4 PS_Outline(VS_OUTPUT_OUTLINE input) : COLOR{
    return float4(0.0, 0.0, 0.0, 1.0); // 完全な黒で描画（輪郭線用）
}

// -----------------------------
// テクニック定義
// -----------------------------

// トゥーンレンダリング
technique ToonTechnique {
    pass P0 {
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader = compile ps_2_0 PS_Main();
    }
}
// アウトライン
technique Outline {
    pass P0 {
        CullMode = CW;  // 裏面（三角形の逆向き）だけ描画
        VertexShader = compile vs_2_0 VS_Outline();
        PixelShader = compile ps_2_0 PS_Outline();
    }
}
