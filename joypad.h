//=============================================================================
//
// ジョイパッドクラス(joypad.h)
// Author : 唐﨑結斗
// Author : 磯江寿希亜
// 概要 : ジョイパッドクラスの管理を行う
//
//=============================================================================
#ifndef _JOYPAD_H_		//このマクロ定義がされなかったら
#define _JOYPAD_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//ライブラリーファイルのリンク
//----------------------------------------------------------------------------
#pragma comment(lib,"dinput8.lib")		// 入力処理に必要(キーボード)
#pragma comment(lib,"xinput.lib")		// 入力処理に必要（ジョイパット）
#pragma comment(lib,"dxguid.lib")		// DirectXコンポーネントに使用

//----------------------------------------------------------------------------
//インクルード
//----------------------------------------------------------------------------
#include <Windows.h>
#pragma push_macro("new")
#undef new
#include <d3dx9.h>
#pragma pop_macro("new")
#define	 DIRECTINPUT_VERSION (0x0800)	// ビルド時の警告対策用マクロ(この位置から動かさない)
#include <Xinput.h>						// ジョイパット処理
#include <dinput.h>						// 入力処理に必要

//=============================================================================
// ジョイパッドクラス
// Author : 唐﨑結斗
// Author : 磯江寿希亜
// 概要 : ジョイパッドクラス生成を行うクラス
//=============================================================================
class CJoypad
{
public:
	//--------------------------------------------------------------------
	// キー種類の列挙型
	//--------------------------------------------------------------------
	typedef enum
	{
		JOYKEY_UP = 0,				// 十字ボタン上
		JOYKEY_DOWN,				// 十字ボタン下
		JOYKEY_LEFT,				// 十字ボタン左
		JOYKEY_RIGHT,				// 十字ボタン右
		JOYKEY_START,				// スタートボタン
		JOYKEY_BACK,				// バックボタン
		JOYKEY_LEFT_THUMB,			// 左スティック押込み
		JOYKEY_RIGHT_THUMB,			// 右スティック押込み
		JOYKEY_LEFT_SHOULDER,		// L１ボタン
		JOYKEY_RIGHT_SHOULDER,		// R１ボタン
		JOYKEY_LEFT_TRIGGER,		// L２トリガー
		JOYKEY_RIGHT_TRIGGER,		// R２トリガー
		JOYKEY_A,					// Aボタン
		JOYKEY_B,					// Bボタン
		JOYKEY_X,					// Xボタン
		JOYKEY_Y,					// Yボタン
		JOYKEY_LEFT_STICK,			// 左スティック
		JOYKEY_RIGHT_STICK,			// 右スティック
		JOYKEY_MAX
	}JOYKEY;

	//--------------------------------------------------------------------
	// ジョイパッド情報の構造体
	//--------------------------------------------------------------------
	struct JOYPAD
	{
		XINPUT_STATE		press;				// ジョイパットのプレス情報
		XINPUT_STATE		trigger;			// ジョイパットのトリガー情報
		D3DXVECTOR3			joyStickPos;		// ジョイスティックの傾き
		XINPUT_VIBRATION	vibration;			// ジョイパッドのバイブレーション
		WORD				wStrength;			// 振動の強さ (0 - 65535)
		int					nTime;				// 振動持続時間
	};

	//--------------------------------------------------------------------
	// コンストラクタとデストラクタ
	//--------------------------------------------------------------------
	CJoypad();
	~CJoypad();

	//--------------------------------------------------------------------
	// メンバ関数
	//--------------------------------------------------------------------
	HRESULT Init(HINSTANCE hInstance, HWND hWnd,const int nMax);		// 入力処理全部の初期化
	void Uninit(void);													// 入力処理全部の終了処理
	void Update(void);													// 入力処理全部の更新処理
	bool GetPress(JOYKEY Key, int nPlayer);								// ジョイパッドプレス処理
	bool GetTrigger(JOYKEY Key, int nPlayer);							// ジョイパッドトリガー処理
	D3DXVECTOR3 GetStick(JOYKEY Key, int nPlayer);						// ジョイパッドスティック処理
	int GetTriggerPedal(JOYKEY Key, int nPlayer);						// ジョイパッドトリガーペダル処理
	void Vibration(int nTime, WORD nStrength, int nPlayer);				// ジョイパッド振動制御
	int GetUseJoyPad() { return m_nUseJoyPad; }							// コントローラーの使用数の取得

private:	
	//--------------------------------------------------------------------
	// メンバ変数
	//--------------------------------------------------------------------
	LPDIRECTINPUT8			m_pInput;					// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8	m_pDevJoypad;				// 入力デバイス（キーボード（コントローラー用は別に作る））へのポインタ
	JOYPAD					*m_pJoyPad;					// ジョイパッド情報
	int						m_nMax;						// コントローラーの使用可能数
	int						m_nUseJoyPad;				// 使用しているコントローラー数
};

#endif