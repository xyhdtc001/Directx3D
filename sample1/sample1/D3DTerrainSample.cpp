#include "stdafx.h"
#include "D3DTerrainSample.h"
#include "d3dUtility.h"
#include "D3DTerrain.h"
#include "CameraBase.h"


D3DTerrainSample::D3DTerrainSample(IDirect3DDevice9 *pDevice)
	:CSampleBase(pDevice)
{

}

D3DTerrainSample::~D3DTerrainSample()
{
}

bool D3DTerrainSample::setup()
{
	TheCamera = new CameraBase(CameraBase::LANDOBJECT);
	D3DXVECTOR3 lightDirection(0.0f, 1.0f, 0.0f);
	TheTerrain  = new CD3DTerrain(m_device, "dx_res\\coastMountain64.raw", 64, 64, 10, 0.5f);
	TheTerrain->genTexture(&lightDirection);


	//
	// Set texture filters.
	//

	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	// Set projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		(float)m_width / (float)m_heigth,
		1.0f,
		1000.0f);
	m_device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

bool D3DTerrainSample::display(float timeDelta)
{

	if (m_device)
	{
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			TheCamera->walk(100.0f * timeDelta);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			TheCamera->walk(-100.0f * timeDelta);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			TheCamera->yaw(-1.0f * timeDelta);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			TheCamera->yaw(1.0f * timeDelta);

		if (::GetAsyncKeyState('N') & 0x8000f)
			TheCamera->strafe(-100.0f * timeDelta);

		if (::GetAsyncKeyState('M') & 0x8000f)
			TheCamera->strafe(100.0f * timeDelta);

		if (::GetAsyncKeyState('W') & 0x8000f)
			TheCamera->pitch(1.0f * timeDelta);

		if (::GetAsyncKeyState('S') & 0x8000f)
			TheCamera->pitch(-1.0f * timeDelta);

		D3DXVECTOR3 pos;
		TheCamera->getPosition(&pos);
		float height = TheTerrain->getHeight(pos.x, pos.z);
		pos.y = height + 5.0f; // add height because we're standing up
		TheCamera->setPosition(&pos);

		D3DXMATRIX V;
		TheCamera->getViewMatrix(&V);
		m_device->SetTransform(D3DTS_VIEW, &V);

		//
		// Draw the scene:
		//

		m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
		m_device->BeginScene();

		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);

		if (TheTerrain)
			TheTerrain->draw(&I, true);


		m_device->EndScene();
		m_device->Present(0, 0, 0, 0);
	}
	return true;
}

bool D3DTerrainSample::cleanup()
{
	d3d::Delete<CD3DTerrain*>(TheTerrain);
	d3d::Delete<CameraBase*>(TheCamera);
	__super::cleanup();
	return true;
}
