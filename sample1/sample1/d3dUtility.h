#pragma once
#include <d3dx9.h>

#ifndef _d3d_utility_h
#define _d3d_utility_h

namespace d3d
{
	bool InitD3D(
		HINSTANCE hInstance, // [in] 应用程序实例
		int width, int height, // [in] Back buffer尺寸
		bool windowed, // [in] 是否全屏
		D3DDEVTYPE deviceType, // [in] HAL 或 REF
		IDirect3DDevice9** device); // [out] 创建的设备

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