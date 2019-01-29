#include "stdafx.h"
#include "D3DFontSample.h"
#include "d3dUtility.h"


D3DFontSample::D3DFontSample(IDirect3DDevice9* pDevice)
	:CSampleBase(pDevice)
{

}



D3DFontSample::~D3DFontSample()
{

}
ID3DXFont * pFont = NULL;
bool D3DFontSample::setup()
{	
	D3DXFONT_DESC lf;
	ZeroMemory(&lf,sizeof(D3DXFONT_DESC));
	lf.Height = 25;
	lf.Width = 12;
	lf.Weight = 500;
	lf.Italic = false;
	lf.CharSet = DEFAULT_CHARSET;
	wcscpy(lf.FaceName,_T("Î¢ÈíÑÅºÚ"));
	D3DXCreateFontIndirect(m_device,&lf, &pFont);
	return true;
}

bool D3DFontSample::display(float timeDelta)
{
	if (!pFont)
	{
		return false;
	}
	m_device->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,
		0xff000000, 1.0f, 0L);
	calc_FPS(timeDelta);
	m_device->BeginScene();
	
	TCHAR szStringBuff[] = _T("samples font ");
	RECT rt = { 10,10,200,40 };
	pFont->DrawText(NULL, szStringBuff,sizeof(szStringBuff)/sizeof(TCHAR),&rt, DT_TOP | DT_LEFT , (DWORD)d3d::WHITE);



	char szStringFPSBuff[100] = {0};
	sprintf(szStringFPSBuff, ("fps:%.1f"),m_Basefps);

	RECT rtFps = { 10,50,200,80 };
	pFont->DrawTextA(NULL, szStringFPSBuff,strlen(szStringFPSBuff), &rtFps, DT_TOP | DT_LEFT, (DWORD)d3d::WHITE);

	m_device->EndScene();
	m_device->Present(0, 0, 0, 0);
	return true;
}

bool D3DFontSample::cleanup()
{
	d3d::Release<ID3DXFont*>(pFont);
	__super::cleanup();
	return true;
}

