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
	_DX_CHECK(m_device->CreateVertexBuffer(3 * sizeof(ColorVertex), D3DUSAGE_WRITEONLY, ColorVertex::FVF, D3DPOOL_MANAGED,&m_vb,0));

	ColorVertex *cv;
	m_vb->Lock(0, 0, (void**)&cv, 0);

	cv[0] = ColorVertex(-1.0f,0.0f,2.0f,D3DCOLOR_XRGB(255,0,0));
	cv[1] = ColorVertex(0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	cv[2] = ColorVertex(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));

	m_vb->Unlock();


	_BASE_PROJ_SETTING;

	m_device->SetRenderState(D3DRS_LIGHTING,false);

	return true;
}

bool D3DColorSample::display(float timeDelta)
{
	if (m_device)
	{
		m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		m_device->BeginScene();
		m_device->SetFVF(ColorVertex::FVF);
		m_device->SetStreamSource(0, m_vb, 0, sizeof(ColorVertex));

		D3DXMatrixTranslation(&m_world, -1.25f, 0.0f, 0.0f);
		m_device->SetTransform(D3DTS_WORLD, &m_world);

		m_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		m_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,1);

		D3DXMatrixTranslation(&m_world, 1.25f, 0.0f, 0.0f);
		m_device->SetTransform(D3DTS_WORLD, &m_world);

		m_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		m_device->EndScene();
		m_device->Present(0, 0, 0, 0);

	}
	return true;
}

bool D3DColorSample::cleanup()
{
	__super::cleanup();
	return true;
}
