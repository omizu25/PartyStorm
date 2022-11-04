//**************************************************
// 
// instancing.h
// Author  : katsuki mizuki
// 
//**************************************************
#ifndef _INSTANCING_H_	//このマクロ定義がされてなかったら
#define _INSTANCING_H_	//２重インクルード防止のマクロ定義

//==================================================
// インクルード
//==================================================
#include <d3dx9.h>

//==================================================
// 定義
//==================================================
class CInstancing
{
	/* 定義 */
private:
	struct Vtx
	{// 頂点バッファ
		float x, y;	// pos
		float u, v;	// tex
	};

	/* ↓静的メンバ関数↓ */
public:
	static CInstancing* Create();	// 生成

	/* ↓メンバ関数↓ */
public:
	CInstancing();	// デフォルトコンストラクタ
	~CInstancing();	// デストラクタ

public:
	void Init();	// 初期化
	void Uninit();	// 終了
	void Draw();	// 描画

	/* ↓メンバ変数↓ */
private:
	IDirect3DVertexBuffer9* m_pVtxBuff;		// 頂点バッファ
	IDirect3DVertexBuffer9* m_pPosBuff;		// 位置のバッファ
	IDirect3DVertexBuffer9* m_pColBuff;		// 色のバッファ
	IDirect3DVertexBuffer9* m_pSizeBuff;	// サイズのバッファ
	IDirect3DVertexDeclaration9* m_pDecl;	// 要素の宣言
	IDirect3DIndexBuffer9* m_pIndexBuff;	// インデックスバッファ
	ID3DXEffect* m_pShader;					// シェーダー
};

#endif // !_INSTANCING_H_
