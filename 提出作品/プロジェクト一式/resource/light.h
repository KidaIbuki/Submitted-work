//=================================
// 
//���C�g�N���X�@�@light.h
//Auther kida ibuki 
// 
//==================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"   // ���C��

//#define MAX_LIGHT (3) //���C�g�̐�
//#define VEC_DIR (D3DXVECTOR3(0.22f, -0.87f, 0.44f))    // 1�ڂ̃��C�g�̕����x�N�g���̐ݒ�
//#define VEC_DIR1 (D3DXVECTOR3(-0.18f, 0.88f, -0.44f))  // 2�ڂ̃��C�g�̕����x�N�g���̐ݒ�
//#define VEC_DIR2 (D3DXVECTOR3(0.89f, 0.11f, 0.44f))    // 3�ڂ̃��C�g�̕����x�N�g���̐ݒ�

class Light;  // �O���錾
//���C�g�̃N���X��`
class CLight
{
public:
	CLight();		    //�R���X�g���N�^
	~CLight();		    //�f�X�g���N�^
	HRESULT Init();     //�����ݒ�
	void Uninit();		//�I������
	void Update();      //�X�V����
	D3DXVECTOR3 GetLightDir() { return position; }

	void Draw();
private:
	//D3DLIGHT9 m_aLight[MAX_LIGHT];  //���C�g�\����
	D3DLIGHT9 light;
	Light* m_directionalLight;
	// ImGui�p���C�g�̕����ύX�ϐ�
	float vecX, vecY, vecZ;
	D3DXVECTOR3 position;
	LPD3DXMESH m_mesh;


};

// �e���C�g�Ăяo���֗��N���X
class Light
{
public:
	Light();
	~Light();
	void InitDirectional(const D3DXVECTOR3& direction, const D3DXCOLOR& color);   // �w�������C�g
	void InitPoint(const D3DXVECTOR3& position, const D3DXCOLOR& color, float range);  // �|�C���g���C�g
	void InitSpot(const D3DXVECTOR3& position, const D3DXVECTOR3& direction, const D3DXCOLOR& color, float range, float theta, float phi);  // �X�|�b�g���C�g

	void Enable(LPDIRECT3DDEVICE9 device, DWORD index);  // ���C�g�̗L����
	void Disable(LPDIRECT3DDEVICE9 device, DWORD index); // ���C�g�̖�����

	void SetPosition(const D3DXVECTOR3& pos);   // ���C�g�̈ʒu�ݒ�
	void SetDirection(const D3DXVECTOR3& dir);  // ���C�g�̕����ݒ�
	void DrawObjectWithLighting(LPDIRECT3DDEVICE9 device, LPD3DXMESH mesh, const D3DXVECTOR3 & position);  // ���C�g�I�u�W�F�N�g�`��
	void UpdateObj(float deltaTime, D3DXVECTOR3 center);

private:
	D3DLIGHT9 m_light;
	D3DXMATRIX world;

	float anglef;             // ���݂̊p�x�i���W�A���j
	float radius;     // ���a
	float speed;       // ��]�X�s�[�h�i���W�A��/�b�j
	//D3DXVECTOR3 center;  // ���S���W�i�~�̒��S�j

};

#endif