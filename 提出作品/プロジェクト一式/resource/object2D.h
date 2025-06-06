//=================================
// 
// �I�u�W�F�N�g2D�N���X�@object2D.h
// Auther kida ibuki 
// 
//==================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "object.h"   // �I�u�W�F�N�g
#include "pch.h"          // �v���R���p�C���ς݃w�b�_�[

#define VECTOR_SIZE (0.5f)  // �ʒu�̍X�Vvector�p(�ς���K�v�Ȃ�)

//2D�I�u�W�F�N�g�Ǘ��N���X�̒�`
class CObject2D :public CObject
{
public:
	CObject2D();		//�R���X�g���N�^
	~CObject2D() override;		//�f�X�g���N�^
	HRESULT Init() override;	//�����ݒ�
	void Uninit() override;		//�I������
	void Update() override;		//�X�V����
	void Draw() override;		//�`�揈��
	void SetAnim(int nDivide, int nTime);	//�A�j���[�V�����ݒ�
	void SetPos(D3DXVECTOR2 pos);			//�e�I�u�W�F�N�g�̈ʒu�ݒ�
	void SetSize(float fWidth, float fHeight);//�e�I�u�W�F�N�g�̃T�C�Y�ݒ�

	//=======================
	void SetAngle(float fAngle);			//������ݒ�
	void SetRot(D3DXVECTOR2 rot) { m_rot = rot; }	//������ݒ�2
	void SetColor(D3DXCOLOR col); // �J���[�ݒ�  �ԁiMax1.0���j,�΁iMax1.0���j, �iMax1.0���j, ���iMax1.0���j

	//========�e�N�X�`���̐ݒ�========
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//�e�N�X�`���̐ݒ�(���)
	void SetTexture(std::string sPath);
	//==========�e�����擾============
	D3DXVECTOR2 GetPos() { return m_pos; }			//�ʒu�̎擾
	D3DXVECTOR2 GetSize() { return m_size; }		//�T�C�Y�̎擾
	float GetAngle() { return m_fAngle; }			//�����̎擾
	D3DXVECTOR2 GetRot() { return m_rot; }			//�������擾
	//=================================
	D3DXVECTOR2 m_pos;							//�ʒu

	D3DXVECTOR2 m_size;							//�T�C�Y
	D3DXVECTOR2 m_rot;							//����2

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_pTexture;				//�e�N�X�`��

private:
	void UpdateAnim();		//�A�j���[�V�����̍X�V
	void UpdateVertex();	//�ʒu�̍X�V

	float m_fAngle;								//����
	float m_fHeight;							//����
	float m_fWidth;								//��

	// �A�j���[�V�����p�����o�ϐ�
	int m_nDivide;								//������
	int m_nTime;								//����
	DWORD m_nextTime;							//�A�j���[�V������i�߂�
	int m_nAnimIdx;								//�A�j���[�V�����J�E���g�p
	bool m_bReverse;							
};

#endif