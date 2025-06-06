//=================================
// 
//カメラクラス　camera.cpp
//Auther kida ibuki 
// 
//==================================
#include "camera.h"   // カメラ
#include "manager.h"    // マネージャー
#include "title.h"     // タイトル
//==============================
//コンストラクタ
//==============================
CCamera::CCamera()
{
}
//==============================
//デストラクタ
//==============================
CCamera::~CCamera()
{
}
//==============================
//初期設定
//==============================
HRESULT CCamera::Init()
{

	//メンバ変数初期化
	m_posV = PERSPECTIVE;			        // 視点
	m_posR = POINT_OF_INTEREST;		        // 注視点
	m_vecU = { 0.0f, 1.0f, 0.0f };				// 上方向
	m_rot = { 0.0f, 0.0f, 0.0f };
    m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	m_fDistance = sqrtf(DISTANCE_LEFT * DISTANCE_LEFT + -DISTANCE_RIGHT * -DISTANCE_RIGHT);		//距離
	m_fScaleDis = SCALEDIS;	//倍率

	m_nShakeFrame = 0;
	m_nShakeVolume = 0;

	m_move = { 0.0f,0.0f,0.0f };
	m_rotMove = { 0.0f,0.0f,0.0f };
	m_nFrame = 10;
	m_nCntFrame = 0;

	return S_OK;

}
//==============================
//終了処理
//==============================
void CCamera::Uninit()
{
}
//==============================
//更新処理
//==============================
void CCamera::Update()
{
	CManager* pManager = CManager::GetInstance();
	CInputMouse* pMouse = pManager->GetMouse();
	CInputKeyboard* pKeyboard = pManager->GetKeyboard();    //キーボード取得

	//m_rot.y += 0.02f;

	//注視点を中心に公転
	m_posV.x = sinf(D3DX_PI + m_rot.y) * m_fDistance * m_fScaleDis;
	m_posV.z = cosf(D3DX_PI + m_rot.y) * m_fDistance * m_fScaleDis;
	//m_posV.y = cosf(D3DX_PI + m_rot.z) * m_fDistance * m_fScaleDis;

	if (pKeyboard->GetPress(DIK_C))
	{
		m_posR.y += 0.2f;
	}
	if (pKeyboard->GetPress(DIK_F))
	{//Aキーが押された場合
		m_rot.y -= 0.02f;

	}
	if (pKeyboard->GetPress(DIK_G))
	{//Dキーが押された場合

		m_rot.y += 0.02f;
	}

}
//==============================
//カメラ設定
//==============================
void CCamera::SetCamera()
{
	CManager* pManager = CManager::GetInstance();

	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();		// デバイスを取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);


	// プロジェクションマトリックスを作成(透視投影)
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,		//near
		5000.0f);   //far


	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	//D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	// カメラの揺れ
	D3DXVECTOR3 adjust = { 0.0f,0.0f,0.0f };
	if (m_nShakeFrame > 0)
	{
		m_nShakeFrame--;
		adjust.x = (float)(rand() % m_nShakeVolume);
		adjust.y = (float)(rand() % m_nShakeVolume);
		//adjust.z = random;
	}
	D3DXVECTOR3 posR, posV;
	posR = m_posR + adjust;  // 注視点
	posV = m_posV + adjust;  // 視点
	D3DXMatrixLookAtLH(&m_mtxView, &posV, &posR, &m_vecU);
	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

}
//==============================
// カメラのリセット
//==============================
void CCamera::ResetCamera()
{
	m_posV = PERSPECTIVE;			        // 視点
	m_posR = POINT_OF_INTEREST;		        // 注視点
	m_vecU = { 0.0f, 1.0f, 0.0f };			// 上方向
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	m_fDistance = sqrtf(DISTANCE_LEFT * DISTANCE_LEFT + -DISTANCE_RIGHT * -DISTANCE_RIGHT);		//距離
	m_fScaleDis = SCALEDIS;	//倍率

}
//==============================
// タイトル画面のカメラ
//==============================
void CCamera::TitleCamera()
{
	m_posV = PERSPECTIVE;			        // 視点
	m_posR = POINT_OF_INTEREST;		        // 注視点
	m_vecU = { 0.0f, 1.0f, 0.0f };			// 上方向
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	m_fDistance = sqrtf(DISTANCE_LEFT * DISTANCE_LEFT + -DISTANCE_RIGHT * -DISTANCE_RIGHT);		//距離
	m_fScaleDis = 1.0f;	//倍率

}
//==============================
// カメラの調節用
//==============================
void CCamera::DebugCamera()
{
	m_posV = { 0.0f, 200.0f, -450.0f };			// 視点
	m_posR = { 0.0f, 0.0f, 40.0f };		        // 注視点
	m_vecU = { 0.0f, 1.0f, 0.0f };				// 上方向
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);

	m_fDistance = sqrtf(100 * 100 + -250 * -250);		//距離
	m_fScaleDis = 1.0f;	//倍率

}
//==============================
// カメラの位置設定
//==============================
void CCamera::SetPos(D3DXVECTOR3 pos)
{
	m_posR = pos;
}
//==============================
// カメラの揺れの設定
//==============================
void CCamera::SetShake(int shakeFrame, float shakeVolume)
{
	m_nShakeFrame = shakeFrame;
	m_nShakeVolume = (int)shakeVolume;
}

