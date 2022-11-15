//**************************************************
// 
// obstacle.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "obstacle.h"
#include "model3D.h"
#include "player.h"
#include "move.h"
#include "game.h"
#include "application.h"
#include "time.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
namespace
{
const int MODEL_TYPE = 17;		// モデルの種類
const float MOVE_SPEED = 5.0f;	// 移動速度
const float POS_Y = -25.0f;		// Yの位置
}

//==================================================
// 定義
//==================================================
float CObstacle::m_move = 0.0f;

//--------------------------------------------------
// 生成
//--------------------------------------------------
CObstacle* CObstacle::Create(float posX, float posZ, float inverse, float diePosZ)
{
	// オブジェクトインスタンス
	CObstacle* pObstacle = new CObstacle;

	// メモリの確保ができなかった
	assert(pObstacle != nullptr);

	// 初期化
	pObstacle->Init();

	// 位置の設定
	pObstacle->SetPos(D3DXVECTOR3(posX, 0.0f, posZ));

	pObstacle->m_inverse = inverse;
	pObstacle->m_diePosZ = diePosZ;

	// インスタンスを返す
	return pObstacle;
}

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CObstacle::CObstacle() :
	m_waveTime(0),
	m_inverse(0.0f),
	m_diePosZ(0.0f)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CObstacle::~CObstacle()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
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
	m_diePosZ = 0.0f;

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CObstacle::Uninit()
{
	// 終了
	CModelObj::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CObstacle::Update()
{
	m_waveTime++;

	// 移動
	Move();

	// 回転
	Rot();

	if (CModelObj::GetPos().z >= m_diePosZ)
	{// 解放する位置を越した
		Uninit();
		return;
	}

	// 当たり判定
	Collison();

	// 更新
	CModelObj::Update();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CObstacle::Draw()
{
	// 描画
	CModelObj::Draw();
}

//--------------------------------------------------
// 移動
//--------------------------------------------------
void CObstacle::Move()
{
	// 位置の取得
	D3DXVECTOR3 pos = CModelObj::GetPos();

	// 移動
	pos.z += (MOVE_SPEED * m_move);

	if (CApplication::GetMode() == CApplication::MODE_GAME &&
		CGame::GetTime()->GetTime() <= 0)
	{// ゲーム中で制限時間切れ
		pos.y += -1.0f;
	}
	else if (pos.z >= 4500.0f)
	{// 指定の値以上
		pos.y += -1.0f;
	}
	else
	{
		// 上下移動
		pos.y = sinf((m_waveTime * 0.01f) * (D3DX_PI * 2.0f)) * 20.0f;
		pos.y += POS_Y;
	}

	// 位置の設定
	CModelObj::SetPos(pos);
}

//--------------------------------------------------
// 回転
//--------------------------------------------------
void CObstacle::Rot()
{
	// 向きの取得
	D3DXVECTOR3 rot = CModelObj::GetRot();

	rot.z = sinf((m_waveTime * 0.01f) * (D3DX_PI * 2.0f)) * (D3DX_PI * 0.03f * m_inverse);

	// 向きの設定
	CModelObj::SetRot(rot);
}

//--------------------------------------------------
// 当たり判定
// Author : 唐﨑結斗
//--------------------------------------------------
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
		{
			// 現在のオブジェクトの次のオブジェクトを保管
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
