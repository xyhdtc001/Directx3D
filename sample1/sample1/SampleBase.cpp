#include "stdafx.h"
#include "SampleBase.h"
#include "d3dUtility.h"
#include <fstream>




CSampleBase::CSampleBase(IDirect3DDevice9* pDevice)
	:m_ib(NULL)
	,m_vb(NULL)
{
	m_device = pDevice;
	m_width = 800;
	m_heigth = 600;
	m_baseFrameCnt = 0;
	m_baseTimeElapsed = 0;
	m_Basefps = 0;
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

void CSampleBase::calc_FPS(float timeDelta)
{
	m_baseFrameCnt++;
	m_baseTimeElapsed += timeDelta;
	if (m_baseTimeElapsed >= 1.0f)
	{
		m_Basefps = (float)m_baseFrameCnt / m_baseTimeElapsed;
		m_baseTimeElapsed = 0.0f;
		m_baseFrameCnt = 0;
	}
}

