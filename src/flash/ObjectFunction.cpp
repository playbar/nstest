// ObjectFunction.cpp: implementation of the ObjectFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectFunction.h"
#include "ObjectCreator.h"
#include "splayer.h"
#include "XActionScript.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XBOOL XXObjectFunction::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(call):
	case _SYSID(apply):
		var=this;
		var.ToFunction(id);
		return XTRUE;
	}
	return XXObject::SysGetMember(id,pCnt,var);
}
XBOOL XXObjectFunction::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
	case _SYSID(call):
		Function(pCnt,list,this);
		return XTRUE;
	case _SYSID(apply):
		{
			int nSize=list.GetSize();
			if(nSize<=0) return XTRUE;
			if(list[0].IsObject())
			//	cnt.thises.Add(ls[0]->pObject);
				pCnt->pThis=list[0].pObject;
			if(nSize>=2)
			{
				if(list[1].IsObject(XXOBJ_ARRAY))//&&
				   //ls[1]->pObject->IsArray())
					Function(pCnt,((XXObjectArray*)list[1].pObject)->m_list,this);
				else
				{
					XU8 vars[sizeof(XXVar)*2];
					XXVARLIST list(vars,2);
					Function(pCnt,list,this);
				}
			}
			else
			{					
				XU8 vars[sizeof(XXVar)*1];
				XXVARLIST list(vars,1);
				Function(pCnt,list,this);
			}
		}
		return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}

void XXObjectFunction::InitObject()
{
	_SYSCALL(call);
	_SYSCALL(apply);
	//AddMember(_strConstructor,XXVar(this));
	//AddMember(XXVar(_strCall),XXVar(XXFUNOBJ(call)));
	//AddMember(XXVar(_strApply),XXVar(XXFUNOBJ(apply)));
}

XXObjectFunction* XXObjectFunction::Clone()
{
	XXObjectFunction*pObj=m_pRoot->m_pGlobal->CreateFunction(m_pConsts,m_nConsts);
	pObj->m_pCodeBuf=m_pCodeBuf;
	pObj->m_nCodeLength=m_nCodeLength;
	pObj->m_nFlags=m_nFlags|XCLONE_FLAG;
	pObj->m_nRegCount=m_nRegCount;
	pObj->m_pArgs=m_pArgs;
	//pObj->m_pBlocks=m_pBlocks;
//	pObj->m_args.Ref(m_args);
//	if(m_pActions)
//		pObj->m_pActions=m_pActions->Clone();
	pObj->m_nConsts=m_nConsts;
	pObj->m_pConsts=m_pConsts;
//	pObj->m_pLocal=m_pLocal;
	pObj->m_pBase=REFOBJECT(m_pBase);//->RefObject();
	pObj->m_nResetCount=m_nResetCount;
	pObj->m_strResetPath=m_strResetPath;
//	if(m_pBase&&m_pBase->IsObject(XXOBJ_CLIP))
//		((XXObjectClip*)m_pBase)->AddFunction(pObj);
	return pObj;
}



//static XXObjectFunction*pCreate=XNULL;

XXObject* XXObjectFunction::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{

   XXObjectFunction*pObj=Clone();
   if(pObj)
   {
	   pObj->SetSuper(this,XFALSE);//(m_bBase&1)?this:m_pSuper,XFALSE);
	   pObj->m_pSuperObj=GetSuperObject();
	   CopyMembers(pObj);
	   pObj->m_nDataType=XODT_OBJECT;
   }

   XXVar var(pObj);
   pCnt->pThis=pObj;
   pCnt->pObject=pObj;
   //if(m_pLocal)//m_pObject&&m_pObject->pObject)
   //{   pCnt->thread=m_pLocal;//m_pObject->pObject;
   //XStack stacks;
   //XStack*pStack=pCnt->pStack;
   //pCnt->pStack=&stacks;
   //int s=pCnt->pStack->GetSize();
   //XXStack stack(32);
   //XXStack*pStack=pCnt->pStack;
   //pCnt->pStack=&stack;
   int s1=pCnt->pStack->GetSize();
   pObj->Function(pCnt,list,pObj);
   int s2=pCnt->pStack->GetSize();
   while(s2>s1)
   {
	   pCnt->pStack->FreePop();
	   s2--;
   }
   //pCnt->pStack=pStack;
   //int s1=pCnt->pStack->GetSize();
   //while(
   //pCnt->pStack=pStacks;

   pCnt->pStack->Push(var);

   return pObj;
}

void XXObjectFunction::InitVariable(XSWFCONTEXT &cnt,XXObject*,XXObject*pLocalObj,
									XXObjectArgs*pArgs,XXVARLIST&args)
{
   int iReg=1;
   //m_args.m_pContext=this;
   pLocalObj->AddMember(_SYSVAR(arguments),XXVar(pArgs));
   //pLocalObj->AddMember(_SYSVAR(this),XXVar(cnt.pThis));
   //pLocalObj->AddMember(_SYSVAR(_parent),XXVar(cnt.baseThread->GetParent()));
   //pArgs->
   if(m_nFlags&XPRELOAD_THIS)//=0x8000,
   {
	  cnt.SaveRegister(iReg,XXVar(cnt.pThis));
	  iReg++;
	  if(iReg>=m_nRegCount) return;
   }
   if(m_nFlags&XPRELOAD_ARGS)
   {
	   cnt.SaveRegister(iReg,XXVar(pArgs));
	   iReg++;
	   if(iReg>=m_nRegCount) return;
   }
   if(m_nFlags&XPRELOAD_SUPER)
   {
	   //XXObject*pObj=m_pSuper?m_pSuper:(XXObjectCreator::CreateConst(XOCT_NULL));
	   XXObject*pObj=cnt.pObject?cnt.pObject:cnt.pThis;
	   cnt.SaveRegister(iReg,XXVar(pObj->GetSuperObject()));
	   iReg++;
	   if(iReg>=m_nRegCount) return;
   }
   if(m_nFlags&XPRELOAD_ROOT)
   {
	   cnt.SaveRegister(iReg,XXVar(cnt.pThis->m_pRoot->rootObject->pObject));
	   iReg++;
	   if(iReg>=m_nRegCount) return;
   }
   if(m_nFlags&XPRELOAD_PARENT)
   {
	   //XXObject*pObj=m_pParent?m_pParent:
		//			XXObjectCreator::CreateConst(XOCT_NULL);
	   cnt.SaveRegister(iReg,XXVar(cnt.baseThread->GetParent()));
	   iReg++;
	   if(iReg>=m_nRegCount) return;
   }
   if(m_nFlags&XPRELOAD_GLOBAL)
   {
	   cnt.SaveRegister(iReg,XXVar(m_pRoot->m_pGlobal));
	   iReg++;
	   if(iReg>=m_nRegCount) return;
   }
   XBOOL bLowCase=NeedLowCase();

   if(m_pArgs)
   {
	   XBOOL bFun2=m_nFlags&XFUNCTION2;
	   SParser parser;
	   parser.Attach(m_pArgs,0);

		//m_pArgs=stream.script+stream.pos;
		// numArgs=stream.GetWord();
		// m_nRegCount=stream.GetByte();
		//m_nFlags=stream.GetWord()|XFUNCTION2;
		//m_nRegCount++;
	   //if(m_nRegCount<4) m_nRegCount=4;
	   //i


	   XU16 nums=parser.GetWord();
	   if(bFun2)
		   parser.SkipBytes(3);
	   XXVar var(XODT_CONST,XOCT_UNDEFINE);
	   for(XU32 i=0;i<nums;i++)//m_args.GetSize();i++)
	   {
		   int nRegCode=bFun2?parser.GetByte():0;
		   char*strParam=parser.GetStringP();
		   XXVar*pVar=i<args.GetSize()?&args[i]:&var;
		   if(nRegCode==0||
			  nRegCode>=m_nRegCount)
		   {
			   pLocalObj->AddMember(XXVar(strParam),*pVar,XTRUE);
		   }
		   else
		   {
			  //if(nRegCode==2&&pVar->IsUndefine())
			//	  int v=0;
			  cnt.SaveRegister(nRegCode,*pVar);
		   }
	   }
	   //for(;i<args.GetSize();i++)
   }
   pArgs->m_list.Append(args);
   //args.Release();
   //args.RemoveAll();
}

XU32 XXObjectFunction::Function(XSWFCONTEXT *pCnt, XXVARLIST &list,XXObject*pCaller)
{
   
   if(!m_nCodeLength||!m_pCodeBuf)//m_pActions==XNULL) 
   {
	 //  CreateObject(pCnt,list);
	   //pCnt->pStack->Push(XXVar::CreateConst(XOCT_UNDEFINE));
	   return XTRUE;
   }

   if(!m_pBase) 
	   return XFALSE;
   if(!m_pBase->m_pObject)
   {
	   if(!m_pBase->IsObject(XXOBJ_CLIP))
	   {
		   ReleaseFunction();
		   return XFALSE;
	   }
	   SObject*t=pCnt->pPlayer->FindTarget(m_pRoot->rootObject,((XXObjectClip*)m_pBase)->m_strPath.strTxt,XNULL);
	   if(!t)
	   {
		   //ReleaseFunction();
		   //return XFALSE;
		   t=m_pRoot->rootObject;
		   m_strResetPath=((XXObjectClip*)m_pBase)->m_strPath;
		   m_nResetCount=1;
		   
	   }
	   FREEOBJECT(m_pBase);
	   m_pBase=REFOBJECT(t->pObject);
	   //return XFALSE;
   }
   else if(m_nResetCount)
   {
	   if(m_nResetCount>100)
		   m_nResetCount=0;
	   else
	   {
		  SObject*t=pCnt->pPlayer->FindTarget(m_pRoot->rootObject,m_strResetPath.strTxt,XNULL);
		  if(t)
		  {
			  FREEOBJECT(m_pBase);
			  m_nResetCount=0;
			  m_pBase=REFOBJECT(t->pObject);
		  }
	   }
   }

   XBOOL bCon=XFALSE;
   //if(m_pParent&&pCnt->containts.GetLast()!=m_pParent)
   //{
	//   pCnt->containts.Add(m_pParent);
	 //  bCon=XTRUE;
   //}

   //pCnt->blocks.Add(this);
     //pCnt->calls.Add(this);
   
//   XXObject*pContaint=pCnt->pCaller;
 //  if(m_pContaint&&
//	  m_pContaint!=pCnt->GetTarget()->GetObject())
//	   pCnt->pCaller=m_pContaint;
   //XDTSpriteDisp*pContaint=pCnt->pContext;
   //pCnt->pContext=m_pContaint;
   //if(m_pContaint)
   //	  pCnt->containts.Add(m_pContaint);
   //XPCTSTR pConst=XNULL;
   //XU16 nCount=0;
   //XVectorFast<
   XCONSTVECTOR backConst;
   XBOOL bBackConst=XFALSE;
   if(m_nConsts)
   {
	    if(pCnt->constants.GetSize()>0)
		{
			if(pCnt->constants[0]!=m_pConsts)
			{
				//nCount=pCnt->constants.GetSize();
				//pConst=pCnt->constants[0];
				bBackConst=XTRUE;
				backConst.AttachFrom(pCnt->constants);
				pCnt->MakeConstant(m_pConsts,m_nConsts);
			}
		}
		else
			pCnt->MakeConstant(m_pConsts,m_nConsts);
   }
   
   /*int nMax=XMAX(list.GetSize(),m_args.GetSize());
   for(int i=m_args.GetSize();i<nMax;i++)
	   m_args.NewArg();
   for(i=0;i<nMax;i++)
   {
	   if(i>=(int)list.GetSize()) //break;
		   m_args[i]->var.ToConst(XOCT_UNDEFINE);
	   else
		   m_args[i]->var=*list[i];
	   
   } */

   //XArray<XXVar> blist;
   //XXVar*blist=pCnt->
   //int nSize=pCnt->regObjs.GetSize();
   //blist.DataFrom(pCnt->regObjs.DataTo(),nSize);
   int nSize=pCnt->regObjs.GetSize();
   int nTotalSize=nSize*sizeof(XXVar);
   XXVar*pRegData=(XXVar*)XXVar::AllocData(nTotalSize);
   if( pRegData == NULL )
   {
	   LOGWHERE();
   }
   memcpy(pRegData,pCnt->regObjs.m_pData,nTotalSize);
   memset(pCnt->regObjs.m_pData,0,nTotalSize);

   pCnt->regObjs.SetSize(m_nRegCount);//,XFALSE);

   XXObject*pLocal=m_pRoot->m_pGlobal->CreateObject();
   XXObjectArgs*pArgs=new XXObjectArgs(m_pRoot,this);//m_pRoot->m_pGlobal->GetBaseArgs()
   pArgs->SetSuper(m_pRoot->m_pGlobal->GetArgsBase(),XFALSE);
   pArgs->RefObject();
   pLocal->RefObject();
   
   
   pLocal->m_pNext=m_pBlocks;
  // m_pActions->InitContext(m_pBase,m_pBase/*pCnt->baseThread*/,pCnt->pThis,pCnt->pWith,pCnt->pCaller);
   XActionScript action(m_pCodeBuf,m_nCodeLength);


   //////////////////////////////////////////////////////////////////////////////////////
   XXObject*pBase=m_pBase;
   //if(pCnt->pThis==pCnt->thread)
	  // pBase=pCnt->thread;
   /*if(pCnt->pThis&&pCnt->pThis->m_pObject)
	   pBase=pCnt->pThis;
   else if(pCnt->thread&&pCnt->thread->m_pObject)
	   pBase=pCnt->thread;*/
   if(m_pRoot->version<=5)//||(m_nFlags&XINITCLASS))
   {
	   if(pCnt->pThis&&pCnt->pThis->m_pObject)
			pBase=pCnt->pThis;
	   else if(pCnt->thread&&pCnt->thread->m_pObject)
			pBase=pCnt->thread;
	   //pBase=pCnt->thread;
   }
   pCnt->baseThread=pBase;
   InitVariable(*pCnt,pCaller,pLocal,pArgs,list);
   action.InitContext(m_pBase,pBase,pCnt->pThis,pCnt->pObject,pCnt->pWith,pCnt->pCaller);
   //if(pBase!=pCnt->thread)
//	   int v=0;
   ///////////////////////////////////////////////////////////////////////////////////////
   //action.InitContext(m_pBase,m_pBase,pCnt->pThis,pCnt->pWith,pCnt->pCaller);	

   action.DoAction(*pCnt,pLocal,XTRUE);

   //local.ClearMembers();

   pCnt->regObjs.Release();
   pCnt->regObjs.SetSize(nSize);
   memcpy(pCnt->regObjs.m_pData,pRegData,nTotalSize);
   XXVar::FreeData(pRegData,nTotalSize);
   //pCnt->regObjs.DataFrom(blist.DataTo(),nSize);//Append(blist);

   pLocal->FreeObject();

 //  if(pLocal->m_nRefs)
//	   int v=0;

   pArgs->FreeObject();

   if(bBackConst)
   {
	   pCnt->constants.AttachFrom(backConst);
	   //pCnt->MakeConstant(pConst,nCount);
   }  
   //if(bCon)
	//   pCnt->containts.RemoveLast();
   //pCnt->blocks.RemoveLast();

   /*if(m_pTokens)
   {
	   int nSize=m_pTokens->GetSize()-1;
	   for(int i=nSize;i>=0;i--)
	   {
		   XXTOKEN*pToken=(*m_pTokens)[i];
		   if(pToken->bGlobal) continue;
		   FREEOBJECT(pToken->pGetter);
		   FREEOBJECT(pToken->pSetter);
		   delete pToken;
		   m_pTokens->RemoveAt(i);
	   }
   }*/
//   pCnt->pCaller=pContaint;
   //if(m_pContaint)
	//   pCnt->contexts.RemoveLast();
//   pCnt->calls.RemoveLast();
   //if(bCnt)
   //ClearLocalVar();
   //pCnt->containts.RemoveLast();
   return XTRUE;
   //pCnt->containts.RemoveLast();
}

XXObjectFunction::XXObjectFunction(ScriptPlayer*pRoot):XXObject(pRoot)
				  //m_args(pRoot)//,m_localObj(pRoot)
{
	//m_nObjType=XOOT_TYPE;
	m_pArgs=XNULL;
	m_pNext=XNULL;
	m_nDataType=XODT_FUNCTION;
//	m_pActions=XNULL;
//	m_pLocal=XNULL;
	m_nResetCount=0;
	m_pBlocks=XNULL;
//	m_localObj.RefObject();
	m_pBase=XNULL;
	m_nFlags=0;
//	m_args.m_pCallee=this;
//	m_args.SetContext(this);
//	m_args.SetSuper(pRoot->m_pGlobal->GetArgsBase(),XFALSE);
//	m_args.RefObject();
	m_nConsts=0;
	m_pCodeBuf=XNULL;
	m_nCodeLength=0;
	m_pConsts=XNULL;
#ifdef _DEBUG
	nObjCount[XXOBJ_FUNCTION]++;
#endif
	m_bNoCase=XTRUE;
//	m_pContaint=XNULL;
//	nObjCount++;
	//m_args.SetSuper(XXObjectCreator::GetObjectBase());
}



XU32 XXObjectFunction::Init(XXObject*pContaint,SParser&stream,XXObject*pLocal)//XStream *pStream)
{
  // m_pContaint=pContaint;
   //if(pContaint)
   //SetContext(pLocal);
   //m_pContext=pLocal->Ref
   if(pLocal&&pLocal->IsObject(XXOBJ_OBJECT))
   {
		m_pBlocks=pLocal;//pLocal->RefObject();//REFOBJECT(pLocal);
		XXObject*p=m_pBlocks;
		while(p)
		{
			//XXObject*pNext=p->m_pNext;
			FLASHASSERT(p->IsObject(XXOBJ_OBJECT));
			p->RefObject();
			p=p->m_pNext;
		}
   }
   m_pBase=pContaint->RefObject();//pContaint->RefObject();
   //if(m_pBase->IsObject(XXOBJ_CLIP))
	//((XXObjectClip*)m_pBase)->AddFunction(this);
   //SetLocal(pLocal);
//   m_args.Release(m_nFlags&XCLONE_FLAG);
//   if(m_pActions==XNULL)
//	   m_pActions=new XActionScript(XNULL);
   
   //if(m_pActions==XNULL) return 0;
   XU16 numArgs;//,nCodeSize=0;
   m_nFlags=XCREATE_SUPER|XCREATE_ARGS|XCREATE_THIS;
   m_nRegCount=4;
   m_pArgs=stream.script+stream.pos;
   numArgs=stream.GetWord();//
   //if(!pStream->ReadWord(numArgs)) return 0;
   for(XU32 i=0;i<numArgs;i++)
   {
	   char*str=stream.GetStringP();
	   //    pData->strParam=str;//.ReadFromStream(pStream);
	   //}
   }
   m_nCodeLength=stream.GetWord();
   //if(!pStream->ReadWord(nCodeSize)) return 0;
   m_pCodeBuf=stream.script+stream.pos;
	   //Init(pStream->ReadAddr(),nCodeSize);
   return m_nCodeLength;
   //return XTRUE;
}

XU32 XXObjectFunction::Init2(XXObject*pContaint,SParser&stream,XXObject*pLocal)//XStream *pStream)
{
   //SetContext(pContaint);
   if(pLocal&&pLocal->IsObject(XXOBJ_OBJECT))
	//if(pLocal&&pLocal->IsObject(XXOBJ_OBJECT))
   {
		m_pBlocks=pLocal;//pLocal->RefObject();//REFOBJECT(pLocal);
		XXObject*p=m_pBlocks;
		while(p)
		{
			//XXObject*pNext=p->m_pNext;
			FLASHASSERT(p->IsObject(XXOBJ_OBJECT));
			p->RefObject();
			p=p->m_pNext;
		}
   }
	//	m_pBlocks=pLocal->RefObject();//REFOBJECT(pLocal);
   m_pBase=pContaint->RefObject();//pContaint->RefObject();
   //if(m_pBase->IsObject(XXOBJ_CLIP))
//	((XXObjectClip*)m_pBase)->AddFunction(this);
//   SetLocal(pLocal);
   //m_pContaint=pContaint;
   //if(m_pContaint)
	 //  m_pContaint->RefObject();
   //SetParent(pContaint);
//   m_args.Release(m_nFlags&XCLONE_FLAG);
//   if(m_pActions==XNULL)
//	   m_pActions=new XActionScript(XNULL);
  // if(m_pActions==XNULL) return 0;
   XU16 numArgs;//,nCodeSize=0;
   //m_nFlags=0;
   //if(!pStream->ReadWord(numArgs)) return 0;
   //if(!pStream->ReadByte(m_nRegCount)) return 0;
   //if(!pStream->ReadWord(m_nFlags)) return 0;
   m_pArgs=stream.script+stream.pos;
   numArgs=stream.GetWord();
   m_nRegCount=stream.GetByte();
   m_nFlags=stream.GetWord()|XFUNCTION2;
   m_nRegCount++;
   if(m_nRegCount<4) m_nRegCount=4;
   for(XU32 i=0;i<numArgs;i++)
   {
	   //XXObjectArgs::ARGDATA*pData=m_args.NewArg();
	   //if(pData)
	   {
		//if(!pStream->ReadByte(pData->nRegCode)) return 0;
		//pData->nRegCode=
		stream.GetByte();
		//XSWFString str;
		//XString8 str;
		//pStream->ReadString(str,0);
		char*str=stream.GetStringP();
		//str.ReadFromStream(pStream);
		//pData->strParam=str;//.ReadFromStream(pStream);
	   }
   }
   //if(!pStream->ReadWord(nCodeSize)) return 0;
   m_nCodeLength=stream.GetWord();
   m_pCodeBuf=stream.script+stream.pos;//,nCodeSize);
   return m_nCodeLength;

}

///////////////////////////////////////////////////////////////////////
XXObjectFunction::~XXObjectFunction()
{
	//FREEOBJECT(m_pContaint);
#ifdef _DEBUG
	nObjCount[XXOBJ_FUNCTION]--;
#endif
//	if(m_strClassName=="BackgroundScroller")
//		int v=0;
	ReleaseFunction();
}

/*XU32 XXObjectFunction::AttachCode(XU8* pCode,XU32 nLen)//;//XStream *pStream)
{
	m_nFlags&=~XCLONE_FLAG;
	if(m_pActions==XNULL)
		m_pActions=new XActionScript();
	return m_pActions->Init(pStream->ReadAddr(),pStream->GetDataSize());//pStream);
}*/

void XXObjectFunction::ReleaseFunction()
{
//	m_args.Release(m_nFlags&XCLONE_FLAG);
//	FREEOBJECT(m_pLocal);
//	if(m_localObj.m_pTokens)
//		int v=0;
//	m_localObj.ClearMembers();
//	if(m_strClassName=="Tween")
//		int v=0;
//	if(m_pBase&&m_pBase->IsObject(XXOBJ_CLIP))
//		((XXObjectClip*)m_pBase)->RemoveFunction(this);
	FREEOBJECT(m_pBase);
	m_pBase=XNULL;
	m_pArgs=XNULL;
	XXObject*p=m_pBlocks;
	while(p)
	{
		XXObject*pNext=p->m_pNext;
		p->FreeObject();
		p=pNext;
	}
	m_pBlocks=XNULL;
	//FREEOBJECT(m_pBlocks);
	//m_pBlock=XNULL;
	//FREEOBJECT(m_pBase);
	//FREEOBJECT(m_pBlock);
}

int XXObjectFunction::GetSubCount(XXObject *pObj)
{
	int cnt=XXObject::GetSubCount(pObj);
	XXObject*p=m_pBlocks;
	while(p)
	{
		if(p==pObj) cnt++;
		cnt+=p->GetSubCount(pObj);
		p=p->m_pNext;
	}
	return cnt;
}
