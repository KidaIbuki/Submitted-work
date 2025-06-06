//================================
//
//x�t�@�C���Ńu���b�N(blockX.h)
//Auther kida ibuki
//================================
#include "blockX.h"
#include "manager.h"

//======================================
//�R���X�g���N�^
//======================================
CBlockX::CBlockX()
{
	m_DiffuseColor = D3DXVECTOR4(1, 1, 1, 1);
	m_SpecularColor = D3DXVECTOR4(1, 1, 1, 1);
	m_Shininess = 32.0f;

	m_LightDir = D3DXVECTOR3(0, -1, -1);
	m_CameraPos = D3DXVECTOR3(0, 5, -10);
	m_RimPowor = 0.0f;
	m_RimIntensity = 0.0f;

	vecX = 0.0f;
	vecY = 0.0f;
	vecZ = 0.0f;

}
//======================================
//�f�X�g���N�^
//======================================
CBlockX::~CBlockX()
{
}
//======================================
//�����ݒ�
//======================================
HRESULT CBlockX::Init()
{
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}
	CManager* pManager = CManager::GetInstance();
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	// ��ނ��u���b�N�ɐݒ�
	SetType(CObject::TYPE::BLOCK);

	return S_OK;
}
//======================================
//�I������
//======================================
void CBlockX::Uninit()
{
	CObjectX::Uninit();
}
//======================================
//�X�V����
//======================================
void CBlockX::Update()
{
	CManager* pManager = CManager::GetInstance();
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();
	D3DXMATRIX mtxViewProj = pManager->GetCamera()->GetViewProj();
	D3DXMATRIX mtxView = pManager->GetCamera()->GetView();
	D3DXMATRIX mtxProj = pManager->GetCamera()->GetProj();
	D3DXMATRIX worldViewProj = GetWorldMatrix();

	// �J���������擾�i����J�����N���X�j
	D3DXVECTOR3 cameraPos = pManager->GetCamera()->GetPosV();
	UpdateToonShader(pDevice);  // �g�D�[���V�F�[�_�[
	//�ړ��ʂ̍X�V
	m_move += Move();

	//pos�𓮂���
	m_pos += m_move;



	m_pos.y += m_move.y;

	//�ړ��ʂ�����
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;
	m_move.y += (0.0f - m_move.y) * 0.1f;

}
//======================================
//�`�揈��
//======================================
void CBlockX::Draw()
{
	CManager* pManager = CManager::GetInstance();
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld, mtxParent;	// �v�Z�p�}�g���N�X	
	D3DMATERIAL9 matDef;					// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL* pMat;						// �}�e���A���f�[�^�ւ̃|�C���^

	D3DXMATRIX mtxViewProj = pManager->GetCamera()->GetViewProj();
	D3DXMATRIX mtxView = pManager->GetCamera()->GetView();
	D3DXMATRIX mtxProj = pManager->GetCamera()->GetProj();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���u���b�N�ȊO�̌�����ʒu�̔��f��
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);  // �V�F�[�_�[�̕`�悱�̏ォ���̂ǂ��炩����������������炨�������Ȃ邺

	RenderToonShader(pDevice);  // �g�D�[���V�F�[�_�[

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
//======================================
//�u���b�N�̐���
//======================================
CBlockX* CBlockX::Create(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CBlockX* pBlockX = new CBlockX();
	
	pBlockX->SetPos(pos);
	pBlockX->SetRot(rot);

	pBlockX->Init();
	//pBlockX->LoadEffect(device, "shader/ToonShader2.fx");
	pBlockX->LoadEffect(device, "shader/ToonShader.fxo");   // fxo�ǂݍ���(�O���ύX�ł��Ȃ�)


	pBlockX->LoadXModel(device, "data/MODEL/ballD.x");


	return pBlockX;
}
//=====================================
// ���f���̈ړ�
//=====================================
D3DXVECTOR3 CBlockX::Move()
{
	CManager* pManager = CManager::GetInstance();  //�}�l�[�W���[�̃C���X�^���X�擾

	CInputKeyboard* pKeyboard = pManager->GetKeyboard();    //�L�[�{�[�h�擾


	D3DXVECTOR3 move = { 0,0,0 };
	//**========�J�����̏��==========**

	if (pKeyboard->GetPress(DIK_A))
	{//A�L�[�������ꂽ�ꍇ
		move.x -= 0.2f;

	}
	if (pKeyboard->GetPress(DIK_D))
	{//D�L�[�������ꂽ�ꍇ

		move.x += 0.2f;
	}
	if (pKeyboard->GetPress(DIK_W))
	{//A�L�[�������ꂽ�ꍇ
		move.z += 0.2f;

	}
	if (pKeyboard->GetPress(DIK_S))
	{//D�L�[�������ꂽ�ꍇ

		move.z -= 0.2f;
	}
	if (pKeyboard->GetPress(DIK_N))
	{//D�L�[�������ꂽ�ꍇ

		move.y += 0.2f;
	}
	if (pKeyboard->GetPress(DIK_M))
	{//D�L�[�������ꂽ�ꍇ

		move.y -= 0.2f;
	}
	if (pKeyboard->GetPress(DIK_B))
	{//D�L�[�������ꂽ�ꍇ

		m_rot.y -= 0.02f;
	}
	if (pKeyboard->GetPress(DIK_V))
	{//D�L�[�������ꂽ�ꍇ

		m_rot.x -= 0.02f;
	}
	if (pKeyboard->GetPress(DIK_Q))
	{//D�L�[�������ꂽ�ꍇ

		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}


	return move;
}
//=====================================
// �V�F�[�_�[�̍X�V
//=====================================
void CBlockX::UpdateToonShader(LPDIRECT3DDEVICE9 device)
{
	CManager* pManager = CManager::GetInstance();  //�}�l�[�W���[�̃C���X�^���X�擾

	D3DXMATRIX world, view, proj, wvp;
	world = GetWorldMatrix();
	// ���[���h�s��F��]�Ȃǉ�����
	//D3DXMatrixRotationY(&world, timeGetTime() / 1000.0f);
	D3DXMATRIX rotY;
	D3DXMatrixRotationY(&rotY, timeGetTime() / 1000.0f);
	world = rotY * world; // ��]��K�p

	// �r���[�s��
	D3DXVECTOR3 eye(0, 2, -5), at(0, 0, 0), up(0, 1, 0);
	D3DXMatrixLookAtLH(&view, &eye, &at, &up);

	// �ˉe�s��
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, 800.0f / 600.0f, 0.1f, 100.0f);

	// ���[���h�r���[�ˉe����
	wvp = world * view * proj;

	// �V�F�[�_�֐ݒ�
	m_pEffect->SetMatrix("World", &world);
	m_pEffect->SetMatrix("View", &view);
	m_pEffect->SetMatrix("Projection", &proj);
	m_pEffect->SetMatrix("WorldViewProj", &wvp);

	// ���C�g�̌���
	D3DXVECTOR3 lightDir = pManager->GetLight()->GetLightDir();

	// ���K���i�K�v�Ȃ�j
	D3DXVec3Normalize(&lightDir, &lightDir);

	// �G�t�F�N�g�ɓn���igLightDir �� fx �t�@�C���� float3 ���ƈ�v������j
	m_pEffect->SetVector("LightDirection", (D3DXVECTOR4*)&lightDir);

	// �@���ϊ��p�̋t�]�u�s����쐬
	D3DXMATRIX worldInvTrans;
	D3DXMatrixInverse(&worldInvTrans, NULL, &world);
	D3DXMatrixTranspose(&worldInvTrans, &worldInvTrans);
	m_pEffect->SetMatrix("gWorldInverseTranspose", &worldInvTrans);
}
//=====================================
// �V�F�[�_�[�`��
//=====================================
void CBlockX::RenderToonShader(LPDIRECT3DDEVICE9 device)
{
	CManager* pManager = CManager::GetInstance();  //�}�l�[�W���[�̃C���X�^���X�擾

	D3DXMATRIX view, proj, wvp;
	D3DXMATRIX mtxWorld = GetWorldMatrix();
	D3DXMATRIX matWorldNoScale, matWorldInvTrans;

	device->GetTransform(D3DTS_VIEW, &view);
	device->GetTransform(D3DTS_PROJECTION, &proj);
	wvp = mtxWorld * view * proj;

	// �G�t�F�N�g�ɕϐ���n��(����4�̈ʒu��ς���Ƃ��������Ȃ��)
	m_pEffect->SetMatrix("World", &mtxWorld);
	m_pEffect->SetMatrix("View", &view);
	m_pEffect->SetMatrix("Projection", &proj);
	m_pEffect->SetMatrix("WorldViewProj", &wvp);

	// �G�t�F�N�g�ݒ�p�X�P�[���E��]�E���s�ړ�
	D3DXVECTOR3 scale, translation;
	D3DXQUATERNION rotation;

	// ���f���s��iWorld�j
	D3DXMatrixIdentity(&mtxWorld); // ��]�E�ړ�������  ����������ƍ����Ƃ���o��

	D3DXVECTOR3 eye(0, 2, -5), at(0, 0, 0), up(0, 1, 0);

	// �r���[�E�v���W�F�N�V�����s��i�Œ� or �J�������琶���j
	D3DXMatrixLookAtLH(&view,
		&eye,  // Eye
		&at,   // LookAt
		&up);  // Up

	// �ˉe�s��
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, 640.0f / 480.0f, 1.0f, 100.0f);

	D3DXVECTOR3 Scaling = D3DXVECTOR3(1, 1, 1);  // �X�P�[��

	D3DXMatrixDecompose(&scale, &rotation, &translation, &mtxWorld);
	D3DXMatrixTransformation(&matWorldNoScale, nullptr, nullptr, &Scaling, nullptr, &rotation, &translation);

	// �@���ϊ��p�FWorld�̃X�P�[�����O�����O���ċt�]�u�s����쐬
	D3DXMatrixInverse(&matWorldInvTrans, NULL, &mtxWorld);
	D3DXMatrixTranspose(&matWorldInvTrans, &matWorldInvTrans);

	m_pEffect->SetMatrix("gWorldInverseTranspose", &matWorldInvTrans);  // ���O�����X�P�[�����O���G�t�F�N�g�ɓ����


		// ���C�g�̌���
	D3DXVECTOR3 lightDir = pManager->GetLight()->GetLightDir();

	// ���K���i�K�v�Ȃ�j
	D3DXVec3Normalize(&lightDir, &lightDir);

	// �G�t�F�N�g�ɓn���igLightDir �� fx �t�@�C���� float3 ���ƈ�v������j
	m_pEffect->SetVector("LightDirection", (D3DXVECTOR4*)&lightDir);

	UINT numPasses = 0;

	// --- �A�E�g���C���`��i��Ɂj ---
	m_pEffect->SetTechnique("Outline");
	m_pEffect->Begin(&numPasses, 0);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		for (int n = 0; n < m_dwNumMat; ++n)
		{
			m_pEffect->CommitChanges();
			m_pMesh->DrawSubset(n);
		}
		m_pEffect->EndPass();
	}
	m_pEffect->End();

	// --- �g�D�[���{�̕`��i��Ɂj ---
	m_pEffect->SetTechnique("ToonTechnique"); 
	m_pEffect->Begin(&numPasses, 0);
	for (UINT i = 0; i < numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		for (int n = 0; n < m_dwNumMat; ++n)
		{
			//m_pEffect->SetTexture("DiffuseTexture", m_pTexture[n]);
			m_pEffect->SetTexture("DiffuseTexture", m_pToonMap);   // �ǂ������������Ǝv��

			m_pEffect->CommitChanges();
			m_pMesh->DrawSubset(n);
		}
		m_pEffect->EndPass();
	}
	m_pEffect->End();

}
