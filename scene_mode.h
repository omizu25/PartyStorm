//=============================================================================
//
// シーンモードクラス(scene_mode.h)
// Author : 唐﨑結斗
// 概要 : シーンモードの派生を行う
//
//=============================================================================
#ifndef _SCENE_MODE_H_		// このマクロ定義がされてなかったら
#define _SCENE_MODE_H_		// 二重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//=============================================================================
// シーンモードクラス
// Author : 唐﨑結斗
// 概要 : シーンモード生成を行うクラス
//=============================================================================
class CSceneMode : public CObject
{
public:
	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CSceneMode();
	~CSceneMode() override;

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init() override;														// 初期化
	void Uninit() override;															// 終了
	void Update() override;															// 更新
	void Draw() override;															// 描画
	void SetPos(const D3DXVECTOR3 &pos) override { pos; }							// 位置のセッター
	void SetPosOld(const D3DXVECTOR3 &posOld) override { posOld; }					// 過去位置のセッター
	void SetRot(const D3DXVECTOR3 &rot) override { rot; }							// 向きのセッター
	void SetSize(const D3DXVECTOR3 &size) override { size; }						// 大きさのセッター
	D3DXVECTOR3 GetPos() override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			// 位置のゲッター
	D3DXVECTOR3 GetPosOld()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 過去位置のゲッター
	D3DXVECTOR3 GetRot()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 向きのゲッター
	D3DXVECTOR3 GetSize()  override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 大きさのゲッター
};

#endif

