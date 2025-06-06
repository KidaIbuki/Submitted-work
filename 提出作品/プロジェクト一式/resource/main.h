//��������������������������
//
//���C�������imain.h)
//Author ibuki kida
// 
//��������������������������
#ifndef _MAIN_H_	//���̃}�N����`������ĂȂ�������
#define _MAIN_H_	//��d�C���N���[�h�h�~�̃}�N����`

#include "pch.h"

//���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;
}VERTEX_2D;

//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���
	D3DXVECTOR3 nor;	//�@���x�N�g��(����)
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_3D;

//=========================
//�}�N����`
//=========================
#define CLASS_NAME	"WindowClass"	//�E�B���h�E�N���X�̖��O
#define WINDOW_NAME	"Shader"	//�E�B���h�E�̖��O

int GetFPS();

#endif 

