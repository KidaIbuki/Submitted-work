//=================================
// 
// �^�C�g���N���X�@title.h
// Auther kida ibuki 
// 
//==================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"  // �V�[��
#include <string>


class CTitle:public CScene
{
public:
    CTitle();			//�R���X�g���N�^
    ~CTitle()override;			//�f�X�g���N�^
    HRESULT Init()override;		//�����ݒ�
    void Uninit()override;      //�I������
    void Update()override;      //�X�V����

    void Draw()override;		//�`�揈��
    static CTitle* Create();     //����
private:
};


#endif