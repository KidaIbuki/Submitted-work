//=================================
// 
// �t�F�[�h�@fade.h
// Auther kida ibuki 
// 
//==================================
#ifndef _FADE_H_
#define _FADE_H_

#include "object2D.h"   // �I�u�W�F�N�g2D
#include "scene.h"       // �V�[��
class CFade :public CObject2D
{
public:
    //�t�F�[�h�̏��
    typedef enum
    {
        FADE_NONE = 0,
        FADE_IN,		//�t�F�[�h�C�����
        FADE_OUT,		//�t�F�[�h�A�E�g���
        FADE_MAX
    }FADE;

    CFade();			//�R���X�g���N�^
    ~CFade()override;			//�f�X�g���N�^
    HRESULT Init()override;		//�����ݒ�
    void Uninit()override;      //�I������
    void Update()override;      //�X�V����
    void Draw()override;		//�`�揈��
    static CFade* SetFade(CScene::MODE mode);
    static CFade* SetFade_Pos(D3DXVECTOR3 pos);
    bool GetNumUse() { return m_bUse; }
    void DeathFlag()override;

private:
    FADE m_Fade;    // �t�F�[�h�̏��
    D3DXCOLOR m_colorFade;    // �J���[
    CScene::MODE m_modeNext;  // ���̃��[�h
    bool m_bUse;         // �g�p����Ă��邩
    D3DXVECTOR3 m_pos;   // �ʒu
};

#endif