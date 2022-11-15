//**************************************************
// 
// obstacle_manager.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _OBSTACLE_MANAGER_H_	// このマクロ定義がされてなかったら
#define _OBSTACLE_MANAGER_H_	// 二重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include "object.h"
#include <d3dx9.h>

//==================================================
// クラス
//==================================================
class CObstacleManager : public CObject
{
	/* ↓定義↓ */
private:
	
	/* ↓静的メンバ関数↓ */
public:
	static CObstacleManager* Create();	// 生成

	/* ↓メンバ関数↓ */
public:
	CObstacleManager();				// デフォルトコンストラクタ
	~CObstacleManager() override;	// デストラクタ

public:
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update() override;		// 更新
	void Draw() override;		// 描画
	void SetPos(const D3DXVECTOR3 &pos) override { m_pos = pos; }				// 位置の設定
	D3DXVECTOR3 GetPos() override { return m_pos; }								// 位置の取得
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }	// 過去の位置の設定
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }						// 過去の位置の取得
	void SetRot(const D3DXVECTOR3 &rot) override { m_rot = rot; }				// 向きの設定
	D3DXVECTOR3 GetRot()  override { return m_rot; }							// 向きの取得
	void SetSize(const D3DXVECTOR3 &size) override { m_size = size; };			// 大きさの設定
	D3DXVECTOR3 GetSize()  override { return m_size; }							// 大きさの取得

public:
	void SetGame(bool game) { m_game = game; }	// ゲーム中かどうか設定
	void SetStop(bool stop) { m_stop = stop; }	// 出現を止めるかどうかの設定

private:
	void Game();	// ゲーム
	void BG();		// 背景

	/* ↓メンバ変数↓ */
private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 過去の位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_size;		// 大きさ
	int m_time;		// 時間
	int m_popIdxBG;	// 背景の出現した番号
	int m_pop;		// 出現数
	int m_interval;	// 間隔
	float m_move;	// 移動量
	bool m_game;	// ゲーム中かどうか
	bool m_stop;	// 出現を止めるかどうか
};

#endif // !_OBSTACLE_MANAGER_H_
