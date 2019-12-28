#pragma once
#include "SDK.h"

#include <d3dx9.h>

extern bool PressedKeys[256];
extern bool was_initialized;
extern WNDPROC pOldWindowProc;
extern HWND hWindow;

typedef HRESULT(STDMETHODCALLTYPE * EndSceneFn)(IDirect3DDevice9*);
HRESULT STDMETHODCALLTYPE hkEndScene(IDirect3DDevice9* vDevice);
LRESULT STDMETHODCALLTYPE hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);