// ObjectArgs.cpp: implementation of the ObjectArgs class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectArgs.h"
#include "ObjectCreator.h"
#include "splayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define XXARGSCLASS(TOKEN) XXCLASS(TOKEN,XXObjectArgs,XXOBJ_ARGUMENT)


XBOOL XXObjectArgs::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
	case _SYSID(callee):
		 var=m_pCallee;
		 return XTRUE;
	case _SYSID(caller):
		 var.ToConst(XOCT_UNDEFINE);
		 return XTRUE;
	}
	return XXObjectArray::SysGetMember(id,pCnt,var);
}

XXObjectArgs::XXObjectArgs(ScriptPlayer*pRoot,XXObject*pSuper,XU8 bInit):
			  XXObjectArray(pRoot)
{

	m_pCallee=XNULL;
	SetSuper(pSuper,bInit);
	//m_nRefs=2;
	if(bInit)
	{
		_SYSCALL(callee);
		_SYSCALL(caller);
		XXObjectArray::InitObject();
		/*AddProperty(XXVar(_strLength),XXFUNOBJ(arrayLenGet),XXFUNOBJ(arrayLenSet));
		//AddMember(XXVar(_strCASEINSENSITIVE,XXVar(1));
		//AddMember(XXVar(_strDESCENDING,XXVar(2));
		//AddMember(XXVar(_strUNIQUESORT,XXVar(4));
		//AddMember(XXVar(_strRETURNINDEXEDARRAY,XXVar(8));
		//AddMember(XXVar(_strNUMERIC,XXVar(16));
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
		//AddMember(XXVar(_strConstructor,XXVar(this));
		//AddProperty(XXVar(_strLength,XXFUNOBJ(argLenGet),XNULL);
		//XXObjectArray::InitObject();
		AddProperty(XXVar(_strCallee),XXFUNOBJ(calleeGet),XNULL);
		AddProperty(XXVar(_strCaller),XXFUNOBJ(callerGet),XNULL);*/
	}
}


