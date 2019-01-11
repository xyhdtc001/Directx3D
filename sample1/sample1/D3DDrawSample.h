#pragma once

#include "SampleBase.h"

typedef struct _st_vertex_drawsamle
{
	_st_vertex_drawsamle()
	{
		_x = 0; _y = 0; _z = 0;
	}
	_st_vertex_drawsamle(float x, float y, float z)
	{
		_x = x; _y = y; _z = z;
	}
	float _x, _y,_z;
	static const DWORD FVF = D3DFVF_XYZ;
}ST_VERTEX;


class D3DDrawSample
	:public CSampleBase
{
public:
	D3DDrawSample(IDirect3DDevice9* pDevice);
	~D3DDrawSample();

	virtual bool setup();

	virtual bool display(float timeDelta) { return true; };


	virtual bool cleanup() { return true; };
private:

	ID3DXMesh* m_Teapot;

	float m_rotationY;
};

