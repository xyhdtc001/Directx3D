#pragma once
#include "SampleBase.h"

class D3DRenderMirrorSample
	:public CSampleBase
{
public:
	D3DRenderMirrorSample(IDirect3DDevice9* pDevice);
	~D3DRenderMirrorSample();

	virtual bool setup() ;

	virtual bool display(float timeDelta) ;


	virtual bool cleanup();

	void render_mirror();

	void render_shadow();

	void render_scence();

protected:

};

