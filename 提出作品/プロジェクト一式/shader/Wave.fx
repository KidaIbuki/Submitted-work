//=====================================
// 
// �t���l�����ʕt���g�V�F�[�_�[�V�F�[�_�[
// Auther �ؓc�А�
// 
// ====================================

//=======================
// �萔�E�p�����[�^
//=======================
float4x4 worldViewProj; // ���[���h �� �r���[ �� �v���W�F�N�V�����s��
float3 cameraPos;       // �J�����ʒu
float time;             // ���ԁiC++������n���j

// �e�N�X�`��
texture NormalMapTex;   // ���ʗp�m�[�}���}�b�v
texture EnvMapTex;      // ���}�b�v�i���˗p�L���[�u�}�b�v�j
sampler2D ReflectionTex  : register(s2);

// �g�A�j���[�V�����p�����[�^
float waveStrength = 0.05;
float waveSpeed = 2.0;
float waveFrequency = 4.0;

//=======================
// �T���v���ݒ�
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
// ���_�V�F�[�_�p�\����
//=======================
struct VS_IN {
    float4 pos : POSITION;   // ���_�ʒu
    float2 uv  : TEXCOORD0;  // UV���W
};

struct VS_OUT {
    float4 pos       : POSITION;
    float2 uv        : TEXCOORD0;
    float3 worldPos  : TEXCOORD1;
    float3 viewDir   : TEXCOORD2;
    float4 screenPos : TEXCOORD4;
};

//=======================
// ���_�V�F�[�_
//=======================
VS_OUT VS_main(VS_IN IN) {
    VS_OUT OUT;

    float4 worldPosition = IN.pos; // �ϊ��s�v�i���[�J�������[���h�z��j
    OUT.worldPos = worldPosition.xyz;

    OUT.pos = mul(worldPosition, worldViewProj); // �ŏI�X�N���[���ʒu
    OUT.uv = IN.uv;
    OUT.screenPos = OUT.pos;
    OUT.viewDir = normalize(cameraPos - OUT.worldPos); // �����x�N�g��

    return OUT;
}

//=======================
// �s�N�Z���V�F�[�_
//=======================
float4 PS_main(VS_OUT IN) : COLOR{
    // �m�[�}���}�b�v�X�N���[��
    float2 uv1 = IN.uv + time * float2(0.05, 0.03);
    float2 uv2 = IN.uv + time * float2(-0.02, 0.04);

    // �@���擾�ƍ���
    float3 n1 = tex2D(NormalSampler, uv1).xyz * 2 - 1;
    float3 n2 = tex2D(NormalSampler, uv2).xyz * 2 - 1;
    float3 normal = normalize(n1 + n2);

    // �r���[�����Ɣ��˃x�N�g��
    float3 I = normalize(IN.viewDir);
    float3 R = reflect(I, normal);

    // ���}�b�v���甽�ːF
    float4 envColor = texCUBE(EnvSampler, R);

    // �t���l����
    float fresnel = pow(1.0 - saturate(dot(normal, I)), 3.0);

    // �x�[�X�J���[
    float4 waterBase = float4(0.5, 0.8, 0.9, 0.3);

    // �ȈՃ��C�e�B���O
    float3 lightDir = normalize(float3(0.3, -1, 0.2));
    float NdotL = saturate(dot(normal, -lightDir));
    float3 litColor = waterBase.rgb * (0.3 + 0.7 * NdotL);

    // ---------- �X�N���[�����W���甽�˃e�N�X�`�����Q�� ----------
    float2 screenUV = IN.screenPos.xy / IN.screenPos.w;
    screenUV = screenUV * 0.5 + 0.5; // -1~1 �� 0~1 �ɕϊ�
    screenUV.y = 1.0 - screenUV.y;  // Y�����]�iDirectX�j

    float4 reflectionColor = tex2D(ReflectionTex, screenUV);
    // -----------------------------------------------------------------------

    // �t���l���Ŋ��}�b�v�Ɣ��˃e�N�X�`��������
    float4 reflectionMixed = lerp(reflectionColor, envColor, 0.5);

    // ���C�e�B���O�J���[�Ɣ��˂��t���l���ō�����
    float4 finalColor = lerp(float4(litColor, waterBase.a), reflectionMixed, fresnel);

    // �A���t�@
    finalColor.a = 0.6;

    return finalColor;
}
//=======================
// �e�N�j�b�N
//=======================
technique WaterTech {
    pass P0 {
        VertexShader = compile vs_2_0 VS_main();
        PixelShader = compile ps_3_0 PS_main();
    }
}