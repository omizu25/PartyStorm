//**************************************************
// 
// menu.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _MENU_H_	//このマクロ定義がされてなかったら
#define _MENU_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>
#include "object.h"

//==================================================
// 前方宣言
//==================================================
class CObject2D;

//==================================================
// 定義
//==================================================
class CMenu : public CObject
{
	/* ↓定義↓ */
private:
	static const float CURSOR_INTERVAL;	// カーソルの間隔

	/* ↓静的メンバ関数↓ */
public:
	static CMenu* Create();	// 生成

	/* ↓メンバ関数↓ */
public:
	CMenu();			// デフォルトコンストラクタ
	~CMenu() override;	// デストラクタ

public:
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Draw() override;		// 解放
	void Update() override;		// 更新
	void SetPos(const D3DXVECTOR3 &pos) override;									// 位置のセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }									// 位置のゲッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// 過去位置のセッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// 過去位置のゲッター
	void SetRot(const D3DXVECTOR3 &rot) override;									// 向きのセッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// 向きのゲッター
	void SetSize(const D3DXVECTOR3 &size) override;									// 大きさのセッター
	D3DXVECTOR3 GetSize()  override { return m_size; }								// 大きさのゲッター
	int Select();		// 選択
	void ColorChange();	// 色の変更
	void CMenu::SetTexture(int idx, int texIdx);	// テクスチャの設定
	void SetFrame(const D3DXVECTOR3& size, const D3DXCOLOR& col);	// 枠の設定
	int GetSelectIdx();	// 選択されている番号の取得

	//--------------------------------------------------
	// 生成
	// pos / 位置
	// size / サイズ
	// numUse / 選択肢の使用数
	// interval / 間隔
	// sort / 並べ方 [ true : 縦 false : 横 ]
	// cursor / カーソルを使うかどうか
	//--------------------------------------------------
	void Set(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, int numUse, float interval, bool sort, bool cursor);	// 設定

private:
	void Add(int add);

	/* ↓メンバ変数↓ */
private:
	CObject2D** m_pOption;	// 選択肢
	CObject2D* m_pFrame;	// 枠
	CObject2D* m_pCursor;	// カーソル
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 過去の位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_size;		// 大きさ
	int m_selectIdx;		// 選択されている番号
	int m_numUse;			// 使用数
	int m_time;				// タイム
	float m_cycle;			// カーブの周期
	bool m_sort;			// 並べ方 [ true : 縦 false : 横 ]
};

#endif // !_MENU_H_
