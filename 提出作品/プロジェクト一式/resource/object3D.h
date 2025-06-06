//=================================
// 
// オブジェクト3Dクラス　object3D.h
// Auther kida ibuki 
// 
//==================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "object.h"   // オブジェクト


#define NUM_OBJECT (10)
#define MOVE_SPD_TEX	(0.0005f)

class CObject3D :public CObject
{
public:
	CObject3D();		//コンストラクタ
	~CObject3D() override;		//デストラクタ
	HRESULT Init() override;	//初期設定
	void Uninit() override;		//終了処理
	void Update() override;		//更新処理
	void Draw() override;		//描画処理
	static CObject3D* Create();		//オブジェクトの生成

	//**===========テクスチャの設定=============**
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//テクスチャの設定
	//================各オブジェクト設定==================
	D3DXVECTOR3 GetPos() { return m_pos; }			//位置の取得
	D3DXVECTOR3 GetRot() { return m_rot; }			//向きの取得

	void SetPos(D3DXVECTOR3 pos);			//各オブジェクトの位置設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }			//各オブジェクトの向き設定

	void SetField(float fWidth, float fDepth);
	void SetAnim(int nDivide, int nTime);	//アニメーション設定
	D3DXVECTOR3 m_pos;							//位置
	D3DXVECTOR3 m_rot;							//向き
private:
	void UpdateAnim();		//アニメーションの更新

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTexture;				//テクスチャ
	D3DXMATRIX m_mtxWorld;						//ワールド変換行列
	D3DXVECTOR2 m_PosTexV;                           // 背景スクロール変数

	// アニメーション用メンバ変数
	int m_nDivide;								//分割数
	int m_nTime;								//時間
	DWORD m_nextTime;							//アニメーションを進める
	int m_nAnimIdx;								//アニメーションカウント用
	bool m_bReverse;

};

#endif