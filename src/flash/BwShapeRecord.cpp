#include "StdAfxflash.h"
#include "BwShapeRecord.h"
#include "BwShapeToGL.h"
#include "Raster_GL.h"
#include "XXVar.h"
#include "../libpng/BWPngEncode.h"

ShapeAlloc* ShapeAlloc::m_pInstance = XNULL;

ShapeAlloc::ShapeAlloc():
			edgeAlloc(sizeof(BwEdge),300),
			colorAlloc(sizeof(GLColor),600),
			pathAlloc(sizeof(BwPath),100),
			lineStyleAlloc(sizeof(BwLineStyle),300),
			pointAlloc(sizeof(GLPoint),2000),
			trangleAlloc(sizeof(GLTrangle),900),
			tranglesOrLinesAlloc(sizeof(GLTranglesOrLines),300),
			rampAlloc(sizeof(float)*16,200),
			recordAlloc(sizeof(BwShapeRecord),50),
			shapeDataAlloc(sizeof(GLShapeData),50),
			shapeListAlloc(sizeof(GLShapeListData),50),
			rectangleAlloc(sizeof(GLRectangle),50),
			simpleRectAlloc(sizeof(GLSimpleRect),50),
			charAlloc(sizeof(GLChar),100),
			imageAlloc(sizeof(_ShapeImage),50)
{
}
ShapeAlloc::~ShapeAlloc()
{
	edgeAlloc.ReleaseData();
	colorAlloc.ReleaseData();
	pathAlloc.ReleaseData();
	lineStyleAlloc.ReleaseData();
}



/*BwEdge::BwEdge()
{
	memset( &mControlPt, 0, sizeof( XPoint ));
	memset( &mEndPt, 0, sizeof( XPoint ));
	mbLine = 0;
}

BwEdge::BwEdge( int cx, int cy, int ax, int ay)
{
	mControlPt.x = cx;
	mControlPt.y = cy;
	mEndPt.x = ax;
	mEndPt.y = ay;
}

BwEdge::~BwEdge()
{

}

void BwEdge::IsLine(int bline )
{
	mbLine = bline;
	return;
}

int BwEdge::IsLine()
{
	return mbLine;
	//return mControlPt == mEndPt;
}

void BwEdge::SetControlPt( XPoint &pt )
{
	mControlPt = pt;
}

void BwEdge::SetEndPt( XPoint &pt )
{
	mEndPt = pt;
}*/

//////////////////////////////////////////////////////////////////////////
void BwPath::AddEdgeHeader(BwEdge*pEdge)
{
	if(pEdges == 0)
	{
		pEdge->pNext=0;
		pEndEdge=pEdges=pEdge;
	}
	else
	{
		pEdge->pNext=pEdges;
		pEdges=pEdge;
	}
	mnPoints++;
}

void BwPath::AddEdge(BwEdge *pEdge )
{
	pEdge->pNext = 0;
	if(pEdges == 0)
	{	
		pEndEdge=pEdges = pEdge;
	}
	else
	{
		pEndEdge->pNext = pEdge;
		pEndEdge = pEdge;
	}
	mnPoints++;
	//mVecEdges.push_back( pedge );
	//return;
}



void BwPath::ClearEdges()
{
	mStartPt.x = 0;
	mStartPt.y = 0;
	mnPoints = 0;
	BwEdge*pEdge=pEdges;
	while(pEdge)
	{
		BwEdge*pNext=pEdge->pNext;
		ShapeAlloc::GetInst()->FreeEdge(pEdge);
		pEdge=pNext;
	}
	pEdges=0;
	pEndEdge=0;
	//mVecEdges.clear();
	//mVecEdges.resize( 0 );
	return;
}

int BwPath::PtInPath(XPOINT &pt )
{
	if( mStartPt.x == pt.x&&
		mStartPt.y == pt.y)
		return en_PtPos_Begin;
	//else if( mVecEdges.back().mEndPt == pt )
	//	return en_PtPos_End;
	else 
	{
		BwEdge*pEdge=pEdges;
		while(pEdge)
		{
			XPOINT&pt0 = pEdge->mEndPt;
			if(pt0.x == pt.x&&
			   pt0.y == pt.y)
			   return en_PtPos_Mid;
			pEdge = pEdge->pNext;
		}
		/*vector< BwEdge >::iterator it = mVecEdges.begin();
		vector< BwEdge >::iterator end = mVecEdges.end();
		for( ; it != end; it++ )
		{
			BwEdge &edge = *it;
			if( edge.mEndPt == pt )
				return en_PtPos_Mid;
		}*/
	}
	return en_PtPos_No;
}

/*XPoint &BwPath::StartPt()
{
	return mStartPt;
}

void BwPath::FillLeft( int fillleft )
{
	mFillLeft = fillleft;
}

int BwPath::FillLeft( )
{
	return mFillLeft;
}

void BwPath::FillRight( int fillRight )
{
	mFillRight = fillRight;
}
int BwPath::FillRight()
{
	return mFillRight;
}

void BwPath::LineStyle(int ls )
{
	mLineStyle = ls;
}

int BwPath::LineStyle()
{
	return mLineStyle;
}*/
BwPath* BwPath::ReversePath()
{
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	BwPath* path=pAlloc->AllocPath();
	XPOINT pt=mStartPt;
	BwEdge*pEdge=pEdges;
	while(pEdge)
	{
		BwEdge*pNew=pAlloc->AllocEdge();
		pNew->mbLine=pEdge->mbLine;
		pNew->mEndPt=pt;
		pNew->mControlPt=pEdge->mControlPt;
		pt=pEdge->mEndPt;
		pEdge=pEdge->pNext;
	}
	path->SetData(pt.x,pt.y,mFillLeft,mFillRight,this->mLineStyle,this->mbNewShape);
	path->mbNeedFree = 1;
	return path;
}
void BwPath::ReleaseEdge()
{
	//vector<BwEdge*>::iterator it;
	//for( it = mVecEdges.begin(); it != mVecEdges.end(); it++ )
	//{
	//	delete *it;
	//}
	//mVecEdges.clear();
	ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
	BwEdge*pEdge=pEdges;
	while(pEdge)
	{
		BwEdge*pNext=pEdge->pNext;
		pAlloc->FreeEdge(pEdge);
		pEdge=pNext;
	}
	pEdges=0;
	pEndEdge=0;
	mnPoints=0;
	return;
}

void BwShapeRecord::Init()
{
	//mClipPath;
	//pRasterData = XNULL;
	//m_bSample = false;
	memset(&m_imgData,0,sizeof(m_imgData));
	m_nSampleType = 0;
	m_bODD = false;
	fMeter = 80.0;
	mpPaths = XNULL;
	mpLastPath = XNULL;
	mnFills = 0;
	mnLines = 0;
	pFirst = 0;
	pLast = 0;
	m_pImage = XNULL;
	this->mVecFillStyles = XNULL;
	this->mVecLineStyles = XNULL;
}

/*BwShapeRecord::~BwShapeRecord()
{
	ReleaseAll();
}*/

void BwShapeRecord::ReleaseTexture()
{
	int i;
	for(i=0;i<this->mnFills;i++)
	{
		BwFillStyle*fs=mVecFillStyles[i];
		if(!fs) continue;
		if(fs->colorType!=glColorBitmap) continue;
		fs->bm.bitmap->PIFree();
	}
	for(i=0;i<mnLines;i++)
	{
		BwLineStyle*ls=mVecLineStyles[i];
		if(!ls) continue;
		if(!ls->color) continue;
		if(ls->color->colorType!=glColorBitmap) continue;
		ls->color->bm.bitmap->PIFree();
	}
}

void BwShapeRecord::AddFillSyle( int index,BwFillStyle *fs )
{
	//mVecFillStyles.Add(fs);
	//mVecFillStyles.push_back( fs );
	mVecFillStyles[index] = fs;
}

void BwShapeRecord::AddLineStyle( int index,BwLineStyle*ls )
{
	//mVecLineStyles.Add(ls);
	//mVecLineStyles.push_back( ls );
	mVecLineStyles[index] = ls;
}

void BwShapeRecord::AddPath( BwPath*path )
{
	//mVecPaths.Add(path);
	//mVecPaths.push_back( path );
	if(!path->mLineStyle&&!path->mFillLeft&&!path->mFillRight)
	{
		path->ReleaseEdge();
		ShapeAlloc::GetInst()->FreePath(path);
		return;
	}
	if(mpPaths == XNULL)
	{
		path->pNext = XNULL;
		mpPaths = path;
		mpLastPath = path;
	}
	else
	{
		mpLastPath->pNext = path;
		mpLastPath = path;
	}
}

static int GetXPos(XPOINT*p1,XPOINT*p2,XPOINT*cp,int y,bool isLine)
{
	if(isLine)
	{
		if(p1->y==p2->y)
			return p1->x<p2->x?p1->x:p2->x;
		return p1->x - (p1->y - y ) * (p1->x - p2->x ) / (p1->y - p2->y );
	}
	else
	{
		float t=(float)(p1->y-y)/(p1->y-p2->y);
		//XPOINT mp1,mp2;
		int x1 = p1->x-(p1->x-cp->x)*t;
		//mp1.y = p1->y-(p1->y-cp->y)*t;
		int x2 = cp->x-(cp->x-p2->x)*t;
		return x1-(x1-x2)*t;
		//return p1->x - (p1->y - y ) * (p1->x - p2->x ) / (p1->y - p2->y );
	}
}

/*static int GetYPos(XPOINT*p1,XPOINT*p2,XPOINT*cp,int x,bool isLine)
{
	if(isLine)
	{
		if(p1->x==p2->x)
			return p1->y<p2->y?p1->y:p2->y;
		return p1->y - (p1->x - x ) * (p1->y - p2->y ) / (p1->x - p2->x );
	}
	else
	{
		float t=(float)(p1->x-x)/(p1->x-p2->x);
		//XPOINT mp1,mp2;
		int y1 = p1->y-(p1->y-cp->y)*t;
		//mp1.y = p1->y-(p1->y-cp->y)*t;
		int y2 = cp->y-(cp->y-p2->y)*t;
		return y1-(y1-y2)*t;
		//return p1->x - (p1->y - y ) * (p1->x - p2->x ) / (p1->y - p2->y );
	}
}*/

static bool IsNearLine(XPOINT*p1,XPOINT*p2,int x,int y,int w)
{
	int minx,maxx;
	int miny,maxy;
	if(p1->x<p2->x)
	{
		minx=p1->x;
		maxx=p2->x;
	}
	else
	{
		minx=p2->x;
		maxx=p1->x;
	}
	if(p1->y<p2->y)
	{
		miny=p1->y;
		maxy=p2->y;
	}
	else
	{
		miny=p2->y;
		maxy=p1->y;
	}
	if(x>=minx-w&&x<=maxx+w&&
	   y>=miny-w&&y<=maxy+w) return true;
	return false;
}

static bool IsOnLine(XPOINT*p1,XPOINT*p2,int x,int y,int lw)
{
	if(p2->y==p1->y) //水平线
	{
		int dx1=p1->x-x;
		int dx2=p2->x-x;
		if(dx1<0) dx1=-dx1;
		if(dx2<0) dx2=-dx2;
		int dx=dx1<dx2?dx1:dx2;
		if(dx<=lw) 
			return true;
	}
	else if(p2->x==p1->x)
	{
		int dy1=p1->y-y;
		int dy2=p2->y-y;
		if(dy1<0) dy1=-dy1;
		if(dy2<0) dy2=-dy2;
		int dy=dy1<dy2?dy1:dy2;
		if(dy<=lw) 
			return true;
	}
	else
	{
		float a=p2->y-p1->y;
		float b=p1->x-p2->x;
		float c=(float)p2->x*p1->y-(float)p2->y*p1->x;
		float d=a*x+b*y+c;
		if(d<0) d=-d;
		if(d<=lw*lw*4) 
			return true;
	}
	return false;
}

static bool IsOnCurve(XPOINT*p1,XPOINT*p2,XPOINT*cp,int x,int y,int lw,int sl)
{
	if(IsNearLine(p1,cp,x,y,lw))
	{
		if(sl<lw*lw)
		{
			return IsOnLine(p1,cp,x,y,lw);
		}
	}
	else if(IsNearLine(cp,p2,x,y,lw))
	{
		if(sl<lw*lw)
		{
			return IsOnLine(cp,p2,x,y,lw);
		}
	}
	else return false;
	XPOINT mp1,mp2,mp;
	mp1.x = (p1->x+cp->x)/2;
	mp1.y = (p1->y+cp->y)/2;
	mp2.x = (cp->x+p2->x)/2;
	mp2.y = (cp->y+p2->y)/2;
	mp.x = (mp1.x+mp2.x)/2;
	mp.y = (mp1.y+mp2.y)/2;
	int dx=mp.x-cp->x;
	int dy=mp.y-cp->y;
	sl = dx*dx+dy*dy;
	if(IsOnCurve(p1,&mp1,&mp,x,y,lw,sl)) return true;
	if(IsOnCurve(&mp,&mp2,p2,x,y,lw,sl)) return true;
	return false;
}

bool BwShapeRecord::IsInPaths(int x, int y )
{
	BwPath *pTmpPath = mpPaths;
	int count = 0;
	int fillcou = 0,nFillID=0;
	int nFills[256];
	memset(nFills,0,sizeof(nFills));
	while( pTmpPath )
	{
		fillcou = 0;
		if ( pTmpPath->mFillLeft > 0 )
		{
			/*GLColor*pColor=this->mVecFillStyles[pTmpPath->mFillLeft];
			switch(pColor->colorType)
			{
			case glColorSolid:
				 if(pColor->rgb.alpha)
					fillcou--;
				 break;
			default:
				 fillcou--;
				 break;
			}*/
			nFillID = pTmpPath->mFillLeft&0xff;
			fillcou--;
		}
		if ( pTmpPath->mFillRight > 0 )
		{
			//GLColor*pColor=this->mVecFillStyles[pTmpPath->mFillRight];
			//switch(pColor->colorType)
			//{
			//case glColorSolid:
			//	 if(pColor->rgb.alpha)
			//		fillcou++;
			//	 break;
			//default:
			//	 fillcou++;
			//	 break;
			//}
			nFillID = pTmpPath->mFillRight&0xff;
			fillcou++;
		}
		
		if(fillcou==0&&!pTmpPath->LineStyle())
		{
			pTmpPath = pTmpPath->pNext;
			continue;
		}
		//BwEdge *pTmpEdge = pTemp->pEdges;
		/*if ( pTmpPath->pEndEdge->mEndPt.x != pTmpPath->mStartPt.x || pTmpPath->pEndEdge->mEndPt.y != pTmpPath->mStartPt.y )
		{
			pTmpPath = pTmpPath->pNext;
			continue;
		}*/

		BwEdge * pTmpEdge = pTmpPath->pEdges;
		XPOINT *p1 = &pTmpPath->mStartPt;
		XPOINT *p2 = &pTmpEdge->mEndPt;
		//XPOINT *cp = &p2;
		XPOINT *posTmp = p2;
		int lw = 0;
		if(pTmpPath->LineStyle())
		{
			BwLineStyle*ps=this->mVecLineStyles[pTmpPath->mLineStyle];
			if(ps)
			{
				lw = ps->mWidth;
				if(lw<20) lw=20;
			}
		}
		while( pTmpEdge )
		{
			p2 = &pTmpEdge->mEndPt;
			if(lw)
			{
				if(pTmpEdge->IsLine())
				{
					if(IsNearLine(p1,p2,x,y,lw))
					{
						if(IsOnLine(p1,p2,x,y,lw))
							return true;
					}
				}
				else if(IsOnCurve(p1,p2,&pTmpEdge->mControlPt,x,y,lw,lw*100))//if(IsNearLine(p1,&pTmpEdge->mControlPt,x,y,lw)||
					 //   IsNearLine(&pTmpEdge->mControlPt,p2,x,y,lw))
				{
					return true;
					/*float t;
					if(p1->y!=p2->y)
					{
						t=(float)(y-p1->y)/(p2->y-p1->y);
					}
					else if(p1->x!=p2->x)
					{
						t=(float)(x-p1->x)/(p2->x-p1->x);
					}
					else return true;
					XPOINT*cp = &pTmpEdge->mControlPt; 
					XPOINT pp1,pp2;
					pp1.x = p1->x+(cp->x-p1->x)*t;
					pp1.y = p1->y+(cp->y-p1->y)*t;
					pp2.x = cp->x+(p2->x-cp->x)*t;
					pp2.y = cp->y+(p2->y-cp->y)*t;
					if(IsNearLine(&pp1,&pp2,x,y,lw))
					{
						float a=pp2.y-pp1.y;
						float b=pp1.x-pp2.x;
						float c=(float)pp2.x*pp1.y-(float)pp2.y*pp1.x;
						float d=a*x+b*y+c;
						if(d<0) d=-d;
						if(d<=lw*lw) 
							return true;
					}*/
				}
			}
			/*if(lw)
			{
				if(p2->y==p1->y)
				{
					if(pTmpEdge->IsLine())
				}
				else
				{
					int xpos=GetXPos(p1,p2,pTmpEdge->mControlPt,y,pTmpEdge->IsLine());
					if(x>xpos-lw&&x<lw+lw)
						return true;
				}
			}*/
			if ( (p1->y < y && p2->y < y) || p1->y > y && p2->y > y ) // 在同侧
			{
				
				p1 = p2;
				if ( pTmpEdge != NULL )
				{
					pTmpEdge = pTmpEdge->pNext;
				}
				continue;
			}
			
			if ( p2->y == y )  //平行
			{
				//if(x>=p2->x-lw&&x<=p2->x+lw)
				//	return true;
				if ( p1->y != y )
				{
					//int posx=GetXPos(p1,p2,&pTmpEdge->mControlPt,x,pTmpEdge->IsLine());
					//if(x>=posx-lw&&x<=posx+lw)
					//	return true;
					pTmpEdge = pTmpEdge->pNext;
					if ( pTmpEdge == NULL )
					{
						continue;
					}
					posTmp = p2;
					p2 = &pTmpEdge->mEndPt;
					if ( posTmp->x < x )
					{
						if ( (y - p1->y > 0 && p2->y - y > 0) || (y - p1->y < 0 && p2->y - y < 0) )
						{
							if(m_bODD)
							{
								nFills[nFillID]++;//count++;
							}
							else
							{
								if(p2->y>p1->y)
									count += fillcou;
								else
									count -= fillcou;
							}
						}
					}
					p1 = p2;
					pTmpEdge = pTmpEdge->pNext;
					continue;
				}
				/*else if(fw)
				{
					int posy=GetYPos(p1,p2,&pTmpEdge->mControlPt,x,pTmpEdge->IsLine());
					if(y>=posy-lw&&y<=posy+lw)
						return true;
				}*/
			}

			float xpos;
			//if(p1->y==p2->y)
			
			//else
			if(p1->y!=p2->y)
			{
				//if(pTmpEdge->IsLine())
				{
					xpos= GetXPos(p1,p2,&pTmpEdge->mControlPt,y,pTmpEdge->IsLine());//p1->x - (p1->y - y ) * (p1->x - p2->x ) / (p1->y - p2->y );
					/*if(lw)
					{
						if(x>=xpos-lw&&x<=xpos+lw)
							return true;
					}*/
					if ( xpos < x )
					{
						if(m_bODD)
						{
							//count++;
							nFills[nFillID]++;
						}
						else
						{
							if(p2->y>p1->y)
								count += fillcou;
							else
								count -= fillcou;
						}
					}
				}
			}
			
			pTmpEdge = pTmpEdge->pNext;
			p1 = p2;
		}
		//if(m_bODD&&(count&1)) return true;
		pTmpPath = pTmpPath->pNext;
	}
	if(m_bODD)
	{
		for(int i=0;i<256;i++)
			if(nFills[i]&1) return true;
		return false;
	}
	//return (count&1)!=0;
	return count!=0;// %2 == 1;
}

void BwShapeRecord::ReleaseFillStyle()
{
	/*vector<BwFillStyle *>::iterator it;
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	for( it = mVecFillStyles.begin(); it != mVecFillStyles.end(); it++ )
	{
		GLColor* pColor = *it;
		if(pColor)
		{
			pColor->FreeCache();
			pAlloc->FreeColor(pColor);
		}
		//delete *it;

	}
	mVecFillStyles.clear();
	mVecFillStyles.resize( 0 );*/
	if(!mVecFillStyles) return;
	int i;
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	for(i=0;i<mnFills;i++)
	{
		BwFillStyle* pFill = mVecFillStyles[i];
		if(pFill)
		{
			pFill->FreeCache();
			pAlloc->FreeColor(pFill);
		}
	}
	XXVar::_FreeData(mVecFillStyles);
	mVecFillStyles = NULL;
	mnFills = 0;
	//mVecFillStyles.RemoveAll();
}

void BwShapeRecord::ReleaseLineStyle()
{
	//vector< BwLineStyle *>::iterator it;
	if(!mVecLineStyles) return;
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	int i;
	for(i=0;i<mnLines;i++)
	{
		BwLineStyle*pLine = mVecLineStyles[i];
		if(!pLine)
			continue;
		if(pLine->color)
		{
			pLine->color->FreeCache();
			pAlloc->FreeColor(pLine->color);
			
		}
		pAlloc->FreeLineStyle(pLine);
	}
	XXVar::_FreeData(mVecLineStyles);
	mVecLineStyles = NULL;
	mnLines = 0;
	/*for( it = mVecLineStyles.begin(); it != mVecLineStyles.end(); it++ )
	{
		BwLineStyle*pStyle=*it;
		if(pStyle!=XNULL)
		{
			if(pStyle->color)
			{
				pStyle->color->FreeCache();
				pAlloc->FreeColor(pStyle->color);
			}
			pAlloc->FreeLineStyle(pStyle);
		}
	}
	mVecLineStyles.clear();
	mVecLineStyles.resize( 0 );*/
	//mVecLineStyles.RemoveAll();
	return;
}

void BwShapeRecord::ReleasePaths()
{
	//vector<BwPath*>::iterator it;
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	BwPath*path = mpPaths;
	while(path)
	{
		BwPath*pNext = path->pNext;
		path->ReleaseEdge();
		pAlloc->FreePath(path);
		path = path->pNext;
	}
	mpPaths = XNULL;
	mpLastPath = XNULL;
	/*for( it = mVecPaths.begin(); it != mVecPaths.end(); it++ )
	{
		pAlloc->FreePath(*it);
		//delete *it;
	}
	mVecPaths.clear();
	mVecPaths.resize( 0 );*/
	//mVecPaths.RemoveAll();
	return;
}

void BwShapeRecord::ReleaseAll(bool bImage)
{
	//OutputDebugStringA("----->Release All ShapeRecord");
	ReleaseFillStyle();
	ReleaseLineStyle();
	ReleasePaths();
	ReleaseData();
	if(bImage)
		ReleaseImage();
	else
		ClearImage();
}

void BwShapeRecord::ReleaseImage()
{
	if(!m_pImage) return;
	if(m_imgData)
		m_imgData.RemoveAll();
	if(m_pImage->id)
		RasterGL::sharedRasterGL()->DeleteTexture(m_pImage->id);
	ShapeAlloc::GetInst()->FreeImage(m_pImage);
	m_pImage = NULL;
}

void BwShapeRecord::ReleaseTrangles(GLTrangle*pTrangle)
{
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	//GLTrangle*pTrangle=pTrangles->pTrangles;
	while(pTrangle)
	{
		GLTrangle*pNextT=pTrangle->pNext;
		pAlloc->FreeTrangle(pTrangle);
		pTrangle = pNextT;
	}
	
}
void BwShapeRecord::ReleaseLines(GLPoint*pPoint)
{
	//void*pNext = pLines->pNext;
	//GLT_TYPE type=pLines->nextType;
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	//GLPoint*pPoint=pLines->pPoints;
	while(pPoint)
	{
		GLPoint*pNextP=pPoint->pNext;		
		pAlloc->FreePoint(pPoint);
		pPoint=pNextP;
	}
}


void BwShapeRecord::ReleaseFontData(GLTranglesOrLines**pData,int nCount)
{
	if(!pData) return;
	for(int i=0;i<nCount;i++)
	{
		GLTranglesOrLines*pFirst=pData[i];
		if(pFirst)
			ReleaseShapeList(pFirst);
	}
	delete (XU8*)pData;
}



void BwShapeRecord::ReleaseShapeList(GLTranglesOrLines*pFirst)//,bool releaseFill)
{
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	RasterGL* raster = RasterGL::sharedRasterGL();
	//if(pData)
	//{
		GLTranglesOrLines*pTL = pFirst;
		while(pTL)
		{
			GLTranglesOrLines*pNext=pTL->pNext;
			switch(pTL->type)
			{
			//case GLT_FONT:
			//case GLT_FONT16:
			//	 if(pTL->fontID)
			//		 raster->DeleteTexture(pTL->fontID);
			//	 break;
			case GLT_FONT:
				 if(pTL->pChar)
				 {
					 //if(pTL->pChar->g==20108)
					//	 int v=0;
					 if(pTL->pChar->id)
						 raster->DeleteTexture(pTL->pChar->id);
					 pAlloc->FreeChar(pTL->pChar);
				 }
				 break;
			case GLT_SIMPLERECT:
			case GLT_SIMPLEFRAME:
				 if(pTL->rect)
					 pAlloc->FreeSimpleRect(pTL->rect);
				 break;
			case GLT_TRANGLES:
				 //XXVar::_FreeData(pTL->trangles.pVertex);
				 if(pTL->trangles.nBufID)
					 raster->DeleteBuffer(pTL->trangles.nBufID);
				 /*if(releaseFill&&pTL->trangles.pFillStyle)
				 {
					 pTL->trangles.pFillStyle->FreeCache();
					 pAlloc->FreeColor(pTL->trangles.pFillStyle);
				 }*/
				 //ReleaseTrangles(pTL->trangles.pTrangles);
				 break;
			case GLT_LINES:
				 //XXVar::_FreeData(pTL->lines.pPoints);
				if(pTL->lines.nBufID)
					raster->DeleteBuffer(pTL->lines.nBufID);
				 //ReleaseLines(pTL->lines.pPoints);
				 break;
			case GLT_RECTANGLE:
				 if(pTL->rectangle)
					 pAlloc->FreeRectangle(pTL->rectangle);
				 break;
			}
			pAlloc->FreeTranglesOrLines(pTL);
			pTL = pNext;
		}
		//pAlloc->FreeRasterData(pData);
	//}
	//pRasterData=NULL;
}

void BwShapeRecord::ReleaseData()
{
	//GLRasterData* pData=pRasterData;
	ReleaseShapeList(this->pFirst);
	pFirst=NULL;
	pLast=NULL;
}

void BwShapeRecord::ClearImage()
{
	if(!m_pImage||!m_pImage->id) return;
	if(!m_imgData.GetSize())
		ReleaseImage();
	else
	{
		RasterGL::sharedRasterGL()->DeleteTexture(m_pImage->id);
		m_pImage->id = 0;
	}
}

void BwShapeRecord::SetImage(XDib&dib,MATRIX&m,bool bSave)
{
	ReleaseImage();
	//XPngEncode en;
	//m_imgData.RemoveAll();
	//en.Encode(dib,m_imgData);
	m_pImage = ShapeAlloc::GetInst()->AllocImage();
	m_pImage->w = dib.Width();
	m_pImage->h = dib.Height();
	//m_pImage->useCount = 0;
	//m_pImage->nTimer = 0;
	m_pImage->mat = m;
	if(!bSave)
	{
		m_pImage->id = RasterGL::sharedRasterGL()->texInitWithData(dib.GetBuffer(),Texture2DPixelFormat_RGBA8888,m_pImage->w,m_pImage->h);
		m_pImage->id->type = 3;
	}
	else
		m_pImage->id = 0;
}

void BwShapeRecord::SetMeter(MATRIX&mat)
	{
		//return;
		if(pFirst) return;
		float s=(float)(mat.a>mat.d?mat.a:mat.d)/fixed_1;
		s=s*s;
		if(s==0) return;
		s = 80.0/s;
		if(s<0.1) s=0.1f;
		else if(s>100) s=100.0f;
		if(s>=fMeter) return;
		fMeter = s;
		//this->ReleaseAll();
	}

/*void BwShapeRecord::Draw(RasterGL*pRaster)
{
	//return;
	if(!this->pRasterData)
	{
		pRasterData = ShapeAlloc::GetInst()->AllocRasterData();
		pRasterData->needMatrix = 0;
		pRasterData->needTransform = 0;
		pRasterData->pFirst = 0;
		pRasterData->pLast = 0;
		TessPloy::Tess(this);
	}
	if(pRasterData&&pRasterData->pFirst)
		pRaster->DrawShape(pRasterData);
}*/

/*BwPath*BwShapeRecord::GetPath(int index )
{
	return mVecPaths[index];
}*/
