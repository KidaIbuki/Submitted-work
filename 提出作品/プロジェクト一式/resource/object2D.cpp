//=================================
// 
// �I�u�W�F�N�g2D�N���X�@object2D.cpp
// Auther kida ibuki 
// 
//==================================
#include "object2D.h"   // �I�u�W�F�N�g2D
#include "manager.h"        // �}�l�[�W���[
#include "pch.h"            // �v���R���p�C���ς݃w�b�_�[
//==============================
// �R���X�g���N�^
//==============================
CObject2D::CObject2D()
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_nDivide = 1;
	m_nTime = 0;
	m_nextTime = timeGetTime();
	m_nAnimIdx = 0;
	m_bReverse = false;
	m_pos = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2(10.0f, 10.0f);
	m_fAngle = 0.0f;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;

	Init();
}

//==============================
// �f�X�g���N�^
//==============================
CObject2D::~CObject2D()
{

}
//==============================
// �����ݒ�(���_�o�b�t�@����)
//==============================
HRESULT CObject2D::Init()
{
	CManager* pManager = CManager::GetInstance();

	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D* pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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

//==============================
// �I������(���_�o�b�t�@�j��)
//==============================
void CObject2D::Uninit()
{
	//�e�N�X�`���̔j��
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

}

//==============================
// �X�V����(���_���W�̍X�V)
//==============================
void CObject2D::Update()
{
	UpdateVertex();
	//UpdateAnim();
}
//==============================
// �`�揈��(�|���S���̐���)
//==============================
void CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	CManager* pManager = CManager::GetInstance();

	pDevice = pManager->GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Z�̔�r���@��ύX
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	//Z�o�b�t�@�ɏ������܂Ȃ�(���ɕ\�����邩�A��O�ɕ\�����邩)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Z�̔�r���@�ύX
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//Z�o�b�t�@�ɏ�������
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// ���ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}
//=========================================
// [CObject2D] ���W�̐ݒ�
//=========================================
void CObject2D::SetPos(D3DXVECTOR2 pos)
{
	m_pos = pos;
}

//=========================================
// �A�j���[�V�����ݒ�
//=========================================
void CObject2D::SetAnim(int nDivide, int nTime)
{
	if (nDivide > 0)
	{
		m_nDivide = nDivide;
		m_nTime = nTime;
		m_nextTime = timeGetTime() + m_nTime;
	}
}

//================================================
// �A�j���[�V�����̍X�V
//================================================
void CObject2D::UpdateAnim()
{
	VERTEX_2D* pVtx; //���_���ւ̃|�C���^
	float fDivideSize = 1.0f / (float)m_nDivide;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	if (!m_bReverse)
	{
		pVtx[0].tex = D3DXVECTOR2(fDivideSize * m_nAnimIdx, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(fDivideSize * (m_nAnimIdx + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(fDivideSize * m_nAnimIdx, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(fDivideSize * (m_nAnimIdx + 1), 1.0f);
	}
	else
	{
		pVtx[0].tex = D3DXVECTOR2(fDivideSize * (m_nAnimIdx + 1), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(fDivideSize * m_nAnimIdx, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(fDivideSize * (m_nAnimIdx + 1), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(fDivideSize * m_nAnimIdx, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �A�j���[�V������i�߂�
	if (timeGetTime() >= m_nextTime)
	{
		// ���̃A�j���[�V�����Ɉڂ�
		m_nAnimIdx++;

		// ���̎��Ԃ�ݒ肷��
		m_nextTime = timeGetTime() + m_nTime;

		if (m_nAnimIdx >= m_nDivide)
		{ // �A�j���[�V�������ő吔�ɒB�����Ƃ�
			m_nAnimIdx = 0;
		}
	}
}
//=============================
// �e�N�X�`���̐ݒ�
//=============================
void CObject2D::SetTexture(std::string sPath)		//�����ɓǂݍ��݂����e�N�X�`���̃p�X������
{
	CManager* pManager = CManager::GetInstance();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	// �e�N�X�`���̔j��
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, sPath.c_str(), &m_pTexture);
}
//============================
// �T�C�Y�ݒ�
//============================
void CObject2D::SetSize(float fWidth, float fHeight)
{
	m_size = D3DXVECTOR2(fWidth, fHeight);
}
//============================
// �����ݒ�
//============================
void CObject2D::SetAngle(float fAngle)
{
	m_fAngle = fAngle;
}
//============================
// �J���[�ݒ�
//============================
void CObject2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D* pVtx; //���_���ւ̃|�C���^

    //���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

}
//==============================
// �e�N�X�`���̐ݒ�2
//==============================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}
//==============================
// ���_���̍X�V
//=============================
void CObject2D::UpdateVertex()
{
	VERTEX_2D* pVtx; //���_���ւ̃|�C���^

	D3DXMATRIX mtx;
	D3DXMatrixIdentity(&mtx);

	D3DXVECTOR3 sizePos[4] =
	{
		{-m_size.x * VECTOR_SIZE, -m_size.y * VECTOR_SIZE, 0.0f},
		{m_size.x * VECTOR_SIZE, -m_size.y * VECTOR_SIZE, 0.0f},
		{-m_size.x * VECTOR_SIZE, m_size.y * VECTOR_SIZE, 0.0f},
		{m_size.x * VECTOR_SIZE, m_size.y * VECTOR_SIZE, 0.0f}
	};
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �T�C�Y�Ɖ�]
	D3DXVec3TransformCoord(&pVtx[0].pos, &sizePos[0], &mtx);
	D3DXVec3TransformCoord(&pVtx[1].pos, &sizePos[1], &mtx);
	D3DXVec3TransformCoord(&pVtx[2].pos, &sizePos[2], &mtx);
	D3DXVec3TransformCoord(&pVtx[3].pos, &sizePos[3], &mtx);

	// �ʒu
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCnt].pos.x += m_pos.x;
		pVtx[nCnt].pos.y += m_pos.y;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
