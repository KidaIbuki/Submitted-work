//================================
//
//x�t�@�C���Ńu���b�N(blockX.h)
//Auther kida ibuki
//================================
#ifndef _BLOCKX_H_
#define _BLOCKX_H_

#include "objectX.h"
#include "object.h"

//�u���b�N�̃N���X��`
class CBlockX : public CObjectX
{
public:
	CBlockX();																				//�R���X�g���N�^
	~CBlockX();																		//�f�X�g���N�^
	HRESULT Init();																//�����ݒ�
	void Uninit();																	//�I������
	void Update();																	//�X�V����
	void Draw();																	//�`�揈��
	static CBlockX* Create(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 rot);					//�u���b�N�̐���
	D3DXVECTOR3 Move();		//�I�u�W�F�N�g(���v���C���[)�̈ړ����ݒ�
	static CBlockX* Create2(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat);


protected:

private:

	// �ގ��ϊ��V�F�[�_�[�̕ϐ�
	D3DXVECTOR4 m_DiffuseColor;
	D3DXVECTOR4 m_SpecularColor;
	float m_Shininess;

	D3DXVECTOR3 m_LightDir;
	D3DXVECTOR3 m_CameraPos;
	bool m_bUseShader;  // �V�F�[�_�[�̐؂�ւ�

	//LPD3DXEFFECT m_pEffect;     // �G�t�F�N�g
	std::string m_fxFilePath;             // �ǂݍ���.fx�t�@�C���̃p�X
	D3DXMATRIX m_matWorld, m_matView, m_matProj;
	float m_RimPowor;
	float m_RimIntensity;

	// ImGui�p���C�g�̕����ύX�ϐ�
	float vecX, vecY, vecZ;

	// �e�N�X�`���̍ގ���ύX�V�F�[�_�[�֐�
	bool LoadTextureEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UninitTextureEffect();
	void UpdateTextureEffect(LPD3DXEFFECT effect, const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& proj, const D3DXVECTOR3& cameraPos, const D3DXVECTOR3& lightDir);
	void DrawTextureEffect(D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj, LPD3DXMESH mesh);

	// ToonShader2
	void UpdateToonShader(LPDIRECT3DDEVICE9 device);
	void RenderToonShader(LPDIRECT3DDEVICE9 device);

	// liltoon(���s)
	void UpdateShaderParameters(LPD3DXEFFECT effect, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX proj);
	void RenderMeshWithEffect(LPDIRECT3DDEVICE9 pDevice);


	// �A�E�g���C���A�������C�g�A�Z�����b�N(���s)
	bool InitShader(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UpdateShader(LPDIRECT3DDEVICE9 device/*, const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& proj*/);
	void RenderWithToonShader(LPD3DXMESH mesh, LPDIRECT3DDEVICE9 device);

	// �����c��
	bool InitDistanceShader(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UpdateDistanceShader(LPDIRECT3DDEVICE9 device, const D3DXVECTOR3& cameraPos);
	void RenderDistanceShader(LPDIRECT3DDEVICE9 device);

};
#endif
