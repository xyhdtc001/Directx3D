#pragma once

#include "d3dUtility.h"
#include <list>

class CameraBase;
namespace psys {

	struct Particle
	{
		D3DXVECTOR3 _position;
		D3DCOLOR _color;
		float _size;
		static const DWORD FVF;
	};

	struct Attribute
	{
		D3DXVECTOR3 _position;      //粒子在世界空间中的位置.
		D3DXVECTOR3 _velocity;      //粒子速度,每秒多少个单位
		D3DXVECTOR3 _acceleration;//粒子加速度.
		float _lifeTime; //粒子生命周期
		float _age;//粒子当前年龄
		D3DXCOLOR _color;//粒子颜色
		D3DXCOLOR _colorFade;//粒子随时间变化而褪去的颜色.
		bool _isAlive; //是否存活
	};


	/*
	*	粒子系统参考类
	*/
	class PSystem
	{
	public:
		PSystem();
		virtual ~PSystem();

		virtual bool init(IDirect3DDevice9* pDevice, TCHAR * szTextFileName);
		virtual void reset();
		virtual void resetParticle(Attribute* pAttribute) = 0;
		virtual void addParticle();
		virtual void updata(float timeDelta) = 0;

		virtual void preRender();
		virtual void render();
		virtual void postRender();

		bool isEmpty();
		bool isDead();


	protected:
		virtual void removeDeadParticle();

	protected:
		IDirect3DDevice9 *_device;
		D3DXVECTOR3 _origin;
		d3d::BoundingBox _boundingBox;
		float _emitRate;
		float _size;
		IDirect3DTexture9* _tex;
		IDirect3DVertexBuffer9* _vb;
		std::list<Attribute>_particles;
		int _maxParticles;

		DWORD  _vbSize;
		DWORD _vbOffset;
		DWORD _vbBatchSize;
	};


	class SnowParticle
		:public PSystem
	{
	public:
		SnowParticle(d3d::BoundingBox* boundingBox, int numParticles);
		virtual void resetParticle(Attribute* pAttribute);
		virtual void updata(float timeDelta);
	};

	class FireworkParticle
		:public PSystem
	{
	public:
		FireworkParticle(D3DXVECTOR3* origin, int numParticles);
		virtual void resetParticle(Attribute* pAttribute);
		virtual void updata(float timeDelta);
		virtual void preRender();
		virtual void postRender();
	};

	class GunParticle
		:public PSystem
	{
	public:
		GunParticle(CameraBase * can);
		virtual void resetParticle(Attribute* pAttribute);
		virtual void updata(float timeDelta);
	private:
		CameraBase* _camera;
		float m_nInternl;
		float m_curDelta;
	};

}

