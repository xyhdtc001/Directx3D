#pragma once

#define  _BASE_PROJ_SETTING  D3DXMATRIX  proj;	\
			D3DXMatrixPerspectiveFovLH(&proj,D3DX_PI*0.5,(float)m_width/(float)m_heigth,1.0f,1000.0f); \
            m_device->SetTransform(D3DTS_PROJECTION,&proj);

#define _BASE_VIEW_SRTTING_(_x,_y,_z) D3DXVECTOR3 position(_x, _y,_z);		\
			D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);		\
			D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);			\
			D3DXMATRIX V;										\
			D3DXMatrixLookAtLH(&V, &position, &target, &up);	\
			m_device->SetTransform(D3DTS_VIEW, &V);		

#define _BASE_VIEW_SRTTING _BASE_VIEW_SRTTING_(-3.0f, 2.0f, -3.0f)


class CSampleBase
{
public:
	CSampleBase(IDirect3DDevice9* pDevice);
	~CSampleBase();

	virtual bool setup() = 0;

	virtual bool display(float timeDelta) =0;


	virtual bool cleanup() ;

protected:
	IDirect3DDevice9 * m_device;
	IDirect3DVertexBuffer9 * m_vb;
	IDirect3DIndexBuffer9* m_ib;

	float m_width;
	float m_heigth;
};

