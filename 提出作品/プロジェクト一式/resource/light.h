//=================================
// 
//ライトクラス　　light.h
//Auther kida ibuki 
// 
//==================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"   // メイン

//#define MAX_LIGHT (3) //ライトの数
//#define VEC_DIR (D3DXVECTOR3(0.22f, -0.87f, 0.44f))    // 1つ目のライトの方向ベクトルの設定
//#define VEC_DIR1 (D3DXVECTOR3(-0.18f, 0.88f, -0.44f))  // 2つ目のライトの方向ベクトルの設定
//#define VEC_DIR2 (D3DXVECTOR3(0.89f, 0.11f, 0.44f))    // 3つ目のライトの方向ベクトルの設定

class Light;  // 前方宣言
//ライトのクラス定義
class CLight
{
public:
	CLight();		    //コンストラクタ
	~CLight();		    //デストラクタ
	HRESULT Init();     //初期設定
	void Uninit();		//終了処理
	void Update();      //更新処理
	D3DXVECTOR3 GetLightDir() { return position; }

	void Draw();
private:
	//D3DLIGHT9 m_aLight[MAX_LIGHT];  //ライト構造体
	D3DLIGHT9 light;
	Light* m_directionalLight;
	// ImGui用ライトの方向変更変数
	float vecX, vecY, vecZ;
	D3DXVECTOR3 position;
	LPD3DXMESH m_mesh;


};

// 各ライト呼び出し便利クラス
class Light
{
public:
	Light();
	~Light();
	void InitDirectional(const D3DXVECTOR3& direction, const D3DXCOLOR& color);   // 指向性ライト
	void InitPoint(const D3DXVECTOR3& position, const D3DXCOLOR& color, float range);  // ポイントライト
	void InitSpot(const D3DXVECTOR3& position, const D3DXVECTOR3& direction, const D3DXCOLOR& color, float range, float theta, float phi);  // スポットライト

	void Enable(LPDIRECT3DDEVICE9 device, DWORD index);  // ライトの有効化
	void Disable(LPDIRECT3DDEVICE9 device, DWORD index); // ライトの無効化

	void SetPosition(const D3DXVECTOR3& pos);   // ライトの位置設定
	void SetDirection(const D3DXVECTOR3& dir);  // ライトの方向設定
	void DrawObjectWithLighting(LPDIRECT3DDEVICE9 device, LPD3DXMESH mesh, const D3DXVECTOR3 & position);  // ライトオブジェクト描画
	void UpdateObj(float deltaTime, D3DXVECTOR3 center);

private:
	D3DLIGHT9 m_light;
	D3DXMATRIX world;

	float anglef;             // 現在の角度（ラジアン）
	float radius;     // 半径
	float speed;       // 回転スピード（ラジアン/秒）
	//D3DXVECTOR3 center;  // 中心座標（円の中心）

};

#endif