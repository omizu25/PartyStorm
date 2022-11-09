//=============================================================================
//
// 障害物クラス (obstacle.h)
// Author : 香月瑞輝
//
//=============================================================================
#ifndef _OBSTACLE_H_	// このマクロ定義がされてなかったら
#define _OBSTACLE_H_	// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "model_obj.h"
#include "main.h"

//=============================================================================
// 障害物クラス
//=============================================================================
class CObstacle : public CModelObj
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static void InitStatic();		// 静的メンバ変数の初期化
	static void Pop();				// 出現
	static void Stop(bool stop);	// 出現を止めるかどうか
	static CObstacle* Create(float posX, float inverse);	// 障害物の生成

	//--------------------------------------------------------------------
	// 静的メンバ変数
	//--------------------------------------------------------------------
	static int m_time;		// 時間
	static int m_pop;		// 出現数
	static int m_interval;	// 間隔
	static float m_move;	// 移動量
	static bool m_stop;		// 出現を止めるかどうか

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CObstacle();
	~CObstacle();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update() override;		// 更新
	void Draw() override;		// 描画
	void Collison();			// 当たり判定

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	int m_waveTime;		// ウェーブ用の時間
	float m_inverse;	// 逆の動きするかどうか
};

#endif
