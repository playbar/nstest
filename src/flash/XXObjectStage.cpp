// XXObjectStage.cpp: implementation of the XXObjectStage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectStage.h"
#include "splayer.h"
#include "ObjectKey.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectStage::XXObjectStage(ScriptPlayer*pRoot):XXObject(pRoot)
{

}

XXObjectStage::~XXObjectStage()
{

}

XXObject* XXObjectStage::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
	XXObjectStage*p=CreateClass(/*m_bBase?this:m_pSuper*/this,XFALSE,m_pRoot);
	if(p)
		p->m_pSuperObj=GetSuperObject();
	pCnt->pStack->Push((p));
	return p;
}


/////////////////////////////////////////////////////////////////////
//#define XXSTAGE(TOKEN) XXCLASS(TOKEN,XXObjectStage,XXOBJ_STAGE)
XBOOL XXObjectStage::SysGetMember(int id, XSWFCONTEXT *pCnt,XXVar &var)
{
	switch(id)
	{
	   case _SYSID(align):
		    var=((_strAligns[pCnt->pPlayer->m_align]));
			return XTRUE;
	   case _SYSID(width):
		    var=((pCnt->pPlayer->GetStageWidth()));
			return XTRUE;
	   case _SYSID(height):
		    var=((pCnt->pPlayer->GetStageHeight()));
			return XTRUE;
	   case _SYSID(addListener):
		    //var=(m_pStageFun[XFUNS_ADDLISTENER]);
			//return XTRUE;
	   case _SYSID(removeListener):
		    //var=(m_pStageFun[XFUNS_REMOVELISTENER]);
			//return XTRUE;
		    var=this;
			var.ToFunction(id);
			return XTRUE;
	   case _SYSID(showMenu):
		    var=(pCnt->pPlayer->menuShow);
			return XTRUE;
	   case _SYSID(scaleMode):
		    var=(pCnt->pPlayer->GetScaleMode());
		    return XTRUE;
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectStage::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	switch(id)
	{
	   case _SYSID(addListener):
		    if(list.GetSize()>0&&list[0].IsObject(XXOBJ_OBJECT))
				XXObject::AddListener(pCnt->baseThread,list[0].pObject);
			pCnt->pStack->PushBool(XTRUE);
			return XTRUE;
	   case _SYSID(removeListener):
		    if(list.GetSize()>0&&list[0].IsObject())
				XXObject::RemoveListener(list[0].pObject);
			pCnt->pStack->PushBool(XTRUE);
			return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}
XBOOL XXObjectStage::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	   case _SYSID(align):
		    //alignSet(*pCnt,var);		    
			{
				var.ToString(XFALSE);
				switch(var.strTxt[0])
				{
				default:
				case 'l':
				case 'L':
					pCnt->pPlayer->m_align=stextAlignLeft;break;
				case 'r':
				case 'R':
					pCnt->pPlayer->m_align=stextAlignRight;break;
				case 'c':
				case 'C':
					pCnt->pPlayer->m_align=stextAlignCenter;break;
				case 'j':
				case 'J':
					pCnt->pPlayer->m_align=stextAlignJustify;break;
				}
			}
		    return XTRUE;
	   case _SYSID(showMenu):
		    pCnt->pPlayer->ShowMenu(var.ToLogic());
		    return XTRUE;
	   case _SYSID(scaleMode):
		    var.ToString(XFALSE);
		    pCnt->pPlayer->SetScaleMode(var.strTxt);
			return XTRUE;
	}
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectStage::InitObject()
{
	_SYSCALL(align);
	_SYSCALL(width);
	_SYSCALL(height);
	_SYSCALL(showMenu);
	_SYSCALL(scaleMode);
	_SYSCALL(addListener);
	_SYSCALL(removeListener);
	/*AddProperty(XXVar(_strAlign),XXFUNOBJ(alignGet),XXFUNOBJ(alignSet));
	AddProperty(XXVar(_strWidthn),XXFUNOBJ(widthGetStage),XNULL);
	AddProperty(XXVar(_strHeightn),XXFUNOBJ(heightGetStage),XNULL);
	AddProperty(XXVar(_strShowMenu),XXFUNOBJ(showMenuGet),XXFUNOBJ(showMenuSet));
	AddProperty(XXVar(_strScaleMode),XXFUNOBJ(scaleModeGet),XNULL);

	AddMember(XXVar(_strAddListener),XXVar(XXFUNOBJ(addListenerStage)));
	AddMember(XXVar(_strRemoveListener),XXVar(XXFUNOBJ(removeListenerStage)));*/
}

