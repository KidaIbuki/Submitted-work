//==============================================
// 
//�|�X�g�G�t�F�N�g[�F���ω�]�@ToneAdjust.h
//Auther kida ibuki 
// 
//==============================================
#ifndef _TONEADJUST_H_
#define _TONEADJUST_H_


#include "pch.h"
#include "object2D.h"
class ToneAdjust :public CObject2D
{
public:
	ToneAdjust();		//�R���X�g���N�^
	~ToneAdjust()override;		//�f�X�g���N�^
	HRESULT Init()override;	//�����ݒ�
	void Uninit()override;		//�I������
	void Update()override;		//�X�V����
	void Draw()override;		//�`�揈��

	static ToneAdjust* Create(D3DXVECTOR2 pos);

	bool InitToneAdjust(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UninitToneAdjust();
	void UpdateToneAdjust();
	void RenderToneAdjust(LPDIRECT3DDEVICE9 device);
	void CreateFullScreenQuad(LPDIRECT3DDEVICE9 device);
private:
	LPD3DXEFFECT m_pEffect;    // �G�t�F�N�g
	std::string m_fxFilePath;             // �ǂݍ���.fx�t�@�C���̃p�X

	struct SCREEN_VERTEX {
		D3DXVECTOR4 pos;
		D3DXVECTOR2 tex;
		D3DCOLOR color; 
	};
#define FVF_SCREEN_VERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE)

	bool m_enableEffect;  // �G�t�F�N�g�̕\�����
	bool m_bActive;
	// �J���[�ێ��ϐ�
	float m_colorR;
	float m_colorG;
	float m_colorB;

};

#endif