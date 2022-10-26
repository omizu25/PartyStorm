//=============================================================================
//
// エネミークラス(Enemy.h)
// Author : 磯江寿希亜
// 概要 : プレイヤーに襲い掛かる
//
//=============================================================================
#ifndef _SHARK_H_			// このマクロ定義がされてなかったら
#define _SHARK_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "motion_model3D.h"
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CMove;
class CShadow;

class CEnemyShark : public CMotionModel3D
{
public:
	//--------------------------------------------------------------------
	// プレイヤーのアクションの列挙型
	//--------------------------------------------------------------------
	enum ACTION_TYPE
	{
		NEUTRAL_ACTION = 0,		// ニュートラル
		MOVE_ACTION,			// 移動
		ATTACK_ACTION,			// 攻撃
		MAX_ACTION,				// 最大数
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CEnemyShark *Create();			// プレイヤーの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CEnemyShark();
	~CEnemyShark();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	virtual HRESULT Init();											// 初期化
	void Uninit() override;											// 終了
	void Update() override;											// 更新
	void Draw() override;											// 描画
	void SetSpeed(const float fSpeed) { m_fSpeed = fSpeed; }		// 移動速度の設定

private:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	D3DXVECTOR3 Move();		// 移動
	void Rotate();			// 回転

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CMove			*m_pMove;				// 移動情報
	CShadow			*m_pShadow;				// 影の情報
	ACTION_TYPE		m_EAction;				// アクションタイプ
	D3DXVECTOR3		m_move;					// 移動ベクトル
	D3DXVECTOR3		m_rotDest;				// 目的の向き
	float			m_fSpeed;				// 移動速度	
	int				m_nNumMotion;			// 現在のモーション番号
};

#endif

