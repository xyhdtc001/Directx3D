// sample1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "resource.h"
#include <windows.h>
#include <d3dx9.h>
#include "d3dUtility.h"
#include "D3DDrawSample.h"
#include "D3DColorSample.h"
#include "D3DLightingSample.h"
#include "D3DTextureSample.h"

HINSTANCE hInst;                                // 当前实例

CSampleBase *gCurSample = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

IDirect3DDevice9 * pdevice = NULL;

bool Display(float timeDelta)
{
	if (gCurSample)
	{
		gCurSample->display(timeDelta);
	}
	else if (pdevice)
	{
		pdevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0x00000000, 1.0f, 0);
		pdevice->Present(0, 0, 0, 0); // 页面切换
	}
	return true;
}

void Step()
{

}



int main()
{
	hInst =  (HINSTANCE)GetModuleHandle(NULL);

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = _T("");
	wcex.lpszClassName = _T("dx");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	DWORD a = RegisterClassExW(&wcex);


	HWND hWnd = CreateWindow(_T("dx"), _T("dx"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);
	DWORD dwError = GetLastError();
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	
	if (!d3d::InitD3D((HINSTANCE)hWnd,800,600,true, D3DDEVTYPE_HAL,&pdevice))
	{
		return 0;
	}
	SetWindowPos(hWnd, NULL, 0,0,800,600,SWP_NOMOVE);
	gCurSample = new D3DTextureSample(pdevice);

	gCurSample->setup();

	d3d::EnterMsgLoop(&Display);

	gCurSample->cleanup();
	d3d::Delete<CSampleBase*>(gCurSample);
	pdevice->Release();
    return 0;
}

