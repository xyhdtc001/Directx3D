#include "stdafx.h"
#include "SampleBase.h"
#include "d3dUtility.h"
#include "CameraBase.h"
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
	m_CameraBase = NULL;
}


CSampleBase::~CSampleBase()
{
}

bool CSampleBase::cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(m_vb);
	d3d::Release<IDirect3DIndexBuffer9*>(m_ib);
	d3d::Delete<CameraBase*>(m_CameraBase);
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

void CSampleBase::base_camera(float timeDelta)
{
	if (!m_CameraBase)
	{
		m_CameraBase = new CameraBase(CameraBase::AIRCRAFT);
	}
	if (::GetAsyncKeyState('W') & 0x8000f)
		m_CameraBase->walk(4.0f * timeDelta);

	if (::GetAsyncKeyState('S') & 0x8000f)
		m_CameraBase->walk(-4.0f * timeDelta);

	if (::GetAsyncKeyState('A') & 0x8000f)
		m_CameraBase->strafe(-4.0f * timeDelta);

	if (::GetAsyncKeyState('D') & 0x8000f)
		m_CameraBase->strafe(4.0f * timeDelta);

	if (::GetAsyncKeyState('R') & 0x8000f)
		m_CameraBase->fly(4.0f * timeDelta);

	if (::GetAsyncKeyState('F') & 0x8000f)
		m_CameraBase->fly(-4.0f * timeDelta);

	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		m_CameraBase->pitch(1.0f * timeDelta);

	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		m_CameraBase->pitch(-1.0f * timeDelta);

	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		m_CameraBase->yaw(-1.0f * timeDelta);

	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		m_CameraBase->yaw(1.0f * timeDelta);

	if (::GetAsyncKeyState('N') & 0x8000f)
		m_CameraBase->roll(1.0f * timeDelta);

	if (::GetAsyncKeyState('M') & 0x8000f)
		m_CameraBase->roll(-1.0f * timeDelta);

	D3DXMATRIX V;
	m_CameraBase->getViewMatrix(&V);
	m_device->SetTransform(D3DTS_VIEW, &V);
}


