#include "stdafx.h"
#include "D3DParticleSample.h"
#include "CameraBase.h"
#include <ctime>

D3DParticleSample::D3DParticleSample(IDirect3DDevice9*pDevice)
	:CSampleBase(pDevice)
{
	m_CameraBase = new CameraBase(CameraBase::AIRCRAFT);
}

D3DParticleSample::~D3DParticleSample()
{

}

bool D3DParticleSample::setup()
{
	srand((unsigned int)time(0));

	D3DXVECTOR3 origin(0.0f, 10.0f, 50.0f);
	m_Exp = dynamic_cast<psys::PSystem*>(new psys::FireworkParticle(&origin, 6000));

	d3d::BoundingBox boudBox;
	boudBox._min = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);
	boudBox._max = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
	m_Snow = dynamic_cast<psys::PSystem*>(new psys::SnowParticle(&boudBox, 6000));
	m_Gun = dynamic_cast<psys::PSystem*>(new psys::GunParticle(m_CameraBase));;

	m_Exp->init(m_device, _T("dx_res/flare.bmp"));
	m_Snow->init(m_device,_T("dx_res/snowflake.dds"));
	m_Gun->init(m_device, _T("dx_res/flare_alpha.dds"));
	DrawBasicScene();

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI / 2.0f, // 45 - degree
		(float)m_width / (float)m_heigth,
		1.0f,
		5000.0f);

	m_device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

bool D3DParticleSample::display(float timeDelta)
{
	if (!m_device)
	{
		return false;
	}
	IDirect3DDevice9* device = m_device;

	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		m_CameraBase->walk(4.0f * timeDelta);

	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		m_CameraBase->walk(-4.0f * timeDelta);

	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		m_CameraBase->yaw(-1.0f * timeDelta);

	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		m_CameraBase->yaw(1.0f * timeDelta);

	if (::GetAsyncKeyState('N') & 0x8000f)
		m_CameraBase->strafe(-4.0f * timeDelta);

	if (::GetAsyncKeyState('M') & 0x8000f)
		m_CameraBase->strafe(4.0f * timeDelta);

	if (::GetAsyncKeyState('W') & 0x8000f)
		m_CameraBase->pitch(1.0f * timeDelta);

	if (::GetAsyncKeyState('S') & 0x8000f)
		m_CameraBase->pitch(-1.0f * timeDelta);

	D3DXMATRIX V;
	m_CameraBase->getViewMatrix(&V);
	m_device->SetTransform(D3DTS_VIEW, &V);

	m_Exp->updata(timeDelta);
	m_Snow->updata(timeDelta);
	m_Gun->updata(timeDelta);


	if (m_Exp->isDead())
		m_Exp->reset();

	//
	// Draw the scene:
	//

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	device->BeginScene();

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	device->SetTransform(D3DTS_WORLD, &I);

	DrawBasicScene(1.0f);

	device->SetTransform(D3DTS_WORLD, &I);
	m_Exp->render();
	m_Snow->render();
	m_Gun->render();

	device->EndScene();
	device->Present(0, 0, 0, 0);
	return true;
}

bool D3DParticleSample::cleanup()
{
	d3d::Delete<psys::PSystem*>(m_Exp);
	releaseBaseScence();
	return __super::cleanup();
}