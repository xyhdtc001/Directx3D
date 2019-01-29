#pragma once
#include "SampleBase.h"

class D3DFontSample
	:public CSampleBase
{
public:
	D3DFontSample(IDirect3DDevice9* pDevice);
	~D3DFontSample();

	virtual bool setup() ;

	virtual bool display(float timeDelta) ;


	virtual bool cleanup();
};

