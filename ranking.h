//**************************************************
// 
// ranking.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _RANKING_H_	//このマクロ定義がされてなかったら
#define _RANKING_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>
#include "object.h"

//==================================================
// 前方宣言
//==================================================
class CScore;
class CObject2D;

//==================================================
// 定義
//==================================================
class CRanking : public CObject
{
	/* ↓定義↓ */
public:
	static const int MAX_RANKING = 5;	// ランキングの最大数
	static const float STD_WIDTH;		// 幅の標準値
	static const float STD_HEIGHT;		// 高さの標準値

private:
	static const char* FILE_NAME;	// ファイルパス

	/* ↓静的メンバ関数↓ */
public:
	static CRanking* Create();	// 生成
	static void Set(int score);	// 設定
	static int Get(int rank);	// 取得

private:
	static void Load();		// 読み込み
	static void Save();		// 保存
	static void Change();	// 変更

	/* ↓静的メンバ変数↓ */
private:
	static int m_score;	// 今回のスコア
	static int m_ranking[MAX_RANKING];	// ランキング

	/* ↓メンバ関数↓ */
public:
	CRanking();				// デフォルトコンストラクタ
	~CRanking() override;	// デストラクタ

public:
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update() override;		// 更新
	void Draw() override;		// 描画
	void Set(const D3DXVECTOR3& pos, float length);			// 設定
	void SetPos(const D3DXVECTOR3 &pos) override;			// 位置の設定
	D3DXVECTOR3 GetPos() override { return m_pos; }			// 位置の取得
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }	// 過去位置の取得
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }	// 過去位置の設定
	void SetRot(const D3DXVECTOR3 &rot) override;			// 向きの設定
	D3DXVECTOR3 GetRot()  override { return m_rot; }		// 向きの取得
	void SetSize(const D3DXVECTOR3 &size) override;			// 大きさの設定
	D3DXVECTOR3 GetSize()  override { return m_size; }		// 大きさの取得

private:
	int Digit(int number);	// 桁数

	/* ↓メンバ変数↓ */
private:
	CScore* m_pRanking[MAX_RANKING];	// ランキング
	CObject2D* m_pRank[MAX_RANKING];	// 位
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 過去の位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_size;		// 大きさ
	int m_newRank;			// 新しいスコア
	int m_time;				// タイム
};

#endif // !_RANKING_UI_H_
