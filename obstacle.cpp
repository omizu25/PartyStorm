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

//*****************************************************************************
// 定数定義
//*****************************************************************************
namespace
{
const int MODEL_TYPE = 23;				// モデルの種類
const int POP_INTERVAL = 120;			// 出現の間隔
const float MOVE_SPEED = 5.0f;			// 移動速度
const float POP_POS_Z = -1500.0f;		// 出現のZの位置
const float RELEASE_POS_Z = 1500.0f;	// 解放のZの位置
const D3DXVECTOR2 POP_POS_X[] =			// 出現のXの位置
{
	D3DXVECTOR2(-60.0f, -30.0f),
	D3DXVECTOR2(-60.0f, 0.0f),
	D3DXVECTOR2(-60.0f, 30.0f),
	D3DXVECTOR2(-60.0f, 60.0f),
	D3DXVECTOR2(-30.0f, 0.0f),
	D3DXVECTOR2(-30.0f, 30.0f),
	D3DXVECTOR2(-30.0f, 60.0f),
	D3DXVECTOR2(0.0f, 30.0f),
	D3DXVECTOR2(0.0f, 60.0f),
	D3DXVECTOR2(30.0f, 60.0f),
};
const int POP_MAX = sizeof(POP_POS_X) / sizeof(POP_POS_X[0]);	// 出現位置の最大数
}

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
int CObstacle::m_time = 0;

//=============================================================================
// 出現
//=============================================================================
void CObstacle::Pop()
{
	m_time++;

	if (m_time % POP_INTERVAL == 0)
	{
		int randam = rand() % POP_MAX;

		Create(POP_POS_X[randam].x);
		Create(POP_POS_X[randam].y);
	}
}

//=============================================================================
// 生成
//=============================================================================
CObstacle* CObstacle::Create(float posX)
{
	// オブジェクトインスタンス
	CObstacle* pObstacle = new CObstacle;

	// メモリの確保ができなかった
	assert(pObstacle != nullptr);

	// 初期化
	pObstacle->Init();

	// 位置の設定
	pObstacle->SetPos(D3DXVECTOR3(posX, 0.0f, POP_POS_Z));

	// インスタンスを返す
	return pObstacle;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CObstacle::CObstacle()
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

	pos.z += MOVE_SPEED;

	// 位置の設定
	CModelObj::SetPos(pos);

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
	for (int nCntPriority = 0; nCntPriority < MAX_LEVEL; nCntPriority++)
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


