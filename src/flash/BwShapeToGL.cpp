#include "StdAfxflash.h"
#include "BwShapeToGL.h"
#include "Raster_gl.h"
#include "XXVar.h"

GLUtesselator* TessPloy::tobj=XNULL;
TessPloy* TessPloy::m_pInstance=XNULL;
BwShapeRecord* TessPloy::m_pRecord=XNULL;
static _GLEdge2* _pVertex=XNULL;
static _GLEdge2* _pLast=XNULL;
static int		_verCount=0;
static float	_fMeter = 20.0;

static XU32 _nTotal = 0;
static XU32 _nCount = 0;

#define _GL_BLOCKSIZE 200*3

//static BwShapeRecord* _pRec=XNULL;
TessPloy::TessPloy():
		allocPath(sizeof(_GLPath),100),
		allocEdge(sizeof(_GLEdge),200),
		allocEdge2(sizeof(_GLEdge2),200)
{
}
TessPloy::~TessPloy()
{
	//Release();
}
void TessPloy::Release()
{
	if(tobj)
	{
		 gluDeleteTess( tobj );
		 tobj = XNULL;
	}
	if(m_pInstance)
		delete m_pInstance;
	m_pInstance=XNULL;
}

void __stdcall vertexCallback(GLdouble *vertex)
{
	_GLEdge2*pEdge = TessPloy::m_pInstance->AllocEdge2();
	pEdge->pNext = XNULL;
	pEdge->lineID = 0;
	pEdge->pXYZ=vertex;
	vertex[2]=0;
	/*pEdge->xyz[0]=vertex[0];
	pEdge->xyz[1]=vertex[1];
	pEdge->xyz[2]=vertex[2];*/
	if(!_pVertex)
	{
		_pVertex = _pLast = pEdge;
	}
	else
	{
		_pLast->pNext = pEdge;
		_pLast = pEdge;
	}
	_verCount++;
	//AddPoint(vertex[0],vertex[1]);
}

void __stdcall beginCallback( GLenum which )
{

}

void __stdcall endCallback( )
{

}		 

void __stdcall errorCallback( GLenum errorCode )
{

}
#define _MAXCOMBINES 2100
static GLdouble _combineVertex[_MAXCOMBINES];
static int      _combineIndex = 0;

void __stdcall combineCallback(GLdouble coords[3], 
							   GLdouble *vertex_data[4],
							   GLfloat weight[4], GLdouble **dataOut )
{
	GLdouble *vertex;
	int i;

	vertex = _combineVertex+_combineIndex;
	_combineIndex += 3;
	if(_combineIndex>=_MAXCOMBINES)
		_combineIndex = 0;
		//(GLdouble *) malloc(3 * sizeof(GLdouble));

	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	/*for (i = 3; i < 6; i++)
		vertex[i] = weight[0] * vertex_data[0][i] 
					+ weight[1] * vertex_data[1][i]
					+ weight[2] * vertex_data[2][i]; 
					+ weight[3] * vertex_data[3][i];*/
	*dataOut = vertex;
	//AddPoint(vertex[0],vertex[1]);
}

void __stdcall edgeCallback(GLboolean flag)
{
}

void TessPloy::Init()
{
	 if(!m_pInstance)
		 m_pInstance = new TessPloy();
	 if(!tobj)
	 {
		 tobj = gluNewTess();
		 gluTessCallback(tobj, GLU_TESS_VERTEX, (void (__stdcall *)())vertexCallback);
		 gluTessCallback(tobj, GLU_TESS_BEGIN,  (void (__stdcall *)())beginCallback);
		 gluTessCallback(tobj, GLU_TESS_END, (void (__stdcall *)())endCallback);
		 gluTessCallback(tobj, GLU_TESS_ERROR, (void (__stdcall *)())errorCallback);
		 gluTessCallback(tobj, GLU_TESS_COMBINE, (void (__stdcall *)())combineCallback);
		 gluTessCallback(tobj,  GLU_TESS_EDGE_FLAG,(void (__stdcall*)())edgeCallback);
	 }
}

#define CREATEEDGE(EDGE,PT)\
	EDGE=pAlloc->AllocEdge();\
	{	EDGE->xyz[0] = (GLdouble)PT.x;EDGE->xyz[1]=PT.y;}
#define SETEDGE(EDGE,PATH)\
	{PATH->pEdges=PATH->pLast=EDGE;PATH->mnPoints=1;}
#define ADDEDGE(EDGE,PATH)\
	{PATH->pLast->pNext=EDGE;PATH->pLast=EDGE;\
	 PATH->mnPoints++;}
#define ADDEDGER(EDGE,PATH)\
	{EDGE->pNext=PATH->pEdges;PATH->pEdges=EDGE;\
	 PATH->mnPoints++;}

#define MIDPT(P1,P2,M)\
{M->xyz[0]=(P1->xyz[0]+P2->xyz[0]+0.5)/2;M->xyz[1]=(P1->xyz[1]+P2->xyz[1]+0.5)/2;}
void TessPloy::AddCurve(_GLPath*path,_GLEdge*start,_GLEdge*control,_GLEdge*end)
{
	_GLEdge m;
	MIDPT(start,end,(&m));
	_GLEdge* mc=m_pInstance->AllocEdge();
	MIDPT((&m),control,mc);
	GLdouble dx = mc->xyz[0]-m.xyz[0];
	GLdouble dy = mc->xyz[1]-m.xyz[1];
	if(dx*dx+dy*dy<_fMeter)   //
	{
		ADDEDGE(control,path);
		ADDEDGE(end,path);
		m_pInstance->FreeEdge(mc);
	}
	else
	{
		_GLEdge* pEdge=m_pInstance->AllocEdge();
		MIDPT(start,control,pEdge);
		AddCurve(path,start,pEdge,mc);
		pEdge = m_pInstance->AllocEdge();
		MIDPT(control,end,pEdge);
		AddCurve(path,mc,pEdge,end);
		m_pInstance->FreeEdge(control);
	}
}

void TessPloy::AddCurveR(_GLPath*path,_GLEdge*start,_GLEdge*control,_GLEdge*end)
{
	_GLEdge m;
	MIDPT(start,end,(&m));
	_GLEdge* mc=m_pInstance->AllocEdge();
	MIDPT((&m),control,mc);
	GLdouble dx = mc->xyz[0]-m.xyz[0];
	GLdouble dy = mc->xyz[1]-m.xyz[1];
	if(dx*dx+dy*dy<_fMeter)   //
	{
		ADDEDGER(control,path);
		ADDEDGER(end,path);
		m_pInstance->FreeEdge(mc);
	}
	else
	{
		_GLEdge* pEdge=m_pInstance->AllocEdge();
		MIDPT(start,control,pEdge);
		AddCurveR(path,start,pEdge,mc);
		pEdge = m_pInstance->AllocEdge();
		MIDPT(control,end,pEdge);
		AddCurveR(path,mc,pEdge,end);
		m_pInstance->FreeEdge(control);
	}
}

_GLPath* TessPloy::PathToGL(BwPath*path)
{
	TessPloy* pAlloc=TessPloy::GetInst();
	_GLPath* gp = pAlloc->AllocPath();
	_GLEdge*pEdge;
	CREATEEDGE(pEdge,path->mStartPt);
	SETEDGE(pEdge,gp);
	BwEdge*pEdges = path->pEdges;
	while(pEdges)
	{
		if(pEdges->mbLine)
		{
			CREATEEDGE(pEdge,pEdges->mEndPt);
			ADDEDGE(pEdge,gp);
		}
		else
		{
			_GLEdge* pEdge1,*pEdge2;
			CREATEEDGE(pEdge1,pEdges->mControlPt);
			CREATEEDGE(pEdge2,pEdges->mEndPt);
			AddCurve(gp,pEdge,pEdge1,pEdge2);
			pEdge = pEdge2;
		}
		pEdges = pEdges->pNext;
	}
	return gp;
}
_GLPath* TessPloy::PathToRevGL(BwPath*path)
{
	TessPloy* pAlloc=TessPloy::GetInst();
	_GLPath* gp = pAlloc->AllocPath();
	_GLEdge*pEdge;
	CREATEEDGE(pEdge,path->mStartPt);
	SETEDGE(pEdge,gp);
	BwEdge*pEdges = path->pEdges;
	while(pEdges)
	{
		if(pEdges->mbLine)
		{
			CREATEEDGE(pEdge,pEdges->mEndPt);
			ADDEDGER(pEdge,gp);
		}
		else
		{
			_GLEdge* pEdge1,*pEdge2;
			CREATEEDGE(pEdge1,pEdges->mControlPt);
			CREATEEDGE(pEdge2,pEdges->mEndPt);
			AddCurveR(gp,pEdge,pEdge1,pEdge2);
			pEdge = pEdge2;
		}
		pEdges = pEdges->pNext;
	}
	return gp;
}

void TessPloy::NormalizePaths(BwPath*path,_GLShape&shape)
{
	if(path->EdgeEmpty()) return;
	else if(path->mFillLeft&&path->mFillRight)
	{
		_GLPath* gp = PathToGL(path);
		gp->pFillStyle=m_pRecord->mVecFillStyles[path->mFillRight];
		if(path->mLineStyle)
			gp->pLineStyle=m_pRecord->mVecLineStyles[path->mLineStyle];
		shape.Add(gp);
		gp = PathToRevGL(path);
		gp->pFillStyle=m_pRecord->mVecFillStyles[path->mFillLeft];
		//if(path->mLineStyle)
		//	gp->pLineStyle=m_pRecord->mVecLineStyles[path->mLineStyle];
		shape.Add(gp);
	}
	else if(path->mFillLeft)
	{
		_GLPath*gp = PathToRevGL(path);
		gp->pFillStyle=m_pRecord->mVecFillStyles[path->mFillLeft];
		if(path->mLineStyle)
			gp->pLineStyle=m_pRecord->mVecLineStyles[path->mLineStyle];
		shape.Add(gp);
	}
	else if(path->mFillRight||path->mLineStyle)
	{
		_GLPath* gp = PathToGL(path);
		if(path->mFillRight)
			gp->pFillStyle=m_pRecord->mVecFillStyles[path->mFillRight];
		if(path->mLineStyle)
			gp->pLineStyle=m_pRecord->mVecLineStyles[path->mLineStyle];
		shape.Add(gp);
	}
}

XBOOL TessPloy::m_bODD = XFALSE;

void TessPloy::Tess(BwShapeRecord*pRec,float fMeter,bool bODD)
{
	if(!tobj) Init();
	//XU32 t1=XTime::GetTickCount();
	_fMeter = pRec->fMeter;
	m_bODD = bODD;
	m_pRecord = pRec;
	BwPath* path = pRec->mpPaths;
	_GLShape shape;//,saveShape;
	_combineIndex = 0;
	while(path)
	{
		if(path->mbNewShape)
		{//Draw path
			//saveShape.Append(shape);
			DrawSubShape(shape);
			ReleaseShape(shape);
		}
		NormalizePaths(path,shape);
		path = path->pNext;
	}
	if(shape.GetSize()>0)
	{
		//saveShape.Append(shape);
		DrawSubShape(shape);
		ReleaseShape(shape);
	}
	
	//if(m_pInstance->allocEdge.m_nAllocs)
	//	int v=0;
	//pRec->ReleasePaths();
}

void TessPloy::ReleaseShape(_GLShape&shape)
{
	TessPloy* pAlloc=TessPloy::GetInst();
	for(int i=0;i<shape.GetSize();i++)
	{
		_GLPath*path = shape[i];
		_GLEdge*pEdge = path->pEdges;
		while(pEdge)
		{
			_GLEdge*pNext = pEdge->pNext;
			pAlloc->FreeEdge(pEdge);
			pEdge = pNext;
		}
		pAlloc->FreePath(path);
	}
	shape.RemoveAll(XFALSE);

}

void TessPloy::CombineShape(_GLShape&shape)
{
	
	int i=0,j,iSize=shape.GetSize();
	for(i=0;i<iSize-1;i++)
	{
		_GLPath*fs = shape[i];
		j=i+1;
		while(j<iSize)
		{
			_GLPath*fp = shape[j];
			if(fs->pLast->xyz[0]==fp->pEdges->xyz[0]&&
				fs->pLast->xyz[1]==fp->pEdges->xyz[1])
			{
				fs->pLast->pNext=fp->pEdges;
				fs->pLast=fp->pLast;
				shape.RemoveAt(j);
				m_pInstance->FreePath(fp);
				iSize--;
				j=i+1;
			}
			else if(fp->pLast->xyz[0]==fs->pEdges->xyz[0]&&
					fp->pLast->xyz[1]==fs->pEdges->xyz[1])
			{
				fp->pLast->pNext = fs->pEdges;
				fs->pEdges = fp->pEdges;
				shape.RemoveAt(j);
				m_pInstance->FreePath(fp);
				iSize--;
				j=i+1;
			}
			else
			{
				j++;
			}
		}
	}
}

GLTranglesOrLines* TessPloy::DrawLine(_GLShape&shape)
{
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	GLTranglesOrLines*pLines = XNULL;
	GLTranglesOrLines*pLast = pLines;
	RasterGL* raster = RasterGL::sharedRasterGL();
	int iBase = 0,iSize = shape.GetSize();
	while(iBase<iSize)
	{
		int iStart=iBase;
		while(iStart<iSize)
		{
			if(shape[iStart]->pLineStyle) break;
			iStart++;
		}
		if(iStart>=iSize) break;
		int iEnd = iStart+1;
		_GLPath*path = shape[iStart];
		int iCount = path->mnPoints;
		/*while(iEnd<iSize)
		{
			if(shape[iEnd]->pLineStyle!=path->pLineStyle) break;
			iCount+=shape[iEnd]->mnPoints;
			iEnd++;
		}*/
		if(iCount>1)
		{
			GLTranglesOrLines* pNew = pAlloc->AllocTrangleOrLines();
			pNew->nCount = iCount;
			pNew->pNext = XNULL;
			pNew->type = GLT_LINES;
			pNew->lines.pLineStyle = path->pLineStyle;
			pNew->lines.nBufID = raster->GenBuffer();//pPoints = (GLdouble*)XXVar::_AllocData(iCount*sizeof(GLdouble)*3);
			//GLdouble*pData = pNew->lines.pPoints;
			//int iSet = 0;
			int iOffset = 0;
			raster->BindBuffer(pNew->lines.nBufID);
			int i;
			if(path->pLineStyle->mWidth<=_MINLINEW)
			{
				 raster->BufferData(sizeof(GLfloat)*3*pNew->nCount,0);
				 int iOffset = 0;
				 int iSubOffset = 0;
				 float data[_GL_BLOCKSIZE];
				 for(i=iStart;i<iEnd;i++)
				 {
					 path = shape[i];
					 _GLEdge* pEdge = path->pEdges;
					 while(pEdge)
					 {
						 _GLEdge*pNext = pEdge->pNext;
						 //raster->BufferSubData(iOffset,sizeof(GLdouble)*3,pEdge->xyz);
						 //memcpy(data+iOffset,pEdge->xyz,sizeof(GLdouble)*3);
						 data[iOffset]=pEdge->xyz[0];
						 data[iOffset+1]=pEdge->xyz[1];
						 data[iOffset+2]=pEdge->xyz[2];
						 iOffset+=3;
						 if(iOffset>=_GL_BLOCKSIZE)
						 {
							 iOffset = 0;
							 raster->BufferSubData(iSubOffset,_GL_BLOCKSIZE*sizeof(GLfloat),data);
							 iSubOffset += _GL_BLOCKSIZE*sizeof(GLfloat);
						 }
						 //pData+=3;
						 //iOffset += 3*sizeof(GLdouble);
						 //m_pInstance->FreeEdge(pEdge);
						 pEdge = pNext;
					 }
				 }
				 if(iOffset)
				 {
					raster->BufferSubData(iSubOffset,iOffset*sizeof(GLfloat),data);
						// iSubOffset += _GL_BLOCKSIZE*sieof(GLfloat);
				 }
			}
			else
			{
				LineTriangle data[6];
				int iSize=sizeof(data);
				raster->BufferData(iSize*(pNew->nCount-1),0);//pNew->nCount*sizeof(GLdouble)*3,0);
				
				double lw = path->pLineStyle->mWidth/2.0;
				//if(lw<12) lw=12;
				for(i=iStart;i<iEnd;i++)
				{
					path = shape[i];
					_GLEdge* pEdge0 = path->pEdges;
					_GLEdge* pEdge = pEdge0->pNext;
					while(pEdge)
					{
						//memcpy(pData,pEdge->xyz,sizeof(GLdouble)*3);
						//pData+=3;
						
						raster->MakeSegment(pEdge0->xyz,pEdge->xyz,lw,data);
						raster->BufferSubData(iOffset,iSize,data);
						iOffset += iSize;
						pEdge0 = pEdge;
						pEdge = pEdge->pNext;
					}
				}
			}
			raster->BindBuffer(0);
			if(!pLines)
			{
				pLines = pLast = pNew;
			}
			else
			{
				pLast->pNext = pNew;
				pLast = pNew;
			}
		}
		iBase = iEnd;
	}
	
	return pLines;
}

inline void TessPloy::AddToRaster(BwShapeRecord*pData,GLTranglesOrLines*p)
{
	if(!pData->pFirst)
	{
		pData->pFirst = pData->pLast = p;
	}
	else
	{
		pData->pLast->pNext = p;
		pData->pLast = p;
		while(pData->pLast->pNext)
			pData->pLast=pData->pLast->pNext;
	}
}

void TessPloy::DrawSubShape(_GLShape &shape)
{
	//GLTranglesOrLines* pMyLines=NULL;
	//GLTranglesOrLines* pMyLast=NULL;
	//XU32 t1=XTime::GetTickCount();
	GLTranglesOrLines*pLines=DrawLine(shape);
	for(;;)
	{
		int i=0,iSize=shape.GetSize();
		if(!iSize) break;
		_GLShape fs;
		_GLPath*path=shape[0];
		shape.RemoveAt(0);
		iSize--;
		//if(!path->pFillStyle)
		//	continue;
		fs.Add(path);

		while(i<iSize)
		{
			_GLPath*sp=shape[i];
			if(sp->pFillStyle==path->pFillStyle)
			{
				//CombineAdd(fs,sp);
				fs.Add(sp);
				shape.RemoveAt(i);
				iSize--;
			}
			else i++;
		}
		//GLTranglesOrLines*pLines=DrawLine(fs);
		if(path->pFillStyle)
		{
			CombineShape(fs);
			_pVertex = _pLast = XNULL;
			_verCount = 0;
			gluTessProperty(tobj, GLU_TESS_WINDING_RULE, m_bODD?GLU_TESS_WINDING_ODD:GLU_TESS_WINDING_NONZERO);
			 gluTessBeginPolygon(tobj, NULL);	
			 int iCount = fs.GetSize();
			 //int iBase = 0;
			 //if(
			 for(i=0;i<iCount;i++)
			 {
				  gluTessBeginContour(tobj);
				  _GLPath* path = fs[i];
				  //int jCount = pts.GetSize();
				  _GLEdge*pEdge = path->pEdges;
				  if(pEdge->xyz[0]==path->pLast->xyz[0]&&
					  pEdge->xyz[1]==path->pLast->xyz[1])
					  pEdge = pEdge->pNext;
				  //for(j=0;j<jCount;j++)
				  //int iSet = 0;
				  while(pEdge)
				  {
					  //_POINT&pt = pts[j];
					  //GLdouble tmp[]={star[j][0],star[j][1],star[j][2],star[j][3],star[j][4],star[j][5]};
					  //pEdge->xyz[2] = iSet|(i<<20);
					  //iSet++;
					  gluTessVertex(tobj,pEdge->xyz,pEdge->xyz);//(double*)&pt,(double*)&pt);
					  pEdge = pEdge->pNext;
					  //gluTessVertex(tobj, star[j], star[j]);
				  }
				  gluTessEndContour(tobj);
			 }
			 gluTessEndPolygon(tobj);
			 if(_verCount>0)
			 {
				 RasterGL* raster = RasterGL::sharedRasterGL();
				 GLTranglesOrLines*pFill = ShapeAlloc::GetInst()->AllocTrangleOrLines();
				 pFill->pNext = NULL;
				 pFill->nCount = _verCount;
				 pFill->type = GLT_TRANGLES;
				 pFill->trangles.pFillStyle = path->pFillStyle;
				 pFill->trangles.nBufID = raster->GenBuffer();//pVertex = (GLdouble*)XXVar::_AllocData(_verCount*sizeof(GLdouble)*3);
				 _GLEdge2* pEdge = _pVertex;
				 /*{

				 }
				 pEdge = _pVerTex;*/
				 raster->BindBuffer(pFill->trangles.nBufID);
				 raster->BufferData(_verCount*3*sizeof(GLfloat),0);
				 //GLdouble*pData = pFill->trangles.pVertex;
				 int iOffset = 0;
				 int iSubOffset = 0;
				 float data[_GL_BLOCKSIZE];
                 //if (_verCount == 6)
                 //    int a = 0;
				 while(pEdge)
				 {
					 _GLEdge2*pNext = pEdge->pNext;
					 //raster->BufferSubData(iOffset,sizeof(GLdouble)*3,pEdge->xyz);
					 //memcpy(data+iOffset,pEdge->xyz,sizeof(GLdouble)*3);
					 data[iOffset]=pEdge->pXYZ[0];
					 data[iOffset+1]=pEdge->pXYZ[1];
					 data[iOffset+2]=0;
					 iOffset+=3;
					 if(iOffset>=_GL_BLOCKSIZE)
					 {
						 iOffset = 0;
						 raster->BufferSubData(iSubOffset,_GL_BLOCKSIZE*sizeof(GLfloat),data);
						 iSubOffset += _GL_BLOCKSIZE*sizeof(GLfloat);
					 }
					 //pData+=3;
					 //iOffset += 3*sizeof(GLdouble);
					 m_pInstance->FreeEdge2(pEdge);
					 pEdge = pNext;
				 }
				 if(iOffset)
				 {
					raster->BufferSubData(iSubOffset,iOffset*sizeof(GLfloat),data);
						// iSubOffset += _GL_BLOCKSIZE*sieof(GLfloat);
				 }
				 //raster->BufferData(_verCount*3*sizeof(GLfloat),data);
				 raster->BindBuffer(0);
				 AddToRaster(m_pRecord,pFill);
			 }
			 
			 //ADDTORASTER(pFill);
		}
		/*if(pLines)
		{
			if(!pMyLines)
				pMyLines = pMyLast = pLines;
			else
			{
				pMyLast->pNext = pLines;
				pMyLast = pLines;
				while(pMyLast->pNext)
					pMyLast=pMyLast->pNext;
			}
		}*/
		//if(pLines)
		//	AddToRaster(m_pRecord,pLines);
		for(i=0;i<fs.GetSize();i++)
		{
			_GLPath* path = fs[i];
			_GLEdge* pEdge = path->pEdges;
			while(pEdge)
			{
				_GLEdge*pNext = pEdge->pNext;
				m_pInstance->FreeEdge(pEdge);
				pEdge = pNext;
			}
			m_pInstance->FreePath(path);
		}
	}
	if(pLines)
		AddToRaster(m_pRecord,pLines);//pMyLines);

	/*XU32 t2=XTime::GetTickCount()-t1;
	_nTotal+=t2;
	_nCount++;
	XU32 nAvg = _nTotal/_nCount;*/
}