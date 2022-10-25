//=============================================================================
//
// 影クラス(shadow.cpp)
// Author : 唐﨑結斗
// 概要 : 影生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "shadow.h"
#include "renderer.h"
#include "application.h"
#include "calculation.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dエフェクトを生成する
//=============================================================================
CShadow * CShadow::Create(CObject *pParent)
{
	// オブジェクトインスタンス
	CShadow *pShadow = nullptr;

	// メモリの解放
	pShadow = new CShadow;

	// メモリの確保ができなかった
	assert(pShadow != nullptr);

	// 数値の初期化
	pShadow->Init();

	// 親の設定
	pShadow->SetParent(pParent);

	// インスタンスを返す
	return pShadow;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CShadow::CShadow() : m_pParent(nullptr)			// 親
{

}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CShadow::Init()
{
	// 初期化
	CObject3D::Init();

	// テクスチャの設定
	LoadTex(-1);

	// Zテストの優先度の設定
	SetZFunc(D3DCMP_LESSEQUAL);

	// アルファテストの透過率の設定
	SetAlphaValue(1);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CShadow::Uninit()
{
	// 終了
	CObject3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CShadow::Update()
{
	// 親の位置の取得
	D3DXVECTOR3 posParent = m_pParent->GetPos();
	posParent.y = 0.5f;

	// 親の向きの取得
	D3DXVECTOR3 rotParent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotParent.x += m_pParent->GetRot().x + D3DX_PI * 0.5f;
	rotParent.x = CCalculation::RotNormalization(rotParent.x);

	// 位置の設定
	SetPos(posParent);

	// 向きの設定
	SetRot(rotParent);

	// 更新
	CObject3D::Update();
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CShadow::Draw()
{// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画
	CObject3D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
