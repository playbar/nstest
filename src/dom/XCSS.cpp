// XCSS.cpp: implementation of the XCSS class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XCSS.h"
#include "XEnumData.h"
#include "XDomPhase.h"
#include "XDom.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XCSS::XCSS()
{
	m_pDom=XNULL;
	m_bClass = false;
}

XCSS::~XCSS()
{
	Reset();
}

void XCSS::Reset()
{
	//m_strNames.RemoveAll();
	m_css.Release();
	m_ids.m_list.FreeAll();
}

void XCSS::AddCSS(XURL*pURL,XString8 &strCss)
{
	//return;
	RemoveEcho(strCss);

	int id=0;
	while(id<(XINT)strCss.GetLength())
	{
	   
	   int ie=strCss.Find('{',id);
	   if(ie==id)
	   {id++;continue;   }
	   if(ie<id+1) 
		   break;
	   XString8 strNames;
	   strCss.Mid(strNames,id,ie-id);
	   id=ie+1;
	   ie=strCss.Find('}',ie+1);
	   if(ie==id)
	   {
		   id++;continue;
	   }
	   if(ie<id+2) 
		   break;
	   XString8 strAttribs;
	   strCss.Mid(strAttribs,id,ie-id);
	   AddCSS(pURL,strNames,strAttribs);
	   id=ie+1;
	}
}

void XCSS::AddCSS(XURL*pURL,XString8 &strNames, XString8 &strAttribs)
{
	//return;
	if(strNames.IsEmpty()||strAttribs.IsEmpty()) return;
	XString8Array list;
	list.SplitString(strNames,',');
	if(list.GetSize()<=0) return;
	XVarList vars;
	//XSelector*pSelect=new XSelector;
	PhaseCSS(pURL,vars,strAttribs);
	if(vars.GetSize()<=0) return;
	/*if(pSelect->m_list.GetSize()<=0)
	{
		delete pSelect;
		return;
	}
	m_css.Add(pSelect);*/
	for(XU32 i=0;i<list.GetSize();i++)
	{
		//XU16Array *pArray=new XU16Array;
		XU16Array entries;
		XU8 nStatus=PhaseSelector(entries,list[i]);
		if(entries.GetSize()<=0) 
			continue;
		if(nStatus<STA_SET)
			m_css.AddAttribs(entries,entries.GetSize()-1,vars,nStatus);
		//pSelect->m_selectors.Add(pArray);
	}
	/*XVarList atts;
	PhaseCSS(atts,strAttribs);
	//return;
	
	if(atts.GetSize()<=0) return;
	//return;
	//XSortString8 sort(m_strNames);
	for(XU32 i=0;i<list.GetSize();i++)
	{
		XString8 strClass;
		//int id=GetID(list[i],XTRUE);
		list[i].TrimRight();
		int idd=PhaseName(list[i],strClass);
		XCSSSelector*p=XNULL;
		int m=0;
		int id=Index(id,m);
		if(id<0)
		{
			p=new XCSSSelector;
			p->m_nSelector=idd;
			//if(idd==0)
			//	int a=0;
			m_css.InsertAt(m,p);
		}
		else p=m_css[id];
		
		p->AddVars(atts,strClass);
		//if(strClass.GetLength()>0)
		//	strClass.EmptyNoFree();
	}*/
}


struct _CSSENTRY
{
	XPCTSTR strKey;
	XU8		nType;
	XU16	nAttribID;
};
static _CSSENTRY _cssEntries[]=
{
	{"background",XCSS::CST_BACKGROUND,XEAB::BGCOLOR},
	{"background-attachment",XCSS::CST_ATTACH,XEAB::ATTACH},
	{"background-color",XCSS::CST_COLOR,XEAB::BGCOLOR},	
	{"background-image",XCSS::CST_IMAGE,XEAB::BACKIMAGE},
	{"background-position",XCSS::CST_POSITION,XEAB::POSITIONX},
	{"background-repeat",XCSS::CST_ALIGN,XEAB::REPEAT},
	{"border",XCSS::CST_BORDERALL,XEAB::BORDER_TOPWIDTH},
	{"border-bottom",XCSS::CST_BORDERONE,XEAB::BORDER_BOTTOMWIDTH},
	{"border-bottom-color",XCSS::CST_COLOR,XEAB::BORDER_BOTTOMCOLOR},
	{"border-bottom-style",XCSS::CST_BORDERSTYLE,XEAB::BORDER_BOTTOMSTYLE},
	{"border-bottom-width",XCSS::CST_BORDERWIDTH,XEAB::BORDER_BOTTOMWIDTH},
	{"border-collapse",XCSS::CST_COLLAPSE,XEAB::BORDER_COLLAPSE},
	{"border-color",XCSS::CST_BORDERCOLORS,XEAB::BORDER_TOPCOLOR},
	{"border-left",XCSS::CST_BORDERONE,XEAB::BORDER_LEFTWIDTH},
	{"border-left-color",XCSS::CST_COLOR,XEAB::BORDER_LEFTCOLOR},
	{"border-left-style",XCSS::CST_BORDERSTYLE,XEAB::BORDER_LEFTSTYLE},
	{"border-left-width",XCSS::CST_BORDERWIDTH,XEAB::BORDER_LEFTWIDTH},
	{"border-right",XCSS::CST_BORDERONE,XEAB::BORDER_RIGHTWIDTH},
	{"border-right-color",XCSS::CST_COLOR,XEAB::BORDER_RIGHTCOLOR},
	{"border-right-style",XCSS::CST_BORDERSTYLE,XEAB::BORDER_RIGHTSTYLE},
	{"border-right-width",XCSS::CST_BORDERWIDTH,XEAB::BORDER_RIGHTWIDTH},
	{"border-style",XCSS::CST_BORDERSTYLES,XEAB::BORDER_TOPSTYLE},
	{"border-top",XCSS::CST_BORDERONE,XEAB::BORDER_TOPWIDTH},
	{"border-top-color",XCSS::CST_COLOR,XEAB::BORDER_TOPCOLOR},
	{"border-top-style",XCSS::CST_BORDERSTYLE,XEAB::BORDER_TOPSTYLE},
	{"border-top-width",XCSS::CST_BORDERWIDTH,XEAB::BORDER_TOPWIDTH},
	{"border-width",XCSS::CST_BORDERWIDTHS,XEAB::BORDER_TOPWIDTH},
	{"bottom",XCSS::CST_INT,XEAB::POS_BOTTOM},
	{"clear",XCSS::CST_FLOAT,XEAB::CLEAR},
	{"color",XCSS::CST_COLOR,XEAB::COLOR},
	{"display",XCSS::CST_DISPLAY,XEAB::DISPLAY_MODE},
	
	{"float",XCSS::CST_FLOAT,XEAB::FLOAT},
	{"font",XCSS::CST_FONTALL,XEAB::FONT_SIZE},
	{"font-family",XCSS::CST_STRING,XEAB::FONT_FAMILY},
	{"font-size",XCSS::CST_FONTSIZE,XEAB::FONT_SIZE},
	{"font-style",XCSS::CST_FONTSTYLE,XEAB::FONT_STYLE},
	{"font-weight",XCSS::CST_FONTWEIGHT,XEAB::FONT_WEIGHT},
	{"fontFamily",XCSS::CST_STRING,XEAB::FONT_FAMILY},
	{"fontSize",XCSS::CST_FONTSIZE,XEAB::FONT_SIZE},
	{"fontStyle",XCSS::CST_FONTSTYLE,XEAB::FONT_STYLE},
	{"fontWeight",XCSS::CST_FONTWEIGHT,XEAB::FONT_WEIGHT},
	{"fontsize",XCSS::CST_FONTSIZE,XEAB::FONT_SIZE},
	{"fontstyle",XCSS::CST_FONTSTYLE,XEAB::FONT_STYLE},
	{"fontweight",XCSS::CST_FONTWEIGHT,XEAB::FONT_WEIGHT},
	{"height",XCSS::CST_PECENT,XEAB::HEIGHT},
	{"left",XCSS::CST_INT,XEAB::POS_LEFT},
	{"line-height",XCSS::CST_PECENT,XEAB::FONT_HEIGHT},
	{"list-style",XCSS::CST_LISTALL,XEAB::LIST_STYLE_TYPE},
	{"list-style-image",XCSS::CST_IMAGE,XEAB::LIST_IMAGE},
	{"list-style-type",XCSS::CST_LISTTYPE,XEAB::LIST_STYLE_TYPE},
	{"margin",XCSS::CST_MARGINALL,XEAB::MARGIN_TOP},
	
	{"margin-bottom",XCSS::CST_PECENT,XEAB::MARGIN_BOTTOM},
	{"margin-left",XCSS::CST_PECENT,XEAB::MARGIN_LEFT},
	{"margin-right",XCSS::CST_PECENT,XEAB::MARGIN_RIGHT},
	{"margin-top",XCSS::CST_PECENT,XEAB::MARGIN_TOP},
	{"marginLeft",XCSS::CST_PECENT,XEAB::MARGIN_LEFT},
	{"marginRgiht",XCSS::CST_PECENT,XEAB::MARGIN_RIGHT},
	{"overflow",XCSS::CST_OVERFLOW,XEAB::OVERFLOW1},
	{"padding",XCSS::CST_MARGINALL,XEAB::PADDING_TOP},
	{"padding-bottom",XCSS::CST_PECENT,XEAB::PADDING_BOTTOM},
	{"padding-left",XCSS::CST_PECENT,XEAB::PADDING_LEFT},
	{"padding-right",XCSS::CST_PECENT,XEAB::PADDING_RIGHT},
	{"padding-top",XCSS::CST_PECENT,XEAB::PADDING_TOP},
	{"position",XCSS::CST_POSITIONMODE,XEAB::POSITION_MODE},
	//{"letter-spacing",XCSS::CST_PECENT,XEAB::LETTER_SPACING},
	{"right",XCSS::CST_INT,XEAB::POS_RIGHT},
	
	{"text-align",XCSS::CST_ALIGN,XEAB::ALIGN},
	{"text-decoration",XCSS::CST_DEC,XEAB::TEXT_DECORATION},
	{"text-indent",XCSS::CST_INT,XEAB::TEXT_INDENT},
	{"text-transform",XCSS::CST_TTRANS,XEAB::TEXT_TRANSFORM},
	{"textAlign",XCSS::CST_ALIGN,XEAB::ALIGN},
	{"textDecoration",XCSS::CST_DEC,XEAB::TEXT_DECORATION},
	{"textIndent",XCSS::CST_ALIGN,XEAB::TEXT_INDENT},
	{"textalign",XCSS::CST_ALIGN,XEAB::ALIGN},
	{"textindent",XCSS::CST_INT,XEAB::TEXT_INDENT},
	{"top",XCSS::CST_INT,XEAB::POS_TOP},
	{"visibility",XCSS::CST_VISIABLE,XEAB::HIDDENITEM},
	{"width",XCSS::CST_PECENT,XEAB::WIDTH},
	//{"word-spacing",XCSS::CST_INT,XEAB::WORD_SPACING},
	//{"width",XCSS::CST_
};

#define CSSCOUNT sizeof(_cssEntries)/sizeof(_CSSENTRY)

class XSortCSS:public XSort
{
public:
	XU32 GetCount(){return CSSCOUNT;}
	void* GetData(XU32 id){return (void*)_cssEntries[id].strKey;}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
};


static XU16 _nStatusID[]=
{ 		XEAB::BGCOLOR,
		XEAB::COLOR,
		XEAB::TEXT_DECORATION,
		XEAB::FONT_STYLE,
		XEAB::FONT_WEIGHT,
		XEAB::LIST_IMAGE,
		XEAB::ATTACH,
		XEAB::POSITIONX,
		XEAB::POSITIONY,
		XEAB::BACKIMAGE,
		XEAB::REPEAT};
static XBOOL IsStatusID(XU16 ID)
{
	for(int i=0;i<sizeof(_nStatusID)/sizeof(XU16);i++)
		if(_nStatusID[i]==ID) return XTRUE;
	return XFALSE;
}

int XCSS::AttribID(XPCTSTR strName,int&type)
{
	XSortCSS css;
	int na=css.Index((void*)strName,XSS_NOCASE);
	if(na<0) return na;
	//if(_cssEntries[na].nAttribID==XEAB::FONT_HEIGHT)
	//	int v=0;
	type=_cssEntries[na].nType;
	return _cssEntries[na].nAttribID;
}

const char* XCSS::AttribName(XVar&var)
{
	if(m_bClass)
	{
		if(var.nID<CSSCOUNT)
			return _cssEntries[var.nID].strKey;
		return XNULL;
	}
	else
		return XEAB::GetName(var.nID);
}

void XCSS::PhaseCSS(XURL*pURL,XVarList &list, XString8 &strAttribs)
{
	XString8Array ats;
	ats.SplitString(strAttribs,';');
	XSortCSS css;
	for(XU32 i=0;i<ats.GetSize();i++)
	{
		int id=ats[i].Find(':');
		if(id<=0) 
		{
			id=ats[i].Find('=');
			if(id<=0)
			continue;
		}
		XString8 strKey;
		
		ats[i].Left(strKey,id);
		strKey.TrimRight();
		//strKey.MakeLower();
		int na=css.Index(strKey);
		if(na<0) continue;
		//switch(_cssEntries[na].nType)
		int aid = _cssEntries[na].nAttribID;
		if(m_bClass)
			aid = na;
		XVar var(aid,0);
		ats[i].TrimRight();
		XPCTSTR strData=ats[i].GetData()+id+1;
		//while(ats[i][id]<=' '&&id<(int)ats[i].GetLength()) 
		while(*strData<=' '&&*strData!=0)
			strData++;
		MakeVar(pURL,list,var,_cssEntries[na].nType,strData);
		//list.Add(var);
	}	
}



void XCSS::MakeVar(XURL*pURL,XVarList&list,XVar &var, XU8 nType, XPCTSTR strCnt)
{
	//return;
	XString8Array args;
	switch(nType)
	{
	case CST_COLLAPSE:
		 var=strCnt[0]=='c'||strCnt[0]=='C';
		 if(var.iData==0) return;
		 break;
	case CST_OVERFLOW:
		 var=strCnt[0]!='h'||strCnt[0]!='H';
		 if(var.iData==0) return;
		 break;
	case CST_POSITIONMODE:
		 var=XELT::GetPositionType(strCnt);
		 break;
	case CST_VISIABLE:
		 var=strCnt[0]!='v'&&strCnt[0]!='V';
		 if(var.iData==0) break;
		 break;
	case CST_DISPLAY:
		 var=XELT::GetDisplayMode(strCnt);
		 if(var.iData==XELT::DISPLAY_EMPTY) return;
		 break;
	case CST_LISTALL:
		{
		   XString8 str(strCnt,STRING_ATTACH);
		   args.SplitStringS(str,", ");
		   str.EmptyNoFree();
		   if(args.GetSize()<=0) return;
		   /*if(args.GetSize()>0)
		   {

		   }*/
		   XString8 strURL;
		   for(int i=0;i<(XINT)args.GetSize();i++)
		   {
			  if(MakeURL(args[i],strURL)>0)
			  {
				XVar var(XEAB::LIST_IMAGE,
						(XINT)m_pDom->m_imgList.SetImage(strURL,XNULL,m_pDom));
 			    list.Add(var);
			  }
			  else {var=XELT::GetType(strCnt);list.Add(var);return;}
		   }

		}break;
	case CST_LISTTYPE:
		var=XELT::GetType(strCnt);
		break;
	case CST_MARGINALL:
		 GetSettings(var.nID,list,args,strCnt);
		 return;
		/*{
		   XString8 str(strCnt,STRING_ATTACH);
		   args.SplitString(str,' ');
		   str.EmptyNoFree();
		   if(args.GetSize()<1) return;
		   int nm[4];
		   
		   for(int i=0;i<(int)args.GetSize();i++)
		   {
			  int v=args[i].ToInt(10,XTRUE);
			  switch(i)
				{
				case 0:nm[0]=nm[1]=nm[2]=nm[3]=v;break;
				case 1:nm[1]=nm[3]=v;break;
				default:nm[i]=v;break;
				}
		   }
		   for(i=0;i<4;i++)
		   {
			   var.iData=nm[i];
			   list.Add(var);
			   var.nID++;
		   }
		}return;*/
	case CST_BORDERWIDTH:
		 var=XELT::GetBorderWidth(strCnt);
		 if(var.iData<0||var.iData>32) return;
		 break;
	case CST_BORDERSTYLE:
		 var=XELT::GetBorderType(strCnt);
		 break;
	case CST_BORDERALL:
		{
			XString8 str(strCnt,STRING_ATTACH);
			//XString8Array args;
			args.SplitStringS(str,", ");//' ');
			str.EmptyNoFree();
			if(args.GetSize()<=0) return;
			while(args.GetSize()>0)
			{
				XBOOL bOK=XFALSE;
				if(GetSettings(XEAB::BORDER_TOPWIDTH,list,args,XNULL)) bOK=XTRUE;
				if(GetSettings(XEAB::BORDER_TOPSTYLE,list,args,XNULL)) bOK=XTRUE;
				if(GetSettings(XEAB::BORDER_TOPCOLOR,list,args,XNULL)) bOK=XTRUE;
				if(!bOK) break;
			}
		}return;
	case CST_BORDERWIDTHS:
		/*{
			XU8 nWidths[4];
			XU8 bSet=XFALSE,bOK=XFALSE;
			if(args.GetSize()<=0)
			{
				XString8 str(strCnt,STRING_ATTACH);
				args.SplitString(str,' ');
				str.EmptyNoFree();
				bSet=XTRUE;
				if(args.GetSize()<=0) return;
			}
			for(XU32 i=0;i<4;i++)
			{
				if(i>=args.GetSize()) break;
				int v=XELT::GetBorderWidth(args[0]);
				if(v<0||v>32) break;
				switch(i)
				{
				case 0:nWidths[0]=nWidths[1]=nWidths[2]=nWidths[3]=(XU8)v;break;
				case 1:nWidths[1]=nWidths[3]=(XU8)v;break;
				default:nWidths[i]=(XU8)v;break;
				}
				bOK=XTRUE;
				//for(XU32 j=i;j<4-i;j++)
				//	nStyles[j]=(XU8)v;
				args.RemoveAt(0);
			}
			if(bOK)
			for(i=0;i<4;i++)
			{
				var.iData=nWidths[i];
				list.Add(var);
				var.nID++;
			}
			if(bSet||args.GetSize()<=0) return;
			else var.nID=XEAB::BORDER_TOPSTYLE;
		}*/
	case CST_BORDERSTYLES:
		/*{
			XU8 nStyles[4];
			XU8 bSet=XFALSE,bOK=XFALSE;
			if(args.GetSize()<=0)
			{
				XString8 str(strCnt,STRING_ATTACH);
				args.SplitString(str,' ');
				str.EmptyNoFree();
				bSet=XTRUE;
				if(args.GetSize()<=0) return;
			}
			for(XU32 i=0;i<4;i++)
			{
				if(i>=args.GetSize()) break;
				int v=XELT::GetBorderType(args[0]);
				if(v==XELT::LS_EMPTY) break;
				switch(i)
				{
				case 0:nStyles[0]=nStyles[1]=nStyles[2]=nStyles[3]=(XU8)v;break;
				case 1:nStyles[1]=nStyles[3]=(XU8)v;break;
				default:nStyles[i]=(XU8)v;break;
				}
				bOK=XTRUE;
				//for(XU32 j=i;j<4-i;j++)
				//	nStyles[j]=(XU8)v;
				args.RemoveAt(0);
			}
			if(bOK)
			for(i=0;i<4;i++)
			{
				var.iData=nStyles[i];
				list.Add(var);
				var.nID++;
			}
			if(bSet||args.GetSize()<=0) return;
			else var.nID=XEAB::BORDER_TOPCOLOR;
		}*/
	case CST_BORDERCOLORS:
		 GetSettings(var.nID,list,args,strCnt);
		 return;
		/*{
			XU32 nColors[4];
			XU8 bSet=XFALSE,bOK=XFALSE;
			if(args.GetSize()<=0)
			{
				XString8 str(strCnt,STRING_ATTACH);
				args.SplitString(str,' ');
				str.EmptyNoFree();
				bSet=XTRUE;
				if(args.GetSize()<=0) return;
			}
			for(XU32 i=0;i<4;i++)
			{
				if(i>=args.GetSize()) break;
				int v=XEnumColor().ColorOf(args[0],XTRUE);
				if(v==0) break;
				switch(i)
				{
				case 0:nColors[0]=nColors[1]=nColors[2]=nColors[3]=v;break;
				case 1:nColors[1]=nColors[3]=v;break;
				default:nColors[i]=v;break;
				}
				//for(XU32 j=i;j<4-i;j++)
				//	nStyles[j]=(XU8)v;
				args.RemoveAt(0);
				bOK=XTRUE;
			}
			if(bOK)
			for(i=0;i<4;i++)
			{
				var.iData=nColors[i];
				list.Add(var);
				var.nID++;
			}
			if(bSet||args.GetSize()<=0) return;
		}
	
		return;*/
	case CST_BORDERONE:
		{
			XString8 str(strCnt,STRING_ATTACH);
			args.SplitStringS(str,", ");//' ');
			str.EmptyNoFree();
			int id=0;
			while(id<(XINT)args.GetSize())
			{
				XU8 bOK=XFALSE;
				int v=XELT::GetBorderType(args[id]);
				if(v!=XELT::LS_EMPTY)
				{
					bOK=XTRUE;
					XVar var1(var.nID+4,v);
					list.Add(var1);
					id++;
				}
				if(id>=(XINT)args.GetSize()) break;
				v=XELT::GetBorderWidth(args[id]);
				if(v>=0&&v<32)
				{
					bOK=XTRUE;
					XVar var1(var.nID,v);
					list.Add(var1);
					id++;
				}
				if(id>=(XINT)args.GetSize()) break;
				v=XEnumColor().ColorOf(args[id],XTRUE);
				if(v!=0)
				{
					bOK=XTRUE;
					XVar var1(var.nID+8,v);
					list.Add(var1);
					id++;
				}

			}
			/*if(args.GetSize()<3) return;
			XU8 nWidth=XELT::GetBorderWidth(args[0]);
			if(nWidth>32) return;
			var.iData=nWidth;
			list.Add(var);
			var.nID+=4;
			nWidth=XELT::GetBorderType(args[1]);
			if(nWidth==XELT::LS_EMPTY) return;
			var.iData=nWidth;
			list.Add(var);
			var.nID+=4;
			var=XEnumColor().ColorOf(args[2],XTRUE);
			list.Add(var);*/
		}return;
	
	
	case CST_FONTSTYLE:
		 var=XELT::GetFontStyle(strCnt);
		 if(var.iData!=XELT::FS_EMPTY)
			 list.Add(var);
		 return;
	case CST_FONTWEIGHT:
		 var=XELT::GetFontWeight(strCnt);
		 if(var.iData!=XELT::FWT_EMPTY)
			 list.Add(var);
		 return;
	case CST_FONTALL:
		{
			XString8 str(strCnt,STRING_ATTACH),strURL;
			//XString8Array args;
			args.SplitStringS(str,", ");//' ');
			str.EmptyNoFree();
			//XU8 iOffset=0;
			for(XU32 i=0;i<args.GetSize();i++)
			{
			   int id=args[i].Find('/');
			   int v=XELT::GetFontStyle(args[i]);
			   if(i==0)
			   {
				   if(v!=XELT::FS_EMPTY)
				   {
					   XVar vv(XEAB::FONT_STYLE,v);
					   list.Add(vv);
				   }
			   }else if(i==1)
			   {
					v=XELT::GetFontWeight(args[i]);
					if(v!=XELT::FWT_EMPTY)
					{
						XVar vv(XEAB::FONT_WEIGHT,v);
						list.Add(vv);
					}
			   }
			   if(i==3||id>0)
			   {
				   XString8Array ss;
				   ss.SplitString(args[i],'/');
				   XVar vv(XEAB::FONT_SIZE,XELT::GetFontSize(ss[0]));
				   list.Add(vv);
				   if(ss.GetSize()>1)
				   {
					   v=ss[1].ToInt();
					   v=XMAX(12,XMIN(v,64));
					   XVar vv1(XEAB::FONT_HEIGHT,v);
					   list.Add(vv1);
				   }
				   return;
			   }
			}
		}return;
	case CST_FONTSIZE:
		 var=XELT::GetFontSize(strCnt);
		 break;
	case CST_DEC:
		 var=XELT::GetDecoration(strCnt);
		 break;
	case CST_TTRANS:
		 var=XELT::GetTransType(strCnt);
		 break;
	case CST_FLOAT:
		 var=XELT::GetFloatType(strCnt);
		 break;
	case CST_ATTACH:
		 var=(strCnt[0]=='f'||strCnt[0]=='F');
		 break;
	case CST_POSITION:
		//break;
		{
			XString8 str(strCnt,STRING_ATTACH);
			//XString8Array list;
			args.SplitStringS(str," ,");//' ');
			str.EmptyNoFree();
			MakePosition(list,args);			
			return;
		}
		 break;
	case CST_ALIGN:
		 var=XEA().Index((void*)strCnt);
		 break;
	case CST_BACKGROUND:
		 	//case CST_COLOR:
		 //var=XEnumColor().ColorOf(strCnt,XTRUE);
		 //break;
		{
			//var=XEnumColor().ColorOf(strCnt,XTRUE);
			//list.Add(var);
			//break;
			XString8 str(strCnt,STRING_ATTACH),strURL;
			XString8Array list1;
			args.SplitStringS(str," ,");//' ');
			str.EmptyNoFree();
			XEnumColor cc;
			for(XU32 i=0;i<args.GetSize();i++)
			{
				XU32 c=cc.Color(args[i],XTRUE);
				if(args[i].Find("trans")>=0)
				{
					var=0;
					list.Add(var);
				}
				else if(c!=0)
				{
				   //XVar var(XEAB::BGCOLOR,0xffff0000);//c);
				   var=c;
				   list.Add(var);
				   //return;
				}
				else if(MakeURL(args[i],strURL)>0)
				{
					if(pURL==XNULL)
					{
						XVar var(XEAB::BACKIMAGE,(XINT)m_pDom->m_imgList.SetImage(strURL,XNULL,m_pDom));
						list.Add(var);
					}
					else
					{
						XURL url=*pURL;
						url.SetURL(strURL);
						XVar var(XEAB::BACKIMAGE,(XINT)m_pDom->m_imgList.SetImage(url.m_strURL,XNULL,m_pDom));
						list.Add(var);
					}
				}
				else
				{
				   int id=XEnumAlign().Index(args[i]);
				   if(id>=0) 
				   {
					   XVar var(XEAB::REPEAT,id);
					   list.Add(var);
				   }
				   else
				   {
					   char c=args[i][0];
					   if(c=='f'||c=='F')
					   {
						   XVar var(XEAB::ATTACH,XTRUE);
						   list.Add(var);
					   }
					   else if(c!='S'&&c!='s')
						    list1.Add(args[i]);
				   }

				}
				if(list1.GetSize()>0)
				{
					MakePosition(list,list1);
					//XVar var(XEAB::POSITION,MakePosition(list1));
					//list.Add(var);
				}
			}
			
			return;
		}break;
		 //if(!bSet)
		 //	   var=XEnumColor().ColorOf(strCnt,XTRUE);
		 break;
	case CST_IMAGE:
		{
			//XU8 bSet=XFALSE;
			//XString8 str(strCnt,STRING_ATTACH);
			XString8 strURL;
			if(MakeURL(strCnt,strURL)>0)
			{
				if(pURL==XNULL)
					var=(XINT)m_pDom->m_imgList.SetImage(strURL,XNULL,m_pDom);
				else
				{
					XURL url=*pURL;
					url.SetURL(strURL);
					var=(XINT)m_pDom->m_imgList.SetImage(url.m_strURL,XNULL,m_pDom);
				}
			}
			//str.EmptyNoFree();
			
		};

		break;

	case CST_INT:
	case CST_PECENT:
		{
			//if(var.nID==XEAB::MARGIN_RIGHT)
			//	int a=0;
			//XLENGTH xlen;
			MakeInt(var,strCnt);
			/*if(bSet)
			{
				if(var.iValue<0) var.iValue=0;
				else var.iValue=-var.iValue;
			}*/
			//var.iData=xlen.iData;
			//if(strCnt
			/*XString8 str(strCnt);//,STRING_ATTACH);
			var=str.ToInt(10,nType==CST_PECENT);
			str.MakeLower();
			if(str.Find("em")>=0)
				var=(XINT)(var.iData|(1<<31));
			else if(str.Find("cm")>=0)
				var=(XINT)(var.iData*50);
			if(var.iData<-100) var.iData=-100;
			else if(var.iData>2048) var.iData=2048;*/
			//str.EmptyNoFree();
		}
		 break;
	case CST_COLOR:
		 var=XEnumColor().ColorOf(strCnt,XTRUE);
		 //if(var.iData==0)
		//	 int a=0;
		 break;
	case CST_STRING:
	case CST_URL:
		 var=strCnt;
		 break;
	}
	list.Add(var);
}


//DEL XINT XCSS::PhaseName(XString8 &strName,XString8&strClass)
//DEL {
//DEL    	int is=strName.Find(' ');
//DEL 	XINT id=0;
//DEL 	//XU8 bParent=XFALSE;
//DEL 	if(is>0)
//DEL 	{
//DEL 		int ie=strName.Find(':',is);
//DEL 		if(ie>0)
//DEL 		{
//DEL 			strClass.SetString(strName.GetData()+is+1,0);
//DEL 			strName[is]=0;
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			id=CreateID(strName.GetData()+is+1);
//DEL 			strName[is]=0;
//DEL 		}
//DEL 	}
//DEL 	id=id<<16;
//DEL 	int ie=strName.Find(':',0);
//DEL 	if(ie>0)
//DEL 		strClass.SetString(strName.GetData(),0);
//DEL 	else
//DEL 	//if(bParent)
//DEL 		id|=CreateID(strName);
//DEL 	//else
//DEL 	//	id=CreateID(strName);
//DEL 	if(id==XDOM_A)
//DEL 	{
//DEL 		id=0;
//DEL 		strClass=strName;
//DEL 	}
//DEL 	return id;
//DEL 	//else 
//DEL 
//DEL }

XINT XCSS::CreateID(XString8&strKey,XU16&nSubKey)
{
	//if(strKey==XNULL||strKey[0]==0) return -1;
	if(strKey=="*") return 0;
	int id=strKey.Find(':');
	if(id>=0)
	{
		//XString8 str(strKey.GetData()+id+1);
		int id0=id;
		id++;
		while(id<(XINT)strKey.GetLength()&&strKey[id]<=' ')
			id++;
		XString8 strTmp(strKey.GetData()+id,STRING_ATTACH);
		if(strTmp.Compare("before",XTRUE,6)==0||
		   strTmp.Compare("after",XTRUE,5)==0) 
		{
			strTmp.EmptyNoFree();
			return 0;
		}
		strTmp.EmptyNoFree();
		switch(strKey[id])
		{
		
		case 'l':
		case 'L':
			  nSubKey=STA_LINK;
			  break;
		
		case 'F':
		case 'f':nSubKey=STA_SELECTED;break;
		default:
		case 'v':
		case 'V':nSubKey=STA_SET;break;
		case 'h':
		case 'H':
		case 'a':
		case 'A':nSubKey=STA_ACTIVE;break;
		}
		//return XI_ATEXT;
		strKey.SetSize(id0);
		strKey.TrimRight();
	}
	//XString8 strCnt;

	XINT nToken=XCSS_TOKEN;
	id=strKey.Find('.');
	int iOffset=0;
	if(id>=0)//||m_bClass)
	{
		nToken=XCSS_CLASS;
		iOffset=id;
		//strKey.SetSize(id);
	}
	else if(m_bClass)
	{
		nToken=XCSS_CLASS;
		iOffset=0;
	}
	else
	{
		id=strKey.Find('#');
		if(id>=0)
		{
			nToken=XCSS_ID;
			iOffset=id+1;
		}
		else
		{
			id=strKey.Find('>');
			if(id>0) iOffset=id+1;
		}
	}
	//if(strKey[0]=='.') nToken=XCSS_CLASS;
	//else if(strKey[0]=='#') nToken=XCSS_ID;
	if(nToken)
	{
		if(nToken==XCSS_ID)
		{
			nToken<<=13;
			nToken|=(m_ids.CreateID(strKey.GetData()+iOffset)&0x1fff);
		}
		else
		{
			nToken<<=13;
			nToken|=(m_ids.CreateID(strKey.GetData()+iOffset)&0x1fff);
		}
	}
	else 
	{
		nToken=XDomPhaseXML::IndexOf(strKey.GetData()+iOffset,XSS_NORMAL)&0x1fff;
		if(nToken==XDOM_A)
		{
			//if(nSubKey==0)
			//	nToken=STA_LINK;
			//else nToken=0;
		}//*/
	}
	return nToken;
}



/*void XCSS::TransID(XU16 &id, XPCTSTR strClass)
{
   if(strClass==XNULL||strClass[0]==0) return;
}*/

XINT XCSS::CreateID(XPCTSTR strID, XU8 nClass)
{
   int nToken=m_ids.CreateID(strID)&0x1fff;
   nToken|=nClass<<13;
   return nToken;
}

//DEL XU32 XCSS::DoCSS(XDom *pDom,XBOOL bClear)
//DEL {
//DEL //	return 0;
//DEL    XU32 s=0;
//DEL    XU32 nCount=m_css.GetSize();
//DEL    for(XU32 i=0;i<nCount;i++)
//DEL    {
//DEL 	   XSelector*p=m_css[i];
//DEL 	   for(XU32 j=0;j<p->m_selectors.GetSize();j++)
//DEL 	   {
//DEL 		   XU16Array*pArray=p->m_selectors[j];
//DEL 		   //if(pArray->GetSize()<=0)
//DEL 		   pDom->SetAttribs(p->m_list,*pArray);
//DEL 
//DEL 	   }
//DEL 	   /*if(p->m_selectors.GetSize==0)
//DEL 		   pDom->SetAttribs(p->m_attribs);
//DEL 	   else
//DEL 	   {
//DEL 		   XDomList	list;
//DEL 		   pDom->m_doms.GetListByID(list,(XU16)(p->m_nSelector&0x7fff));
//DEL 		   if(list.GetSize()==0)
//DEL 			   pDom->SetAttribs(p->m_attribs);
//DEL 		   else
//DEL 		   {
//DEL 			   for(XU32 i=0;i<list.GetSize();i++)
//DEL 				   list[i]->SetAttribs(p->m_attribs,(XU16)(p->m_nSelector>>16),pDom);
//DEL 		   }
//DEL 	   }*/
//DEL 	  // p->m_nSelector;
//DEL    }
//DEL    if(bClear)
//DEL 	   m_css.FreeAll();
//DEL    return s;
//DEL }

int XCSS::MakeURL(XPCTSTR str, XString8 &strURL)
{
	XString8 strCnt(str,STRING_ATTACH);
	int is=strCnt.Find("url(");
	int ie=strCnt.Find(')',is+4);
	if(is>=0&&ie>is)
	{	int ir=ie+1;
		is+=3;
		//strCnt[ie]=0;
		int ii=is+1;
		while(ii<ie)
		{
			if(strCnt[ii]=='\''||
			   strCnt[ii]=='"')
			{
				is=ii;break;
			}
			ii++;
		}
		ii=ie-1;
		while(ii>is)
		{
			if(strCnt[ii]=='\''||
			   strCnt[ii]=='"')
			{
				ie=ii;break;
			}
			ii--;
		}
		if(ie>is+2)
		{
			strCnt.Mid(strURL,is+1,ie-is-1);
			strCnt.EmptyNoFree();
			return ir;
			//bSet=XTRUE;
		}
	}
	strCnt.EmptyNoFree();
	return 0;

}

void XCSS::MakePosition(XVarList&vars,XString8Array &list)
{
	//if(list.GetSize()>0)
	if(list.GetSize()<=0) return;
	for(int i=0;i<2;i++)
	{
		XVar var(XEAB::POSITIONX+i,0);
		var.iValue=-50;
		if(i<(XINT)list.GetSize())
		{
			int id=XEA().Index(list[i]);
			switch(id)
			{
				case XEA::LEFT:var.iValue=0;break;
				case XEA::CENTER:var.iValue=-50;break;
				case XEA::RIGHT:var.iValue=-100;break;
				default:
					 MakeInt(var,list[i]);
			}	
		}
		vars.Add(var);
		/*short x=list[0].ToInt(10,XTRUE);
		short y=list.GetSize()>1?list[1].ToInt(10,XTRUE):-50;
		XEA ea;
		int id=ea.Index(list[0]);
		switch(id)
		{
		case XEA::LEFT:x=0;break;
		case XEA::CENTER:x=-50;break;
		case XEA::RIGHT:x=-100;break;
		}
		if(list.GetSize()>1)
		{
			id=ea.Index(list[1]);
			switch(id)
			{
			case XEA::TOP:y=0;break;
			case XEA::CENTER:y=-50;break;
			case XEA::BOTTOM:y=-100;break;
			}
		}*/
		
		//return ((XU16)y<<16)|(XU16)x;
	}
	//return 0;
}

XU8 XCSS::PhaseSelector(XU16Array &list, XString8 &strNames)
{	
	XString8Array names;
	names.SplitStringS(strNames," ,");//' ');
	for(XU32 i=0;i<names.GetSize();i++)
	{
		XU16 nSubKey=0;
		int v=CreateID(names[i],nSubKey);
		if(v) list.Add(v);
		//if(nSubKey) list.Add(nSubKey);
		if(nSubKey) return (XU8)nSubKey;
	}
	return 0;
}

void XCSS::RemoveEcho(XString8 &strCss)
{
	XString8 str(strCss.DataTo(),STRING_ATTACH);
	XPCTSTR pData=str;
	XU8 nRemove=0,nQuo=0;
	strCss.SetSize(1);
	strCss[0]=0;
	for(int i=0;i<(XINT)str.GetLength();i++)
	{
		if(!nQuo)
		{
			if(pData[i]=='/')
			{
				if(nRemove==1)
				{
					if(pData[i-1]=='*') {nRemove=0;continue;}
				}
				else if(pData[i+1]=='*')
					{nRemove=1;i++;continue;}
				//else if(pData[i+1]=='/') 
				//	{nRemove=2;i++;continue;}
			}
			if(nRemove==2)
			{
				if(pData[i]==XCR||
					pData[i]==XCT) {nRemove=0;continue;}
			}
		}
		//else
		{	
			switch(pData[i])
			{
			case '\'':if(nQuo==0) nQuo=1;
					  else if(nQuo==1) nQuo=0;
					  break;
			case '"':if(nQuo==0) nQuo=2;
				     else if(nQuo==2) nQuo=0;
			}
			if(!nRemove&&pData[i]>=' ') 
				strCss+=pData[i];
		}
	}
	//str.EmptyNoFree();
}

void XCSS::GetAttribs(XDomItem *pItem, XVARLIST &vars)
{
	 XU16Array entries,parents;
	 pItem->GetEntries(entries);
	 //XSortU16 sort(&parents);
	 pItem=pItem->GetParent();
	 while(pItem!=XNULL)
	 {
		 pItem->GetEntries(parents);
		 pItem=pItem->GetParent();
	 }

	 //if(m_css.m_entries.GetSize()>0&&
	//	m_css.m_entries[0]->m_nSelector==0)
	//	m_css.m_entries[0]->GetAttribs(vars,sort);
	 for(XU32 i=0;i<entries.GetSize();i++)
	 {
		 int id=m_css.Index((void*)entries[i]);
		 if(id<0) continue;
		 m_css.m_entries[id]->GetAttribs(vars,parents,0);
	 }
}


void XCSS::XSelector::AddAttrib(XVar&var,XU8 nStatus)
{
		if(nStatus&&!IsStatusID(var.nID)) 
			nStatus=0;
		XSortVar sort(&m_attribs);
		int mi;
		int idd=var.nID|(nStatus<<14);
		int id=sort.Index((void*)idd,mi);
		if(id>=0) {*m_attribs[id]=var;m_attribs[id]->nID=idd;}
		else
		{
			XVar*p=new XVar;
			*p=var;
			p->nID=idd;
			m_attribs.InsertAt(mi,p);
		}
}

void XCSS::XSelector::GetAttribs(XVARLIST &vars,XU16Array&parents,int nOffset)//XSortU16&parents)
 {
	XU32 i;
 	for(i=0;i<m_attribs.GetSize();i++)
	{
		XSortVar sort(&vars);
		int mi;
		int id=sort.Index((void*)m_attribs[i]->nID,mi);
		if(id<0) vars.InsertAt(mi,m_attribs[i]);
		else vars[id]=m_attribs[i];
	}
	if(m_entries.GetSize()<=0||(XINT)parents.GetSize()<=nOffset) return;//GetCount()<=0) return;
	for(i=0;i<m_entries.GetSize();i++)
	{
		//if(parents.Index(m_entries[i]->m_nToken)<0) continue;
		for(int j=nOffset;j<(XINT)parents.GetSize();j++)
		{
		  if(parents[j]==m_entries[i]->m_nToken)
		  {
			  m_entries[i]->GetAttribs(vars,parents,j+1);
			  break;
		  }
		}
		//m_entries[i]->GetAttribs(vars,parents);
	}
 }

void XCSS::DoCSS(XDom *pDom, XU8 bClear)
{
	if(m_css.m_entries.GetSize()<=0) return;
}


XBOOL XCSS::GetSettings(XU16 ID,XVarList &list, XString8Array &args,XPCTSTR strCnt)
{
	XINT i;
	if(strCnt!=XNULL)
	{
		XString8 str(strCnt,STRING_ATTACH);
		args.SplitStringS(str," ,");//' ');
		str.EmptyNoFree();
	}
	if(args.GetSize()<=0) return XFALSE;
	int nWidths[4],bOK=XFALSE;
	for(i=0;i<4;i++)
	{
		//if(i>=args.GetSize()) break;
		if(args.GetSize()<=0) break;
		int v=0;
		XU8 bSet=XTRUE;
		switch(ID)
		{
		case XEAB::PADDING_TOP:
			 v=args[0].ToInt(10,XTRUE);
			 if(v<0) bSet=XFALSE;
			 break;
		case XEAB::BORDER_TOPWIDTH:
			 v=XELT::GetBorderWidth(args[0]);
			 if(v<0||v>32) bSet=XFALSE;
			 break;
		case XEAB::BORDER_TOPSTYLE:
			 v=XELT::GetBorderType(args[0]);
			 if(v==XELT::LS_EMPTY) bSet=XFALSE;
			 break;
		case XEAB::BORDER_TOPCOLOR:
			 v=XEnumColor().ColorOf(args[0],XTRUE);
			 if(v==0) bSet=XFALSE;
			 break;
		case XEAB::MARGIN_TOP:
			 v=args[0].ToInt(10,XTRUE);
			 bSet=XTRUE;
			 break;
		}
		if(!bSet) break;		
		switch(i)
		{
			case 0:nWidths[0]=nWidths[1]=nWidths[2]=nWidths[3]=v;break;
			case 1:nWidths[1]=nWidths[3]=v;break;
			default:nWidths[i]=v;break;
		}
		bOK=XTRUE;
		args.RemoveAt(0);
	}
	if(bOK)
	for(i=0;i<4;i++)
	{
		XVar var(ID+i,0);
		if(nWidths[i]<0&&ID!=XEAB::BORDER_TOPCOLOR)
		{
			var.iValue=-nWidths[i];
			var.iSign=1;
		}
		else var.iValue=nWidths[i];
		//var.iData=nWidths[i];
		list.Add(var);
		var.nID++;
	}
	return bOK;
}

void XCSS::MakeInt(XVar &var, XPCTSTR strCnt)
{
	var.iUnit=0;
	var.iSign=0;
	//XU8 bSet=XFALSE;
	while(*strCnt<=' '&&*strCnt!=0) strCnt++;
	if(*strCnt=='-') {strCnt++;var.iSign=1;}
	XString8 str(strCnt);
	var.iValue=str.ToInt(10,XTRUE);
	if(str.Find("em")>=0)
		var.iUnit=1;
	else if(str.Find("cm")>=0)
		var.iValue=var.iValue*50;
}
