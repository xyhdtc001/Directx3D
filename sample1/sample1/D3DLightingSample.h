#pragma once
#include "SampleBase.h"


typedef struct _st_vertex_lightingsamle
{
	_st_vertex_lightingsamle()
	{
		_x = 0; _y = 0; _z = 0;
	}
	_st_vertex_lightingsamle(float x, float y, float z,float nx,float ny,float nz)
	{
		_x = x; _y = y; _z = z;
		_nx = nx;
		_ny = ny;
		_nz = nz;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz; //¶¥µã·¨Ïß
	static const DWORD FVF = D3DFVF_XYZ| D3DFVF_NORMAL;
}ST_VERTEX_LIGHT;

class D3DLightingSample
	:public CSampleBase
{
public:
	D3DLightingSample(IDirect3DDevice9* pDevice);
	~D3DLightingSample();
	virtual bool setup() ;

	virtual bool display(float timeDelta);


	virtual bool cleanup();

protected:
	ID3DXMesh* m_mesh ;
};

