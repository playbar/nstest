// XXObjectLoadVars.cpp: implementation of the XXObjectLoadVars class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectLoadVars.h"
#include "splayer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


XXObjectLoadVars::XXObjectLoadVars(ScriptPlayer*pRoot):XXObject(pRoot)//,m_headers(pRoot)
{
	m_pHeaders=new XXObject(pRoot);
	//if(m_pHeaders)
	REFOBJECT(m_pHeaders);
	//m_headers.RefObject();
	m_loadedBytes=0;
	m_totalBytes=0;
	m_bNoCase=XTRUE;
	m_bLoaded=XFALSE;
}

XXObjectLoadVars::~XXObjectLoadVars()
{
	FREEOBJECT(m_pHeaders);
}


XBOOL XXObjectLoadVars::SysGetMember(int id, XSWFCONTEXT *pCnt,XXVar &var)
{
	switch(id)
	{
	case _SYSID(contentType):
		 if(m_pHeaders)
		 {
			 m_pHeaders->GetMember(*pCnt,_SYSNAME(contentType),var,XFALSE);
		 }
		 return XTRUE;
	case _SYSID(loaded):
		 if(m_bLoaded==0)
			 var.ToConst(XOCT_UNDEFINE);
		 else
		 {
			 var.ToLogic();
			 var=m_bLoaded==2;
		 }
			 //pCnt->pStack->PushBool(m_loaded==2);
		 return XTRUE;
	case _SYSID(addRequestHeader):
	case _SYSID(decode):
	case _SYSID(getBytesLoaded):
	case _SYSID(getBytesTotal):
	case _SYSID(load):
	case _SYSID(send):
	case _SYSID(sendAndLoad):
	case _SYSID(toString):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectLoadVars::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	int nSize=list.GetSize();
	switch(id)
	{
	case _SYSID(addRequestHeader)://XXLOAD_addRequestHeader:
		 if(nSize>=2&&m_pHeaders)
		 {
			 list[0].ToString(XFALSE);
			 list[1].ToString(XFALSE);
			 m_pHeaders->AddMember(list[0],list[1]);
		 }
		 break;
	case _SYSID(decode)://XXLOAD_decode:
		 if(nSize)
		 {
			 list[0].ToString(XFALSE);
			 DecodeQueryString(*pCnt,this,list[0].strTxt);
		 }
		 break;
	case _SYSID(getBytesLoaded)://XXLOAD_getBytesLoaded:
		 pCnt->pStack->Push((m_loadedBytes));
		 break;
	case _SYSID(getBytesTotal)://XXLOAD_getBytesTotal:
		 pCnt->pStack->Push((m_totalBytes));
		 break;
	case _SYSID(load)://XXLOAD_load:
		 //break;
		 if(nSize)
		 {
			 list[0].ToString(XFALSE);
			 XXVar var;
			 pCnt->pPlayer->GetTargetPath(pCnt->thread->m_pObject,var);
			 if(var.IsString()&&!var.IsNull())
			 {
				pCnt->pPlayer->GetURL3(*pCnt,list[0].strTxt,var.strTxt,kHttpLoadVariables|kHttpSendUseGet,this,XNULL);
				//FreeStr(target);
			 }
		 }
		 pCnt->pStack->PushBool(XTRUE);//(XXVar::CreateBool(XTRUE));
		 m_bLoaded=1;
		 break;
	case _SYSID(toString):
		{
			URLBuilder ub;
			ub.EncodeVariables(*pCnt,this);
			XXVar var;
			if(ub.len)
				var.SetString(ub.buf,ub.len);
			//cnt.pStack->Push((ub.buf));
			else 
				var=_strEmpty;
			pCnt->pStack->Push(var);
		}return XTRUE;
		//cnt.pStack->Push((""));

	case _SYSID(sendAndLoad)://XXLOAD_sendAndLoad:
	case _SYSID(send)://XXLOAD_send:
		 if(nSize)
		 {
			 list[0].ToString(XFALSE);
			 XXVar target;
			 pCnt->pPlayer->GetTargetPath(pCnt->thread->m_pObject,target);
			 int nMethod=kHttpLoadVariables|kHttpSendUseGet;//|kHttpLoadTarget;
			 URLBuilder ub;
			 ub.EncodeVariables(*pCnt,m_pHeaders);
			 ub.EncodeVariables(*pCnt,this);
			 m_target.Release();
			 if(nSize>=1)
			 {
				 if(list[1].IsString2())
				 {
					list[1].ToString(XFALSE);
					//FreeStr(target);
					target=list[1];//.StringTo();
				 }
				 else if(list[1].IsObject())
				 {
					 m_target=list[1];
				 }


			 }
			 if(nSize>=3)
			 {
				 list[2].ToString(XFALSE);
				 switch(list[2].strTxt[0])
				 {
				 case 'p':
				 case 'P':
					  nMethod=kHttpLoadVariables|kHttpSendUsePost;//|kHttpLoadTarget;
				 }
			 }
			 if(target.IsString())
			 {
				pCnt->pPlayer->GetURL3(*pCnt,list[0].strTxt,target.strTxt,nMethod,this,(char*)(ub.len?ub.buf:XNULL));
//				FreeStr(target);
			 }
		 }pCnt->pStack->PushBool(XTRUE);//(XXVar::CreateBool(XTRUE));
		 break;
	}

	return XXObject::SysCallMethod(id,pCnt,list);
}

XBOOL XXObjectLoadVars::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{

	switch(id)
	{
	case _SYSID(contentType):
		 if(m_pHeaders)
			 m_pHeaders->SetMember(*pCnt,_SYSVAR(contentType),var,XNULL);
		 return XTRUE;
	case _SYSID(loaded):
		 if(var.IsUndefine()) m_bLoaded=0;
		 else if(var.ToLogic()) m_bLoaded=2;
		 else m_bLoaded=1;
	}
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectLoadVars::InitObject()
{
	//AddProperty(_strContentType
	//AddProperty(_strLoaded
	_SYSCALL(contentType);
	_SYSCALL(loaded);
	_SYSCALL(addRequestHeader);
	_SYSCALL(decode);
	_SYSCALL(getBytesLoaded);
	_SYSCALL(getBytesTotal);
	_SYSCALL(load);
	_SYSCALL(send);
	_SYSCALL(sendAndLoad);
	_SYSCALL(toString);
	/*AddMember(XXVar(_strAddRequestHeader),XXFUNVAR(addRequestHeader));
	AddMember(XXVar(_strDecode),XXFUNVAR(decode));
	AddMember(XXVar(_strGetBytesLoaded),XXFUNVAR(getBytesLoaded));
	AddMember(XXVar(_strGetBytesTotal),XXFUNVAR(getBytesTotal));
	AddMember(XXVar(_strLoadOnly),XXFUNVAR(load));
	AddMember(XXVar(_strSend),XXFUNVAR(send));
	AddMember(XXVar(_strSendAndLoad),XXFUNVAR(sendAndLoad));
	AddMember(XXVar(_strToString),XXFUNVAR(toString));*/
}


XXObject* XXObjectLoadVars::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
   XXObjectLoadVars*pObj=new XXObjectLoadVars(m_pRoot);
   if(pObj)
   {
	   pObj->m_pSuperObj=GetSuperObject();
		pObj->SetSuper(this,XFALSE);
   }
   pCnt->pStack->Push((pObj));
   return pObj;
}

//DEL void XXObjectLoadVars::DoCallBack(XSWFCONTEXT &cnt, XXVARLIST &list, int funID)
//DEL {
//DEL 	int nSize=list.GetSize();
//DEL 	switch(funID)
//DEL 	{
//DEL 	case XXLOAD_addRequestHeader:
//DEL 		 if(nSize>=2&&m_pHeaders)
//DEL 		 {
//DEL 			 list[0].ToString(XFALSE);
//DEL 			 list[1].ToString(XFALSE);
//DEL 			 m_pHeaders->AddMember(list[0],list[1]);
//DEL 		 }
//DEL 		 break;
//DEL 	case XXLOAD_decode:
//DEL 		 if(nSize)
//DEL 		 {
//DEL 			 list[0].ToString(XFALSE);
//DEL 			 DecodeQueryString(cnt,this,list[0].strTxt);
//DEL 		 }
//DEL 		 break;
//DEL 	case XXLOAD_getBytesLoaded:
//DEL 		 cnt.pStack->Push((m_loadedBytes));
//DEL 		 break;
//DEL 	case XXLOAD_getBytesTotal:
//DEL 		 cnt.pStack->Push((m_totalBytes));
//DEL 		 break;
//DEL 	case XXLOAD_load:
//DEL 		 //break;
//DEL 		 if(nSize)
//DEL 		 {
//DEL 			 list[0].ToString(XFALSE);
//DEL 			 XXVar var;
//DEL 			 cnt.pPlayer->GetTargetPath(cnt.thread->m_pObject,var);
//DEL 			 if(var.IsString()&&!var.IsNull())
//DEL 			 {
//DEL 				cnt.pPlayer->GetURL3(cnt,list[0].strTxt,var.strTxt,kHttpLoadVariables|kHttpSendUseGet,this,XNULL);
//DEL 				//FreeStr(target);
//DEL 			 }
//DEL 		 }
//DEL 		 cnt.pStack->PushBool(XTRUE);//(XXVar::CreateBool(XTRUE));
//DEL 		 break;
//DEL 	case XXLOAD_sendAndLoad:
//DEL 	case XXLOAD_send:
//DEL 		 if(nSize)
//DEL 		 {
//DEL 			 list[0].ToString(XFALSE);
//DEL 			 XXVar target;
//DEL 			 cnt.pPlayer->GetTargetPath(cnt.thread->m_pObject,target);
//DEL 			 int nMethod=kHttpLoadVariables|kHttpSendUseGet;//|kHttpLoadTarget;
//DEL 			 URLBuilder ub;
//DEL 			 ub.EncodeVariables(cnt,m_pHeaders);
//DEL 			 ub.EncodeVariables(cnt,this);
//DEL 			 if(nSize>=1)
//DEL 			 {
//DEL 				 if(list[1].IsString2())
//DEL 				 {
//DEL 					list[1].ToString(XFALSE);
//DEL 					//FreeStr(target);
//DEL 					target=list[1];//.StringTo();
//DEL 				 }
//DEL 				 //else if(list[1].IsObject())
//DEL 
//DEL 
//DEL 			 }
//DEL 			 if(nSize>=3)
//DEL 			 {
//DEL 				 list[2].ToString(XFALSE);
//DEL 				 switch(list[2].strTxt[0])
//DEL 				 {
//DEL 				 case 'p':
//DEL 				 case 'P':
//DEL 					  nMethod=kHttpLoadVariables|kHttpSendUsePost;//|kHttpLoadTarget;
//DEL 				 }
//DEL 			 }
//DEL 			 if(target.IsString())
//DEL 			 {
//DEL 				cnt.pPlayer->GetURL3(cnt,list[0].strTxt,target.strTxt,nMethod,this,(char*)(ub.len?ub.buf:XNULL));
//DEL //				FreeStr(target);
//DEL 			 }
//DEL 		 }cnt.pStack->PushBool(XTRUE);//(XXVar::CreateBool(XTRUE));
//DEL 		 break;
//DEL 	case XXLOAD_toString:
//DEL 		{
//DEL 			URLBuilder ub;
//DEL 			ub.EncodeVariables(cnt,this);
//DEL 			if(ub.len)
//DEL 				cnt.pStack->Push((ub.buf));
//DEL 			else 
//DEL 				cnt.pStack->Push((""));
//DEL 		}
//DEL 		 break;
//DEL 	}
//DEL }
