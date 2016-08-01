#include "StdAfxGGBase.h"
#include "GGBase.h"
#include "GGType.h"
#include "XGraphics.h"
#include "XWindow.h"
#include "XString.h"
#include "XResource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction  const char*
//////////////////////////////////////////////////////////////////////

//static XPCTSTR _strIcons[XMAX_ICONS]={("LOADING"),("FAILED"),("CONNECT"),
//	("HOME"),("ANI"),("MAINICON"),"GREENBLOCK","BLUEBLOCK","GRAYBLOCK", "BUTTON"};

static XPCTSTR _strIcons[XMAX_ICONS]={"LOADING", "FAILED", "BLUEBLOCK","GRAYBLOCK", "BUTTON","PAGELOADING"};
#ifdef _ANALY

static _XCALLENTRY* m_pRootCall=XNULL;
static _XCALLENTRY* m_pCurrentCall=XNULL;
static XS64			m_nStartTime=_ANGetNowTime();
static XS64			m_nTotalTime=0;
static XU32			m_nStartTick=XTime::GetTickCount();
static XMutex		m_ANData;
static XU32Array    m_ANItems;
XBOOL			    m_bStart=XTRUE;
_XCALLGRAPH*		m_pGraph=XNULL;
_XCALLGRAPH*		m_pCurrentGraph=XNULL;
#define MAXDATAS	65536

XBOOL _ANGraphEnter(XPCTSTR strFunction,int nCount)
{
	if(m_pCurrentGraph!=XNULL) return XFALSE;
	m_pCurrentGraph=m_pGraph;
	while(m_pCurrentGraph)
	{
		if(m_pCurrentGraph->strFunction==strFunction) break;
		m_pCurrentGraph=m_pCurrentGraph->pNext;
	}
	if(m_pCurrentGraph==XNULL)
	{
		m_pCurrentGraph=new _XCALLGRAPH;
		if(m_pCurrentGraph==XNULL) return XFALSE;
		m_pCurrentGraph->pDatas=new XU64[MAXDATAS];
		if(m_pCurrentGraph->pDatas==XNULL)
		{
			m_pCurrentGraph=XNULL;
			return XFALSE;
		}
		m_pCurrentGraph->nDataLength=MAXDATAS;
		m_pCurrentGraph->strFunction=strFunction;
		m_pCurrentGraph->pNext=m_pGraph;
		m_pGraph=m_pCurrentGraph;
		m_pCurrentGraph->nCount=0;
		m_pCurrentGraph->nCurData=0;
		m_pCurrentGraph->nMaxTime=0;
		m_pCurrentGraph->nMaxCount=nCount;
		m_pCurrentGraph->nPosition=0;
		//m_pCurrentGraph
	}

	XS32 high;
	XU32 low;
	XTime::GetFrequency(high,low);
	m_pCurrentGraph->nEnterTime=(((XS64)high)<<32)|low;

	return XTRUE;
}
void  _ANGraphLeave()
{
	if(m_pCurrentGraph==XNULL) return;
	if(m_pCurrentGraph->nPosition>=m_pCurrentGraph->nDataLength)
	{
		int nl=m_pCurrentGraph->nDataLength+MAXDATAS;
		XU64*pNew=new XU64[nl];
		if(pNew==XNULL) return;
		memcpy(pNew,m_pCurrentGraph->pDatas,m_pCurrentGraph->nDataLength*sizeof(XU64));
		delete m_pCurrentGraph->pDatas;
		m_pCurrentGraph->nDataLength=nl;
		m_pCurrentGraph->pDatas=pNew;
	}
	XS32 high;
	XU32 low;
	XTime::GetFrequency(high,low);
	XU32 nTime=(((XS64)high)<<32)|low;
	nTime-=m_pCurrentGraph->nEnterTime;
	if(m_pCurrentGraph->nCurData<nTime)
		m_pCurrentGraph->nCurData=nTime;
	m_pCurrentGraph->nCount++;
	if(m_pCurrentGraph->nCount>=m_pCurrentGraph->nMaxCount)
	{

		//m_pCurrentGraph->nCurData/=m_pCurrentGraph->nCount;
		m_pCurrentGraph->nCount=0;
		m_pCurrentGraph->pDatas[m_pCurrentGraph->nPosition]=m_pCurrentGraph->nCurData;
		if(m_pCurrentGraph->nMaxTime<m_pCurrentGraph->nCurData)
			m_pCurrentGraph->nMaxTime=m_pCurrentGraph->nCurData;
		m_pCurrentGraph->nCurData=0;
		m_pCurrentGraph->nPosition++;
	}
	m_pCurrentGraph=XNULL;
}
_XCALLGRAPH* _ANGetGraph()
{
	return m_pGraph;
}

void _ANDisable(XU32 id)
{
	XSortU32 sort(&m_ANItems);
	int mi;
	if(sort.Index((void*)id,mi)>=0) return;
	m_ANItems.InsertAt(mi,id);
}
void _ANEnable(XU32 id)
{
	XSortU32 sort(&m_ANItems);
	int mi=sort.Index((void*)id);
	if(mi>=0)
		m_ANItems.RemoveAt(mi);
}
XBOOL _ANIsDisabled(XU32 id)
{
	XSortU32 sort(&m_ANItems);
	if(sort.Index((void*)id)>=0) return XTRUE;
	return XFALSE;
}

void _ANFreeData(_XCALLENTRY*p)
{
	if(!p) return;
	while(p)
	{
		if(p->pChild)
			_ANFreeData(p->pChild);
		_XCALLENTRY*pn=p->pNext;

		delete p;
		p=pn;
	}
}

void _ANStop()
{
	m_bStart=XFALSE;
}
void _ANStart()
{
	m_bStart=XTRUE;
}
void _ANClear()
{
	XLock lock(&m_ANData);
	if(!lock.Lock(1000)) return;
	m_nTotalTime=0;
	_ANFreeData(m_pRootCall);
	m_pRootCall=XNULL;
	m_pCurrentCall=XNULL;
	_XCALLGRAPH*p=m_pGraph;
	while(p)
	{
		_XCALLGRAPH*pNext=p->pNext;
		if(p->pDatas)
			delete p->pDatas;
		delete p;
		p=pNext;
	}
	m_pCurrentGraph=XNULL;
	m_pGraph=XNULL;
	lock.UnLock();
}
XBOOL _ANIsStarted()
{
	return m_bStart;
}

XBOOL _ANEntryFunction(XPCTSTR strFunction)
{
	//if(m_pCurrentCall&&m_pCurrentCall->strFunction==strFunction) return XFALSE;

//	if(strcmp(strFunction,"DoEvent")==0&&
//	   m_pCurrentCall&&m_pCurrentCall->strFunction=="Draw")
//	   int v=0;
	if(!m_bStart) return XFALSE;
	XLock lock(&m_ANData);
	if(!lock.Lock(1000)) return XFALSE;
	_XCALLENTRY*pCall=XNULL;
	_XCALLENTRY**pRoot=&m_pRootCall;
	if(m_pCurrentCall)
		pRoot=&m_pCurrentCall->pChild;
	if(*pRoot)
	{
		_XCALLENTRY*p=*pRoot;
		//if(p->strFunction==strFunction) return XFALSE;
		while(p)
		{
			if(p->strFunction==strFunction)
			{
				pCall=p;
				break;
			}
			p=p->pNext;
		}
	}

	//_XCALLENTRY*pCur=m_pCurrentCall;

	if(pCall==XNULL)
	{
		pCall=new _XCALLENTRY;
		pCall->strFunction=strFunction;

		pCall->nTotalTime=0;
		pCall->pNext=*pRoot;
		pCall->pChild=XNULL;
		pCall->nPecent=0;
		pCall->bExtern=XFALSE;
		pCall->nMaxTime=0;
		pCall->nTimes=0;
		*pRoot=pCall;
		pCall->pParent=m_pCurrentCall;

	}

	XS32 high;
	XU32 low;
	XTime::GetFrequency(high,low);
	pCall->nInTime=(((XS64)high)<<32)|low;
	pCall->nTimes++;
	m_pCurrentCall=pCall;
	lock.UnLock();
	return XTRUE;
}
void _ANLeaveFunction()
{
	if(!m_bStart) return;
	if(!m_pCurrentCall) return;
	XLock lock(&m_ANData);
	if(!lock.Lock(1000)) return;
	_XCALLENTRY*pCall=m_pCurrentCall;
	XS32 high;
	XU32 low;
	XTime::GetFrequency(high,low);
	pCall->nOutTime=(((XS64)high)<<32)|low;
//	if(pCall->nInTime>pCall->nOutTime)
//		int v=0;
	XS64 time=(pCall->nOutTime-pCall->nInTime);
	pCall->nTotalTime+=time;
	m_pCurrentCall=m_pCurrentCall->pParent;
	if(pCall->nMaxTime<time)
		pCall->nMaxTime=time;
	if(m_pCurrentCall==XNULL)
		m_nTotalTime+=time;
	lock.UnLock();

}
_XCALLENTRY* _ANGetRoot()
{
	return m_pRootCall;
}

XS64 _ANGetStartTime()
{
	return m_nStartTime;
}
XU32 _ANGetStartTick(){return m_nStartTick;}
XS64 _ANGetTotalTime(){return m_nTotalTime;}

XS64 _ANGetNowTime()
{
	XS32 high;
	XU32 low;
	XTime::GetFrequency(high,low);
	return (((XS64)high)<<32)|low;
}

void _ANFreeData(_XCALLENTRY*p,XFile&file,XString8&strHead)
{
	if(!p) return;
	while(p)
	{
		XString8 strTmp(strHead);
		strTmp+=p->strFunction;
		//int k=strTmp.GetLength()+1;
		strTmp+="                 ";
		int k=strTmp.GetLength()-1;
		XS64 v=p->nTotalTime;
		while(1)
		{
			strTmp[k]=(char)((v%10)+'0');
			k--;
			v=v/10;
			if(!v) break;
		}
		strTmp+="         \r\n";
		k=strTmp.GetLength()-3;
		v=p->nTimes;
		while(1)
		{
			strTmp[k]=(char)((v%10)+'0');
			k--;
			v=v/10;
			if(!v) break;
		}
		file.Write(strTmp,strTmp.GetLength());
		if(p->pChild)
		{
			XString8 str(strHead);
			str+="+";
			_ANFreeData(p->pChild,file,str);
		}
		_XCALLENTRY*pn=p->pNext;

		delete p;
		p=pn;
	}
}

void _ANFreeData()
{
	XFile file;
	XString8 strHead;
	file.Open("\\_analyzer.txt",XFile::XCREATE|XFile::XWRITE);
    _ANFreeData(m_pRootCall,file,strHead);
	file.Close();
	_XCALLGRAPH*p=m_pGraph;
	while(p)
	{
		_XCALLGRAPH*pNext=p->pNext;
		if(p->pDatas)
			delete p->pDatas;
		delete p;
		p=pNext;
	}
	m_pCurrentGraph=XNULL;
	m_pGraph=XNULL;
}

#endif

XBOOL STDCALL XInitGGBase()
{
    //	XWindow::m_nTextSize = g_deviceData.m_defaultFontSize;
    //
    //	XGraphics g(1, 1);
    //	XSize sz;

	XGraphics g(1,1);
	XSize sz;

	g.SetFontSize(XWindow::m_nTextSize);
	g.MeasureString(XResource::LoadWString(XIDS_HELLO), sz, 0);
	XWindow::m_nTextHeight = (XU8)sz.cy;
	for(int i=0;i<XMAX_ICONS;i++)
	{
		if(_strIcons[i]==XNULL) break;
		XWindow::GetIcon(i)->CreateFromResource(_strIcons[i]);
	}

	return XTRUE;
}

void STDCALL XReleaseGGBase()
{
	for(int i=0;i<XMAX_ICONS;i++)
		XWindow::GetIcon(i)->Release();
}
