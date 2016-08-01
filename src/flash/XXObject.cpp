// XXObject.cpp: implementation of the XXObject class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObject.h"
//#include "Action.h"
#include "splayer.h"
#include "ObjectString.h"
#include "ObjectFunction.h"
#include "XActionScript.h"
#include "ObjectCreator.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XXObject::XXObjects XXObject::_freeObjs;



void XXObject::FreeObjs()
{
	/*int nSize=_freeObjs.GetSize();
	while(nSize)
	{	
		XXObject**pData=(XXObject**)_freeObjs.DataTo();
		for(int i=0;i<nSize;i++)
			delete pData[i];
		delete pData;
		nSize=_freeObjs.GetSize();
		//objs.FreeAll();	
	}*/
	_freeObjs.Release();
}

void XXObject::FreeAll()
{
	//XXObject* p=_allObjs.objs.DataTo();
	//_freeObjs.Release();
	//_allObjs.ReleaseAll();
	//_freeObjs.RemoveAll();
//	while(_allObjs.GetCount())
	{
		_allObjs.DestroyAll();
		_freeObjs.Release();
		_allObjs.Release();
	}
}

//static _proto _proto_;



///////////////////////////////////////////////////////////////
XBOOL XXSortToken::Remove(XPCTSTR strName,XBOOL bLowCase)
{
	//XS32 tokenID=IndexOf(strName,XFALSE);
	//if(tokenID<0) return tokenID;
	int id=Index((void*)strName,bLowCase?XSS_NOCASE:XSS_NORMAL);
	if(id<0) return XFALSE;
	FREEOBJECT((*m_pTokens)[id]->pGetter);
	FREEOBJECT((*m_pTokens)[id]->pSetter);
	FreeToken((*m_pTokens)[id]);
	m_pTokens->RemoveAt(id);
	return XTRUE;
}


#define XXOBJECT(TOKEN) new _##TOKEN(m_pRoot)


XBOOL XXObject::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{		   
	case _SYSID(addListener):
	case _SYSID(removeListener):
	case _SYSID(broadcastMessage):
		var=this;
		var.ToFunction(id);
		return XTRUE;
	case _SYSID(listeners):
		{
				XXObjectArray*pArray=m_pRoot->m_pGlobal->CreateArray();
				_XEVENT*p=m_pEvents;
				while(p)
				{
					pArray->m_list.Push((p->pHandler));
					p=p->pNext;
				}
				var=pArray;
		}return XTRUE;
	case _SYSID(constructor)://constructor	
		var=this;return XTRUE;
	case _SYSID(_root):
		var=m_pRoot->rootObject->pObject;return XTRUE;
	case _SYSID(this):
		var=this;return XTRUE;
	case _SYSID(_parent):
		var=GetParent();return XTRUE;
	case _SYSID(__proto__):
		
		if(m_pSuper)
			var=m_pSuper;
		else 
			var.ToConst(XOCT_NULL);
		return XTRUE;
	case _SYSID(prototype)://prototype
		 //if(m_bBase&1)
		 var=this;
			 //pCnt->pStack->Push(this);	
			 //pCnt->pStack->Push((m_pRoot->m_pGlobal->GetObjBase()));
		 //else
		 //	 var=(m_pSuper?m_pSuper:this);
			//	pCnt->pStack->Push(this);
		 return XTRUE;
	case _SYSID(valueOf):
	case _SYSID(addProperty):
	case _SYSID(toString)://TOSTRING):
	case _SYSID(hasOwnProperty)://hasOwnProperty
	case _SYSID(isPropertyEnumerable)://isPropertyEnumerable
	case _SYSID(isPrototypeOf)://isPrototypeOf
	case _SYSID(registerClass):
	case _SYSID(unwatch):
	case _SYSID(watch):
		var=this;
		var.ToFunction(id);
		return XTRUE;
	}
	return XFALSE;
}

XBOOL XXObject::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	int nSize=list.GetSize();
	switch(id)
	{
		case _SYSID(addListener):
			if(nSize&&list[0].IsObject())
				AddListener(pCnt->baseThread,list[0].pObject);
			pCnt->pStack->PushBool(XTRUE);
			return XTRUE;
		case _SYSID(removeListener):
			if(nSize&&list[0].IsObject())
				RemoveListener(list[0].pObject);
			pCnt->pStack->PushBool(XTRUE);
			return XTRUE;
		case _SYSID(broadcastMessage):
			if(nSize)
			 {
				 //XXVar*pVar=&list[0];
				 XXVar var=list[0];
				 list.RemoveAt(0);
				 var.ToString(XFALSE);
				 
				 _XEVENT*p=m_pEvents;
				 while(p)
				 {
					 pCnt->baseThread=p->pThread;
					 pCnt->pThis=p->pHandler;
					 XXObject*pHandler=p->pHandler;
					 p=p->pNext;
					 if(pHandler)
					 {
						REFOBJECT(pHandler);
						pHandler->CallMethod(pCnt,var.strTxt,list,XTRUE);
						FREEOBJECT(pHandler);
					 }
						//HandlerEvent(cnt.pPlayer,cnt.baseThread,nCode,&list);
					 
				 }
			 }
			return XTRUE;
		case _SYSID(valueOf):ValueOf(pCnt);return XTRUE;
		case _SYSID(addProperty):
			 pCnt->pStack->PushBool(AddProperty(list));return XTRUE;
		case _SYSID(toString):
			{
				XXVar var;
				GetString(var);
				pCnt->pStack->Push(var);
			}return XTRUE;
		case _SYSID(hasOwnProperty)://hasOwnProperty
			if(nSize)
			{
				list[0].ToString(XFALSE);
				pCnt->pStack->PushBool(GetMemberOf(*pCnt,list[0].strTxt,XFALSE)!=XNULL);
			}return XTRUE;
		case _SYSID(isPropertyEnumerable)://isPropertyEnumerable
			if(nSize)
			{
				list[0].ToString(XFALSE);
				XXVar var;
				if(GetMember(*pCnt,list[0].strTxt,var,XFALSE)&&!
				   var.IsSysCall())
				   pCnt->pStack->PushBool(XTRUE);
				else
					pCnt->pStack->PushBool(XFALSE);
			}return XTRUE;
		case _SYSID(isPrototypeOf)://isPrototypeOf
			if(nSize&&list[0].IsObject())
			{
				pCnt->pStack->PushBool(IsSuperOf(list[0].pObject));
			}
			else
				pCnt->pStack->PushBool(XFALSE);
			return XTRUE;
		case _SYSID(registerClass):
			 	if(nSize>=2)
				{
					XXVar*pName=&(list)[0];
					XXVar*pClass=&(list)[1];
					//XXObject*pObject=XNULL;
					//if(pName->IsObject())
					//	pObject=pName->pObject;
					if(pClass->IsNull())
						m_pRoot->m_pGlobal->RegisterClass(*pCnt,*pName,XNULL);
					else if(pClass->IsObject())
						m_pRoot->m_pGlobal->RegisterClass(*pCnt,*pName,pClass->pObject);
					pCnt->pStack->PushBool(XTRUE);//(XXVar::CreateBool(XTRUE));
					//return XTRUE;
				}
				else
					pCnt->pStack->PushBool(XFALSE);//(XXVar::CreateBool(XTRUE));
				return XTRUE;
		case _SYSID(unwatch):
		case _SYSID(watch):
			pCnt->pStack->PushConst(XOCT_UNDEFINE);
			return XTRUE;
	}
	return XFALSE;
}

XBOOL XXObject::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(__proto__):
		if(var.IsObject())
		{
			SetSuper(var.pObject,XTRUE);
			m_pSuperObj=var.pObject;
			var.pObject->CopyMembers(this);
		}
		return XTRUE;
	}
	return XFALSE;
}
XBOOL XXObject::InitObject()
{
	_SYSCALL(constructor);//constructor
	_SYSCALL(addProperty);
	_SYSCALL(toString);
	_SYSCALL(_root);
	_SYSCALL(this);
	_SYSCALL(_parent);//_PARENT);
	_SYSCALL(valueOf);//VALUEOF);
	_SYSCALL(hasOwnProperty);//hasOwnProperty
	_SYSCALL(isPropertyEnumerable);//isPropertyEnumerable
	_SYSCALL(isPrototypeOf);//isPrototypeOf
	_SYSCALL(registerClass);
	_SYSCALL(unwatch);
	_SYSCALL(watch);
	_SYSCALL(prototype);
	//if(p->m_pSuper)
	_SYSCALL(__proto__);
/*	AddMember(XXVar(_strAddProperty),XXVar(XXOBJECT(addProperty)));
	//AddMember(XXVar(_strConstructor,XXVar(this));
	AddMember(XXVar(_strToString),XXVar(XXOBJECT(toString)));
	AddMember(XXVar(_strValueOf),XXVar(XXOBJECT(valueOf)));
	AddMember(XXVar(_strRegisterClass),XXVar(XXOBJECT(registerClass)));
	
	AddProperty(XXVar(_strConstructor),XXOBJECT(constructor),XNULL);
	AddProperty(XXVar(_strRoot),XXOBJECT(root),XNULL);
	//m_pSuper=this;
	AddProperty(XXVar(_strProtoType),XXOBJECT(constructor),XNULL);
	//	if(p->m_pSuper)
	AddProperty(XXVar(_str__Proto__),XXOBJECT(proto),XNULL);*/
//	SetSuper(this,XTRUE);
	return XTRUE;
}

XXObject* XXObject::m_pCreate=XNULL;

#ifdef _DEBUG
	int XXObject::nObjCount[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#endif
	XXObject::XXObjects XXObject::_allObjs;

XXObject::XXObject(ScriptPlayer*pRoot)
{
//	if(m_pRoot==XNULL)
//		int v=0;
	m_bNoCase=XTRUE;
	m_pRoot=pRoot;
	//InitObject();
	m_bBase=XFALSE;
	m_pTokens=XNULL;
	m_pSuper=XNULL;
	m_pSuperObj=XNULL;
	m_pNext=XNULL;
	m_pEvents=XNULL;
//	m_pContext=XNULL;//pRoot->rootObject->pObject;
	m_nRefs=0;
	//m_nObjType=XOOT_OBJECT;
	m_nDataType=XODT_OBJECT;
	m_pObject=XNULL;
#ifdef _DEBUG
	nObjCount[XXOBJ_OBJECT]++;
#endif
	_allObjs.Add(this);

//	nObjCount++;
	//nObjCount[GetObjectType()]++;
}

XXObject::~XXObject()
{
	//if(m_pCreate==this)
	//	int v=0;
	//Release();
#ifdef _DEBUG
	nObjCount[XXOBJ_OBJECT]--;
#endif
	_allObjs.Remove(this);

//	nObjCount--;
	//nObjCount[GetObjectType()]--;
}

//DEL void XXObject::DoEventEvent(XSWFCONTEXT &cnt, XU32 con)
//DEL {
//DEL 	//return;
//DEL 	cnt.thises.Add(this);
//DEL 	XXVARLIST list;
//DEL 	if(con&XClipActionRec::CLIPNEWDATA)
//DEL 		DoObjEvent(cnt,XOBJEVENT_DATA);
//DEL 		//CallMethod(&cnt,_strData,list,XTRUE);
//DEL 	if(con&XClipActionRec::CLIPKILLFOCUS)
//DEL 		DoObjEvent(cnt,XOBJEVENT_KILLFOCUS);
//DEL 		//CallMethod(&cnt,_strKillFocus,list,XTRUE);
//DEL 	if(con&XClipActionRec::CLIPSETFOCUS)
//DEL 		DoObjEvent(cnt,XOBJEVENT_SETFOCUS);
//DEL 		//CallMethod(&cnt,_strSetFocus,list,XTRUE);
//DEL 	if(con&XClipActionRec::CLIPLOAD)
//DEL 		DoObjEvent(cnt,XOBJEVENT_LOAD);
//DEL 		//CallMethod(&cnt,_strLoad,list,XTRUE);
//DEL 	if(con&XClipActionRec::CLIPENTERFRAME)
//DEL 		DoObjEvent(cnt,XOBJEVENT_ENTERFRAME);
//DEL 			//CallMethod(&cnt,_strEnterFrame,list,XTRUE);
//DEL 	{//Mouse
//DEL 		//list.Add(&XXVar(cnt.nPointX));
//DEL 		//list.Add(&XXVar(cnt.nPointY));
//DEL 		if(con&XActionCondition::OVERDOWNTOOUTDOWN)
//DEL 			DoObjEvent(cnt,XOBJEVENT_DRAGOUT);
//DEL 			//CallMethod(&cnt,_strDragOut,list,XTRUE);
//DEL 		if(con&XActionCondition::OUTDOWNTOOVERDOWN)
//DEL 			DoObjEvent(cnt,XOBJEVENT_DRAGOVER);
//DEL 			//CallMethod(&cnt,_strDragOver,list,XTRUE);				
//DEL 		/*if(con&CLIPPOINT)
//DEL 		{
//DEL 			if(con&XClipActionRec::CLIPPOINTDOWN)
//DEL 				CallMethod(&cnt,_strMouseDown,list,XTRUE);
//DEL 			if(con&XClipActionRec::CLIPPOINTUP)
//DEL 				CallMethod(&cnt,_strMouseUp,list,XTRUE);
//DEL 			if(con&XClipActionRec::CLIPPOINTMOVE)
//DEL 				CallMethod(&cnt,_strMouseMove,list,XTRUE);
//DEL 		}*/
//DEL 		if(con&XActionCondition::OVERUPTOOVERDOWN)
//DEL 		{
//DEL 			
//DEL 			DoObjEvent(cnt,XOBJEVENT_PRESS);
//DEL 		}
//DEL 			//CallMethod(&cnt,_strPress,list,XTRUE);
//DEL 		if(con&XActionCondition::OVERDOWNTOOVERUP)
//DEL 			DoObjEvent(cnt,XOBJEVENT_RELEASE);
//DEL 			//CallMethod(&cnt,_strRelease,list,XTRUE);
//DEL 		if(con&XActionCondition::OUTDOWNTOIDLE)
//DEL 			DoObjEvent(cnt,XOBJEVENT_RELEASEOUTSIDE);
//DEL 			//CallMethod(&cnt,_strReleaseOutside,list,XTRUE);
//DEL 		if(con&XClipActionRec::CLIPROLLOUT)
//DEL 			DoObjEvent(cnt,XOBJEVENT_ROLLOUT);
//DEL 			//CallMethod(&cnt,_strRollOut,list,XTRUE);
//DEL 		if(con&XClipActionRec::CLIPROLLOVER)
//DEL 			DoObjEvent(cnt,XOBJEVENT_ROLLOVER);
//DEL 			//CallMethod(&cnt,_strRollOver,list,XTRUE);
//DEL 		//list.RemoveAll();
//DEL 	}
//DEL 
//DEL 	cnt.thises.RemoveLast();
//DEL }


//DEL void XXObject::DoEventMouse(XSWFCONTEXT &cnt)
//DEL {
//DEL 	if(!cnt.nPointType) return;
//DEL 	cnt.thises.Add(this);
//DEL 	XXVARLIST list;
//DEL 	switch(cnt.nPointType)
//DEL 	{
//DEL 	case XSEVENT_PTDOWN:
//DEL 		 //CallMethod(&cnt,_strMouseDown,list,XTRUE);
//DEL 		 DoObjEvent(cnt,XOBJEVENT_MOUSEDOWN);
//DEL 		 break;
//DEL 	case XSEVENT_PTUP:
//DEL  		 //CallMethod(&cnt,_strMouseUp,list,XTRUE);
//DEL 		 DoObjEvent(cnt,XOBJEVENT_MOUSEUP);
//DEL 		 break;
//DEL 	case XSEVENT_PTONMOVE:
//DEL 	case XSEVENT_PTOVERMOVE:
//DEL          //CallMethod(&cnt,_strMouseMove,list,XTRUE);
//DEL 		 DoObjEvent(cnt,XOBJEVENT_MOUSEMOVE);
//DEL 		 break;
//DEL 	}
//DEL 	cnt.thises.RemoveLast();
//DEL }

//DEL void XXObject::DoEventKey(XSWFCONTEXT &cnt)
//DEL {
//DEL 	if(!cnt.nKeyType) return;
//DEL 	cnt.thises.Add(this);
//DEL 	XXVARLIST list;
//DEL 	//list.Add(&XXVar(cnt.nKeyCode));
//DEL 	switch(cnt.nKeyType)
//DEL 	{
//DEL 	case XSEVENT_KEYDOWN:
//DEL 		 DoObjEvent(cnt,XOBJEVENT_KEYDOWN);
//DEL 		 //list.Add((cnt.nKeyCode));
//DEL 		 //CallMethod(&cnt,_strKeyDown,list,XTRUE);
//DEL 		 break;
//DEL 	case XSEVENT_KEYUP:
//DEL 		 //list.Add((cnt.nKeyCode));
//DEL 		 //CallMethod(&cnt,_strKeyUp,list,XTRUE);
//DEL 		 DoObjEvent(cnt,XOBJEVENT_KEYUP);
//DEL 		 break;
//DEL 	}	
//DEL 	cnt.thises.RemoveLast();
//DEL }

#ifdef _VARCACHE_

inline XBOOL RestoreToken(_VARCACHE&c,XXTOKENS&tokens)
{
	int i,nSize=tokens.GetSize();
	for(i=c.nRefID+1;i<nSize;i++)
	{
		if(c.pToken==tokens[i])
		{
			c.nRefID=i;
			c.pToken=tokens[i];
			return XTRUE;
		}
	}
	for(i=c.nRefID-1;i>=0;i--)
	{
		if(c.pToken==tokens[i])
		{
			c.nRefID=i;
			c.pToken=tokens[i];
			return XTRUE;
		}
	}
	return XFALSE;
}

void XXObject::InstallEvent(int id, XXObject *pFun,XSWFCONTEXT*pCnt)
{
   if(id<0||id>=XOBJEVENT_MAX) return;


   if(id>=(int)m_events.GetSize())
   {
	   int s=m_events.GetSize();
	   m_events.SetSize(id+1);
	   for(int i=s;i<id+1;i++)
		   m_events[i]=XNULL;
   }
   m_events[id]=pFun;//->RefObject();//REFOBJECT(pFun);
   if(pCnt&&id==XOBJEVENT_LOAD)
   {
	  XU8 vars[sizeof(XXVar)*2];
	  XXVARLIST list(vars,2);
	  pFun->Function(pCnt,list,this);
   }
}

void XXObject::RemoveEvent(int id)
{
   if(id<0||id>(int)m_events.GetSize()) return;
   if(m_events[id]!=XNULL)
   {
		m_events[id]=XNULL;	
   }   
}

XBOOL XXObject::SetMemberCache(XSWFCONTEXT &cnt,const XXVar&name, XXVar &var)
{
	if(m_pTokens==XNULL) return XFALSE;
	//if(strcmp(strName,"ShotGo")==0)
	//		 int v=0;
	//XS32 tokenID=XXSortToken::IndexOf(strName,XTRUE);
	//if(tokenID<0) return XFALSE;
//	if(XString8(strName)=="onEnterFrame")
//		int v=0;
	_VARCACHE&c=cnt.varCache;	
	//if(c.pObjectThis!=this) return XFALSE;
	if(c.pObject->m_pTokens==XNULL||
	   c.nRefID>=c.pObject->m_pTokens->GetSize()) return XFALSE;
	if(c.pToken==XNULL) return XFALSE;
	
	XXTOKEN*pToken=(*c.pObject->m_pTokens)[c.nRefID];

	cnt.pOwer=this;
	SetValueOf(cnt,pToken,var,c.pObject);
	
	UpdateEditText(cnt,pToken,XNULL);

	if(c.nEventID>=0)
	{
		if(var.IsObject())
			InstallEvent(c.nEventID,var.pObject,&cnt);
		else
			RemoveEvent(c.nEventID);
	}
	return XTRUE;

}

XBOOL XXObject::GetMemberCache(XSWFCONTEXT&cnt,XPCTSTR strName,XXVar&var)
{

#ifdef _ANALY	
	//XString8 str="GetMember:";
	//str+=strName;
#endif
	_VARCACHE&c=cnt.varCache;	
	//if(c.pObjectThis!=this) return XFALSE;
	if(c.pObject->m_pTokens==XNULL||
	   c.nRefID>=c.pObject->m_pTokens->GetSize()) return XFALSE;
    
	if(c.pToken==XNULL)
		var=(XXObject*)c.nRefID;
	else
	{
		if(cnt.pOwer==XNULL)
			cnt.pOwer=this;
		//XXTOKEN*pToken=(*c.pObject->m_pTokens)[c.nRefID];
		if(c.pToken!=(*c.pObject->m_pTokens)[c.nRefID])
		{
			if(!RestoreToken(c,*c.pObject->m_pTokens)) return XFALSE;
			//int v=0;
		}
		LinkClip(c.pToken);
		GetValueOf(cnt,c.pToken,var,cnt.pObject);
	}
	return XTRUE;
}
#endif

XBOOL XXObject::GetMember(XSWFCONTEXT&cnt,XPCTSTR strName,XXVar&var,XBOOL bInc)
{
//	int nNum=Number(strName);
//	if(nNum>=0)
//		return GetMember(nNum,var,XNULL);

					   //+	m_pData	0x0a27e7a0 "__controller"

//	if(strcmp(strName,"prototype")==0)
//		int v=0;
#ifdef _ANALY	
	//XString8 str="GetMember:";
	//str+=strName;
#endif

#ifdef _VARCACHE_
	cnt.varCache.pObject=XNULL;
#endif
	XXTOKEN*pToken=XNULL;//
	cnt.pOwer=XNULL;
	pToken=GetMemberOf(cnt,strName,bInc);
	if(!pToken) 
	{
		//if(bInc)
		//	return GetClipOf(&cnt,strName,var);
		if(GetClipOf(&cnt,strName,var))
		{
			cnt.pOwer=this;
#ifdef _VARCACHE_
			if(var.IsObject())
			{
				cnt.varCache.pObjectThis=this;
				cnt.varCache.pObject=this;
				cnt.varCache.nRefID=(XU32)var.pObject;
				cnt.varCache.pToken=XNULL;
			}
#endif
			return XTRUE;
		}
		return XFALSE;
	}
	cnt.pToken=pToken->pOwer;
	if(cnt.pOwer==XNULL)
		cnt.pOwer=this;
	LinkClip(pToken);
	GetValueOf(cnt,pToken,var,cnt.pOwer?cnt.pOwer:this);
#ifdef _VARCACHE_
	cnt.varCache.pObjectThis=this;
#endif
	return XTRUE;


}


void XXObject::GetValueOf(XSWFCONTEXT&cnt,XXTOKEN*pToken,XXVar&var,XXObject*pCaller)
{
	if(!pToken) return;
	if(pToken->pGetter==XNULL)
	{
		if(pToken->member.IsSysCall())
			SysGetMember(pToken->member.nStringType,&cnt,var);
		else
			var=pToken->member;
	}
	else
	 {
		XU8 vars[sizeof(XXVar)*2];
		XXVARLIST list(vars,2);
		cnt.pThis=this;
		if(pToken->pOwer)
			cnt.pObject=pToken->pOwer;
		else if(cnt.pOwer)
			cnt.pObject=cnt.pOwer;
		pToken->pGetter->Function(&cnt,list,pCaller);
		//XXVar*pVar=cnt.pStack->Pop();
		cnt.pStack->Pop(var);
		//if(pVar)
		//{	var=*pVar;
		//	delete pVar;
		//}
	 }
}


void XXObject::SetSuper(XXObject*p,XU8 bInit)
{			
	if(m_pSuper==p) return;
	if(!p) {FREEOBJECT(m_pSuper);m_pSuper=p;}
	else if(!p->IsSubOf(this))
	{
		FREEOBJECT(m_pSuper);
		m_pSuper=REFOBJECT(p);
	}
	else 
		return;
	
}

XXTOKEN* XXObject::GetMemberOf(XSWFCONTEXT&cnt,XPCTSTR strKey,XU8 bInc)//XPCWSTR strName)
{
	 if(m_pTokens)
	 {
		XXSortToken sort(m_pTokens);
#ifdef _VARCACHE_
		XXTOKEN*pToken=sort.TokenOfOnly(strKey,cnt.varCache.nRefID,NeedLowCase());
		if(pToken) 
		{
			cnt.varCache.pObject=this;
			cnt.varCache.pToken=pToken;
			return pToken;
		}
#else
		XXTOKEN*pToken=sort.TokenOfOnly(strKey,NeedLowCase());
		if(pToken) 
			return pToken;
#endif
	 }
	 if(bInc&&m_pSuper) 
	 {
		XXTOKEN*pToken=m_pSuper->GetMemberOf(cnt,strKey,bInc);
		if(pToken) 
		{
//			FLASHASSERT(pToken->strKey==strKey);
			return pToken;
		}
	 }
/*	 if(bPar&&m_pParent)
	 {
		XXTOKEN*p=m_pParent->GetMemberOf(cnt,strKey,XFALSE,bPar);
		if(p&&cnt.pOwer==XNULL)
		{
			//cnt.nVarID=id;
			cnt.pOwer=m_pParent;
			//cnt.pCaller=m_pParent;
		}
		return p;
	 }*/
	 return XNULL;
}

XBOOL XXObject::FreeObject()
{
	//if(m_pObject&&m_pObject->character->tag==109)
	//	int v=0;
	if(!m_nRefs) 
		return XFALSE;
	if(m_nRefs) m_nRefs--;
	if(!m_nRefs) 
	{			
		
		//delete this;
		//Destroy();
		//delete this;
		//if((XU32)m_pCreate==(XU32)this)
		//	int v=0;
		//if(m_pRoot->rootObject&&
		 //  m_pRoot->rootObject->pObject==this)
		//	int v=0;
		Release();			
		_freeObjs.Add(this);
		return XTRUE;
	}
	return XFALSE;
}

XBOOL XXObject::GetClipOf(XSWFCONTEXT*pCnt,XPCTSTR name,XXVar&var)
{
	if(m_pObject==XNULL) return XFALSE;
	//if(m_pObject==XNULL) return XFALSE;
    //if(m_pDisp->GetClipOf(strName,*pVar)) return XTRUE;
	SObject*pObj=m_pObject->GetChildOf(name,XFALSE);
	if(pObj)
	{
		var=pObj->pObject;
		return XTRUE;
	}
	return XFALSE;
}

XBOOL XXObject::CallMethod(XSWFCONTEXT*pCnt,XPCTSTR strName, XXVARLIST&list,XU8 bEvent)
{
#ifdef _ANALY
//	XString8 str="CallMethod:";
//	str+=strName;
 //  _ANALYENTER(str);
#endif
   if(strName[0]==0)
   {
	   //NewObject(pCnt,&list,XFALSE);
	   //pCnt->pThis=this;
	   //pCnt->pObject=this;
	   Construct(pCnt,list,this);
	   pCnt->pToken=XNULL;
//	   _ANALYLEAVE();
	   return XTRUE;
   }
   //XXObject*pc=pCnt->pCaller;
   pCnt->pOwer=XNULL;
   pCnt->pToken=XNULL;
   XXVar var;
   if(bEvent)
   {
	   GetMember(*pCnt,strName,var,XFALSE);
   }
   else
	   GetMember(*pCnt,strName,var,XTRUE);

   //XString8 str=strName;

   if(var.IsSysCall())
   {
	   SysCallMethod(var.nStringType,pCnt,list);
   } 
   else if(!var.IsObject())
   {
	   //if(!bForce) return XFALSE;
	   if(!bEvent)
		   pCnt->pStack->PushConst(XOCT_UNDEFINE);//(XXVar::CreateConst(XOCT_UNDEFINE));
	   pCnt->pToken=XNULL;
//	   _ANALYLEAVE();
	   return XTRUE;
   }
   else 
   {
	   XXObject*pFun=var.pObject;
	   if(pCnt->pToken)
		   pCnt->pObject=pCnt->pToken;
	   else  if(pCnt->pOwer)
		   pCnt->pObject=pCnt->pOwer;
	   /*pCnt->pThis=this;
	   //if(m_pObject)
		 //  pCnt->thread=(XXObjectClip*)m_pObject->thread->rootObject->pObject;
	   //if(bThis)
	   //	   pCnt->thises.Add(pThis);
	   //if(pFun->IsObject(XXOBJ_FUNCTION)
	   if(pFun->IsObject(XXOBJ_FUNCTION))
		 pCnt->thread=((XXObjectFunction*)pFun)->GetBase();
	   else if(m_pObject)
	   {
		   XXObject*thread=m_pObject->thread->rootObject->pObject;
		   //pCnt->baseThread=thread;
		   //if(thread!=this)
		   pCnt->thread=thread;
	   }*/
		   //pCnt->thread=
		   //(((XXObjectFunction*)pFun)
	   pFun->Function(pCnt,list,pCnt->pOwer?pCnt->pOwer:this);
	   if(bEvent)
		   pCnt->pStack->FreePop();
	   //if(bThis) pCnt->thises.RemoveLast();
	   //if(pt) pCnt->contexts.RemoveLast();
	   //pCnt->pTarget=pTarget;
	   //pCnt->pToken=var.pObject;
   }
 //  pCnt->pCaller=pc; 
//   _ANALYLEAVE();
   return XTRUE;
  
}

XU32 XXObject::Function(XSWFCONTEXT*pCnt,XXVARLIST&,XXObject*)
{
   XXObject*pObj=new XXObject(m_pRoot);
   //pObj->m_pSuper=this;
   pObj->SetSuper((m_bBase&1)?this:m_pSuper,XFALSE);
   //pObj->SetType(XOOT_TYPE);
   pCnt->pStack->Push((pObj));
   return XTRUE;
}


XBOOL XXObject::SetMemberLocal(XSWFCONTEXT &cnt, XPCTSTR strName, XXVar &var,EditText*pEdit)
{
	if(m_pTokens==XNULL) return XFALSE;
	//if(strcmp(strName,"ShotGo")==0)
	//		 int v=0;
	//XS32 tokenID=XXSortToken::IndexOf(strName,XTRUE);
	//if(tokenID<0) return XFALSE;
//	if(XString8(strName)=="onEnterFrame")
//		int v=0;
#ifdef _VARCACHE_
	cnt.varCache.pObject=XNULL;
	XXSortToken sort(m_pTokens);
	XXTOKEN*pToken=sort.TokenOfOnly(strName,cnt.varCache.nRefID,NeedLowCase());
#else
	XXSortToken sort(m_pTokens);
	XXTOKEN*pToken=sort.TokenOfOnly(strName,NeedLowCase());
#endif
	if(pToken)//&&!pToken->bGlobal) 
	{
#ifdef _VARCACHE_
		cnt.varCache.pObject=this;
		cnt.varCache.pObjectThis=this;
		cnt.varCache.pToken=pToken;
#endif
		cnt.pOwer=this;
		SetValueOf(cnt,pToken,var,this);
		UpdateEditText(cnt,pToken,pEdit);

		if(var.IsObject())
			InstallEvent(strName,var.pObject,&cnt);
			//Handle(&cnt,(XXVARLIST*)strName,var.pObject,XOH_SETEVENT);
		else
			RemoveEvent(strName);
			//Handle(XNULL,XNULL,(void*)strName,XOH_REMOVEEVENT);
		//return XTRUE;

		return XTRUE;
		//return pToken;
	}

	return XFALSE;
}


XU32 XXObject::SetMember(XSWFCONTEXT&cnt,const XXVar&name,XXVar&var,EditText*pEdit)//XXObject*pObj)
{
	//if(strcmp(name.strTxt,"ShotGo")==0)
	//		 int v=0;
	XXTOKEN*pToken=XNULL;//GetMemberOf(cnt,tokenID,XTRUE,bParent);
	XXObject*pCaller=this;
#ifdef _VARCACHE_
	cnt.varCache.pObject=XNULL;
#endif
	//if(pToken==XNULL)
	{
		pToken=GetMemberOf(cnt,name.strTxt,XTRUE);
		if(pToken)
		{
			if(pToken->member.IsSysCall())
			{
				if(!SysSetMember(pToken->member.nStringType,&cnt,var)) pToken=XNULL;
				else 
				{
#ifdef _VARCACHE_
					cnt.varCache.pObjectThis=this;
#endif
					return XTRUE;
				}
			}
		}
	}
	if(pToken==XNULL)
	{

		if(!InitTokens())
		 return 0;
		XXSortToken sort(m_pTokens);
#ifdef _VARCACHE_
		pToken=sort.TokenOf(name,cnt.varCache.nRefID,NeedLowCase(),XTRUE);
		if(pToken==XNULL)
		{			
			return XFALSE;
		}
#else
		pToken=sort.TokenOf(name,NeedLowCase(),XTRUE);
		if(pToken==XNULL)
		{
//			_ANALYLEAVE();
			return XFALSE;
		}
#endif
#ifdef _VARCACHE_
		cnt.varCache.pObject=this;
		cnt.varCache.pToken=pToken;
#endif
		//cnt.nVarID=tokenID;
		cnt.pOwer=this;
	}
#ifdef _VARCACHE_
	cnt.varCache.pObjectThis=this;
#endif
	SetValueOf(cnt,pToken,var,pCaller);
	
	UpdateEditText(cnt,pToken,pEdit);
#ifdef _VARCACHE_
	if(var.IsObject())
		cnt.varCache.nEventID=InstallEvent(name.strTxt,var.pObject,&cnt);
		//Handle(&cnt,(XXVARLIST*)strKey,var.pObject,XOH_SETEVENT);
	else
		cnt.varCache.nEventID=RemoveEvent(name.strTxt);
#else
	if(var.IsObject())
		InstallEvent(name.strTxt,var.pObject,&cnt);
		//Handle(&cnt,(XXVARLIST*)strKey,var.pObject,XOH_SETEVENT);
	else
		RemoveEvent(name.strTxt);
#endif
		//Handle(XNULL,XNULL,(void*)strKey,XOH_REMOVEEVENT);
//	return XTRUE;
//	_ANALYLEAVE();
	return XTRUE;
	//return AddMember(pCnt,strName,var);//pObj);
}




XXObject* XXObject::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&,XBOOL)
{
   XXObject*pObj=new XXObject(m_pRoot);
   if(pObj)
   {
	pObj->SetSuper(this,XFALSE);//(m_bBase&1)?this:m_pSuper,XFALSE);
	pObj->m_pSuperObj=GetSuperObject();
    CopyMembers(pObj);
   }
   pCnt->pStack->Push((pObj));
   return pObj;
}

XU32 XXObject::NewMethod(XPCTSTR strName,XSWFCONTEXT *pCnt, XXVARLIST *pList,XU8 bObj,XU8 bForce)
{
   //XS32 id=XXSortToken::IndexOf(strName,XFALSE);
   XXVar var;
   //if(id>=0)
   if(!GetMember(*pCnt,strName,var,XTRUE)) return XFALSE;
   if(!var.IsObject())
   {
	   if(bForce)
		   pCnt->pStack->PushConst(XOCT_UNDEFINE);//(XXVar::CreateConst(XOCT_UNDEFINE));
	   else
		  return XFALSE;
   }
   else
   {
	   //pCnt->thises.Add(var.pObject);
	   if(var.IsSysCall())
	   {
		   var.nObject=(XGlobal::RandInt()&0x7f)+1;
		   pCnt->pStack->Push(var);
	   }
	   else
	   {
		pCnt->pThis=var.pObject;
		var.pObject->CreateObject(pCnt,*pList,bObj);
	   }
	   //pCnt->thises.RemoveLast();
	   //XXVar*pVar=pCnt->pStack->GetLast();
	   //if(pVar&&pVar->IsObject())
		//   pVar->pObject->SetType(bObj?XOOT_OBJECT:XOOT_TYPE);
   }
   return XTRUE;
}

//DEL XU32 XXObject::GetValue(XSWFCONTEXT &cnt)
//DEL {
//DEL 	cnt.pStack->Push((this));
//DEL 	return XTRUE;
//DEL }

XFDOUBLE XXObject::GetNumber()
{
	switch(m_nDataType)
	{
	case XODT_BOOLEAN:
		 return ((XXObjectBool*)this)->GetInt();
	case XODT_INT:
	case XODT_FLOAT:
		 return ((XXObjectNumber*)this)->GetNumber();
	case XODT_STRING:
		 return ((XXObjectString*)this)->GetNumber();
	/*case XODT_OBJECT:
		 XXVar var;
		 if(GetMember(0,&var,XNULL))
		 {
			 return var.ToInt();
		 }*/
	default:
		{
			XXVar var;
			if(GetMember(0,var,XNULL))
				return var.ToFloat();
		}
		 return 0;
	}
}

int XXObject::GetInt()
{
	switch(m_nDataType)
	{
	case XODT_BOOLEAN:
		 return ((XXObjectBool*)this)->GetInt();
	case XODT_INT:
	case XODT_FLOAT:
		 return ((XXObjectNumber*)this)->GetInt();
	case XODT_STRING:
		 return ((XXObjectString*)this)->GetInt();
	default:
		 {
			XXVar var;
			if(GetMember(0,var,XNULL))
				return var.ToInt();
		}
		 return 0;
	}
}

XBOOL XXObject::IsTrue()
{
   switch(m_nDataType)
   {
   case XODT_BOOLEAN:
	    return ((XXObjectBool*)this)->IsTrue();
   case XODT_CONST:
	    return XFALSE;
	    //return ((XXObjectConst*)this)->IsTrue();
   case XODT_INT:
   case XODT_FLOAT:
	    return ((XXObjectNumber*)this)->IsTrue();
   default:
	    return XTRUE;
   }
}



//////////////////////////////////////////////////////////////////////////////
XXObject* XXObjectBool::CreateObject(XSWFCONTEXT *pCnt,XXVARLIST&list,XBOOL)
{
   XXObjectBool*pObj=CreateClass(this,XFALSE,m_pRoot);//m_bBase?this:m_pSuper,XFALSE,m_pRoot);
   if(pObj)
   {
	   pObj->m_pSuperObj=GetSuperObject();
	   if(list.GetSize()>0)
		  pObj->m_bValue=list[0].ToLogic();
   }
   /*if(list.GetSize()<=0)
	   pObj=XXObjectCreator::CreateBool(XFALSE);
   else
	   pObj=XXObjectCreator::CreateBool(list[0]->ToLogic());*/
   //pObj->SetSuper((m_bBase&1)?this:m_pSuper,XFALSE);

   pCnt->pStack->Push((pObj));
   return pObj;
}

//DEL XU32 XXObjectBool::Function(XSWFCONTEXT &cnt,XXVARLIST&list)
//DEL {
//DEL    //XXObjectBool*pObj;
//DEL    XXVar*pVar;
//DEL    if(list.GetSize()<=0)
//DEL 	   pVar=XXVar::CreateBool(XFALSE);//new XXObjectBool();
//DEL    else
//DEL 	   pVar=XXVar::CreateBool(list[0]->ToLogic());
//DEL    //pObj->SetSuper(m_bBase?this:m_pSuper);
//DEL    //pObj->SetType(XOOT_TYPE);
//DEL    cnt.pStack->Push(pVar);
//DEL    return XTRUE;
//DEL }
///////////////////////////////////////////////////////////

/*XXObject* XXObjectConst::CreateObject(XSWFCONTEXT *pCnt,XXVARLIST&list,XBOOL)
{
   XXVar var;

   if(list.GetSize()<=0)
	   var.ToConst(XOCT_UNDEFINE);
   else
	   var=list[0];
   pCnt->pStack->Push(var);//pVar);
   return XNULL;
}

XU32 XXObjectConst::Function(XSWFCONTEXT *pCnt,XXVARLIST&list)
{
   XXVar var;//*pObj;
   if(list.GetSize()<=0)
	   var.ToConst(XOCT_UNDEFINE);
	   //pObj=XXVar::CreateConst(XOCT_UNDEFINE);
   else
	   var=list[0];
	   //pObj=new XXVar(*list[0]);
   pCnt->pStack->Push(var);//pObj);
   return XTRUE;
}*/

//DEL XU32 XXObjectNumber::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list)
//DEL {
//DEL    XXObject*pObj;
//DEL    if(list.GetSize()<=0)
//DEL 	   pObj=new XXObjectNumber(0);
//DEL    else
//DEL 	   pObj=new XXObjectNumber(list[0]->ToNum());//new XXObjectNumber(list[0]->GetNumber());
//DEL    //pObj->Init();
//DEL    pObj->SetSuper((m_bBase&1)?this:m_pSuper,XFALSE);
//DEL    //pObj->SetType(XOOT_OBJECT);
//DEL    pCnt->pStack->Push(new XXVar(pObj));
//DEL    return XTRUE;
//DEL }

#include <math.h>

/////////////////////////////////////////////////////////////
//
void XXObjectNumber::InitObject()
{
	//AddMember(XXVar(_strConstructor,XXVar(this));
	AddMember(_SYSVAR(MAX_VALUE),XXVar(3.402823466e+38F));
	AddMember(_SYSVAR(MIN_VALUE),XXVar(1.175494351e-38F));
	AddMember(_SYSVAR(NaN),XXVar(XODT_CONST,XOCT_NAN));
	AddMember(_SYSVAR(NEGATIVE_INFINITY),XXVar(XODT_CONST,XOCT_INFINITY));
	AddMember(_SYSVAR(POSITIVE_INFINITY),XXVar(XODT_CONST,XOCT_RINFINITY));
}

XXObject* XXObjectNumber::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL bObj)
{
   XXObject*pObj=CreateClass(m_bBase?this:m_pSuper,XFALSE,m_pRoot);
   if(list.GetSize()>0)
	   SetValue(list[0].ToFloat());
	//   pObj=XXObjectCreator::CreateNumber(0);
   //else
	 //pObj=XXObjectCreator::CreateNumber(list[0]->ToNum());//new XXObjectNumber(list[0]->GetNumber());
   //pObj->Init();
   //pObj->SetType(XOOT_TYPE);
   pCnt->pStack->Push((pObj));
   return pObj;	
}





/*ScriptThread* XXObject::GetTarget(XSWFCONTEXT *pCnt)
{
	XXObject*pParent=this;//GetParent();
	while(pParent)
	{
		if(pParent->GetObjectType()==XXOBJ_CLIP)
			return ((XXObjectClip*)pParent)->m_pDisp;
		pParent=pParent->GetParent();
	}
	return pCnt->GetTarget();
}*/

int XXObject::EnumVariables(XSWFCONTEXT &cnt)
{
	//if(m_pTokens==XNULL) return 0;
	//return 0;
	int k=0;
	if(IsArray())
	{
		XXVar var,name;
		//XString8 strName;
		while(GetMember(k,var,&name))
		{
			if(name.IsString()&&!name.IsNull())//strName.GetLength())
				cnt.pStack->Push(name);//new XXVar(strName,XTRUE));
			else
				cnt.pStack->Push(k);//new XXVar(k));
//			name=_strEmpty;
			name.Release();
//			strName.Empty();
			k++;
		}
		return k;
	}
	
	if(m_pObject)
	{
		SObject*p=m_pObject->bottomChild;
		while(p)
		{
			if(p->pObject&&!p->name.IsNull()) //continue;
			{
				cnt.pStack->Push(p->name);//new XXVar(p->name));
				k++;
			}
			p=p->above;
		}
	}
	if(m_pTokens)
	{
		int nSize=m_pTokens->GetSize();
		for(int i=0;i<nSize;i++)
		{
			cnt.pStack->Push(((*m_pTokens)[i]->strKey));
			k++;
		}
	}
	return k;
	/*int k=0;
	XXVar var;
	XString8 strName;
	while(GetMember(k,var,&strName))
	{
		if(strName.GetLength())
			cnt.pStack->Push(new XXVar(strName,XTRUE));
		else
			cnt.pStack->Push(new XXVar(k));
		strName.Empty();
		k++;
	}
	return k;*/
}

XBOOL XXObject::IsInstanceOf(XXObject *pObj)
{
   XXObject*p=m_pSuper;
   while(p)
   {
	   if(p==pObj) return XTRUE;
	   p=p->GetSuper();
   }
   return XFALSE;
}

void XXObject::ClearLocalVar()
{
   if(!m_pTokens) return;
   XU32 nSize=m_pTokens->GetSize();
   for(int i=nSize-1;i>=0;i--)
   {
	   XXTOKEN*p=(*m_pTokens)[i];
	   if(p->bGlobal) continue;
	   FREEOBJECT(p->pGetter);
	   FREEOBJECT(p->pSetter);
	   delete p;
	   m_pTokens->RemoveAt(i);
   }
}


XBOOL XXObject::IsNumber3()
{
	switch(m_nDataType)
	{
		case XODT_INT:
		case XODT_FLOAT:return XTRUE;
		case XODT_STRING:
			{
			XXObjectString* p=(XXObjectString*)this;
			return p->m_strValue.IsNum();
			}
			break;
	}
	return XFALSE;
}



void XXObject::SetValueOf(XSWFCONTEXT &cnt, XXTOKEN *pToken, XXVar &var,XXObject*pCaller)
{
	if(pToken->pSetter==XNULL)
	{
		if(pToken->member.IsSysCall())
		{
			SysSetMember(pToken->member.nStringType,&cnt,var);
				//pToken->member=var;
		}
		else
		{
		//	if(strcmp(pToken->strKey.strTxt,"ShotGo")==0)
		//		int v=0;
			pToken->member=var;
			pToken->pOwer=XNULL;
		}
	}	
	 else
	 {
		XU8 vars[sizeof(XXVar)*2];
		XXVARLIST list(vars,1);
		list.Push(var);//new XXVar(var));
		cnt.pThis=this;
		if(pToken->pOwer)
			cnt.pObject=pToken->pOwer;
		else if(cnt.pOwer)
			cnt.pObject=cnt.pOwer;
		pToken->pSetter->Function(&cnt,list,pCaller);
		list.Release();
		//list.FreeAll();
	 }
}

XBOOL XXObject::GetIndex(XU32 id, XXVar &var,XXVar*name)//XString8 *pName)
{
	if(!m_pTokens) return XFALSE;
	if(id<m_pTokens->GetSize())
	 {
		 XXTOKEN*pToken=(*m_pTokens)[id];
		 var=pToken->member;
		 if(name)
		 {
			 *name=pToken->strKey;
			//XPCWSTR str=XXSortToken::NameOf(pToken->nID);
			// if(str)
			//	 *pName=str;
		 }
		 return XTRUE;
	 }
	return XFALSE;
}

XBOOL XXObject::FindVariable(XPCTSTR strKey, XXVar &var)
{
	if(m_pTokens)
	 {
		XXSortToken sort(m_pTokens);
		XXTOKEN*pToken=sort.TokenOfOnly(strKey,NeedLowCase());
		if(pToken) 
		{
			LinkClip(pToken);
			var=pToken->member;
			//cnt.pOwer=this;
			//cnt.nVarID=id;
			//cnt.pCaller=this;
			//return pToken;
			return XTRUE;
		}
	 }
	return XFALSE;
}

ScriptThread* XXObject::GetRoot()
{
	if(m_pRoot) return m_pRoot;
	ScriptThread*pThread=GetTarget();
	if(pThread)
	{
		return pThread->player;
	}
	return XNULL;
}




void XXObject::UpdateEditText(XSWFCONTEXT&cnt,XXTOKEN *pToken, EditText *pEdit)
{
	if(!pToken) return;
	if(pEdit&&m_pObject&&m_pObject->character->tag==92)
		int v=0;
	if(pEdit)
	{
		EditText*p=pToken->pEdits;
		while(p)
		{
			if(p==pEdit) return;
			p=p->pNext;
		}
		pEdit->pNext=pToken->pEdits;
		pToken->pEdits=pEdit;
	}
	else if(pToken->pEdits)
	{
		EditText*p=pToken->pEdits;
		XXVar var;
		GetValueOf(cnt,pToken,var,cnt.pOwer?cnt.pOwer:this);
		var.ToString(XFALSE);
		if(m_pRoot->version<=5)
		{
			XString8 str;
			str.ConvertFrom(var.strTxt,XCHARSET_GB2312);
			var.SetString(str.GetData(),str.GetLength());
		}
		//U16* txt=p->MBCSToWide(var.strData,0,strlen(var.strData));
		while(p)
		{
			//char*ps=p->GetBuffer();
			//XXVar var;
			//p->
			//if(XString8::Compare(var.strTxt,ps))
			{
				if(p->IsHtml())
					p->SetHTMLText(&cnt,var.strTxt);
				else
					p->SetBuffer(var.strTxt,false);
				//p->m_obj->Modify();
			}
			//FreeStr(ps);
			p=p->pNext;
		}
		//delete txt;
	}
}

XBOOL XXObject::RemoveEditText(XSWFCONTEXT &cnt, XPCTSTR strName, EditText *pEdit)
{
	if(m_pTokens==XNULL) return XFALSE;
	XXSortToken sort(m_pTokens);
	XXTOKEN*pToken=sort.TokenOfOnly(strName,NeedLowCase());
	if(pToken)//&&!pToken->bGlobal) 
	{
		EditText**pRoot=&pToken->pEdits;
		for(;;)
		{
			EditText*pe=*pRoot;
			if(!pe) break;
			if(pe==pEdit)
			{
				*pRoot=pe->pNext;
			}
			else
				pRoot=&pe->pNext;
		}
		return XTRUE;
		//return pToken;
	}

	return XFALSE;
}

XBOOL XXObject::HasEvent(XU32 f)
{
	for(int i=0;i<(int)m_events.GetSize();i++)
	{
		if(m_events[i]&&(f&i))
			return XTRUE;
	}
	_XEVENT*p=m_pEvents;
	while(p)
	{
		if(p->pHandler->HasEvent(f)) return XTRUE;
		p=p->pNext;
	}
	return m_pSuper?m_pSuper->HasEvent(f):XFALSE;
}

XU32 XXObject::HandlerEvent(XSWFPlayer*pPlayer,XXObject*baseThread, XU32 id,XXVARLIST*pArgs)
{
   //if(id>=m_events.GetSize()&&m_pEvents==XNULL) return XFALSE;
   //if(m_pObject&&m_pObject->character->tag==13)
	//   int v=0;
   //if(id==XOBJEVENT_RELEASE) return XFALSE;
   XBOOL bOK=XFALSE;
   XXObject*pThis=this;
   //XXObjectClip*pThread=baseThread;
   SObject*pBaseAll=m_pObject;
   if(!pBaseAll&&baseThread)
	   pBaseAll=baseThread->m_pObject;
   if(!pBaseAll)
	   pBaseAll=m_pRoot->rootObject;

   if(!pBaseAll) return XFALSE;

 //  if(id==XOBJEVENT_RELEASE) //return XFALSE;
//	   int v=0;


   XXObject*pET=this;
   //while(pET&&!bOK)
   {

	   if(id<pET->m_events.GetSize())
	   {
		   if(pET->m_events[id]!=XNULL&&pET->m_events[id]->IsObject(XXOBJ_FUNCTION))
		   {
			   XXObjectFunction*pFun=(XXObjectFunction*)pET->m_events[id];
			   XXObject*pBaseObj=pFun->GetBase();
			   SObject*pBase=pBaseObj?pBaseObj->m_pObject:XNULL;
			   if(pBase==XNULL) pBase=pBaseAll;
		//	   if(id==XOBJEVENT_ENTERFRAME&&
		//		  pBase->character->tag==343)
		//			int v=0;
			   pPlayer->display.PushAction((XU8*)pFun->RefObject(),
											pBase,pThis,XActionList::XA_FUNCTION,pArgs);
			   bOK=XTRUE;
		   }
	   }
	   _XEVENT*p=pET->m_pEvents;
	   while(p)
	   {
		   //_XEVENT*p=*pe;
		   //if(!p) break;
		   //if(p->pHandler->
		   XXObject*pObj=p->pHandler;
		   if(id<pObj->m_events.GetSize())
		   {
			if(pObj->m_events[id]!=XNULL&&pObj->m_events[id]->IsObject(XXOBJ_FUNCTION))
			   {
				   XXObjectFunction*pFun=(XXObjectFunction*)pObj->m_events[id];
				   if(pFun->GetBase()!=XNULL)
				   {
					   XXObject*p=pFun->GetBase();
					   SObject*pBase=p->m_pObject;//:XNULL;
					   if(pBase==XNULL) pBase=pBaseAll;
					   pPlayer->display.PushAction((XU8*)pFun->RefObject(),
													pBase,pObj,XActionList::XA_FUNCTION,pArgs);
					   bOK=XTRUE;
				   }
			   }
		   }
		   p=p->pNext;

	   }
	   pET=pET->m_pSuper;

   }
   /*if(m_pSuper)
   {
	   if(m_pSuper->HandlerEvent(pPlayer,baseThread,id,pArgs)) bOK=XTRUE;
   }*/
   return bOK;

   //return m_pSuper?m_pSuper->HandlerEvent;
}

XXObject* XXObject::GetParent()
{
	if(!m_pObject||!m_pObject->parent) return XNULL;
	if(m_pObject->character&&
	   m_pObject->character->type==rootChar) return XNULL;
	return m_pObject->thread->rootObject->parent->pObject;
}



XBOOL XXObject::HasMouseEvent()
{
	//if(m_pEvents) return XTRUE;
	//if(!m_events) return XFALSE;
	

	if(m_events.GetSize())
	{XU8 nID[]={
	  XOBJEVENT_DRAGOUT,
	  XOBJEVENT_DRAGOVER,
	  XOBJEVENT_MOUSEDOWN,
	  XOBJEVENT_MOUSEMOVE,
	  XOBJEVENT_MOUSEUP,
	  XOBJEVENT_PRESS,
	  XOBJEVENT_RELEASE,
	  XOBJEVENT_RELEASEOUTSIDE,
	  XOBJEVENT_ROLLOUT,
	  XOBJEVENT_ROLLOVER,
	};
	XXVar var;
	 for(XU32 i=0;i<sizeof(nID);i++)
	 {
		 //if(FindVariable(_strEvents[nID[i]],var)) return XTRUE;
		 if(i>=m_events.GetSize()) break;
		 if(m_events[nID[i]]) return XTRUE;
	 }
	}
	return m_pSuper?m_pSuper->HasMouseEvent():XFALSE;
}

XBOOL XXObject::HasPressEvent()
{
	//if(m_pEvents) return XTRUE;
	//if(!m_events) return XFALSE;
	

	if(m_events.GetSize())
	{XU8 nID[]={
	  XOBJEVENT_MOUSEDOWN,
	  XOBJEVENT_MOUSEUP,
	  XOBJEVENT_PRESS,
	  XOBJEVENT_RELEASE,
	};
	XXVar var;
	 for(XU32 i=0;i<sizeof(nID);i++)
	 {
		 //if(FindVariable(_strEvents[nID[i]],var)) return XTRUE;
		 if(i>=m_events.GetSize()) break;
		 if(m_events[nID[i]]) return XTRUE;
	 }
	}
	return m_pSuper?m_pSuper->HasMouseEvent():XFALSE;
}

XBOOL XXObject::HasKeyEvent()
{

	if(m_events.GetSize())
	{ XU8 nID[]={
	  XOBJEVENT_KEYDOWN,
	  XOBJEVENT_KEYUP,
	};
	 //XXVar var;
	 for(XU32 i=0;i<sizeof(nID);i++)
	 {
		 //if(FindVariable(_strEvents[nID[i]],var)) return XTRUE;
		 if(i>=m_events.GetSize()) break;
		 if(m_events[nID[i]]) return XTRUE;
	 }
	}
	return m_pSuper?m_pSuper->HasKeyEvent():XFALSE;
}

XBOOL XXObject::HasAnyEvent()
{
	if(m_pEvents) return XTRUE;
	//XXVar var;
	//for(XU32 i=0;i<sizeof(_strEvents)/sizeof(XPCTSTR);i++)
	//	if(FindVariable(_strEvents[i],var)) return XTRUE;	
	//if(m_events[i]) return XTRUE;
	for(int i=0;i<(int)m_events.GetSize();i++)
		if(m_events[i]) return XTRUE;
	return m_pSuper?m_pSuper->HasAnyEvent():XFALSE;
}




XBOOL XXObject::LinkClip(XXTOKEN*pToken)
{
	if(!pToken) return XFALSE;
	if(pToken->pGetter) return XFALSE;
	if(!pToken->member.IsObject(XXOBJ_CLIP)) return XFALSE;
	if(pToken->member.pObject->m_pObject) return XTRUE;
	XXObjectClip*pClip=(XXObjectClip*)pToken->member.pObject;
	if(pClip->m_strPath.IsNull()) return XFALSE;
	
	SObject*pObj=m_pRoot->splayer->FindTarget(m_pRoot->rootObject,pClip->m_strPath.strTxt,XNULL);
	if(pObj)
		pToken->member=pObj->pObject;
	else return XFALSE;
	return XTRUE;
}

//DEL XXObject* XXObject::GetMemberOf(XPCTSTR name)
//DEL {
//DEL 	if(m_pObject)
//DEL 	{
//DEL 		SObject* p=m_pObject->GetChildOf(name,XFALSE);
//DEL 		if(p&&p->pObject) return p
//DEL 	}
//DEL }

XXObject* XXObject::GetMember(char *name)
{
	//XXObject*pObj=XNULL;
	
	XXVar var;
	if(FindVariable(name,var)&&var.IsObject())
		return var.pObject;
	if(m_pObject)
	{
		SObject*p=m_pObject->GetChildOf(name,XFALSE);
		if(p)
			return p->pObject;
			//pObj=p->pObject;
	}
	return XNULL;
}

XU32 XXObject::SetMemberOnly(XSWFCONTEXT &cnt, XPCTSTR strKey, XXVar &var, EditText *pEdit)
{
	//if(strcmp(strKey,"ShotGo")==0)
	//		 int v=0;
	XXTOKEN*pToken=XNULL;//GetMemberOf(cnt,tokenID,XTRUE,bParent);
	XXObject*pCaller=this;
#ifdef _VARCACHE_
	cnt.varCache.pObject=XNULL;
#endif
	//if(pToken==XNULL)
	{
		pToken=GetMemberOf(cnt,strKey,XTRUE);
	}
	if(pToken==XNULL) return XFALSE;

	SetValueOf(cnt,pToken,var,pCaller);
	
	UpdateEditText(cnt,pToken,pEdit);

	if(var.IsObject())
		InstallEvent(strKey,var.pObject,&cnt);
		//Handle(&cnt,(XXVARLIST*)strKey,var.pObject,XOH_SETEVENT);
	else
		RemoveEvent(strKey);
#ifdef _VARCACHE_
	cnt.varCache.pObjectThis=this;
#endif
		//Handle(XNULL,XNULL,(void*)strKey,XOH_REMOVEEVENT);
//	return XTRUE;

	return XTRUE;
}

void XXObject::AddListener(XXObject *thread, XXObject *pObj)
{
	_XEVENT*p=new _XEVENT;
	if(!p) return;
	p->pHandler=pObj->RefObject();
	p->pThread=thread;
	p->pNext=XNULL;
	_XEVENT**pi=&m_pEvents;
	for(;;)
	{
		_XEVENT*po=*pi;
		if(po)
			pi=&po->pNext;
		else
		{
			*pi=p;
			break;
		}
	}
}

void XXObject::ReleaseEvents()
{
	_XEVENT*p=m_pEvents;
	m_pEvents=XNULL;
	while(p)
	{
		p->pHandler->FreeObject();
		_XEVENT*pi=p;
		p=p->pNext;
		delete pi;
	}
	
	m_events.RemoveAll();
}

void XXObject::RemoveListener(XXObject *pObj)
{
	_XEVENT**p=&m_pEvents;
	for(;;)
	{
		_XEVENT*pi=*p;
		if(!pi) break;

		if(pi->pHandler==pObj)
		{
			pObj->FreeObject();
			*p=pi->pNext;
			delete pi;
		}
		else
			p=&pi->pNext;
	}
}

int XXObject::GetEventCode(XPCTSTR strCode)
{
	switch(strCode[0])
	{
	case 'o':
	case 'O':break;
	default:return -1;
	}
	XBOOL bLowCase=NeedLowCase();
    XSortConstStr sort((XPCTSTR*)_strEvents,XOBJEVENT_UNLOAD+1);
    return sort.Index((void*)strCode,bLowCase?XSS_NOCASE:XSS_NORMAL);
}

int XXObject::InstallEvent(XPCTSTR strName, XXObject *pFun,XSWFCONTEXT*pCnt)
{
	switch(strName[0])
	{
	case 'o':
	case 'O':break;
	default:return -1;
	}
	XBOOL bLowCase=m_pRoot->NeedLowCase();
   XSortConstStr sort((XPCTSTR*)_strEvents,XOBJEVENT_UNLOAD+1);
   int id=sort.Index((void*)strName,bLowCase?XSS_NOCASE:XSS_NORMAL);
   if(id<0) return id;

 //  if(m_pObject&&m_pObject->character->tag==712)
//	   int v=0;
//   if(m_pObject)
//	   int v=0;

   if(id>=(int)m_events.GetSize())
   {
	   int s=m_events.GetSize();
	   m_events.SetSize(id+1);
	   for(int i=s;i<id+1;i++)
		   m_events[i]=XNULL;
   }
   m_events[id]=pFun;//->RefObject();//REFOBJECT(pFun);
   /*if(pCnt&&id==XOBJEVENT_LOAD)
   {
	  XU8 vars[sizeof(XXVar)*2];
	  XXVARLIST list(vars,2);
	  pFun->Function(pCnt,list,this);
   }*/
   return id;
   //m_events.InsertAt(id,pFun);
   //return XTRUE;
	//return XFALSE;
}

int XXObject::RemoveEvent(XPCTSTR strName)
{
   switch(strName[0])
	{
	case 'o':
	case 'O':break;
	default:return -1;
	}
   XSortConstStr sort((XPCTSTR*)_strEvents,XOBJEVENT_UNLOAD+1);
   XBOOL bLowCase=m_pRoot->NeedLowCase();
   int id=sort.Index((void*)strName,bLowCase?XSS_NOCASE:XSS_NORMAL);
   if(id<0||id>(int)m_events.GetSize()) return id;
   if(m_events[id]!=XNULL)
   {
		m_events[id]=XNULL;	
   }
   return id;
}

void XXObject::PostDestroy(XXObject*pObj)
{
	if(!m_pTokens) return;
	for(int i=0;i<(int)m_pTokens->GetSize();i++)
	{
		XXTOKEN*p=(*m_pTokens)[i];
		if(!p->member.IsObject()) continue;
		if(p->member.pObject==pObj)
			p->member.ToConst(XOCT_UNDEFINE);
		else
		   p->member.pObject->PostDestroy(pObj);
	}
}

int XXObject::GetSubCount(XXObject *pObj)
{
	if(!m_pTokens) return 0;
	int cnt=0;
	int nSize=m_pTokens->GetSize();
	for(int i=0;i<nSize;i++)
	{
		XXTOKEN*p=(*m_pTokens)[i];
		if(p->pGetter==pObj) cnt++;
		if(p->pSetter==pObj) cnt++;
		if(p->member.pObject==pObj) cnt++;
	}
	return cnt;
}



void XXObject::CopyMembers(XXObject*pObj)
{
   XXObject*pSuper=this;
   //while(pSuper)
   if(!m_bBase&&m_pTokens)
   {
	   //if(pSuper->m_bBase) break;//!m_bBase&&m_pTokens)
	   //if(pSuper->m_pTokens)//
	   {
		   int nSize=pSuper->m_pTokens->GetSize();
		   for(int i=0;i<nSize;i++)
		   {
			   XXTOKEN*p=(*pSuper->m_pTokens)[i];
			   if(p->pGetter||p->pSetter) continue;
			   XXObject*pOwer=p->pOwer?p->pOwer:pSuper;
			   pObj->AddMember(p->strKey,p->member,XTRUE,pOwer);
			   //pObj->m_events.Append(m_events);
		   }
	   }
	   //pSuper=pSuper->m_pSuper;
   }
}


//DEL XBOOL XXObject::GetGlobal(XSWFCONTEXT &cnt, XPCTSTR strKey, XXVar &var)
//DEL {
//DEL 	switch(strKey[0])
//DEL 	{
//DEL 	case '_':
//DEL 
//DEL 		break;
//DEL 	}
//DEL 	return XFALSE;
//DEL }

///////////////////////////////////////////////////////////////////////
XFastAlloc XXSortToken::alloc(sizeof(XXTOKEN),1000);//,XTRUE,0xC1111111);

