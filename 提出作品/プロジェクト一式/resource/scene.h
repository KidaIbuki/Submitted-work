//=================================
// 
// シーンクラス　scene.h
//Auther kida ibuki 
// 
//==================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "pch.h"  // プリコンパイル済みヘッダー
#include "object.h"  // オブジェクト

class CScene
{
public:
	//画面の種類
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_MAX,
	}MODE;

	CScene();			//コンストラクタ
	virtual ~CScene();			//デストラクタ
	virtual HRESULT Init();		//初期設定
	virtual void Uninit();      //終了処理
	virtual void Update();      //更新処理
	virtual void Draw();		//描画処理
	void SetTexture(std::string sPath);  // テクスチャの設定
	void UpdateVertex();
	static CScene* Create(MODE mode);   //生成
	void SetSize(float fWidth, float fHeight);//各テクスチャのサイズ設定
	void SetPos(float x, float y);			//各テクスチャの位置設定
	//CSaveObject* CreateSaveObject(CObject* pObject);
	//CSaveObject* CreateSaveObjectPause(CObject* pObject);   // ポーズ中のオブジェクト
	MODE GetMode() { return m_Mode; }
	int m_nTimerCnt;
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファ
	LPDIRECT3DTEXTURE9 m_pTexture;				//テクスチャ
private:
	MODE m_Mode;   // モードもの取得
	D3DXVECTOR2 m_size;
	D3DXVECTOR2 m_pos;

};

#endif