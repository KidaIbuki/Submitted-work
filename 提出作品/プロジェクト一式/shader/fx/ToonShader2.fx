//=====================================
// 
// �A�E�g���C���V�F�[�_�[���g�D�[���V�F�[�_�[
// Auther �ؓc�А�
// 
//=====================================

// -----------------------------
// �O���[�o���ϐ��錾
// -----------------------------
float4x4 World;     // ���[���h�s��
float4x4 View;      // �r���[�s��
float4x4 Projection;        // �v���W�F�N�V�����s��
float4x4 WorldViewProj;     // ���[���h�r���[�v���W�F�N�V�����ꊇ�ϊ��p�s��
float3x3 gWorldInverseTranspose;    // �@���ϊ��p�̋t�]�u�s��

float3 LightDirection; // ���s�����̕���

float4 LightColor = float4(1, 1, 1, 1);                       // ���̐F�i���j
float4 ShadowColor = float4(0.4, 0.3, 0.3, 1);                // �e�F�i��┧�F���̈ÐF�j
float4 MainColor = float4(1, 1, 1, 1);                        // �e�N�X�`���ɏ�Z�����{�F

// -----------------------------
// �e�N�X�`���T���v���u���b�N
// -----------------------------
texture DiffuseTexture;
sampler2D DiffuseSampler = sampler_state {
    Texture = <DiffuseTexture>;    // �g�p����e�N�X�`�����w��
    MinFilter = LINEAR;            // �~�b�v�}�b�v�ȉ��ł̕��
    MagFilter = LINEAR;            // �g�厞�̕��
    MipFilter = LINEAR;            // �~�b�v�}�b�v�Ԃ̕��
};

// -----------------------------
// ���_���o�͍\����
// -----------------------------

// ���_�V�F�[�_�[���͍\����
struct VS_INPUT {
    float4 Position : POSITION;     // ���_�̈ʒu�i���[�J�����W�j
    float3 Normal : NORMAL;         // �@���x�N�g���i���[�J����ԁj
    float2 TexCoord : TEXCOORD0;    // �e�N�X�`�����W
};

// �ʏ�`��p�̒��_�V�F�[�_�[�o��
struct VS_OUTPUT {
    float4 Position : POSITION;          // �o�͈ʒu�i�X�N���[����ԍ��W�j
    float2 TexCoord : TEXCOORD0;         // �e�N�X�`�����W�i�s�N�Z���V�F�[�_�[�ɓn���j
    float  LightIntensity : TEXCOORD1;   // ���C�g�Ƃ̊p�x�i���Ô���p�j
};

// �A�E�g���C���`��p�̒��_�V�F�[�_�[�o��
struct VS_OUTPUT_OUTLINE {
    float4 Pos : POSITION;    // �ʒu
    float2 Tex : TEXCOORD0;   // �e�N�X�`�����W
    float3 LightIntensity : COLOR0;      // ���C�e�B���O����
};

// -----------------------------
// �g�D�[���p���_�V�F�[�_�[
// -----------------------------
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;

    // ���_���W�����[���h��Ԃ֕ϊ�
    float4 worldPos = mul(input.Position, World);

    // �@�������[���h��Ԃɕϊ�
    float3 worldNormal = normalize(mul(float4(input.Normal, 0.0), World).xyz);

    output.Position = mul(input.Position, World);
    // �r���[���v���W�F�N�V�����ϊ�
    output.Position = mul(worldPos, View);
    output.Position = mul(output.Position, Projection);

    output.Position = mul(input.Position, WorldViewProj);
    // UV���W�͂��̂܂�
    output.TexCoord = input.TexCoord;

    output.LightIntensity = saturate(dot(worldNormal, -normalize(LightDirection)));

    return output;
}

// -----------------------------
// �A�E�g���C���p ���_�V�F�[�_�[
// -----------------------------
VS_OUTPUT_OUTLINE VS_Outline(VS_INPUT input)
{
    VS_OUTPUT_OUTLINE output;

    // ���[���h��Ԃ̖@���x�N�g���ɕϊ�
    float3 normalWS = normalize(mul(input.Normal, gWorldInverseTranspose)); // ���[���h��Ԃ̖@��

    // �@�������ɏ��������o���ė֊s�������
    float3 offset = normalWS * 0.3;

    // ���̒��_�ʒu�ɃI�t�Z�b�g��������
    float4 localPos = input.Position;
    localPos.xyz += offset; // ���[�J����ԂŃI�t�Z�b�g

    // ���[�J�����W����ŏI�X�N���[�����W�֕ϊ�
    output.Pos = mul(localPos, WorldViewProj);

    output.LightIntensity = float3(0.0, 0.0, 0.0);
    output.Tex = input.TexCoord;
    return output;
}

// -----------------------------
// �g�D�[�� �s�N�Z���V�F�[�_�[
// -----------------------------
float4 PS_Main(VS_OUTPUT input) : COLOR
{
    // �e�N�X�`������F���擾
    float4 texColor = tex2D(DiffuseSampler, input.TexCoord);

    // �g�D�[�����ɖ��Â�2�i�K�Ő؂�ւ���i0.5�����E)
    float lighting = smoothstep(0.4, 0.6, input.LightIntensity);

    // ���Âɉ����ĐF����
    float4 shadeColor = lerp(ShadowColor, LightColor, lighting);

    // �e�N�X�`���F �~ ��{�F �~ ���̉e��
    return texColor * MainColor * shadeColor;
}

// -----------------------------
// �A�E�g���C�� �s�N�Z���V�F�[�_�[
// -----------------------------
float4 PS_Outline(VS_OUTPUT_OUTLINE input) : COLOR{
    return float4(0.0, 0.0, 0.0, 1.0); // ���S�ȍ��ŕ`��i�֊s���p�j
}

// -----------------------------
// �e�N�j�b�N��`
// -----------------------------

// �g�D�[�������_�����O
technique ToonTechnique {
    pass P0 {
        VertexShader = compile vs_2_0 VS_Main();
        PixelShader = compile ps_2_0 PS_Main();
    }
}
// �A�E�g���C��
technique Outline {
    pass P0 {
        CullMode = CW;  // ���ʁi�O�p�`�̋t�����j�����`��
        VertexShader = compile vs_2_0 VS_Outline();
        PixelShader = compile ps_2_0 PS_Outline();
    }
}
