//=================================
// 
//�J�����N���X�@camera.h
//Auther kida ibuki 
// 
//==================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"   // ���C��
#include "scene.h"   // �V�[��

// �J�����p�}�N����`
#define PERSPECTIVE (D3DXVECTOR3(0.0f,100.0f,-100.0f))  // ���_��Z��
#define POINT_OF_INTEREST (D3DXVECTOR3(0.0f,0.0f,-100.0f)) // �����_��Z��
#define DISTANCE_LEFT (100)     // �����̍���(sqrtf�̂��)
#define DISTANCE_RIGHT (20)    // �����̉E��(sqrtf�̂��)
#define SCALEDIS (2.0f)  // �����̔{��
//�O���錾
class CObjectX;

//�J�����N���X�̒�`
class CCamera
{
public:
	CCamera();					//�R���X�g���N�^
	~CCamera();					//�f�X�g���N�^
	HRESULT Init();				//�����ݒ�
	void Uninit();				//�I������
	void Update();				//�X�V����
	void SetCamera();			//�J�����ݒ�

	void ResetCamera();
	void TitleCamera();
	void DebugCamera(); // �J�����ʒu���ߗp
	D3DXVECTOR3 GetRot() { return m_rot; }			//�������擾
	D3DXVECTOR3 GetPosR() { return m_posR; }		//�����_�̎擾
	D3DXVECTOR3 GetPosV() { return m_posV; }		//���_�̎擾

	void SetPos(D3DXVECTOR3 pos);
	void SetShake(int shakeFrame, float shakeVolume);

	D3DXMATRIX GetViewProj() { return m_mtxViewProj = m_mtxView * m_mtxProjection; }  // �J�����̃r���[�v���W�F�N�V�������擾
	D3DXMATRIX GetView() { return m_mtxView; }
	D3DXMATRIX GetProj() { return m_mtxProjection; }

private:
	D3DXVECTOR3 m_posV;			// ���_
	D3DXVECTOR3 m_posR;         // �����_
	D3DXVECTOR3 m_vecU;			// �����
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 m_move;         // �ړ���
	D3DXMATRIX m_mtxProjection;	// �v���W�F�N�V�����s��
	D3DXMATRIX m_mtxView;		// �r���[�s��
	float m_fScaleDis;
	float m_fDistance;		// ����
	D3DXVECTOR3 m_rotMove;  // rot�p��move
	
	int m_nFrame;         // ������ς���t���[��
	int m_nCntFrame;      // ��̃t���[�����J�E���g����

	int m_nShakeFrame;     // �h�炷�t���[��
	int m_nShakeVolume;  // �h��̗�

	D3DXMATRIX m_mtxViewProj;
};

#endif
