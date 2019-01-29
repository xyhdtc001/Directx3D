#include "stdafx.h"
#include "D3DRenderMirrorSample.h"
#include "d3dUtility.h"

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		_x = x;  _y = y;  _z = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u;  _v = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


IDirect3DTexture9* FloorTex = 0;
IDirect3DTexture9* WallTex = 0;
IDirect3DTexture9* MirrorTex = 0;

D3DMATERIAL9 FloorMtrl = d3d::WHITE_MTRL;
D3DMATERIAL9 WallMtrl = d3d::WHITE_MTRL;
D3DMATERIAL9 MirrorMtrl = d3d::WHITE_MTRL;

ID3DXMesh* Teapot = 0;
D3DXVECTOR3 TeapotPosition(0.0f, 3.0f, -7.5f);
D3DMATERIAL9 TeapotMtrl = d3d::YELLOW_MTRL;



D3DRenderMirrorSample::D3DRenderMirrorSample(IDirect3DDevice9* pDevice)
	:CSampleBase(pDevice)
{

}

D3DRenderMirrorSample::~D3DRenderMirrorSample()
{
}

bool D3DRenderMirrorSample::setup()
{
	//
	// Make walls have low specular reflectance - 20%.
	//

	WallMtrl.Specular = d3d::WHITE * 0.2f;

	//
	// Create the teapot.
	//

	D3DXCreateTeapot(m_device, &Teapot, 0);

	//
	// Create and specify geometry.  For this sample we draw a floor
	// and a wall with a mirror on it.  We put the floor, wall, and
	// mirror geometry in one vertex buffer.
	//
	//   |----|----|----|
	//   |Wall|Mirr|Wall|
	//   |    | or |    |
	//   /--------------/
	//  /   Floor      /
	// /--------------/
	//
	m_device->CreateVertexBuffer(
		24 * sizeof(Vertex),
		0, // usage
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&m_vb,
		0);

	Vertex* v = 0;
	m_vb->Lock(0, 0, (void**)&v, 0);

	// floor
	v[0] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// wall
	v[6] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[7] = Vertex(-7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[9] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[10] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[11] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Note: We leave gap in middle of walls for mirror

	v[12] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[13] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[15] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[16] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[17] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror
	v[18] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[21] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	m_vb->Unlock();

	//
	// Load Textures, set filters.
	//

	D3DXCreateTextureFromFile(m_device, _T("dx_res\\checker.jpg"), &FloorTex);
	D3DXCreateTextureFromFile(m_device,_T( "dx_res\\brick0.jpg"), &WallTex);
	D3DXCreateTextureFromFile(m_device, _T("dx_res\\ice.bmp"), &MirrorTex);

	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	// Lights.
	//

	D3DXVECTOR3 lightDir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&lightDir, &color);

	m_device->SetLight(0, &light);
	m_device->LightEnable(0, true);

	m_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Set Camera.
	//

	D3DXVECTOR3    pos(-10.0f, 3.0f, -15.0f);
	D3DXVECTOR3 target(0.0, 0.0f, 0.0f);
	D3DXVECTOR3     up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);

	m_device->SetTransform(D3DTS_VIEW, &V);

	//
	// Set projection matrix.
	//
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI / 4.0f, // 45 - degree
		(float)m_width / (float)m_heigth,
		1.0f,
		1000.0f);
	m_device->SetTransform(D3DTS_PROJECTION, &proj);
	return true;
}

bool D3DRenderMirrorSample::display(float timeDelta)
{
	if (!m_device)
	{
		return false;
	}
	static float radius = 20.0f;

	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
		TeapotPosition.x -= 3.0f * timeDelta;

	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		TeapotPosition.x += 3.0f * timeDelta;

	if (::GetAsyncKeyState(VK_UP) & 0x8000f)
		radius -= 2.0f * timeDelta;

	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
		radius += 2.0f * timeDelta;


	static float angle = (3.0f * D3DX_PI) / 2.0f;

	if (::GetAsyncKeyState('A') & 0x8000f)
		angle -= 0.5f * timeDelta;

	if (::GetAsyncKeyState('S') & 0x8000f)
		angle += 0.5f * timeDelta;
	float a = cosf(D3DX_PI);
	float b = sinf(D3DX_PI);
	D3DXVECTOR3 position(cosf(angle) * radius, 3.0f, sinf(angle) * radius);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	m_device->SetTransform(D3DTS_VIEW, &V);

	//
	// Draw the scene:
	//
	m_device->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0xff000000, 1.0f, 0L);

	m_device->BeginScene();

	render_scence();

	render_mirror();

	render_shadow();

	m_device->EndScene();
	m_device->Present(0, 0, 0, 0);
	return true;
}

bool D3DRenderMirrorSample::cleanup()
{
	d3d::Release<IDirect3DTexture9*>(FloorTex);
	d3d::Release<IDirect3DTexture9*>(WallTex);
	d3d::Release<IDirect3DTexture9*>(MirrorTex);
	d3d::Release<ID3DXMesh*>(Teapot);
	return __super::cleanup();
}

void D3DRenderMirrorSample::render_mirror()
{
	//
	// Draw Mirror quad to stencil buffer ONLY.  In this way
	// only the stencil bits that correspond to the mirror will
	// be on.  Therefore, the reflected teapot can only be rendered
	// where the stencil bits are turned on, and thus on the mirror 
	// only.
	//

	m_device->SetRenderState(D3DRS_STENCILENABLE, true);
	m_device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	m_device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	m_device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	m_device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	m_device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	// disable writes to the depth and back buffers
	m_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE    );
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// draw the mirror to the stencil buffer
	m_device->SetStreamSource(0, m_vb, 0, sizeof(Vertex));
	m_device->SetFVF(Vertex::FVF);
	m_device->SetMaterial(&MirrorMtrl);
	m_device->SetTexture(0, MirrorTex);
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	m_device->SetTransform(D3DTS_WORLD, &I);
	m_device->SetRenderState(D3DRS_STENCILREF, 0x1);
	m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

	// re-enable depth writes
	m_device->SetRenderState(D3DRS_ZWRITEENABLE, true);

	// only draw reflected teapot to the pixels where the mirror
	// was drawn to.
	m_device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	m_device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// position reflection
	D3DXMATRIX W, T, R;
	D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
	D3DXMatrixReflect(&R, &plane);

	D3DXMatrixTranslation(&T,
		TeapotPosition.x,
		TeapotPosition.y,
		TeapotPosition.z);

	W = T * R;

	// clear depth buffer and blend the reflected teapot with the mirror
	m_device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// Finally, draw the reflected teapot
	m_device->SetTransform(D3DTS_WORLD, &W);
	m_device->SetMaterial(&TeapotMtrl);
	m_device->SetTexture(0, 0);

	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_device->SetRenderState(D3DRS_STENCILREF, 0x1);
	Teapot->DrawSubset(0);

	// Restore render states.
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_device->SetRenderState(D3DRS_STENCILENABLE, false);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void D3DRenderMirrorSample::render_shadow()
{
	m_device->SetRenderState(D3DRS_STENCILENABLE, true);
	m_device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	m_device->SetRenderState(D3DRS_STENCILREF, 0x0);
	m_device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	m_device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	m_device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	m_device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	// compute the transformation to flatten the teapot into a shadow.
	D3DXVECTOR4 lightDirection(0.707f, -0.707f, 0.707f, 0.0f);  // lightDir(0.707f, -0.707f, 0.707f);
	D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, 0.0f);
	D3DXMATRIX S;
	D3DXMatrixShadow(&S, &lightDirection, &groundPlane);
	D3DXMATRIX T;
	D3DXMatrixTranslation(&T, TeapotPosition.x, TeapotPosition.y,
		TeapotPosition.z);
	D3DXMATRIX W = T * S;
	m_device->SetTransform(D3DTS_WORLD, &W);


	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK,
		d3d::BLACK, d3d::BLACK, 0.0f);
	mtrl.Diffuse.a = 0.5f; // 50% transparency.
						   // Disable depth buffer so that z-fighting doesn't occur when we
						   // render the shadow on top of the floor.
	m_device->SetRenderState(D3DRS_ZENABLE, false);
	m_device->SetMaterial(&mtrl);
	m_device->SetTexture(0, 0);
	Teapot->DrawSubset(0);
	m_device->SetRenderState(D3DRS_ZENABLE, true);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_device->SetRenderState(D3DRS_STENCILENABLE, false);
}

void D3DRenderMirrorSample::render_scence()
{
	// draw teapot
	m_device->SetMaterial(&TeapotMtrl);
	m_device->SetTexture(0, 0);
	D3DXMATRIX W;
	D3DXMatrixTranslation(&W,
		TeapotPosition.x,
		TeapotPosition.y,
		TeapotPosition.z);

	m_device->SetTransform(D3DTS_WORLD, &W);
	Teapot->DrawSubset(0);

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	m_device->SetTransform(D3DTS_WORLD, &I);

	m_device->SetStreamSource(0, m_vb,0, sizeof(Vertex));
	m_device->SetFVF(Vertex::FVF);

	// draw the floor
	m_device->SetMaterial(&FloorMtrl);
	m_device->SetTexture(0, FloorTex);
	m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// draw the walls
	m_device->SetMaterial(&WallMtrl);
	m_device->SetTexture(0, WallTex);
	m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 4);

	// draw the mirror
	m_device->SetMaterial(&WallMtrl);
	m_device->SetTexture(0, WallTex);
	m_device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
}
