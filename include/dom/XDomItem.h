// XDomItem.h: interface for the XDomItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMITEM_H__935D134E_BD36_405F_AE70_3EBF06518F08__INCLUDED_)
#define AFX_XDOMITEM_H__935D134E_BD36_405F_AE70_3EBF06518F08__INCLUDED_

#include "XVar.h"
#include "XURL.h"
#include "XEnumData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define XBOXSIZE 14
#define XPOTSIZE 4
class XDomNode;
class XSelection;


enum {XDO_GET_PARENT=0,
      XDO_SET_PARENT,

      XDO_GET_CHILDCOUNT,
	  XDO_GET_CHILDBYINDEX,
	  XDO_GET_CHILDLIST,
	  XDO_ADD_CHILD,
	  XDO_REMOVE_CHILD,

	  XDO_SET_ATTR,
	  XDO_GET_ATTR,
	  XDO_ADD_ATTR,
	  XDO_GET_ATTR_COUNT,
	  XDO_GET_ATTR_BYINDEX,
	  XDO_GET_TEXT,
	  XDO_GET_ALLTEXT,

	  XDO_HITTEST,
	  XDO_SUBMIT,
	  XDO_RESETRADIO,
	  XDO_SETRADIO,
	  XDO_RESETINPUT,
	  XDO_SETINPUT,
	  XDO_LAYEROUT_CELL,
	  XDO_LAYEROUT_CELLS,

	  XDO_PAINT_TABLE,
	  XDO_LAYEROUT_TAB,
	  XDO_LAYEROUT_TABS,
	  XDO_LAYEROUT_TABPRE,

	  XDO_PAINT,
	  XDO_RESET,
	  XDO_REFRESH,

	  XDO_KEEP_FORMAT,
	  XDO_SET_DATA,
	  XDO_GET_STYLE,

	  XDO_SET_TEMPLATE,
	  XDO_SET_FINAL,
	  XDO_SET_START,
	  //XDO_GET_IMAGE,
	  //XDO_SET_IMAGE,
	  XDO_GET_RECT,
	  XDO_EVENT_ONTIMER,
	  XDO_EVENT_ONTIMEROVER,
	  XDO_EVENT_ONENTERBACKWARD,
	  XDO_EVENT_ONENTERFORWARD,

	  XDO_EVENT_ONLOAD,
	  XDO_EVENT_ONUNLOAD,
	  XDO_EVENT_ONCHANGE,
	  XDO_EVENT_ONSUBMIT,
	  XDO_EVENT_ONSELECT,
	  XDO_EVENT_ONBLUR,
	  XDO_EVENT_ONFOCUS,

	  XDO_EVENT_ONCLICK,
	  XDO_EVENT_DBLCLICK,
	  XDO_EVENT_ONMOUSEDOWN,
	  XDO_EVENT_ONMOUSEUP,
	  XDO_EVENT_ONMOUSEMOVE,
	  XDO_EVENT_ONMOUSEOUT,
	  XDO_EVENT_ONMOUSEOVER,
	  XDO_EVENT_ONPICK,

	  XDO_EVENT_ONKEYDOWN,
	  XDO_EVENT_ONKEYUP,
	  XDO_EVENT_ONKEYPRESS,
	  XDO_EVENT_ONCHAR,
	  XDO_EVENT_ONSTRING,
	  XDO_UPDATE_TABLE,
	  XDO_UPDATE_TABLECHILD,
	  XDO_UPDATE_IMAGE,
	  XDO_DO_STYLE,
	  XDO_DO_CSS,
	  XDO_SELECT,
	  XDO_SELECTION,
	  XDO_CONNECT,
	  XDO_GET_ENTRIES,
	 // XDO_SET_TITLE
};


#define XDEC_CREATE(class_name)\
	public:\
	static XDomItem* _stdcall CreateObject(XU16 ID){\
		return new class_name(ID);}

#define XUSE_CREATE(class_name) class_name::CreateObject


enum {
	    XDOM_ECHO=0,
	    XDOM_A,
		XDOM_ACC,
		XDOM_ACCESS,
		XDOM_ADSET,
		XDOM_ANCHOR,
		XDOM_APPLET,
		XDOM_AREA,
		XDOM_B,
		XDOM_BIG,
		XDOM_BODY,
		XDOM_BR,
		XDOM_BUTTON,
		XDOM_CAPTION,
		XDOM_CARD,
		XDOM_CENTER,
		XDOM_CODE,
		XDOM_DEFINE,
		XDOM_DFN,
		XDOM_DISPLAYDEFINE,
		XDOM_DIV,
		XDOM_DO,
		XDOM_EM,
		XDOM_EMBED,
		XDOM_ENDSET,
		XDOM_FIELDSET,
		XDOM_FILEINFO,
		XDOM_FONT,
		XDOM_FORM,
		XDOM_FRAME,
		XDOM_FRAMESET,
		XDOM_GO,
		XDOM_H1,
		XDOM_H2,
		XDOM_H3,
		XDOM_H4,
		XDOM_H5,
		XDOM_H6,
		XDOM_HEAD,
		XDOM_HR,
		XDOM_HTML,
		XDOM_I,
		XDOM_IFRAME,
		XDOM_IMG,
		XDOM_INPUT,
		XDOM_INPUTA,
		XDOM_KEYDEFINE,
		XDOM_KEYMAP,
		XDOM_KBD,
		XDOM_LI,
		XDOM_LINK,
		XDOM_MAP,
		XDOM_META,
		XDOM_MOUSENORMALDEFINE,
		XDOM_MOUSENORMALMAP,
		XDOM_MOUSETABLEDEFINE,
		XDOM_MOUSETABLEMAP,
		XDOM_NETWORKDEFINE,
		XDOM_NOBR,
		XDOM_NOFRAMES,
		XDOM_NOOP,
		XDOM_NOSCRIPT,
		XDOM_OBJECT,
		XDOM_OL,
		XDOM_ONEVENT,
		XDOM_OPRATEDEFINE,
		XDOM_OPTGROUP,
		XDOM_OPTION,
		XDOM_P,
		XDOM_PARAM,
		XDOM_POSTFIELD,
		XDOM_PRE,
		XDOM_PREV,
		XDOM_REFRESH,
		XDOM_S,
		XDOM_SAMP,
//#if (__CORE_VERSION__>=0x02077000)
		XDOM_SBR,
//#endif
		XDOM_SCRIPT,
		XDOM_SELECT,
		XDOM_SETVAR,
		XDOM_SHOWTAG,
		XDOM_SMALL,
		XDOM_SOUNDDEFINE,
		XDOM_SPAN,
		XDOM_STRIKE,
		XDOM_STRONG,
		XDOM_STYLE,
		XDOM_SUB,
		XDOM_SUP,
		XDOM_TABLE,
		XDOM_TD,
		XDOM_TEMPLATE,
		XDOM_TEXTAREA,
		XDOM_TEXTFORMAT,
		
		XDOM_TH,
		XDOM_TIMER,
		XDOM_TITLE,
		XDOM_TOUCHDEFINE,
		XDOM_TOUCHMAP,
		XDOM_TR,
		XDOM_TT,
		//XDOM_TXT,
		XDOM_TXTDEFINE,
		XDOM_U,
		XDOM_UL,
		XDOM_VAR,
		XDOM_WML,
		XDOM_QUESTION=0xfffc,
		XDOM_SPACE=0xfffd,
	    XDOM_TEXT=0xfffe
};

class XGraphics;

class XDomItem;
class XDomView;
class XCSS;
class XDom;

#define DCBACK paintData.nBackColor//nDCColors[XCW_BACK]
#define DCTEXT paintData.nTextColor[0]//nDCColors[XCW_TEXT]
#define DCLINK paintData.nTextColor[1]//nDCColors[XCW_LINK]
#define DCSELECTBACK paintData.nSelectBack//nDCColors[XCW_SELECTBACK]
#define DCSELECTTEXT paintData.nTextColor[3]//nDCColors[XCW_SELECTTEXT]
#define DCACTIVE	 paintData.nTextColor[2]//nDCColors[XCW_ACTIVE]
#define DCBUTTON	 nButtonColor//nDCColors[XCW_BUTTON]
#define IND			 paintData.ind
#define SPACE		 paintData.nSpace
#define PADDING		 paintData.nPadding
#define SPACING		 paintData.nSpacing
#define DATA		 paintData.nData
#define TABBORDER	 paintData.nBorder
#define FONTSIZEALL	 paintData.nFontSize
#define FONTSTYLE	 paintData.nFontStyle
#define TRANS		 paintData.nTrans
#define ALIGNW		 paintData.alignw
#define ALIGNH		 paintData.alignh
#define WRAPFLAG	 paintData.nowrap
#define DROWSPAN	 paintData.rowspan
#define DCOLSPAN	 paintData.colspan
#define SETWIDTH	 paintData.nWidth
#define SETHEIGHT	 paintData.nHeight
#define SETFLOAT	 paintData.nFloat
#define SETCLEAR	 paintData.nClear

typedef struct _BORDERDATA
{
	enum {TOP,RIGHT,BOTTOM,LEFT};
	XU32 nColor[4];
	XU8  nBorderStyle[4];
	XU8  nBorderWidth[4];
	void Init()
	{
		nColor[0]=nColor[1]=nColor[2]=nColor[3]=0;
		nBorderStyle[0]=nBorderStyle[1]=nBorderStyle[2]=
			nBorderStyle[3]=XELT::LS_NONE;
		nBorderWidth[0]=nBorderWidth[1]=nBorderWidth[2]=nBorderWidth[3]=0;
	}
}BORDERDATA;

typedef struct _LAYEROUTDATA
{
	XU8 nBorderWidth[4];
	short nMargin[4];
	XU8 nPadding[4];
	XINT nOffset[4];
	void Init()
	{
		nBorderWidth[0]=nBorderWidth[1]=nBorderWidth[2]=nBorderWidth[3]=0;
		nMargin[0]=nMargin[1]=nMargin[2]=nMargin[3]=0;
		nPadding[0]=nPadding[1]=nPadding[2]=nPadding[3]=0;
	}
	void Total()
	{
		for(int i=0;i<4;i++)
			nOffset[i]=nMargin[i]+nPadding[i]+nBorderWidth[i];
	}
}LAYEROUTDATA;

enum {LA_SAME,LA_SAMPLE,LA_ADJUST};

typedef struct _DRAWCONTEXT
{
	enum {XDFS_FILLBACK=0x01};
	XRect	   paint;
	XRect	   win,view;
	XPoint     offset;
	XGraphics* pDraw,*pContext;
	//XU32       nButtonColor;
	XU8		   nStatus;
	//XU16	   nFixWin;
	XDomItem   *pSelect,*pActive;//,*pText;
	XDomView   *pView;
	XDom	   *pDom;
	XCSS	   *pCss;

#ifdef _SYMBIAN	  // modify by hgl
#ifdef _DEBUG
	#pragma pack(1)
#endif
	struct PAINTDATA
	{
		XU32	   nData;
		XU32	   nBackColor,nSelectBack;//DCColors[XCW_BUTTON+1];
		XU32	   nTextColor[4];
		XU8		   nSpace,nBorder,alignw,alignh,nowrap;
		XU8		   nPadding,nSpacing;//,bFillStyle;
		XU8		   nTrans;//,nDecLink,nDecActive,nDecSelect;
		XU32	   nFontStyle;
		XU32	   nFontSize;
		DOMPOS	   ind;
		//XU16	   nMargin[4];
		DOMPOS	   nWidth,nHeight;//,nWinLeft,nWinRight;
		XU8		   rowspan;
		XU8		   colspan;
		XU8		   nFloat,nClear,nListType;
	}paintData;


	XU8 nLayerMode,bCollapse;
#ifdef _DEBUG
	#pragma pack()
#endif
#else
#pragma pack(1)
	struct PAINTDATA
	{
		XU32	   nData;
		XU32	   nBackColor,nSelectBack;//DCColors[XCW_BUTTON+1];
		XU32	   nTextColor[4];
		XU8		   nSpace,nBorder,alignw,alignh,nowrap;
		XU8		   nPadding,nSpacing;//,bFillStyle;
		XU8		   nTrans;//,nDecLink,nDecActive,nDecSelect;
		XU32	   nFontStyle;
		XU32	   nFontSize;
		DOMPOS	   ind;
		//XU16	   nMargin[4];
		DOMPOS	   nWidth,nHeight;//,nWinLeft,nWinRight;
		XU8		   rowspan;
		XU8		   colspan;
		XU8		   nFloat,nClear,nListType;
	}paintData;
	XU8 nLayerMode,bCollapse;
	//XU16 nMargin[4];
#pragma pack()
#endif

	XStack<PAINTDATA> stacks;
public:
	void SetFontSize(XU8 nSize)
	{
	   for(int i=0;i<4;i++)
	   {
		   //((XU8*)&FONTSIZEALL)[i]&=~0xff00;
		   ((XU8*)&FONTSIZEALL)[i]|=nSize;//(nSize<<8);
	   }
	}

	void SetBold(XBOOL b)
	{
		for(int i=0;i<4;i++)
		{
		   if(b)
			   ((XU8*)&FONTSTYLE)[i]|=XGraphics::XF_BOLD;
		   else
			   ((XU8*)&FONTSTYLE)[i]&=~XGraphics::XF_BOLD;
		   //((XU8*)&FONTSIZEALL)[i]|=(nSize<<8);
		}
	}
	void SetItalic(XBOOL b)
	{
		for(int i=0;i<4;i++)
		{
		   if(b)
			   ((XU8*)&FONTSTYLE)[i]|=XGraphics::XF_ITALIC;
		   else
			   ((XU8*)&FONTSTYLE)[i]&=~XGraphics::XF_ITALIC;
		   //((XU8*)&FONTSIZEALL)[i]|=(nSize<<8);
		}
	}
	void SetStrike(XBOOL b)
	{
		for(int i=0;i<4;i++)
		{
		   if(b)
			   ((XU8*)&FONTSTYLE)[i]|=XGraphics::XF_STRIKE;
		   else
			   ((XU8*)&FONTSTYLE)[i]&=~XGraphics::XF_STRIKE;
		}
	}
	void SetUnderline(XBOOL b)
	{
		for(int i=0;i<4;i++)
		{
		   if(b)
			   ((XU8*)&FONTSTYLE)[i]|=XGraphics::XF_UNDERLINE;
		   else
			   ((XU8*)&FONTSTYLE)[i]&=~XGraphics::XF_UNDERLINE;
		}
	}
	int ToPixel(XVar&var,int tw)
	{
		int v=var.iValue;
		if(var.iUnit)
			v=v*(SPACE);
		if(tw)
		{
			if(v<0)
				v=-v*tw/100;
			if(var.iSign)
				v=-v;
		}
		return v;
		//if(var.iUnit) v=v*(
	}
	XU8 IsWrap(){return paintData.nowrap==XEA::WRAP;}
	void SaveData()
	{
	    stacks.Add(paintData);
	}
	void RestoreData()
	{
		stacks.Pop(paintData);
	}
	void Init()
	{
		stacks.RemoveAll();
		//nLayerMode=LA_SAME;
		//pContext=XNULL;
		nStatus=0;
		bCollapse=0;
		paintData.rowspan=1;
		paintData.colspan=1;
		paintData.nListType=XELT::TYPE_DISC;
		//paintData.nClear=0;
		paintData.nWidth=0;
		paintData.nHeight=0;
		paintData.nFloat=XELT::FLOAT_NONE;
		paintData.nClear=XELT::FLOAT_NONE;
		paintData.alignw=XEA::LEFT;
		paintData.alignh=XEA::BOTTOM;
		paintData.nowrap=XEA::WRAP;
		paintData.nData=0;
		paintData.nTrans=0;
		paintData.nPadding=0;
		paintData.nSpacing=0;
		paintData.nBorder=0;
		paintData.ind=0;
		paintData.nFontStyle=(XGraphics::XF_UNDERLINE<<8)|(XGraphics::XF_UNDERLINE<<16);
		paintData.nFontSize=XWindow::m_nTextSize|(XWindow::m_nTextSize<<8)|(XWindow::m_nTextSize<<16)|(XWindow::m_nTextSize<<24);
			//XGraphics::XS_NORMAL|((XGraphics::XS_NORMAL)<<8)|
			//((XGraphics::XS_NORMAL)<<16)|(XGraphics::XS_NORMAL<<24);
		//for(int i=0;i<=XCW_BUTTON;i++)
		//	nDCColors[i]=XWindow::GetSysColor(i);
		paintData.nBackColor=XWindow::GetSysColor(XCW_BACK);
		//nButtonColor=XWindow::GetSysColor(XCW_BUTTON);
		paintData.nTextColor[STA_NORMAL]=XWindow::GetSysColor(XCW_TEXT);
		paintData.nTextColor[STA_LINK]=XWindow::GetSysColor(XCW_LINK);
		paintData.nTextColor[STA_ACTIVE]=XWindow::GetSysColor(XCW_ACTIVE);
		paintData.nTextColor[STA_SELECTED]=XWindow::GetSysColor(XCW_SELECTTEXT);
		paintData.nSelectBack=XWindow::GetSysColor(XCW_SELECTBACK);
		paintData.nSpace=XWindow::m_nTextHeight;
		//paintData.nParaSpace=nParaSpace;
		//paintData.nRowSpace=nRowSpace;
		pDraw->SetBackColor(0);
		pDraw->SetColor(DCTEXT);
	}
	/*void SaveStyle()
	{
		nStyles.Push(nRowSpace);
		nStyles.Push(nParaSpace);
		//nStyles.Push(bFillStyle);
		nStyles.Push(nPadding);
		nStyles.Push(nSpacing);
		nStyles.Push(nBorder);
	}
	void ResotreStyle()
	{
		nStyles.Pop(nBorder);
		nStyles.Pop(nSpacing);
		nStyles.Pop(nPadding);
		//nStyles.Pop(bFillStyle);
		nStyles.Pop(nParaSpace);
		nStyles.Pop(nRowSpace);
	}
	void Save()
	{
		nColors.Push(pDraw->GetBackColor());
		nColors.Push(pDraw->GetColor());
		XU32 s=pDraw->GetFontStyle();
		nColors.Push(s);
	}
	void Restore()
	{
		XU32 nColor;
		if(nColors.Pop(nColor))
			pDraw->SetFontStyle(nColor);
		if(nColors.Pop(nColor))
			pDraw->SetColor(nColor);
		if(nColors.Pop(nColor))
			pDraw->SetBackColor(nColor);
	}*/
}DRAWCONTEXT,*PDRAWCONTEXT;

class XDom;

typedef struct _CELLDATA
{
	XPOINT	     cur;
	XINT	     nCol,nRow,nMin,nSubWidth,nSubHeight;
	DOMPOSARRAY  cols,rowws,fixCols,spans;
	DOMPOSARRAY  lfloat,rflat;
	XS16Array    setCols;
	XU8Array     tabs;
	XSIZE	     max;
	XU8		     bEmpty,bFinal,bNewLine;
	LAYEROUTDATA*pData;
//	LAYEROUTDATA margin;
	//DOMPOS	  ind;
	//XU16		  nFixWin;
//	XBOOL	  IsWrap()
//	{return nowrap!=XEA::NOWRAP;}
/*	void Reset(int x,int y,XU8 aw,XU8 ah,XU8 no)
	{
//		ind=in;
		nMargin[0]=nMargin[1]=nMargin[2]=nMargin[3]=0;
		bEmpty=XTRUE;
		cur.x=x;
		bFinal=XFALSE;
		cur.y=y;
		alignw=aw;
		alignh=ah;
		nRow=nCol=0;
		bNewLine=XTRUE;
		max=XSize(x,y);
		tabs.RemoveAll();
		spans.RemoveAll();
		nMin=0;
		nowrap=no;
	}*/
	void Reset(int x,int y,XU8 bInit)
	{
		cur.x=x;
		cur.y=y;
		bFinal=XFALSE;
		bNewLine=XTRUE;
		bEmpty=XTRUE;
		max=XSize(x,y);
		if(bInit)
		{

			pData=XNULL;
			nSubWidth=0;
			nSubHeight=0;
//			margin.Init();
		}
		tabs.RemoveAll();
		spans.RemoveAll();
		nMin=0;
		nRow=nCol=0;
	}
}CELLDATA,*PCELLDATA;

enum {XL_WAIT=0,XL_LOADING=1,XL_FAILED=2,XL_OK=3};
enum {XIT_IMAGE,XIT_DATA};
class XDomItem;
typedef XVector<XDomItem*> XDomList;
class XImageList;
typedef struct _XIMAGEDATA
{
	friend class XImageList;
	XString8 strURL;
	XU8		 nStatus;
	XU8 	 nMethod;
	XU8		 nType;
	XU8		 bModify;
	XU16	 nIndex,nTicks;
	XDom*	 pDom;

	~_XIMAGEDATA()
	{
	}
	XBOOL FromResource(XDom*pDom,XPCTSTR strFile)
	{
		XBOOL bOK=XFALSE;
		if(nType==XIT_IMAGE)
		{
			bOK=image.CreateFromResource(strFile);
		}
		else
		{
			bOK=XResource::LoadData(data,strFile);
		}
		nStatus=bOK?XL_OK:XL_FAILED;
		if(bOK)
			Final(pDom,XTRUE);
		return bOK;
	}
	XU8 Timer()
	{
		if(nType!=XIT_IMAGE) return 0;
		if(image.IsEmpty()||image.FrameCount()<2) return 0;
		nTicks++;
		int nc=image.GetFrameDelay(nIndex);
		if(nTicks<nc) return 0;
		nTicks=0;
		nIndex++;
		if(nIndex>=image.FrameCount())
			nIndex=0;
		image.SetActive(nIndex);
		bModify=XTRUE;
		return 1;

	}

	void ResetStream()
	{
		if(nType==XIT_IMAGE)
			image.ResetStream();
		else
			data.RemoveAll();
	}
	XBOOL Final(XDom*pDom,XU8 bUpdate=XFALSE);
	void AppendData(XU8*pData,int nSize)
	{
		if(nType==XIT_IMAGE)
			image.Append(pData,nSize);
		else
			data.Append(pData,nSize);
	}
	XImage* GetImage(){return &image;}

//protected:
	XDomList list;
	XU8Array data;
	XImage	 image;
}XIMAGEDATA;

class XImageList:public XSort
{
public:
	~XImageList()
	{
		m_list.FreeAll();
	}
	XU8 Timer()
	{
		XU8 bOK=XFALSE;
		for(XU32 i=0;i<m_list.GetSize();i++)
		   bOK|=m_list[i]->Timer();
		return bOK;
	}
	void ClearModify()
	{
		for(XU32 i=0;i<m_list.GetSize();i++)
		   m_list[i]->bModify=XFALSE;
	}
	void Release(){m_list.FreeAll();}
	XIMAGEDATA* operator[](int id){return m_list[id];}
	XU32 GetCount(){return m_list.GetSize();}
	void* GetData(XU32 id){return m_list[id]->strURL;}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}

	XIMAGEDATA* GetImage(XU32 id){return m_list[id];}
	XIMAGEDATA* SetImage(XPCTSTR strURL,XDomItem*pItem,XDom*pDom,XU8 nType=XIT_IMAGE)
	{
		if(strURL==XNULL||strURL[0]==0) return XNULL;
		int mi=0;
		int id=Index((void*)strURL,mi);//,XSS_NORMAL);
		XIMAGEDATA*p=XNULL;
		if(id<0)
		{
			p=new XIMAGEDATA;
			m_list.InsertAt(mi,p);
			p->pDom=pDom;
			p->strURL=strURL;
			p->nStatus=XL_WAIT;
			p->bModify=XFALSE;
			p->nIndex=0;
			p->nTicks=0;
			p->nMethod=XEM::GET;
			//p->nSize=0;
			p->nType=nType;
		}
		else p=m_list[id];
		if(pItem!=XNULL) p->list.Add(pItem);
		return p;
	}
	XBOOL CanLayerout()
	{
		for(XU32 i=0;i<m_list.GetSize();i++)
		{
			switch(m_list[i]->nType)
			{
			case XIT_DATA:
				 if(m_list[i]->nStatus<XL_FAILED)
					 return XFALSE;
				 break;
			}
		}
		return XTRUE;

	}
public:
	XVector<XIMAGEDATA*> m_list;
};

typedef struct _SELECTMARK
{
	XDomItem*pSelect;
	XU8		 nDirect;
	XU8		 nCount;
	enum {XNEXT=1,XLAST=2};
}SELECTMARK;
/*typedef struct _SELECTINFO
{
	XDomItem* pSelect;
	XDomItem* pLast;
	XDomItem* pText;
	XDomItem* pNextText;
	XU8		  nDirect;
	XU8       nCount,nTxtCount,nLength;
	enum {XUP=XK_UP,XDOWN=XK_DOWN,XLEFT=XK_LEFT,XNEXT=1,XLAST=2};
}SELECTINFO,*PSELEFINFO;*/

enum {XRNONE=0,
	XRREPAINT=1,
	XRCONNECT=2,
	XRLAYEROUT=4,
	XRTIMEOVER=8,
	XROFFSET=0x10,
	XRSETHEADER=0x20,
	XRSETVAR=0x40,
	XRCANCHOR=0x80,
	XRCINPUT=0x100,
	XRIMAGE=0x200,
	XRACTIVE=0x400,
	XRBACKWORD=0x800,
	XRFORWARD=0x1000,
	XRNEXT=0x2000,
	XRVARCHANGE=0x4000,
	XREFRESH=0x8000,
	/*XREDIT=0x400,
	XRCOMMAND=0x800,
	XRLIST=0x1000,
	XRMENU=0x2000,
	XRMESSAGE=0x4000,
	XRTITLE=0x8000*/};

enum {XCUR_ARROW=0,XCUR_ANCHOR=1,XCUR_INPUT=2,XCUR_HAND=3,XCUR_HIDE=4};

typedef struct _XEVENTDATA
{
	XDom*		  pDom;
	XPCWSTR		  strTitle;
	XURL		  tempURL;
	XURL		  setURL;
	XString8	  strRefer;
	XU8			  nMethod,bAuto;
	XString8Array strHeaders;
	XString8Array strValues;
	XString8Array strDatas;
	XRect		  rect;
	XPoint		  offset;
	XDomView*	  pView;
	XRect		  viewRect;
	XU32		  nData;
//	XTime		  time;
	void SetURL(XPCTSTR strURL,XU8 m=XEM::GET)
	{
		//bJustCard=XTRUE;

		tempURL=setURL;
		tempURL.SetURL(strURL);
		//if(nMethod!=m) bJustCard=XFALSE;
		//else if(tempURL.m_strURL!=setURL.m_strURL)
		//	bJustCard=XFALSE;
		nMethod=m;
	}
	void SetURL(XURL&url,XU8 m=XEM::GET)
	{
		tempURL=url;
		//bJustCard=XTRUE;
		//if(nMethod!=m) bJustCard=XFALSE;
		//else if(tempURL.m_strURL!=setURL.m_strURL)
		//	bJustCard=XFALSE;
		nMethod=m;
	}
}XEVENTDATA;

typedef XVector<XIMAGEDATA*>XIMAGELIST;
typedef XDomItem* ( _stdcall*_CREATEDOM)(XU16);

class _XEXT_CLASS XDomItem
{
	friend class XDomNode;
	XDEC_CREATE(XDomItem)
public:
	virtual XDomList* GetChilds(){return XNULL;}
	XBOOL GetAttrib(XU32 nID,XINT&vr)
	{
		XVar v(nID,0);
		if(!GetAttrib(v)) 
			return XFALSE;
		vr=(XINT)v.ToInt();
		return XTRUE;
	}
	XBOOL GetAttrib(XU32 nID,XString8&vr)
	{
	    XVar v(nID,0);
		if(!GetAttrib(v)) return XFALSE;
		vr=v.ToString();
		return XTRUE;
	}
public:
	XU32 GetChildCount()
	{
		return Handle(XDO_GET_CHILDCOUNT,0,0);
	}
	XDomItem* GetChild(XU32 id)
	{
		return (XDomItem*)Handle(XDO_GET_CHILDBYINDEX,id,0);
	}
public:
	XBOOL GetEntries(XU16Array&list)
	{
		return Handle(XDO_GET_ENTRIES,(XU32)&list,0);
	}
	XU32 SetCSS(XCSS*pCss)
	{
		return Handle(XDO_DO_CSS,(XU32)pCss,0);
	}
	XBOOL IsLeafOf(XDomItem*p);
	void EndFloat(DRAWCONTEXT*pDraw,CELLDATA*pData);
	int GetWinWidth(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void NewRowMust(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 nType);
	XU8 NewRowIfMust(DRAWCONTEXT*pDraw,CELLDATA*pData,int w);
	XU8 NewFloatIfNeed(DRAWCONTEXT*pDraw,CELLDATA*pData,int w,XU8 bLeft);
	void NewFloat(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bLeft);
	void NewTabRow(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void SetTabRow(DRAWCONTEXT*pDraw,CELLDATA*pData,int w,int h);//CELLDATA*pData,XSIZE&sz,int nMin,XU8 bCells);

	int GetWinRight(DRAWCONTEXT*pDraw,CELLDATA*pData);
	int GetWinLeft(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void SetFloat(DRAWCONTEXT*pDraw,CELLDATA*pData,const XSIZE&sz,XU8 bLeft);
	void SetAttribs(XVarList&list,XU16Array&ids,int nOffset,XDom*pDom);
	//static XBOOL Translate(XString8Array&list,

	//					   XString8&strValue,XString16&strNew);
	/*XBOOL SetTitle(XDomView*pView)
	{
		return Handle(XDO_SET_TITLE,(XU32)pView,0);
	}*/
	void DestoryChild();
	void GetList(XDomList&list,XU16 id,bool bChild=false);
	static XU16 FormatString(XGraphics&g,XString16&strTxt,int bx,int w,XU16Array&list);
	static XBOOL Translate(XString8Array&list,XString8&strValue);
	XU32 OnRefresh(XEVENTDATA*pData)
	{
		return Handle(XDO_REFRESH,(XU32)pData,0);
	}
	XU32 Connect(XEVENTDATA*pData)
	{	return Handle(XDO_CONNECT,(XU32)pData,0);}

	XU32 OnMouseDown(XPoint&pt,XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_ONMOUSEDOWN,(XU32)&pt,(XU32)pData);
	}
	XU32 OnPick(XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_ONPICK,(XU32)pData,0);
	}
	XU32 OnDblClick(XPoint&pt,XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_DBLCLICK,(XU32)&pt,(XU32)pData);
	}
	XU32 OnMouseUp(XPoint&pt,XEVENTDATA*pData)
	{   return Handle(XDO_EVENT_ONMOUSEUP,(XU32)&pt,(XU32)pData); }
	XU32 OnMouseMove(XPoint&pt,XEVENTDATA*pData)
	{   return Handle(XDO_EVENT_ONMOUSEMOVE,(XU32)&pt,(XU32)pData); }
	XU32 OnMouseOut(XPoint&pt,XEVENTDATA*pData)
	{   return Handle(XDO_EVENT_ONMOUSEOUT,(XU32)&pt,(XU32)pData); }
	XU32 OnMouseOver(XPoint&pt,XEVENTDATA*pData)
	{   return Handle(XDO_EVENT_ONMOUSEOVER,(XU32)&pt,(XU32)pData); }
//	XU32 OnKeyDown(XU32 c,XEVENTDATA*pData)
//	{	return Handle(XDO_EVENT_ONKEYDOWN,c,(XU32)pData);}
	XU32 OnKeyIp(XU32 c,XEVENTDATA*pData)
	{	return Handle(XDO_EVENT_ONKEYUP,c,(XU32)pData);}
	XU32 OnKeyPress(XU32 c,XEVENTDATA*pData)
	{	return Handle(XDO_EVENT_ONKEYPRESS,c,(XU32)pData);}
	XU32 OnString(XPCWSTR buf,XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_ONSTRING,(XU32)buf,(XU32)pData);
	}
	XU32 OnChar(XU32 c,XEVENTDATA*pData)
	{	return Handle(XDO_EVENT_ONCHAR,c,(XU32)pData);}
	XU32 OnTimerOver(XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_ONTIMEROVER,(XU32)pData,0);
	}
	XU32 OnClick(const XPoint&pt,XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_ONCLICK,(XU32)&pt,(XU32)pData);
	}
	XU32 OnFocus(XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_ONFOCUS,(XU32)pData,0);
	}
	XU32 OnBlur(XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_ONBLUR,(XU32)pData,0);
	}
	XU32 OnKeyDown(XWCHAR c,XEVENTDATA*pData)
	{
		//pData->rect.SetRectEmpty();
		return Handle(XDO_EVENT_ONKEYDOWN,c,(XU32)pData);
	}
	XU32 OnEnterForward(XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_ONENTERFORWARD,(XU32)pData,0);
	}
	XU32 OnEnterBackward(XEVENTDATA*pData)
	{
		return Handle(XDO_EVENT_ONENTERBACKWARD,(XU32)pData,0);
	}
	XU32 OnLoad(XEVENTDATA*pData)
	{	return Handle(XDO_EVENT_ONLOAD,(XU32)pData,0);}
	XU32 OnUnLoad(XEVENTDATA*pData)
	{	return Handle(XDO_EVENT_ONUNLOAD,(XU32)pData,0);}

	void MarkSelection(XSelection*p)
	{
		Handle(XDO_SELECTION,(XU32)p,0);
	}

	XDomItem* Select(XDomItem*pSelect,XU8 nDir=_SELECTMARK::XNEXT)//XDomItem**pText,XU8 nLength,XU8 nDirect)
	{
		SELECTMARK mark={pSelect,nDir,0};
		/*SELECTINFO info={pSelect,NULL,*pText==XNULL?pSelect:*pText,
						NULL,nDirect,0,0,nLength};
		pSelect=(XDomItem*)Handle(XDO_SELECT,(XU32)&info,0);
		if(pSelect==XNULL)
		{
			if(info.pLast!=info.pSelect)
				pSelect=info.pLast;
		}
		*pText=info.pNextText;*/
		//XU32 bStart=0;
		pSelect=(XDomItem*)Handle(XDO_SELECT,(XU32)&mark,0);//(XU32)&bStart);
		return pSelect;
	}
	XBOOL OnTimer(XEVENTDATA*pData)
	{	return Handle(XDO_EVENT_ONTIMER,(XU32)pData,0); }
	XBOOL GetAllText(XString16&strTxt)
	{
		return Handle(XDO_GET_ALLTEXT,(XU32)&strTxt,0);
	}
	XBOOL GetText(XString16&pText)
	{
		return Handle(XDO_GET_TEXT,(XU32)&pText,0);
	}
	XBOOL GetRoundRect(XRect&rect,XU32 nType=0)
	{	return Handle(XDO_GET_RECT,(XU32)&rect,nType); }
//	XBOOL SetImage(XIMAGEDATA*pData)
//	{ return Handle(XDO_SET_IMAGE,(XU32)pData,0);}
//	XBOOL GetImage(XVar&var)
//	{return Handle(XDO_GET_IMAGE,(XU32)&var,0);}
	XBOOL Paint(DRAWCONTEXT*pDraw)
	{
		//LOGWHERE();
		return Handle(XDO_PAINT,(XU32)pDraw,0);
	}
	XBOOL SetAttrib(XVar&var)
	{return Handle(XDO_SET_ATTR,(XU32)&var,0);}
	XBOOL AddAttrib(XVar&var)
	{return Handle(XDO_ADD_ATTR,(XU32)&var,0);}
	XBOOL GetAttribByIndex(XVar&var,XU32 nID)
	{
		return Handle(XDO_GET_ATTR_BYINDEX,(XU32)&var,nID);
	}
	XU32 GetAttribCount()
	{
		return Handle(XDO_GET_ATTR_COUNT,0,0);
	}
	XBOOL GetAttrib(XVar&var)
	{return Handle(XDO_GET_ATTR,(XU32)&var,0);}
	XBOOL LayeroutItem(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bCells)
	{return Handle(bCells?XDO_LAYEROUT_CELLS:XDO_LAYEROUT_CELL,
				   (XU32)pDraw,(XU32)pData);}
	XBOOL SetData(XPCTSTR pData,XINT nLength)
	{return Handle(XDO_SET_DATA,(XU32)pData,nLength);}
	XDomItem* GetParent()
	{return (XDomItem*)Handle(XDO_GET_PARENT,0,0);}
	XDomItem* HitTest(int x,int y)
	{return (XDomItem*)Handle(XDO_HITTEST,x,y);}
	XBOOL SetParent(XDomItem*pItem)
	{return Handle(XDO_SET_PARENT,(XU32)pItem,0);}
	XBOOL IsFormat(){return Handle(XDO_KEEP_FORMAT,0,0);}
	XBOOL Final(DRAWCONTEXT*pDraw,XEVENTDATA*pData){return Handle(XDO_SET_FINAL,(XU32)pDraw,(XU32)pData);}
	XBOOL Start(DRAWCONTEXT*pDraw,XEVENTDATA*pData)
	{
		return Handle(XDO_SET_START,(XU32)pDraw,(XU32)pData);
	}
	XU32 GetStyle() {return Handle(XDO_GET_STYLE,0,0);}
	XBOOL AddChild(XDomItem*pItem) {return Handle(XDO_ADD_CHILD,(XU32)pItem,0);}
	XBOOL SetTemplate(XDomItem*pItem)
	{return Handle(XDO_SET_TEMPLATE,(XU32)pItem,0);}
	enum {DOM_NODE=1,DOM_TEMPLATE=2,DOM_SCRIPT=4,DOM_DATA=8,DOM_IMAGE=0x10,
		  DOM_ANCHOR=0x20,DOM_LEAF=0x40,DOM_TABLE=0x80,
		  DOM_SELECTABLE=0x100,DOM_PAINT=0x200,
		  DOM_CARD=0x400,DOM_INPUT=0x800,DOM_STATIC=0x1000,
		  DOM_EDITING=0x2000};
	XDomItem(XU16 ID)
	{
		m_nID=ID;
	};
	virtual ~XDomItem()
	{
		//int l = 0;
	}
	XU16 GetID() {return m_nID;}
	virtual XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2)
	{
		switch(nOpera)
		{
		case XDO_GET_ENTRIES:
			 XSortU16((XU16Array*)pData1).Add(GetID(),XTRUE);
			 return XTRUE;
		case XDO_SELECTION:
			 return Selection((XSelection*)pData1);
		case XDO_SELECT:
			 //return (XU32)SelectItem((SELECTINFO*)pData1);
			 return (XU32)SelectItem((SELECTMARK*)pData1);
		case XDO_UPDATE_TABLECHILD:
		case XDO_UPDATE_TABLE:
		case XDO_RESETRADIO:
		case XDO_RESETINPUT:
//		case XDO_UPDATE_TABLE:
		case XDO_SUBMIT:
			{
				XU32 s=0;
				XDomItem*p=GetParent();
				if(p!=XNULL)
					s|=p->Handle(nOpera,pData1,pData2);
				return s;
			}break;
		}
		return 0;
	}
	XDomItem* FindByAttrib(XVar &var, XU32 &nIndex, XU32 nID=0);
	XDomItem* FindByIndex(XU32 ix, XU32 &nIndex, XU32 nID=0);
	XU32	  FindByItem(XDomItem*p,XU32 &nIndex);
protected:
	XU8 GetStatus(DRAWCONTEXT*pDraw);
	//XBOOL PaintPrev(DRAWCONTEXT*pDraw,const XRect&rect);
	//void PaintEnd(DRAWCONTEXT*pDraw,const XRect&rect);
	//void LayeroutPrev(DRAWCONTEXT*pDraw,CELLDATA*pData);
	void PaintSelect(XRect&rect,DRAWCONTEXT*pDraw,XBOOL bForce=XFALSE);
	XU32 Selection(XSelection*p);
	XDomItem* SelectItem(SELECTMARK*pMark);//XDomItem*pSelect,XU32*bStart);
	//XDomItem* SelectItem(XDomItem*,XU8&bSet);

	XBOOL HitRoundRect(XINT x,XINT y)
	{
		XRect rect;
		if(Handle(XDO_GET_RECT,(XU32)&rect,0))
		{
			 if(rect.PtInRect(x,y)) return XTRUE;
		}
		return XFALSE;
	}
	XU16 AlignCol(DRAWCONTEXT*pDraw,CELLDATA*pData,DOMPOS h,XU8 aligh,DOMPOS th=0);
	void AlignRow(DRAWCONTEXT*pDraw,CELLDATA*pData);
	XBOOL NewRowIfNeed(DRAWCONTEXT*pDraw,CELLDATA*pData,int w);
	void NewRow(int h,DRAWCONTEXT*pDraw,CELLDATA*pData,DOMPOS ind);//const XSIZE&sz,XBOOL bAdd=XTRUE);
	void SetRowSize(DRAWCONTEXT*pDraw,CELLDATA*pData,const XSIZE&sz,DOMPOS nMin,XU8 bInit);
	//void NewTabRow(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bTable=XFALSE);//const XSIZE&sz,XBOOL bAdd=XTRUE);
	//XBOOL SetTabRow(DRAWCONTEXT*pDraw,CELLDATA*pData,const XSIZE&sz,XU16 nMin);
	XU16 m_nID;
};


#define XC_SAVEBACK   0x0001
#define XC_SAVETEXT   0x0002
#define XC_SAVELINK	   0x0004
#define XC_SAVEACTIVE      0x0008
//#define XC_SAVEFONT		   0x0010
#define XC_SAVESELECT	   0x0010
#define XC_SAVEIND		   0x0020
#define XC_SAVETRANS	   0x0040
#define XC_SAVESTYLE	   0x0080
#define XC_SAVESIZE		   0x0100
//#define XC_SAVELINK 0x0004
//#define XC_SAVEACTIVE 0x0008
//#define XC_SAVEFONT 0x000
class _XEXT_CLASS XDomLeaf : public XDomItem
{
	XDEC_CREATE(XDomLeaf)
public:

	void PaintBorder(DRAWCONTEXT*pDraw,XRect&rect);
	void PaintBack(DRAWCONTEXT*pDraw,XRect&rect,XU8 bInside=XFALSE);

	XBOOL SetVariable(XEVENTDATA*pData,XPCTSTR strKey,XPCTSTR strValue);
	XBOOL CheckName(XPCTSTR strKey);
	XPCTSTR RefreshVar(XDomLeaf* pItem,XPCTSTR strMyKey,XEVENTDATA*pData);
	XBOOL PostVariable(XEVENTDATA*pData,XPCTSTR strValue=XNULL);
	XINT FindAttrib(XU16 id)
	{
		for(XU32 i=0;i<m_attribs.GetSize();i++)
			if(m_attribs[i].nID==id) return i;
		return -1;
	}
	XINT FindAttrib(XU16 id,XINT nDefault)
	{
		for(XU32 i=0;i<m_attribs.GetSize();i++)
		{
			if((m_attribs[i].nID&0x3fff)==id)
				return (XINT)m_attribs[i].iData;
		}
		return nDefault;
	}
	XPCTSTR FindAttrib(XU16 id,XPCTSTR strDefault)
	{
		for(XU32 i=0;i<m_attribs.GetSize();i++)
		{	if((m_attribs[i].nID&0x3fff)==id)
				return m_attribs[i].strData;
		}
		return strDefault;
	}
	XBOOL SetAttrib(XU16 id,XPCTSTR strValue);

	XBOOL SetAttrib(XU16 id,XU32 iValue);

	void SetAttrib(const XVar&p,XBOOL bAdd=XTRUE);

	XBOOL GetAttrib(XVar&var)
	{
	    XINT id=FindAttrib(var.nID);
		if(id<0) return XFALSE;
		var=m_attribs[id];
		return XTRUE;
	}
	XDomLeaf(XU16 ID):XDomItem(ID)
	{
		m_bCssOK=XFALSE;
		m_pParent=XNULL;
	}
    XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
protected:
	XU8 IsInline();
	XBOOL IsFingerInline();
	XBOOL IsFingerBlock();
	XBOOL IsVisiable();
	void SetMargin(LAYEROUTDATA*pData,int&w,int&h,XSize&sz);
	void InitMargin(DRAWCONTEXT*pDraw,CELLDATA*pData,LAYEROUTDATA*pd,DOMPOS&x,DOMPOS&y,DOMPOS&w,DOMPOS&h,XU8 bTableItem=XFALSE);
	void DoStyle(XCSS*pCss);
	void DoCSS(XCSS*pCss);
	void CalcLineData(BORDERDATA&data,XRect&rect,int&bx,int&by,
					  int&ex,int&ey,XU8 nDir,XU8 nStep,XU8 bFull);
	XU8 MakeBorderData(BORDERDATA&data,XVarList&list,XU8 nStatus);
	XU32 SetLayerData(DRAWCONTEXT*pDraw,LAYEROUTDATA*pd,XVar&var);

	void SetFontType(XU32&s,XU8 nType,XU8 nStatus,XU8 bSet);
	XU32 SetDrawContext(DRAWCONTEXT*pDraw,XVar&var);
	XU32 OnTimer(XEVENTDATA*pData,XRect&rect);
	void SetFontStyle(XU8&s,XU8 nMode);
	void EndLayerout(DRAWCONTEXT*pDraw,CELLDATA*pData);
	XU32 PreLayerout(DRAWCONTEXT*pDraw,CELLDATA*pData,LAYEROUTDATA*pl);
	void EndPaint(DRAWCONTEXT*pDraw);
	XU32 PrePaint(DRAWCONTEXT*pDraw);

	XVarList  m_attribs;
	XDomItem* m_pParent;
	XU8		  m_bCssOK;
};

class _XEXT_CLASS XDomNode : public XDomLeaf
{
	XDEC_CREATE(XDomNode)
public:
	XDomList* GetChilds(){return &m_childs;}
	XDomNode(XU16 ID):XDomLeaf(ID)
	{
		m_pParent=NULL;
	}
    XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
public:
//	XDomItem* FindByAttrib(XVar&var,XU32&nIndex,XU32 nID=0);
//	XDomItem* FindByIndex(XU32 ix,XU32&nIndex,XU32 nID=0);
//	XDomItem*m_pParent;
	XDomList m_childs;
protected:
	XU32 HandleChild(XU32 nOpera,XU32 pData1,XU32 pData2);
	XDomItem* SelectItem(SELECTMARK*pMark);//XDomItem*pSelect,XU32*bStart)//SELECTINFO*pInfo);
	XBOOL GetRoundRect(XRect&rect,XU32 nType);
};

class _XEXT_CLASS XDomData:public XDomItem
{
	XDEC_CREATE(XDomData)
public:
	XDomData(XU16 ID):XDomItem(ID){}
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2)
	{
		switch(nOpera)
		{
		default:
			return XDomItem::Handle(nOpera,pData1,pData2);
		case XDO_SET_DATA:
			m_data.Append((XU8*)pData1,pData2);
			return XTRUE;
		case XDO_GET_STYLE:
			return DOM_DATA|XDomItem::Handle(nOpera,pData1,pData2);
		case XDO_GET_ATTR_COUNT:
			return 1;
		/*case XDO_GET_ATTR_BYINDEX:
		case XDO_GET_ATTR:

			{
				XVar*p=(XVar*)pData1;
				*p=m_data;
				p->nID=XEAB::TEXT;
			}
			return XTRUE;*/
		}
		return 0;
	}
protected:
	XU8Array m_data;
};


#endif // !defined(AFX_XDOMITEM_H__935D134E_BD36_405F_AE70_3EBF06518F08__INCLUDED_)
