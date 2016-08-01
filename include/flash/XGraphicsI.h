#ifndef __XGRAPHICSI__
#define __XGRAPHICSI__

#include "flashgeom.h"
#include "sobject.h"

class XGraphicsI
{
public:
	void AddCommand(XCOMMAND*pCmd);
	void MoveTo(int x,int y);
	void LineTo(int x,int y,XBOOL bMove=XFALSE);
	void LineStyle(int nWidth,XU32 nColor);
	void EndFill();
	void CurveTo(int ax,int ay,int cx,int cy);
	void ReleaseDraw();
	void BeginFill(XU32 nColor,int nWidth);
	void BeginFill(XXObjectBitmap*pBitmap,MATRIX*pMatrix,XBOOL bRepeat,XBOOL bCreate);
	XBOOL AttachBitmap(XXObjectBitmap*pBitmap,int nDepth,XBOOL  IsCreated);
	void BeginFill(XU8 nType,MATRIX*pMatrix,XU32Array&colors,XU8Array&ratios,XU8 nSpread,XU8 nInterMode);
};

#endif