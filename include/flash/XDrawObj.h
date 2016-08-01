// XDrawObj.h: interface for the XDrawObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDRAWOBJ_H__DCE0DFC9_BB74_4D72_8CF2_F7F7B9D14DFE__INCLUDED_)
#define AFX_XDRAWOBJ_H__DCE0DFC9_BB74_4D72_8CF2_F7F7B9D14DFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "splay.h"

class XDrawObj:public SCharacterParser
{
public:
	enum 
	{
		DC_BEGINFILLGRAD,
		DC_BEGINFILLBITMAP,
		DC_BEGINFILL,
		DC_ENDFILL,
		DC_ENDLINE,
		DC_LINESTYLE,
		DC_BEGINLINEGRAD,
		DC_MOVETO,
		DC_CURVE,
		DC_BEGINFILLBITMAP_EX,
		DC_CIRCLE,
		DC_ELLIPSE,
		DC_RECT,
		DC_ROUNDRECT,
		DC_LINEGRADIENTSTYLE,
	};

public:
	XDrawObj(ScriptPlayer*,BwShapeRecord*);
	virtual ~XDrawObj();

public:
	//void BuildScrollEdges(SRECT* devBounds,SRECT&bound);
	void BuildEdges(SBitmapCore*bitmap,bool bRelease=false);
	void BuildEdgesCamera(SBitmapCore*bitmap);
	void BuildEdges(_XSCharacter*ch);
	void BuildEdgesImage(_XSCharacter*ch);
	void BuildEdges(_XCOMMAND*cmds);
	//	void BuildEdges(SRECT*devBounds,,SBitmapCore*bitmap);

	//void AddCurveFL( );
	//void LineTo( int x,int y );
	//void MoveTo( int x,int y );
	//void CurveTo( int cx,int cy,int x,int y );
	void BwAddEdge( BwPath &path, int cx, int cy, int x, int y,bool isLine );
protected:
	//bool mbLineBegin;
	//bool mbHasFillStyle; //hasLine,bBeginLine;
	//bool mbHasLineStyle;
	//bool bBeginLine;
	XU32   thick;
	//RColor*lineColor;
	SPOINT mPtBegin;
	SPOINT mPtEnd;
	_GLColor* GetColor(_XCOMMAND*pCmd,int nCmd);
	BwLineStyle* GetLineStyle(_XCOMMAND*pCmd,int nCmd);
	//	_XCOMMAND* m_cmds;
};

#endif // !defined(AFX_XDRAWOBJ_H__DCE0DFC9_BB74_4D72_8CF2_F7F7B9D14DFE__INCLUDED_)
