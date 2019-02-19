#pragma once
#include "SampleBase.h"

class D3DShaderSimple
	:public CSampleBase
{
public:
	D3DShaderSimple(IDirect3DDevice9* pDevice);
	~D3DShaderSimple();


	virtual bool setup() ;

	virtual bool display(float timeDelta) ;

	virtual bool cleanup();

protected:

	ID3DXMesh* m_Teapot;

	ID3DXBuffer *m_shader;
	ID3DXBuffer* m_errorBuff;
	ID3DXConstantTable* m_DiffuseConstTable ;
	IDirect3DVertexShader9* m_DiffuseShader ;
};

