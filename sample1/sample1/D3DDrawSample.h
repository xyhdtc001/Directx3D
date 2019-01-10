#pragma once


typedef struct _st_vertex_drawsamle
{
	_st_vertex_drawsamle()
	{
		_x = 0; _y = 0; _z = 0;
	}
	_st_vertex_drawsamle(float x, float y, float z)
	{
		_x = x; _y = y; _z = z;
	}
	float _x, _y,_z;
	static const DWORD FVF = D3DFVF_XYZ;
}ST_VERTEX;


class D3DDrawSample
{
public:
	D3DDrawSample(IDirect3DDevice9* pDevice);
	~D3DDrawSample();

	virtual bool setup();


private:
	IDirect3DVertexBuffer9 * m_vb;
	IDirect3DIndexBuffer9*    m_ib;
	IDirect3DDevice9* m_device;

};

