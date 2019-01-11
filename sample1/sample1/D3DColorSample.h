#pragma once
#include "SampleBase.h"

typedef struct  _st_ColorVertex
{
	float _x, _y, _z;
	D3DXCOLOR _color;
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


};

