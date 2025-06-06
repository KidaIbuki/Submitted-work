//＝＝＝＝＝＝＝＝＝＝＝＝＝
//
//メイン処理（main.h)
//Author ibuki kida
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝
#ifndef _MAIN_H_	//このマクロ定義がされてなかったら
#define _MAIN_H_	//二重インクルード防止のマクロ定義

#include "pch.h"

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換用係数(1.0fで固定)
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;
}VERTEX_2D;

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点情報
	D3DXVECTOR3 nor;	//法線ベクトル(向き)
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_3D;

//=========================
//マクロ定義
//=========================
#define CLASS_NAME	"WindowClass"	//ウィンドウクラスの名前
#define WINDOW_NAME	"Shader"	//ウィンドウの名前

int GetFPS();

#endif 

