//=============================================================================
//
// 障害物クラス　(obstacle.cpp)
// Author : 香月瑞輝
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "obstacle.h"
#include "model3D.h"
#include "player.h"
#include "move.h"
#include "game.h"
#include "application.h"
#include "time.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
namespace
{
const int MODEL_TYPE = 16;				// モデルの種類
const int MIN_INTERVAL = 60;			// 間隔の最小値
const int MAX_INTERVAL = 120;			// 間隔の最小値
const int POP_INCREASE = 1200;			// 出現が増える
const int MIN_POP = 2;					// 出現の最小数
const int MAX_POP = 4;					// 出現の最大数
const float MOVE_SPEED = 5.0f;			// 移動速度
const float POP_POS_Z = -1000.0f;		// 出現のZの位置
const float POS_Y = -25.0f;				// Yの位置
const float RELEASE_POS_Z = 1250.0f;	// 解放のZの位置
const float POP_POS_X[] =				// 出現のXの位置
{
	-200.0f,
	-100.0f,
	0.0f,
	100.0f,
	200.0f,
};
const int POP_MAX = sizeof(POP_POS_X) / sizeof(POP_POS_X[0]);	// 出現位置の最大数
}

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CObstacle::m_time = 0;
int CObstacle::m_pop = 0;
int CObstacle::m_interval = 0;
float CObstacle::m_move = 0.0f;
bool CObstacle::m_stop = false;

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
void CObstacle::InitStatic()
{
	m_time = 0;
	m_pop = MIN_POP;
	m_interval = MAX_INTERVAL;
	m_move = 1.0f;
}

//=============================================================================
// 出現
//=============================================================================
void CObstacle::Pop()
{
	if (m_stop)
	{// 出現を止める
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
			{// 出現数のランダムが終わった
				break;
			}
		}

		float inverse = 1.0f;

		for (int i = 0; i < m_pop; i++)
		{
			// 生成
			Create(POP_POS_X[random[i]], inverse);

			inverse *= -1.0f;
		}

		if (m_pop >= MAX_POP)
		{// 最大数を越した
			m_move += 0.05f;
			m_interval--;

			if (m_interval <= MIN_INTERVAL)
			{// 最小値以下
				m_interval = MIN_INTERVAL;
			}
		}
	}

	m_time++;

	if (m_time % POP_INCREASE == 0)
	{// 一定間隔
		m_pop++;

		if (m_pop >= MAX_POP)
		{// 最大数を越した
			m_pop = MAX_POP;
		}
	}
}

//=============================================================================
// 出現を止めるかどうか
//=============================================================================
void CObstacle::Stop(bool stop)
{
	m_stop = stop;
}

//=============================================================================
// 生成
//=============================================================================
CObstacle* CObstacle::Create(float posX, float inverse)
{
	// オブジェクトインスタンス
	CObstacle* pObstacle = new CObstacle;

	// メモリの確保ができなかった
	assert(pObstacle != nullptr);

	// 初期化
	pObstacle->Init();

	// 位置の設定
	pObstacle->SetPos(D3DXVECTOR3(posX, 0.0f, POP_POS_Z));

	pObstacle->m_inverse = inverse;

	// インスタンスを返す
	return pObstacle;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CObstacle::CObstacle() :
	m_waveTime(0),
	m_inverse(0.0f)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CObstacle::~CObstacle()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CObstacle::Init()
{
	// 初期化
	CModelObj::Init();

	// 種類の設定
	CModelObj::SetType(MODEL_TYPE);

	// 当たり判定の設定
	CModel3D::MODEL_MATERIAL *material = GetModel()->GetMaterial();
	D3DXVECTOR3 size = material[GetModel()->GetModelID()].size;
	SetColisonSize(size);
	SetColisonPos(D3DXVECTOR3(0.0f, size.y / 2.0f, 0.0f));

	m_waveTime = 0;
	m_inverse = 0.0f;

	return E_NOTIMPL;
}

//=============================================================================
// 終了
//=============================================================================
void CObstacle::Uninit()
{
	// 終了
	CModelObj::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CObstacle::Update()
{
	// 位置の取得
	D3DXVECTOR3 pos = CModelObj::GetPos();

	pos.z += (MOVE_SPEED * m_move);

	m_waveTime++;

	if (CApplication::GetMode() == CApplication::MODE_GAME &&
		CGame::GetTime()->GetTime() <= 0)
	{// ゲーム中で制限時間切れ
		pos.y += -1.0f;
	}
	else
	{
		pos.y = sinf((m_waveTime * 0.01f) * (D3DX_PI * 2.0f)) * 20.0f;

		pos.y += POS_Y;
	}

	// 位置の設定
	CModelObj::SetPos(pos);

	// 向きの取得
	D3DXVECTOR3 rot = CModelObj::GetRot();

	rot.z = sinf((m_waveTime * 0.01f) * (D3DX_PI * 2.0f)) * (D3DX_PI * 0.03f * m_inverse);

	// 向きの設定
	CModelObj::SetRot(rot);

	if (pos.z >= RELEASE_POS_Z)
	{// 解放する位置を越した
		Uninit();
		return;
	}

	// 当たり判定
	Collison();

	// 更新
	CModelObj::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CObstacle::Draw()
{
	// 描画
	CModelObj::Draw();
}

//=============================================================================
// 当たり判定
// Author : 唐﨑結斗
// 概要 : 当たり判定を行う
//=============================================================================
void CObstacle::Collison()
{
	if (CApplication::GetMode() == CApplication::MODE_GAME &&
		CGame::GetTime()->GetTime() <= 0)
	{// ゲーム中で制限時間切れ
		return;
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 変数宣言
		CObject *pObject = CObject::GetTop(nCntPriority);

		while (pObject != nullptr)
		{// 現在のオブジェクトの次のオブジェクトを保管
			CObject *pObjectNext = pObject->GetNext();

			if (pObject->GetFlagDeath() ||
				(pObject->GetObjType() != OBJTYPE_3DPLAYER) ||
				!pObject->ColisonRectangle3D(this, true))
			{
				// 現在のオブジェクトの次のオブジェクトを更新
				pObject = pObjectNext;
				continue;
			}

			// プレイヤーの移動
			CPlayer *pPlayer = dynamic_cast<CPlayer*>(pObject);
			CMove *pMove = pPlayer->GetMove();
			D3DXVECTOR3 pos = pPlayer->GetPos();
			pos.z += MOVE_SPEED - pMove->GetMove().z;
			pPlayer->SetPos(pos);

			// 現在のオブジェクトの次のオブジェクトを更新
			pObject = pObjectNext;
		}
	}
}
