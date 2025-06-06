//=================================
// 
//�J�����N���X�@camera.cpp
//Auther kida ibuki 
// 
//==================================
#include "camera.h"   // �J����
#include "manager.h"    // �}�l�[�W���[
#include "title.h"     // �^�C�g��
//==============================
//�R���X�g���N�^
//==============================
CCamera::CCamera()
{
}
//==============================
//�f�X�g���N�^
//==============================
CCamera::~CCamera()
{
}
//==============================
//�����ݒ�
//==============================
HRESULT CCamera::Init()
{

	//�����o�ϐ�������
	m_posV = PERSPECTIVE;			        // ���_
	m_posR = POINT_OF_INTEREST;		        // �����_
	m_vecU = { 0.0f, 1.0f, 0.0f };				// �����
	m_rot = { 0.0f, 0.0f, 0.0f };
    m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	m_fDistance = sqrtf(DISTANCE_LEFT * DISTANCE_LEFT + -DISTANCE_RIGHT * -DISTANCE_RIGHT);		//����
	m_fScaleDis = SCALEDIS;	//�{��

	m_nShakeFrame = 0;
	m_nShakeVolume = 0;

	m_move = { 0.0f,0.0f,0.0f };
	m_rotMove = { 0.0f,0.0f,0.0f };
	m_nFrame = 10;
	m_nCntFrame = 0;

	return S_OK;

}
//==============================
//�I������
//==============================
void CCamera::Uninit()
{
}
//==============================
//�X�V����
//==============================
void CCamera::Update()
{
	CManager* pManager = CManager::GetInstance();
	CInputMouse* pMouse = pManager->GetMouse();
	CInputKeyboard* pKeyboard = pManager->GetKeyboard();    //�L�[�{�[�h�擾

	//m_rot.y += 0.02f;

	//�����_�𒆐S�Ɍ��]
	m_posV.x = sinf(D3DX_PI + m_rot.y) * m_fDistance * m_fScaleDis;
	m_posV.z = cosf(D3DX_PI + m_rot.y) * m_fDistance * m_fScaleDis;
	//m_posV.y = cosf(D3DX_PI + m_rot.z) * m_fDistance * m_fScaleDis;

	if (pKeyboard->GetPress(DIK_C))
	{
		m_posR.y += 0.2f;
	}
	if (pKeyboard->GetPress(DIK_F))
	{//A�L�[�������ꂽ�ꍇ
		m_rot.y -= 0.02f;

	}
	if (pKeyboard->GetPress(DIK_G))
	{//D�L�[�������ꂽ�ꍇ

		m_rot.y += 0.02f;
	}

}
//==============================
//�J�����ݒ�
//==============================
void CCamera::SetCamera()
{
	CManager* pManager = CManager::GetInstance();

	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();		// �f�o�C�X���擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);


	// �v���W�F�N�V�����}�g���b�N�X���쐬(�������e)
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,		//near
		5000.0f);   //far


	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	//D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// �J�����̗h��
	D3DXVECTOR3 adjust = { 0.0f,0.0f,0.0f };
	if (m_nShakeFrame > 0)
	{
		m_nShakeFrame--;
		adjust.x = (float)(rand() % m_nShakeVolume);
		adjust.y = (float)(rand() % m_nShakeVolume);
		//adjust.z = random;
	}
	D3DXVECTOR3 posR, posV;
	posR = m_posR + adjust;  // �����_
	posV = m_posV + adjust;  // ���_
	D3DXMatrixLookAtLH(&m_mtxView, &posV, &posR, &m_vecU);
	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

}
//==============================
// �J�����̃��Z�b�g
//==============================
void CCamera::ResetCamera()
{
	m_posV = PERSPECTIVE;			        // ���_
	m_posR = POINT_OF_INTEREST;		        // �����_
	m_vecU = { 0.0f, 1.0f, 0.0f };			// �����
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	m_fDistance = sqrtf(DISTANCE_LEFT * DISTANCE_LEFT + -DISTANCE_RIGHT * -DISTANCE_RIGHT);		//����
	m_fScaleDis = SCALEDIS;	//�{��

}
//==============================
// �^�C�g����ʂ̃J����
//==============================
void CCamera::TitleCamera()
{
	m_posV = PERSPECTIVE;			        // ���_
	m_posR = POINT_OF_INTEREST;		        // �����_
	m_vecU = { 0.0f, 1.0f, 0.0f };			// �����
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	m_fDistance = sqrtf(DISTANCE_LEFT * DISTANCE_LEFT + -DISTANCE_RIGHT * -DISTANCE_RIGHT);		//����
	m_fScaleDis = 1.0f;	//�{��

}
//==============================
// �J�����̒��ߗp
//==============================
void CCamera::DebugCamera()
{
	m_posV = { 0.0f, 200.0f, -450.0f };			// ���_
	m_posR = { 0.0f, 0.0f, 40.0f };		        // �����_
	m_vecU = { 0.0f, 1.0f, 0.0f };				// �����
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	m_fDistance = sqrtf(100 * 100 + -250 * -250);		//����
	m_fScaleDis = 1.0f;	//�{��

}
//==============================
// �J�����̈ʒu�ݒ�
//==============================
void CCamera::SetPos(D3DXVECTOR3 pos)
{
	m_posR = pos;
}
//==============================
// �J�����̗h��̐ݒ�
//==============================
void CCamera::SetShake(int shakeFrame, float shakeVolume)
{
	m_nShakeFrame = shakeFrame;
	m_nShakeVolume = (int)shakeVolume;
}

