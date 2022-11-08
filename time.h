//=============================================================================
//
// タイムクラス(time.h)
// Author : 唐﨑結斗
// 概要 : タイムの設定を行う
//
//=============================================================================
#ifndef _TIME_H_		// このマクロ定義がされてなかったら
#define _TIME_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;
class CScore;

//=============================================================================
// タイムクラス
// Author : 唐﨑結斗
// 概要 : タイム生成を行うクラス
//=============================================================================
class CTime : public CObject
{
public:
	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static CTime *Create();			// エネルギーゲージの生成

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CTime(int nPriority = CObject::PRIORITY_2D);
	~CTime() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override;									// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }		// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;									// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;									// 大きさのセッター
	void SetTime(const int nTime);													// 時間のセッター
	void SetCountDown();															// カウントダウンのセッター
	D3DXVECTOR3 GetPos() override { return m_pos; }									// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }							// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }								// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }								// 大きさのゲッター
	int GetTime() { return m_nTime; }												// 時間のゲッター
	CScore *GetScore() { return m_pScore; }											// スコアのゲッター
	void StopTime(bool bStop) { m_bStop = bStop; }									// 時間停止

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CScore				*m_pScore;			// スコア
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_posOld;			// 過去の位置
	D3DXVECTOR3			m_rot;				// 向き
	D3DXVECTOR3			m_size;				// 大きさ
	int					m_nTime;			// 時間
	int					m_nCntFrame;		// フレームカウント
	bool				m_bStop;			// 停止判定
	bool				m_bCountDown;		// カウントダウン
};

#endif




