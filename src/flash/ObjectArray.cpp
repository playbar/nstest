// ObjectArray.cpp: implementation of the ObjectArray class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectArray.h"
#include "ObjectCreator.h"
#include "splayer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
XXObject* XXObjectArray::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL)
{
   //XXObjectArray*pArg=pList->GetSize()>0?(*pList)[0]
   //if(pList->GetSize()>0&&
//	return 0;
   XXObjectArray*pObj=new XXObjectArray(m_pRoot);
   pObj->SetSuper(this,XFALSE);//(m_bBase&1)?this:m_pSuper,XFALSE);
   m_pSuperObj=GetSuperObject();
   //for(XU32 i=0;i<list.GetSize();i++)
	//   pObj->m_list.Push(list[i]);
   //pCnt->pStack->Push(pObj);//(new XXVar(pObj));
   pObj->Construct(pCnt,list,XNULL);
   return pObj;
}

#define XARRAYCLASS(TOKEN) XXCLASS(TOKEN,XXObjectArray,XXOBJ_ARRAY)

XINT XXObjectArray::XSortList::CompareData(XXVar &var1, XXVar &var2)
{
	int iRet=0;
	if(m_method.IsObject())
	{
		XU8 vars[sizeof(XXVar)*2];
		XXVARLIST list(vars,2);
		list.Push(var1);
		list.Push(var2);
		if(m_method.IsSysCall())
			m_method.pObject->SysCallMethod(m_method.nStringType,m_pContext,list);
		else
			m_method.pObject->Function(m_pContext,list,m_method.pObject->GetParent());
		list.Release();
//		list.FreeAll();
		XXVar var;
		//XXVar*pVar=
		m_pContext->pStack->Pop(var);
		iRet=var.ToInt();
		//SAFEDELETE(pVar);
		if(iRet>0) iRet=1;
		else if(iRet<0) iRet=-1;
	}
	else
	{
		
		if(m_nFlag&NUMERIC)
		{
			var1.ToFloat();
			var2.ToFloat();
			iRet=CompareDouble(var1.fData,var2.fData);
		}
		else
		{
			var1.ToString(XFALSE);
			var2.ToString(XFALSE);
			iRet=CompareString8(var1.strTxt,var2.strTxt,(m_nFlag&CASEINSENSITIVE)?XSS_NOCASE:XSS_NORMAL);
		}
	}
	return iRet;
}


inline XINT XXObjectArray::XSortList::Compare(void*iSrt,void*iDst,XU8 /*nMode*/)
{
	XXVar*pVar1=(XXVar*)iSrt;
	XXVar*pVar2=(XXVar*)iDst;

	if(m_pKey==XNULL||!pVar1->IsObject()||!pVar2->IsObject())
		return CompareData(*pVar1,*pVar2);
	//XXVar var1,var2;
    int index,iRet=0;	
	for(index=0;index<10;index++)
	{
		XXVar key;
		if(!GetKey(key,index)) break;
		XXVar var1,var2;
		key.ToString(XFALSE);
		if(!pVar1->pObject->GetMember(*m_pContext,key.strTxt,var1,XFALSE)) break;
		if(!pVar2->pObject->GetMember(*m_pContext,key.strTxt,var2,XFALSE)) break;
		iRet=CompareData(var1,var2);
		if(iRet) break;
	}
	return iRet;
}


XBOOL XXObjectArray::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(length):
		{
			int i,l=var.ToInt();
			if(l>XMAXARRAY) break;
			int s=m_list.GetSize();
			if(l<=0) {m_list.Release();break;}
			for(i=l;i<s;i++)
				m_list[i].Release();
			m_list.SetSize(l);
			for(i=s;i<l;i++)
				m_list[i].ToConst(XOCT_UNDEFINE);
		}
		 break;
	default:
		 return XXObject::SysSetMember(id,pCnt,var);
	}
	return XTRUE;
}


XBOOL XXObjectArray::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(length):
		var=m_list.GetSize();return XTRUE;
	case _SYSID(CASEINSENSITIVE):
		var=1;return XTRUE;
	case _SYSID(DESCENDING)://,XXVar(2)):
		var=2;return XTRUE;
	case _SYSID(UNIQUESORT)://,XXVar(4)):
		var=4;return XTRUE;
	case _SYSID(RETURNINDEXEDARRAY):
		var=8;return XTRUE;
	case _SYSID(NUMERIC):
		var=16;return XTRUE;
		 break;
	case _SYSID(pop):

	case _SYSID(concat):
	case _SYSID(push):
	case _SYSID(shift):
	case _SYSID(sort):
	case _SYSID(unshift):
	case _SYSID(slice):
	case _SYSID(splice):
	case _SYSID(reverse):
	case _SYSID(join):
	case _SYSID(sortOn):
		var=this;
		var.ToFunction(id);
		return XTRUE;
	}
	return XXObject::SysGetMember(id,pCnt,var);
}


XBOOL XXObjectArray::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
		case _SYSID(pop):
			 Pop(*pCnt,list);
			 return XTRUE;
		case _SYSID(concat):
			 Concat(*pCnt,list);
			 return XTRUE;
		case _SYSID(push):
			 Push(*pCnt,list);
			 return XTRUE;
		case _SYSID(shift):
			 Shift(*pCnt,list);
			 return XTRUE;
		case _SYSID(sort):
			 Sort(*pCnt,list);
			 return XTRUE;
		case _SYSID(unshift):
			 UnShift(*pCnt,list);
			 return XTRUE;
		case _SYSID(slice):
			 Slice(*pCnt,list);
			 return XTRUE;
		case _SYSID(splice):
			 Splice(*pCnt,list);
			 return XTRUE;
		case _SYSID(reverse):
			 Reverse(*pCnt,list);
			 return XTRUE;
		case _SYSID(join):
			 Join(*pCnt,list);
			 return XTRUE;
		case _SYSID(sortOn):
			 SortOn(*pCnt,list);
			 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}


void XXObjectArray::InitObject( )
{
	//AddMember(_strConstructor,XXVar(this));
	_SYSCALL(length);
	AddMember(_SYSVAR(CASEINSENSITIVE),XXVar(1));
	AddMember(_SYSVAR(DESCENDING),XXVar(2));
	AddMember(_SYSVAR(UNIQUESORT),XXVar(4));
	AddMember(_SYSVAR(RETURNINDEXEDARRAY),XXVar(8));
	AddMember(_SYSVAR(NUMERIC),XXVar(16));
	_SYSCALL(pop);
	_SYSCALL(concat);
	_SYSCALL(push);
	_SYSCALL(shift);
	_SYSCALL(sort);
	_SYSCALL(unshift);
	_SYSCALL(slice);
	_SYSCALL(splice);
	_SYSCALL(reverse);
	_SYSCALL(join);
	_SYSCALL(sortOn);
	/*AddMember(XXVar(_strCASEINSENSITIVE),XXVar(1));
	AddMember(XXVar(_strDESCENDING),XXVar(2));
	AddMember(XXVar(_strUNIQUESORT),XXVar(4));
	AddMember(XXVar(_strRETURNINDEXEDARRAY),XXVar(8));
	AddMember(XXVar(_strNUMERIC),XXVar(16));
	AddMember(XXVar(_strPop),XXVar(XXFUNOBJ(Pop)));
	AddMember(XXVar(_strConcat),XXVar(XXFUNOBJ(Concat)));
	AddMember(XXVar(_strPush),XXVar(XXFUNOBJ(Push)));
	AddMember(XXVar(_strShift),XXVar(XXFUNOBJ(Shift)));
	AddMember(XXVar(_strSort),XXVar(XXFUNOBJ(Sort)));
	AddMember(XXVar(_strUnshift),XXVar(XXFUNOBJ(UnShift)));
	AddMember(XXVar(_strSlice),XXVar(XXFUNOBJ(Slice)));
	AddMember(XXVar(_strSplice),XXVar(XXFUNOBJ(Splice)));
	AddMember(XXVar(_strReverse),XXVar(XXFUNOBJ(Reverse)));
	AddMember(XXVar(_strJoin),XXVar(XXFUNOBJ(Join)));
	AddMember(XXVar(_strSortOn),XXVar(XXFUNOBJ(SortOn)));
	AddProperty(XXVar(_strLength),XXFUNOBJ(arrayLenGet),XXFUNOBJ(arrayLenSet));*/
}

void XXObjectArray::arrayLenSet(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	int i;
	if(list.GetSize()<=0) return;
	int l=list[0].ToInt();
	if(l>XMAXARRAY) return;
	int s=m_list.GetSize();
	if(l<=0) {m_list.Release();return;}
	for(i=l;i<s;i++)
		m_list[i].Release();
		//delete m_list[i];
	
	m_list.SetSize(l);
	for(i=s;i<l;i++)
		m_list[i].ToConst(XOCT_NULL);
}
void XXObjectArray::arrayLenGet(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//if(m_list.GetSize()>20)
	//	int v=0;
	//int v=m_list.GetSize();
	//if(v>1) v=1;
	cnt.pStack->Push((int)m_list.GetSize());//new XXVar((int)m_list.GetSize()));
}	


void XXObjectArray::Concat(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XXObjectArray*pArray=Clone(XTRUE);
	for(XU32 i=0;i<list.GetSize();i++)
	{
		if(list[i].IsObject()&&
		   list[i].pObject->IsArray())
		{
			XXStack&l=((XXObjectArray*)list[i].pObject)->m_list;
			for(XU32 j=0;j<l.GetSize();j++)
				pArray->m_list.Push(l[j]);
		}
		else
		   pArray->m_list.Push(list[i]);
	}
	cnt.pStack->Push(pArray);
}
void XXObjectArray::Pop(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(m_list.GetSize()<=0)
		cnt.pStack->PushConst(XOCT_NULL);//XXVar::CreateConst(XOCT_NULL));
	else
	{
		XXVar var;
		m_list.Pop(var);
		cnt.pStack->Push(var);//*m_list.RemoveLast());
	}
}
void XXObjectArray::Join(XSWFCONTEXT&cnt,XXVARLIST&list)
{

	XXVar str(_strEmpty);
	XPCTSTR strSp=_strSplitQ;
	int nSize=list.GetSize();
	if(nSize)
	{
		list[0].ToString(XFALSE);
		strSp=list[0].strTxt;
	}
	//if(strSp[0]==',')
	//	int v=0;
	for(XU32 i=0;i<m_list.GetSize();i++)
	{
		XXVar var=m_list[i];
		//if(i==22&&m_list[i].IsObject())
		//	int v=0;
		var.ToString(XFALSE);
		if(!str.IsNull())//IsEmpty())
			str.StringAdd(strSp);
			//str+=;
		str.StringAdd(var);
		//str+=var.strTxt;
	}
	//GetString(var);
	cnt.pStack->Push(str);
}
void XXObjectArray::Push(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	for(XU32 i=0;i<list.GetSize();i++)
		m_list.Push(list[i]);
	cnt.pStack->Push((int)m_list.GetSize());
}
void XXObjectArray::Reverse(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	int nSize=m_list.GetSize()>>1;
	int nMax=m_list.GetSize()-1;
	for(int i=0;i<nSize;i++)
	{
		XXVar var=m_list[i];
		m_list[i]=m_list[nMax-i];
		m_list[nMax-i]=var;
	}
}
void XXObjectArray::Shift(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(m_list.GetSize()<=0)
		cnt.pStack->PushConst(XOCT_NULL);//(XXVar::CreateConst(XOCT_NULL));
	else
	{
		cnt.pStack->Push(m_list[0]);
		m_list.RemoveAt(0);
	}
}
void XXObjectArray::Slice(XSWFCONTEXT&cnt,XXVARLIST&ls)
{
	int nSize=ls.GetSize();
	int is=nSize>0?ls[0].ToInt():0;
	int ie=nSize>1?ls[1].ToInt():m_list.GetSize();
	XXObjectArray*pArray=Clone(XFALSE);
	if(is<0) is=m_list.GetSize()+is;
	if(ie<0) ie=m_list.GetSize()+ie;
	ie=ie-is;
	if(is<0) is=0;
	else if(is>(int)m_list.GetSize())
		is=m_list.GetSize();
	ie=is+ie;
	if(ie>(int)m_list.GetSize())
		ie=m_list.GetSize();
	/*nc+=id;
	if(id<0) id=0;	
	int nMax=m_list.GetSize()-1;
	if(nc>nMax) nc=nMax;*/
	for(int i=is;i<ie;i++)
		pArray->m_list.Push(m_list[i]);
	cnt.pStack->Push(pArray);
}
void XXObjectArray::Sort(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XU32 i;
	XU8 nFlag=list.GetSize()>1?list[1].ToInt():0;
	//XXObject*pMethod=XNULL;
	XXVar method;
	if(list.GetSize()>0&&list[0].IsObject())
		method=list[0];
	XSortList sort(&cnt,nFlag,method,m_list.GetSize(),XNULL);
	for(i=0;i<m_list.GetSize();i++)
		sort.Add(cnt,m_list[i]);
	XXObjectArray*pArray=Clone(XFALSE);
	XU8 bIndex=nFlag&XSortList::RETURNINDEXEDARRAY;
	if(!bIndex)
		m_list.Release();
	for(i=0;i<sort.list.GetSize();i++)
	{
		pArray->m_list.Push(sort.list[i]);
		if(!bIndex)
			m_list.Push(sort.list[i]);
	}
	cnt.pStack->Push(pArray);
	//XU8 bNum=nFlag&0x20;
	//XU8 bCase=nFlag&0x01;
	//XU8 bDes=nFlag&0x02;
	//XU8 bUni=nFlag&0x04;
	//XU8 bReta=nFlag&0x08;
	
}
void XXObjectArray::SortOn(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	XU32 i;
	if(list.GetSize()<=0) {Sort(cnt,list);return;}
	//list[0].ToString(XFALSE);
	XU8 nFlag=list.GetSize()>1?list[1].ToInt():0;
	XXVar method;
	XSortList sort(&cnt,nFlag,method,m_list.GetSize(),&list[0]);
	for(i=0;i<m_list.GetSize();i++)
		sort.Add(cnt,m_list[i]);//.strTxt);
	XXObjectArray*pArray=Clone(XFALSE);
	XU8 bIndex=nFlag&XSortList::RETURNINDEXEDARRAY;
	if(!bIndex)
		m_list.Release();
	if(nFlag&XSortList::DESCENDING)
	{
		XU32 nCount=sort.list.GetSize();
		for(i=0;i<nCount;i++)
		{
			pArray->m_list.Push(sort.list[nCount-i-1]);
			if(!bIndex)
				m_list.Push(sort.list[nCount-i-1]);
		}
	}
	else
	{
		for(i=0;i<sort.list.GetSize();i++)
		{
			pArray->m_list.Push(sort.list[i]);
			if(!bIndex)
				m_list.Push(sort.list[i]);
		}
	}
	cnt.pStack->Push(pArray);
}
void XXObjectArray::Splice(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//return;
	int nSize=list.GetSize();
	int id=nSize>0?list[0].ToInt():0;
	int nc=nSize>1?list[1].ToInt():m_list.GetSize()-id;
	int nMax=m_list.GetSize();
	if(id<0) {nc-=id;id=0;}
	if(nc+id>nMax) nc=nMax-id;
	XXObjectArray*pArray=Clone(XFALSE);
	if(id<nMax)
	{
		if(nc)
		{
			for(int i=0;i<nc;i++)
				pArray->m_list.Push(m_list[i+id]);
			m_list.RemoveAt(id,nc);
		}
		if(nSize>=3)		
		{
			//m_list.Append(
			for(int i=2;i<nSize;i++)
			{
				m_list.InsertAt(id+i-2,list[i]);
			}
		}
	}
	cnt.pStack->Push(pArray);
}
void XXObjectArray::UnShift(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	for(XU32 i=0;i<list.GetSize();i++)
		m_list.InsertAt(i,list[i]);
	cnt.pStack->Push((int)m_list.GetSize());
}


void XXObjectArray::GetData(XU8Array &datas)
{
	int nSize=m_list.GetSize();
	datas.SetSize(nSize,XFALSE);
	for(int i=0;i<nSize;i++)
		datas[i]=m_list[i].ToInt();
		//datas.Add(m_list[i]->ToInt());
}

void XXObjectArray::GetData(XIntArray &datas)
{
	int nSize=m_list.GetSize();
	datas.SetSize(nSize,XFALSE);
	for(int i=0;i<nSize;i++)
		datas[i]=m_list[i].ToInt();
		//datas.Add(m_list[i]->ToInt());
}

void XXObjectArray::GetData(XU32Array &datas)
{
    int nSize=m_list.GetSize();
	datas.SetSize(nSize,XFALSE);
	for(int i=0;i<nSize;i++)
		datas[i]=m_list[i].ToInt();
		//datas.Add(m_list[i]->ToInt());
}


