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
	m_bInitState = false;
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



static IDirect3DVertexBuffer9* _floor = 0;
static IDirect3DTexture9*      tex = 0;
static ID3DXMesh*              pillar = 0;

bool CSampleBase::DrawBasicScene(float fscale /*= 1.0f*/)
{
	if (!m_device)
	{
		return false;
	}
	IDirect3DDevice9* device = m_device;
	HRESULT hr = 0;

	if (!_floor && !tex && !pillar)
	{
		// they don't exist, create them
		device->CreateVertexBuffer(
			6 * sizeof(d3d::Vertex),
			0,
			d3d::Vertex::FVF,
			D3DPOOL_MANAGED,
			&_floor,
			0);

		d3d::Vertex* v = 0;
		_floor->Lock(0, 0, (void**)&v, 0);

		v[0] = d3d::Vertex(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[1] = d3d::Vertex(-20.0f, -2.5f, 20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[2] = d3d::Vertex(20.0f, -2.5f, 20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

		v[3] = d3d::Vertex(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[4] = d3d::Vertex(20.0f, -2.5f, 20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		v[5] = d3d::Vertex(20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

		_floor->Unlock();

		D3DXCreateCylinder(device, 0.5f, 0.5f, 5.0f, 20, 20, &pillar, 0);

		D3DXCreateTextureFromFile(
			device,
			_T("dx_res/desert.bmp"),
			&tex);
	}
	else
	{
		//
		// Pre-Render Setup
		//
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
		D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &col);

		device->SetLight(0, &light);
		device->LightEnable(0, true);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);

		//
		// Render
		//

		D3DXMATRIX T, R, P, S;

		D3DXMatrixScaling(&S, fscale, fscale, fscale);

		// used to rotate cylinders to be parallel with world's y-axis
		D3DXMatrixRotationX(&R, -D3DX_PI * 0.5f);

		// draw floor
		D3DXMatrixIdentity(&T);
		T = T * S;
		device->SetTransform(D3DTS_WORLD, &T);
		device->SetMaterial(&d3d::WHITE_MTRL);
		device->SetTexture(0, tex);
		device->SetStreamSource(0, _floor, 0, sizeof(d3d::Vertex));
		device->SetFVF(d3d::Vertex::FVF);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		// draw pillars
		device->SetMaterial(&d3d::BLUE_MTRL);
		device->SetTexture(0, 0);
		for (int i = 0; i < 5; i++)
		{
			D3DXMatrixTranslation(&T, -5.0f, 0.0f, -15.0f + (i * 7.5f));
			P = R * T * S;
			device->SetTransform(D3DTS_WORLD, &P);
			pillar->DrawSubset(0);

			D3DXMatrixTranslation(&T, 5.0f, 0.0f, -15.0f + (i * 7.5f));
			P = R * T * S;
			device->SetTransform(D3DTS_WORLD, &P);
			pillar->DrawSubset(0);
		}
	}
	return true;
}

void CSampleBase::releaseBaseScence()
{
	if (_floor && tex && pillar)
	{
		// they already exist, destroy them
		d3d::Release<IDirect3DVertexBuffer9*>(_floor);
		d3d::Release<IDirect3DTexture9*>(tex);
		d3d::Release<ID3DXMesh*>(pillar);
	}
}


