// ObjectString.cpp: implementation of the ObjectString class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectString.h"
#include "ObjectCreator.h"
#include "splayer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define XXSTRINGCLASS(TOKEN) XXCLASS(TOKEN,XXObjectString,XXOBJ_STRING)

XBOOL XXObjectString::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
	switch(id)
	{
	case _SYSID(charAt):
	case _SYSID(charCodeAt):
	case _SYSID(concat):
	case _SYSID(fromCharCode):
	case _SYSID(indexOf):
	case _SYSID(lastIndexOf):
	case _SYSID(slice):
	case _SYSID(split):
	case _SYSID(substr):
	case _SYSID(substring):
	case _SYSID(toLowerCase):
	case _SYSID(toString):
	case _SYSID(toUpperCase):
	case _SYSID(valueOf):	
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	case _SYSID(length):
		 var=m_strValue.GetLength();//.GetSize();
		 return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectString::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{			
	switch(id)
	{
	case _SYSID(charAt):
		 charAt(*pCnt,list);
		 return XTRUE;
	case _SYSID(charCodeAt):
		 charCodeAt(*pCnt,list);
		 return XTRUE;
	case _SYSID(concat):
		 concatStr(*pCnt,list);
		 return XTRUE;
	case _SYSID(fromCharCode):
		 fromCharCode(*pCnt,list);
		 return XTRUE;
	case _SYSID(indexOf):
		 indexOf(*pCnt,list);
		 return XTRUE;
	case _SYSID(lastIndexOf):
		 lastIndexOf(*pCnt,list);
		 return XTRUE;
	case _SYSID(slice):
		 sliceStr(*pCnt,list);
		 return XTRUE;
	case _SYSID(split):
		 splitStr(*pCnt,list);
		 return XTRUE;
	case _SYSID(substr):
		 substr(*pCnt,list);
		 return XTRUE;
	case _SYSID(substring):
		 substring(*pCnt,list);
		 return XTRUE;
	case _SYSID(toLowerCase):
		 toLowerCase(*pCnt,list);
		 return XTRUE;
	case _SYSID(valueOf):
	case _SYSID(toString):
		 pCnt->pStack->Push(m_strValue);
		 return XTRUE;
	case _SYSID(toUpperCase):
		 toUpperCase(*pCnt,list);
		 return XTRUE;
		 
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}

XBOOL XXObjectString::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(length):
		{
			int l=var.ToInt();
			if(l>XMAXARRAY) return XTRUE;
			if(l<0) l=0;
			m_strValue.SetLength(l);//SetSize(l+1);
			m_strValue.strTxt[l]=0;
		}return XTRUE;
	}
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectString::InitObject()
{
   //AddMember(_strConstructor,XXVar(this));
	_SYSCALL(length);
	_SYSCALL(charAt);
	_SYSCALL(charCodeAt);
   _SYSCALL(concat);
   _SYSCALL(fromCharCode);
   _SYSCALL(indexOf);
   _SYSCALL(lastIndexOf);
   _SYSCALL(slice);
   _SYSCALL(split);
   _SYSCALL(substr);
   _SYSCALL(substring);
   _SYSCALL(toLowerCase);
   _SYSCALL(toUpperCase);
  /* AddProperty(XXVar(_strLength),XXFUNOBJ(strLenGet),XXFUNOBJ(strLenSet));
   AddMember(XXVar(_strCharAt),XXVar(XXFUNOBJ(charAt)));
   AddMember(XXVar(_strCharCodeAt),XXVar(XXFUNOBJ(charCodeAt)));
   AddMember(XXVar(_strConcat),XXVar(XXFUNOBJ(concatStr)));
   AddMember(XXVar(_strFromCharCode),XXVar(XXFUNOBJ(fromCharCode)));
   AddMember(XXVar(_strIndexOf),XXVar(XXFUNOBJ(indexOf)));
   AddMember(XXVar(_strLastIndexOf),XXVar(XXFUNOBJ(lastIndexOf)));
   AddMember(XXVar(_strSlice),XXVar(XXFUNOBJ(sliceStr)));
   AddMember(XXVar(_strSplit),XXVar(XXFUNOBJ(splitStr)));
   AddMember(XXVar(_strSubstr),XXVar(XXFUNOBJ(substr)));
   AddMember(XXVar(_strSubstring),XXVar(XXFUNOBJ(substring)));
   AddMember(XXVar(_strToLowerCase),XXVar(XXFUNOBJ(toLowerCase)));
   AddMember(XXVar(_strToUpperCase),XXVar(XXFUNOBJ(toUpperCase)));*/
}


void XXObjectString::charAt(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XString8 str;
	XXVar var;
	var.SetString(XNULL,0);
	if(list.GetSize()>0)
	{
		int id=list[0].ToInt();
		if(id>=0&&id<(int)m_strValue.GetLength())
			var.AddChar(m_strValue.strTxt[id]);
			//str+=m_strValue.GetData()[id];
	}
	cnt.pStack->Push(var);//new XXVar(str,XTRUE));
}
void XXObjectString::charCodeAt(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	int v=0;
	if(list.GetSize()>0)
	{
		int id=list[0].ToInt();
		if(id>=0&&id<=(int)m_strValue.GetLength())
			v=m_strValue.strTxt[id];
	}
	cnt.pStack->Push(v);//new XXVar(v));
}
void XXObjectString::concatStr(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXVar str=m_strValue;
	if(list.GetSize()>0)
	{
		//list[0].ToString();
		str.StringAdd(list[0]);
		//str+=list[0].strData;
	}
	cnt.pStack->Push(str);//new XXVar(str,XTRUE));
}
void XXObjectString::fromCharCode(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XString8 str;
	XXVar var;
	var.SetString(XNULL,0);
	for(XU32 i=0;i<list.GetSize();i++)
	{
		var.AddWChar(list[i].ToInt());
	}
		//var.AddChar((
		//str+=(XWCHAR)list[i].ToInt();
	
	//var.AttachString(str.DataTo());
	cnt.pStack->Push(var);//new XXVar(str,XTRUE));
}
void XXObjectString::indexOf(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	int index=list.GetSize()>=2?list[1].ToInt():0;
	int v=-1;
	if(list.GetSize()>0)
	{
		list[0].ToString(XFALSE);
		XString8 str(m_strValue.strTxt,STRING_ATTACH);
		v=str.Find(list[0].strTxt,index);
		str.DataTo();
	}
	cnt.pStack->Push(v);
}
void XXObjectString::lastIndexOf(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	int v=-1;
	int myLen=m_strValue.GetLength();
	int index=list.GetSize()>1?list[1].ToInt():myLen-1;
	if(list.GetSize()>0)
	{
		list[0].ToString(XFALSE);
		XPCTSTR str=list[0].strTxt,srt=m_strValue.strTxt;
		int len=XString8::SafeStrlen(str);
		if(index+len>myLen) index=myLen-len;
		
		for(int i=index;i>=0;i--)
		{
			if(/*wcsncmp*/strncmp(srt+i,str,len)==0)
			{
				v=i;break;
			}
			/*if(srt[i]!=str[0]) continue;
			XU8 bOK=XTRUE;
			for(int j=1;j<len;j++)
			{
				if(srt[j+i]!=str[j]) {bOK=XFALSE;break;}
			}*/
		}
	}
	cnt.pStack->Push(v);
}
void XXObjectString::sliceStr(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	int start=list.GetSize()>0?list[0].ToInt():0;
	int end=list.GetSize()>1?list[1].ToInt():m_strValue.GetLength();
	//XString8 str;
	XXVar var;
	//var.SetString(XNULL,end-start);
	m_strValue.Mid(var,start,end-start);
	//XXVar var;
	//var.AttachString(str.DataTo());
	cnt.pStack->Push(var);
}
void XXObjectString::splitStr(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectArray*pArray=m_pRoot->m_pGlobal->CreateArray();
	XPCTSTR strSplit=" ";
	if(list.GetSize()>0)
	{
		list[0].ToString(XFALSE);
		strSplit=list[0].strTxt;
	}
	//if(strSplit[0]=='_')
	//	int v=0;
	int iLimit=list.GetSize()>1?list[1].ToInt():0;
	int iStart=0,iLen=XString8::SafeStrlen(strSplit);
	XPCTSTR strValue=m_strValue.strTxt;
	int len=m_strValue.GetLength();
	//XXVar var;
	//var.SetString(XNULL,0);
	XString8 str8(strValue,STRING_ATTACH);
	while(iStart<(int)len)
	{
		if(strSplit[0]==0)
		{
		   //XString8 str(strValue[iStart]);
		   XXVar var;
		   var.SetString(XNULL,0);
		   var.AddChar(strValue[iStart]);
		   iStart++;
		   //var.
		   pArray->m_list.Push(var);//Add(new XXVar(str,XTRUE));
		}
		else 
		{
			int id=str8.Find(strSplit,iStart);
			if(id<iStart)
			{
				//XString8 str(strValue+iStart);
				XXVar var;
				var.SetString(strValue+iStart,len-iStart);
				pArray->m_list.Push(var);//Add(new XXVar(str,XTRUE));
				break;
			}
			else if(id==iStart)
			{
				iStart+=iLen;
				pArray->m_list.Push(_strEmpty);//Add(new XXVar(_XXKEY("")));
			}
			else
			{
				//XString8 str(strValue+iStart,id-iStart);
				XXVar var;
				var.SetString(strValue+iStart,id-iStart);
				iStart=id+iLen;
				pArray->m_list.Push(var);//Add(new XXVar(str,XTRUE));
			}
		}
		if(iLimit&&(int)pArray->m_list.GetSize()>=iLimit) break;
	}
	str8.DataTo();
	cnt.pStack->Push(pArray);

}
void XXObjectString::substr(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	int iStart=list.GetSize()>0?list[0].ToInt():0;
	if(iStart<0)
		iStart=m_strValue.GetLength()+iStart;
	int iCount=list.GetSize()>1?list[1].ToInt():m_strValue.GetLength()-iStart;
	//XString8 str;
	XXVar var;
	m_strValue.Mid(var,iStart,iCount);
	cnt.pStack->Push(var);
}
void XXObjectString::substring(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	int iStart=list.GetSize()>0?list[0].ToInt():0;
//	if(iStart!=0)
//		int v=0;
	if(iStart<0) iStart=0;
	int iCount=list.GetSize()>1?list[1].ToInt()-iStart:m_strValue.GetLength()-iStart;
	//XString8 str;
	XXVar str(_strEmpty);
	if(iCount>0)
		m_strValue.Mid(str,iStart,iCount);
	//if(str.IsUndefine())
	//	int v=0;
	cnt.pStack->Push(str);//new XXVar(str,XTRUE));
}
void XXObjectString::toLowerCase(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XString8 str=m_strValue;
	//str.MakeLower();
	XXVar var=m_strValue;
	var.ToString(XTRUE);
	XString8 str8(var.strTxt,STRING_ATTACH);
	str8.MakeLower();
	str8.DataTo();
	cnt.pStack->Push(var);//new XXVar(str,XTRUE));
}
void XXObjectString::toUpperCase(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XString8 str=m_strValue;
	XXVar var=m_strValue;
	var.ToString(XTRUE);
	XString8 str(var.strTxt,STRING_ATTACH);
	str.MakeUpper();
	str.DataTo();
	cnt.pStack->Push(var);//new XXVar(str,XTRUE));
}


XXObject* XXObjectString::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
   XXObject*pObj;
   if(list.GetSize()<=0)
	   pObj=new XXObjectString(m_pRoot);//,"");
   else
   {
	   list[0].ToString(XFALSE);
	   pObj=new XXObjectString(m_pRoot,list[0]);//XXObjectCreator::CreateString(list[0].strData);
   }
   if(pObj)
   {
	 pObj->m_pSuperObj=GetSuperObject();
	 pObj->SetSuper(/*(m_bBase&1)?this:m_pSuper*/this,XFALSE);
   }
//   pObj->SetType(XOOT_OBJECT);
   pCnt->pStack->Push(pObj);//new XXVar(pObj));
   return pObj;
}

XU32 XXObjectString::Function(XSWFCONTEXT *pCnt, XXVARLIST &list)
{
   //XXObject*pObj;
   //XXVar*pVar;
   XXVar var;
   if(list.GetSize()<=0)
	   var=_strEmpty;
	   //pVar=new XXVar("");
   else
   {
	   list[0].ToString(XFALSE);
	   var=list[0];
	   //pVar=new XXVar(list[0].strData);//XXObjectCreator::CreateString(list[0].strData);
   }
   //pObj->SetSuper(this);
   //pObj->SetType(XOOT_TYPE);
   pCnt->pStack->Push(var);//new XXVar(pObj));
   return XTRUE;
}
