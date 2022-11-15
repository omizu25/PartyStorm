///=============================================================================
//
// タイムクラス(time.cpp)
// Author : 唐﨑結斗
// 概要 : タイムの設定を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>
#include <math.h>

#include "time.h"
#include "object.h"
#include "object2D.h"
#include "score.h"
#include "renderer.h"
#include "application.h"
#include "texture.h"
#include "game.h"
#include "player.h"
#include "obstacle_manager.h"
#include "pause.h"

//=============================================================================
// インスタンス生成
// Author : 唐﨑結斗
// 概要 : スコアを生成する
//=============================================================================
CTime *CTime::Create()
{
	// オブジェクトインスタンス
	CTime *pTime = nullptr;

	pTime = new CTime;

	// メモリの確保ができなかった
	assert(pTime != nullptr);

	// 数値の初期化
	pTime->Init();

	// インスタンスを返す
	return pTime;
}

//=============================================================================
// コンストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CTime::CTime(int nPriority /*= CObject::PRIORITY_LEVEL3*/) : CObject(nPriority)
{
	m_pScore = nullptr;									// スコア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 過去の位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 大きさ
	m_nTime = 0;										// 時間
	m_nCntFrame = 0;									// フレームカウント
	m_bStop = false;									// 停止判定
	m_bCountDown = false;								// カウントダウン
}

//=============================================================================
// デストラクタ
// Author : 唐﨑結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CTime::~CTime()
{

}

//=============================================================================
// 初期化
// Author : 唐﨑結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CTime::Init()
{
	// スコアの生成
	m_pScore = CScore::Create(10, true);
	assert(m_pScore != nullptr);
	m_pScore->SetScore(0);

	// 大きさの設定
	m_pScore->SetWholeSize(D3DXVECTOR3(1000.0f, 50.0f, 0.0f));
	m_pScore->SetSize(D3DXVECTOR3(50.0f, 75.0f, 0.0f));

	// 位置の設定
	SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	// 向きの設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐﨑結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CTime::Uninit()
{
	m_pScore->Uninit();

	// インスタンスの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐﨑結斗
// 概要 : 更新を行う
//=============================================================================
void CTime::Update()
{
	if (m_bCountDown)
	{
		m_nCntFrame++;

		if (m_nCntFrame % 60 == 0)
		{// 時間の設定

			m_pScore->AddScore(-1);

			// 時間の取得
			m_nTime--;

			if (m_nTime <= 0)
			{// 制限時間が来た
				Uninit();
				CPlayer** pPlayer = CGame::GetPlayer();

				int max = CApplication::GetPersonCount();

				for (int i = 0; i < max; i++)
				{
					pPlayer[i]->SetAction(true);
				}

				// 時間が進む
				CGame::GetTime()->StopTime(false);

				// 出現する
				CGame::GetObstacle()->SetStop(false);

				// ポーズできる
				CApplication::GetPause()->SetSelect(true);

				return;
			}
		}

		return;
	}

	if (m_nTime >= 0 && !m_bStop)
	{// フレームカウント
		int nPlayer = CApplication::GetPersonCount();

		if (nPlayer > 1)
		{// マルチプレイ
			m_nCntFrame++;

			if (m_nCntFrame % 60 == 0)
			{// 時間の設定

				m_pScore->AddScore(-1);

				// 時間の取得
				m_nTime--;

				if (m_nTime <= 0)
				{// 制限時間が来た
					m_bStop = true;
					m_nTime = 0;
					m_pScore->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
					CGame::SetGame(false);

					// 出現する
					CGame::GetObstacle()->SetStop(true);

					// ポーズできなくする
					CApplication::GetPause()->SetSelect(false);
				}

			}

			if (m_nTime <= 5)
			{// マルチプレイで制限時間まで後少し
				float sinCurve = (sinf((m_nCntFrame * 0.05f) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f;
				float alpha = (sinCurve * 0.75f) + 0.25f;

				// 色の設定
				m_pScore->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, alpha));
			}
		}
		else
		{// シングルプレイ
			m_pScore->AddScore(13);
			
			// 時間の取得
			m_nTime += 13;
		}
	}	
}

//=============================================================================
// 描画
// Author : 唐﨑結斗
// 概要 : 描画を行う
//=============================================================================
void CTime::Draw()
{
	
}

//=============================================================================
// 位置のセッター
// Author : 唐﨑結斗
// 概要 : 位置のメンバ変数に引数を代入
//=============================================================================
void CTime::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	m_pScore->SetPos(m_pos);
}

//=============================================================================
// 向きのセッター
// Author : 唐﨑結斗
// 概要 : 向きのメンバ変数に引数を代入
//=============================================================================
void CTime::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;

	m_pScore->SetRot(m_rot);
}

//=============================================================================
// 大きさのセッター
// Author : 唐﨑結斗
// 概要 : 大きさのメンバ変数に引数を代入
//=============================================================================
void CTime::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	m_pScore->SetWholeSize(D3DXVECTOR3(m_size.x, size.y / 2.0f, size.z));
	m_pScore->SetSize(D3DXVECTOR3(m_size.x / 2.5f, size.y / 2.5f, size.z));
}

//=============================================================================
// ナンバーのセッター
// Author : 唐﨑結斗
// 概要 : ナンバーのメンバ変数に引数を代入
//=============================================================================
void CTime::SetTime(const int nTime)
{// 時間の設定
	m_nTime = nTime;

	// スコアの設定
	m_pScore->SetScore(m_nTime);
	m_pScore->SetDestScore(m_nTime);
	m_pScore->AddDigit();
}

//=============================================================================
// カウントダウンのセッター
// Author : 香月瑞輝
// 概要 : カウントダウン化する
//=============================================================================
void CTime::SetCountDown()
{
	m_bCountDown = true;

	m_nTime = 3;

	m_pScore->SetScore(3);
	m_pScore->SetDestScore(3);
	m_pScore->AddDigit();

	// 大きさの設定
	m_pScore->SetWholeSize(D3DXVECTOR3(1000.0f, 50.0f, 0.0f));
	m_pScore->SetSize(D3DXVECTOR3(200.0f, 400.0f, 0.0f));

	// 位置の設定
	SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));

	// テクスチャの設定
	m_pScore->SetTexture(16);

	CPlayer** pPlayer = CGame::GetPlayer();

	int max = CApplication::GetPersonCount();

	for (int i = 0; i < max; i++)
	{
		pPlayer[i]->SetAction(false);
	}

	// 時間の停止
	CGame::GetTime()->StopTime(true);

	// 出現の停止
	CGame::GetObstacle()->SetStop(true);

	// ポーズできなくする
	CApplication::GetPause()->SetSelect(false);
}
