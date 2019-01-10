#pragma once
#include <d3dx9.h>

#ifndef _d3d_utility_h
#define _d3d_utility_h

namespace d3d
{
	bool InitD3D(
		HINSTANCE hInstance, // [in] Ӧ�ó���ʵ��
		int width, int height, // [in] Back buffer�ߴ�
		bool windowed, // [in] �Ƿ�ȫ��
		D3DDEVTYPE deviceType, // [in] HAL �� REF
		IDirect3DDevice9** device); // [out] �������豸

	int EnterMsgLoop(
		bool(*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}
	template<class T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}
}

#endif