#pragma once
#include "SampleBase.h"


class CD3DTerrain;
class D3DTerrainSample
	:public CSampleBase
{
public:
	D3DTerrainSample(IDirect3DDevice9 *pDevice);
	~D3DTerrainSample();



	virtual bool setup() ;

	virtual bool display(float timeDelta) ;


	virtual bool cleanup();

private:
	CD3DTerrain *TheTerrain;
	CameraBase*  TheCamera;
};

