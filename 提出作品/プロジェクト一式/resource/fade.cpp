//=================================
// 
// フェード　fade.h
// Aouther kida ibuki 
// 
//==================================
#include "fade.h"     // フェード
#include "manager.h"   // マネージャー
#include "pch.h"       // プリコンパイル済みヘッダー

//====================================
// コンストラクタ
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
// デストラクタ
//====================================
CFade::~CFade()
{
}
//====================================
// 初期設定
//====================================
HRESULT CFade::Init()
{
	CObject2D::Init();

	SetPos(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
	SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	return S_OK;
}
//====================================
// 終了処理
//====================================
void CFade::Uninit()
{
	CObject2D::Uninit();
}
//====================================
// 更新処理
//====================================
void CFade::Update()
{
	CObject2D::Update();
	CManager* pManager = CManager::GetInstance();

	if (m_Fade != FADE_NONE)
	{
		if (m_Fade == FADE_IN)
		{ //フェードインのとき
			m_colorFade.a -= 0.03f;

			if (m_colorFade.a <= 0.0f)
			{
				m_colorFade.a = 0.0f;
				m_Fade = FADE_NONE;
			}
		}

		else if (m_Fade == FADE_OUT)
		{ //フェードアウトのとき
			m_colorFade.a += 0.03f;

			if (m_colorFade.a >= 1.0f)
			{
				m_colorFade.a = 1.0f;
				m_Fade = FADE_IN;

				pManager->SetMode(m_modeNext);
				pManager->GetCamera()->SetPos(m_pos);
			}

		}

		VERTEX_2D* pVtx; //頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, m_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, m_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, m_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, m_colorFade.a);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();

	}
}
//====================================
// 描画処理
//====================================
void CFade::Draw()
{
	CObject2D::Draw();
}
//=================================
// フラグを立てる
//=================================
void CFade::DeathFlag()
{
	m_bUse = true;
	CObject::DeathFlag();
}
//=================================
// フェードの設定
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
// フェードで指定の位置に戻す
//=================================
CFade* CFade::SetFade_Pos(D3DXVECTOR3 pos)
{
	CFade* pFade = new CFade();
	pFade->Init();
	pFade->m_pos = pos;
	pFade->m_Fade = FADE_OUT;

	return pFade;
}
