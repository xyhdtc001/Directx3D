#pragma once

#define  _BASE_PROJ_SETTING_(_x)  D3DXMATRIX  proj;	\
			D3DXMatrixPerspectiveFovLH(&proj,D3DX_PI*_x,(float)m_width/(float)m_heigth,1.0f,1000.0f); \
            m_device->SetTransform(D3DTS_PROJECTION,&proj);

#define _BASE_PROJ_SETTING _BASE_PROJ_SETTING_(0.5)

#define _BASE_VIEW_SRTTING_(_x,_y,_z) D3DXVECTOR3 position(_x, _y,_z);		\
			D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);		\
			D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);			\
			D3DXMATRIX V;										\
			D3DXMatrixLookAtLH(&V, &position, &target, &up);	\
			m_device->SetTransform(D3DTS_VIEW, &V);		

#define _BASE_VIEW_SRTTING _BASE_VIEW_SRTTING_(-3.0f, 2.0f, -3.0f)


#define  _BASE_FONT_CRATE(facename,outFont) D3DXFONT_DESC lf; ZeroMemory(&lf, sizeof(D3DXFONT_DESC)); lf.Height = 25;lf.Width = 12; lf.Weight = 500; \
				lf.Italic = false;lf.CharSet = DEFAULT_CHARSET;wcscpy(lf.FaceName, _T(facename));D3DXCreateFontIndirect(m_device, &lf, &outFont);

#define _BASE_FONT_CRATE_DEF(outFont) _BASE_FONT_CRATE("Î¢¶ËÑÅºÚ",outFont)


#define _BASE_RENDER_FPSCOUNT(font,_l,_t,_r,_b,_color) if(!font)  {_BASE_FONT_CRATE_DEF(font) } \
			char szStringFPSBuff[100] = {0};sprintf(szStringFPSBuff, ("fps:%.1f"), m_Basefps);RECT rtFps = { _l,_t,_r,_b };font->DrawTextA(NULL, szStringFPSBuff, strlen(szStringFPSBuff), &rtFps, DT_TOP | DT_LEFT, (DWORD)_color);


class CSampleBase
{
public:
	CSampleBase(IDirect3DDevice9* pDevice);
	~CSampleBase();

	virtual bool setup() = 0;

	virtual bool display(float timeDelta) =0;


	virtual bool cleanup() ;

	virtual void calc_FPS(float timeDelta);

protected:
	IDirect3DDevice9 * m_device;
	IDirect3DVertexBuffer9 * m_vb;
	IDirect3DIndexBuffer9* m_ib;

	float m_width;
	float m_heigth;

	DWORD m_baseFrameCnt;
	float m_baseTimeElapsed;
	float m_Basefps;
};

