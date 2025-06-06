//==============================================
// 
//�I�u�W�F�N�g(x�t�@�C���̓ǂݍ���)�@objectX.h
//Auther kida ibuki 
// 
//==============================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "object.h"
#include "pch.h"
#include "main.h"

#define MAX_CHAR (256)
#define NUM_MODEL (21) //���f���̎��
#define MAX_TEX_X (36) // 1���f���̃e�N�X�`���̍ő吔

class CCamera;

//�I�u�W�F�N�gX�N���X�̒�`
class CObjectX :public CObject
{
public:
	//**==================��{����====================**
	CObjectX();		//�R���X�g���N�^
	CObjectX(int nPriority);		//�R���X�g���N�^
	~CObjectX() override;		//�f�X�g���N�^
	HRESULT Init() override;	//�����ݒ�
	void Uninit() override;		//�I������
	void Update() override;		//�X�V����
	void Draw() override;		//�`�揈��

	void InitEffect(LPDIRECT3DDEVICE9 device);
	void UpdateEffect(LPDIRECT3DDEVICE9 device);
	void DrawWithEffect(LPDIRECT3DDEVICE9 device);
	bool LoadEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void LoadXModel(LPDIRECT3DDEVICE9 device, const char* filename);
	//void DrawModelWithEffect(LPD3DXEFFECT pEffect);
	bool LoadEffect_fxo(LPDIRECT3DDEVICE9 device, const char* fxoFile);


	//ID3DXMesh* LoadMeshFromX(LPDIRECT3DDEVICE9 device, const char* filename);
	//**=================�e�����̏���==================**
	static CObjectX* Create();		//�I�u�W�F�N�g�̐���
	void SetModel(std::string sPath);		//���f���̃Z�b�g
	void SizeVtx();						// �I�u�W�F�N�g�̃T�C�Y�̍ő�ŏ������߂�
	//**===============�e�ݒ�̎擾======================**
	D3DXVECTOR3 GetPos() { return m_pos; }			//�ʒu�̎擾
	D3DXVECTOR3 GetSize() { return m_size; }		//�T�C�Y�̎擾
	D3DXVECTOR3 GetRot() { return m_rot; }

	void SetPos(D3DXVECTOR3 pos);			//�e�I�u�W�F�N�g�̈ʒu�ݒ�
	void SetRot(D3DXVECTOR3 rot);
	void SetSize(D3DXVECTOR3 size);	//�e�I�u�W�F�N�g�̃T�C�Y�ݒ�

	D3DXMATRIX GetWorldMatrix() { return m_mtxWorld; }  //���[���h�}�g���b�N�X���擾(�K�w�\���ɕK�v)
	void SetWorldMatrix(D3DXMATRIX mat) { m_mtxWorld = mat; }
	LPD3DXMESH GetMesh() { return m_pMesh; }

	D3DXVECTOR3 m_pos;							//�ʒu
	D3DXVECTOR3 m_rot;							//����
	D3DXVECTOR3 m_move;							//�ړ���
	D3DXVECTOR3 m_size;							//�T�C�Y
	LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEX_X];				//�e�N�X�`��

protected:
	//**==============�`��̏��=================**
	D3DXMATRIX m_mtxWorld;		//���[���h�ϊ��s��
	LPD3DXMESH m_pMesh;			//���b�V���ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;    //�}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;           //�}�e���A���̐�

	// ���b�V���̕`��p�ϐ�
	ID3DXMesh* mesh;
	ID3DXBuffer* adjacencyBuffer;
	ID3DXBuffer* materialBuffer;
	DWORD numMaterials;
	//���f���t�@�C���̓ǂݍ��ݍ\����
	typedef struct
	{
		int nType;
		char cFile[MAX_CHAR];		//�t�@�C����ǂݍ���
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		int nIndexFile;
		int Parent;			//�p�[�c�̐e�q�֌W
	}MODEL_FILENAME;

	MODEL_FILENAME m_gModelData[NUM_MODEL];

	//**==========�����o�ϐ�================**
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//���_�o�b�t�@
	//LPDIRECT3DTEXTURE9 m_pTexture;				//�e�N�X�`��

	std::string m_fxFilePath;             // �ǂݍ���.fx�t�@�C���̃p�X
	LPDIRECT3DTEXTURE9 m_pToonMap;  // �g�D�[���}�b�v

	ID3DXMesh* m_pMesh1;
	D3DMATERIAL9* m_pMaterials;
	LPDIRECT3DTEXTURE9* m_pTextures1;
	DWORD m_dwNumMaterials;
	LPD3DXEFFECT m_pEffect;
	LPD3DXBUFFER m_pMaterialBuffer;
private:
	//**=============���̃I�u�W�F�N�gX�ł����g���Ȃ��֐�===============**




	//**===========���f���̃��b�V�����擾=============**
	D3DXVECTOR3 m_vtxMinModel;		//���f���̒��_�̍ŏ��l
	D3DXVECTOR3 m_vtxMaxModel;		//���f���̒��_�̍ő�l
	int m_nNumVtx;					//���_��
	DWORD m_sizeFVF;				//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* m_bpVtxBuff;				//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_nMaxVtx;					//���f���̍ő�l
	D3DXVECTOR3 m_nMinVtx;					//���f���̍ŏ��l


		// �V���h�E�}�b�v�p�V�F�[�_�[�ϐ�
	LPDIRECT3DTEXTURE9 m_pShadowMap;
	LPDIRECT3DSURFACE9 m_pShadowSurf;
	static const int SHADOWMAP_SIZE;
	std::vector<CObjectX*> objects;
	LPDIRECT3DSURFACE9 m_pMainBackBuffer;
	D3DXMATRIX m_matLightView, m_matLightProj;
	LPDIRECT3DSURFACE9 m_pShadowDepth;
	LPDIRECT3DSURFACE9 m_pMainDepthBuffer;
	LPDIRECT3DSURFACE9 m_pShadowDepthBuffer;
	D3DXMATRIX m_LightView, m_LightProj, m_world;



};

//class Terrain
//{
//public:
//	Terrain(LPD3DXMESH mesh);
//	
//
//	struct Triangle {
//		D3DXVECTOR3 v0, v1, v2;  // �O�p�`�̒��_
//		D3DXVECTOR3 normal;      // �@���x�N�g��
//	};
//
//	const std::vector<Triangle>& GetTriangles() const { return triangles; }
//
//private:
//	std::vector<Triangle> triangles;
//
//	void ExtractPolygons(LPD3DXMESH mesh);
//	
//};

#endif
