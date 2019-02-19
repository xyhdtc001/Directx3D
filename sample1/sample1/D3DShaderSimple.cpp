#include "stdafx.h"
#include "D3DShaderSimple.h"
#include "d3dUtility.h"

D3DShaderSimple::D3DShaderSimple(IDirect3DDevice9* pDevice)
	:CSampleBase(pDevice)
{
	m_Teapot = NULL;
	m_shader = NULL;
	m_errorBuff = NULL;
	m_DiffuseConstTable = NULL;
	m_DiffuseShader = NULL;
}

D3DShaderSimple::~D3DShaderSimple()
{
}

bool D3DShaderSimple::setup()
{
	//创建茶壶mesh
	D3DXCreateTeapot(m_device, &m_Teapot, 0);

	HRESULT hr = 0;

	hr = D3DXCompileShaderFromFile(_T("shader/shader_1.fx"),NULL,NULL,"main","vs_1_1",D3DXSHADER_DEBUG,&m_shader,&m_errorBuff,&m_DiffuseConstTable);

	if (m_errorBuff)
	{
		::MessageBoxA(0, (char*)m_errorBuff->GetBufferPointer(), 0, 0);
		d3d::Release<ID3DXBuffer*>(m_errorBuff);
	}

	if (FAILED(hr))
	{
		_BX_CHECK(true, "error setup");
		m_bInitState = false;
		return false;
	}

	//创建顶点着色 
	hr = m_device->CreateVertexShader((DWORD*)m_shader->GetBufferPointer(),&m_DiffuseShader);

	if (FAILED(hr))
	{
		_BX_CHECK(true, "error setup");
		m_bInitState = false;
		return false;
	}

	d3d::Release<ID3DXBuffer*>(m_shader);

	//

	return true;
}

bool D3DShaderSimple::display(float timeDelta)
{
	return true;
}

bool D3DShaderSimple::cleanup()
{
	return true;
}
