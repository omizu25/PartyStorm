//**************************************************
// 
// obstacle_manager.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "obstacle_manager.h"
#include "obstacle.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int MIN_INTERVAL = 60;			// 間隔の最小値
const int MAX_INTERVAL = 120;			// 間隔の最大値
const int POP_INCREASE = 1200;			// 出現が増える
const int MIN_POP = 2;					// 出現の最小数
const int MAX_POP = 4;					// 出現の最大数
const float POP_POS_Z = -1000.0f;		// 出現のZの位置
const float RELEASE_POS_Z = 1250.0f;	// 解放のZの位置
const float POP_POS_X[] =
{// 出現のXの位置
	-200.0f,
	-100.0f,
	0.0f,
	100.0f,
	200.0f,
};
const int POP_MAX = sizeof(POP_POS_X) / sizeof(POP_POS_X[0]);	// 出現位置の最大数
const int BG_INTERVAL = 180;			// 背景の間隔
const float RELEASE_POS_Z_BG = 5000.0f;	// 背景の解放のZの位置
const float POP_POS_X_BG[] =
{// 背景の出現のXの位置
	-1300.0f,
	-1050.0f,
	-650.0f,
	550.0f,
	1200.0f,
};
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CObstacleManager* CObstacleManager::Create()
{
	CObstacleManager* pManager = nullptr;

	pManager = new CObstacleManager;

	if (pManager != nullptr)
	{// nullチェック
		// 初期化
		pManager->Init();
	}

	return pManager;
}

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CObstacleManager::CObstacleManager() :
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_time(0),
	m_popIdxBG(0),
	m_pop(0),
	m_interval(0),
	m_move(0.0f),
	m_game(false),
	m_stop(false)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CObstacleManager::~CObstacleManager()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CObstacleManager::Init()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_time = 0;
	m_popIdxBG = -1;
	m_pop = MIN_POP;
	m_interval = MAX_INTERVAL;
	m_move = 1.0f;
	m_game = false;
	m_stop = false;

	// 移動量の設定
	CObstacle::SetMove(m_move);

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CObstacleManager::Uninit()
{
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CObstacleManager::Update()
{
	if (m_stop)
	{// 出現を止める
		return;
	}

	m_time++;

	// 背景
	BG();

	// ゲーム
	Game();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CObstacleManager::Draw()
{
}

//--------------------------------------------------
// ゲーム
//--------------------------------------------------
void CObstacleManager::Game()
{
	if (!m_game)
	{// ゲーム中かどうか
		return;
	}

	if (m_time % m_interval == 0)
	{// 一定間隔
		int random[MAX_POP];
		int count = 0;

		while (true)
		{
			random[count] = rand() % POP_MAX;
			bool same = false;

			for (int i = 0; i < count; i++)
			{
				if (random[i] == random[count])
				{// 同じ値がある
					same = true;
					break;
				}
			}

			if (!same)
			{// 同じ値がなかった
				count++;
			}

			if (count >= m_pop)
			{// 出現数分のランダムが終わった
				break;
			}
		}

		float inverse = 1.0f;

		for (int i = 0; i < m_pop; i++)
		{
			// 生成
			CObstacle::Create(POP_POS_X[random[i]], POP_POS_Z, inverse, RELEASE_POS_Z);

			inverse *= -1.0f;
		}

		if (m_pop >= MAX_POP)
		{// 最大数を越した
			m_move += 0.05f;

			// 移動量の設定
			CObstacle::SetMove(m_move);

			m_interval--;

			if (m_interval <= MIN_INTERVAL)
			{// 最小値以下
				m_interval = MIN_INTERVAL;
			}
		}
	}

	if (m_time % POP_INCREASE == 0)
	{// 一定間隔
		m_pop++;

		if (m_pop >= MAX_POP)
		{// 最大数を越した
			m_pop = MAX_POP;
		}
	}
}
//--------------------------------------------------
// 背景
//--------------------------------------------------
void CObstacleManager::BG()
{
	if (m_time % BG_INTERVAL == 0)
	{// 一定間隔
		int random = 0;

		while (true)
		{
			random = rand() % POP_MAX;

			if (random != m_popIdxBG)
			{// 前回の出現した番号ではない
				m_popIdxBG = random;
				break;
			}
		}

		// 生成
		CObstacle::Create(POP_POS_X_BG[random], 0.0f, 1.0f, RELEASE_POS_Z_BG);
	}
}
