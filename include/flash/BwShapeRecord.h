#ifndef __BWSHAPERECORD_H__
#define __BWSHAPERECORD_H__

#include "BwFillStyle.h"
//#include <vector>
//using namespace std;
#include "XWindow.h"
#include "XFastAlloc.h"
#include "XXVar.h"


struct BwEdge
{
	XPOINT mControlPt;
	XPOINT mEndPt;
	int    mbLine;
	BwEdge*pNext;
	void SetControlPt(XPOINT&pt)
	{
		mControlPt = pt;
	}
	void SetEndPt(XPOINT&pt)
	{
		mEndPt = pt;
	}
	int  IsLine(){return mbLine;}
	void IsLine(int bLine){mbLine=bLine;}
	void Init()
	{
		memset(this,0,sizeof(BwEdge));
	}
	void SetData(int cx,int cy,int ax,int ay)
	{
		mControlPt.x = cx;
		mControlPt.y = cy;
		mEndPt.x = ax;
		mEndPt.y = ay;
	}
};


enum PtPos
{
	en_PtPos_No = 0,
	en_PtPos_Begin,
	en_PtPos_Mid,
	en_PtPos_End
};

struct BwPath
{
public:
	void Init()
	{
		memset(this,0,sizeof(BwPath));
	}
	void SetData( int ax, int ay, int fillLeft, int fillRight, int line, bool bNewShape)
	{
		mStartPt.x = ax;
		mStartPt.y = ay;
		mFillLeft = fillLeft;
		mFillRight = fillRight;
		mLineStyle = line;
		mbNewShape = bNewShape;
		pEdges = 0;
		pEndEdge = 0;
		mnPoints = 0;
		mbNeedFree = 0;
	}

public:
	void AddEdge(BwEdge *pedge );
	void AddEdgeHeader(BwEdge*pEdge);
	void StartPt( XPOINT &pt )
	{
		mStartPt = pt;
	}
	XPOINT &StartPt(){return mStartPt;}
	void EmptyStartPt()
	{
		mStartPt.x = 0;
		mStartPt.y = 0;
	}
	bool IsStartPtEmpty()
	{
		return mStartPt.x == 0&&
			   mStartPt.y == 0;
	}
	void ClearEdges();
	bool EdgeEmpty()
	{
		return pEdges == 0/* &&( mStartPt.x < -0x7FFFFEC || mStartPt.y < -0x7FFFFEC )*/;
	}
	int PtInPath(XPOINT &pt );


	void FillLeft( int fillleft )
	{
		mFillLeft = fillleft;
	}
	int FillLeft( ){return mFillLeft;}

	void FillRight( int fillRight )
	{
		mFillRight = fillRight;
	}
	int FillRight()
	{
		return mFillRight;
	}

	void LineStyle(int ls )
	{
		mLineStyle = ls;
	}
	int LineStyle()
	{
		return mLineStyle;
	}
	XPOINT& EndPt()
	{
		return pEndEdge->mEndPt;
	}
	void ReleaseEdge();
	BwPath* ReversePath();
public:
	XPOINT mStartPt;	// start point
	BwEdge*pEdges,*pEndEdge;
	BwPath* pNext;
	//vector< BwEdge > mVecEdges;
public:
	int mbNeedFree;
	int mnPoints;
	int mFillLeft;
	int mFillRight;
	int mLineStyle;
	bool mbNewShape;
};

struct GLTrangle
{
	float xyz[9];
	//double y[3];
	GLTrangle* pNext;
};
enum GLT_TYPE
{
	GLT_TRANGLES,GLT_LINES,GLT_RECTANGLE,GLT_SIMPLEFRAME,GLT_SIMPLERECT,GLT_FONT//,GLT_FONT16,GLT_FONT12
};
struct GLTrangles
{
	//GLTrangle* pTrangles;
	//GLdouble   *pVertex;
	//void*	   pNext;
	GLuint		 nBufID;
	BwFillStyle* pFillStyle;
	//int		   nCount;
	//GLT_TYPE   nextType;//if true to draw lines
};

struct GLRectangle
{
	GLfloat		  pos[4];
	BwFillStyle*  pFillStyle;
};
struct GLSimpleRect
{
	GLfloat		  pos[4];
	GLfloat		  color[4];
};

struct GLPoint
{
	float xyz[3];
	GLPoint*pNext;
};
struct GLLines
{
	GLuint nBufID;
	//GLdouble* pPoints;
	//int		  nSize;
	//void*	 pNext;
	BwLineStyle*pLineStyle;
	//int		 nCount;
	//GLT_TYPE nextType;//if true to draw trangles
};

struct GLChar
{
	int w,h;
	float fw;
	int fontSize;
	//float id;
	struct _TEXTUREID* id;
	int   offx,offy;
	int	  fontID;
	//int   nCount;
	U16	  g;
	//XU32  nLastT;
	void* ch;
};
#define MAXCACHET 5000

struct GLTranglesOrLines
{
	union
	{
		GLTrangles		trangles;
		GLLines			lines;
		GLRectangle*	rectangle;
		GLSimpleRect*	rect;
		GLChar*			pChar;
	};
	GLTranglesOrLines* pNext;
	int		 nCount;
	GLT_TYPE type;
};

/*struct GLRasterData
{
	//float matrix[16];
	//float colorTransform[16];
	GLTranglesOrLines* pFirst;
	GLTranglesOrLines* pLast;
	//int		   needMatrix;
	//int		   needTransform;
	//MATRIX matrix;
};*/
struct _ShapeImage
{
	MATRIX mat;
	//GLuint id;
	struct _TEXTUREID* id;
	//XU32   nTimer;
	//int	   useCount;
	int    w,h;
};
struct BwShapeRecord
{
public:
	//BwShapeRecord();
	//~BwShapeRecord();
	void ClearImage();
	int m_nSampleType;
	_ShapeImage* m_pImage;
	XU8Array	 m_imgData;
	void SetImage(XDib&dib,MATRIX&m,bool bSave);
public:
	//void Draw(RasterGL*pRaster);
	void AddFillSyle( int index,BwFillStyle *fs );
	void AddLineStyle( int index,BwLineStyle*ls );
	void AddFillStyle(BwFillStyle*fs)
	{
		int id=mnFills;
		SetFills(mnFills+1);
		mVecFillStyles[id] = fs;
	}
	void AddLineStyle(BwLineStyle*ls)
	{
		int id=mnLines;
		SetLines(mnLines+1);
		mVecLineStyles[id]=ls;
	}
	void AddPath( BwPath*path );
	bool IsInPaths(int x, int y );
	void SetBounds( );
	void ReleaseTexture();
	void ReleaseFillStyle();
	void ReleaseLineStyle();
	void ReleasePaths();
	void ReleaseData();
	void ReleaseImage();
	void ReleaseTrangles(GLTrangle*pTrangles);
	void ReleaseLines(GLPoint*pLines);
	void ReleaseAll(bool bImage=true);
	void Init();
	static void ReleaseShapeList(GLTranglesOrLines*pData);
	static void ReleaseFontData(GLTranglesOrLines**pData,int nCount);
	//static void ReleaseFontColor(GLColor**pColor,int nCount);
	//BwPath*GetPath(int index );
	XBOOL IsEmpty()
	{
		return mpPaths==XNULL;
	}
	void SetFills(int iCount)
	{
		//iCount+=mnFills;
		if(iCount<=mnFills) return;
		BwFillStyle** pFills = (BwFillStyle**)XXVar::_AllocData(iCount*sizeof(BwFillStyle*));
		memset(pFills,0,iCount*sizeof(BwFillStyle*));
		if(mnFills)
		{
			//int i;
			memcpy(pFills,mVecFillStyles,mnFills*sizeof(BwFillStyle*));
		}
		if(mVecFillStyles)
			XXVar::_FreeData(mVecFillStyles);
		mVecFillStyles = pFills;
		mnFills = iCount;
	}
	void SetLines(int iCount)
	{
		//iCount+=mnLines;
		if(iCount<=mnLines)
			return;
		BwLineStyle** pLines = (BwLineStyle**)XXVar::_AllocData(iCount*sizeof(BwLineStyle*));
		memset(pLines,0,iCount*sizeof(BwLineStyle*));
		if(mnLines)
		{
			//int i;
			memcpy(pLines,mVecLineStyles,mnLines*sizeof(BwLineStyle*));
		}
		if(mVecLineStyles)
			XXVar::_FreeData(mVecLineStyles);
		mVecLineStyles = pLines;
		mnLines = iCount;
	}
public:
	void AddData(GLTranglesOrLines*pTL)
	{
		if(!pFirst)
		{
			pFirst = pLast = pTL;
		}
		else
		{
			pLast->pNext = pTL;
			pLast = pTL;
		}
		pLast->pNext = NULL;
	}
	//vector<BwFillStyle*> mVecFillStyles;
	//vector<BwLineStyle*> mVecLineStyles;
	BwFillStyle** mVecFillStyles;
	BwLineStyle** mVecLineStyles;
	//vector<BwPath*> mVecPaths;
	BwPath* mpPaths,*mpLastPath;
	int mnFills,mnLines;
public:
	bool  m_bODD;
	float fMeter;
	void SetMeter(MATRIX&mat);
	GLTranglesOrLines* pFirst;
	GLTranglesOrLines* pLast;
	//GLRasterData* pRasterData;
	//BwPath mClipPath;
};
struct GLShapeData
{
	GLTranglesOrLines* pData;
	float			   color[4];
	float*			   matrix;
	GLShapeData*	   pNext;
};
struct GLShapeListData
{
	GLShapeData* pData;
	float*		 matrix;
	float*		 colorTransform;
	int			 blendMode;
	GLShapeListData* pNext;
};
class ShapeAlloc
{
public:
	XFastAlloc edgeAlloc;
	XFastAlloc pathAlloc;
	XFastAlloc colorAlloc;
	XFastAlloc lineStyleAlloc;
	XFastAlloc pointAlloc;
	XFastAlloc trangleAlloc;
	XFastAlloc tranglesOrLinesAlloc;
	XFastAlloc rampAlloc;
	XFastAlloc recordAlloc;
	XFastAlloc shapeDataAlloc;
	XFastAlloc shapeListAlloc;
	XFastAlloc rectangleAlloc;
	XFastAlloc simpleRectAlloc;
	XFastAlloc charAlloc;
	XFastAlloc imageAlloc;
public:
	_ShapeImage* AllocImage(){return (_ShapeImage*)imageAlloc.Alloc();}
	void FreeImage(_ShapeImage*pImage){imageAlloc.Free(pImage);}
	GLChar* AllocChar()
	{
		return (GLChar*)charAlloc.Alloc();
	}
	void FreeChar(GLChar*pChar)
	{
		charAlloc.Free(pChar);
	}
	GLSimpleRect* AllocSimpleRect()
	{
		return (GLSimpleRect*)simpleRectAlloc.Alloc();
	}
	void FreeSimpleRect(GLSimpleRect*pRect)
	{
		simpleRectAlloc.Free(pRect);
	}
	GLRectangle* AllocRectangle()
	{
		return (GLRectangle*)rectangleAlloc.Alloc();
	}
	void FreeRectangle(GLRectangle*pRect)
	{
		rectangleAlloc.Free(pRect);
	}
	GLShapeData* AllocShapeData()
	{
		GLShapeData* pData =(GLShapeData*)shapeDataAlloc.Alloc();
		memset(pData,0,sizeof(GLShapeData));
		//pData->matrix = 0;
		//pData->pNext = 0;
		return pData;
	}
	void FreeShapeData(GLShapeData*pData)
	{
		shapeDataAlloc.Free(pData);
	}
	GLShapeListData* AllocShapeList()
	{
		GLShapeListData* pData = (GLShapeListData*)shapeListAlloc.Alloc();
		pData->pData = NULL;
		pData->matrix = NULL;
		pData->pNext = NULL;
		pData->colorTransform = NULL;
		return pData;
	}

	void FreeShapeList(GLShapeListData*pData)
	{
		if(pData->matrix)
			FreeRamp(pData->matrix);
		if(pData->colorTransform)
			FreeRamp(pData->colorTransform);
		shapeListAlloc.Free(pData);
	}

	BwShapeRecord* AllocRecord()
	{
		BwShapeRecord*pRec = (BwShapeRecord*)recordAlloc.Alloc();
		pRec->Init();
	}
	void FreeRecord(BwShapeRecord*pRec)
	{
		pRec->ReleaseAll();
		recordAlloc.Free(pRec);
	}
	float* AllocRamp()
	{
		return (float*)rampAlloc.Alloc();
	}
	void FreeRamp(float*pData)
	{
		rampAlloc.Free(pData);
	}
	GLTranglesOrLines* AllocTrangleOrLines()
	{
		return (GLTranglesOrLines*)tranglesOrLinesAlloc.Alloc();
	}
	void FreeTranglesOrLines(GLTranglesOrLines*pData)
	{
		tranglesOrLinesAlloc.Free(pData);
	}
	GLPoint* AllocPoint()
	{
		return (GLPoint*)pointAlloc.Alloc();
	}
	void FreePoint(GLPoint*point)
	{
		pointAlloc.Free(point);
	}
	GLTrangle* AllocTrangle()
	{
		return (GLTrangle*)trangleAlloc.Alloc();
	}
	void FreeTrangle(GLTrangle*p)
	{
		trangleAlloc.Free(p);
	}

	BwEdge* AllocEdge()
	{
		BwEdge*pEdge=(BwEdge*)edgeAlloc.Alloc();
		memset(pEdge,0,sizeof(BwEdge));
		return pEdge;
	}
	void FreeEdge(BwEdge*pEdge)
	{
		edgeAlloc.Free(pEdge);
	}
	BwPath* AllocPath()
	{
		BwPath*path= (BwPath*)pathAlloc.Alloc();
		memset(path,0,sizeof(BwPath));
		return path;
	}
	void FreePath(BwPath*path)
	{
		pathAlloc.Free(path);
	}
	GLColor* AllocColor()
	{
		GLColor*pColor= (GLColor*)colorAlloc.Alloc();
		memset(pColor,0,sizeof(GLColor));
		return pColor;
	}
	void FreeColor(GLColor*color)
	{
		colorAlloc.Free(color);
	}
	BwLineStyle* AllocLineStyle()
	{
		BwLineStyle*pStyle= (BwLineStyle*)lineStyleAlloc.Alloc();
		memset(pStyle,0,sizeof(BwLineStyle));
		return pStyle;
	}
	void FreeLineStyle(BwLineStyle*style)
	{
		lineStyleAlloc.Free(style);
	}
public:
	ShapeAlloc();
	~ShapeAlloc();
	static ShapeAlloc*		m_pInstance;
	static ShapeAlloc*		GetInst()
	{
		if(!m_pInstance)
			m_pInstance=new ShapeAlloc();
		return m_pInstance;
	}
	static			void	Release()
	{
		if(m_pInstance) 
			delete m_pInstance;
		m_pInstance=0;
	}

};

#endif

