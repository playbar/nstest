// XDomBR.h: interface for the XDomBR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMBR_H__DDA0D0EE_FC25_4E22_AB3B_FB7F5BE35A3D__INCLUDED_)
#define AFX_XDOMBR_H__DDA0D0EE_FC25_4E22_AB3B_FB7F5BE35A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomBR : public XDomLeaf  
{
	XDEC_CREATE(XDomBR);
public:
	//static XDomItem* _stdcall CreateObject(XU16 ID){return &m_br;}

	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomBR():XDomLeaf(XDOM_BR){}
	XDomBR(XU16 ID):XDomLeaf(XDOM_BR){}
protected:
	void LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bAdd=XTRUE);
public:
	static XDomBR m_br;
};

//#if (__CORE_VERSION__>=0x02077000)
class _XEXT_CLASS XDomSBR : public XDomLeaf  
{
	XDEC_CREATE(XDomSBR);
public:
	//static XDomItem* _stdcall CreateObject(XU16 ID){return &m_sbr;}

	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomSBR(XU16 ID);
	~XDomSBR();
protected:
	void LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bAdd=XTRUE);
public:
	//static XDomSBR *m_sbr;
};
//#endif

class _XEXT_CLASS XDomSpace:public XDomItem
{
	XDEC_CREATE(XDomSpace);
public:
	//static XDomItem* _stdcall CreateObject(XU16 ID){return &m_space;}
	XDomSpace():XDomItem(XDOM_SPACE){}
	XDomSpace(XU16 ID):XDomItem(XDOM_SPACE){}
public:
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
	static XDomSpace m_space;
};

class _XEXT_CLASS XDomQuestion:public XDomItem
{
	XDEC_CREATE(XDomSpace);
	static XDomItem* CreateObject(){return new XDomQuestion();}
public:
	XDomQuestion():XDomItem(XDOM_QUESTION){}
	XDomQuestion(XU16 ID):XDomItem(XDOM_QUESTION){}
	//static XDomItem* CreateObject(){return new XDomQuestion();}
};

#endif // !defined(AFX_XDOMBR_H__DDA0D0EE_FC25_4E22_AB3B_FB7F5BE35A3D__INCLUDED_)
