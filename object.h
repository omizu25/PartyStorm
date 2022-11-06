//=============================================================================
//
// オブジェクトクラス(object.h)
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行う
//
//=============================================================================
#ifndef _OBJECT_H_		// このマクロ定義がされてなかったら
#define _OBJECT_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//=============================================================================
// オブジェクトクラス
// Author : 唐﨑結斗
// 概要 : オブジェクト生成を行うクラス
//=============================================================================
class CObject
{
public:
	//--------------------------------------------------------------------
	// プライオリティのレベル
	// Author : 唐﨑結斗
	// 概要 : プライオリティのレベルを設定
	//--------------------------------------------------------------------
	enum PRIORITY_LEVEL
	{
		PRIORITY_NONE = 0,	// その他
		PRIORITY_3D,		// 3D
		PRIORITY_EFFECT,	// エフェクト
		PRIORITY_2D,		// 2D
		PRIORITY_FADE,		// フェード
		PRIORITY_MAX		// プライオリティの最大
	};

	//--------------------------------------------------------------------
	// オブジェクトの種別の列挙型
	// Author : 唐﨑結斗
	// 概要 : オブジェクトの種別情報を格納
	//--------------------------------------------------------------------
	enum EObjectType
	{
		OBJTYPE_NONE = 0,			// タイプ無し
		OBJTYPE_3DPLAYER,			// 3Dプレイヤー
		OBJTYPE_3DENEMY,			// 3Dエネミー
		OBJTYPE_3DMODEL,			// 3Dモデル
		OBJTYPE_FADE,				// フェード
		OBJTYPE_PAUSE,				// ポーズ
		MAX_OBJTYPE,				// 種別の最大数
	};

	//--------------------------------------------------------------------
	// 静的メンバ関数
	//--------------------------------------------------------------------
	static void ReleaseAll(bool bPermanent);										// すべてのオブジェクトの解放
	static void UpdateAll();														// すべてのオブジェクトの更新
	static void DrawAll();															// すべてのオブジェクトの描画
	static void ReleaseListAll();													// すべてのオブジェクトのリスト解除
	static CObject *GetTop(int nPriority) { return m_pTop[nPriority]; }				// 先頭オブジェクトへのポインタのゲッター
	static CObject *GetCurrent(int nPriority) { return m_pCurrent[nPriority]; }		// 現在の(一番後ろ)オブジェクトへのポインタのゲッター
	static void SetPause(const bool bPause) { m_bPause = bPause; }					// ポーズ状態のセッター

#ifdef _DEBUG
	static void ReleaseTop(int nPriority);
	static void ReleaseCurrent(int nPriority);
	static void RandomRelease(int nPriority);
#endif // DEBUG

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	explicit CObject(int nPriority = PRIORITY_NONE);
	virtual ~CObject();

	//--------------------------------------------------------------------
	// 純粋仮想関数
	//--------------------------------------------------------------------
	virtual HRESULT Init() = 0;																// 初期化
	virtual void Uninit() = 0;																// 終了
	virtual void Update() = 0;																// 更新
	virtual void Draw() = 0;																// 描画
	virtual void SetPos(const D3DXVECTOR3 &pos) = 0;										// 位置のセッター
	virtual void SetPosOld(const D3DXVECTOR3 &posOld) = 0;									// 過去位置のセッタ
	virtual void SetRot(const D3DXVECTOR3 &rot) = 0;										// 向きのセッター
	virtual void SetSize(const D3DXVECTOR3 &size) = 0;										// 大きさのセッター
	virtual D3DXVECTOR3 GetPos() = 0;														// 位置のゲッター
	virtual D3DXVECTOR3 GetPosOld() = 0;													// 過去位置のゲッター
	virtual D3DXVECTOR3 GetRot() = 0;														// 向きのゲッター
	virtual D3DXVECTOR3 GetSize() = 0;														// 大きさのゲッター
	void SetObjType(EObjectType objectType) { m_objectType = objectType; }					// オブジェクトの種別設定
	EObjectType GetObjType() { return m_objectType; }										// オブジェクトの種別設定
	void SetColisonPos(const D3DXVECTOR3 &colisonPos) { m_colisonPos = colisonPos; }		// あたり判定の位置のセッター
	D3DXVECTOR3 GetColisonPos() { return m_colisonPos; }									// あたり判定の位置のゲッター
	void SetColisonSize(const D3DXVECTOR3 &colisonSize) { m_colisonSize = colisonSize; }	// あたり判定の大きさのセッター
	D3DXVECTOR3 GetColisonSize() { return m_colisonSize; }									// あたり判定の大きさのゲッター
	CObject *GetPrev() { return m_pPrev; }													// 前のオブジェクトへのポインタのゲッター
	CObject *GetNext() { return m_pNext; }													// 次のオブジェクトへのポインタのゲッター
	bool GetFlagDeath() { return m_bDeath; }												// 死亡フラグの取得
	bool ColisonRectangle3D(CObject *target, bool bExtrude);								// 矩形の判定(3D)
	bool ColisonSphere3D(CObject *target, bool bExtrude);									// 球の判定(3D)

protected:
	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	void Release(void);												// オブジェクトの解放
	void ReleaseList(void);											// オブジェクトのリスト解除

private:
	//--------------------------------------------------------------------
	// 静的メンバ変数
	//--------------------------------------------------------------------
	static CObject *m_pTop[PRIORITY_MAX];			// 先頭オブジェクトへのポインタ
	static CObject *m_pCurrent[PRIORITY_MAX];		// 現在の(一番後ろ)オブジェクトへのポインタ
	static int m_nMaxObject;						// 使用数
	static int m_nPriorityMaxObj[PRIORITY_MAX];		// プライオリティごとのオブジェクト数
	static bool m_bPause;							// ポーズを使用しているかどうか

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CObject *m_pPrev;				// 前のオブジェクトへのポインタ
	CObject *m_pNext;				// 次のオブジェクトへのポインタ
	EObjectType m_objectType;		// オブジェクトの種別
	D3DXVECTOR3 m_colisonPos;		// あたり判定の位置
	D3DXVECTOR3 m_colisonSize;		// あたり判定の大きさ
	int	 m_nLevPriority;			// プライオリティのレベル
	bool m_bDeath;					// 死亡フラグ
};

#endif

