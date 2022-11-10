//**************************************************
// 
// rankingUI.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "ranking.h"
#include "score.h"
#include "number.h"
#include "object2D.h"

#include <assert.h>
#include <stdio.h>

//==================================================
// 定義
//==================================================
const float CRanking::STD_WIDTH = 40.0f;
const float CRanking::STD_HEIGHT = 50.0f;
const char* CRanking::FILE_NAME = "data/FILE/Ranking.txt";

namespace
{
const int texIdx[CRanking::MAX_RANKING]
{
	24,
	25,
	26,
	27,
	28,
};
}

//==================================================
// 静的メンバ変数
//==================================================
int CRanking::m_score = 0;
int CRanking::m_ranking[MAX_RANKING] = {};

//--------------------------------------------------
// 生成
//--------------------------------------------------
CRanking* CRanking::Create()
{
	CRanking* pRanking = nullptr;

	pRanking = new CRanking;

	if (pRanking != nullptr)
	{// nullチェック
		// 初期化
		pRanking->Init();
	}

	return pRanking;
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void CRanking::Set(int score)
{
	m_score = score;

	// 読み込み
	Load();

	// 変更
	Change();

	// 保存
	Save();
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
int CRanking::Get(int rank)
{
	if (rank == -1)
	{// 指定の値
		return m_score;
	}

	assert(rank >= 0 && rank < MAX_RANKING);

	// 読み込み
	Load();

	return m_ranking[rank];
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CRanking::Load()
{
	FILE *pFile = nullptr;

	// ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != nullptr)
	{// ファイルが開いた場合

		for (int i = 0; i < MAX_RANKING; i++)
		{
			fscanf(pFile, "%d", &m_ranking[i]);
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// 保存
//--------------------------------------------------
void CRanking::Save()
{
	FILE *pFile = nullptr;

	// ファイルを開く
	pFile = fopen(FILE_NAME, "w");

	if (pFile != nullptr)
	{// ファイルが開いた場合

		for (int i = 0; i < MAX_RANKING; i++)
		{
			fprintf(pFile, "%d\n\n", m_ranking[i]);
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		assert(false);
	}
}

//--------------------------------------------------
// 変更
//--------------------------------------------------
void CRanking::Change()
{
	int ranking[MAX_RANKING + 1] = {};

	for (int i = 0; i < MAX_RANKING; i++)
	{
		ranking[i] = m_ranking[i];
	}

	ranking[MAX_RANKING] = m_score;

	int save = 0;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		save = ranking[i];

		for (int j = i + 1; j < MAX_RANKING + 1; j++)
		{
			if (save < ranking[j])
			{// 相手が大きかったら交代
				save = ranking[j];
			}
		}

		for (int j = i + 1; j < MAX_RANKING + 1; j++)
		{
			if (save == ranking[j])
			{// 最大値を探して交代
				ranking[j] = ranking[i];
				ranking[i] = save;
				break;
			}
		}
	}

	for (int i = 0; i < MAX_RANKING; i++)
	{
		m_ranking[i] = ranking[i];
	}
}

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CRanking::CRanking() :
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_newRank(0),
	m_time(0)
{
	for (int i = 0; i < MAX_RANKING; i++)
	{
		m_pRanking[i] = nullptr;
		m_pRank[i] = nullptr;
	}
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CRanking::~CRanking()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CRanking::Init()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_newRank = 0;
	m_time = 0;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		m_pRanking[i] = nullptr;
		m_pRank[i] = nullptr;
	}

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CRanking::Uninit()
{
	for (int i = 0; i < MAX_RANKING; i++)
	{
		if (m_pRanking[i] != nullptr)
		{// nullチェック
			// 終了
			m_pRanking[i]->Uninit();
			delete m_pRanking[i];
			m_pRanking[i] = nullptr;
		}

		if (m_pRank[i] != nullptr)
		{// nullチェック
			// 終了
			m_pRank[i]->Uninit();
			m_pRank[i] = nullptr;
		}
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CRanking::Update()
{
	if (m_newRank == -1)
	{// 指定の値
		return;
	}

	m_time++;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	col.a = 1.0f - ((cosf((m_time * 0.01f) * (D3DX_PI * 2.0f)) + 1.0f) * 0.5f) * 0.9f;

	// 色の設定
	m_pRanking[m_newRank]->SetColor(col);
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CRanking::Draw()
{
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void CRanking::Set(const D3DXVECTOR3& pos, float length)
{
	m_time = 0;

	m_pos = pos;
	m_size = D3DXVECTOR3(STD_WIDTH, STD_HEIGHT, 0.0f);

	float height = STD_HEIGHT;

	if (length <= 0.0f)
	{// 値がマイナス
		height *= -1.0f;
	}

	float posY = 0.0f;
	float interval = 3 * (m_size.x * 0.5f);
	float maxWidth = 10 * m_size.x;
	float rankPosX = 0.0f;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		posY = pos.y + (i * (length + height));

		// スコアの生成
		m_pRanking[i] = CScore::Create(Digit(m_ranking[i]), false);

		// 大きさの設定
		m_pRanking[i]->SetWholeSize(D3DXVECTOR3(1000.0f, 50.0f, 0.0f));
		m_pRanking[i]->SetSize(D3DXVECTOR3(50.0f, 50.0f, 0.0f));

		// 位置の設定
		m_pRanking[i]->SetPos(D3DXVECTOR3(pos.x, posY, 0.0f));

		// 向きの設定
		m_pRanking[i]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// スコアの設定
		m_pRanking[i]->SetScore(0);
		m_pRanking[i]->SetDestScore(m_ranking[i]);

		rankPosX = pos.x - maxWidth - interval - (STD_HEIGHT * 0.5f);

		// 位の生成
		m_pRank[i] = CObject2D::Create();
		m_pRank[i]->SetPos(D3DXVECTOR3(rankPosX - 100.0f, posY, 0.0f));
		m_pRank[i]->SetSize(D3DXVECTOR3(100.0f, 50.0f, 0.0f));
		m_pRank[i]->LoadTex(texIdx[i]);
	}

	m_newRank = -1;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		if (m_ranking[i] != m_score)
		{// 指定の値ではない
			continue;
		}

		m_newRank = i;

		break;
	}
}

//--------------------------------------------------
// 位置の設定
//--------------------------------------------------
void CRanking::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;

	float posY = 0.0f;
	float interval = 3 * (m_size.x * 0.5f);
	float maxWidth = 10 * m_size.x;
	float rankPosX = 0.0f;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		posY = pos.y + (i * (STD_HEIGHT));

		rankPosX = pos.x - maxWidth - interval - (STD_HEIGHT * 0.5f);

		// 位置の設定
		m_pRanking[i]->SetPos(D3DXVECTOR3(820.0f, posY, 0.0f));
		m_pRank[i]->SetPos(D3DXVECTOR3(rankPosX - 20.0f, posY, 0.0f));
	}
}

//--------------------------------------------------
// 向きの設定
//--------------------------------------------------
void CRanking::SetRot(const D3DXVECTOR3 &rot)
{
	m_rot = rot;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		// 向きの設定
		m_pRanking[i]->SetRot(rot);
		m_pRank[i]->SetRot(rot);
	}
}

//--------------------------------------------------
// サイズの設定
//--------------------------------------------------
void CRanking::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		// サイズの設定
		m_pRanking[i]->SetSize(size);
		m_pRank[i]->SetSize(size);
	}
}

//--------------------------------------------------
// 桁数
//--------------------------------------------------
int CRanking::Digit(int number)
{
	if (number == 0)
	{
		return 1;
	}

	return (int)log10f((float)number) + 1;
}
