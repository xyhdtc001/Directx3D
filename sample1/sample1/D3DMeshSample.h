#pragma once
#include "SampleBase.h"

class D3DMeshSample
	:public CSampleBase
{
public:
	D3DMeshSample(IDirect3DDevice9* pDevice);
	~D3DMeshSample();

	virtual bool setup() ;

	virtual bool display(float timeDelta) ;


	virtual bool cleanup();

};

