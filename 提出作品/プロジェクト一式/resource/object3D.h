//=================================
// 
// �I�u�W�F�N�g3D�N���X�@object3D.h
// Auther kida ibuki 
// 
//==================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "object.h"   // �I�u�W�F�N�g


#define NUM_OBJECT (10)
#define MOVE_SPD_TEX	(0.0005f)

class CObject3D :public CObject
{
public:
	CObject3D();		//�R���X�g���N�^
	~CObject3D() override;		//�f�X�g���N�^
	HRESULT Init() override;	//�����ݒ�
	void Uninit() override;		//�I������
	void Update() override;		//�X�V����
	void Draw() override;		//�`�揈��
	static CObject3D* Create();		//�I�u�W�F�N�g�̐���

	//**===========�e�N�X�`���̐ݒ�=============**
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//�e�N�X�`���̐ݒ�
	//================�e�I�u�W�F�N�g�ݒ�==================
	D3DXVECTOR3 GetPos() { return m_pos; }			//�ʒu�̎擾
	D3DXVECTOR3 GetRot() { return m_rot; }			//�����̎擾

	void SetPos(D3DXVECTOR3 pos);			//�e�I�u�W�F�N�g�̈ʒu�ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }			//�e�I�u�W�F�N�g�̌����ݒ�

	void SetField(float fWidth, float fDepth);
	void SetAnim(int nDivide, int nTime);	//�A�j���[�V�����ݒ�
	D3DXVECTOR3 m_pos;							//�ʒu
	D3DXVECTOR3 m_rot;							//����
private:
	void UpdateAnim();		//�A�j���[�V�����̍X�V

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pTexture;				//�e�N�X�`��
	D3DXMATRIX m_mtxWorld;						//���[���h�ϊ��s��
	D3DXVECTOR2 m_PosTexV;                           // �w�i�X�N���[���ϐ�

	// �A�j���[�V�����p�����o�ϐ�
	int m_nDivide;								//������
	int m_nTime;								//����
	DWORD m_nextTime;							//�A�j���[�V������i�߂�
	int m_nAnimIdx;								//�A�j���[�V�����J�E���g�p
	bool m_bReverse;

};

#endif