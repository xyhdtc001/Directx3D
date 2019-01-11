#include "stdafx.h"
#include "D3DDrawSample.h"


const int  Width = 800;
const int  Heigth = 600;

//static const DWORD ST_VERTEX::FVF = D3DFVF_XYZ;

D3DDrawSample::D3DDrawSample(IDirect3DDevice9* pDevice)
	:CSampleBase(pDevice),
	m_vb(NULL),
	m_ib(NULL)
{
	m_device = pDevice;
}


D3DDrawSample::~D3DDrawSample()
{
}

bool D3DDrawSample::setup()
{
	D3DSAMPLE_CHECKPTR(m_device, false);

	_DX_CHECK(m_device->CreateVertexBuffer(8*sizeof(ST_VERTEX),D3DUSAGE_WRITEONLY,ST_VERTEX::FVF,D3DPOOL_MANAGED,&m_vb,0));

	return true;
}
