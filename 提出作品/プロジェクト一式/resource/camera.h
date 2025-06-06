//=================================
// 
//カメラクラス　camera.h
//Auther kida ibuki 
// 
//==================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"   // メイン
#include "scene.h"   // シーン

// カメラ用マクロ定義
#define PERSPECTIVE (D3DXVECTOR3(0.0f,100.0f,-100.0f))  // 視点のZ軸
#define POINT_OF_INTEREST (D3DXVECTOR3(0.0f,0.0f,-100.0f)) // 注視点のZ軸
#define DISTANCE_LEFT (100)     // 距離の左辺(sqrtfのやつ)
#define DISTANCE_RIGHT (20)    // 距離の右辺(sqrtfのやつ)
#define SCALEDIS (2.0f)  // 距離の倍率
//前方宣言
class CObjectX;

//カメラクラスの定義
class CCamera
{
public:
	CCamera();					//コンストラクタ
	~CCamera();					//デストラクタ
	HRESULT Init();				//初期設定
	void Uninit();				//終了処理
	void Update();				//更新処理
	void SetCamera();			//カメラ設定

	void ResetCamera();
	void TitleCamera();
	void DebugCamera(); // カメラ位置調節用
	D3DXVECTOR3 GetRot() { return m_rot; }			//向きを取得
	D3DXVECTOR3 GetPosR() { return m_posR; }		//注視点の取得
	D3DXVECTOR3 GetPosV() { return m_posV; }		//視点の取得

	void SetPos(D3DXVECTOR3 pos);
	void SetShake(int shakeFrame, float shakeVolume);

	D3DXMATRIX GetViewProj() { return m_mtxViewProj = m_mtxView * m_mtxProjection; }  // カメラのビュープロジェクションを取得
	D3DXMATRIX GetView() { return m_mtxView; }
	D3DXMATRIX GetProj() { return m_mtxProjection; }

private:
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;         // 注視点
	D3DXVECTOR3 m_vecU;			// 上方向
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_move;         // 移動量
	D3DXMATRIX m_mtxProjection;	// プロジェクション行列
	D3DXMATRIX m_mtxView;		// ビュー行列
	float m_fScaleDis;
	float m_fDistance;		// 距離
	D3DXVECTOR3 m_rotMove;  // rot用のmove
	
	int m_nFrame;         // 向きを変えるフレーム
	int m_nCntFrame;      // 上のフレームをカウントする

	int m_nShakeFrame;     // 揺らすフレーム
	int m_nShakeVolume;  // 揺れの量

	D3DXMATRIX m_mtxViewProj;
};

#endif
