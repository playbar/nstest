// ObjectCreator.cpp: implementation of the ObjectCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectCreator.h"
#include "splayer.h"
#include "XXObjectError.h"
#include "XXObjectDate.h"
#include "XXObjectSound.h"
#include "XXObjectLoadVars.h"
#include "XXObjectAsBroadcaster.h"
#include "XXObjectTransform.h"
#include "XXObjectTextSnapshot.h"
#include "XXObjectStyleSheet.h"
#include "XXObjectTextFormat.h"
#include "XXObjectSystem.h"
#include "XXObjectSelection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//XXObjectCreator XXObjectCreator::_global;

//#define XXFUN(TOKEN) XXCLASS(TOKEN,XXObjectCreator,XXOBJ_CREATOR)
XBOOL XXObjectCreator::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
//	return XFALSE;
	switch(id)
	{
		case _SYSID(display):
		case _SYSID(geom):
			 var=this;
			 break;	
		//Global Function Here
		case _SYSID(clearInterval):
		case _SYSID(escape):
		case _SYSID(getVersion):
		case _SYSID(isFinite):
		case _SYSID(parseFloat):
		case _SYSID(parseInt):
		case _SYSID(setInterval):
		case _SYSID(unescape):
		case _SYSID(updateAfterEvent):
		case _SYSID(isNaN):
		case _SYSID(asfunction):
		case _SYSID(setTimeout):
			 var=this;
			 var.ToFunction(id);
			 //var=CreateSysCall(id,this);
			 return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);
}
XBOOL XXObjectCreator::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
//	return XFALSE;
	int nSize=list.GetSize();
	switch(id)
	{		
		//Global Function Here
		case _SYSID(asfunction):
			if(nSize&&list[0].pObject)
			{
				XXVARLIST args(nSize);
				for(int i=1;i<(int)list.GetSize();i++)
					args[i-1]=list[i];
				if(list[0].IsSysCall())
					list[0].pObject->SysCallMethod(list[0].nStringType,pCnt,args);
				else if(list[0].IsObject())
					list[0].pObject->Function(pCnt,args,pCnt->baseThread);

			}break;
		case _SYSID(isNaN):
			if(nSize)
			{
			   //if(list[0].IsUndefine())
				//   int v=0;
			   if(list[0].IsNaN())
					pCnt->pStack->PushBool(XTRUE);
			   else
				   pCnt->pStack->PushBool(XFALSE);
			}break;
		case _SYSID(clearInterval):
			 //clearInterval(*pCnt,list);return XTRUE;
			 if(nSize)
			 {
				XU32 id=list[0].ToInt();
				pCnt->pStack->PushBool(pCnt->pPlayer->RemoveCaller(id));
			 }return XTRUE;
		case _SYSID(escape):
			 escape(*pCnt,list);return XTRUE;
		case _SYSID(getVersion):
			 pCnt->pStack->Push((int)pCnt->pPlayer->GetVersion());
			 return XTRUE;
		case _SYSID(isFinite):
			 if(nSize)
				pCnt->pStack->PushBool(list[0].IsFinite());
			 else
				pCnt->pStack->PushConst(XOCT_UNDEFINE);
			 return XTRUE;
		case _SYSID(parseFloat):
			 if(list.GetSize()<=0) 
				pCnt->pStack->PushConst(XOCT_UNDEFINE);
			 else
				pCnt->pStack->Push(list[0].ToFloat());
			 return XTRUE;
		case _SYSID(parseInt):
			 if(list.GetSize()>0)
				 pCnt->pStack->Push(list[0].ToInt());
			 else
				 pCnt->pStack->PushConst(XOCT_UNDEFINE);
			 return XTRUE;
		case _SYSID(setInterval):
			 setInterval(*pCnt,list);
			 return XTRUE;
		case _SYSID(setTimeout):
			 setTimeout(*pCnt,list);
			 break;
		case _SYSID(unescape):
			 unescape(*pCnt,list);
			 return XTRUE;
		case _SYSID(updateAfterEvent):
			 pCnt->pStack->PushConst(XOCT_UNDEFINE);
			 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}
//XXFUN(loadMovie);




void XXObjectCreator::InitObject()
{
	_SYSCALL(clearInterval);
	_SYSCALL(escape);
	_SYSCALL(getVersion);
	_SYSCALL(isFinite);
	_SYSCALL(parseFloat);
	_SYSCALL(parseInt);
	_SYSCALL(setInterval);
	_SYSCALL(unescape);
	_SYSCALL(updateAfterEvent);
	_SYSCALL(geom);
	_SYSCALL(display);
	_SYSCALL(isNaN);
	_SYSCALL(asfunction);
	_SYSCALL(setTimeout);
	//AddMember(XXVar(_strLoadVariables,XXVar(XXFUNOBJ(loadVariables)));
	//AddMember(XXVar(_strLoadMovie,XXVar(XXFUNOBJ(loadMovie)));
}

void XXObjectCreator::updateAfterEvent(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//m_pRoot->rootObject->Modify();
}




void XXObjectCreator::clearInterval(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0) return;
	XU32 id=list[0].ToInt();
	cnt.pPlayer->RemoveCaller(id);
}
void XXObjectCreator::escape(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0) return;
	list[0].ToString(XFALSE);

	XPCTSTR strData=list[0].strTxt;
	int l=list[0].GetLength();

	XXVar strDst;
	strDst.SetString("",0);
	for(int i=0;i<l;i++)
	{
		XU8 c=strData[i];
		if(c>='0'&&c<='9')
			strDst.AddChar(c);
			//strDst+=str[i];
		else if(c>='a'&&c<'z')
			strDst.AddChar(c);
			//strDst+=str[i];
		else if(c>='A'&&c<'Z')
			strDst.AddChar(c);
			//strDst+=str[i];
		else
		{	
			strDst.AddChar('%');
			char ch=c>>4;
			if(ch<10) ch+='0';
			else ch=(ch-10)+'A';
			strDst.AddChar(ch);
			ch=c&0xf;
			if(ch<10) ch+='0';
			else ch=(ch-10)+'A';
			strDst.AddChar(ch);
			
		}
		
	}
	cnt.pStack->Push(strDst);

}
void XXObjectCreator::getVersion(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	cnt.pStack->Push((int)cnt.pPlayer->GetVersion());
}
void XXObjectCreator::isFinite(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0) return;
	cnt.pStack->PushBool(list[0].IsFinite());//XXVar::CreateBool(list[0].IsFinite()));
}
void XXObjectCreator::isNaN(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0) return;
	XU8 bNum=list[0].IsNaN();
	
	//if(list[0].IsString
	cnt.pStack->PushBool(bNum);//XXVar::CreateBool(bNum));
}
void XXObjectCreator::phaseFloat(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0) return;
	cnt.pStack->Push(list[0].ToFloat());
}
void XXObjectCreator::phaseInt(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0) return;
	cnt.pStack->Push(list[0].ToInt());
}

void XXObjectCreator::setTimeout(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XXVar*pVar=XXVar::CreateConst(XOCT_NULL);
	int nSize=list.GetSize();
	if(nSize<2) return;
	if(!list[0].IsFunction())
		return;
	int nTimer=nSize>=2?list[1].ToInt():0;
	XXObjectFunction*pFun=(XXObjectFunction*)list[0].pObject;
	XU8 vars[sizeof(XXVar)*32];
	XXVARLIST args(vars,32);
	for(XU32 i=2;i<list.GetSize();i++)
		args.Push(list[i]);
	list.SetSize(2);
	cnt.pPlayer->AddCaller(pFun->GetBase(),list[0].pObject,XXVar(_strEmpty),nTimer,args,1);
	
	/*if(nSize>=2)
	{
		if(list[1].IsNum3())
		{
			if(list[0].IsFunction())//&&list[1].IsNum3())
			{
				XXObjectFunction*pFun=(XXObjectFunction*)list[0].pObject;
				XU8 vars[sizeof(XXVar)*32];
				XXVARLIST args(vars,32);
				for(XU32 i=2;i<list.GetSize();i++)
					args.Push(list[i]);
				list.SetSize(2);
				//list.RemoveAt(2,list.GetSize()-2);
				int id=cnt.pPlayer->AddCaller(pFun->GetBase(),
											  list[0].pObject,
											  XXVar(_strEmpty),
											  list[1].ToInt(),args);
				var=id;
			}				
		}
		else if(nSize>=3&&list[0].IsObject())
		{
			list[1].ToString(XFALSE);
			XU8 vars[sizeof(XXVar)*32];
			XXVARLIST args(vars,32);//list.GetSize()-3);
			if(list.GetSize()>3)
			{
				for(XU32 i=3;i<list.GetSize();i++)
					args.Push(list[i]);
				list.SetSize(3);
				//list.RemoveAt(3,list.GetSize()-3);
			}
			int id=cnt.pPlayer->AddCaller(cnt.baseThread,
										  list[0].pObject,
										  list[1],
										  list[2].ToInt(),
										  args);
			var=id;
		}
		//else if(list[0].IsObject())
	}
	cnt.pStack->Push(var);*/
}

void XXObjectCreator::setInterval(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XXVar*pVar=XXVar::CreateConst(XOCT_NULL);
	XXVar var;
	var.ToConst(XOCT_NULL);
	int nSize=list.GetSize();
	if(nSize>=2)
	{
		if(list[1].IsNum3())
		{
			if(list[0].IsFunction())//&&list[1].IsNum3())
			{
				XXObjectFunction*pFun=(XXObjectFunction*)list[0].pObject;
				XU8 vars[sizeof(XXVar)*32];
				XXVARLIST args(vars,32);
				for(XU32 i=2;i<list.GetSize();i++)
					args.Push(list[i]);
				list.SetSize(2);
				//list.RemoveAt(2,list.GetSize()-2);
				int id=cnt.pPlayer->AddCaller(pFun->GetBase(),
											  list[0].pObject,
											  XXVar(_strEmpty),
											  list[1].ToInt(),args);
				var=id;
			}				
		}
		else if(nSize>=3&&list[0].IsObject())
		{
			list[1].ToString(XFALSE);
			XU8 vars[sizeof(XXVar)*32];
			XXVARLIST args(vars,32);//list.GetSize()-3);
			if(list.GetSize()>3)
			{
				for(XU32 i=3;i<list.GetSize();i++)
					args.Push(list[i]);
				list.SetSize(3);
				//list.RemoveAt(3,list.GetSize()-3);
			}
			int id=cnt.pPlayer->AddCaller(cnt.baseThread,
										  list[0].pObject,
										  list[1],
										  list[2].ToInt(),
										  args);
			var=id;
		}
		//else if(list[0].IsObject())
	}
	cnt.pStack->Push(var);
}
void XXObjectCreator::unescape(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0) return;
	list[0].ToString(XFALSE);

	XPCTSTR str=list[0].strTxt;
	int i,l=list[0].GetLength();
	XXVar strDst;
	for(i=0;i<l;i++)
	{
		XU8 c=str[i];
		if(c!='%')
		  strDst.AddChar(c);
		  //strDst+=str[i];
		else
		{
		  if(i+2<l)
		  {
			  c=0;
			  char ch=str[i+1];
			  if(ch>='0'&&ch<='9')
				  c=(ch-'0')<<4;
			  else if(ch>='a'&&ch<='f')
				  c=(ch-'a'+10)<<4;
			  else if(ch>='A'&&ch<='F')
				  c=(ch-'A'+10)<<4;
			  ch=str[i+2];
			  if(ch>='0'&&ch<='9')
				  c|=(ch-'0');
			  else if(ch>='a'&&ch<='f')
				  c|=(ch-'a'+10);
			  else if(ch>='A'&&ch<='F')
				  c|=(ch-'A'+10);
			  if(c)
				  strDst.AddChar(c);//+=c;
		  }
		  i+=2;
		}
	}
	cnt.pStack->Push(strDst);
}

void XXObjectCreator::Init(XBOOL nMode)
{
	//return;
	//XXSortToken::InitDicts(nMode);
	if(m_pClip&&m_pTokens)
	{
		XBOOL bLowCase=XTRUE;
		int nSize=m_pTokens->GetSize();
		XXTOKEN**p=m_pTokens->m_pData;//DataTo();
		int s=m_pTokens->m_nBufSize;
		XXSortToken ps(m_pTokens);
		for(int i=0;i<nSize;i++)
		{
			XXTOKEN*token=p[i];
			int id,mi;
			id=ps.Index((void*)token->strKey.strTxt,mi,bLowCase?XSS_NOCASE:XSS_NORMAL);
			m_pTokens->InsertAt(mi,token);
			//AddMember(
		}
		XXVar::FreeData(p,s);
		//delete p;
		return;
	}
	Release();
	InitObject();

	

	m_pSuper=XXObject::CreateClass(XNULL,XTRUE,m_pRoot);
	REFOBJECT(m_pSuper);


	if(m_pSuper)
	{
		
		m_pClip=XXObjectClip::CreateClass(m_pSuper,XTRUE,m_pRoot);
		m_pButton=XXObjectButton::CreateClass(m_pSuper,XTRUE,m_pRoot);
		m_pText=XXObjectText::CreateClass(m_pSuper,XTRUE,m_pRoot);
		
		//REFOBJECT(m_pClip);
		//REFOBJECT(m_pButton);
		//AddMember(
		//REFOBJECT(m_pText);
		AddMember(_SYSVAR(Object),XXVar(m_pSuper));

		AddMember(_SYSVAR(TextField),XXVar(m_pText));
		AddMember(_SYSVAR(Button),XXVar(m_pButton));
		AddMember(_SYSVAR(MovieClip),XXVar(m_pClip));

		//m_pConst=XXObjectConst::CreateClass(m_pSuper,XTRUE,m_pRoot);
		//REFOBJECT(m_pConst);
		//if(m_pConst)
		//	AddMember(XXVar(_strConst),XXVar(m_pConst));
		m_pBool=XXObjectBool::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pBool)
			AddMember(_SYSVAR(Boolean),XXVar(m_pBool));
		m_pString=XXObjectString::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pString)
			AddMember(_SYSVAR(String),XXVar(m_pString));
		m_pNumber=XXObjectNumber::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pNumber)
			AddMember(_SYSVAR(Number),XXVar(m_pNumber));
		
		if(m_pClip)
			AddMember(_SYSVAR(MovieClip),XXVar(m_pClip));
		m_pFunction=XXObjectFunction::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pFunction)
		{
			AddMember(_SYSVAR(Function),XXVar(m_pFunction));
			//XString16 str(_strFunction);
			//str.MakeLower();
			//AddMember(str,XXVar(m_pFunction));
		}
		m_pArray=XXObjectArray::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pArray)
		{
			AddMember(_SYSVAR(Array),XXVar(m_pArray));
			//XString16 str(_strArray);
			//str.MakeLower();
			//AddMember(str,XXVar(m_pArray));
		}
		m_pArgs=new XXObjectArgs(m_pRoot,m_pSuper,XTRUE);
		REFOBJECT(m_pArgs);
		XXObjectMath*pMath=XXObjectMath::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(pMath)
		{
			AddMember(_SYSVAR(Math),XXVar(pMath));
			//XString16 str(_strMath);
			//str.MakeLower();
			//AddMember(str,XXVar(pMath));
//				AddMember(XXVar(_strMathc,XXVar(pMath));
		}
		
		m_pKey=XXObjectKey::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pKey)
		{
			AddMember(_SYSVAR(Key),XXVar(m_pKey));
			//XString16 str(_strKey);
			//str.MakeLower();
			//AddMember(str,XXVar(pKey));
//				AddMember(XXVar(_strKeyc,XXVar(pKey));
		}
		XXObjectBitmap*pBmp=XXObjectBitmap::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(pBmp)
		{
			AddMember(_SYSVAR(BitmapData),XXVar(pBmp));
			//XString16 str(_strBitmap);
			//str.MakeLower();
			//AddMember(str,XXVar(pBmp));
		}
		m_pPoint=XXObjectPoint::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pPoint)
		{
			AddMember(_SYSVAR(Point),XXVar(m_pPoint));
			//XString16 str(_strPoint);
			//str.MakeLower();
			//AddMember(str,XXVar(m_pPoint));
		}
		m_pRect=XXObjectRectangle::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pRect)
		{
			AddMember(_SYSVAR(Rectangle),XXVar(m_pRect));
			//XString16 str(_strRectangle);
			//str.MakeLower();
			//AddMember(str,XXVar(m_pRect));
		}
		m_pStage=XXObjectStage::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pStage)
		{
			AddMember(_SYSVAR(Stage),XXVar(m_pStage));
			//XString16 str(_strStage);
			//str.MakeLower();
			//AddMember(str,XXVar(pStage));
		}
		m_pMouse=XXObjectMouse::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pMouse) 
		{
			AddMember(_SYSVAR(Mouse),XXVar(m_pMouse));
			//XString16 str(_strMouse);
			//str.MakeLower();
			//AddMember(str,XXVar(pMouse));
		}
		XXObjectColor*pColor=XXObjectColor::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(pColor)
		{
			AddMember(_SYSVAR(Color),XXVar(pColor));
			//XString16 str(_strColor);
			//str.MakeLower();
			//AddMember(str,XXVar(pColor));
		}
		m_pColorTransform=XXObjectColorTransform::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pColorTransform)
		{
			AddMember(_SYSVAR(ColorTransform),XXVar(m_pColorTransform));
			//XString16 str(_strColorTransform);
			//str.MakeLower();
			//AddMember(str,XXVar(m_pColorTransform));
		}
		m_pMatrix=XXObjectMatrix::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pMatrix)
		{
			AddMember(_SYSVAR(Matrix),XXVar(m_pMatrix));
			//XString16 str(_strMatrix);
			//str.MakeLower();
			//AddMember(str,XXVar(m_pMatrix));
		}
		m_pTransform=XXObjectTransform::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(m_pTransform)
		{
			//AddMember(XXVar(_strTransform,XXVar(m_pTransform));
			AddMember(_SYSVAR(Transform),XXVar(m_pTransform));
		}
		XXObjectShared*ps=XXObjectShared::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(ps)
		{
			AddMember(_SYSVAR(SharedObject),XXVar(ps));
			//XString16 str(_strSharedObject);
			//str.MakeLower();
			//AddMember(str,XXVar(ps));
		}
		XXObjectSystem*pSystem=XXObjectSystem::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(pSystem)
		{
			AddMember(_SYSVAR(System),XXVar(pSystem));
		}
		XXObjectError*pError=XXObjectError::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(pError)
			AddMember(_SYSVAR(Error),XXVar(pError));
		XXObjectDate*pDate=XXObjectDate::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(pDate)
			AddMember(_SYSVAR(Date),XXVar(pDate));
		XXObjectSound*pSound=XXObjectSound::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(pSound)
			AddMember(_SYSVAR(Sound),XXVar(pSound));
		XXObjectLoadVars*pVar=XXObjectLoadVars::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(pVar)
			AddMember(_SYSVAR(LoadVars),XXVar(pVar));
		XXObjectAsBroadcaster*pCaster=XXObjectAsBroadcaster::CreateClass(m_pSuper,XTRUE,m_pRoot);
		if(pCaster)
			AddMember(_SYSVAR(AsBroadcaster),XXVar(pCaster));
		//if(m_pButton)
		//	AddMember(XXVar(_strButton,XXVar(m_pButton));
	}
}



XBOOL XXObjectCreator::RegisterClass(XSWFCONTEXT&cnt,XXVar &var, XXObject *pObj)
{
	//return XFALSE;
	/*XXObject*pObject=XNULL;
	if(var.IsObject())
		pObject=var.pObject;
	else
	{
		var.ToString(XFALSE);
		XXVar vari;
		if(cnt.pPlayer->GetVariable(cnt,var.strTxt,vari)&&
		   vari.IsObject())
		   pObject=vari.pObject;
	}
	if(pObject&&pObject->m_pObject)
	{
		SObject*p=pObject->m_pObject;
		if(pObj==XNULL)
		{
			p->FreeObject();
			//p->pObject=XNULL;pClass->pObject->RefObject();
			//cnt.pStack->Push(XXVar::CreateBool(XTRUE));
		}
		else //if(->IsObject())
		{
			p->CreateObject(pObj,&cnt);			
			//pCnt->pStack->Push(XXVar::CreateBool(XTRUE));
			//return XTRUE;
		}
	}*/
	

	//m_pRoot->rootObject
	if(var.IsString())
	{
		//if(strcmp(var.strTxt,"engine")==0)
		//	int v=0;
		_registerClasses**p=&m_regs;
		for(;;)
		{
			_registerClasses*pi=*p;
			if(!pi) break;
			if(pi->strName==(XPCTSTR)var.strTxt)
			{
				FREEOBJECT(pi->pObject);
				if(pObj) pi->pObject=REFOBJECT(pObj);
				else 
				{
					*p=pi->pNext;
					delete pi;
				}
				break;
			}
			else
			{
				p=&pi->pNext;
			}
		}
		if(pObj)
		{
			_registerClasses*pNew=new _registerClasses;
			if(pNew)
			{
				pNew->pNext=m_regs;
				pNew->strName=var.strTxt;
				pNew->pObject=REFOBJECT(pObj);
				m_regs=pNew;
			}
		}
		if(m_pRoot->rootObject)
			m_pRoot->rootObject->UpdateClass(var,pObj,cnt);
	}
	return XTRUE;
}

XXObject* XXObjectCreator::FindRegisterClass(XPCTSTR strName)
{
	//return XNULL;
	_registerClasses*p=m_regs;
	while(p)
	{
		if(p->strName.Compare(strName,NeedLowCase())==0)
		{
			return p->pObject;//XXObject::CreateClass(p->pObject,XFALSE,m_pRoot);
		}
		p=p->pNext;
	}
	return XNULL;
}

void XXObjectCreator::loadVariables(XSWFCONTEXT &cnt, XXVARLIST &list)
{
	int nSize=list.GetSize();
	if(nSize<2) return;
	SObject*thisObj=cnt.pThis->m_pObject;
	if(!thisObj) return;
	int nMethod=kHttpLoadVariables|kHttpLoadTarget|kHttpSendUseGet;
	list[0].ToString(XFALSE);
	if(nSize>2)
	{
		list[2].ToString(XFALSE);
		switch(list[2].strTxt[0])
		{
		case 'p':
		case 'P':
			nMethod=kHttpLoadVariables|kHttpLoadTarget|kHttpSendUsePost;
			break;
		}
	}
	ScriptThread*thread=cnt.pPlayer->ThreadOf(cnt,thisObj,&list[1],CLIPTHREAD(cnt.thread));
	XXVar var;
	var.SetString("",0);
	cnt.pPlayer->GetTargetPath(thisObj,var);
	cnt.pPlayer->GetURL2(cnt,thread,list[0].strTxt,var.strTxt,nMethod);
	//FreeStr(target);
}

void XXObjectCreator::getGeom(XSWFCONTEXT &cnt, XXVARLIST &list)
{
	cnt.pStack->Push(this);
}
