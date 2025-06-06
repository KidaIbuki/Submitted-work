//=================================
// 
// タイトルクラス　title.h
// Auther kida ibuki 
// 
//==================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"  // シーン
#include <string>


class CTitle:public CScene
{
public:
    CTitle();			//コンストラクタ
    ~CTitle()override;			//デストラクタ
    HRESULT Init()override;		//初期設定
    void Uninit()override;      //終了処理
    void Update()override;      //更新処理

    void Draw()override;		//描画処理
    static CTitle* Create();     //生成
private:
};


#endif