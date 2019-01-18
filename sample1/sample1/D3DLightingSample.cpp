#include "stdafx.h"
#include "D3DLightingSample.h"
#include "d3dUtility.h"


D3DLightingSample::D3DLightingSample(IDirect3DDevice9* pDevice)
	:CSampleBase(pDevice)
{
	m_mesh = NULL;
}

D3DLightingSample::~D3DLightingSample()
{
}

bool D3DLightingSample::setup()
{
	//��ʼ������������vertext
	if (m_device)
	{
		m_device->SetRenderState(D3DRS_LIGHTING, true);
		m_device->CreateVertexBuffer(12 * sizeof(ST_VERTEX_LIGHT), D3DUSAGE_WRITEONLY, ST_VERTEX_LIGHT::FVF, D3DPOOL_MANAGED, &m_vb, 0);
		ST_VERTEX_LIGHT *p;
		m_vb->Lock(0, 0, (void**)&p, 0);
		// front face
		p[0] = ST_VERTEX_LIGHT(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
		p[1] = ST_VERTEX_LIGHT(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f);
		p[2] = ST_VERTEX_LIGHT(1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
		// left face
		p[3] = ST_VERTEX_LIGHT(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
		p[4] = ST_VERTEX_LIGHT(0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
		p[5] = ST_VERTEX_LIGHT(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);
		// right face
		p[6] = ST_VERTEX_LIGHT(1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
		p[7] = ST_VERTEX_LIGHT(0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
		p[8] = ST_VERTEX_LIGHT(1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);
		// back face
		p[9] = ST_VERTEX_LIGHT(1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
		p[10] = ST_VERTEX_LIGHT(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
		p[11] = ST_VERTEX_LIGHT(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
		m_vb->Unlock();

// 		m_device->CreateIndexBuffer(15*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,&m_ib,0);
// 
// 		WORD* indices = 0;
// 		m_ib->Lock(0, 0, (void**)&indices, 0);
// 		indices[0] = 

		D3DXCreateTeapot(m_device, &m_mesh, 0);


		D3DMATERIAL9 mat;
		mat.Ambient = d3d::WHITE;
		mat.Diffuse = d3d::WHITE;
		mat.Specular = d3d::WHITE;
		mat.Emissive = d3d::BLACK; // ���Է��� 
		mat.Power = 0.5f;
		m_device->SetMaterial(&mat);


		// ������Դ
		D3DLIGHT9  dir_light;
		::ZeroMemory(&dir_light,sizeof(D3DLIGHT9));
		dir_light.Type = D3DLIGHT_DIRECTIONAL;
		dir_light.Diffuse = d3d::WHITE;
		dir_light.Specular = d3d::WHITE*0.3f;
		dir_light.Ambient = d3d::WHITE*0.6f;
		dir_light.Direction = D3DXVECTOR3(1.0f,0.0f,0.0f);
		m_device->SetLight(0, &dir_light);
		m_device->LightEnable(0, true);

		m_device->SetRenderState(D3DRS_NORMALIZENORMALS, true); //ʹ���ߴ��¹��
		m_device->SetRenderState(D3DRS_SPECULARENABLE ,true);
		_BASE_VIEW_SRTTING;
		_BASE_PROJ_SETTING;

	}
	return true;
}

bool D3DLightingSample::display(float timeDelta)
{
	// ��ת������
	D3DXMATRIX Rx, Ry;
	// x����ת45����
	D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);
	// ÿһ֡������y��Ļ���
	static float y = 0.0f;
	D3DXMatrixRotationY(&Ry, y);
	y += timeDelta;
	//��y����ת2��ʱ�����»ص�0����
	if (y >= 6.28f)
		y = 0.0f;
	// ���x����y�����ת����
	D3DXMATRIX p = Rx * Ry;
	m_device->SetTransform(D3DTS_WORLD, &p);


	m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0x00000000, 1.0f, 0);
	m_device->BeginScene(); // ��ʼ���Ƴ���
	m_device->SetStreamSource(0, m_vb, 0, sizeof(ST_VERTEX_LIGHT)); // ������Դ��
	m_device->SetFVF(ST_VERTEX_LIGHT::FVF); // ���ö����ʽ
	

 	D3DXMATRIX world;
// 	D3DXMatrixTranslation(&world, 0.0f, 0.0f, 0.0f);
// 	m_device->SetTransform(D3DTS_WORLD, &world);
// 	m_device->SetMaterial(&d3d::RED_MTRL);
// 	m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);


	m_device->SetTransform(D3DTS_WORLD, &p);
	m_device->SetMaterial(&d3d::GREEN_MTRL);
	m_mesh->DrawSubset(0);

	m_device->EndScene(); // �������Ƴ���

	m_device->Present(0, 0, 0, 0); // ��ת����
	return true;
}

bool D3DLightingSample::cleanup()
{
	return true;
}
