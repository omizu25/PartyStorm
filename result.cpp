//=============================================================================
//
// リザルトクラス(result.cpp)
// Author : 唐�ｱ結斗
// 概要 : リザルトクラスの管理を行う
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <assert.h>

#include "result.h"
#include "keyboard.h"
#include "object2D.h"
#include "sound.h"

//=============================================================================
// コンストラクタ
// Author : 唐�ｱ結斗
// 概要 : インスタンス生成時に行う処理
//=============================================================================
CResult::CResult()
{
	m_nextMode = CApplication::MODE_NONE;		// 次に設定するモード
}

//=============================================================================
// デストラクタ
// Author : 唐�ｱ結斗
// 概要 : インスタンス終了時に行う処理
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化
// Author : 唐�ｱ結斗
// 概要 : 頂点バッファを生成し、メンバ変数の初期値を設定
//=============================================================================
HRESULT CResult::Init()
{
	// 次に行くモードの設定
	m_nextMode = CApplication::MODE_TITLE;

	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// リザルトBGMの再生
	pSound->PlaySound(CSound::SOUND_LABEL_RESULTBGM);

	return S_OK;
}

//=============================================================================
// 終了
// Author : 唐�ｱ結斗
// 概要 : テクスチャのポインタと頂点バッファの解放
//=============================================================================
void CResult::Uninit()
{
	// サウンド情報の取得
	CSound *pSound = CApplication::GetSound();

	// サウンド終了
	pSound->StopSound();

	// スコアの解放
	Release();
}

//=============================================================================
// 更新
// Author : 唐�ｱ結斗
// 概要 : 更新を行う
//=============================================================================
void CResult::Update()
{
	// 入力情報の取得
	CKeyboard *pKeyboard = CApplication::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_RETURN))
	{
		CApplication::SetNextMode(m_nextMode);
	}
}

//=============================================================================
// 描画
// Author : 唐�ｱ結斗
// 概要 : 描画を行う
//=============================================================================
void CResult::Draw()
{

}

