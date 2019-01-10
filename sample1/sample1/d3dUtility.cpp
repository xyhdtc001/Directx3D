#include "stdafx.h"
#include "d3dUtility.h"

IDirect3D9 *m_d3d9 = NULL;


bool d3d::InitD3D(HINSTANCE hInstance, /* [in] 应用程序实例 */ int width, int height, /* [in] Back buffer尺寸 */ bool windowed, /* [in] 是否全屏 */ D3DDEVTYPE deviceType, /* [in] HAL 或 REF */ IDirect3DDevice9** device)
{
	m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	D3DCAPS9 caps;
	m_d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT, // 主显示设备
		deviceType, // 设备类型，一般是D3DDEVTYPE_HAL.
		&caps); // 返回填充后的D3DCAPS9 结构，包含主显示设备的能力
				// 是否可以使用硬件顶点处理?
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		// 是，支持硬件顶点处理
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		// 不，只能用软件顶点处理
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // 像素格式
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow =(HWND) hInstance;
	d3dpp.Windowed = windowed; // fullscreen
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; // depth format
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = m_d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		D3DDEVTYPE_HAL, // device type
		(HWND)hInstance, // window associated with device
		vp, // vertex processing type
		&d3dpp, // present parameters
		device); // returned created device
	if (FAILED(hr))
	{
		::MessageBox(0, _T("CreateDevice() - FAILED"), 0, 0);
		return 0;
	}
	return true;
}


int d3d::EnterMsgLoop(bool(*ptr_display)(float timeDelta))
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	static float lastTime = (float)timeGetTime();
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			ptr_display(timeDelta); // call display function
			lastTime = currTime;
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}