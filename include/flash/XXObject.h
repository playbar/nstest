// XXObject.h: interface for the XXObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECT_H__BAA7022F_581B_4095_B54B_F333CD48973C__INCLUDED_)
#define AFX_XXOBJECT_H__BAA7022F_581B_4095_B54B_F333CD48973C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "flashgeom.h"
//#include "XPaintDib.h"
#include "XXVar.h"
#include "splayer.h"
#include "flashgeom.h"

//class XXObject;
//#define REFOBJ(OBJ) ((OBJ)?(OBJ)->REF():XNULL)
//#define FREEOBJ(OBJ) {if(OBJ) {(OBJ)->FREE();OBJ=XNULL;}}
//#define SETOBJ(DST,SRT) {if(DST&&DST!=SRT) (DST)->FREE();DST=SRT;}



#define REFOBJECT(OBJ) ((OBJ)?(OBJ)->RefObject():XNULL)
#define FREEOBJECT(OBJ) {if(OBJ) (OBJ)->FreeObject();OBJ=XNULL;}
#define XDESTROY(OBJ) {if(OBJ) {OBJ->Destroy();OBJ=XNULL;}}
#define SETOBJECT(DST,SRT) {if(DST&&DST!=SRT) (DST)->FreeObject();DST=(SRT)?(SRT)->RefObject():XNULL;}

//#define XSETOBJ2(DST,SRT) {if(DST!=SRT){if(DST) (DST)->FREE();DST=XREFOBJ(SRT);}}
#define _XXKEY

const XPCTSTR _strXMin="xMin";
const XPCTSTR _strYMin="yMin";
const XPCTSTR _strXMax="xMax";
const XPCTSTR _strYMax="yMax";

const XPCTSTR _strSplitQ=_XXKEY(",");


const XFDOUBLE _floatE=(XFDOUBLE)2.71828182845905;
const XFDOUBLE _floatLN10=(XFDOUBLE)2.302585092994046;
const XFDOUBLE _floatLN2=(XFDOUBLE)0.6931471805599453;
const XFDOUBLE _floatLOG10E=(XFDOUBLE)0.4342944819032518;
const XFDOUBLE _floatLOG2E=(XFDOUBLE)1.442695040888963387;
const XFDOUBLE _floatPI=(XFDOUBLE)3.141592653589793;
const XFDOUBLE _floatSQRT1_2=(XFDOUBLE)0.7071067811865476;
const XFDOUBLE _floatSQRT2=(XFDOUBLE)1.4142135623730951;


const XPCTSTR _strEvents[]={
	
	_SYSNAME(onChanged),
	_SYSNAME(onData),//_strData,
	_SYSNAME(onDragOut),//_strDragOut,
	_SYSNAME(onDragOver),//_strDragOver,
	_SYSNAME(onEnterFrame),//_strEnterFrame,
	_SYSNAME(onKeyDown),//_strKeyDown,
	_SYSNAME(onKeyUp),//_strKeyUp,
	_SYSNAME(onKillFocus),//_strKillFocus,
	_SYSNAME(onLoad),//_strLoad,
	_SYSNAME(onMouseDown),//_strMouseDown,
	_SYSNAME(onMouseMove),//_strMouseMove,
	_SYSNAME(onMouseUp),//_strMouseUp,
	_SYSNAME(onPress),
 	_SYSNAME(onRelease),
	_SYSNAME(onReleaseOutside),
	_SYSNAME(onRollOut),
	_SYSNAME(onRollOver),
	_SYSNAME(onSetFocus),
	_SYSNAME(onUnload),	
	_SYSNAME(onScroller),
	_SYSNAME(onSoundComplete),
	_SYSNAME(onHTTPStatus),//"onHTTPStatus",
};

enum {XEVENT_MOUSE,XEVENT_KEY,XEVENT_ALL};

enum {
	  XOBJEVENT_ONCHANGED=0,
	  XOBJEVENT_DATA=1,
	  XOBJEVENT_DRAGOUT=2,
	  XOBJEVENT_DRAGOVER=3,
	  XOBJEVENT_ENTERFRAME=4,
	  XOBJEVENT_KEYDOWN=5,
	  XOBJEVENT_KEYUP=6,
	  XOBJEVENT_KILLFOCUS=7,
	  XOBJEVENT_LOAD=8,
	  XOBJEVENT_MOUSEDOWN=9,
	  XOBJEVENT_MOUSEMOVE=10,
	  XOBJEVENT_MOUSEUP=11,
	  XOBJEVENT_PRESS=12,
	  XOBJEVENT_RELEASE=13,
	  XOBJEVENT_RELEASEOUTSIDE=14,
	  XOBJEVENT_ROLLOUT=15,
	  XOBJEVENT_ROLLOVER=16,
	  XOBJEVENT_SETFOCUS=17,
	  XOBJEVENT_UNLOAD=18,	  
	  XOBJEVENT_ONSCROLLER=19,
	  XOBJEVENT_HTTPSTATUS=20,
	  XOBJEVENT_MAX=40,
};

/*#define XXCLASS(TOKEN,TYPENAME,TYPEID) \
class _##TOKEN:public XXObject{\
public:\
	_##TOKEN(){m_nRefs=2;}\
	XU32 Handle(XSWFCONTEXT*pCnt,XXVARLIST*pList,void*pData,XU32 nType)\
	{\
		if(nType==XOH_CONSTRUCT){\
			TYPENAME *pObj=(TYPENAME*)pData;\
			if(pObj&&pObj->GetObjectType()==TYPEID)\
				pObj->##TOKEN(*pCnt,*pList);\
			else\
				pCnt->runstack.Push(XXVar::CreateConst(XOCT_UNDEFINE));return XTRUE;\
		}\
		return XXObject::Handle(pCnt,pList,pData,nType);\
	};\
};static _##TOKEN _##TOKEN##_*/

class XXObject;
typedef XVector<XXObject*> XOBJLIST;

class ScriptThread;
class XActionScript;
class XXStack;
class XXObjectString;
class XXObjectBool;
class XXVar;
//typedef XVector<XXVar*> XXVARLIST;
#define FREEOBJS(LIST)\
{for(XU32 i=0;i<LIST.GetSize();i++) FREEOBJ(LIST[i]);}

enum {XXOBJ_OBJECT=0,
	  XXOBJ_NUMBER=1,
	  XXOBJ_MATH=2,
	  XXOBJ_ARRAY=3,
	  XXOBJ_BOOLEAN=4,
	  XXOBJ_STRING=5,
	  XXOBJ_ARGUMENT=6,
	  XXOBJ_FUNCTION=7,
	  XXOBJ_CLIP=8,
	  XXOBJ_CONST=9,
	  XXOBJ_BUTTON=10,
	  XXOBJ_TEXT=11,
	  XXOBJ_KEY=12,
	  XXOBJ_CREATOR=13,
	  XXOBJ_BITMAP=14,
	  XXOBJ_RECTANGLE=15,
	  XXOBJ_POINT=16,
	  XXOBJ_STAGE=17,
	  XXOBJ_MOUSE=18,
	  XXOBJ_COLOR=19,
	  XXOBJ_COLORTRANSFORM=20,
	  XXOBJ_MATRIX=21,
	  XXOBJ_TRANSFORM=22,
	  XXOBJ_SHAREDOBJECT=23,
	  XXOBJ_SYSTEM=24,
	  XXOBJ_SECURITY=25,
	  XXOBJ_CAP=26,
	  XXOBJ_IME=27,
	  XXOBJ_ERROR=28,
	  XXOBJ_DATE=29,
//	  XXOBJ_EVENT=30,
	  XXOBJ_SOUND=31,
	  XXOBJ_LOADVARS=32,
	  XXOBJ_CASTER=33,
	 // XXOBJ_DOEVENT=14,
};

struct XXTOKEN
{ 
	//XS32		nID;
	//XString8	strKey;
	//XString8	strPath;
	XXVar		strKey;
	XXVar	    member;
	XXObject*	pGetter;
	XXObject*   pSetter;
	XXObject*	pOwer;
	EditText*   pEdits;
	XU8		    bGlobal,bStatic;
};
typedef XVectorFast<XXTOKEN*> XXTOKENS;
//#define XXTOKENS XVoidArray

class XXSortToken:public XSort
{
/*	struct DICTINDEX
	{
		XS32 iIndex;
		XS32 iPosition;
	};*/
public:
	static XFastAlloc alloc;
	inline static XXTOKEN* CreateToken()
	{
		XXTOKEN*p=(XXTOKEN*)alloc.Alloc();
		if(p) 
		{
			p->member.nType=0;
			p->strKey.nType=0;
			//p->pEdits=XNULL;
			//p->
			//memset(p,0,sizeof(XXTOKEN));
		}
		return p;
		//return p;
		//return new XXTOKEN;
	}
	inline static void FreeToken(XXTOKEN*pToken)
	{
		//XXVar::alloc64(
		//delete pToken;
		pToken->member.Release();
		pToken->strKey.Release();
		//pToken->
		alloc.Free(pToken);
	}
public:
	//XU32 GetCount(){return 
	XXSortToken(XXTOKENS*pTokens)
	{	m_pTokens=pTokens;	}
	XU32 GetCount(){return m_pTokens->GetSize();}
	void* GetData(XU32 id){return (void*)(*m_pTokens)[id]->strKey.strTxt;}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{	return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);	}
	XBOOL Remove(XPCTSTR strName,XBOOL bLowCase);
	XXTOKEN* TokenOfOnly(XPCTSTR strKey,XBOOL bLowCase)
	{
		int mi;
		int id=Index((void*)strKey,mi,bLowCase?XSS_NOCASE:XSS_NORMAL);
		if(id>=0) return (*m_pTokens)[id];
		return XNULL;
	}
	XXTOKEN* TokenOf(const XXVar&name,XBOOL bLowCase,XU8 bAdd)
	{
		int mi;
		int id=Index((void*)name.strTxt,mi,bLowCase?XSS_NOCASE:XSS_NORMAL);
		if(id>=0) return (*m_pTokens)[id];
		else if(bAdd)
		{
			XXTOKEN* p=CreateToken();//{tokenID,XNULL,XNULL,XNULL};
			p->strKey=name;
			p->bGlobal=XTRUE;
			p->pGetter=XNULL;
			p->pSetter=XNULL;
			p->pOwer=XNULL;
			p->pEdits=XNULL;
			m_pTokens->InsertAt(mi,p);
			return (*m_pTokens)[mi];
		}
		return XNULL;
	}
#ifdef _VARCACHE_
	XXTOKEN* TokenOfOnly(XPCTSTR strKey,XU32&idd,XBOOL bLowCase)
	{
		int mi;
		int id=Index((void*)strKey,mi,bLowCase?XSS_NOCASE:XSS_NORMAL);
		idd=id;
		if(id>=0) return (*m_pTokens)[id];
		return XNULL;
	}
	XXTOKEN* TokenOf(const XXVar&name,XU32&idd,XBOOL bLowCase,XU8 bAdd)
	{
		int mi;
		int id=Index((void*)name.strTxt,mi,bLowCase?XSS_NOCASE:XSS_NORMAL);
		idd=id;
		if(id>=0) return (*m_pTokens)[id];
		else if(bAdd)
		{
			XXTOKEN* p=CreateToken();//{tokenID,XNULL,XNULL,XNULL};
			p->strKey=name;
			p->bGlobal=XTRUE;
			p->pGetter=XNULL;
			p->pSetter=XNULL;
			p->pOwer=XNULL;
			p->pEdits=XNULL;
			m_pTokens->InsertAt(mi,p);
			idd=mi;
			return (*m_pTokens)[mi];
		}
		return XNULL;
	}
#endif
protected:
	//XU8 m_nMode;
	XXTOKENS* m_pTokens;
};
class _constructor;
class _protoType;
class _proto;

#define XDEC_XOBJECT(class_name) \
public: \
	virtual XXObject* CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL bObj=XFALSE);\
	static class_name * CreateClass(XXObject*pSuper,XU8 bInit,ScriptPlayer*pRoot)\
	{class_name*pObj=new class_name(pRoot);\
	if(pObj){\
		pObj->SetSuper(pSuper,bInit);\
		if(bInit) pObj->InitObject();\
		pObj->m_bBase=bInit;}\
	return pObj;}
	
#define XNEEDOBJR(OBJ,CNT)\
	if(OBJ==XNULL) {cnt.pStack->PushConst(XOCT_NULL);return;}
#define XNEEDOBJ(OBJ)\
	if(OBJ==XNULL) return;
class XXObjectShared;
#ifdef _USE_CUSTOM_ALLOCATOR
#include "GGAllocator.h"
class XXObject : public GGAllocator
#else
class XXObject//:public XSortObj 
#endif
{

public:

#ifdef WIN32
	#ifdef _ANALY
	//XString8 m_strClassName;
		XXVar m_strClassNameT;
	#endif
#else
	XXVar m_strClassNameT;
#endif 

public:
#ifdef _VARCACHE_
	void RemoveEvent(int id);
	void InstallEvent(int id,XXObject*pObject,XSWFCONTEXT*pCnt);
	XBOOL GetMemberCache(XSWFCONTEXT&cnt,XPCTSTR strName,XXVar&var);
	XBOOL SetMemberCache(XSWFCONTEXT&cnt,const XXVar&name,XXVar&var);//XXObject*pObj)
#endif
	virtual void Destruct(){FREEOBJECT(m_pSuper);}
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);

	XBOOL AddPropertySystem(const XXVar&name,int id)
	 {
		 if(!InitTokens()) return XFALSE;
		 XXSortToken sort(m_pTokens);
		 XXTOKEN*pToken=sort.TokenOf(name,m_pRoot->NeedLowCase(),XTRUE);//strName);
		 if(pToken==XNULL) return XFALSE;
		 
		 pToken->member.nType=XODT_FUNCTION;//ToFunction(id);
		 pToken->member.nStringType=id;
		 pToken->member.pObject=0;

		 return XTRUE;
	 }
	struct _XEVENT
	{
		XXObject*pHandler;
		XXObject*pThread;
		_XEVENT*pNext;
	};
	_XEVENT* m_pEvents;
	XOBJLIST m_events;
	int RemoveEvent(XPCTSTR strName);
	int InstallEvent(XPCTSTR strName,XXObject*pObject,XSWFCONTEXT*pCnt);
	XDEC_XOBJECT(XXObject)
protected:
	friend class XXObjectShared;
	friend class XXStack;
	friend class _constructor;
	friend class _protoType;
	friend class _proto;
public:
	XBOOL NeedLowCase()
	{
		return m_pRoot->NeedLowCase();
		//if(m_pRoot->NeedLowCase()) return XTRUE;
		//return m_bBase&&m_bNoCase;
	}
	XBOOL IsSubOf(XXObject*p)
	{
		if(p==XNULL||m_pSuper==this) return XFALSE;
		if(p==this) return XTRUE;
		if(m_pSuper) return m_pSuper->IsSubOf(p);
		return XFALSE;
	}
	XBOOL IsSuperOf(XXObject*p)
	{
		if(!p||!p->m_pSuperObj) return XFALSE;
		XXObject*ps=p->m_pSuperObj;
		//XXObject*ps=this;
		while(ps)
		{
			if(ps==this) return XTRUE;
			ps=ps->m_pSuperObj;
		}
		return XFALSE;
	}
/*	XBOOL IsChildOf(XXObject*p)
	{
		if(p==XNULL) return XFALSE;//||m_pParent==this) return XFALSE;
		if(p==this) return XTRUE;
		XXObject*pp=GetParent();
		//if(m_pParent) return m_pParent->IsChildOf(p);
		if(pp) return pp->IsChildOf(p);
		return XFALSE;
	}*/
public:
	XXObject* GetSuperObject()
	{
		return m_pSuperObj?m_pSuperObj:m_pSuper;
	}
	XBOOL MemberOf(XU32 id,XXVar&var,XXVar&name)
	{
		if(m_pTokens==XNULL) return XFALSE;
		if(id>=m_pTokens->GetSize()) return XFALSE;
		var=(*m_pTokens)[id]->member;
		name=(*m_pTokens)[id]->strKey;
		return XTRUE;
		//return id;
		//return XXSortToken::NameOf(id);
	}
	XXObjectCreator* GetGlobal(){return m_pRoot->m_pGlobal;}
	
	//XXObject* GetContext(){return m_pContext;}
	int GetVarCount()
	{
		if(!m_pTokens) return 0;
		return m_pTokens->GetSize();
	}
	XXTOKENS* GetTokens()
	{
		return m_pTokens;
	}
	enum {BASE_BASE=0x01,BASE_CALLER=0x80,BASE_KEYLIS=0x40,BASE_PTLIS=0x20};

	void GetValueOf(XSWFCONTEXT&cnt,XXTOKEN*pToken,XXVar&var,XXObject*pCaller);
	void SetValueOf(XSWFCONTEXT&cnt,XXTOKEN*pToken,XXVar&var,XXObject*pCaller);
	/*int MemberOf(XU32 id,XXVar&var)
	{
		if(m_pTokens==XNULL) return -1;
		if(id>=m_pTokens->GetSize()) return -1;
		var=(*m_pTokens)[id]->member;
		id=(*m_pTokens)[id]->nID;
		return id;
		//return XXSortToken::NameOf(id);
	}*/
	/*void SetContext(XXObject*p)
	{	
		m_pContext=p;
		//if(!p) m_pParent=p;
		//else if(!p->IsChildOf(this))
		//{m_pParent=p;}
		//else
		//	int v=0;
	}*/
	
	void SetSuper(XXObject*pObj,XU8 bInit);
	XXObject* GetSuper()
	{
		return (m_pSuper);
	}
	
public:
	int ConstOf(XString8&strValue)
	{
		switch(strValue[0])
		{
		case 'N':
			 if(strValue==_strConstType[4]) return 4;
			 break;
		case '-':
			 if(strValue==_strConstType[3]) return 3;
			 break;
		case 'I':
			 if(strValue==_strConstType[2]) return 2;
			 break;
		case 'n':
			 if(strValue==_strConstType[1]) return 1;
			 break;
		case 'u':
			 if(strValue==_strConstType[0]) return 0;
			 break;
		}
		return -1;
	}
	XPCTSTR TypeOf()
	{
		/*switch(m_nObjType)
		{
		case XOOT_OBJECT:
			 return _strType[XOOT_OBJECT];
		default:
			 //switch(m_nDataType)
			 //{
			 //case XODT_CONST:
			 //default:
			  return _strTypeString[m_nDataType];
			 //}
		} */
		return _strTypeString[m_nDataType];
		//return _strType[0];
	}
	XBOOL IsObject(XU8 nClass){return GetObjectType()==nClass;}
	XBOOL IsType(XU8 nType){return m_nDataType==nType;}
	XBOOL IsString(){return m_nDataType==XODT_STRING;}
	XBOOL IsFunction(){return m_nDataType==XODT_FUNCTION;}
	XBOOL IsFunction2(){return m_nDataType>=XODT_STRING;}
	XBOOL IsMovieClip(){return m_nDataType==XODT_MOVIECLIP;}
	XBOOL IsConst(){return m_nDataType==XODT_CONST;}
	XBOOL IsNumber()
	{
		switch(m_nDataType)
		{
		case XODT_INT:
		case XODT_FLOAT:return XTRUE;
		}
		return XFALSE;
	}
	XBOOL IsNumber3();
	XBOOL IsObject(){return m_nDataType==XODT_OBJECT;}
	//XBOOL IsMovieClip(){return m_nDataType==XODT_MOVIECLIP;}
	XU8 GetDataType(){return m_nDataType;}
public:
	 XBOOL AddProperty(XXVARLIST&list)
	 {
		 if(list.GetSize()<2) return XFALSE;
		 if(!list[1].IsObject()) return XFALSE;
		 //XString8 strName;
		 list[0].ToString(XFALSE);//(strName);
		 
		 XXObject*pGetter=list[1].pObject;
		 XXObject*pSetter=(list.GetSize()>2&&list[2].IsObject())
						  ?list[2].pObject:XNULL;
		 return AddProperty(list[0],pGetter,pSetter);
		 //return pObject;
		 //return id;
	 }
	 XBOOL AddProperty(const XXVar&name,XXObject*pGetter,XXObject*pSetter)
	 {
		 if(!InitTokens()) return XFALSE;
		 XXSortToken sort(m_pTokens);
		 XXTOKEN*pToken=sort.TokenOf(name,m_pRoot->NeedLowCase(),XTRUE);//strName);
		 if(pToken==XNULL) return XFALSE;
		 
		 SETOBJECT(pToken->pGetter,pGetter);
		 SETOBJECT(pToken->pSetter,pSetter);

		 return XTRUE;
	 }
	 XU32 AddMember(XXVar&name,XPCTSTR strValue)//XXObject*pObject)
	 {
		 //int iValue=_strConst.IndexOf(strName);
		 if(!InitTokens())
			 return 0;
		 XXSortToken sort(m_pTokens);
		 XXTOKEN*pToken=sort.TokenOf(name,m_pRoot->NeedLowCase(),XTRUE);
		 if(pToken==XNULL)
			 return 0;
		 pToken->member=strValue;			//Need Modify
		 		 
		 //(!bGlobal)
		//	 pToken->bGlobal=XFALSE;
		 return 1;
		 //return pObject;
		 //return id;
	 }

	 XU32 AddMember(const XXVar&name,const XXVar&var,XU8 bGlobal=XTRUE,XXObject*pOwer=XNULL)//XXObject*pObject)
	 {
		 //int iValue=_strConst.IndexOf(strName);
		 //if(strcmp(name.strTxt,"func")==0)
		//	 int v=0;
		 if(!InitTokens())
			 return 0;
		 XXSortToken sort(m_pTokens);
		 XXTOKEN*pToken=sort.TokenOf(name,NeedLowCase(),XTRUE);
		 if(pToken==XNULL)
			 return 0;
		 pToken->member=var;			//Need Modify
		 if(var.IsObject())
			InstallEvent(name.strTxt,var.pObject,XNULL);		 
		 else
			RemoveEvent(name.strTxt);
		 
		 pToken->pOwer=pOwer;
		 if(!bGlobal)
			 pToken->bGlobal=XFALSE;
		 return 1;
		 //return pObject;
		 //return id;
	 }
	XU32 SetMemberOnly(XSWFCONTEXT&cnt,XPCTSTR strKey,XXVar&var,EditText*pEdit);
	XBOOL SetMemberLocal(XSWFCONTEXT&cnt,XPCTSTR strName,XXVar&var,EditText*pEdit);
	XXTOKEN* GetMemberOf(XSWFCONTEXT&cnt,XPCTSTR,XU8 bInc);//XPCTSTR strName)
	XBOOL GetIndex(XU32 id,XXVar&var,XXVar*name);//XString8*pName);
protected:
	 XU32 Remove(XPCTSTR strName)
	 {
		 //RemoveEvent(strName);
		 //XString8 str(strName);
		 //Handle(XNULL,XNULL,(void*)strName,XOH_REMOVEEVENT);
		 RemoveEvent(strName);
		 if(!InitTokens()) return XFALSE;
		 XXSortToken sort(m_pTokens);
		 
		 return sort.Remove(strName,m_pRoot->NeedLowCase());
	 }
public:
/*	 void ClearData()
	 {
		ReleaseEvents();
		if(m_pTokens)
		{

			delete m_pTokens;
		}
	 }*/
protected:
	 virtual void Release()
	 {	
		 //if(m_pSuper==XNULL)
		 //	 int v=0;
		 Destruct();
		 FREEOBJECT(m_pSuper);
		 ReleaseEvents();
		 ClearMembers();
		 //FREEOBJECT(m_pParent);
		 //m_pContext=XNULL;
		 //m_pSuper=XNULL;
		 //Handle(XNULL,XNULL,XNULL,XOH_RELEASE);
		 //FREEOBJ(m_pParent);
		 //FREEOBJ(m_pSuper);
	 }
	 //static void InitConst()
	 //{  _strConst.Release(); }
	 /*void RefTokens(XXTOKENS&tokens)
	 {
		 tokens.Append(m_tokens);
		 for(XU32 i=0;i<tokens.GetSize();i++)
		 {
			 REFOBJ(tokens[i]->pObject);
			 REFOBJ(tokens[i]->pGetter);
			 REFOBJ(tokens[i]->pSetter);
		 }
	 }*/
public:	
	 void ClearMembers()
	 {
		 //if(m_pRoot->m_pGlobal==this)
		//	 int v=0;
		 if(m_pTokens)//==XNULL) return;
		 {
			 XXTOKENS*p=m_pTokens;
			 m_pTokens=XNULL;
			 for(XU32 i=0;i<p->GetSize();i++)
			 {
				 XXTOKEN*pt=(*p)[i];
//				 if(pt->strKey=="showPreGameAd")
//					 int v=0;
				 /*if(pt->member.IsObject())
				 {
					if(((XXObject*)pt->member.pObject)->
						m_pContext==this)
						pt->member.SetContext(XNULL);
				 }*/
				 FREEOBJECT(pt->pGetter);
				 FREEOBJECT(pt->pSetter);
				 XXSortToken::FreeToken(pt);
			 }
			 //p->FreeAll();
			 delete p;
			 
		 }
	 }


	XU32 RemoveMember(XPCTSTR strName)
	{
		return Remove(strName);
		//return XTRUE;
	}
	virtual XBOOL GetMember(XU32 id,XXVar&var,XXVar*name)//XString8*strName)
	{
		return XFALSE;
		//return Handle((XSWFCONTEXT*)strName,(XXVARLIST*)id,&var,XOH_INDEXOF);
	}
	virtual XBOOL SetMember(XU32 id,XXVar&var)
	{
		if(m_pTokens)
		 {
			 //XU32 id=(XU32)pList;
			 //XXVar*pVar=(XXVar*)pData;
			 if(id<m_pTokens->GetSize())
			 {
				 (*m_pTokens)[id]->member=var;//*pVar;					 
				 return XTRUE;
			 }
		 }
		return XFALSE;
		//return XFALSE;
		//return Handle(XNULL,(XXVARLIST*)id,&var,XOH_SETINDEX);
	}
//DEL 	XBOOL DoObjEvent(XSWFCONTEXT&cnt,XU32 id)
//DEL 	{
//DEL 		return Handle(&cnt,XNULL,(void*)id,XOH_DOEVENT);
//DEL 	}
	/*XBOOL SetMember(XSWFCONTEXT&cnt,XPCTSTR strName,XXVar&var,XBOOL bParent,XU8 bWith)
	{
		XS32 tokenID=XXSortToken::IndexOf(strName,XTRUE);
		if(SetMember(cnt,tokenID,var,bParent,bWith))
		{
			if(var.IsObject())
				Handle(&cnt,(XXVARLIST*)strName,var.pObject,XOH_SETEVENT);
			else
				Handle(XNULL,XNULL,(void*)strName,XOH_REMOVEEVENT);
			return XTRUE;
		}
		return XFALSE;
	}*/
	XBOOL GetMember(XSWFCONTEXT&cnt,XPCTSTR strName,XXVar&var,
					XBOOL bInc);
	XU32 SetMember(XSWFCONTEXT&cnt,const XXVar&,XXVar&var,EditText*pEdit);
protected:
	
	//XBOOL NewObject(XSWFCONTEXT*pCnt,XOBJLIST*pList,XNULL
public:
	
	XFDOUBLE GetNumber();
	int   GetInt();
	XBOOL IsTrue();
	XU32 NewMethod(XPCTSTR strName,XSWFCONTEXT*pCnt,
				   XXVARLIST*pList,XU8 bObj=XFALSE,XU8 bForce=XTRUE);
	
	XBOOL CallMethod(XSWFCONTEXT*pCnt,XPCTSTR strName,XXVARLIST&list,XU8 bEvent);
	/*XBOOL HasEvent(XU32 type)
	{

		//return Handle(XNULL,XNULL,(void*)type,XOH_HASEVENT);
	}*/
	XBOOL HasMouseEvent();
	XBOOL HasKeyEvent();
	XBOOL HasAnyEvent();
	XBOOL HasPressEvent();
	virtual void GetString(XXVar&var)//XString8&str)
	{
	   var.SetString(_XXKEY("["),1);
	   var.StringAdd(_strTypeString[m_nDataType]);
	   var.StringAdd(_XXKEY("]"));
	}
	//
	virtual XBOOL GetClipOf(XSWFCONTEXT*pCnt,XPCTSTR strName,XXVar&var);
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller);
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller)
	{	
		pCnt->pStack->Push(this);
		return XTRUE;
		//Function(pCnt,list,pCaller);
		//Handle(pCnt,pList,pCaller,XOH_CONSTRUCT);	
	}
// 	void DoFunction(XSWFCONTEXT*pCnt,XXVARLIST*pList,XXObject*pCaller)
// 	{	Handle(pCnt,pList,pCaller,XOH_FUNCTION); }
//	XBOOL NewObject(XPCTSTR strName,XSWFCONTEXT*pCnt,XXVARLIST*pList,XU8 bForce)
//	{
//		return NewMethod(strName,pCnt,pList,XTRUE,bForce);
//	}
	//void NewObject(XSWFCONTEXT*pCnt,XXVARLIST*pList,XBOOL bObj);
	virtual void ValueOf(XSWFCONTEXT*pCnt)
	{
		pCnt->pStack->Push(this);
		//Handle(pCnt,XNULL,XNULL,XOH_VALUEOF);
	}
	//void Init()
	//{	Handle(XNULL,XNULL,XNULL,XOH_INIT);	}
	//XU32 Length(XXVar&var)
	//{
	//	return Handle(XNULL,XNULL,&var,XOH_LENGTH);
	//}
	virtual XU32 IsArray()
	{
		return XFALSE;
		//return Handle(XNULL,XNULL,XNULL,XOH_ISARRAY);
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_OBJECT;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	/*XXVar* IndexOf(XU32 id)
	{
		return (XXVar*)Handle(XNULL,XNULL,(void*)id,XOH_INDEXOF);
	}*/
	/////////////////////////////////////////////////////////////////////////////
	// for OPERATOR
protected:	
	
	//virtual XBOOL Function(XSWFCONTEXT*pCnt,XXTOKENS&list)
	 
public:	
	XU32 HandlerEvent(XSWFPlayer*pPlayer,XXObject*baseThread, XU32 id,XXVARLIST*pArgs);
	XXObject* GetParent();
	XBOOL RemoveEditText(XSWFCONTEXT&cnt,XPCTSTR strName,EditText*pEdit);
	XBOOL FindVariable(XPCTSTR strKey,XXVar&var);
	XBOOL IsInstanceOf(XXObject*pObj);
	int EnumVariables(XSWFCONTEXT&cnt);
	virtual ScriptThread* GetTarget()//XSWFCONTEXT*pCnt)
	{
		if(m_pObject) return m_pObject->thread;
		return XNULL;//(ScriptThread*)Handle(XNULL,XNULL,XNULL,XOH_GETTARGET);
	}
	//static XXObject* CreateClass(XXObject*pSuper,XXObject*pParent,XU8 bInit);
	virtual ~XXObject();
	ScriptThread* GetRoot();
//	void SetType(XU8 nType){m_nObjType=nType;}
//	XU8  GetType(){return m_nObjType;}
	void SetCaller(){m_bBase|=BASE_CALLER;}
	void SetKeyListener(){m_bBase|=BASE_KEYLIS;}
	void SetPtListener(){m_bBase|=BASE_PTLIS;}
	void ClearCaller(){m_bBase&=~BASE_CALLER;}
	void ClearKeyListener(){m_bBase&=~BASE_KEYLIS;}
	void ClearPtListener(){m_bBase&=~BASE_PTLIS;}
	XXObject* RefObject()
	{
		m_nRefs++;
		return this;
	}
	XXObject* GetMember(char* name);
	virtual XBOOL FreeObject();
	virtual int GetSubCount(XXObject*pObj);
	virtual void Destroy()
	{
		if(!FreeObject())
		{
			if(m_nRefs)
				Release();
		}
		//Handle(XNULL,XNULL,XNULL,XOH_DESTROY);
		/*if(m_nRefs)
		{
			m_nRefs--;
			if(!m_nRefs)
				_freeObjs.Add(this);
		}*/
	}
	XU32 GetRefs(){return m_nRefs;}
	//XU32 nDebugID;
	static XXObject* m_pCreate;
public:
	
	void PostDestroy(XXObject*pObj);
	void RemoveListener(XXObject*pObj);
	void ReleaseEvents();
	void AddListener(XXObject*thread,XXObject*pObj);
	void UpdateEditText(XSWFCONTEXT&cnt,XXTOKEN*pToken,EditText*pEdit);
	//static int Number(XPCTSTR name);
	void ClearLocalVar();
	
	XBOOL InitTokens()
	{
		if(m_pTokens!=XNULL) return XTRUE;
		m_pTokens=new XXTOKENS;
		return m_pTokens!=XNULL;
	}
	
	int GetEventCode(XPCTSTR strCode);
	XBOOL InitObject();
	
#pragma pack(1)
	//ScriptThread*m_pThread;
public:
	XBOOL HasEvent(XU32 f);
	void CopyMembers(XXObject*pObj);
	XXObject(ScriptPlayer*pRoot);
	XBOOL LinkClip(XXTOKEN*pToken);
	SObject*m_pObject;
	XU32	  m_nRefs;
	XU8		  m_nDataType,m_bBase,m_bNoCase;
	XXTOKENS* m_pTokens;
	XXObject* m_pSuper,*m_pNext,*m_pSuperObj;
//	XXObject* m_pContext;
	ScriptPlayer*m_pRoot;
//	XXObject* m_pParent;
#pragma pack()
public:
	//void SetThread(ScriptThread*p){m_pThread=p;}
	//ScriptThread* GetThread(
	class XXObjects:public XSort			
	{
	public:
		void Add(XXObject*pObj)
		{
//			if(pObj->m_strClassName=="Controller")
//				int v=0;

			int mi;
			int id=Index(pObj,mi);
			if(id>=0) 
				return;
			objs.InsertAt(mi,pObj);
		}
		void Remove(XXObject*pObj)
		{
			int id=Index(pObj);
			if(id>=0) objs.RemoveAt(id);
		}
		void RemoveAll(){objs.RemoveAll();}
		void Release()
		{	
			int nSize=objs.GetSize();
			while(nSize)
			{	
				XXObject**pData=(XXObject**)objs.DataTo();
				for(int i=0;i<nSize;i++)
				{
					//if(pData[i]->m_nRefs)
					//	int v=0;
					delete pData[i];
				}
				delete pData;
				nSize=objs.GetSize();
				//objs.FreeAll();	
			}
		}
		void DestroyAll()
		{	
			int nSize=objs.GetSize();
			for(int i=0;i<nSize;i++)
			{
				XXObject*pObj=objs[i];
				pObj->m_pRoot=XNULL;
				pObj->Destroy();
			}
			/*//while(nSize)
			{	
				XXObject**pData=(XXObject**)objs.DataTo();
				for(int i=0;i<nSize;i++)
				{
					//if(pData[i]->m_nRefs)
					//	int v=0;
					//delete pData[i];
					XXObject*pObj=pData[i];
					pObj->m_pRoot=XNULL;
					pObj->Destroy();
				}
				delete pData;
				//nSize=objs.GetSize();
				//objs.FreeAll();	
			}*/
		}
		void DestructAll()
		{	
			int nSize=objs.GetSize();
			for(int i=0;i<nSize;i++)
			{
				XXObject*pObj=objs[i];
				//pObj->m_pRoot=XNULL;
				pObj->Destruct();
			}
			/*//while(nSize)
			{	
				XXObject**pData=(XXObject**)objs.DataTo();
				for(int i=0;i<nSize;i++)
				{
					//if(pData[i]->m_nRefs)
					//	int v=0;
					//delete pData[i];
					XXObject*pObj=pData[i];
					pObj->m_pRoot=XNULL;
					pObj->Destroy();
				}
				delete pData;
				//nSize=objs.GetSize();
				//objs.FreeAll();	
			}*/
		}
		/*void ReleaseAll()
		{	
			int nSize=objs.GetSize();
			//while(nSize)
			{	
				XXObject**pData=(XXObject**)objs.DataTo();
				for(int i=0;i<nSize;i++)
				{
					//XXObject*pObj=pData[i];
					//pObj->ClearData();
					//delete pObj;
					//pObj->m_pRoot=XNULL;
					//pObj->Destroy();
				}
				delete pData;
				//nSize=objs.GetSize();
				//objs.FreeAll();	
			}
		}*/
		
	XU32 GetCount(){return objs.GetSize();}
	protected:
		XINT Compare(void*iSrt,void*pDst,XU8 /*nArg*/)
		{
			return CompareUInt((XU32)iSrt,(XU32)pDst);
		}
		void* GetData(XU32 iData){return objs[iData];}
		
		XVector<XXObject*> objs;
	};
#ifdef _DEBUG
	static int nObjCount[100];
	
#endif
	static XXObjects _allObjs;
	static void FreeAll();
	static void DestroyAll(){_allObjs.DestroyAll();}
	static void DestructAll(){_allObjs.DestructAll();}
	//static XVector<XXObject*> _freeObjs;
	static XXObjects _freeObjs;
	static void FreeObjs();	
//	static XCONST _strConst;
};

/*class XXObjectConst:public XXObject
{
	XDEC_XOBJECT(XXObjectConst);
public:	
	
	XBOOL InitObject(){return XTRUE;}
	int GetInt(){return m_nIndex;}
	XBOOL IsTrue()
	{
		switch(m_nIndex)
		{
		case XOCT_NULL:
		case XOCT_UNDEFINE:return XFALSE;
		default:
		return XTRUE;
		}
	}
	XXObjectBool* ToBoolean();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller)
	{
		int id=XOCT_UNDEFINE;
		if(list.GetSize())
			id=list[0].ToInt();
		pCnt->pStack->PushConst(id);
		return id;
	}
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller)
	{	
		if(list.GetSize())
			m_nIndex=list[0].ToInt();
		else
			m_nIndex=XOCT_UNDEFINE;
		pCnt->pStack->Push(this);
		return (XU32)this;
		//Handle(pCnt,pList,pCaller,XOH_CONSTRUCT);	
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_CONST;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}	
	virtual void GetString(XXVar&var)//XString8&str)
	{
		var=_strConstType[m_nIndex];
	}
	void SetValue(XU8 c){m_nIndex=c;}
protected:
	XXObjectConst(ScriptPlayer*pRoot,XU8 id=0):XXObject(pRoot)
	{
//		m_nObjType=XOOT_TYPE;
		m_nDataType=XODT_CONST;
		m_nIndex=id;
		//m_nRefs=nRefs;
	}
	XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list);
	XU8 m_nIndex;
};*/


class XXObjectNumber:public XXObject
{
	XDEC_XOBJECT(XXObjectNumber);
public:
//DEL 	static XXObjectNumber* CreateClass(XXObject *pSuper, XFDOUBLE v, XU8 bInit)
//DEL 	{
//DEL 		XXObjectNumber*pObj=new XXObjectNumber(v);
//DEL 		if(pObj)
//DEL 		{
//DEL 			pObj->m_bBase=bInit;
//DEL 			if(bInit)
//DEL 				pObj->InitObject();
//DEL 			pObj->SetSuper(pSuper,bInit);
//DEL 			//pObj->SetParent(pParent);
//DEL 		}
//DEL 		return pObj;
//DEL 	}
	XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list)
	{
		if(list.GetSize())
		{
			list[0].ToNum();
			pCnt->pStack->Push(list[0]);
		}
		else
			pCnt->pStack->Push(0);
		return XTRUE;
	}
	XFDOUBLE GetNumber()
	{
		switch(m_nDataType)
		{
		case XODT_FLOAT:return m_fValue;
		default:return (XFDOUBLE)m_iValue;
		}
		//return m_bValue;
	}
	int GetInt()
	{
		switch(m_nDataType)
		{
		case XODT_FLOAT:return (int)m_fValue;
		default:return m_iValue;
		}
		//return m_bValue;
	}
	XBOOL IsTrue()
	{
		switch(m_nDataType)
		{
		case XODT_FLOAT:return m_fValue!=0;
		default:
		case XODT_INT:return m_iValue;

		}
	}
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller)
	{
		if(list.GetSize())
		{
			list[0].ToNum();
			pCnt->pStack->Push(list[0]);
			return XTRUE;
		}
		else
			pCnt->pStack->Push(0);
		return XFALSE;
	}
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pCaller)
	{	
		//Function(pCnt,list,pCaller);
		if(list.GetSize())
		{
			list[0].ToNum();
			m_nDataType=list[0].nType;
			m_fValue=list[0].fData;
		}
		pCnt->pStack->Push(this);
		return (XU32)this;
		//Handle(pCnt,pList,pCaller,XOH_CONSTRUCT);	
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_NUMBER;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	

	virtual void GetString(XXVar&var)//XString8&str)
	{
		 ToIntIf();			 
		 switch(m_nDataType)
		 {
		 case XODT_INT:
			  var=m_iValue;
			  break;
			  //(*(XString8*)pData).FromInt(m_iValue);break;
		 case XODT_FLOAT:
			  var=m_fValue;
			  break;
			  //(*(XString8*)pData).FromFloat(m_fValue);break;
		 }
		 var.ToString(XFALSE);
	}
	XFDOUBLE ToFloat()
	{
		switch(m_nDataType)
		{
		case XODT_INT:m_fValue=(XFDOUBLE)m_iValue;break;
		case XODT_FLOAT:m_fValue;
		}
		m_nDataType=XODT_FLOAT;
		return m_fValue;
	}
	XFDOUBLE ToIntIf()
	{
		if(m_nDataType==XODT_FLOAT)
		{
			int v=(int)m_fValue;
			if(v==m_fValue)
			{
				m_nDataType=XODT_INT;
				m_iValue=v;
				return (XFDOUBLE)v;
			}
			return m_fValue;
		}
		return (XFDOUBLE)m_iValue;
	}
	int ToInt()
	{
		switch(m_nDataType)
		{
		case XODT_INT:return m_iValue;
		case XODT_FLOAT:m_iValue=(XU32)m_fValue;break;
		}
		m_nDataType=XODT_INT;
		return m_iValue;
	}
	XXObjectNumber(ScriptPlayer*pRoot,int v=0):XXObject(pRoot)
	{		
		//m_nObjType=XOOT_TYPE;
		SetValue(v);
	}
	void SetValue(int v)
	{
		m_iValue=v;
	//	m_nObjType=XOOT_TYPE;
		m_nDataType=XODT_INT;
	}
	XXObjectNumber(ScriptPlayer*pRoot,XFDOUBLE v):XXObject(pRoot)
	{		
		//m_nObjType=XOOT_TYPE;
		SetValue(v);
	}
	void SetValue(XFDOUBLE v)
	{
		int vv=(int)v;
		if(vv==v)
		{
			m_iValue=vv;
			m_nDataType=XODT_INT;
		}
		else
		{
			m_fValue=v;
			m_nDataType=XODT_FLOAT;
		}
	}
protected:
	void InitObject();
	union
	{
		//XU32  m_uValue;
		XS32  m_iValue;
		XFDOUBLE m_fValue;
	};
};


typedef XVector<XXObject*> XOBJLIST;


class XXObjectBool:public XXObject
{
	XDEC_XOBJECT(XXObjectBool);
public:

	//XFDOUBLE GetNumber(){return m_bValue;}
	int   GetInt(){return m_bValue;}
	void SetValue(XBOOL b){m_bValue=b;}
	XBOOL InitObject(){return XTRUE;}
	XBOOL IsTrue()
	{
		return m_bValue;
	}
	XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		if(list.GetSize()>0)
			pCnt->pStack->PushBool(list[0].ToLogic());
		else
			pCnt->pStack->PushBool(XFALSE);
		return XTRUE;
	}
	XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		if(list.GetSize())
			m_bValue=list[0].ToLogic();
		else
			m_bValue=XFALSE;
		pCnt->pStack->Push(this);
		return (XU32)this;
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_BOOLEAN;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	virtual void GetString(XXVar&var)//XString8&str)
	{
		var=m_bValue;
		var.ToLogic();
		var.ToString(XFALSE);
	}
	XXObjectBool(ScriptPlayer*pRoot,XU8 id=0):XXObject(pRoot)
	{
		//m_nObjType=XOOT_TYPE;
		m_nDataType=XODT_BOOLEAN;
		m_bValue=id;
	}
protected:
	XU8 m_bValue;
};


typedef void (*_ASFunctionCall)(XSWFCONTEXT&cnt,XXVARLIST&list,XXObject*pCaller);
/*class XXObjectStock:public XXObject
{
public:
	XXObjectStock(ScriptPlayer*pRoot,_ASFunctionCall pCall,XXObject*,XU8 id):
				 XXObject(pRoot)
	{
		m_pCall=pCall;
		m_nID=id;
	//	m_pCaller=pCaller;
	}
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*pData);
	//XU32 Handle(XSWFCONTEXT*pCnt,XXVARLIST*pList,void*pData,XU32 nType);	
protected:
	XU8			    m_nID;
	_ASFunctionCall m_pCall;
	//XXObject*		m_pCaller;
	//XXObject*		m_pHandler;
	//XXObject*		m_pCaller;
};

#define XXFUNCTION(TOKEN,CLASS)\
	static void _##TOKEN(XSWFCONTEXT&cnt,XXVARLIST&list,XXObject*pCaller)\
	{	((CLASS*)pCaller)->TOKEN(cnt,list); }*/

#endif // !defined(AFX_XXOBJECT_H__BAA7022F_581B_4095_B54B_F333CD48973C__INCLUDED_)
