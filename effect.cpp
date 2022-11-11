//**************************************************
// 
// effect.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "effect.h"
#include "application.h"
#include "calculation.h"
#include <assert.h>

//==================================================
// 定義
//==================================================
const int CEffect::MAX_EXPLOSION = 3000;
const int CEffect::MAX_LIFE = 100;
const int CEffect::STD_MOVE = 75;
const float CEffect::STD_SIZE = 12.0f;

//==================================================
// 静的メンバ変数
//==================================================
CEffect* CEffect::m_pEffect[MAX_EFFECT] = {};
int CEffect::m_numAll = 0;

namespace
{
float FloatRandom(float min, float max);	// 少数のランダム
}// namespaceはここまで

//--------------------------------------------------
// 生成
//--------------------------------------------------
CEffect* CEffect::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXCOLOR& col)
{
	if (m_numAll >= MAX_EFFECT)
	{// 最大数を越した
		return nullptr;
	}

	CEffect* pEffect = nullptr;

	pEffect = new CEffect;
	
	if (pEffect != nullptr)
	{// nullチェック
		pEffect->Init();
		pEffect->m_pos = pos;
		pEffect->m_move = move;
		pEffect->m_col = col;
	}

	return pEffect;
}

//--------------------------------------------------
// 全ての解放
//--------------------------------------------------
void CEffect::ReleaseAll()
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (m_pEffect[i] != nullptr)
		{// NULLチェック
			m_pEffect[i]->Uninit();
			m_pEffect[i] = nullptr;
		}
	}
}

//--------------------------------------------------
// 全ての更新
//--------------------------------------------------
void CEffect::UpdateAll()
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (m_pEffect[i] != nullptr)
		{// NULLチェック
			m_pEffect[i]->Update();
		}
	}
}

//--------------------------------------------------
// 総数の取得
//--------------------------------------------------
int CEffect::GetNumAll()
{
	return m_numAll;
}

//--------------------------------------------------
// エフェクトの情報の取得
//--------------------------------------------------
CEffect** CEffect::GetEffect()
{
	assert(m_numAll >= 0 && m_numAll < MAX_EFFECT);

	return m_pEffect;
}

//--------------------------------------------------
// 爆発
//--------------------------------------------------
void CEffect::Explosion(const D3DXVECTOR3& pos)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	int max = STD_MOVE * 2;

	for (int i = 0; i < MAX_EXPLOSION; i++)
	{
		move.x = ((float)(rand() % max) - STD_MOVE);
		move.y = ((float)(rand() % max) - STD_MOVE);
		move.z = ((float)(rand() % max) - STD_MOVE);

		// 生成
		CEffect::Create(pos, move, col);
	}
}

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CEffect::CEffect() :
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR2(0.0f, 0.0f)),
	m_col(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)),
	m_life(0)
{
	assert(m_numAll >= 0 && m_numAll < MAX_EFFECT);

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (m_pEffect[i] == nullptr)
		{// NULLチェック
			m_pEffect[i] = this;
			m_index = i;
			m_numAll++;
			break;
		}
	}
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CEffect::~CEffect()
{
	m_numAll--;
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void CEffect::Init()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(STD_SIZE, STD_SIZE);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_life = MAX_LIFE;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CEffect::Uninit()
{
	// 解放
	Release();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CEffect::Update()
{
	m_life--;

	if (m_life <= 0)
	{
		Uninit();
		return;
	}

	m_pos += m_move;

	//慣性・移動量を更新 (減衰させる)
	m_move.x += (0.0f - m_move.x) * 0.05f;
	m_move.y += (0.0f - m_move.y) * 0.05f;
	m_move.z += (0.0f - m_move.z) * 0.05f;

	{// 色の減算
		float ratio = ((float)(MAX_LIFE - m_life) / MAX_LIFE);
		m_col.a = 1.0f - (ratio * ratio);
	}
}

//--------------------------------------------------
// 解放
//--------------------------------------------------
void CEffect::Release()
{
	if (m_pEffect[m_index] != nullptr)
	{// NULLチェック
		int index = m_index;
		delete m_pEffect[index];
		m_pEffect[index] = nullptr;
	}
}

//--------------------------------------------------
// 位置の取得
//--------------------------------------------------
const D3DXVECTOR3& CEffect::GetPos() const
{
	return m_pos;
}

//--------------------------------------------------
// 色の取得
//--------------------------------------------------
const D3DXCOLOR& CEffect::GetCol() const
{
	return m_col;
}

//--------------------------------------------------
// 移動量の取得
//--------------------------------------------------
const D3DXVECTOR3& CEffect::GetMove() const
{
	return m_move;
}

//--------------------------------------------------
// サイズの取得
//--------------------------------------------------
const D3DXVECTOR2& CEffect::GetSize() const
{
	return m_size;
}

namespace
{
//--------------------------------------------------
// 小数点のランダム
//--------------------------------------------------
float FloatRandom(float min, float max)
{
	return ((rand() / (float)RAND_MAX) * (max - min)) + min;
}
}
