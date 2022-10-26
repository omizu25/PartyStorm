//=============================================================================
//
// 3Dエフェクトクラス(effect3D.cpp)
// Author : 唐﨑結斗
// 概要 : 3Dエフェクト生成を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "effect3D.h"
#include "renderer.h"
#include "application.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : 3Dエフェクトを生成する
//=============================================================================
CEffect3D * CEffect3D::Create(void)
{
	// オブジェクトインスタンス
	CEffect3D *pEffect3D = nullptr;

	// メモリの解放
	pEffect3D = new CEffect3D;

	// メモリの確保ができなかった
	assert(pEffect3D != nullptr);

	// 数値の初期化
	pEffect3D->Init();

	// インスタンスを返す
	return pEffect3D;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CEffect3D::CEffect3D()
{
	m_renderMode = MODE_NORMAL;								// レンダーステートの計算方法
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 移動量
	m_moveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動方向
	m_subSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさの減少量
	m_fSpeed = 0.0f;										// 速度
	m_fSubAlpha = 0.0f;										// アルファ値の減少量
	m_nLife = 0;											// 寿命
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CEffect3D::~CEffect3D()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CEffect3D::Init()
{
	// 初期化
	CObject3D::Init();

	// テクスチャの設定
	LoadTex(-1);

	// Zバッファの設定
	SetZFunc(D3DCMP_ALWAYS);

	// αテストの透過率の設定
	SetAlphaValue(0);

	// ビルボードオン
	SetBillboard(true);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CEffect3D::Uninit()
{
	// 終了
	CObject3D::Uninit();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 2D更新を行う
//=============================================================================
void CEffect3D::Update()
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPos();

	// 移動量の算出
	m_move.z = sinf(m_moveVec.x) * cosf(m_moveVec.y) * m_fSpeed;
	m_move.x = sinf(m_moveVec.x) * sinf(m_moveVec.y) * m_fSpeed;
	m_move.y = cosf(m_moveVec.x) * m_fSpeed;

	// 位置の更新
	pos += m_move;

	// 位置の更新
	SetPos(pos);

	// ライフの減少
	DecreaseLife();

	if (m_nLife == 0)
	{// 終了
		Uninit();
	}
	else
	{// 更新
		CObject3D::Update();
	}
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CEffect3D::Draw()
{// レンダラーのゲット
	CRenderer *pRenderer = CApplication::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	switch (m_renderMode)
	{
	case CEffect3D::MODE_NORMAL:
		break;

	case CEffect3D::MODE_ADD:
		// αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	case CEffect3D::MODE_SUB:
		// αブレンディングを減算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;

	default:
		break;
	}

	// 描画
	CObject3D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 寿命の設定
// Author : 唐﨑結斗
// 概要 : 寿命の設定
//=============================================================================
void CEffect3D::SetLife(const int nLife)
{
	m_nLife = nLife;
	m_subSize = GetSize() / (float)m_nLife;
	m_fSubAlpha = GetColor().a / m_nLife;
}

//=============================================================================
// ライフの減少処理
// Author : 唐﨑結斗
// 概要 : 2D描画を行う
//=============================================================================
void CEffect3D::DecreaseLife()
{
	if (m_nLife > 0)
	{
		D3DXVECTOR3 size = GetSize();
		D3DXCOLOR color = GetColor();

		// ライフの減少
		m_nLife--;
		SetSize(size - m_subSize);
		SetColor(D3DXCOLOR(color.r, color.g, color.b, color.a - m_fSubAlpha));

		if (m_nLife <= 0)
		{
			m_nLife = 0;
		}
	}
}

