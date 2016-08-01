// XXObjectColorTransform.cpp: implementation of the XXObjectColorTransform class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectColorTransform.h"
#include "splayer.h"
#include "ObjectCreator.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectColorTransform::XXObjectColorTransform(ScriptPlayer*pRoot):XXObject(pRoot)
{
   /*alphaMulti=1;
   redMulti=1;
   greenMulti=1;
   blueMulti=1;
   alphaAdd=0;
   redAdd=0;
   greenAdd=0;
   blueAdd=0;*/
	//cxForm.SetAlpha(); 
	cxForm.nForeColor=0;
}

XXObjectColorTransform::~XXObjectColorTransform()
{

}

#define alphaAddTerm  ab
#define alphaMultTerm aa
#define blueAddTerm   bb
#define blueMultTerm  ba
#define redAddTerm	  rb
#define redMultTerm	  ra
#define greenAddTerm  gb
#define greenMultTerm ga

XXObject* XXObjectColorTransform::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
   XXObjectColorTransform*pObj=
	   m_pRoot->m_pGlobal->CreateColorTransform();
   if(pObj)
   {
	   pObj->m_pSuperObj=GetSuperObject();
	   pObj->SetSuper(this,XFALSE);
	   pObj->Construct(pCnt,list,this);
   }
   else
	   pCnt->pStack->Push((pObj));
   return pObj;
}

//#define XXFUN(TOKEN) XXCLASS(TOKEN,XXObjectColorTransform,XXOBJ_COLORTRANSFORM)
XBOOL XXObjectColorTransform::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(concat):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;
	case _SYSID(alphaMultiplier):
		 var=cxForm.alphaMultTerm/256.0F;
		 return XTRUE;
	case _SYSID(alphaOffset):
		 var=cxForm.alphaAddTerm;
		 return XTRUE;
	case _SYSID(blueMultiplier):
		 var=cxForm.blueMultTerm/256.0F;
		 return XTRUE;
	case _SYSID(blueOffset):
		 var=cxForm.blueAddTerm;
		 return XTRUE;
	case _SYSID(greenMultiplier):
		 var=cxForm.greenMultTerm/256.0F;
		 return XTRUE;
	case _SYSID(greenOffset):
		 var=cxForm.greenAddTerm;
		 return XTRUE;
	case _SYSID(redMultiplier):
		 var=cxForm.redMultTerm/256.0F;
		 return XTRUE;
	case _SYSID(redOffset):
		 var=cxForm.redAddTerm;
		 return XTRUE;
	case _SYSID(rgb):
		 var=(cxForm.redAddTerm<<16)|(cxForm.greenAddTerm<<8)|cxForm.blueAddTerm;
		 return XTRUE;
	}
	return XXObject::SysGetMember(id,pCnt,var);
}
XBOOL XXObjectColorTransform::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
		case _SYSID(concat):
			 concatCXForm(*pCnt,list);
			 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}
XBOOL XXObjectColorTransform::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
		case _SYSID(concat):
			 //var=this;
			 //var.ToFunction(id);
			 return XFALSE;//XTRUE;
		case _SYSID(alphaMultiplier):
			{
				cxForm.NeedMult();
				XFDOUBLE v=var.ToFloat()*256;
				cxForm.alphaMultTerm=XROUND(v);
			}
			return XTRUE;
		case _SYSID(alphaOffset):
			{ 
				cxForm.alphaAddTerm=var.ToInt();
				cxForm.NeedAdd();
			}
			return XTRUE;
		case _SYSID(blueMultiplier):
			{
				XFDOUBLE v=var.ToFloat()*256;
				cxForm.blueMultTerm=XROUND(v);
				cxForm.NeedMult();
			}
			return XTRUE;
		case _SYSID(blueOffset):
			cxForm.NeedAdd();
			cxForm.blueAddTerm=var.ToInt();
			return XTRUE;
		case _SYSID(greenMultiplier):
			{
				double v=var.ToFloat()*256;
				cxForm.greenMultTerm=XROUND(v);
				//cxForm.CalcFlags();
				cxForm.NeedMult();
			}
			return XTRUE;
		case _SYSID(greenOffset):
			cxForm.NeedAdd();
			cxForm.greenAddTerm=var.ToInt();
			return XTRUE;
		case _SYSID(redMultiplier):
			{
				XFDOUBLE v=var.ToFloat()*256;
				cxForm.redMultTerm=XROUND(v);
				//cxForm.CalcFlags();
				cxForm.NeedMult();
			}
			return XTRUE;
		case _SYSID(redOffset):
			cxForm.redAddTerm=var.ToInt();
			cxForm.NeedAdd();
			return XTRUE;
		case _SYSID(rgb):
			{
				int rgb=var.ToInt();
				//cxForm.hasAdd=XTRUE;
				cxForm.NeedAdd();
				cxForm.redAddTerm=(rgb>>16)&0xff;
				cxForm.greenAddTerm=(rgb>>8)&0xff;
				cxForm.blueAddTerm=(rgb&0xff);
			}
			return XTRUE;
	}
	return XXObject::SysSetMember(id,pCnt,var);
}


void XXObjectColorTransform::InitObject( )
{
	 _SYSCALL(alphaMultiplier);
	 _SYSCALL(alphaOffset);
	 _SYSCALL(blueMultiplier);
	 _SYSCALL(blueOffset);
	 _SYSCALL(greenMultiplier);
	 _SYSCALL(greenOffset);
	 _SYSCALL(redMultiplier);
	 _SYSCALL(redOffset);
	 _SYSCALL(rgb);
	 _SYSCALL(concat);

	 /*AddProperty(XXVar(_strAlphaMulti),XXFUNOBJ(alphaMultiGet),XXFUNOBJ(alphaMultiSet));
	 AddProperty(XXVar(_strRedMulti),XXFUNOBJ(redMultiGet),XXFUNOBJ(redMultiSet));
	 AddProperty(XXVar(_strGreenMulti),XXFUNOBJ(greenMultiGet),XXFUNOBJ(greenMultiSet));
	 AddProperty(XXVar(_strBlueMulti),XXFUNOBJ(blueMultiGet),XXFUNOBJ(blueMultiSet));
	 AddProperty(XXVar(_strAlphaOffset),XXFUNOBJ(alphaOffsetGet),XXFUNOBJ(alphaOffsetSet));
	 AddProperty(XXVar(_strRedOffset),XXFUNOBJ(redOffsetGet),XXFUNOBJ(redOffsetSet));
	 AddProperty(XXVar(_strGreenOffset),XXFUNOBJ(greenOffsetGet),XXFUNOBJ(greenOffsetSet));
	 AddProperty(XXVar(_strBlueOffset),XXFUNOBJ(blueOffsetGet),XXFUNOBJ(blueOffsetSet));
	 AddProperty(XXVar(_strRGB),XXFUNOBJ(rgbGet),XXFUNOBJ(rgbSet));

	 AddMember(XXVar(_strConcat),XXVar(XXFUNOBJ(concatCXForm)));*/
}

void XXObjectColorTransform::concatCXForm(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0&&!list[0].IsObject()) return;
	XXObjectColorTransform*p=(XXObjectColorTransform*)list[0].pObject;
	if(p->IsObject(XXOBJ_COLORTRANSFORM)) return;
	cxForm.Concat(&p->cxForm);
	//cxForm.MultiForm(p->cxForm);
	//XCXForm m=p->cxForm;
	//m.MultForm(cxForm);
	//cxForm=m;
	/*redMulti=redMulti*p->redMulti;
	greenMulti=greenMulti*p->greenMulti;
	blueMulti=blueMulti*p->blueMulti;
	alphaMulti=alphaMulti*p->alphaMulti;
	redAdd=(int)(redAdd*p->redMulti+p->redAdd);
	greenAdd=(int)(greenAdd*p->greenMulti+p->greenAdd);
	blueAdd=(int)(blueAdd*p->blueMulti+p->blueAdd);
	alphaAdd=(int)(alphaAdd*p->alphaMulti+p->alphaAdd);*/
}


void XXObjectColorTransform::GetString(XXVar&var)//XString8 &str)
{
	var.SetString("(",1);
    //str+=_XXKEY("(");
	XString8 tmp;
	for(int i=0;i<8;i++)
	{
		if(i)
			var.AddChar(',');
			//str+=_XXKEY(",");
		var.StringAdd(_strColorTransformIndex[i]);
		var.AddChar('=');
		//str+=_XXKEY("=");
		tmp.FromFloat(GetIndex(i));
		var.StringAdd(tmp);
		//str+=tmp;
	}
	var.AddChar(')');
	//str+=_XXKEY(")");
	//return var.GetLength();
}

float XXObjectColorTransform::GetIndex(int id)
{
   switch(id)
   {
   default:return 0;
   case 0://red Multi
	   return cxForm.redMultTerm/256.0F;
   case 1://green Multi
	   return cxForm.greenMultTerm/256.0F;	
   case 2:
	   return cxForm.blueMultTerm/256.0F;
   case 3:
	   return cxForm.alphaMultTerm/256.0F;
   case 4:
	   return (float)cxForm.redAddTerm;
   case 5:
	   return (float)cxForm.greenAddTerm;
   case 6:
	   return (float)cxForm.blueAddTerm;
   case 7:
	   return (float)cxForm.alphaAddTerm;
   }
}

XU32 XXObjectColorTransform::Construct(XSWFCONTEXT *pCnt, XXVARLIST &list, XXObject *)
{
	   XFDOUBLE v;
	   int nSize=list.GetSize();
	   switch(nSize)
	   {
	   case 8:
		    cxForm.alphaAddTerm=(int)(list[7].ToFloat());
	   case 7:
		    cxForm.blueAddTerm=(int)(list[6].ToFloat());
	   case 6:
		    cxForm.greenAddTerm=(int)(list[5].ToFloat());
	   case 5:
		    cxForm.redAddTerm=(int)(list[4].ToFloat());
//		    cxForm.hasAdd=XTRUE;
	   case 4:
		    v=list[3].ToFloat()*256;
		    cxForm.alphaMultTerm=XROUND(v);
	   case 3:
		    v=list[2].ToFloat()*256;
		    cxForm.blueMultTerm=XROUND(v);
	   case 2:
		    v=list[1].ToFloat()*256;
		    cxForm.greenMultTerm=XROUND(v);
	   case 1:
		    v=list[0].ToFloat()*256;
		    cxForm.redMultTerm=XROUND(v);
		    break;
	   }	   
	   cxForm.CalcFlags();
	   pCnt->pStack->Push(this);
	   return (XU32)this;
}


