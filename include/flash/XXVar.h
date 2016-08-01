// XXVar.h: interface for the XXVar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXVAR_H__6B8AC3EE_D47E_4646_94C8_ABEA8CEBBA39__INCLUDED_)
#define AFX_XXVAR_H__6B8AC3EE_D47E_4646_94C8_ABEA8CEBBA39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define _XXKEY

#include "flashgeom.h"
#include "XSort.h"

const XPCTSTR _strType[]={_XXKEY("type"),_XXKEY("object")};
const XPCTSTR _strConstType[]={_XXKEY("undefined"),_XXKEY("null")
							  ,_XXKEY("Infinity"),_XXKEY("-Infinity"),_XXKEY("NaN")};
const XPCTSTR _strTypeString[]={_XXKEY("const"),_XXKEY("boolean"),
								_XXKEY("number"),_XXKEY("number"),
								_XXKEY("string"),
								_XXKEY("object"),
								_XXKEY("function"),_XXKEY("movieclip")};

enum {XOCT_UNDEFINE=0,XOCT_NULL,XOCT_INFINITY,XOCT_RINFINITY,XOCT_NAN};
enum {XOOT_TYPE=0, XOOT_OBJECT};
enum {XODT_CONST=0,XODT_BOOLEAN,XODT_INT,XODT_FLOAT,XODT_STRING,XODT_OBJECT,XODT_FUNCTION,XODT_MOVIECLIP};
//enum {XSTR_DATA,XSTR_CONST,XSTR_ATTACH};
class XXObject;

#include "flashglobal.h"

const XU8 _XAllocTab[]={0,
						1,
						2,2,
						3,3,3,3,
						4,4,4,4,4,4,4,4,
						5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
						6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6};
//#define _VARCACHE_
class XXVar  
{	  
public:
	inline static void* _AllocData(int nSize)
	{
		nSize+=sizeof(XU32);
		XU32*data=(XU32*)XXVar::AllocData(nSize);
		if( data == NULL )
		{
			LOGWHERE();
			return NULL;
		}
		data[0]=nSize;
		return data+1;
	}
	inline static void _FreeData(void*p)
	{
		if(!p) return;
		XU32*data=((XU32*)p)-1;
		XXVar::FreeData(data,data[0]);
	}
	inline XSTRINGDATA* GetData()const
	{
		return ((XSTRINGDATA*)strTxt)-1;
	}
	inline XBOOL IsConstString(){return nStringType==STRING_CONST&&nType==XODT_STRING;}
public:
	static void ReleaseData()
	{
		alloc64.ReleaseData();
		alloc128.ReleaseData();
		alloc256.ReleaseData();
		alloc512.ReleaseData();
		alloc1024.ReleaseData();
		alloc2048.ReleaseData();
		alloc4096.ReleaseData();
		alloc16384.ReleaseData();
	}
	static XFastAlloc alloc64;  //0
	static XFastAlloc alloc128; //1
	static XFastAlloc alloc256; //2
	static XFastAlloc alloc512; //3
	static XFastAlloc alloc1024;//4
	static XFastAlloc alloc2048; //5
	static XFastAlloc alloc4096; //6
	static XFastAlloc alloc16384;
    

	static void* AllocData(int& nSize)
	{
		if(nSize<=4096)
		{
			switch(_XAllocTab[(nSize-1)>>6])
			{
			case 0://if(nSize<=64)
					nSize=64;
					return alloc64.Alloc();
			case 1://else if(nSize<=128)
					nSize=128;
					return alloc128.Alloc();
			case 2://else if(nSize<=256)
					nSize=256;
					return alloc256.Alloc();
			case 3://else if(nSize<512)
				nSize=512;
				return alloc512.Alloc();
			case 4://1024
					nSize=1024;
					return alloc1024.Alloc();
			case 5://2048
					nSize=2048;
					return alloc2048.Alloc();
			case 6://4096
					nSize=4096;
					return alloc4096.Alloc();
				
			}
		}
		else 
		{
			if(nSize<=16384)
			{
				nSize=16384;
				return alloc16384.Alloc();
			}
		}
        return new XU8[nSize];
	}
	static void FreeData(void*pData,int nSize)
	{
		if(nSize == 0)
			return;
		switch(nSize)
		{
		//case 1:
		//	delete (XU8*)pData;
		//	break;
		case 64:
                alloc64.Free(pData);
                break;
		case 128:
                alloc128.Free(pData);
                break;
		case 256:
                alloc256.Free(pData);
                break;
		case 512:
                alloc512.Free(pData);
                break;
		case 1024:
                alloc1024.Free(pData);
                break;
		case 2048:
                alloc2048.Free(pData);
                break;
		case 4096:
                alloc4096.Free(pData);
                break;
		case 16384:
                alloc16384.Free(pData);
                break;
        default:
                delete [] (XU8*)pData;
                return;
		}
	}
#pragma pack(1)
	union
	{
		XS32     iData32;
		XFDOUBLE fData;
		XPTSTR   strTxt;
		XXObject*pObject;
	};
	XU8  nType,nObject;
	XS16 nStringType;
    
#ifdef _VARCACHE_
	enum {XINVALID_REF=0xffff};
	XU16 nRefID;//,nConstID;
#endif
	//XString8* strDebug;
#pragma pack()
public:
	XBOOL IsSysCall(){return nType==XODT_FUNCTION;}
	void AddWChar(XU16 c)
	{
		 if(c<0x80)
			AddChar((char)(c&0x7f));
		 else if(c<0x800)
		 {
			AddChar((char)((c>>6)|0xc0));
			AddChar((char)((c&0x3f)|0x80));
		 }
		 else
		 {
			AddChar((char)((c>>12)|0xe0));
			AddChar((char)(((c>>6)&0x3f)|0x80));
			AddChar((char)((c&0x3f)|0x80));
		 }
	}
	void AppendInt(int i)
	{
		ToString(XTRUE);
		if (!i) {
			AddChar('0');
			//AppendChar('0');
		} else {
			int n = 0;
			char buf[10];
			while (i > 0) {
				buf[n++] = (i % 10)+'0';
				i /= 10;
			}
			char* c = buf+n;
			while ( n-- ) {
				AddChar(*--c);
				//AppendChar(*--c);
			}
		}

	}
	void Mid(XXVar&str,int nIndex,int nSize)
	{
		int len=GetLength();
		if(nIndex<0) nIndex=0;
		if(nSize<=0) nSize=len-nIndex-1;
	 	if(nIndex>=len) {str.SetString(XNULL,0);return;}
		str.SetString(strTxt+nIndex,nSize);
		//if(nSize<=0) return XString8("");
		//str.SetString(m_pData+nIndex,nSize);
	}
	void AddChar(char c)
	{
		int l=GetData()->nLength;
		SetLength(l+1);
		strTxt[l]=c;
	}
	XBOOL SetString(XPCTSTR strBuf,int l);	
	static XPTSTR StringIndex(XPTSTR str,int index);
	static XBOOL IsMultiByte();
	static XBOOL IsLeadByte(XU8 ch);
	XBOOL IsObject(XU8 id) const;
	inline XBOOL IsNull() const
	{
		switch(nType)
		{
		case XODT_CONST:
			 if(iData32==XOCT_NULL) return XTRUE;
			 break;
		//case XODT_CONST_STRING:
		case XODT_STRING:
			 if(!strTxt||!strTxt[0]) return XTRUE;
			 break;
		case XODT_FUNCTION:
		case XODT_OBJECT:
			 if(pObject==XNULL) return XTRUE;
			 break;
		}
		return XFALSE;
	}
	int GetLength()const
	{
		switch(nStringType)
		{
		case STRING_ATTACH:
		case STRING_CONST:
			 return XString8::SafeStrlen(strTxt);
		default:
			 return GetData()->nLength;
		}
	}
	/*XPTSTR StringTo()
	{
		nType=XODT_INT;
		return strData;
	}*/
	XXVar(XU8 t,int iValue)
	{
		nType=t;
		iData32=iValue;
	}
	inline XPCTSTR TypeOf()
	{
		int t=GetObjectType();
		switch(t)
		{
		case XODT_CONST:
			 return _strConstType[ToInt()];
		/*case XODT_FUNCTION:
			 if(nObject)
			 {
				 return _strTypeString[XODT_OBJECT];
				 break;
			 }*/
			 //break;
		default:
			 return _strTypeString[t];
		}
	}
	
	inline void FToFixed(int v)
	{
		v=(int)(fData*v);
		iData32=v;
		nType=XODT_INT;
	}
	inline void IToFloat(int v)
	{
		XFDOUBLE f=(XFDOUBLE)iData32/v;
		fData=f;
		nType=XODT_FLOAT;
	}

	inline static XXVar* CreateBool(XU8 b)
	{
		XXVar*pVar=new XXVar;
		pVar->ToLogic();
		pVar->iData32=b;
		return pVar;
	}
	inline static XXVar* CreateConst(XU8 c)
	{
		XXVar*pVar=new XXVar;
		pVar->ToConst(c);
		return pVar;
	}
	//void SetContext(XXObject*pObj);
	XU8 GetDataType()const;
	XU8 GetObjectType()const;
	/*inline void operator=(XPCTSTR str)
	{		
		if(str==XNULL)
		{
			SetString("",0);
		}
		else
		{
			Release();
			nType=XODT_STRING;
			nStringType=STRING_CONST;
			strTxt=(XPTSTR)str;
		}
	}*/
	
	inline void operator=(XFDOUBLE v)
	{
		Release();
		nType=XODT_FLOAT;
		fData=v;
	}
	inline void operator=(XU32 v)
	{
		Release();
		nType=XODT_INT;
		iData32=v;
	}
	inline void operator=(XS32 v)
	{
		Release();
		nType = XODT_INT;
		iData32 = v;
	}
	void operator=(XXObject*pObj);
	inline void operator=(int v)
	{
		Release();
		nType=XODT_INT;
		iData32=v;
	}
	//void operator=(const XXVar&var);
	void AttachString(XPTSTR str)
	{
		Release();
		nType=XODT_STRING;
		nStringType=STRING_ATTACH;
		strTxt=str;
	}
	inline XXVar(const XXVar&v){Set(v);}
	XXVar(XXObject*pObj);
	/*XXVar(XString8&str,XU8 bAttach)
	{
		if(!bAttach) 
			XXVar(str);
		else
		{
			nType=XODT_STRING;
			strData=str.DataTo();
		}
	}*/
#ifdef _VARCACHE_
	inline void SetConst(XPCTSTR str,XU16 iRef)//,XU16 iConst)
	{
	   Release();
	   nType=XODT_STRING;
	   nStringType=STRING_CONST;
	   strTxt=(XPTSTR)str;
	   nRefID=iRef;
//	   nConstID=iConst;
	}
#endif
	inline void Set(XPCTSTR str)
	{
	    //Release();
		nType=XODT_STRING;
		nStringType=STRING_CONST;
		strTxt=(XPTSTR)str;
#ifdef _VARCACHE_
		nRefID=XINVALID_REF;
//		nConstID=XINVALID_REF;
#endif
	}
	inline void operator=(XPCTSTR str)
	{
		Release();
		Set(str);
		//if(str==XNULL)
		//	strData=XString8("").DataTo();
		//else
		//	strData=XString8(str).DataTo();
	}
	inline void operator=(const XXVar&var){Release();Set(var);}
	XXVar(XPCTSTR str)
	{
		Set(str);
	}
	XXVar(XINT v)
	{
		nType=XODT_INT;
		iData32=v;
	}
	XXVar(XU8 v)
	{
		nType=XODT_INT;
		iData32=v;
	}
	XXVar(XU16 v)
	{
		nType=XODT_INT;
		iData32=v;
	}
	XXVar(XS16 v)
	{
		nType=XODT_INT;
		iData32=v;
	}
	XXVar(XS8 v)
	{
		nType=XODT_INT;
		iData32=v;
	}
	XXVar(XU32 v)
	{
		nType=XODT_INT;
		iData32=v;
	}
	XXVar(XS32 v)
	{
		nType=XODT_INT;
		iData32=v;
	}
	XXVar(XFDOUBLE v)
	{
		nType=XODT_FLOAT;
		fData=v;
	}
	/*XXVar(float v)
	{
		nType=XODT_FLOAT;
		fData=v;
	}*/
	XXVar()
	{
		nType=XODT_CONST;
		iData32=XOCT_UNDEFINE;
	}
	XFDOUBLE ToFloat();
	int  ToInt();
	inline XBOOL ToFunction(int id)
	{
		if(!IsObject()) return XFALSE;
		nType=XODT_FUNCTION;
		nObject=0;
		nStringType=(XS16)id;
		return XTRUE;
	}
	inline XFDOUBLE ToNum()
	{
		switch(nType)
		{
		case XODT_FLOAT:return fData;
		case XODT_INT:return (XFDOUBLE)iData32;
		default:return ToFloat();
			break;
		}
	}
	inline void ToIntIf();
	XBOOL ToLogic();
	inline void ToConst(XS32 i)
	{
		Release();
		nType=XODT_CONST;
		iData32=i;		
	}
	void ToString(XBOOL bForce);
	//XBOOL IsClass(){return nType==XSV_CLASS;}
	inline XBOOL IsObject() const
	{
		switch(nType)
		{
		case XODT_MOVIECLIP:
		case XODT_FUNCTION:
		case XODT_OBJECT:return XTRUE;
		}
		return XFALSE;
	}
	inline XBOOL IsFunction()const
	{
		if(!IsObject()) return XFALSE;
		return GetDataType()==XODT_FUNCTION;
	}
	inline XBOOL IsFunction2()
	{
		return IsObject();
		//return nType==XODT_OBJECT;
	}
	inline XBOOL IsString()
	{
		return nType==XODT_STRING;
		/*switch(nType)
		{
		case XODT_STRING:
		case XODT_CONST_STRING:
			 return XTRUE;
		}
		return XFALSE;*/
	}
	inline XBOOL IsString2()
	{
		return GetDataType()==XODT_STRING;
		/*{
		case XODT_STRING:
		case XODT_CONST_STRING:
			 return XTRUE;
		}
		return XFALSE;*/
	}
	inline XBOOL IsMovieClip()
	{
		//if(nType!=XODT_OBJECT) return XFALSE;
		return GetDataType()==XODT_MOVIECLIP;
	}
	inline XBOOL IsUndefine()
	{
		/*if(GetDataType()!=XODT_CONST) return XFALSE;
		switch(nType)
		{
		case XODT_FUNCTION:
			 return XFALSE;
		case XODT_OBJECT:
			 ToInt();
		case XODT_CONST:
			 return iData32==XOCT_UNDEFINE;
//		case XODT_OBJECT:
//			 ToInt();
		}
		return XFALSE;*/
		return (nType==XODT_CONST&&iData32==XOCT_UNDEFINE);
	}
	/*XBOOL IsNull()
	{
		if(GetDataType()!=XODT_CONST) return XFALSE;
		switch(nType)
		{
		case XODT_OBJECT:
			 ToInt();
		case XODT_CONST:
			 return iData32==XOCT_NULL;
		case XODT_OBJECT:
			 ToInt();
		}
		return XFALSE;
	}*/
	inline XBOOL IsFinite()
	{
		switch(GetDataType())
		{
		case XODT_CONST:
			 switch(nType)
			 {
			 case XODT_CONST:
				  return iData32==XOCT_INFINITY||
					     iData32==XOCT_RINFINITY;
			 case XODT_FUNCTION:
				  return XFALSE;
			 case XODT_OBJECT:
				  ToInt();
				  return iData32==XOCT_INFINITY||
					     iData32==XOCT_RINFINITY;
			 }break;
		default:
			return XFALSE;
		}
		return XFALSE;
	}
	inline XBOOL IsNum()
	{
		switch(nType)
		{
		case XODT_INT:
		case XODT_FLOAT:return XTRUE;
		}
		return XFALSE;
	}
	XBOOL IsNum3();
	/*XBOOL IsNaN2()
	{
		if(GetDataType()!=XODT_CONST) return XFALSE;
		switch(nType)
		{
		case XODT_OBJECT:
			 ToInt();
		default:
			 switch(iData32)
			 {
			 case XOCT_UNDEFINE:
			 case XOCT_
			 }
		}
	}*/
	inline XBOOL IsNaN()
	{
		return !IsNum3();
		/*switch(GetDataType())
		{
		case XODT_OBJECT:
			 return XTRUE;
		case XODT_STRING:
			 //return ToInt()==0;
			{
				if(nType==XODT_OBJECT)
				   return !pObject->IsNum3();
				//ToString();
			}
			 break;
		default:
			 return XTRUE;
		}*/
	}
	inline XBOOL IsNum2()
	{
		switch(GetDataType())
		{
		 case XODT_INT:
		 case XODT_FLOAT:return XTRUE;
		}
		return XFALSE;
	}
	inline void ToType(XU8 t,XBOOL b)
	{
		switch(t)
		{
		case XODT_CONST:
			 ToConst(0);
			 break;
		case XODT_INT:ToInt();break;
		case XODT_BOOLEAN:ToLogic();break;
		case XODT_FLOAT:ToFloat();break;
		default:ToString(b);
		}
	}

	~XXVar()
	{
		Release();
	}
	//////////////////////////////////////////////////////
	//void operator==(constXXVar&v)
	void EQ2(XXVar&v);
	void StrictEQ(XXVar&v)
	{
		XU8 bEQ;
		if(IsNum()&&v.IsNum())
		{
			EQ(v);
			return;
		}
		if(nType!=v.nType) bEQ=XFALSE;
		switch(nType)
		{
		case XODT_STRING:
			 StringEQ(v);
			 return;
		case XODT_FLOAT:
			 bEQ=fData==v.fData;
			 nType=XODT_BOOLEAN;
			 iData32=bEQ;
			 break;
		default:
			 bEQ=iData32==v.iData32;
			 //Release();
			 Release();
			 nType=XODT_BOOLEAN;
			 iData32=bEQ;
		}
	}
	void EQ(XXVar&v);
	//void operator<(XXVar&v)
	inline void Greater(XXVar&v)
	{
		/*if(nType==XODT_STRING||
		   v.nType==XODT_STRING)
		   StringGreater(v);
		else
		{
			ToFloat();
			v.ToFloat();
			nType=XODT_BOOLEAN;
			iData32=fData>v.fData;
		   //Less(v);
		}*/
		v.Less2(*this);
		*this=v;
		//Less2(v);
		//iData32=!iData32;
	}
	void Less2(XXVar&v);
	/*{
		if(nType==XODT_STRING||
		   v.nType==XODT_STRING)
		   StringLess(v);
		else
		   Less(v);
	}*/
	inline void Less(XXVar&v)
	{
		//if(nType==XSV_
		ToFloat();
		v.ToFloat();
		nType=XODT_BOOLEAN;
		iData32=fData<v.fData;
	}
	//void operator&&(XXVar&v)
	inline void And(XXVar&v)
	{
		
		ToLogic();
		v.ToLogic();
		iData32=iData32&&v.iData32;
	}
	//void operator!()
	inline void Not()
	{
		ToLogic();
		iData32=!iData32;
	}
	//void operator||(XXVar&v)
	inline void Or(XXVar&v)
	{
		ToLogic();
		v.ToLogic();
		iData32=iData32||v.iData32;
	}
	//void operator+=(XXVar&v)
	inline void AddECMA(XXVar&v)
	{
		if(GetDataType()==XODT_CONST||
		   v.GetDataType()==XODT_CONST)
		{
			ToConst(XOCT_NAN);
			return;
		}
		ToFloat();
		v.ToFloat();
		fData+=v.fData;
	}
	inline void Add(XXVar&v)
	{
		/*if(IsUndefine()||v.IsUndefine())
		{
			ToConst(XOCT_UNDEFINE);
			return;
		}*/
		ToFloat();
		v.ToFloat();
		fData+=v.fData;
	}
	inline void Sub(XXVar&v)
	{
		ToFloat();
		v.ToFloat();
		fData-=v.fData;
	}
	inline void SubECMA(XXVar&v)
	{
		if(GetDataType()==XODT_CONST||
		   v.GetDataType()==XODT_CONST)
		{
			ToConst(XOCT_NAN);
			return;
		}
		ToFloat();
		v.ToFloat();
		fData-=v.fData;
	}
	//void operator*=(XXVar&v)
	inline void MulECMA(XXVar&v)
	{
		if(GetDataType()==XODT_CONST||
		   v.GetDataType()==XODT_CONST)
		{
			ToConst(XOCT_NAN);
			return;
		}
		ToFloat();
		v.ToFloat();
		fData*=v.fData;
	}
	inline void Mul(XXVar&v)
	{
		ToFloat();
		v.ToFloat();
		fData*=v.fData;
	}
	//void operator/=(XXVar&v)
	inline void DivECMA(XXVar&v)
	{		
		if(GetDataType()==XODT_CONST||
		   v.GetDataType()==XODT_CONST)
		{
			ToConst(XOCT_NAN);
			return;
		}
		v.ToFloat();
		if(v.fData==0)
		{
			ToConst(fData>=0?XOCT_INFINITY:XOCT_RINFINITY);
		}
		else
		{
			ToFloat();
			fData/=v.fData;
		}
	}
	inline void Div(XXVar&v)
	{		
		v.ToFloat();
		if(v.fData==0)
		{
			ToConst(fData>=0?XOCT_INFINITY:XOCT_RINFINITY);
		}
		else
		{
			ToFloat();
			fData/=v.fData;
		}
	}
	//void operator++()
	inline void DecECMA()
	{
		if(GetDataType()==XODT_CONST)
		{
			ToConst(XOCT_NAN);
			return;
		}
		ToNum();
		switch(nType)
		{
		case XODT_INT:iData32--;break;
		case XODT_FLOAT:fData--;break;
		}
	}
	inline void Dec()
	{
		ToNum();
		switch(nType)
		{
		case XODT_INT:iData32--;break;
		case XODT_FLOAT:fData--;break;
		}
	}
	inline void IncECMA()
	//void operator--()
	{
		if(GetDataType()==XODT_CONST)
		{
			ToConst(XOCT_NAN);
			return;
		}
		ToNum();
		switch(nType)
		{
		case XODT_INT:iData32++;break;
		case XODT_FLOAT:fData++;break;
		}
	}
	inline void Inc()
	//void operator--()
	{
		ToNum();
		switch(nType)
		{
		case XODT_INT:iData32++;break;
		case XODT_FLOAT:fData++;break;
		}
	}
	////////////////////////////////////////////////////////////
	// String
	inline void StringGreater(XXVar&var)
	{
		ToString(XFALSE);
		var.ToString(XFALSE);
		XBOOL bOK=XString8::Compare(strTxt,var.strTxt)>0;
		Release();
		nType=XODT_BOOLEAN;
		iData32=bOK;
		//StringLess(v);
		//iData32=!iData32;
	}
	inline void StringEQ(XXVar&v)
	{
		ToString(XFALSE);
		v.ToString(XFALSE);
		XU32 bOK=strTxt==v.strTxt?XFALSE:XString8::Compare(strTxt,v.strTxt);
		ToLogic();
		iData32=!bOK;
	}
	void StringLength(XBOOL);
	void StringAdd(XPCTSTR strBuf,int al=0)
	{
		if(strBuf==XNULL) return;
		ToString(XTRUE);
		if(al<=0)
			al=XString8::SafeStrlen(strBuf);
		if(al<=0) return;
		int l=GetData()->nLength;
		SetLength(l+al);
		XGlobal::Memcpy(strTxt+l,(void*)strBuf,al);
		strTxt[l+al]=0;

	}
	void StringAdd(XXVar&var)
	{
		ToString(XTRUE);
		var.ToString(XFALSE);

		//XSTRINGDATA*pData=GetData();
		int l=GetData()->nLength,al;
		switch(var.nStringType)
		{
		case STRING_CONST:
		case STRING_ATTACH:
			 al=XString8::SafeStrlen(var.strTxt);
			 break;
		default:
			 al=var.GetLength();//.GetData()->nLength;
		}
		//if(var.bConst)
		//	al=XString8::SafeStrlen(var.strTxt);
		//else
		//	al=var.GetData()->nLength;
		SetLength(l+al);
		XGlobal::Memcpy(strTxt+l,var.strTxt,al);
		strTxt[l+al]=0;
		//XString8 s(strData,STRING_ATTACH);
		//s+=var.strData;
		//strData=s.DataTo();
	}
	inline XU8 IsInt(){return nType==XODT_INT||nType==XODT_INT;}
	void StringExtract(XXVar&index,XXVar&count,XBOOL doMultiByte);
	inline void StringLess(XXVar&var)
	{
		ToString(XFALSE);
		var.ToString(XFALSE);
		XBOOL bOK=XString8::Compare(strTxt,var.strTxt)<0;
		Release();
		nType=XODT_BOOLEAN;
		iData32=bOK;
	}
	//////////////////////////////////////////////////////////
	// 
	inline void CharToASCII(XBOOL doMultiByte)
	{
		ToString(XFALSE);
		int i=strTxt[0];
		if(doMultiByte&&IsLeadByte(*strTxt))
			i=strTxt[0]<<8|strTxt[1];
		Release();
		nType=XODT_INT;
		iData32=i;
	}
	inline void ASCIIToChar(XBOOL doMultiByte)
	{
		ToInt();
		int v=iData32;
		char tmp[2];
		if(doMultiByte&&iData32>0xff)
		{
			//strData=new char[3];
			tmp[0]=(char)(v>>8);
			tmp[1]=(char)(v&0xff);
			//tmp[2]=0;
			SetString(tmp,2);
		}
		else
		{
			//strData=new char[2];
			tmp[0]=(char)v;
			SetString(tmp,1);
			//strData[1]=0;
		}
		//XString8 str((XWCHAR)iData32);
		//nType=XODT_STRING;
		//strData=str.DataTo();
	}
	////////////////////////////////////////////////////////////
	inline void Add2(XXVar&var)
	{
		if(nType==XODT_STRING||
		   var.nType==XODT_STRING)
		   StringAdd(var);
		else
		   Add(var);
	}
	inline void Add2ECMA(XXVar&var)
	{
		if(nType==XODT_STRING||
		   var.nType==XODT_STRING)
		   StringAdd(var);
		else
		   AddECMA(var);
	}
	inline void Mod(XXVar&var)
	{
		//var.ToInt();
		//ToInt();
		var.ToFloat();
		ToFloat();
		if(var.fData==0)
		{
			ToConst(iData32>=0?XOCT_INFINITY:XOCT_RINFINITY);
			//iData32=0;
		}
		else
			fData=fData-((int)(fData/var.fData))*var.fData;
		//else iData32=iData32%var.iData32;
	}
	////////////////////////////////////////////////////////////
	inline void BitAnd(XXVar&var)
	{
	    ToInt();
		var.ToInt();
		iData32&=var.iData32;
	}
	inline void BitLShift(XXVar&var)
	{
		ToInt();
		var.ToInt();
		if(var.iData32==0) return;
		iData32<<=var.iData32;
	}
	inline void BitRShift(XXVar&var)
	{
		ToInt();
		var.ToInt();
		if(var.iData32==0) return;
		iData32>>=var.iData32;
	}
	inline void BitOr(XXVar&var)
	{
		ToInt();
		var.ToInt();
		iData32=iData32|var.iData32;
	}
	inline void BitXor(XXVar&var)
	{
		ToInt();
		var.ToInt();
		iData32=iData32^var.iData32;
	}
	inline void BitURShift(XXVar&var)
	{
		ToInt();
		var.ToInt();
		iData32=((XU32)iData32)>>((XU32)var.iData32&0x1f);
	}
	void Release();//XBOOL bKeepString);
	void AdJustLength();
	XBOOL SetLength(int l,XBOOL bCopy=XTRUE);
public:
	void Set(const XXVar&var);
	static int m_nVersion;
	inline XSTRINGDATA* AllocBuffer(int l)
	{
		XSTRINGDATA*pData=(XSTRINGDATA*)AllocData(l);
		if( pData == NULL )
		{
			LOGWHERE();
			return pData;
		}
		pData->nMaxLength=l;
		memset(pData+1,0,l-sizeof(XSTRINGDATA));
		pData->nLength=0;
		pData->nRefs=1;
		return pData;
	}
	inline void FreeBuffer(XSTRINGDATA *pData)
	{
		if(pData->nRefs>1){ pData->nRefs--;return;}
		FreeData(pData,pData->nMaxLength);
	}	
};

#define SAFEDELETE(VV) if(VV) delete VV;

template <class _XT> class _XEXT_CLASS XVectorFast
{
public:
	_XT *m_pData;
	XU32 m_nSize,m_nMaxSize;
	int m_nBufSize;
	
public:
	typedef XVectorFast<_XT> MyVector;
	void AttachFrom(MyVector&d)
	{
		if(m_pData)
		   XXVar::FreeData(m_pData,m_nBufSize);
		m_nBufSize=d.m_nBufSize;
		m_nMaxSize=d.m_nMaxSize;
		m_nSize=d.m_nSize;
		m_pData=d.m_pData;
		d.m_pData=XNULL;
		d.m_nSize=0;
		d.m_nMaxSize=0;
		d.m_nBufSize=0;
	}
	XVectorFast()
	{
		m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
	}
	XVectorFast(XUINT nSize)
	{	m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
		SetSize(nSize,XFALSE);
	}
	~XVectorFast()
	{
		if(m_pData)//!=XNULL) 
		{
			//if(m_pData)
			XXVar::FreeData(m_pData,m_nBufSize);
		}
		m_pData=XNULL;
	}
	XVectorFast(const MyVector&src)
	{   //¥”¡ÌÕ‚“ª∏ˆ ˝◊Èππ‘�?


		m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
		m_nBufSize=0;
		Copy(src);
	}
public:
	XBOOL EnumAdd(const _XT&src);
	void EnumAdd(const _XT&key,const _XT&val);
	XBOOL operator==(const MyVector&src)
	{
		if(m_nSize!=src.m_nSize) return XFALSE;
		if(m_nSize>0)
			return XCMP(m_pData,src.m_pData,m_nSize)==0;
		return XTRUE;
	}
	void operator=(const MyVector&src)
	{	
		Copy(src);
	}
	XUINT GetSize() const
	{return m_nSize;}
	XUINT GetMenSize() const
	{return m_nSize*sizeof(_XT);}
	const _XT& GetAt(XINT nIndex)const
	{return m_pData[nIndex];}
	_XT* GetData()const
	{return m_pData;}
	operator _XT*()const
	{return m_pData;}
	void SetSize(XUINT nNewSize,XBOOL bExtern=XTRUE);
	void Add(const _XT&v)
	{
		SetSize(m_nSize+1);
		m_pData[m_nSize-1]=v;

	}
	XUINT Append(const MyVector& src);
	void Copy(const MyVector& src)
	{
		SetSize(src.m_nSize,XFALSE);
		XCOPY(m_pData, src.m_pData, src.m_nSize * sizeof(_XT));
	}
	void InsertAt(XUINT nIndex,const _XT&newElement, XUINT nCount=1);
	_XT RemoveLast()
	{
		if(m_nSize>0)
			m_nSize--;
		return m_pData[m_nSize];
	}
	_XT GetFirst()
	{
		if(m_nSize<=0) return XNULL;
		return m_pData[0];
	}
	_XT GetLast()
	{
		if(m_nSize<=0) return XNULL;
		return m_pData[m_nSize-1];
	}
	void RemoveAt(XUINT nIndex, XUINT nCount=1);
	void RemoveAll(XBOOL bClear=XTRUE)
	{	m_nSize=0;}
	void InsertAt(XUINT nIndex, MyVector&pNewArray);
	XUINT Append(const _XT* pData,XUINT nSize);
	XUINT InsertAt(XUINT nIndex,const _XT* pData,XUINT nSize);
protected:
	XUINT CalcSize(XUINT nSize,XBOOL bExtern);
};

template<class _XT>
XBOOL XVectorFast<_XT>::EnumAdd(const _XT&src)
{
	for(XU32 i=0;i<m_nSize;i++)
		if(m_pData[i]==src) return XFALSE;
	Add(src);
	return XTRUE;
}
template<class _XT>
void XVectorFast<_XT>::EnumAdd(const _XT&key,const _XT&val)
{
	for(XU32 i=0;i<m_nSize;i+=2)
	{
		if(m_pData[i]==key) 
		{
			m_pData[i+1]=val;
			return;
		}
	}
	Add(key);
	Add(val);
}

///////////////////////////////////////////////////////////
// ÃÌº”¡ÌÕ‚“ª∏ˆ ˝◊Èƒ⁄»›µΩ±æ È◊ÈµƒŒ≤≤ø
// src(IN) “™ÃÌº”µƒ ˝◊È
template<class _XT>
XUINT XVectorFast<_XT>::Append(const MyVector& src)
{
	XUINT nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize,XFALSE);
	XCOPY(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(_XT));
	return nOldSize;
}

template<class _XT>
void XVectorFast<_XT>::RemoveAt(XUINT nIndex, XUINT nCount)
{
	if(nIndex+nCount>=m_nSize)
		nCount=m_nSize-nIndex;
	if(nCount<=0) return;
	//º∆À„“™“∆∂Øµƒ≥§∂�?


	XUINT nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount>0)	//“∆∂Ø ˝æ�?


		XMOVE(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(_XT));
	//if(nIndex+nCount<m_nSize)
	XSET(m_pData+nIndex+nMoveCount,0,sizeof(_XT)*nCount);
	//for(XINT i=nIndex+nCount;i<m_nSize;i++)
	//	m_pData[i
	m_nSize -= nCount;
}

template<class _XT>
void XVectorFast<_XT>::InsertAt(XUINT nIndex, MyVector&pNewArray)
{
	if (pNewArray.GetSize() >0)
	{
		XUINT nOldSize=m_nSize;
		//…Ë÷√≥§∂»
		SetSize(m_nSize+pNewArray.m_nSize);
		//“∆∂Ø ˝æ›ƒ⁄»�?


		XMOVE(&m_pData[nIndex+pNewArray.m_nSize],&m_pData[nIndex],
			  (nOldSize-nIndex)*sizeof(_XT));
		//∏¥÷∆ƒ⁄»�?


		XCOPY(&m_pData[nIndex],pNewArray.m_pData,pNewArray.m_nSize*sizeof(_XT));
	}
}
//////////////////////////////////////////////////////////
// ÃÌº”“ª∂Œ ˝æ›µΩ ˝◊ÈµƒŒ≤≤ø
// pData(IN) “™ÃÌº”µƒ ˝æ›µÿ÷∑
// nSize(IN) “™ÃÌº”µƒ ˝æ›≥§∂�?


template<class _XT>
XUINT XVectorFast<_XT>::Append(const _XT* pData,XUINT nSize)
{
	if(pData==XNULL) return m_nSize;
	XUINT nOldSize = m_nSize;
	//…Ë÷√–¬µƒ≥§∂�?


	SetSize(m_nSize + nSize,XFALSE);
	XCOPY(m_pData + nOldSize, pData, nSize * sizeof(_XT));
	return nOldSize;	
}
template<class _XT>
XUINT XVectorFast<_XT>::InsertAt(XUINT nIndex,const _XT* pData,XUINT nSize)
{
	if(nIndex>=m_nSize) return Append(pData,nSize);
	int nOldSize=m_nSize;
	SetSize(m_nSize+nSize);
	XMOVE(&m_pData[nIndex+nSize],&m_pData[nIndex],
		  (nOldSize-nIndex)*sizeof(_XT));
	XCOPY(&m_pData[nIndex],pData,nSize*sizeof(_XT));
	return nOldSize;
}

template<class _XT>
XUINT XVectorFast<_XT>::CalcSize(XUINT nSize,XBOOL bExtern)
	{
		if(!bExtern) return nSize;
		if(nSize==0) return 0;
		else if(nSize<4) nSize=4;
		else if(nSize<64) nSize+=16;
		else if(nSize<256) nSize+=nSize>>1;
		else if(nSize<1024) nSize+=nSize>>2;
		else nSize+=nSize>>4;
		return nSize;
	}

template<class _XT>
void XVectorFast<_XT>::InsertAt(XUINT nIndex,const _XT&newElement, XUINT nCount)
	{
		if(nCount<=0) return;
		if (nIndex >= m_nSize)
		{
			SetSize(nIndex + nCount);  // grow so nIndex is valid
		}
		else
		{
			XUINT nOldSize = m_nSize;
			SetSize(m_nSize + nCount);
			XMOVE(&m_pData[nIndex+nCount], &m_pData[nIndex],
				  (nOldSize-nIndex) * sizeof(_XT));
			XSET(&m_pData[nIndex], 0, nCount * sizeof(_XT));
		}
		while (nCount--)
			m_pData[nIndex++] = newElement;

	}

template<class _XT>
void XVectorFast<_XT>::SetSize(XUINT nNewSize,XBOOL bExtern)
{
		if (nNewSize == 0)
		{
			m_nSize = 0;
		}
		else if (m_pData == XNULL)
		{	m_nMaxSize=CalcSize(nNewSize,bExtern);
			m_nBufSize=m_nMaxSize*sizeof(_XT);
			m_pData=(_XT*)XXVar::AllocData(m_nBufSize);
			m_nMaxSize=m_nBufSize/sizeof(_XT);
			if(m_pData==XNULL)
			{
				m_nSize=m_nMaxSize=0;
				return;
			}
			XSET(m_pData, 0, m_nMaxSize * sizeof(_XT));
			m_nSize = nNewSize;
		}
		else if (nNewSize <= m_nMaxSize)
		{	if(nNewSize<m_nSize)
				XSET(m_pData+nNewSize,0,(m_nSize-nNewSize)*sizeof(_XT));
			m_nSize = nNewSize;
		}
		else
		{	
			int s=m_nBufSize;
			m_nMaxSize=CalcSize(nNewSize,bExtern);//+nExt;
			m_nBufSize=m_nMaxSize*sizeof(_XT);
			_XT*pNewData=(_XT*)XXVar::AllocData(m_nBufSize);
			m_nMaxSize=m_nBufSize/sizeof(_XT);
			if(pNewData==XNULL)
			{
				m_nSize=m_nMaxSize=0;
				return;
			}
			//∏¥÷∆‘≠¿¥µƒ ˝æ�?


			XCOPY(pNewData, m_pData, m_nSize * sizeof(_XT));
			XSET(&pNewData[m_nSize], 0, (m_nMaxSize-m_nSize) * sizeof(_XT));

			//…Ë÷√–¬µƒ ˝æ›÷∏œÚ
			XXVar::FreeData(m_pData,s);
			m_pData = pNewData;
			m_nSize = nNewSize;
		}
	}



typedef XVectorFast<XU8>  XU8ArrayFast;
typedef XVectorFast<XU16> XU16ArrayFast;
typedef XVectorFast<XU32> XU32ArrayFast;
typedef XVectorFast<XS8>  XS8ArrayFast;
typedef XVectorFast<XS16> XS16ArrayFast;
typedef XVectorFast<XS32> XS32ArrayFast;
typedef XVectorFast<char> XCharArrayFast;
typedef XVectorFast<int>  XIntArrayFast;

template<class _XT,class _ET> class _XEXT_CLASS XSortArrayFast:public XSort
{
public:
	XBOOL Add(_ET data,XU8 bOnly)
	{
		int mi;
		int id= this->Index((void*)data,mi);
		if(id>=0&&bOnly) return XFALSE;

		m_pData->InsertAt(mi,data);
		return XTRUE;

	}
	XINT Find(_ET data)
	{
		int mi;
		return this->Index((void*)data,mi);
	}
	XBOOL Remove(_ET data)
	{
		int mi;
		int id=this->Index((void*)data,mi);
		if(id>=0)
		{
			m_pData->RemoveAt(id);
			return XTRUE;
		}
		return XFALSE;
	}
	XSortArrayFast(_XT data)
	{
		m_pData=data;
	}
	void* GetData(XU32 nID)
	{
		if(nID>=m_pData->GetSize()) return 0;
		return (void*)(*m_pData)[nID];
	}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		int r=(_ET)((int)iSrt)-(_ET)((int)iDst);
		if(r>0)
			return 1;
		else if(r<0)
			return -1;
		return 0;
	}
	XU32 GetCount(){return m_pData->GetSize();}
protected:
	_XT m_pData;
};

typedef XSortArrayFast<XU8ArrayFast*,XU8> XSortU8Fast;
typedef XSortArrayFast<XU16ArrayFast*,XU16> XSortU16Fast;
typedef XSortArrayFast<XU32ArrayFast*,XU32> XSortU32Fast;

#endif // !defined(AFX_XXVAR_H__6B8AC3EE_D47E_4646_94C8_ABEA8CEBBA39__INCLUDED_)
