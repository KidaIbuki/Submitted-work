//================================
//
// �t���l�����ʕt���̔g�V�F�[�_�[(WaterSurface.h)
// Auther kida ibuki
// 
//================================
#ifndef _WATERSURFACE_H_
#define _WATERSURFACE_H_

#include "pch.h"
#include "object3D.h"
#include "objectX.h"
class Wave : public CObjectX
{
public:
	Wave();	// �R���X�g���N�^
	~Wave();	// �f�X�g���N�^
	HRESULT Init();			// �����ݒ�
	void Uninit();			// �I������
	void Update();			// �X�V����
	void Draw();			// �`�揈��
	static Wave* Create();	// ����
private:

	bool InitWaterEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void ShutdownWaterEffect();  // �ϐ��̔j��
	void UpdateWaterEffect(float deltaTime, D3DXVECTOR3 camPos);  // �V�F�[�_�[�̍X�V
	void RenderWaterEffect(LPDIRECT3DDEVICE9 device, const D3DXMATRIX& viewProj);  // �V�F�[�_�[�̕`��
	void RenderReflectionTexture(LPDIRECT3DDEVICE9 device);
	void RenderSceneForReflection(LPDIRECT3DDEVICE9 device);
	std::string m_fxFilePath;             // �ǂݍ���.fx�t�@�C���̃p�X

	static constexpr int NUM_TEX = 3;
	LPD3DXEFFECT m_pWaterEffect;
	LPD3DXMESH m_pWaterMesh;
	LPDIRECT3DCUBETEXTURE9 m_pEnvMap;
	D3DXHANDLE m_hTech, m_hWorld, m_hView, m_hProj, m_hTime, m_hEnvMap;
	float m_time;
	float m_animPow;
	LPDIRECT3DTEXTURE9 m_normalMap[NUM_TEX];
	bool m_bChange;
	int m_currentTextureIndex;
	LPDIRECT3DTEXTURE9 m_pReflectionTex;
	LPDIRECT3DSURFACE9 m_pReflectionSurf;
	D3DXMATRIX m_reflectionViewProj;
};

#endif