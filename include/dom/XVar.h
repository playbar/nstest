// XVar.h: interface for the XVar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XVAR_H__A1E05A83_B7E9_476C_9090_F5A617BBA14C__INCLUDED_)
#define AFX_XVAR_H__A1E05A83_B7E9_476C_9090_F5A617BBA14C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*typedef struct _XLENGTH
{  union
	{
		XS64 iData;
        struct {XINT iUnit,iValue;};
	};
}XLENGTH;*/

enum {VLOGIC,VINT,VFLOAT,VSTRING,VERROR};
class _XEXT_CLASS XVar
{
protected:
	void FreeBuf()
	{
		if(nType==VSTRING) 
			delete strData;
	}
public:
#pragma pack(1)
	XU8	nType;
	XU16 nID;
	union
	{
	   struct {XINT iValue;XU16 iUnit,iSign;};
	   XS64 iData;	   
	   XF64 fData;
	   XPTSTR strData;
	};
	//XString8 strData;
#pragma pack()	

public:
	XPCTSTR DataTo()//XString8&str)
	{
		if(nType!=VSTRING) return XNULL;
		//str.SetString(strData,STRING_ATTACH);
		XPCTSTR str=strData;
		nType=VINT;
		strData=XNULL;
		return str;
	}
	operator XS64(){ToInt();return iData;}
	operator XINT(){ToInt();return (XINT)iData;}
	operator XF64(){ToFloat();return fData;}
	operator XPCTSTR(){ToString();return strData;}
	XVar operator+(XVar&v)
	{
		XVar r(*this);
		r+=v;
		return r;
	}
	void operator+=(XVar&v)
	{
	   XU8 t=XMAX(nType,v.nType);
	   ToType(t);
	   XVar v1(v);
	   switch(t)
	   {
	   case VLOGIC:
	   case VINT:iData+=v1.iData;break;
	   case VFLOAT:fData+=v1.fData;break;
	   case VSTRING:
		   {
			   XString8 str(strData,STRING_ATTACH);
			   str+=v1.strData;
			   strData=str.DataTo();
		   }break;		    
	   }
	}
	XVar operator-(XVar&v)
	{
		XVar r(*this);
		r-=v;
		return r;
	}
	void operator-=(XVar&v)
	{
	   XU8 t=XMAX(nType,v.nType);
	   ToType(t);
	   XVar v1(v);
	   switch(t)
	   {
	   case VLOGIC:
	   case VINT:iData-=v1.iData;break;
	   case VFLOAT:fData-=v1.fData;break;
	   case VSTRING:
		   {   int s=strcmp(strData,v1.strData);
			   ToInt();
			   iData=s;
		   }break;
		    
	   }
	}

	~XVar()
	{
		FreeBuf();
	}

	XVar(XINT id,XINT v)
	{
		nType=VINT;
		nID=(XU16)id;
		iData=v;
	}

	XVar(){nType=VINT;fData=0;}
	XVar(const XVar&v)
	{
		nType=v.nType;
		iData=v.iData;
		nID=v.nID;
		if(nType==VSTRING)
			strData=XString8(strData).DataTo();
	}
	void From(XINT id,XString8&str)
	{
		nType=VSTRING;
		strData=str.GetData();
		nID=(XU16)id;
	}
	XVar(XINT id,XPCTSTR val)
	{
		nType=VSTRING;
		XString8 s(val);
		strData=s.DataTo();
		nID=(XU16)id;
	}
	/*XVar(XINT id,XINT v)
	{
		nType=VINT;
		nID=id;
		iData=v;
	}*/
	/*XVar(XINT v){XVar(0,v);}
	XVar(XF64 v){XVar(0,v);}
	XVar(XPCTSTR v){XVar(0,v);}*/
	XVar(XINT id,XF64 v)
	{
		nType=VFLOAT;
		nID=(XU16)id;
		fData=v;
	}
	XBOOL operator==(const XVar&src)
	{
		ToType(src.nType);
		switch(nType)
		{
		case VSTRING:
			 return strcmp(strData,src.strData)==0;
		default:
			 return iData==src.iData;
		}
		return XFALSE;
	}
	void operator=(const XVar&src)
	{
		FreeBuf();
		nType=src.nType;
		fData=src.fData;
		nID=src.nID;
		if(nType==VSTRING)
		{
			strData=XString8(src.strData).DataTo();
		}
	}
	void operator=(XF64 v)
	{
		FreeBuf();
		nType=VFLOAT;
		fData=v;
	}
	void operator=(int data)
	{
		FreeBuf();
		nType=VINT;
		iData=data;
	}
	void operator=(XU32 data)
	{
		FreeBuf();
		nType=VINT;
		iData=data;
	}
	void operator=(XPCTSTR str)
	{
		FreeBuf();
		nType=VSTRING;
		XString8 s(str);
		strData=s.DataTo();
	}
	void operator=(XU8Array&data)
	{
		FreeBuf();
		nType=VSTRING;
		XString8 s((XPCTSTR)data.GetData(),data.GetSize());
		strData=s.DataTo();
	}
	void operator=(XPCWSTR str)
	{
		FreeBuf();
		nType=VSTRING;
		XString8 s(str);
		strData=s.DataTo();
	}
	
	void ToType(XU8 t)
	{
		switch(t)
		{
		case VINT:ToInt();break;
		case VLOGIC:ToLogic();break;
		case VFLOAT:ToFloat();break;
		default:ToString();
		}
	}
	XPCTSTR ToString();

	XF64 ToFloat()
	{
		switch(nType)
		{
		case VLOGIC:
		case VINT:
			 fData=(XF64)iData;
			 break;
		case VSTRING:
			 fData=XString8(strData,STRING_ATTACH).ToFloat();
			 break;
		}
		nType=VFLOAT;
		return fData;
	}
	XS64 ToInt()
	{		
		switch(nType)
		{
		case VFLOAT:
			 iData=(XINT)fData;
			 break;
		case VSTRING:
			 iData=XString8(strData,STRING_ATTACH).ToInt();
			 break;
		}
		nType=VINT;
		return iData;
	}
	XINT ToLogic()
	{
		switch(nType)
		{
		case VFLOAT:
			 iData=fData!=0;
			 break;
		case VINT:
			 iData=iData!=0;
			 break;
		case VSTRING:
			 iData=XString8(strData,STRING_ATTACH).ToLogic();
			 break;
		}
		nType=VLOGIC;
		return (XBOOL)iData;
	}
};



typedef XArray<XVar>XVarList;
typedef XArray<XVar*>XVARLIST;

class XSortVar:public XSort
{
public:
	XSortVar(XVARLIST*p)
	{
		m_pList=p;
	}
	XU32 GetCount(){return m_pList->GetSize();}
	void* GetData(XU32 id){return (void*)(*m_pList)[id]->nID;}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareInt((XS32)iSrt,(XS32)iDst);
	}
public:
	XVARLIST* m_pList;
};

#endif // !defined(AFX_XVAR_H__A1E05A83_B7E9_476C_9090_F5A617BBA14C__INCLUDED_)
