// XXVar.cpp: implementation of the XXVar class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXVar.h"
#include "XXObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int XXVar::m_nVersion=8;

XFastAlloc XXVar::alloc64(64,400);//,XTRUE,0xF1111111);
XFastAlloc XXVar::alloc128(128,200);//,XTRUE,0xF2222222);
XFastAlloc XXVar::alloc256(256,80);//,XTRUE,0xF3333333);
XFastAlloc XXVar::alloc512(512,40);//,XTRUE,0xF4444444);
XFastAlloc XXVar::alloc1024(1024,20);//,XTRUE,0xF4444444);
XFastAlloc XXVar::alloc2048(2048,10);//,XTRUE,0xF4444444);
XFastAlloc XXVar::alloc4096(4096,5);//,XTRUE,0xF4444444);
XFastAlloc XXVar::alloc16384(16384,5);


//XFastAlloc XXVar::alloc10240(10240,4);
//XChunkAlloc XXVar::alloc1024(1024,1024,0xF5555555);

XFDOUBLE XXVar::ToFloat()
	{
		switch(nType)
		{
		case XODT_FUNCTION:
			 FREEOBJECT(pObject);
			 fData=0;
			 nType=XODT_FLOAT;
			 break;
		case XODT_OBJECT:
			{
			 XFDOUBLE v=pObject->GetNumber();
			 FREEOBJECT(pObject);
			 fData=v;
			 nType=XODT_FLOAT;
			}
			 break;
		case XODT_CONST:
			 fData=0; 
			 nType=XODT_FLOAT;
			 break;
		case XODT_BOOLEAN:
		case XODT_INT:
			 fData=(float)iData32;
			 nType=XODT_FLOAT;
			 break;
		case XODT_STRING:
			 //if(strData==XNULL)
			//	 int v=0;
			{
			 XFDOUBLE v=(XFDOUBLE)XString8::ToFloat(strTxt,GetLength());//(float)XString8(strData,STRING_ATTACH).ToFloat();
			 Release();
			 fData=v;
			 nType=XODT_FLOAT;
			}
			 break;
		}	
		return fData;
	}
void XXVar::ToIntIf()
{
	if(nType!=XODT_FLOAT) return;
	int v=(int)fData;
	if(v==fData)
		ToInt();
}


void XXVar::StringExtract(XXVar&index,XXVar&count,XBOOL doMultiByte)
{
	int id=index.ToInt()-1;
	int c=count.ToInt();
	ToString(XFALSE);

	if(doMultiByte&&!IsMultiByte())
	{
		XPTSTR start=StringIndex(strTxt,id);
		XPTSTR end=StringIndex(start,c);
		id=start-strTxt;
		c=end-start;
	}
	
	if(id<0) id=0;
	if(c<0) c=0;
	if(c)
	{
		switch(nStringType)
		{
		case STRING_ATTACH:
			{
				XPTSTR str=strTxt;
				SetString(str+id,c);
				delete str;
			};break;
		case STRING_CONST:
			SetString(strTxt+id,c);
			break;
		default:
			{
		   	  XSTRINGDATA*pData=GetData();
			  if(id+c>pData->nLength) 
			  {
				  c=pData->nLength-id;
				  if(c<0) c=0;
			  }
			  //int al=l+sizeof(XSTRINGDATA);			  
			  if(pData->nRefs<2)//&&pData->nMaxLength>=al)
			  {
				  if(c)
					  XGlobal::Memcpy(strTxt,strTxt+id,c);
				  strTxt[c]=0;
				//	strTxt[l]=0;
			  }
			  else
			  {
					//int ol=XMIN(pData->nLength,l);
					XSTRINGDATA*pNew=AllocBuffer(c+sizeof(XSTRINGDATA)+1);
					XPTSTR strNew=(XPTSTR)(pNew+1);
					if(c)
						XGlobal::Memcpy(strNew,strTxt+id,c);
					strTxt=strNew;
					strTxt[c]=0;
					pData->nRefs--;
					pData=pNew;
			  }
			  pData->nLength=c;
			}
		}
		//XString8 tmp(strData,STRING_ATTACH);
		//XString8 sub;
	
		//tmp.Mid(sub,id,c);//index.iData32,count.iData32);
		//strData=sub.DataTo();
	}
}

void XXVar::StringLength(XBOOL doMultiByte)
{
	ToString(XFALSE);
	
	int count=0;
	XPTSTR string=strTxt;
	if (!doMultiByte || !IsMultiByte()) {
		count=strlen(string);
	} else {
		int count = 0;
		while (*string) {
			count++;
			if (IsLeadByte(*string)) {
				string++;
				if (!*string) { break; }
			}
			string++;
		}
		//return count;
	}
	Release();
	//delete strData;
	nType=XODT_INT;
	iData32=count;
	//XString8 str(strData,STRING_ATTACH);
	//nType=XODT_INT;
	//iData32=str.GetLength();
}

//DEL XU32 XXVar::Length()
//DEL {
//DEL 	switch(nType)
//DEL 	{
//DEL 	default:
//DEL 		 return 1;
//DEL 		 break;
//DEL 	case XODT_OBJECT:
//DEL 		 return pObject->Get
//DEL 	case XODT_STRING:
//DEL 		{
//DEL 			XString16 str(strData,STRING_ATTACH);
//DEL 			XU32 l=str.GetLength();
//DEL 			str.EmptyNoFree();
//DEL 			return l;
//DEL 		}
//DEL 	}
//DEL 	return 1;
//DEL }

XBOOL XXVar::ToLogic()
{
	XU8 bEQ=XFALSE;
	switch(nType)
	{
		case XODT_BOOLEAN:
			 return iData32;
		case XODT_FUNCTION:
			 bEQ=XTRUE;
			 FREEOBJECT(pObject);
			 break;
		case XODT_OBJECT:
			 if(pObject==XNULL)
				 bEQ=XFALSE;
			 else
				 bEQ=pObject->IsTrue();
			 FREEOBJECT(pObject);
			 //Release();
			 break;
		case XODT_CONST:
			 switch(iData32)
			 {
			 case XOCT_INFINITY:
			 case XOCT_RINFINITY:bEQ=XTRUE;
			 default:bEQ=XFALSE;
			 }break;
		case XODT_FLOAT:
			 bEQ=fData!=0;
			 break;
		case XODT_INT:
			 bEQ=iData32!=0;
			 break;
		case XODT_STRING:
			{
				if(m_nVersion<=6)
				{
					bEQ=(strTxt[0]=='t'||strTxt[0]=='T');
					if(!bEQ) bEQ=ToInt()!=0;
				}
				else
					bEQ=strTxt[0]!=0;
				Release();
				//XString8 str(strData,STRING_ATTACH);
				//bEQ=str.ToLogic();
				//if(!bEQ) bEQ=str.ToInt()!=0;
			}
			 break;
	}
	nType=XODT_BOOLEAN;
	iData32=bEQ;
	return bEQ;
}


//DEL XBOOL XXVar::IsNum4()
//DEL {
//DEL     switch(GetDataType())
//DEL 	{
//DEL 	case XODT_CONST:
//DEL 		 switch(iData32)
//DEL 		 {
//DEL 		 case XOCT_UNDEFINE:
//DEL 		 case XOCT_NAN:return XFALSE;
//DEL 		 default:return XTRUE;
//DEL 		 }
//DEL 		 break;
//DEL 	case XODT_MOVIECLIP:
//DEL 	case XODT_FUNCTION:return XFALSE;
//DEL 	case XODT_INT:
//DEL 	case XODT_FLOAT:return XTRUE;
//DEL 	case XODT_STRING:
//DEL 		 ToString(XFALSE);
//DEL 
//DEL 		 /*if(nType==XODT_OBJECT)
//DEL 			 return pObject->IsNumber3();
//DEL 		 else
//DEL 		 {
//DEL 			 XString8 str(strTxt,STRING_ATTACH);
//DEL 			 XBOOL bNum=str.IsNum();
//DEL 			 str.EmptyNoFree();
//DEL 			 return bNum;
//DEL 		 }*/
//DEL 	}
//DEL 	return XFALSE;
//DEL }


XBOOL XXVar::IsNum3()
{
	switch(GetDataType())
	{
	case XODT_CONST:
		 switch(iData32)
		 {
		 case XOCT_UNDEFINE:
		 case XOCT_NAN:return XFALSE;
		 default:return XTRUE;
		 }
		 break;
	case XODT_MOVIECLIP:
	case XODT_FUNCTION:return XFALSE;
	case XODT_BOOLEAN:
	case XODT_INT:
	case XODT_FLOAT:return XTRUE;
	case XODT_STRING:
		 if(nType==XODT_OBJECT)
			 return pObject->IsNumber3();
		 else
		 {
			 XString8 str(strTxt,STRING_ATTACH);
			 XBOOL bNum=str.IsNum();
			 str.EmptyNoFree();
			 return bNum;
		 }
	}
	return XFALSE;
}


XU8 XXVar::GetDataType()const
{
	switch(nType)
	{
	case XODT_FUNCTION:
		 return XODT_FUNCTION;
	case XODT_OBJECT:
		 return pObject->GetDataType();
	}
	return nType;
}

XU8 XXVar::GetObjectType()const
{
	switch(nType)
	{
	case XODT_FUNCTION:
		 if(nObject) return XODT_OBJECT;
		 else		 return XODT_FUNCTION;
		 break;
	case XODT_OBJECT:
		{
			int t=pObject->GetDataType();
			switch(t)
			{
			  case XODT_FUNCTION:
			  case XODT_MOVIECLIP:return t;
			  default:return XODT_OBJECT;
			}
		}

	}
	return nType;
}

void XXVar::Less2(XXVar&v)
{
	int t1=GetDataType();
	int t2=v.GetDataType();
	switch(t2)
	{
	case XODT_CONST:
		 switch(v.ToInt())
		 {
		 case XOCT_UNDEFINE:
		 case XOCT_NAN:
			  ToConst(XOCT_UNDEFINE);
			  return;
		 case XOCT_INFINITY:
			  ToLogic();
			  iData32=XTRUE;
			  return;
		 case XOCT_RINFINITY:
			  ToLogic();
			  iData32=XFALSE;
			  return;
		 }break;
	}
	switch(t1)
	{
	case XODT_CONST:
		 switch(ToInt())
		 {
		 case XOCT_UNDEFINE:
		 case XOCT_NAN:
			  ToConst(XOCT_UNDEFINE);
			  return;
		 case XOCT_INFINITY:
			  ToLogic();
			  iData32=XFALSE;
			  return;
		 case XOCT_RINFINITY:
			  ToLogic();
			  iData32=XTRUE;
			  return;
		 }
	case XODT_STRING:
		 if(t2==XODT_STRING)
		 {
			ToString(XFALSE);
			v.ToString(XFALSE);
			XBOOL bLess=XString8::Compare(strTxt,v.strTxt,0,false)<0;
			ToLogic();
			iData32=bLess;
			return;
		 }
		 break;
	}
	ToFloat();
	v.ToFloat();
	iData32=fData<v.fData;
	nType=XODT_BOOLEAN;
}

void XXVar::EQ(XXVar&v)
{
	XU8 bEQ;//=iData32==v.iData32;
	//if(!bEQ)
	{
		int t=GetDataType();
		switch(t)
		{
		case XODT_CONST:
			 if(v.GetDataType()!=XODT_CONST) 
				bEQ=XFALSE;
			 else
				bEQ=iData32==v.ToInt();
			 break;
		case XODT_FUNCTION:
			 bEQ=pObject==v.pObject&&
				 nStringType==v.nStringType&&
				 nObject==v.nObject;
			 FREEOBJECT(pObject);
			 break;
		case XODT_OBJECT:
			 bEQ=pObject==v.pObject;
			 FREEOBJECT(pObject);
			 break;
        case XODT_STRING:
			 if(iData32==v.iData32)
			 {
				 bEQ=XTRUE;
				 Release();
				 break;
			 }
		default:
			
			ToFloat();
			v.ToFloat();
			bEQ=fData==v.fData;
		    break;
		}
	}
	//else Release();
	nType=XODT_BOOLEAN;
	iData32=bEQ;
}

void XXVar::Release()
{
	switch(nType)
	{
	case XODT_STRING:
		 switch(nStringType)
		 {
		 case STRING_ATTACH:
			  delete strTxt;
			  break;
		 case STRING_CONST:
//			  if(*strDebug!=strTxt)
//				  int v=0;
//			  delete strDebug;
			  break;
		 default:
			  FreeBuffer(GetData());
			  break;
		 }
		 nType=XODT_INT;
		 //strData=XNULL;
		 break;
	case XODT_FUNCTION:
	case XODT_OBJECT:
		 //pObject->FreeObject();
		 if(pObject)
			 pObject->FreeObject();
		 pObject=XNULL;
		 nType=XODT_INT;
		 break;
	}
	//nType=XODT_CONST;
	//iData32=XOCT_UNDEFINE;

}

XXVar::XXVar(XXObject*pObj)
{
	if(pObj)//&&pObj->GetRefs())
	{
		nType=XODT_OBJECT;
		pObject=pObj->RefObject();
	}
	else
	{
		nType=XODT_CONST;
		iData32=XOCT_NULL;
	}
}

void XXVar::operator=(XXObject*pObj)
{
	Release();
	if(pObj==XNULL)//||pObj->GetRefs()==0)
	{
		ToConst(XOCT_NULL);
		return;
	}
	
	nType=XODT_OBJECT;
	pObject=REFOBJECT(pObj);
}

//DEL void XXVar::Set(const XXVar &var)
//DEL {
//DEL 		switch(var.nType)
//DEL 		{
//DEL 		case XODT_STRING:
//DEL   			 //strData=XString8(var.strData).DataTo();
//DEL 			 
//DEL 			 switch(var.nStringType)
//DEL 			 {
//DEL 			 case STRING_CONST:
//DEL 				  nStringType=var.nStringType;
//DEL 				  strTxt=var.strTxt;
//DEL #ifdef _VARCACHE_
//DEL 				  nRefID=var.nRefID;
//DEL #endif
//DEL 				  break;
//DEL 			 case STRING_ATTACH:
//DEL 				  SetString(var.strTxt,XString8::SafeStrlen(var.strTxt));
//DEL 				  break;
//DEL 			 default:
//DEL 				  nStringType=var.nStringType;
//DEL 				  strTxt=var.strTxt;
//DEL 			 	  GetData()->nRefs++;
//DEL 				  break;
//DEL 			 }
//DEL 			 nType=var.nType;
//DEL 
//DEL 			 break;
//DEL 		case XODT_FUNCTION:
//DEL 			 nObject=var.nObject;
//DEL 			 pObject=REFOBJECT(var.pObject);
//DEL 			 nType=var.nType;
//DEL 			 nStringType=var.nStringType;
//DEL 			 break;
//DEL 		case XODT_OBJECT:
//DEL 			 nType=var.nType;
//DEL 			 if(var.pObject)
//DEL 				pObject=var.pObject->RefObject();
//DEL 			 else
//DEL 			 {
//DEL 				 nType=XODT_CONST;
//DEL 				 iData32=XOCT_NULL;
//DEL 			 }
//DEL 			 break;
//DEL 		case XODT_FLOAT:
//DEL 			 nType=var.nType;
//DEL 			 fData=var.fData;
//DEL 			 break;
//DEL 		default:
//DEL 			 nType=var.nType;
//DEL 			 iData32=var.iData32;
//DEL 		}
//DEL }
void XXVar::Set(const XXVar &var)
{
	switch(var.nType)
		{
		case XODT_STRING:
  			 //strData=XString8(var.strData).DataTo();
			 
			 switch(var.nStringType)
			 {
			 case STRING_CONST:
				  nStringType=var.nStringType;
				  strTxt=var.strTxt;
#ifdef _VARCACHE_
				  nRefID=var.nRefID;
#endif
				  break;
			 case STRING_ATTACH:
				  SetString(var.strTxt,XString8::SafeStrlen(var.strTxt));
				  break;
			 default:
				  nStringType=var.nStringType;
				  strTxt=var.strTxt;
			 	  GetData()->nRefs++;
				  break;
			 }
			 nType=var.nType;
			 break;
		case XODT_FUNCTION:
			 nObject=var.nObject;
			 pObject=REFOBJECT(var.pObject);
			 nType=var.nType;
			 nStringType=var.nStringType;
			 break;
		case XODT_OBJECT:
			 nType=var.nType;
			 if(var.pObject)
				pObject=var.pObject->RefObject();
			 else
			 {
				 nType=XODT_CONST;
				 iData32=XOCT_NULL;
			 }
			 break;
		case XODT_FLOAT:
			 nType=var.nType;
			 fData=var.fData;
			 break;
		default:
			 nType=var.nType;
			 iData32=var.iData32;
		}
}


/*XXVar::XXVar(const XXVar&var)
{
		switch(var.nType)
		{
		case XODT_STRING:
  			 //strData=XString8(var.strData).DataTo();
			 
			 switch(var.nStringType)
			 {
			 case STRING_CONST:
				  nStringType=var.nStringType;
				  strTxt=var.strTxt;
#ifdef _VARCACHE_
				  nRefID=var.nRefID;
#endif
				  break;
			 case STRING_ATTACH:
				  SetString(var.strTxt,XString8::SafeStrlen(var.strTxt));
				  break;
			 default:
				  nStringType=var.nStringType;
				  strTxt=var.strTxt;
			 	  GetData()->nRefs++;
				  break;
			 }
			 nType=var.nType;
			 break;
		case XODT_FUNCTION:
			 nObject=var.nObject;
			 pObject=REFOBJECT(var.pObject);
			 nType=var.nType;
			 nStringType=var.nStringType;
			 break;
		case XODT_OBJECT:
			 nType=var.nType;
			 if(var.pObject)
				pObject=var.pObject->RefObject();
			 else
			 {
				 nType=XODT_CONST;
				 iData32=XOCT_NULL;
			 }
			 break;
		case XODT_FLOAT:
			 nType=var.nType;
			 fData=var.fData;
			 break;
		default:
			 nType=var.nType;
			 iData32=var.iData32;
		}
}

void XXVar::operator=(const XXVar&var)
{
		Release();
		switch(var.nType)
		{
		case XODT_STRING:
  			 //strData=XString8(var.strData).DataTo();
			 
			 switch(var.nStringType)
			 {
			 case STRING_CONST:
				  nStringType=var.nStringType;
				  strTxt=var.strTxt;
#ifdef _VARCACHE_
				  nRefID=var.nRefID;
#endif
				  break;
			 case STRING_ATTACH:
				  SetString(var.strTxt,XString8::SafeStrlen(var.strTxt));
				  break;
			 default:
				  nStringType=var.nStringType;
				  strTxt=var.strTxt;
			 	  GetData()->nRefs++;
				  break;
			 }
			 nType=var.nType;

			 break;
		case XODT_FUNCTION:
			 nObject=var.nObject;
			 pObject=REFOBJECT(var.pObject);
			 nType=var.nType;
			 nStringType=var.nStringType;
			 break;
		case XODT_OBJECT:
			 nType=var.nType;
			 if(var.pObject)
				pObject=var.pObject->RefObject();
			 else
			 {
				 nType=XODT_CONST;
				 iData32=XOCT_NULL;
			 }
			 break;
		case XODT_FLOAT:
			 nType=var.nType;
			 fData=var.fData;
			 break;
		default:
			 nType=var.nType;
			 iData32=var.iData32;
		}
}*/




/*void XXVar::SetContext(XXObject*pObj)
{
	if(nType!=XODT_OBJECT) return;
	if(pObject->GetContext()==XNULL)
		pObject->SetContext(pObj);
}*/

/*void XXVar::operator=(XXObject*pObj)
{
	Release();
	if(pObj==XNULL)//||pObj->GetRefs()==0)
	{
		ToConst(XOCT_NULL);
		return;
	}
	
	nType=XODT_OBJECT;
	pObject=REFOBJECT(pObj);
}*/

void XXVar::EQ2(XXVar&v)
{
	XU8 bEQ=XFALSE;//iData32==v.iData32;
	//if(!bEQ)
	{
		XU8 t1=v.GetDataType();
		switch(GetDataType())
		{
		case XODT_CONST:
			 if(t1==XODT_CONST)
			 {
				 //ToInt();
				 //v.ToInt();
				 //bEQ=iData32==v.iData32;
				 switch(iData32)
				 {
				 case XOCT_NULL:
				 case XOCT_UNDEFINE:bEQ=XTRUE;break;
				 case XOCT_NAN:bEQ=XFALSE;break;
				 default:
					  bEQ=iData32==v.iData32;
					  break;
				 }
			 }
			 else
				 bEQ=XFALSE;
			 //if(bEQ&&iData32==XOCT_UNDEFINE)
			//	 int vv=0;
			 break;
		case XODT_BOOLEAN:
		case XODT_INT:		 
		case XODT_FLOAT:
			 switch(t1)
			 {
			 case XODT_CONST:bEQ=XFALSE;break;
			 case XODT_STRING:
				  if(!v.IsNum3())
				  {	  bEQ=XFALSE;break;}
			 case XODT_BOOLEAN:
			 case XODT_FLOAT:
			 case XODT_INT:
				  ToFloat();
				  v.ToFloat();
				  
				  bEQ=fData==v.fData;
				  break;
			 default:
				  bEQ=XFALSE;
			 }
			 break;
		case XODT_STRING:
			 switch(t1)
			 {
			 case XODT_CONST:bEQ=XFALSE;break;
			 case XODT_BOOLEAN:
			 case XODT_FLOAT:
			 case XODT_INT:
				  if(!IsNum3())
				  {
					  bEQ=XFALSE;
					  break;
				  }
				  ToFloat();
				  v.ToFloat();
				  bEQ=fData==v.fData;
				  break;
			 case XODT_STRING:
				  StringEQ(v);
				  //bOK=GetNumber()==pObj->GetNumber();
				  return;		 
			 default:
				  bEQ=XFALSE;
			 }break;
		case XODT_FUNCTION:
			 bEQ=pObject==v.pObject&&nStringType==v.nStringType;
			 break;
		default:
			 bEQ=pObject==v.pObject;
			 break;
		}
	}
	Release();
	nType=XODT_BOOLEAN;
	iData32=bEQ;
}

//DEL XXObject* XXVar::ToObject()
//DEL {
//DEL 	/*switch(nType)
//DEL 	{
//DEL 	case XODT_CONST:
//DEL 		 pObject=XXObjectCreator::CreateConst((XU8)iData32);
//DEL 		 if(pObject)
//DEL 			pObject->RefObject();
//DEL 		 break;
//DEL 	case XODT_BOOLEAN:
//DEL 		 pObject=XXObjectCreator::CreateBool(iData32);//break;
//DEL 		 if(pObject)
//DEL 			pObject->RefObject();
//DEL 		 break;
//DEL 	case XODT_INT:
//DEL 		 pObject=XXObjectCreator::CreateNumber((float)iData32);//break;
//DEL 		 if(pObject)
//DEL 			pObject->RefObject();
//DEL 		 break;
//DEL 	case XODT_FLOAT:
//DEL 		 pObject=XXObjectCreator::CreateNumber(fData);//break;
//DEL 		 if(pObject)
//DEL 			pObject->RefObject();
//DEL 		 break;
//DEL 	case XODT_STRING:
//DEL 		{
//DEL 			XPWSTR str=strData;
//DEL 			pObject=XXObjectCreator::CreateString(strData);
//DEL 			pObject->RefObject();
//DEL 			delete str;
//DEL 		};
//DEL 		break;
//DEL 	}*/
//DEL 	nType=XODT_OBJECT;
//DEL 	return pObject;
//DEL }

int XXVar::ToInt()
{		
	switch(nType)
	{
	case XODT_FUNCTION:
		 FREEOBJECT(pObject);
		 iData32=0;
		 nType=XODT_INT;
		 break;
	case XODT_OBJECT:
		 //Release();
		{
		 int v=pObject->GetInt(); 
		 //Release();
		 FREEOBJECT(pObject);
		 iData32=v;
		 nType=XODT_INT;
		 break;
		}
	case XODT_CONST:
		 iData32=0;
		 break;
	case XODT_FLOAT:
		 iData32=(XINT)fData;
		 break;
	case XODT_STRING:
		{
			int v=XString8::ToInt(strTxt,GetLength(),10,XFALSE);
			Release();
			iData32=v;
		}
		 //iData32=XString8(strData,STRING_ATTACH).ToInt();
		 break;
	}
	nType=XODT_INT;
	return iData32;
}

void XXVar::ToString(XBOOL bForce)
{
	//if(nType==XODT_STRING) return;
	//XString8 s;
	switch(nType)
	{
	case XODT_STRING:
		 if(!bForce) 
			 return;
		 else
		 {			
			switch(nStringType)
			{
			case STRING_ATTACH:
				{
					XPTSTR str=strTxt;
					SetString(strTxt,XString8::SafeStrlen(strTxt));
					delete str;
				}
				 break;
			case STRING_CONST:
				 SetString(strTxt,XString8::SafeStrlen(strTxt));
				 break;
			default:
				{
					SetLength(GetData()->nLength,XTRUE);
				}break;
			}
		 }break;
	case XODT_FUNCTION:
		 FREEOBJECT(pObject);
		 nType=XODT_STRING;
		 nStringType=STRING_CONST;
		 strTxt=(XPTSTR)_strTypeString[XODT_FUNCTION];
		 break;
	case XODT_OBJECT:
		{
			XXObject*pObj=pObject;
			//SetString(XNULL,0);
			XXVar var;
			//if(pObj)
		    pObj->GetString(var);
			*this=var;
		}
		 break;
	case XODT_CONST:
		 if(bForce)
			 SetString(_strConstType[iData32],XString8::SafeStrlen(_strConstType[iData32]));
		 else
			*this=_strConstType[iData32];
		 break;
	case XODT_BOOLEAN:
		 //SetString(XNULL,10);
		 if(bForce)
		 {
			 XPCTSTR str=iData32?XSTRUE:XSFALSE;
			 SetString(str,XString8::SafeStrlen(str));
		 }
		 else
			*this=iData32?XSTRUE:XSFALSE;
		 //s.FromLogic((XBOOL)iData32);
		 break;
	case XODT_INT:
		 //s.FromInt(iData32);
		{
			int v=iData32;
			SetString(XNULL,32);
			//XString8 str(strTxt,STRING_ATTACH);
			//str.FromInt(v);
			//str.DataTo();
			XString8::FormatV(strTxt,v,XFALSE);
			GetData()->nLength=XString8::SafeStrlen(strTxt);
		}
		 break;
	case XODT_FLOAT:
		{

			XFDOUBLE v=fData;
			SetString(XNULL,32);
			XString8::FormatV(strTxt,v);
			GetData()->nLength=XString8::SafeStrlen(strTxt);
		}
		 break;
	}
	//Release();
	//nType=XODT_STRING;
	//strData=s.DataTo();		
}

XBOOL XXVar::IsObject(XU8 id) const
{
	if(pObject==XNULL) return XFALSE;
	if(!IsObject()) return XFALSE;
	return !id||pObject->GetObjectType()==id;
}


XBOOL XXVar::IsLeadByte(XU8 ch)
{
	return XFALSE;
}

XBOOL XXVar::IsMultiByte()
{
	return XFALSE;
}

XPTSTR XXVar::StringIndex(XPTSTR string, int index)
{
	while (index--) {
		if (IsLeadByte(*string)) {
			string++;
			if (!*string) { break; }
		}
		string++;
	}
	return string;
}



XBOOL XXVar::SetString(XPCTSTR strBuf, int l)
{
	if(nType!=XODT_STRING||nStringType!=STRING_REF)
	{
		Release();
		int nl=(l<<1)+sizeof(XSTRINGDATA)+1;
		if(nl<64) nl=64;
		XSTRINGDATA*pData=AllocBuffer(nl);
		if(pData==XNULL) 
			return XFALSE;
		strTxt=(XPTSTR)(pData+1);
		strTxt[l]=0;	
	}
	else 
	{
		if(!SetLength(l)) return XFALSE;
	}
	if(strBuf)
	{
		l=XMIN(l,(int)XString8::SafeStrlen(strBuf));
		XGlobal::Memcpy(strTxt,(void*)strBuf,l);
		strTxt[l]=0;
	}
	else l=0;
	GetData()->nLength=l;
	nType=XODT_STRING;
	nStringType=STRING_REF;
	return XTRUE;
}

void XXVar::AdJustLength()
{
	XSTRINGDATA*pData=GetData();
	pData->nLength=strlen(strTxt);
}

XBOOL XXVar::SetLength(int l,XBOOL bCopy)
{
	XSTRINGDATA*pData=GetData();
	int al=l+sizeof(XSTRINGDATA)+1;
	if(pData->nRefs<2&&(int)pData->nMaxLength>=al)
	{
		pData->nLength=l;
		strTxt[l]=0;
	}
	else
	{
		int ol=XMIN((int)pData->nLength,l);
		int nl=al+l;
		if(nl<64) nl=64;
		XSTRINGDATA*pNew=AllocBuffer(nl);
		if(pNew==XNULL) return XFALSE;
		pNew->nLength=l;
		XPTSTR strNew=(XPTSTR)(pNew+1);
		if(bCopy&&ol)
		{
			pNew->nLength=l;
			XGlobal::Memcpy(strNew,strTxt,ol);
		}
		else
			pNew->nLength=bCopy?l:0;
		strTxt=strNew;
		//strTxt[ol]=0;
		FreeBuffer(pData);
		
	}
	return XTRUE;
}

/*static int _nLastCount = 0;
static XU32 _nLastTime = 0;
static int  _SpeedA = 0;
void* XXVar::AllocData(int& nSize)
{
	if(nSize<=4096)
	{
		switch(_XAllocTab[(nSize-1)>>6])
		{
		case 0://if(nSize<=64)
				{
					XU32 t=XTime::GetTimeMSec();
					int  newCount = alloc64.m_nAllocs;
					if(_nLastTime==0)
					{
						_nLastTime = t;
						_nLastCount = newCount;
					}
					else if(newCount-_nLastCount>50||
						    t-_nLastTime>100)
					{
						_SpeedA = (newCount-_nLastCount)*1000/((int)(t-_nLastTime+1));
						if(_SpeedA>100)
							int v=0;
						_nLastTime = t;
						_nLastCount = newCount;
					}
				}
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
}*/


