#include "stdafx.h"
#include "PSystem.h"
#include "CameraBase.h"

using namespace psys;



const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;


PSystem::PSystem()
{
	_device = 0;
	_vb = 0;
	_tex = 0;
}


PSystem::~PSystem()
{
	d3d::Release<IDirect3DVertexBuffer9*>(_vb);
	d3d::Release<IDirect3DTexture9*>(_tex);
}

bool PSystem::init(IDirect3DDevice9* pDevice, TCHAR * szTextFileName)
{
	_device = pDevice;

	HRESULT hr = 0;

	hr = _device->CreateVertexBuffer(
		_vbSize*sizeof(Particle),
		D3DUSAGE_DYNAMIC	|	D3DUSAGE_POINTS	|D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT,
		&_vb,
		0);
	if (FAILED(hr))
	{
		_BX_CHECK(true,"CreateVertexBuffer Failed --PSystem");
		return false;
	}

	hr = D3DXCreateTextureFromFile(_device,szTextFileName,&_tex);
	if (FAILED(hr))
	{
		_BX_CHECK(true, "D3DXCreateTextureFromFile Failed --PSystem");
		return false;
	}

	return true;
}

void PSystem::reset()
{
	std::list<Attribute>::iterator listIter = _particles.begin();
	for (listIter; listIter != _particles.end();++listIter)
	{
		resetParticle(&(*listIter));
	}
}

void PSystem::addParticle()
{
	Attribute attribute;

	resetParticle(&attribute);

	_particles.push_back(attribute);
}

void PSystem::preRender()
{
	_device->SetRenderState(D3DRS_LIGHTING,false);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE,true);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSIZE, d3d::FtoDw(_size));
	_device->SetRenderState(D3DRS_POINTSIZE_MIN, d3d::FtoDw(0));

	_device->SetRenderState(D3DRS_POINTSCALE_A,d3d::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_B, d3d::FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_C, d3d::FtoDw(1.0f));

	//Ê¹ÓÃtexture alpha 
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void PSystem::render()
{
	if (_particles.empty())
	{
		return;
	}
	preRender();

	_device->SetTexture(0, _tex);
	_device->SetFVF(Particle::FVF);
	_device->SetStreamSource(0,_vb,0,sizeof(Particle));

	if (_vbOffset  >= _vbSize)
	{
		_vbOffset = 0;
	}

	Particle * v = 0;
	_vb->Lock(
		_vbOffset*sizeof(Particle),
		_vbBatchSize*sizeof(Particle),
		(void**)&v,
		_vbOffset?D3DLOCK_NOOVERWRITE  : D3DLOCK_DISCARD	);

	DWORD numberParticleInBatch = 0;

	std::list<Attribute>::iterator i = _particles.begin();
	for (i;i!=_particles.end();++i)
	{
		if (!i->_isAlive)
		{
			continue;
		}
		v->_position = i->_position;
		v->_color = (D3DCOLOR)i->_color;
		v++;

		numberParticleInBatch++;
		if (numberParticleInBatch == _vbBatchSize)
		{
			_vb->Unlock();
			_device->DrawPrimitive(D3DPT_POINTLIST, _vbOffset, _vbBatchSize);
			_vbOffset += _vbBatchSize;
			if (_vbOffset >= _vbSize) _vbOffset = 0;
			_vb->Lock(
				_vbOffset * sizeof(Particle),
				_vbBatchSize * sizeof(Particle),
				(void**)&v,
				_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

			numberParticleInBatch = 0; // reset for new batch
		}
		if (!v)
		{
			break;;
		}
	}
	_vb->Unlock();

	if (numberParticleInBatch)
	{
		_device->DrawPrimitive(
			D3DPT_POINTLIST,
			_vbOffset,
			numberParticleInBatch);
	}

	_vbOffset += _vbBatchSize;

	postRender();

}

void PSystem::postRender()
{
	//render state ¸´Î»
	_device->SetRenderState(D3DRS_LIGHTING, true);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

bool PSystem::isEmpty()
{
	return _particles.empty();
}

bool PSystem::isDead()
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		// is there at least one living particle?  If yes,
		// the system is not dead.
		if (i->_isAlive)
			return false;
	}
	// no living particles found, the system must be dead.
	return true;
}

void PSystem::removeDeadParticle()
{
	std::list<Attribute>::iterator i;

	i = _particles.begin();

	while (i != _particles.end())
	{
		if (i->_isAlive == false)
		{
			// erase returns the next iterator, so no need to
			// incrememnt to the next one ourselves.
			i = _particles.erase(i);
		}
		else
		{
			i++; // next in list
		}
	}
}

psys::SnowParticle::SnowParticle(d3d::BoundingBox* boundingBox, int numParticles)
{
	_boundingBox = *boundingBox;
	_size = 0.25f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

void psys::SnowParticle::resetParticle(Attribute* pAttribute)
{
	pAttribute->_isAlive = true;

	// get random x, z coordinate for the position of the snow flake.
	d3d::GetRandomVector(
		&pAttribute->_position,
		&_boundingBox._min,
		&_boundingBox._max);

	// no randomness for height (y-coordinate).  Snow flake
	// always starts at the top of bounding box.
	pAttribute->_position.y = _boundingBox._max.y;

	// snow flakes fall downwards and slightly to the left
	pAttribute->_velocity.x = d3d::GetRandomFloat(0.0f, 1.0f) * -3.0f;
	pAttribute->_velocity.y = d3d::GetRandomFloat(0.0f, 1.0f) * -10.0f;
	pAttribute->_velocity.z = 0.0f;

	// white snow flake
	pAttribute->_color = d3d::WHITE;
}

void psys::SnowParticle::updata(float timeDelta)
{
	std::list<Attribute>::iterator i;
	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		i->_position += i->_velocity * timeDelta;

		// is the point outside bounds?
		if (_boundingBox.isPointInside(i->_position) == false)
		{
			// nope so kill it, but we want to recycle dead 
			// particles, so respawn it instead.
			resetParticle(&(*i));
		}
	}
}

psys::FireworkParticle::FireworkParticle(D3DXVECTOR3* origin, int numParticles)
{
	_origin = *origin;
	_size = 0.9f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		addParticle();
}

void psys::FireworkParticle::resetParticle(Attribute* pAttribute)
{
	pAttribute->_isAlive = true;
	pAttribute->_position = _origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	d3d::GetRandomVector(
		&pAttribute->_velocity,
		&min,
		&max);

	// normalize to make spherical
	D3DXVec3Normalize(
		&pAttribute->_velocity,
		&pAttribute->_velocity);

	pAttribute->_velocity *= 100.0f;

	pAttribute->_color = D3DXCOLOR(
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		1.0f);

	pAttribute->_age = 0.0f;
	pAttribute->_lifeTime = 2.0f; // lives for 2 seconds
}

void psys::FireworkParticle::updata(float timeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		// only update living particles
		if (i->_isAlive)
		{
			i->_position += i->_velocity * timeDelta;

			i->_age += timeDelta;

			if (i->_age > i->_lifeTime) // kill 
				i->_isAlive = false;
		}
	}
}

void psys::FireworkParticle::preRender()
{
	PSystem::preRender();

	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void psys::FireworkParticle::postRender()
{
	PSystem::postRender();

	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

psys::GunParticle::GunParticle(CameraBase * can)
{
	_camera = can;
	_size = 0.8f;
	_vbSize = 2048;
	_vbOffset = 0;
	_vbBatchSize = 512;
}

void psys::GunParticle::resetParticle(Attribute* pAttribute)
{
	pAttribute->_isAlive = true;

	D3DXVECTOR3 cameraPos;
	_camera->getPosition(&cameraPos);

	D3DXVECTOR3 cameraDir;
	_camera->getLook(&cameraDir);

	// change to camera position
	pAttribute->_position = cameraPos;
	pAttribute->_position.y -= 1.0f; // slightly below camera
									// so its like we're carrying a gun

									// travels in the direction the camera is looking
	pAttribute->_velocity = cameraDir * 100.0f;

	// green
	pAttribute->_color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	pAttribute->_age = 0.0f;
	pAttribute->_lifeTime = 1.0f; // lives for 1 seconds
}

void psys::GunParticle::updata(float timeDelta)
{
	std::list<Attribute>::iterator i;

	for (i = _particles.begin(); i != _particles.end(); i++)
	{
		i->_position += i->_velocity * timeDelta;

		i->_age += timeDelta;

		if (i->_age > i->_lifeTime) // kill 
			i->_isAlive = false;
	}
	removeDeadParticle();
}
