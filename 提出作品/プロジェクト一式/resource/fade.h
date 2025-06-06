//=================================
// 
// フェード　fade.h
// Auther kida ibuki 
// 
//==================================
#ifndef _FADE_H_
#define _FADE_H_

#include "object2D.h"   // オブジェクト2D
#include "scene.h"       // シーン
class CFade :public CObject2D
{
public:
    //フェードの状態
    typedef enum
    {
        FADE_NONE = 0,
        FADE_IN,		//フェードイン状態
        FADE_OUT,		//フェードアウト状態
        FADE_MAX
    }FADE;

    CFade();			//コンストラクタ
    ~CFade()override;			//デストラクタ
    HRESULT Init()override;		//初期設定
    void Uninit()override;      //終了処理
    void Update()override;      //更新処理
    void Draw()override;		//描画処理
    static CFade* SetFade(CScene::MODE mode);
    static CFade* SetFade_Pos(D3DXVECTOR3 pos);
    bool GetNumUse() { return m_bUse; }
    void DeathFlag()override;

private:
    FADE m_Fade;    // フェードの状態
    D3DXCOLOR m_colorFade;    // カラー
    CScene::MODE m_modeNext;  // 次のモード
    bool m_bUse;         // 使用されているか
    D3DXVECTOR3 m_pos;   // 位置
};

#endif