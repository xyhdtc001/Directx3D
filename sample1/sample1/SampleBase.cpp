#include "stdafx.h"
#include "SampleBase.h"
#include "d3dUtility.h"


CSampleBase::CSampleBase(IDirect3DDevice9* pDevice)
	:m_ib(NULL)
	,m_vb(NULL)
{
	m_device = pDevice;
	m_width = 800;
	m_heigth = 600;
}


CSampleBase::~CSampleBase()
{
}

bool CSampleBase::cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(m_vb);
	d3d::Release<IDirect3DIndexBuffer9*>(m_ib);
	return true;
}
