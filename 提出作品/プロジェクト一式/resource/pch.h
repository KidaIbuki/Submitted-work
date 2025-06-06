//=================================
// 
//�v���R���p�C���ς݃w�b�_�[�@pch.h
//Auther kida ibuki 
// 
//==================================
#ifndef _PCH_H_
#define _PCH_H_

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include <cassert>		//assert�g������
#include <windows.h>	// Windows�A�v���P�[�V�����p
#include <d3dx9.h>		// DirectX9�p

#define DIRECTINPUT_VERSION	(0x0800)	// DirectInput�̃o�[�W����
#include <dinput.h>					    // DirectInput�p
#include <xinput.h>					    // �W���C�p�b�h�����ɕK�v
#include <xaudio2.h>				    // �T�E���h�����ɕK�v
#include <string>						//str���g������(texture�Ȃǂ̃t�@�C���p�X����������)
#include <mmsystem.h>
#include "time.h"                       // �����ɕK�v
#include <cmath>                        // sqrt sin cos
#include <algorithm>                    // max,min���g�p����̂ɕK�v
#include <cmath>                        // std::time
#include <cstdlib>                      // std::rand std::srand
#include <vector>
#include <string>
#include <memory>                       // �X�}�[�g�|�C���^�ȂǂɕK�v
#include <strsafe.h>
//=====================================================
// ���C�u�����̃����N
//=====================================================
#pragma	comment(lib,"d3d9.lib")		// �`�揈���ɕK�v
#pragma	comment(lib,"d3dx9.lib")	// [d3d9.lib]�̊g�����C�u����
#pragma	comment(lib,"dxguid.lib")	// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma	comment(lib,"dinput8.lib")	// ���͏����ɕK�v
#pragma comment(lib,"xinput.lib")	// �W���C�p�b�h�����ɕK�v
#pragma comment(lib, "winmm.lib")	// �V�X�e�������擾�ɕK�v

//=====================================================
// �}�N����`
//=====================================================

// ���_�t�H�[�}�b�g
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)			//2D�̂��
#define FVF_VERTEX_3D   (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//3D�p�̂��

// �E�B���h�E�T�C�Y
#define SCREEN_WIDTH	(1280)	    //�E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)		//�E�C���h�E�̍���

#define POSITION1 (7000.0f)    // �ʒu�͈̔�1
#define POSITION2 (7010.0f)    // �J�����̈ʒu�͈�
#define POSITION3 (13000.0f)   // �ʒu�͈̔�2
#define POSITION4 (13010.0f)   // �J�����̈ʒu�͈�2
#define POSITION5 (7003.0f)    // �{�X�̏o���͈�
#define POSITION6 (6480.0f)    // ���������J�n�ʒu

using namespace std;
//=====================================================
// �\���̒�`
//=====================================================

//���f���t�@�C���̓ǂݍ��ݍ\����
typedef struct
{
	int nType;
	char cFile[256];		//�t�@�C����ǂݍ���
}MODEL_FILENAME;

// �G�̂��
typedef struct
{
	int nType;
	char cFile[256];		//�t�@�C����ǂݍ���
}ENEMY_FILENAME;

#endif