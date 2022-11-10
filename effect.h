//**************************************************
// 
// effect.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _EFFECT_H_	//このマクロ定義がされてなかったら
#define _EFFECT_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>
#include "object.h"

//==================================================
// 定義
//==================================================
class CEffect
{
	/* 定義 */
public:
	static const int MAX_EFFECT = 30000;	// エフェクトの最大数

private:
	static const int MAX_EXPLOSION;	// 爆発の最大数
	static const int MAX_LIFE;		// 寿命の最大値
	static const int STD_MOVE;		// 移動量の最大値
	static const float STD_SIZE;	// サイズの標準値

	/* ↓静的メンバ関数↓ */
public:
	static CEffect* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXCOLOR& col);	// 生成
	static void ReleaseAll();		// 全ての解放
	static void UpdateAll();		// 全ての更新
	static int GetNumAll();			// 総数の取得
	static CEffect** GetEffect();	// エフェクトの情報の取得
	static void Explosion(const D3DXVECTOR3& pos);	// 爆発

	/* ↓静的メンバ変数↓ */
private:
	static CEffect* m_pEffect[MAX_EFFECT];	// エフェクトの情報
	static int m_numAll;	// 総数

	/* ↓メンバ関数↓ */
public:
	CEffect();	// デフォルトコンストラクタ
	~CEffect();	// デストラクタ

public:
	void Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新

private:
	void Release();	// 解放

public:
	const D3DXVECTOR3& GetPos() const;	// 位置の取得
	const D3DXCOLOR& GetCol() const;	// 色の取得
	const D3DXVECTOR3& GetMove() const;	// 移動量の取得
	const D3DXVECTOR2& GetSize() const;	// サイズの取得

	/* ↓メンバ変数↓ */
private:
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR2 m_size;	// サイズ
	D3DXCOLOR m_col;	// 色
	int m_life;			// 寿命
	int m_index;		// 番号
};

#endif // !_EFFECT_H_
