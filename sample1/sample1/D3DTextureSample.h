#pragma once

#include "SampleBase.h"

typedef struct  _st_TextureVertex
{
	_st_TextureVertex(float x,float y,float z,float nx,float ny,float nz,float u,float v)
	{
		_x = x;
		_y = y;
		_z = z;
		_nx = nx;
		_ny = ny;
		_nz = nz;
		_u = u;
		_v = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	static const DWORD FVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
}TextureVertex,*lp_TextureVertex;

class D3DTextureSample
	:public CSampleBase
{
public:
	D3DTextureSample(IDirect3DDevice9 * pDevice);
	~D3DTextureSample();

	virtual bool setup() ;

	virtual bool display(float timeDelta) ;


	virtual bool cleanup() ;


private:
	IDirect3DTexture9 * m_tex1;
};

