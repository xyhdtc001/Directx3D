#pragma once
#include "SampleBase.h"

typedef struct  _st_ColorVertex
{
	_st_ColorVertex(float x,float y,float z,D3DXCOLOR cr)
	{
		_x = x;
		_y = y;
		_z = z;
		_color = cr;
	}
	float _x, _y, _z;
	D3DCOLOR _color;
	static const DWORD FVF;
}ColorVertex ,*LP_ColorVertex;

class D3DColorSample :
	public CSampleBase
{
public:
	D3DColorSample(IDirect3DDevice9* pDevice);
	~D3DColorSample();


	virtual bool setup() ;

	virtual bool display(float timeDelta) ;

	virtual bool cleanup() ;


protected:
	D3DXMATRIX m_world;
};

