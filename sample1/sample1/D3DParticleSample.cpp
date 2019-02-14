#include "stdafx.h"
#include "D3DParticleSample.h"


struct Particle
{
	D3DXVECTOR3 _position;
	D3DCOLOR _color;
	float _size;
	static const DWORD FVF;
};
const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE |
D3DFVF_PSIZE;

D3DParticleSample::D3DParticleSample(IDirect3DDevice9*pDevice)
	:CSampleBase(pDevice)
{

}

D3DParticleSample::~D3DParticleSample()
{

}
