#include "stdafx.h"
#include "D3DColorSample.h"



const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

D3DColorSample::D3DColorSample(IDirect3DDevice9* pDevice) : CSampleBase(pDevice)
{
}

D3DColorSample::~D3DColorSample()
{
}

bool D3DColorSample::setup()
{
	return true;
}

bool D3DColorSample::display(float timeDelta)
{
	return true;
}

bool D3DColorSample::cleanup()
{
	return true;
}
