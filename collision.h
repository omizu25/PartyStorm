//=============================================================================
//
// 当たり判定クラス(collision.h)
// Author : 唐﨑結斗
// 概要 : 当たり判定生成を行う
//
//=============================================================================
#ifndef _COLLISION_H_			// このマクロ定義がされてなかったら
#define _COLLISION_H_			// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "main.h"

//=============================================================================
// 当たり判定クラス
// Author : 唐﨑結斗
// 概要 : 当たり判定生成を行うクラス
//=============================================================================
class CCollision : public CObject
{
public:
	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CCollision();
	~CCollision();

	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	HRESULT Init() override;																			// 初期化
	void Uninit() override;																				// 終了
	void Update() override;																				// 更新
	void Draw() override;																				// 描画
	void SetParent(CObject *pParent);																	// 親のセッター
	void SetPos(const D3DXVECTOR3 &pos) override;														// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { m_posOld = posOld; }							// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override;														// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override;														// 大きさのセッター
	bool ColisonRange2D(CObject *target);																// 範囲の判定(2D)
	bool ColisonRectangle2D(CObject *target, bool bExtrude);											// 矩形の判定(2D)
	bool ColisonCircle2D(CObject *target, bool bExtrude);												// 円の判定(2D)
	bool ColisonRectangle3D(CObject *target, bool bExtrude);											// 矩形の判定(3D)
	bool ColisonSphere3D(CObject *target, D3DXVECTOR3 size, D3DXVECTOR3 targetSize, bool bExtrude);		// 球の判定(3D)
	CObject	*GetParent() { return m_pParent; }															// 親のゲッター
	D3DXVECTOR3 GetPos() override { return m_pos; }														// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return m_posOld; }												// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return m_rot; }													// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return m_size; }													// 大きさのゲッター

private:
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	CObject				*m_pParent;			// 親
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_posOld;			// 過去位置
	D3DXVECTOR3			m_posAdd;			// 位置の追加分
	D3DXVECTOR3			m_rot;				// 向き
	D3DXVECTOR3			m_size;				// 大きさ
};

#endif
