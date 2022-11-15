//**************************************************
// 
// obstacle.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _OBSTACLE_H_	// このマクロ定義がされてなかったら
#define _OBSTACLE_H_	// 二重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "model_obj.h"
#include "main.h"

//==================================================
// クラス
//==================================================
class CObstacle : public CModelObj
{
	/* ↓静的メンバ関数↓ */
public:
	static void SetMove(float move) { m_move = move; }	// 移動量の設定
	static CObstacle* Create(float posX, float posZ, float inverse, float diePosZ);	// 生成

	/* ↓静的メンバ変数↓ */
private:
	static float m_move;	// 移動量

	/* ↓メンバ関数↓ */
public:
	CObstacle();			// デフォルトコンストラクタ
	~CObstacle() override;	// デストラクタ

public:
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update() override;		// 更新
	void Draw() override;		// 描画

private:
	void Move();		// 移動
	void Rot();			// 回転
	void Collison();	// 当たり判定

	/* ↓メンバ変数↓ */
private:
	int m_waveTime;		// ウェーブ用の時間
	float m_inverse;	// 逆の動きするかどうか
	float m_diePosZ;	// 破棄するZの位置
};

#endif // !_OBSTACLE_H_
