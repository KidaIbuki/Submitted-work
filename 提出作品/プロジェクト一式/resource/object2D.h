//=================================
// 
// オブジェクト2Dクラス　object2D.h
// Auther kida ibuki 
// 
//==================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "object.h"   // オブジェクト
#include "pch.h"          // プリコンパイル済みヘッダー

#define VECTOR_SIZE (0.5f)  // 位置の更新vector用(変える必要なし)

//2Dオブジェクト管理クラスの定義
class CObject2D :public CObject
{
public:
	CObject2D();		//コンストラクタ
	~CObject2D() override;		//デストラクタ
	HRESULT Init() override;	//初期設定
	void Uninit() override;		//終了処理
	void Update() override;		//更新処理
	void Draw() override;		//描画処理
	void SetAnim(int nDivide, int nTime);	//アニメーション設定
	void SetPos(D3DXVECTOR2 pos);			//各オブジェクトの位置設定
	void SetSize(float fWidth, float fHeight);//各オブジェクトのサイズ設定

	//=======================
	void SetAngle(float fAngle);			//向きを設定
	void SetRot(D3DXVECTOR2 rot) { m_rot = rot; }	//向きを設定2
	void SetColor(D3DXCOLOR col); // カラー設定  赤（Max1.0ｆ）,緑（Max1.0ｆ）, 青（Max1.0ｆ）, α（Max1.0ｆ）

	//========テクスチャの設定========
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	//テクスチャの設定(一つ目)
	void SetTexture(std::string sPath);
	//==========各情報を取得============
	D3DXVECTOR2 GetPos() { return m_pos; }			//位置の取得
	D3DXVECTOR2 GetSize() { return m_size; }		//サイズの取得
	float GetAngle() { return m_fAngle; }			//向きの取得
	D3DXVECTOR2 GetRot() { return m_rot; }			//向きを取得
	//=================================
	D3DXVECTOR2 m_pos;							//位置

	D3DXVECTOR2 m_size;							//サイズ
	D3DXVECTOR2 m_rot;							//向き2

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTexture;				//テクスチャ

private:
	void UpdateAnim();		//アニメーションの更新
	void UpdateVertex();	//位置の更新

	float m_fAngle;								//向き
	float m_fHeight;							//高さ
	float m_fWidth;								//幅

	// アニメーション用メンバ変数
	int m_nDivide;								//分割数
	int m_nTime;								//時間
	DWORD m_nextTime;							//アニメーションを進める
	int m_nAnimIdx;								//アニメーションカウント用
	bool m_bReverse;							
};

#endif