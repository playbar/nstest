#include "stdafx.h"
#include <math.h>
#include "AccEnu.h"
#include "BitmapDC.h"
#include "XAcc.h"
BEGIN_MESSAGE_MAP(CAccEnu, CButton)
	//{{AFX_MSG_MAP(CWaveBut)	
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAccEnu::CAccEnu(void)
{
	_enuW = 0;
	_enuH = 0;
	_accX = 0;
	_accY = 0;
	_accZ = 0;
	_gAccX=0;	_gAccY=1;	_gAccZ=0;
}

CAccEnu::~CAccEnu(void)
{
}


static int _cameraLength= 500;

static double _pts[4][8][3]=
		{
			{{-0.5,0.95,0.05},{-0.45,1,0.05},{0.45,1,0.05},{0.5,0.95,0.05},{0.5,-0.95,0.05},{0.45,-1,0.05},{-0.45,-1,0.05},{-0.5,-0.95,0.05}},
			{{-0.5,0.95,-0.05},{-0.45,1,-0.05},{0.45,1,-0.05},{0.5,0.95,-0.05},{0.5,-0.95,-0.05},{0.45,-1,-0.05},{-0.45,-1,-0.05},{-0.5,-0.95,-0.05}},
			{{-0.45,0.70,0.04},{-0.40,0.75,0.04},{0.40,0.75,0.04},{0.45,0.70,0.04},{0.45,-0.9,0.04},{0.40,-0.95,0.04},{-0.40,-0.95,0.04},{-0.45,-0.90,0.04}},
			{{-0.1,0.95,0.04},{0.1,0.95,0.04},{0.1,0.80,0.04},{-0.1,0.80,0.04},{-0.1,0.95,0.04},{0.1,0.95,0.04},{0.1,0.80,0.04},{-0.1,0.80,0.04}}
		};

static double _hitPts[4][4][3]=
		{
		  {{0,1,0},{0,-1,0},{-2,-2,-2},{-2,-2,-2}},//Rotate X
		  {{-0.5,0,0},{0.5,0,0},{-2,-2,-2},{-2,-2,-2}},//Rotate Y
		  {{-0.5,-1,0},{-0.5,1,0},{0.5,1,0},{0.5,-1,0}},//RotateZ
		  {{0,0,0},{-2,-2,-2},{-2,-2,-2},{-2,-2,-2}},//Move
		};
static double _matrix[4][4]=
	{{1,0,0,0},
	 {0,1,0,0},
	 {0,0,1,0},
	 {0,0,0,1}
	};

static void matrixClear(double m[4][4])
{
	int i,j;
	for(i=0;i<4;i++)
	for(j=0;j<4;j++)
		m[i][j]=0;
}

static void matrixCopy(double m[4][4],double r[4][4])
{
	//for(i=0;i<4;i++)
	//for(j=0;j<4;j++)
	//	r[i][j]=m[i][j];
	memcpy(r,m,4*4*sizeof(double));
}

static void matrixConcat(double m1[4][4],double m2[4][4],double r[4][4])
{
	int i,j,k;
	matrixClear(r);
	for(j=0;j<4;j++)
	{
		for(i=0;i<4;i++)
		{
			for(k=0;k<4;k++)
			{
				r[j][i]+=m1[j][k]*m2[k][i];
			}
		}
	}
}

static void rotateX(double _mat[4][4],double rot,double r[4][4])
{
	double cosa=cos(rot);
	double sina=sin(rot);
	double m[4][4]={{1,0,0,0},
					{0,cosa,sina,0},
					{0,-sina,cosa,0},
					{0,0,0,1}
					};
	matrixConcat(_mat,m,r);
}

static void rotateY(double _mat[4][4],double rot,double r[4][4])
{
	double cosa=cos(rot);
	double sina=sin(rot);
	double m[4][4]={{cosa,0,-sina,0},
					{0,1,0,0},
					{sina,0,cosa,0},
					{0,0,0,1}
					};
	matrixConcat(_mat,m,r);
}

static void rotateZ(double _mat[4][4],double rot,double r[4][4])
{
	double cosa=cos(rot);
	double sina=sin(rot);
	double m[4][4]={{cosa,sina,0,0},
					{-sina,cosa,0,0},
					{0,0,1,0},
					{0,0,0,1}
					};
	matrixConcat(_mat,m,r);
}

static void scale(double _mat[4][4],double sx,double sy,double sz,double r[4][4])
{
	double m[4][4]={{sx,0,0,0},
				    {0,sy,0,0},
					{0,0,sz,0},
					{0,0,0,1}};
	matrixConcat(_mat,m,r);
}

static void translate(double _mat[4][4],double tx,double ty,double tz,double r[4][4])
{
	double m[4][4]={{1,0,0,0},
				    {0,1,0,0},
					{0,0,1,0},
					{tx,ty,tz,1}};
	matrixConcat(_mat,m,r);
}


static void translatePoint(double _mat[4][4],double pt[3],double r[3])
{	
	r[0]=pt[0]*_mat[0][0]+pt[1]*_mat[1][0]+pt[2]*_mat[2][0]+_mat[3][0];
	r[1]=pt[0]*_mat[0][1]+pt[1]*_mat[1][1]+pt[2]*_mat[2][1]+_mat[3][1];
	r[2]=pt[0]*_mat[0][2]+pt[1]*_mat[1][2]+pt[2]*_mat[2][2]+_mat[3][2];

}

static void project(double pt[3],double project,int _enuW,int _enuH)
{
	double scale=(project+pt[2])/project;
	double x=pt[0]*scale+_enuW/2;
	double y=pt[1]*scale+_enuH/2;
	pt[0]=x;
	pt[1]=y;
}

void CAccEnu::DrawEnu(CDC*pDC)
{
		//var p0:Array=[];
		//var p1:Array=[];
		//var p2:Array=[];
		//var p3:Array=[];
		double p0[8][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
		double p1[8][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};;
		double p2[8][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
		double p3[8][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
		int i;
		for(i=0;i<8;i++)
		{
			translatePoint(_matrix,_pts[0][i],p0[i]);
			translatePoint(_matrix,_pts[1][i],p1[i]);
			translatePoint(_matrix,_pts[2][i],p2[i]);
			translatePoint(_matrix,_pts[3][i],p3[i]);
			//p0.push(translatePoint(_matrix,_pts[0][i]));
			//p1.push(translatePoint(_matrix,_pts[1][i]));
			//p2.push(translatePoint(_matrix,_pts[2][i]));
			//p3.push(translatePoint(_matrix,_pts[3][i]));
		}
		for(i=0;i<8;i++)
		{
			project(p0[i],_cameraLength,_enuW,_enuH);
			project(p1[i],_cameraLength,_enuW,_enuH);
			project(p2[i],_cameraLength,_enuW,_enuH);
			project(p3[i],_cameraLength,_enuW,_enuH);
		}
		//var g:Graphics=_show.graphics;
		//g.clear();
		//g.lineStyle(1,0);
		pDC->MoveTo(p0[0][0],p0[0][1]);
		for(i=1;i<8;i++)
			pDC->LineTo(p0[i][0],p0[i][1]);
		pDC->LineTo(p0[0][0],p0[0][1]);
		pDC->MoveTo(p1[0][0],p1[0][1]);
		for(i=1;i<8;i++)
			pDC->LineTo(p1[i][0],p1[i][1]);
		pDC->LineTo(p1[0][0],p1[0][1]);
		pDC->MoveTo(p2[0][0],p2[0][1]);
		for(i=1;i<8;i++)
			pDC->LineTo(p2[i][0],p2[i][1]);
		pDC->LineTo(p2[0][0],p2[0][1]);
		
		pDC->MoveTo(p3[0][0],p3[0][1]);
		for(i=1;i<4;i++)
			pDC->LineTo(p3[i][0],p3[i][1]);
		pDC->LineTo(p3[0][0],p3[0][1]);
		
		for(i=0;i<8;i++)
		{
			pDC->MoveTo(p0[i][0],p0[i][1]);
			pDC->LineTo(p1[i][0],p1[i][1]);
		}
}

static int nDownX;
static int nDownY;
static int nDownAccX;
static int nDownAccY;
static int nDownAccZ;
static double downMatrix[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
static double downRotate;
static int hitType=-1;
static int _hitWidth=20;
#define PI 3.1425926536

double CAccEnu::getRotateZ(double x,double y)
{
	x-=_enuW/2;
	y-=_enuH/2;
	double l=sqrt(x*x+y*y);
	if(l<=0) l=_enuW;
	//trace(y);
	double rot=acos(y/l);
	
	if(x>0)
		rot=PI-rot;
	else
		rot+=PI;
	//trace(rot);
	return rot;
}

double CAccEnu::getRotateX(double x,double y)
{
	x-=_enuW/2.0;
	y-=_enuH/2.0;
	double dl=_enuH*0.4;
	if(y>dl) y=dl;
	else if(y<-dl) y=-dl;
	double rot=acos(y/dl);
	//trace(rot);
	//if(x>0)
	//	rot=Math.PI-rot;
	//else
		//rot+=Math.PI;
	//trace(rot);
	return rot;
}

double CAccEnu::getRotateY(double x,double y)
{
	x-=_enuW/2;
	y-=_enuH/2;
	double dl=_enuH*0.2;
	if(x>dl) x=dl;
	else if(x<-dl) x=-dl;
	double rot=PI-acos(x/dl);

	return rot;
}

void CAccEnu::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int mx=point.x;
	int my=point.y;
	nDownX = point.x;
	nDownY = point.y;
	int i,j;
	//int iCount=4;
	hitType = -1;			
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			double pt[3]={0,0,0};
			if(_hitPts[i][j][0]<-10.0) continue;
			translatePoint(_matrix,_hitPts[i][j],pt);
			project(pt,_cameraLength,_enuW,_enuH);
			if(mx>=pt[0]-_hitWidth&&mx<=pt[0]+_hitWidth&&
			  my>=pt[1]-_hitWidth&&my<=pt[1]+_hitWidth)
			{
					hitType=i;
					break;
			}
		}
		if(hitType>=0)
		break;
	}
	if(hitType<0) return;
	matrixCopy(_matrix,downMatrix);	
	switch(hitType)
	{
		case 0://Rotate X
		     //BXStage.rootStage.addEventListener(MouseEvent.MOUSE_MOVE,onRotateX);
			 downRotate=getRotateX(nDownX,nDownY);
		     break;
		case 1://Rotate Y
		     //BXStage.rootStage.addEventListener(MouseEvent.MOUSE_MOVE,onRotateY);
			 downRotate=getRotateY(nDownX,nDownY);
		     break;
		case 2://Rotate Z
			 //BXStage.rootStage.addEventListener(MouseEvent.MOUSE_MOVE,onRotateZ);
			 downRotate=getRotateZ(nDownX,nDownY);
		     break;
		case 3://
			 //BXStage.rootStage.addEventListener(MouseEvent.MOUSE_MOVE,onMove);
		     break;
	}
	SetCapture();
	//		BXStage.rootStage.addEventListener(MouseEvent.MOUSE_UP,onUp);
}

void CAccEnu::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(hitType<0) return;
	int mx=point.x;
	int my=point.y;
	switch(hitType)
	{
	case 0://Rotate X
		{
		 double rot=getRotateX(mx,my)-downRotate;
		 rotateX(downMatrix,rot,_matrix);
		}break;
	case 1://Rotate Y
		{
		 double rot=getRotateY(mx,my)-downRotate;
		 rotateY(downMatrix,rot,_matrix);
		}break;
	case 2://Rotate Z
		{
		 double rot=getRotateZ(mx,my)-downRotate;
		 rotateZ(downMatrix,rot,_matrix);
		}break;
	case 3://Move
		{
		 	double tx=_matrix[3][0];
			double ty=_matrix[3][1];
			double tz=_matrix[3][2];
			translate(downMatrix,mx-nDownX,my-nDownY,0,_matrix);
			double mat[4][4]={{1/(_enuW*0.4),0,0,0},
				{0,1/(_enuW*0.4),0,0},
				{0,0,1/(_enuW*0.4),0},
				{0,0,0,1}};
			double dx=(_matrix[3][0]-tx)*4/_enuW;
			double dy=(_matrix[3][1]-ty)*4/_enuW;
			double dz=(_matrix[3][2]-tz)*4/_enuW;
			double matrix[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
			matrixConcat(mat,_matrix,matrix);
			matrix[3][0]=0;
			matrix[3][1]=0;
			matrix[3][2]=0;
			double pt[3]={1,0,0};
			double r[3]={0,0,0};
			translatePoint(matrix,pt,r);
			double pt1[3]={0,-1,0};
			double r1[3]={0,0,0};
			translatePoint(matrix,pt1,r1);
			double ddx=dx*r[0]+dy*r1[0];
			double ddy=dx*r[1]+dy*r1[1];
			double ddz=dx*r[2]+dy*r1[2];
			//trace(ddx+":"+ddy+":"+ddz);
			_accX+=ddx;
			_accY+=ddy;
			_accZ+=ddz;
		}break;						
	}

	double mat[4][4]={{1/(_enuW*0.4),0,0,0},
					{0,1/(_enuW*0.4),0,0},
					{0,0,1/(_enuW*0.4),0},
					{0,0,0,1}};
	double matrix[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	matrixConcat(mat,_matrix,matrix);
	matrix[3][0]=0;
	matrix[3][1]=0;
	matrix[3][2]=0;
	double pt[3]={0,1,0};
	double r[3]={0,0,0};
	translatePoint(matrix,pt,r);
	_gAccX=-r[0];
	_gAccY=r[1];
	_gAccZ=r[2];
	XAcc::SetAcc(_accX+_gAccX,_accY+_gAccY,_accZ+_gAccZ);
	Invalidate();
}

void CAccEnu::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	hitType = -1;
	Invalidate();
	ReleaseCapture();
}

BOOL CAccEnu::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return TRUE;
	
	return TRUE;
	//return CButton::OnEraseBkgnd(pDC);
}void CAccEnu::Init(){	_accX=0;	_accY=0;	_accZ=0;	_gAccX=0;	_gAccY=1;	_gAccZ=0;	CRect rect;	GetClientRect(rect);	_enuW = rect.Width();
	_enuH = rect.Height();
	double r[4][4];
	scale(_matrix,_enuW*0.8/2,_enuW*0.8/2,_enuW*0.8/2,r);
	matrixCopy(r,_matrix);	SetTimer(50,2,NULL);	XAcc::SetAcc(_accX+_gAccX,_accY+_gAccY,_accZ+_gAccZ);}void CAccEnu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	//XRect rect(0,0,m_pDib->Width(),m_pDib->Height());
	//m_pDib->Paint(lpDrawItemStruct->hDC,m_nOffX,m_nOffY,rect);
	CRect rect(lpDrawItemStruct->rcItem);
	if(_enuW<=0||_enuH<=0)
	{
		_enuW = rect.Width();
		_enuH = rect.Height();
		double r[4][4];
		scale(_matrix,_enuW*0.8/2,_enuW*0.8/2,_enuW*0.8/2,r);
		matrixCopy(r,_matrix);
	}
	CBitmapDC dc;
	dc.CreateBitmapDC(rect.Width(),rect.Height());
	CBrush brush((COLORREF)0xe0f0f0);
	CBrush black((COLORREF)0);
	dc.FillRect(rect,&brush);
	

	CPen pen(PS_SOLID,1,0xb0c0f0);
	CPen* old=dc.SelectObject(&pen);
	dc.MoveTo(0,_enuH/2);
	dc.LineTo(_enuW,_enuH/2);
	dc.MoveTo(_enuW/2,0);
	dc.LineTo(_enuW/2,_enuH);
	dc.SelectObject(old);
	
	dc.FrameRect(rect,&black);
	DrawEnu(&dc);

	dc.SetTextColor((COLORREF)0x9f9f7f);
	dc.SetBkMode(TRANSPARENT);//=true;
	if(hitType>=0)
	{
		static const PCTSTR _strType[]={_T("X 旋转"),_T("Y 旋转"),_T("Z 旋转"),_T("移动")};
		dc.TextOut(4,_enuH-20,_strType[hitType]);
	}
	CString strTmp;
	strTmp.Format(_T("Acc X:%1.5f"),_accX+_gAccX);
	dc.TextOut(4,4,strTmp);
	strTmp.Format(_T("Acc Y:%1.5f"),_accY+_gAccY);
	dc.TextOut(4,24,strTmp);
	strTmp.Format(_T("Acc Z:%1.5f"),_accZ+_gAccZ);
	dc.TextOut(4,44,strTmp);
	CDC*pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY);
}

static double _acc=0.05;

void CAccEnu::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(_accX==0&&_accY==0&&_accZ==0)
	{
	}
	else
	{
		if(_accX>_acc)
			_accX-=_acc;
		else if(_accX<-_acc)
			_accX+=_acc;
		else
			_accX=0;
		if(_accY>_acc)
			_accY-=_acc;
		else if(_accY<-_acc)
			_accY+=_acc;
		else
			_accY=0;
		if(_accZ>_acc)
			_accZ-=_acc;
		else if(_accZ<-_acc)
			_accZ+=_acc;
		else
			_accZ=0;
		Invalidate();
		XAcc::SetAcc(_accX+_gAccX,_accY+_gAccY,_accZ+_gAccZ);
	}
	CButton::OnTimer(nIDEvent);
}
