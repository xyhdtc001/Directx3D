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

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d,D3DXCOLOR s, D3DXCOLOR e, float p);

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction,D3DXCOLOR* color);


	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0, 255));


	const D3DXCOLOR BEACH_SAND(D3DCOLOR_XRGB(255, 249, 157));
	const D3DXCOLOR DESERT_SAND(D3DCOLOR_XRGB(250, 205, 135));

	const D3DXCOLOR LIGHTGREEN(D3DCOLOR_XRGB(60, 184, 120));
	const D3DXCOLOR  PUREGREEN(D3DCOLOR_XRGB(0, 166, 81));
	const D3DXCOLOR  DARKGREEN(D3DCOLOR_XRGB(0, 114, 54));

	const D3DXCOLOR LIGHT_YELLOW_GREEN(D3DCOLOR_XRGB(124, 197, 118));
	const D3DXCOLOR  PURE_YELLOW_GREEN(D3DCOLOR_XRGB(57, 181, 74));
	const D3DXCOLOR  DARK_YELLOW_GREEN(D3DCOLOR_XRGB(25, 123, 48));

	const D3DXCOLOR LIGHTBROWN(D3DCOLOR_XRGB(198, 156, 109));
	const D3DXCOLOR DARKBROWN(D3DCOLOR_XRGB(115, 100, 87));


	const D3DXCOLOR redAmbient(1.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR blueDiffuse(0.0f, 0.0f, 1.0f, 1.0f);
	const D3DXCOLOR whiteSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK,8.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED, RED, RED, BLACK, 8.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN, GREEN, GREEN, BLACK,8.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE, BLUE, BLUE, BLACK, 8.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW,BLACK, 8.0f);

	float Lerp(float a, float b, float t);

	const float _INFINITY = FLT_MAX;
	const float EPSILON = 0.001f;

	//
	// Bounding Objects
	//

	struct BoundingBox
	{
		BoundingBox();

		bool isPointInside(D3DXVECTOR3& p);

		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};

	struct BoundingSphere
	{
		BoundingSphere();

		D3DXVECTOR3 _center;
		float       _radius;
	};

	static DWORD  FtoDw(float f)
	{
		return *((DWORD*)&f);
	}

	float GetRandomFloat(float lowBound, float highBound);

	void GetRandomVector(D3DXVECTOR3* out,
		D3DXVECTOR3* min,
		D3DXVECTOR3* max);


	struct Vertex
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float nx, float ny, float nz,
			float u, float v)
		{
			_x = x;  _y = y;  _z = z;
			_nx = nx; _ny = ny; _nz = nz;
			_u = u;  _v = v;
		}
		float _x, _y, _z;
		float _nx, _ny, _nz;
		float _u, _v;

		static const DWORD FVF;
	};
}
#endif