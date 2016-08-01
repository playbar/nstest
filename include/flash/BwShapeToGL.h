#ifndef __BWSHAPETOGL_H__
#define __BWSHAPETOGL_H__

#include "BwFillStyle.h"
#include "BwShapeRecord.h"
#include "glu/tess.h"
//#include <vector>
//using namespace std;

struct _GLEdge
{
	GLdouble xyz[3];
	_GLEdge* pNext;
};

struct _GLEdge2
{
	GLdouble* pXYZ;
	XU32	 lineID;
	_GLEdge2* pNext;
};

struct _GLPath
{
	_GLEdge* pEdges;
	_GLEdge* pLast;
	int mnPoints;
	BwFillStyle* pFillStyle;
	BwLineStyle* pLineStyle;
};

typedef XVector<_GLPath*> _GLShape;

class TessPloy
{
public:
	
	static BwShapeRecord* m_pRecord;
	XFastAlloc allocPath;
	XFastAlloc allocEdge;
	XFastAlloc allocEdge2;
public:
	_GLEdge* AllocEdge()
	{
		_GLEdge*pEdge=(_GLEdge*)allocEdge.Alloc();
		memset(pEdge,0,sizeof(_GLEdge));
		return pEdge;
	}
	void FreeEdge2(_GLEdge2*pEdge)
	{
		allocEdge2.Free(pEdge);
	}
	_GLEdge2* AllocEdge2()
	{
		return (_GLEdge2*)allocEdge2.Alloc();
	}
	void FreeEdge(_GLEdge*pEdge)
	{
		allocEdge.Free(pEdge);
	}
	_GLPath* AllocPath()
	{
		_GLPath*path=(_GLPath*)allocPath.Alloc();
		memset(path,0,sizeof(_GLPath));
		return path;
	}
	void FreePath(_GLPath*path)
	{
		allocPath.Free(path);
	}
private:
	static GLUtesselator* tobj;
	static void DrawSubShape(_GLShape&shape);
	static void ReleaseShape(_GLShape&shape);
	static void NormalizePaths(BwPath*path,_GLShape&shape); 
	static _GLPath* PathToGL(BwPath*path);
	static _GLPath* PathToRevGL(BwPath*path);
	static void AddCurve(_GLPath*path,_GLEdge*start,_GLEdge*control,_GLEdge*end);
	static void AddCurveR(_GLPath*path,_GLEdge*start,_GLEdge*control,_GLEdge*end);
	static void CombineShape(_GLShape&shape);
	static GLTranglesOrLines* DrawLine(_GLShape&shape);
	static void AddToRaster(BwShapeRecord*pData,GLTranglesOrLines*p);
public:
	static TessPloy* m_pInstance;
	static XBOOL	 m_bODD;
	TessPloy();
	~TessPloy();
	static TessPloy* GetInst()
	{
		if(!m_pInstance) Init();
		return m_pInstance;
	}
	static void Init();
	static void Release();
	static void Tess(BwShapeRecord*pRec,float fMeter,bool bODD=false);
};


#endif

