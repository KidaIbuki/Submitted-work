//=================================
// 
// オブジェクトクラス　object.h
// Auther kida ibuki 
// 
//==================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"   // メイン
#include "pch.h"    // プリコンパイル済みヘッダー

#define NUM_MAX_PRIORITY (8) // プライオリティの最大数


//全オブジェクトの基底クラス
class CObject
{
public:
	// オブジェクトタイプ
	typedef enum
	{
		NONE = 0,	         // 無し
		BLOCK,		         // 配置オブジェクト
		BLOCK2,		         // 配置オブジェクト
		CAMERA,              // カメラ
		PAUSE,               // ポーズ
		PLAYER,
		LIGHT,
		MAX
	}TYPE;

	CObject();		// コンストラクタ
	CObject(int nPriority);		// コンストラクタ オーバーライド

	virtual ~CObject();		// デストラクタ
	virtual HRESULT Init() = 0;		// 純粋仮想関数の初期設定
	virtual void Uninit() = 0;		// 純粋仮想関数の終了処理
	virtual void Update() = 0;		// 純粋仮想関数の更新処理
	virtual void Draw() = 0;		// 純粋仮想関数の描画処理
	static void ReleaseAll();		// 全オブジェクトの解放
	static void UpdateAll();		// 全オブジェクトの更新
	static void DrawAll();			// 全オブジェクトの描画
	static void DeathAll();         // 全オブジェクト破棄

	void SetNormalUpdate(bool bNormalUpdate) { m_bNormalUpdate = bNormalUpdate; }   // 全体で更新するか設定
	void SetPauseUpdate(bool bPauseUpdate) { m_bPauseUpdate = bPauseUpdate; }       // ポーズ中で更新するか設定
	void SetNormalDraw(bool bNormalDraw) { m_bNormalDraw = bNormalDraw; }   // 全体で描画するか設定
	void SetPauseDraw(bool bPauseDraw) { m_bPauseDraw = bPauseDraw; }   // ポーズ中で描画するか設定


	bool IsNormalUpdate() { return m_bNormalUpdate; }   // 全体で更新
	bool IsPauseUpdate() { return m_bPauseUpdate; }  // ポーズ中で更新
	bool IsNormalDraw() { return m_bNormalDraw; }     // 全体で描画
	bool isPauseDraw() { return m_bPauseDraw; }    // 全体で描画

	static CObject* GetObj(int nPriority);						// オブジェクト取得
	static void GetObj(CObject* pObject[NUM_MAX_PRIORITY]);						// オブジェクト取得

	TYPE GetType() { return m_type; }		// タイプ取得
	CObject* GetNext() { return m_pNext; }  // 次のオブジェクトを取得
	void SetType(TYPE type);				// タイプ設定

	void Release();					// 自分自身の解放
	bool m_bDeath;

	virtual void DeathFlag();
private:
	TYPE m_type;

	//==========リスト構造==============================================

	static CObject* m_pTop[NUM_MAX_PRIORITY];  // 先頭のオブジェクトのポインタ
	static CObject* m_pCur[NUM_MAX_PRIORITY];  // 現在(最後尾)のポインタ

	CObject* m_pPrev;  // 前のオブジェクトのポインタ
	CObject* m_pNext;  // 次のオブジェクトのポインタ

	static bool m_bReleaseAll;	// TODO: これやめる。update で例外スローされる理由 => update 中に releaseAll されて、pNext がすでに破棄されているから。

	bool m_bNormalUpdate;   // 全体でアップデート
	bool m_bPauseUpdate;    // ポーズ中にアップデート
	bool m_bNormalDraw;     // 全体で描画
	bool m_bPauseDraw;      // ポーズ中に描画

	bool m_bDeathAll;
};

#endif
