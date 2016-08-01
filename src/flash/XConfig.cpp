// XConfig.cpp: implementation of the XConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XConfig.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int XConfig::_nCurData[4][4]=
{
	{0,15,0,0},
	{15,17,6,2},
	{33,8,5,12},
	{42,23,0,0}
};


XConfig::XConfig()
{
	m_pGrayDib=XNULL;
	m_pGreenDib=XNULL;
	//m_pOnDib=XNULL;
	//m_pOffDib=XNULL;
	m_nAutoTag=0;
	m_fMaxScore=0;
	m_nADMode=XDOMFILTER::AD_TOP;
	m_nWaitID=1;
	//m_nAutoDepth;
	//m_nQuality=HIGH;//default Quality
	m_bConfigOK=XFALSE;
	m_bMouseShow=XFALSE;
	//m_strTagPlace=_strEmpty;
	Reset();
	m_bACCMap=XTRUE;
}

void XConfig::Reset()
{
	XU32 i;
	m_nScoreType=SCOREVAR;
	m_bButtons=1;
	m_nContentLevel=6;
	m_nMouseStep=3;
	m_nLongLockTime=800;
	m_nZoomHotKey='#';
	m_pActiveMode=XNULL;
	m_nADMode=XDOMFILTER::AD_TOP;
	//m_nScreenDirect=2;
	m_nZoomMode=0;
	m_bMute=0;
	m_nVolume=4;
	m_nSoundDevice=0;
	//m_nTouchSize=0;
	m_nAutoCap=0;
	m_mouseModes.FreeAll();
	m_keyMaps.RemoveAll();
	for(i=0;i<m_buttonMaps.GetSize();i++)
	{
		if(m_buttonMaps[i].pDib)
			delete m_buttonMaps[i].pDib;
	}
	m_buttonMaps.RemoveAll();
	if(m_pGrayDib)
		delete m_pGrayDib;
	if(m_pGreenDib)
		delete m_pGreenDib;
	//if(m_pOnDib)
	//	delete m_pOnDib;
	//if(m_pOffDib)
	//	delete m_pOffDib;
	m_pGrayDib=XNULL;
	m_pGreenDib=XNULL;
	//m_pOnDib=XNULL;
	//m_pOffDib=XNULL;

}


XConfig::~XConfig()
{
	/*XU32 i;
	for(i=0;i<m_buttonMaps.GetSize();i++)
	{
		if(m_buttonMaps[i].pDib)
			delete m_buttonMaps[i].pDib;
	}
	m_mouseModes.FreeAll();*/
	Reset();
}

void XConfig::FreeExtra()
{
	m_keyMaps.FreeExtra();
	m_buttonMaps.FreeExtra();
	m_mouseModes.FreeExtra();
}

XBOOL XConfig::SetMouseMode(XPCTSTR strMode,XBOOL bForce)
{
	int i,v=m_mouseModes.GetSize();
	if(v<=0) return XFALSE;
	for(i=0;i<v;i++)
	{
		if(m_mouseModes[i]->strName==strMode)
		{
			m_pActiveMode=m_mouseModes[i];
			return XTRUE;
		}
	}
	if(bForce)
	{
		m_pActiveMode=m_mouseModes[0];
		return XTRUE;
	}
	return XFALSE;
}


const XPCTSTR _strButtonRes[]=
//enum {A,B,DOWN,LEFT,LEFT_KEY,RIGHT,RIGHT_KEY,UP};
{
	"KEYA","KEYB","KEYDOWN","KEYLEFT","","","","KEYRIGHT","","KEYUP",
	"KEY_0","KEY_1","KEY_2","KEY_3","KEY_4","KEY_5","KEY_6","KEY_7","KEY_8","KEY_9",	
	"KEY_A","KEY_B","KEY_C","KEY_D","KEY_DOWN","KEY_E","KEY_ENTER","KEY_F","KEY_G",
	"KEY_H","KEY_I","KEY_J","KEY_K","KEY_L","KEY_LEFT","KEY_M","KEY_N",
	"KEY_O","KEY_P","KEY_Q","KEY_R","KEY_RIGHT","KEY_S","KEY_SPACE","KEY_T","KEY_U","KEY_UP",
	"KEY_V","KEY_W","KEY_X","KEY_Y","KEY_Z",
	"KEY_UP","KEY_DOWN","KEY_LEFT","KEY_RIGHT",
};
const XPCTSTR _strButtonLight[]={"GRAYDOT","GREENDOT","BUTTONON","BUTTONOFF"};

void XConfig::InitConfig()
{
    XU32 i;
	for(i=0;i<m_keyMaps.GetSize();i++)
	{
		m_keyMaps[i].nPressTime=0;
		m_keyMaps[i].bLock=KLK_RELEASE;
	}
	int nMax=sizeof(_strButtonRes)/sizeof(XPCTSTR);
	XBOOL bLight=XFALSE,bButton=XFALSE;
	for(i=0;i<m_buttonMaps.GetSize();i++)
	{
		m_buttonMaps[i].nPressTime=0;
		m_buttonMaps[i].bLock=KLK_RELEASE;
		m_buttonMaps[i].nShowTime=0;
		if(m_buttonMaps[i].nButtonID>=nMax) continue;
		bButton=XTRUE;
		if(m_buttonMaps[i].bLongPress)
			bLight=XTRUE;
		int nID=m_buttonMaps[i].nButtonID;
		if(nID>=0&&nID<sizeof(_strButtonRes)/sizeof(XPCTSTR))
			m_buttonMaps[i].pDib=CreateDib(_strButtonRes[nID]);
		/*XImage image;
		if(image.CreateFromResource(_strButtonRes[m_buttonMaps[i].nButtonID],XTRUE))
		{
			XDib*pDib=new XDib;
			int w=image.Width(),h=image.Height();
			if(pDib)
			{
				if(pDib->Create(w,h,32))
				{					
					image.GetBitmapData(pDib->GetAddress(),w,h,32,pDib->m_nDataLength);
				}
				else
				{					
					delete pDib;
					pDib=XNULL;
				}
				m_buttonMaps[i].pDib=pDib;
			}
		}*/
	}
	XBOOL bCursor=XFALSE;
	for(i=0;i<m_mouseModes.GetSize();i++)
	{
		if(m_mouseModes[i]->nMode==_XMOUSEMODE::XNORMAL)
		{
			bCursor=XTRUE;
			break;
		}
	}
	if(bCursor)
	{
		m_cursor.CreateFromResource("CURSOR",XTRUE);
	}
	if(bLight)
	{
		m_pGrayDib=CreateDib(_strButtonLight[0]);
		m_pGreenDib=CreateDib(_strButtonLight[1]);
	}
	//if(bButton)
	//{
		//m_pOnDib=CreateDib(_strButtonLight[2]);
		//m_pOffDib=CreateDib(_strButtonLight[3]);
	//}
}

XDib* XConfig::CreateDib(XPCTSTR strName)
{
	XImage image;
	if(image.CreateFromResource(strName,XTRUE))
	{
		XDib*pDib=new XDib;
		int w=image.Width(),h=image.Height();
		if(pDib)
		{
			if(pDib->Create(w,h,32))
			{					
				image.GetBitmapData(pDib->GetAddress(),w,h,32,pDib->m_nDataLength);
			}
			else
			{					
				delete pDib;
				pDib=XNULL;
			}
			return pDib;
		}
	}
	return XNULL;
}
