//==============================================
// 
//�I�u�W�F�N�g(x�t�@�C���̓ǂݍ���)�@objectX.cpp
//Auther kida ibuki 
// 
//==============================================
#include "objectX.h"
#include "manager.h"
#include <fstream>  // �o�C�i�����ɕK�v
const int CObjectX::SHADOWMAP_SIZE = 1024;   // �V���h�E�}�b�v�̑傫��

//==================================================
//�R���X�g���N�^
//==================================================
CObjectX::CObjectX()
{
	// �ϐ��̏�����
	m_pMesh = nullptr;
	m_pBuffMat = nullptr;
	m_dwNumMat = 0;
	m_pVtxBuff = nullptr;				// ���_�o�b�t�@
	for (int i = 0; i < MAX_TEX_X; i++)
	{
		m_pTexture[i] = nullptr;			// �e�N�X�`��
	}
	m_pos = { 0.0f, 0.0f, 0.0f };		// �ʒu
	m_rot = { 0.0f, 0.0f, 0.0f };		// ����
	//m_move = { 0.0f, 0.0f, 0.0f };		//�ړ���
	m_size = { 0.0f,0.0f,0.0f };
	m_nMaxVtx = { 0.0f,0.0f,0.0f };
	m_nMinVtx = { 0.0f,0.0f,0.0f };


	// ���b�V���̕`��p�ϐ�������
	mesh = nullptr;
	adjacencyBuffer = nullptr;
	materialBuffer = nullptr;
	numMaterials = 0;

	m_pMesh1 = nullptr;
	m_pMaterials = nullptr;
	m_pTextures1 = nullptr;
	m_dwNumMaterials = 0;
	m_pEffect = nullptr;

	// �V�F�[�_�[�ϐ�������
	m_pEffect = nullptr;
	//m_pShadowMap = nullptr;
	//m_pShadowSurf = nullptr;
	//m_pMainBackBuffer = nullptr;
	//m_pShadowDepth = nullptr;
	//m_pMainDepthBuffer = nullptr;
	//m_pShadowDepthBuffer = nullptr;

}
CObjectX::CObjectX(int nPrioroty) :CObject(nPrioroty)
{
	// �ϐ��̏�����
	m_pMesh = nullptr;
	m_pBuffMat = nullptr;
	m_dwNumMat = 0;
	m_pVtxBuff = nullptr;				// ���_�o�b�t�@
	for (int i = 0; i < MAX_TEX_X; i++)
	{
		m_pTexture[i] = nullptr;			// �e�N�X�`��
	}
	m_pos = { 0.0f, 0.0f, 0.0f };		// �ʒu
	m_rot = { 0.0f, 0.0f, 0.0f };		// ����
	//m_move = { 0.0f, 0.0f, 0.0f };		//�ړ���
	m_size = { 0.0f,0.0f,0.0f };
	m_nMaxVtx = { 0.0f,0.0f,0.0f };
	m_nMinVtx = { 0.0f,0.0f,0.0f };

	m_pMesh1 = nullptr;
	m_pMaterials = nullptr;
	m_pTextures1 = nullptr;
	m_dwNumMaterials = 0;
	m_pEffect = nullptr;
	m_pMaterialBuffer = nullptr;

	// �V�F�[�_�[�ϐ�������
	m_pEffect = nullptr;
	//m_pShadowMap = nullptr;
	//m_pShadowSurf = nullptr;
	//m_pMainBackBuffer = nullptr;
	//m_pShadowDepth = nullptr;
	//m_pMainDepthBuffer = nullptr;
	//m_pShadowDepthBuffer = nullptr;

}

//==================================================
//�f�X�g���N�^
//==================================================
CObjectX::~CObjectX()
{
}
//==================================================
//�����ݒ�
//==================================================
HRESULT CObjectX::Init()
{
	CManager* pManager = CManager::GetInstance();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	VERTEX_3D* pVtx; //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
	

	return S_OK;


}

//===============================================
//���f���̒��_�������ɃT�C�Y�̍ő�l�ŏ��l�����߂�
//===============================================
void CObjectX::SizeVtx()
{
	//**========================���b�V���̏����擾================**

	if (m_pMesh != nullptr)
	{
		//���_���̎擾
		m_nNumVtx = m_pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		m_sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		//���_�o�b�t�@�̃��b�N
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_bpVtxBuff);

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
		{
			//���_���W����
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_bpVtxBuff;

			//���_���W���r���ă��f���̍ő�l�A�ŏ��l���擾
			//x�̍ő�l
			if (vtx.x > m_nMaxVtx.x)
			{
				m_nMaxVtx.x = vtx.x;
			}
			//x�̍ŏ��l
			else if (vtx.x < m_nMinVtx.x)
			{
				m_nMinVtx.x = vtx.x;
			}
			//y�̍ő�l
			if (vtx.y > m_nMaxVtx.y)
			{
				m_nMaxVtx.y = vtx.y;
			}
			//y�̍ŏ��l
			else if (vtx.y < m_nMinVtx.y)
			{
				m_nMinVtx.y = vtx.y;
			}
			//z�̍ő�l
			if (vtx.z > m_nMaxVtx.z)
			{
				m_nMaxVtx.z = vtx.z;
			}
			//z�̍ŏ��l
			else if (vtx.z < m_nMinVtx.z)
			{
				m_nMinVtx.z = vtx.z;
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			m_bpVtxBuff += m_sizeFVF;
		}
		//���_�o�b�t�@�̃A�����b�N
		m_pMesh->UnlockVertexBuffer();

		//**=========================================================**
		m_size = m_nMaxVtx - m_nMinVtx;
	}
}

//==================================================
//�I������
//==================================================
void CObjectX::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	for (int i = 0; i < MAX_TEX_X; i++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[i] != nullptr)
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = nullptr;
		}
	}

	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	if (m_pMaterialBuffer)
	{
		m_pMaterialBuffer->Release();
		m_pMaterialBuffer = nullptr;
	}

	if (m_pEffect) {
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
	if (m_pToonMap) {
		m_pToonMap->Release();
		m_pToonMap = nullptr;
	}

}
//==================================================
//�X�V����
//==================================================
void CObjectX::Update()
{
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();		// �f�o�C�X���擾


}
//==================================================
//�`�揈��
//==================================================
void CObjectX::Draw()
{
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();		// �f�o�C�X���擾
	D3DXMATRIX mtxRot, mtxTrans;											// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;				// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;					// �}�e���A���f�[�^�ւ̃|�C���^


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���u���b�N�ȊO�̌�����ʒu�̔��f��
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//==================================================================================
	// �I�u�W�F�N�g�̕`��[blockX]
	//==================================================================================
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// ���f���̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �ۑ����Ă����}�e���A���ɖ߂�
	pDevice->SetMaterial(&matDef);

}
bool CObjectX::LoadEffect(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
	m_fxFilePath = fxFile; // �p�X�ۑ�

	LPD3DXBUFFER pErr = nullptr;
#ifdef _DEBUG
	// �f�o�b�N��
	HRESULT hr = D3DXCreateEffectFromFileA(
		device, fxFile, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pEffect, &pErr);
#else	
	// �����[�X��
	HRESULT hr = D3DXCreateEffectFromFileA(
		device, fxFile, nullptr, nullptr, D3DXSHADER_OPTIMIZATION_LEVEL3, nullptr, &m_pEffect, &pErr);
#endif	

	// �ʃv���W�F�N�g�Ŏg�����
	//HRESULT hr = D3DXCreateEffectFromFileA(
	//	device,
	//	"Shader/MyShader.fxo", // .fx �ł͂Ȃ� .fxo ���w��
	//	nullptr,
	//	nullptr,
	//	D3DXFX_NOT_CLONEABLE,  // �o�C�i������ǂݍ��ނƂ��̃t���O
	//	nullptr,
	//	&m_pEffect,
	//	&pErr);

	if (FAILED(hr))
	{
		if (pErr)
		{
			MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
			pErr->Release();
		}
		return false;
	}

	return true;
}
//================================================
// fxo�t�@�C���ǂݍ���(�蓮�R���p�C��������I)
//================================================
bool CObjectX::LoadEffect_fxo(LPDIRECT3DDEVICE9 device, const char* fxoFile)
{
	m_fxFilePath = fxoFile; // �p�X�ۑ��i�g���q .fxo�j

	// .fxo �o�C�i���t�@�C����ǂݍ���
	std::ifstream ifs(fxoFile, std::ios::binary | std::ios::ate);
	if (!ifs.is_open()) {
		MessageBoxA(nullptr, "FXO �t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	std::streamsize size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<char> buffer(static_cast<size_t>(size));
	if (!ifs.read(buffer.data(), size)) {
		MessageBoxA(nullptr, "FXO �o�b�t�@�ǂݍ��݂Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	HRESULT hr = D3DXCreateEffect(
		device,
		buffer.data(),
		static_cast<UINT>(buffer.size()),
		nullptr,
		nullptr,
#ifdef _DEBUG
		D3DXSHADER_DEBUG,
#else
		D3DXSHADER_OPTIMIZATION_LEVEL3,
#endif
		nullptr,
		&m_pEffect,
		nullptr
	);

	if (FAILED(hr) || m_pEffect == nullptr) {
		MessageBoxA(nullptr, "D3DXCreateEffect �Ɏ��s���܂���", "FX Error", MB_OK);
		return false;
	}

	// �g�D�[���}�b�v�ǂݍ���
	if (FAILED(D3DXCreateTextureFromFileA(device, "data/TEXTURE/ToonMap_Test.png", &m_pToonMap))) {
		MessageBoxA(nullptr, "ToonMap.png �̓ǂݍ��݂Ɏ��s���܂���", "Error", MB_OK);
		return false;
	}

	return true;
}

void CObjectX::LoadXModel(LPDIRECT3DDEVICE9 device, const char* filename)
{
	// �אڏ����󂯎��o�b�t�@
	LPD3DXBUFFER pAdjacency = nullptr;

	// �}�e���A���o�b�t�@�����݂���ꍇ�͉���j
	if (m_pBuffMat) {
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	// X�t�@�C�����烁�b�V����ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(
		filename,             // �ǂݍ���X�t�@�C����
		D3DXMESH_MANAGED,     // ���b�V���̃������Ǘ����@
		device,               // Direct3D�f�o�C�X
		&pAdjacency,          // �אڏ��
		&m_pBuffMat,          // �}�e���A�����o�b�t�@�j
		nullptr,              // �G�t�F�N�g���
		&m_dwNumMat,          // �ǂݍ��܂ꂽ�}�e���A����
		&m_pMesh              // ���b�V���I�u�W�F�N�g
	);

	// ���b�V���̓ǂݍ��݂Ɏ��s�����ꍇ�A�G���[���b�Z�[�W���o���ďI��
	if (FAILED(hr)) {
		MessageBoxA(nullptr, "���f���ǂݍ��ݎ��s", "Error", MB_OK);
		return;
	}

	// �e�N�X�`�����̏���𒴂��Ȃ��悤�ɐ���
	if (m_dwNumMat > MAX_TEX_X) m_dwNumMat = MAX_TEX_X;

	// �ǂݍ��񂾃}�e���A���o�b�t�@��D3DXMATERIAL�\���̂̔z��Ƃ��Ĉ���
	D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �Ǝ��̃}�e���A���z��ɃR�s�[�p�̔z����m��
	m_pMaterials = new D3DMATERIAL9[m_dwNumMat];

	// �e�N�X�`���z��̃|�C���^�����O�ɂ��ׂ�nullptr�ŏ�����
	for (DWORD i = 0; i < MAX_TEX_X; i++) {
		m_pTexture[i] = nullptr;
	}

	// �}�e���A���ƃe�N�X�`���̐ݒ�
	for (DWORD i = 0; i < m_dwNumMat; i++) {
		m_pMaterials[i] = pMaterials[i].MatD3D;               // �}�e���A���̃R�s�[
		m_pMaterials[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_pMaterials[i].Ambient = m_pMaterials[i].Diffuse;    // �������g�U���Ɠ����ɐݒ�

		// �e�N�X�`���t�@�C�������ݒ肳��Ă���ꍇ�͓ǂݍ���
		if (pMaterials[i].pTextureFilename) {
			HRESULT texHr = D3DXCreateTextureFromFileA(
				device,
				pMaterials[i].pTextureFilename,
				&m_pTexture[i]
			);
			// �e�N�X�`���̓ǂݍ��݂Ɏ��s�����ꍇ�̓G���[�\��
			if (FAILED(texHr)) {
				char buf[256];
				sprintf_s(buf, "�e�N�X�`���ǂݍ��ݎ��s: %s", pMaterials[i].pTextureFilename);
				MessageBoxA(NULL, buf, "ERROR", MB_OK);
			}
		}
	}
	// �@���������ꍇ�ɔ����āA�Čv�Z���Ă���
	if (!(m_pMesh->GetFVF() & D3DFVF_NORMAL)) {
		HRESULT normHr = m_pMesh->CloneMeshFVF(
			m_pMesh->GetOptions(),
			m_pMesh->GetFVF() | D3DFVF_NORMAL,
			device,
			&m_pMesh
		);
		if (SUCCEEDED(normHr)) {
			D3DXComputeNormals(m_pMesh, nullptr);
		}
	}
	// �אڏ��o�b�t�@�̉��
	if (pAdjacency) pAdjacency->Release();
}
//==================================================
//�I�u�W�F�N�g�̐���(x�t�@�C����)
//==================================================
CObjectX* CObjectX::Create()
{
	CObjectX* pObjectX = new CObjectX;
	pObjectX->Init();
	return pObjectX;

}
//==================================================
//���f���̐ݒ�(�����Ƀp�X������)
//==================================================
void CObjectX::SetModel(std::string sPath)
{
	CManager* pManager = CManager::GetInstance();
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	// ���f���̔j��
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(sPath.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);


	//SizeVtx();	// pMesh�̏����擾
}
//======================================
//�ʒu�̐ݒ�
//======================================
void CObjectX::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
//======================================
//�����̐ݒ�
//======================================
void CObjectX::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//======================================
//�T�C�Y�̐ݒ�
//======================================
void CObjectX::SetSize(D3DXVECTOR3 size)
{
	m_size =size;
}

