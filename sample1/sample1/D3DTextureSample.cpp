#include "stdafx.h"
#include "D3DTextureSample.h"


D3DTextureSample::D3DTextureSample(IDirect3DDevice9 * pDevice)
	:CSampleBase(pDevice)
{
	m_tex1 = NULL;
}

D3DTextureSample::~D3DTextureSample()
{
}

bool D3DTextureSample::setup()
{
	m_device->CreateVertexBuffer(6 * sizeof(TextureVertex), D3DUSAGE_WRITEONLY, TextureVertex::FVF, D3DPOOL_MANAGED, &m_vb, 0);
	TextureVertex* v;
	m_vb->Lock(0, 0, (void**)&v, 0);
	// quad built from two triangles, note texture coordinates:
	v[0] = TextureVertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = TextureVertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = TextureVertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = TextureVertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = TextureVertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = TextureVertex(1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	m_vb->Unlock();

	D3DXCreateTextureFromFile(m_device,_T("dx5_logo.bmp"),&m_tex1);
	m_device->SetTexture(0, m_tex1);

	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0,D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_device->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.5f, // 90 - degree
			(float)800 / (float)600,
			1.0f,
			1000.0f);
	m_device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

bool D3DTextureSample::display(float timeDelta)
{
	if (m_device)
	{
// 		D3DXMATRIX Rx, Ry;
// 
// 		// rotate 45 degrees on x-axis
// 		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);
// 
// 		// incremement y-rotation angle each frame
// 		static float y = 0.0f;
// 		D3DXMatrixRotationY(&Ry, y);
// 		y += timeDelta;
// 
// 		// reset angle to zero when angle reaches 2*PI
// 		if (y >= 6.28f)
// 			y = 0.0f;
// 
// 		// combine x- and y-axis rotation transformations.
// 		D3DXMATRIX p = Rx * Ry;
// 
// 		m_device->SetTransform(D3DTS_WORLD, &p);

		m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f,
			0);
		m_device->BeginScene();
		m_device->SetStreamSource(0, m_vb, 0, sizeof(TextureVertex));
		m_device->SetFVF(TextureVertex::FVF);
		m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		m_device->EndScene();
		m_device->Present(0, 0, 0, 0);
	}
	return true;
}

bool D3DTextureSample::cleanup()
{
	return true;
}
