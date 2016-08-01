// ObjectMath.cpp: implementation of the ObjectMath class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectMath.h"
#include "ObjectCreator.h"
#include "splayer.h"
#include "XActionScript.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//DEL XXObjectMath* XXObjectMath::CreateClass(XXObject *pSuper, XU8 bInit)
//DEL {
//DEL 	XXObjectMath*pObj=new XXObjectMath();
//DEL 	if(pObj)
//DEL 	{
//DEL 		if(bInit) pObj->InitObject();
//DEL 		pObj->SetSuper(pSuper,bInit);
//DEL 		pObj->m_bBase=bInit;
//DEL 	}
//DEL 	return pObj;
//DEL }

XXObject* XXObjectMath::CreateObject(XSWFCONTEXT *pCnt,XXVARLIST&,XBOOL)
{
   XXObjectMath*pObj=new XXObjectMath(m_pRoot);
   if(pObj)
   {
	   pObj->m_pSuperObj=GetSuperObject();
	   pObj->SetSuper(this,XFALSE);
   }
//   pObj->SetType(XOOT_OBJECT);
   pCnt->pStack->Push(pObj);//new XXVar(pObj));
   return pObj;
}

#include "math.h"

#define xabs(VV) VV>0?VV:-VV;
#define round(VV) (VV>=0?((int)(VV+0.5)):((int)(VV-0.5)))

XBOOL XXObjectMath::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
	switch(id)
	{
	   case _SYSID(tan)://AddMember(_strTan,XXVar(XXMATH(tan))):
	   case _SYSID(sin)://AddMember(_strSin,XXVar(XXMATH(sin))):
	   case _SYSID(sqrt)://AddMember(_strSqrt,XXVar(XXMATH(sqrt))):
	   case _SYSID(round)://AddMember(_strRound,XXVar(XXMATH(round))):
	   case _SYSID(random)://AddMember(_strRandom,XXVar(XXMATH(random))):
	   case _SYSID(min)://AddMember(_strMin,XXVar(XXMATH(xmin))):
	   case _SYSID(max)://AddMember(_strMax,XXVar(XXMATH(xmax))):
	   case _SYSID(pow)://AddMember(_strPow,XXVar(XXMATH(pow))):
	   case _SYSID(log)://AddMember(_strLog,XXVar(XXMATH(log))):
	   case _SYSID(floor)://AddMember(_strFloor,XXVar(XXMATH(floor))):
	   case _SYSID(exp)://AddMember(_strExp,XXVar(XXMATH(exp))):
	   case _SYSID(cos)://AddMember(_strCos,XXVar(XXMATH(cos))):
	   case _SYSID(ceil)://AddMember(_strCeil,XXVar(XXMATH(ceil))):
	   case _SYSID(acos)://AddMember(_strAcos,XXVar(XXMATH(acos))):
	   case _SYSID(asin)://AddMember(_strAsin,XXVar(XXMATH(asin))):
	   case _SYSID(abs)://AddMember(_strAbs,XXVar(XXMATH(fabs))):
	   case _SYSID(atan)://AddMember(_strAtan,XXVar(XXMATH(atan))):
	   case _SYSID(atan2)://AddMember(_strAtan2,XXVar(XXMATH(atan2))):
		   var=this;
		   var.ToFunction(id);
		   return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectMath::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	int nSize=list.GetSize();
	switch(id)
	{
	   case _SYSID(tan)://AddMember(_strTan,XXVar(XXMATH(tan))):
		   if(nSize) pCnt->pStack->Push((XFDOUBLE)tan(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE);
		   return XTRUE;
	   case _SYSID(sin)://AddMember(_strSin,XXVar(XXMATH(sin))):
		   if(nSize) pCnt->pStack->Push((XFDOUBLE)sin(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE);
		   return XTRUE;
	   case _SYSID(sqrt)://AddMember(_strSqrt,XXVar(XXMATH(sqrt))):
		   if(nSize) pCnt->pStack->Push((XFDOUBLE)sqrt(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE);
		   return XTRUE;
	   case _SYSID(round)://AddMember(_strRound,XXVar(XXMATH(round))):
		   if(nSize)
		   {
				XFDOUBLE f=list[0].ToFloat();
				//int      v=(int)(f>=0?f+0.5F:f-0.5F);
				//if(v==14)
				//	int v=0;
				//if(v<0)
				//	int tt=0;
				pCnt->pStack->Push((int)(f>=0?f+0.5F:f-0.5F));
			   //pCnt->pStack->Push((int)(list[0].ToFloat()));
		   }
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE);
		   return XTRUE;
	   case _SYSID(random)://AddMember(_strRandom,XXVar(XXMATH(random))):
		   {
				XFDOUBLE v=(XFDOUBLE)Random(0x7FFFFFFF)/0x7FFFFFFF;
					//XGlobal::Random();
				//if(v<0.1) v=0.1F;
				pCnt->pStack->Push(v);//new XXVar(v));
				return XTRUE;
			}
		   return XTRUE;
	   case _SYSID(min)://AddMember(_strMin,XXVar(XXMATH(xmin))):
		   if(nSize>=2)
		   {
			   XFDOUBLE f0=list[0].ToFloat();
			   XFDOUBLE f1=list[1].ToFloat();
			   pCnt->pStack->Push(XMIN(f0,f1));			   
		   }
		   else pCnt->pStack->PushConst(XOCT_UNDEFINE);

		   return XTRUE;
	   case _SYSID(max)://AddMember(_strMax,XXVar(XXMATH(xmax))):
		   if(nSize>=2)
		   {
			   XFDOUBLE f0=list[0].ToFloat();
			   XFDOUBLE f1=list[1].ToFloat();
			   pCnt->pStack->Push(XMAX(f0,f1));			   
		   }
		   else pCnt->pStack->PushConst(XOCT_UNDEFINE);
		   return XTRUE;
	   case _SYSID(pow)://AddMember(_strPow,XXVar(XXMATH(pow))):
		   if(nSize>=2)
		   {
			   XFDOUBLE f0=list[0].ToFloat();
			   XFDOUBLE f1=list[1].ToFloat();
			   pCnt->pStack->Push((XFDOUBLE)pow(f0,f1));			   
		   }
		   else pCnt->pStack->PushConst(XOCT_UNDEFINE);
		   return XTRUE;
	   case _SYSID(log)://AddMember(_strLog,XXVar(XXMATH(log))):
		   if(nSize) pCnt->pStack->Push((XFDOUBLE)log(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
	   case _SYSID(floor)://AddMember(_strFloor,XXVar(XXMATH(floor))):
		   if(nSize) 
		   {
			   //XFDOUBLE f=list[0].ToFloat();
			   //if(f>=0) 
			   pCnt->pStack->Push((int)floor(list[0].ToFloat()));
			   //else	    
				//   pCnt->pStack->Push((int)f);
			   //pCnt->pStack->Push((XFDOUBLE)floor(list[0].ToFloat()));
		   }
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
	   case _SYSID(exp)://AddMember(_strExp,XXVar(XXMATH(exp))):
		   if(nSize) pCnt->pStack->Push((XFDOUBLE)exp(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
	   case _SYSID(cos)://AddMember(_strCos,XXVar(XXMATH(cos))):
		   if(nSize) pCnt->pStack->Push((XFDOUBLE)cos(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
	   case _SYSID(ceil)://AddMember(_strCeil,XXVar(XXMATH(ceil))):
		   if(nSize) pCnt->pStack->Push((int)ceil(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
		   return XTRUE;
	   case _SYSID(acos)://AddMember(_strAcos,XXVar(XXMATH(acos))):
		   if(nSize) pCnt->pStack->Push((XFDOUBLE)acos(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
	   case _SYSID(asin)://AddMember(_strAsin,XXVar(XXMATH(asin))):
		   if(nSize) pCnt->pStack->Push((XFDOUBLE)asin(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
	   case _SYSID(abs)://AddMember(_strAbs,XXVar(XXMATH(fabs))):
		   if(nSize) 
		   {
			   XFDOUBLE v=list[0].ToFloat();
			   if(v<0) v=-v;
			   pCnt->pStack->Push(v);
		   }
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
	   case _SYSID(atan)://AddMember(_strAtan,XXVar(XXMATH(atan))):
		   if(nSize) pCnt->pStack->Push((XFDOUBLE)atan(list[0].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
	   case _SYSID(atan2)://AddMember(_strAtan2,XXVar(XXMATH(atan2))):
		   if(nSize>=2) pCnt->pStack->Push((XFDOUBLE)atan2(list[0].ToFloat(),list[1].ToFloat()));
		   else	     pCnt->pStack->PushConst(XOCT_UNDEFINE); 
		   return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}

void XXObjectMath::InitObject()
{
   //float v=round(-0.7); 
   //AddMember(_strConstructor,XXVar(this));
   _SYSCALL(tan);
   _SYSCALL(sin);
   _SYSCALL(sqrt);
   _SYSCALL(round);
   _SYSCALL(random);
   _SYSCALL(min);
   _SYSCALL(max);
   _SYSCALL(pow);
   _SYSCALL(log);
   _SYSCALL(floor);
   _SYSCALL(exp);
   _SYSCALL(cos);
   _SYSCALL(ceil);
   _SYSCALL(acos);
   _SYSCALL(asin);
   _SYSCALL(abs);
   _SYSCALL(atan);
   _SYSCALL(atan2);
   //AddMember(_strRandom,XXVAR(XXMATH(random)));
   AddMember(_SYSVAR(E),XXVar(_floatE));
   AddMember(_SYSVAR(LN10),XXVar(_floatLN10));
   AddMember(_SYSVAR(LN2),XXVar(_floatLN2));
   AddMember(_SYSVAR(LOG10E),XXVar(_floatLOG10E));
   AddMember(_SYSVAR(LOG2E),XXVar(_floatLOG2E));
   AddMember(_SYSVAR(PI),XXVar(_floatPI));
   AddMember(_SYSVAR(SQRT1_2),XXVar(_floatSQRT1_2));
   AddMember(_SYSVAR(SQRT2),XXVar(_floatSQRT2));
}


