//================================
//
//�u�C�v�̒~��(EffectAccumulation.h)
// Auther kida ibuki
// 
//================================
#ifndef _EFFECTACCUMULATION_H_
#define _EFFECTACCUMULATION_H_

#include "pch.h"

class EffectAccumulation :public CObject2D
{
public:
	EffectAccumulation();	// �R���X�g���N�^
	~EffectAccumulation();	// �f�X�g���N�^
	HRESULT Init();			// �����ݒ�
	void Uninit();			// �I������
	void Update();			// �X�V����
	void Draw();			// �`�揈��
	static EffectAccumulation* Create();	// ����

private:
	LPDIRECT3DTEXTURE9 m_sceneRenderTarget;
	LPDIRECT3DTEXTURE9 m_fireTexture;
	LPD3DXEFFECT m_fireEffect;
	LPDIRECT3DSURFACE9 m_sceneSurface;
	LPDIRECT3DVERTEXBUFFER9 m_fullscreenVB; // �t���X�N���[���N�A�b�h�p
	float m_fireTime;
	std::string m_fxFilePath;             // �ǂݍ���.fx�t�@�C���̃p�X

	// �J���[�ێ��ϐ�
	float m_colorR;
	float m_colorG;
	float m_colorB;
	// �~�Ɛ��̋���ϐ�
	float m_circlePowor;
	float m_sharpnessPowor;
	float m_circleSize;
	// �Ԃ�������ϐ�
	//float m_holeMin;
	//float m_holeMax;
	bool m_enableEffect;  // �G�t�F�N�g�̕\�����
	bool m_bActive;
	void InitPostEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);  // �V�F�[�_�[�Ɗe�����̏�����
	void ShutdownPostEffect();  // �ϐ��̔j��
	void UpdatePostEffect(float deltaTime);  // �V�F�[�_�[�̍X�V
	void RenderEffect(LPDIRECT3DDEVICE9 device);  // �V�F�[�_�[�̕`��

	// ���_�̍\����
	struct ScreenVertex {
		D3DXVECTOR4 pos;
		D3DXVECTOR2 uv;
	};
	// ���_�t�H�[�}�b�g(���̃V�F�[�_�[��p)
#define FVF_SCREENVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

	static constexpr float TEX_SIZE_X = 1280.0f;  // �e�N�X�`�����W X
	static constexpr float TEX_SIZE_Y = 720.0f;   // �e�N�X�`�����W Y

};


#endif