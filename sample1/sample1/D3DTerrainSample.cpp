#include "stdafx.h"
#include "D3DTerrainSample.h"


D3DTerrainSample::D3DTerrainSample(IDirect3DDevice9 *pDevice)
	:CSampleBase(pDevice)
{

}

D3DTerrainSample::~D3DTerrainSample()
{
}

bool D3DTerrainSample::setup()
{
	return true;
}

bool D3DTerrainSample::display(float timeDelta)
{
	return true;
}

bool D3DTerrainSample::cleanup()
{
	__super::cleanup();
	return true;
}
