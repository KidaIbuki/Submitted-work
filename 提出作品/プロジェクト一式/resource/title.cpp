//=================================
// 
// シーンクラス　scene.cpp
// Auther kida ibuki 
// 
//==================================
#include "title.h"     // タイトル
#include "fade.h"      // フェード
#include "manager.h"    // マネージャー
#include "blockX.h"   // ブロック

#include "ToneAdjust.h"
#include "EffectAccumulation.h"
#include "WaterSurface.h"

//====================================
// コンストラクタ
//====================================
CTitle::CTitle()
{
}
//====================================
// デストラクタ
//====================================
CTitle::~CTitle()
{
}
//====================================
// 初期設定
//====================================
HRESULT CTitle::Init()
{
	//CManager* pManager = CManager::GetInstance();

	CManager* pManager = CManager::GetInstance();
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	CScene::Init();
	
	CBlockX::Create(pDevice, D3DXVECTOR3(60.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));  // トゥーン
	ToneAdjust::Create(D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));  // 色調変化
	EffectAccumulation::Create();  // 炎シェーダー



	Wave::Create();  // フレネル効果付きの波シェーダー

	return S_OK;
}
//====================================
// 終了処理
//====================================
void CTitle::Uninit()
{

	CScene::Uninit();
	CObject* pTopObject[NUM_MAX_PRIORITY];
	CObject::GetObj(pTopObject);
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		//オブジェクト取得
		CObject* pObj = pTopObject[nPriority];
		// 最大数が不明なのでwhileを使用
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();

			pObj->DeathFlag();

			pObj = pNext;

		}
	}

}
//====================================
// 更新処理
//====================================
void CTitle::Update()
{
	CScene::Update();

	CObject* pTopObject[NUM_MAX_PRIORITY];
	CObject::GetObj(pTopObject);
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		//オブジェクト取得
		CObject* pObj = pTopObject[nPriority];
		// 最大数が不明なのでwhileを使用
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();

			pObj->Update();

			pObj = pNext;

		}
	}

}

//====================================
// 描画処理
//====================================
void CTitle::Draw()
{
	CObject* pTopObject[NUM_MAX_PRIORITY];
	CObject::GetObj(pTopObject);
	for (int nPriority = 0; nPriority < NUM_MAX_PRIORITY; nPriority++)
	{
		//オブジェクト取得
		CObject* pObj = pTopObject[nPriority];
		// 最大数が不明なのでwhileを使用
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();

			pObj->Draw();

			pObj = pNext;

		}
	}


}

//======================================
// タイトルの生成
//======================================
CTitle* CTitle::Create()
{
	CTitle* pTitle = new CTitle();
	pTitle->Init();

	return pTitle;
}