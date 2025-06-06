//=================================
// 
// �t�F�[�h�@fade.h
// Aouther kida ibuki 
// 
//==================================
#include "fade.h"     // �t�F�[�h
#include "manager.h"   // �}�l�[�W���[
#include "pch.h"       // �v���R���p�C���ς݃w�b�_�[

//====================================
// �R���X�g���N�^
//====================================
CFade::CFade()
{
	m_Fade = FADE_NONE;
	m_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_bUse = true;
	m_pos = { 0.0f,0.0f,0.0f };
	SetNormalDraw(true);
	
}
//====================================
// �f�X�g���N�^
//====================================
CFade::~CFade()
{
}
//====================================
// �����ݒ�
//====================================
HRESULT CFade::Init()
{
	CObject2D::Init();

	SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	return S_OK;
}
//====================================
// �I������
//====================================
void CFade::Uninit()
{
	CObject2D::Uninit();
}
//====================================
// �X�V����
//====================================
void CFade::Update()
{
	CObject2D::Update();
	CManager* pManager = CManager::GetInstance();

	if (m_Fade != FADE_NONE)
	{
		if (m_Fade == FADE_IN)
		{ //�t�F�[�h�C���̂Ƃ�
			m_colorFade.a -= 0.03f;

			if (m_colorFade.a <= 0.0f)
			{
				m_colorFade.a = 0.0f;
				m_Fade = FADE_NONE;
			}
		}

		else if (m_Fade == FADE_OUT)
		{ //�t�F�[�h�A�E�g�̂Ƃ�
			m_colorFade.a += 0.03f;

			if (m_colorFade.a >= 1.0f)
			{
				m_colorFade.a = 1.0f;
				m_Fade = FADE_IN;

				pManager->SetMode(m_modeNext);
				pManager->GetCamera()->SetPos(m_pos);
			}

		}

		VERTEX_2D* pVtx; //���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, m_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, m_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, m_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, m_colorFade.a);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();

	}
}
//====================================
// �`�揈��
//====================================
void CFade::Draw()
{
	CObject2D::Draw();
}
//=================================
// �t���O�𗧂Ă�
//=================================
void CFade::DeathFlag()
{
	m_bUse = true;
	CObject::DeathFlag();
}
//=================================
// �t�F�[�h�̐ݒ�
//=================================
CFade* CFade::SetFade(CScene::MODE mode)
{
	CFade* pFade = new CFade();
	pFade->Init();

	pFade->m_modeNext = mode;
	pFade->m_Fade = FADE_OUT;
	return pFade;
}
//=================================
// �t�F�[�h�Ŏw��̈ʒu�ɖ߂�
//=================================
CFade* CFade::SetFade_Pos(D3DXVECTOR3 pos)
{
	CFade* pFade = new CFade();
	pFade->Init();
	pFade->m_pos = pos;
	pFade->m_Fade = FADE_OUT;

	return pFade;
}
