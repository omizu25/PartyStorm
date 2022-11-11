//**************************************************
// 
// instancing.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "instancing.h"
#include "application.h"
#include "renderer.h"
#include "camera.h"
#include "texture.h"
#include "effect.h"

#include <assert.h>

namespace
{
void CopyBuf(IDirect3DVertexBuffer9* pBuf, void* pSrc, unsigned size);	// バッファのコピー
}// namespaceはここまで

//--------------------------------------------------
// 生成
//--------------------------------------------------
CInstancing* CInstancing::Create()
{
	CInstancing* pInstancing = nullptr;

	pInstancing = new CInstancing;

	if (pInstancing != nullptr)
	{// nullチェック
		pInstancing->Init();
	}

	return pInstancing;
}

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CInstancing::CInstancing() :
	m_pVtxBuff(nullptr),
	m_pPosBuff(nullptr),
	m_pColBuff(nullptr),
	m_pSizeBuff(nullptr),
	m_pDecl(nullptr),
	m_pIndexBuff(nullptr),
	m_pShader(nullptr)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CInstancing::~CInstancing()
{
	assert(m_pVtxBuff == nullptr);
	assert(m_pPosBuff == nullptr);
	assert(m_pColBuff == nullptr);
	assert(m_pSizeBuff == nullptr);
	assert(m_pDecl == nullptr);
	assert(m_pIndexBuff == nullptr);
	assert(m_pShader == nullptr);
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void CInstancing::Init()
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	
	{// 頂点バッファ
		Vtx vtx[4] =
		{// 頂点バッファ
			{ -1.0f, +1.0f, 0.0f, 0.0f },
			{ +1.0f, +1.0f, 1.0f, 0.0f },
			{ -1.0f, -1.0f, 0.0f, 1.0f },
			{ +1.0f, -1.0f, 1.0f, 1.0f }
		};

		// 頂点バッファ作成
		pDevice->CreateVertexBuffer(sizeof(vtx), 0, 0, D3DPOOL_MANAGED, &m_pVtxBuff, 0);

		// バッファのコピー
		CopyBuf(m_pVtxBuff, vtx, sizeof(vtx));
	}

	{// インデックスバッファ
		WORD index[4] = { 0, 1, 2, 3 };

		pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndexBuff, 0);

		void *p = 0;
		m_pIndexBuff->Lock(0, 0, &p, 0);
		memcpy(p, index, sizeof(index));
		m_pIndexBuff->Unlock();
	}

	{// 頂点宣言作成
		D3DVERTEXELEMENT9 declElems[] = {
			{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	// LocalPos
			{ 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	// UV
			{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },		// WorldPos
			{ 2, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1 },		// Color
			{ 3, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	// size
			D3DDECL_END()
		};

		pDevice->CreateVertexDeclaration(declElems, &m_pDecl);
	}

	{// シェーダ作成
		ID3DXBuffer* pErrBuff = nullptr;
	
		D3DXCreateEffectFromFileA(pDevice, "data/FILE/Effect.fx", 0, 0, 0, 0, &m_pShader, &pErrBuff);
		
		if (pErrBuff != nullptr)
		{// nullチェック
			const char *errMsg = (const char*)pErrBuff->GetBufferPointer();
			OutputDebugStringA(errMsg);
			assert(false);
		}
	}

	pDevice->CreateVertexBuffer(sizeof(D3DXVECTOR4) * CEffect::MAX_EFFECT, 0, 0, D3DPOOL_MANAGED, &m_pPosBuff, 0);
	pDevice->CreateVertexBuffer(sizeof(D3DXCOLOR) * CEffect::MAX_EFFECT, 0, 0, D3DPOOL_MANAGED, &m_pColBuff, 0);
	pDevice->CreateVertexBuffer(sizeof(D3DXVECTOR2) * CEffect::MAX_EFFECT, 0, 0, D3DPOOL_MANAGED, &m_pSizeBuff, 0);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CInstancing::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{// nullチェック
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pPosBuff != nullptr)
	{// nullチェック
		m_pPosBuff->Release();
		m_pPosBuff = nullptr;
	}

	if (m_pColBuff != nullptr)
	{// nullチェック
		m_pColBuff->Release();
		m_pColBuff = nullptr;
	}

	if (m_pSizeBuff != nullptr)
	{// nullチェック
		m_pSizeBuff->Release();
		m_pSizeBuff = nullptr;
	}

	if (m_pDecl != nullptr)
	{// nullチェック
		m_pDecl->Release();
		m_pDecl = nullptr;
	}

	if (m_pIndexBuff != nullptr)
	{// nullチェック
		m_pIndexBuff->Release();
		m_pIndexBuff = nullptr;
	}

	if (m_pShader != nullptr)
	{// nullチェック
		m_pShader->Release();
		m_pShader = nullptr;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CInstancing::Draw()
{
	int numAll = CEffect::GetNumAll();

	if (numAll == 0)
	{// 表示されていない
		return;
	}

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	{// エフェクトの情報取得
		D3DXVECTOR4* worldPos = new D3DXVECTOR4[numAll];	// ワールド座標位置バッファ
		D3DXCOLOR* col = new D3DXCOLOR[numAll];
		D3DXVECTOR2* size = new D3DXVECTOR2[numAll];

		CEffect** pObject = CEffect::GetEffect();
		D3DXVECTOR3 objPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
		int num = 0;
		
		for (int i = 0; i < CEffect::MAX_EFFECT; i++)
		{
			if (pObject[i] == nullptr)
			{// nullチェック
				continue;
			}
			objPos = pObject[i]->GetPos();
			move = pObject[i]->GetMove();
			
			worldPos[num].x = objPos.x;
			worldPos[num].y = objPos.y;
			worldPos[num].z = objPos.z;
			worldPos[num].w = atan2f(move.x, move.y);

			col[num] = pObject[i]->GetCol();
			size[num] = pObject[i]->GetSize();

			num++;
		}

		assert(num == numAll);

		// バッファのコピー
		CopyBuf(m_pPosBuff, worldPos, sizeof(D3DXVECTOR4) * numAll);
		CopyBuf(m_pColBuff, col, sizeof(D3DXCOLOR) * numAll);
		CopyBuf(m_pSizeBuff, size, sizeof(D3DXVECTOR2) * numAll);

		delete[] worldPos;
		delete[] col;
		delete[] size;
	}

	// インスタンス宣言
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | numAll);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | (unsigned)1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | (unsigned)1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | (unsigned)1);

	// 頂点とインデックスを設定して描画
	pDevice->SetVertexDeclaration(m_pDecl);
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(Vtx));
	pDevice->SetStreamSource(1, m_pPosBuff, 0, sizeof(D3DXVECTOR4));
	pDevice->SetStreamSource(2, m_pColBuff, 0, sizeof(D3DXCOLOR));
	pDevice->SetStreamSource(3, m_pSizeBuff, 0, sizeof(D3DXVECTOR2));
	pDevice->SetIndices(m_pIndexBuff);

	m_pShader->SetTechnique("tech");

	m_pShader->SetTexture("g_tex", CApplication::GetTexture()->GetTexture(40));

	CCamera* pCamera = CApplication::GetCamera();

	D3DXMATRIX view = pCamera->GetMtxView();
	D3DXMATRIX proj = pCamera->GetMtxProj();

	m_pShader->SetMatrix("g_view", &view);
	m_pShader->SetMatrix("g_proj", &proj);

	UINT passNum = 0;
	m_pShader->Begin(&passNum, 0);
	m_pShader->BeginPass(0);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);

	m_pShader->EndPass();
	m_pShader->End();

	// 後始末
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSourceFreq(3, 1);

	// レンダーステートを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//: ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

namespace
{
//--------------------------------------------------
// バッファのコピー
//--------------------------------------------------
void CopyBuf(IDirect3DVertexBuffer9* pBuf, void* pSrc, unsigned size)
{
	void *p = 0;
	pBuf->Lock(0, 0, &p, 0);
	memcpy(p, pSrc, size);
	pBuf->Unlock();
}
}// namespaceはここまで
