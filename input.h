//=============================================================================
//
// ???͏???(input.h)
// Author : ???????l
// ?T?v : ?I?u?W?F?N?g???????s??
//
//=============================================================================
#ifndef _INPUT_H_		// ???̃}?N?????`???????ĂȂ???????
#define _INPUT_H_		// ???d?C???N???[?h?h?~?̃}?N?????`

//*****************************************************************************
// ???C?u?????[?????N
//*****************************************************************************
#pragma comment(lib,"dinput8.lib")		// ???͏????ɕK?v

//*****************************************************************************
// ?C???N???[?h
//*****************************************************************************
#define DIRECTINPUT_VERSION		(0x0800)	// ?r???h???̌x???Ώ??p?}?N??
#include "dinput.h"							// ???͏????ɕK?v

//***************************************************************************
// ?}?N?????`
//***************************************************************************
// ?L?[?̍ő吔
#define NUM_KEY_MAX		(0x100)	

//=============================================================================
// ???̓N???X
// Author : ???????l
// ?T?v : ???͂??s???N???X
//=============================================================================
class CInput
{
public:
	//--------------------------------------------------------------------
	// ?R???X?g???N?^?ƃf?X?g???N?^
	//--------------------------------------------------------------------
	CInput();
	virtual ~CInput() = 0;

	//--------------------------------------------------------------------
	// ?????o?֐?
	//--------------------------------------------------------------------
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd) = 0;					// ??????
	virtual void Uninit(void);													// ?I??
	virtual void Update(void) = 0;												// ?X?V
	virtual bool GetPress(int nKey) = 0;										// ?v???X?????̎擾
	virtual bool GetTrigger(int nKey) = 0;										// ?g???K?[?????̎擾
	virtual bool GetRelease(int nKey) = 0;										// ?????[?X?????̎擾
	LPDIRECTINPUTDEVICE8 GetDevice() { return m_pDevice; }						// Device?̎擾
	LPDIRECTINPUT8 GetInput() { return m_pInput; }								// ?C???v?b?g?I?u?W?F?N?g?̃Q?b?^?[
	void SetDevice(LPDIRECTINPUTDEVICE8 pDevice) { m_pDevice = pDevice; }		// Device?̃Z?b?^?[
	void SetInput(LPDIRECTINPUT8 pInput) { m_pInput = pInput; }					// ?C???v?b?g?̃Z?b?^?[

private:
	//--------------------------------------------------------------------
	// ?????o?ϐ?
	//--------------------------------------------------------------------
	LPDIRECTINPUTDEVICE8		m_pDevice;			// ???̓f?o?C?X?ւ̃|?C???^
	LPDIRECTINPUT8				m_pInput;			// DirectInput?I?u?W?F?N?g?ւ̃|?C???^
};

#endif
