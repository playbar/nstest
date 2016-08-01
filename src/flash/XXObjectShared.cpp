// XXObjectShared.cpp: implementation of the XXObjectShared class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectShared.h"
#include "splayer.h"
#include "ObjectCreator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectShared::XXObjectShared(ScriptPlayer*pRoot):XXObject(pRoot)
{
   m_pData=XNULL;
   m_strPath=_strEmpty;
}

XXObjectShared::~XXObjectShared()
{
	SaveObject();
	FREEOBJECT(m_pData);	
}

//DEL XXObjectShared* XXObjectShared::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list)
//DEL {
//DEL 	//XXObjectShared*p=CreateClass(m_bBase?this:m_pSuper,XNULL,XFALSE);
//DEL 	//pCnt->pStack->Push((p));
//DEL 	//return p;
//DEL 	getLocal(*pCnt,list);
//DEL 	return 1;//pCnt->pStack->GetLast()->pObject;
//DEL }

XBOOL XXObjectShared::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
	switch(id)
	{
	case _SYSID(data):
		 if(!m_pData)
		 {
			m_pData=m_pRoot->m_pGlobal->CreateObject();
			REFOBJECT(m_pData);
		 }
		 var=m_pData;
		 return XTRUE;
	case _SYSID(clear):
	case _SYSID(flush):
	case _SYSID(getLocal):
	case _SYSID(getSize):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectShared::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
	case _SYSID(clear):
		 if(!m_strPath.IsNull()) //return;
		 {
			XFile::RemoveFile(m_strPath.strTxt);
			FREEOBJECT(m_pData);
		 }
		 pCnt->pStack->PushBool(XTRUE);
		 return XTRUE;
	case _SYSID(flush):
		 SaveObject();
		 pCnt->pStack->Push(m_pData);
		 return XTRUE;
	case _SYSID(getLocal):
		 CreateObject(pCnt,list,XTRUE);
		 return XTRUE;
	case _SYSID(getSize):
		 int s=SaveObject();
		 if(s) s-=3;
		 pCnt->pStack->Push((s));
		 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}
XBOOL XXObjectShared::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(data):
		 if(var.IsObject()) return XTRUE;
			m_pData=REFOBJECT(var.pObject);
		 return XTRUE;
	}
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectShared::InitObject( )
{
	_SYSCALL(data);
	_SYSCALL(clear);
	_SYSCALL(flush);
	_SYSCALL(getLocal);
	_SYSCALL(getSize);
	//AddProperty(XXVar(_strDatas),XXFUNOBJ(dataGet),XXFUNOBJ(dataSet));
	//AddMember(XXVar(_strClear),XXVar(XXFUNOBJ(clear)));
	//AddMember(XXVar(_strFlush),XXVar(XXFUNOBJ(flush)));
	//AddMember(XXVar(_strGetLocal),XXVar(XXFUNOBJ(getLocal)));
	//AddMember(XXVar(_strGetSize),XXVar(XXFUNOBJ(getSize)));
}



XXObject* XXObjectShared::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL bObj)
{
	XXObject*pObj=XNULL;
	int nSize=list.GetSize();
	if(nSize>0)
	{		
		XXObjectShared*ps=CreateClass(m_bBase?this:m_pSuper,XFALSE,m_pRoot);
		if(ps)
		{
			/*list[0].ToString(XFALSE);
			ps->m_strPath=m_pRoot->splayer->m_strWorkPath;
			//ps->m_strPath+="\\";
			ps->m_strPath.StringAdd(m_pRoot->splayer->m_strAppName);
			ps->m_strPath.StringAdd(list[0].strTxt);
			ps->LoadObject();*/
			ps->Construct(pCnt,list,this);
			pObj=ps;
		}
	}
	else 
		pCnt->pStack->Push(pObj);
	return pObj;
}


XBOOL XXObjectShared::LoadObject()
{
	//return XFALSE;
	if(m_strPath.IsNull()) return XFALSE;
	if(!m_pData)
	{
		m_pData=m_pRoot->m_pGlobal->CreateObject();
		if(m_pData)
			m_pData->RefObject();
	}
	if(!m_pData) return XFALSE;
	XFile file;
	if(file.Open(m_strPath.strTxt,XFile::XBINARY|XFile::XREAD))
	{
		XU8 chars[3];
		if(file.Read(chars,3)==3&&
		   chars[0]=='x'&&
		   chars[1]=='f'&&
		   chars[2]=='d')
		{
		   XU16 nSize=0;
		   if(file.Read(&nSize,sizeof(nSize))==sizeof(nSize))
		   {
			   //XString8 strName;
			   XXVar name;
			   for(int i=0;i<nSize;i++)
			   {
				   XXVar var;
				   //XXVar*pVar=
				   if(Load(file,name,XTRUE,var))
				   {
					   m_pData->AddMember(name,var);
					   //delete pVar;
				   }
			   }
		   }
		}
		file.Close();
	}
	return XTRUE;
}

int XXObjectShared::SaveObject()
{
	if(m_pData==XNULL||m_strPath.IsNull()) return XFALSE;
	XFile file;
	int s=3;
	if(file.Open(m_strPath.strTxt,XFile::XBINARY|XFile::XWRITE|XFile::XCREATE))
	{
		file.Write((void*)"xfd",3);
		s+=Save(file,m_pData);
		file.Close();
	}
    return s;
}

int XXObjectShared::Save(XFile &file, XXObject *pObj)
{
    if(pObj->m_pTokens==XNULL) return 0;
	XU16 nSize=pObj->m_pTokens->GetSize();
	file.Write(&nSize,sizeof(nSize));
	int ss=sizeof(nSize);
	for(int i=0;i<nSize;i++)
	{
		
		//XPCWSTR str=XXSortToken::NameOf((*pObj->m_pTokens)[i].nID);
		XPCTSTR str=(*pObj->m_pTokens)[i]->strKey.strTxt;
		if(str==XNULL) continue;
		
		//XString8 strName=str;
		ss+=Save(file,(*pObj->m_pTokens)[i]->member,str);		
		//Save(file,(*pObj->m_pTokens[i])->member);
	}
	return ss;
}

int XXObjectShared::Save(XFile &file, XXVar &var,XPCTSTR strName)
{
	    XU8 v=var.GetDataType();
		switch(v)
		{
		   default:return 0;
		   case XODT_CONST:			 
		   case XODT_INT:
			    var.ToInt();
				break;
		   case XODT_FLOAT:
			    var.ToFloat();
			    break;
		   case XODT_STRING:
			    var.ToString(XFALSE);
				break;
		   case XODT_OBJECT:
			    if(!var.IsObject(XXOBJ_ARRAY)) return 0;
				break;
		}

		int ss=Save(file,strName);
		
		file.Write(&v,sizeof(v));
		ss+=sizeof(v);
		
		switch(v)
		{
		case XODT_FLOAT:
			 file.Write(&var.fData,sizeof(var.fData));
			 ss+=sizeof(var.fData);
			 break;
		case XODT_CONST:
		case XODT_INT:
			 file.Write(&var.iData32,sizeof(var.iData32));
			 ss+=sizeof(var.iData32);
		     break;
		case XODT_STRING:
			 ss+=Save(file,var.strTxt);
				 //XU16 s=XString16::SafeStrlen(var.strData);
				 //file.Write(&s,sizeof(s));
				 //file.Write(var.strData,s);
			 break;
		case XODT_OBJECT:
			 if(var.IsObject(XXOBJ_ARRAY))
			 {
				//XString8 str;				
			    XXObjectArray*pArray=(XXObjectArray*)var.pObject;
				XU16 s=pArray->m_list.GetSize();
				file.Write(&s,sizeof(s));
				ss+=sizeof(s);
				for(int i=0;i<s;i++)
					ss+=Save(file,pArray->m_list[i],XNULL);
			 }break;
		}
		return ss;
}

XBOOL XXObjectShared::Load(XFile &file, XXVar &name, XU8 bName,XXVar&var)
{

	if(bName)
	{	
		if(!Load(file,name)) return XFALSE;
	}
	XU8 v=-1;//var.GetDataType();
	if(!file.Read(&v,sizeof(v))) return XNULL;
	
//	XXVar pVar;//=XNULL;
	
	switch(v)
	{
	   case XODT_CONST:
	   case XODT_INT:
		   {
			  XU32 iValue=0;
			  if(file.Read(&iValue,sizeof(iValue))==sizeof(iValue))
			  {
				  if(v==XODT_INT)
					  var=(int)(iValue);
				  else
					  var.ToConst(iValue);//=XXVar::CreateConst((XU8)iValue);
			  }
		   }
			break;
	   case XODT_FLOAT:
		   {
			   XFDOUBLE fValue;
			   if(file.Read(&fValue,sizeof(fValue))==sizeof(fValue))
				  var=(fValue);
		   }
			break;
	   case XODT_STRING:
		   {
			  //XString8 strValue;
			  Load(file,var);
				//pVar=(strValue,XTRUE);
		   }
			break;
	   case XODT_OBJECT:
			//if(!var.IsObject(XXOBJ_ARRAY)) return;
		   {
			   XU16 s=0;
			   if(file.Read(&s,sizeof(s))==sizeof(s))
			   {
				   XXObjectArray*pArray=m_pRoot->m_pGlobal->CreateArray();//new XXObjectArray;
				   if(pArray)
				   {
					   var=(pArray);
					   //XString8 str;
					   for(int i=0;i<s;i++)
					   {
						   XXVar var,name;
						   if(Load(file,name,XFALSE,var))
							  pArray->m_list.Push(var);
					   }
				   }
			   }
		   }
			break;
	}
	return XTRUE;
//	return pVar;
}

int XXObjectShared::Save(XFile &file, XPCTSTR strName)
{

	if(strName==XNULL) return 0;
	//XString8 str=strName;
	XU16 s=XString8::SafeStrlen(strName);
	int ss=sizeof(s);
	file.Write(&s,sizeof(s));
	ss+=s;
	file.Write(strName,s);
	return XTRUE;
}

XBOOL XXObjectShared::Load(XFile &file, XXVar&var)//XString8 &str)
{
	XU16 s;
	if(file.Read(&s,sizeof(s))!=sizeof(s)) return XFALSE;
	//str.SetSize(s+1);
	//XString8 strV;
	//strV.SetSize(s+1);
	if(!var.SetString(XNULL,s)) return XFALSE;	
	if(file.Read(var.strTxt,s)!=s) return XFALSE;
	var.GetData()->nLength=s;
	//var.SetString(XNULL,s);
	//str=strV;
	return XTRUE;
}
