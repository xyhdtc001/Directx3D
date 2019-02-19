#pragma once
#include "SampleBase.h"
#include "PSystem.h"

class CameraBase;
class D3DParticleSample
	:public CSampleBase
{
public:
	D3DParticleSample(IDirect3DDevice9*pDevice);
	~D3DParticleSample();

	virtual bool setup() ;

	virtual bool display(float timeDelta) ;


	virtual bool cleanup();


protected:
	psys::PSystem* m_Exp ;
	psys::PSystem* m_Snow;
	psys::PSystem* m_Gun;
};

