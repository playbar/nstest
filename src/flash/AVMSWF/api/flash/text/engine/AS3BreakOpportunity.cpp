#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3BreakOpportunity.h"
namespace avmshell{
BreakOpportunityClass::BreakOpportunityClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* BreakOpportunityClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BreakOpportunityObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

int BreakOpportunityClass::GetBreakMode(Stringp strMode)
{
	if(strMode==NULL)
		return BREAK_AUTO;
	else if(strMode->equals(m_slots_BreakOpportunityClass.ALL))
		return BREAK_ALL;
	else if(strMode->equals(m_slots_BreakOpportunityClass.NONE))
		return BREAK_NONE;
	else if(strMode->equals(m_slots_BreakOpportunityClass.ANY))
		return BREAK_ANY;
	return BREAK_AUTO;
}
Stringp BreakOpportunityClass::GetBreakName(int nMode)
{
	switch(nMode)
	{
	default:
	case BREAK_AUTO:
		 return m_slots_BreakOpportunityClass.AUTO;
		 break;
	case BREAK_NONE:
		 return m_slots_BreakOpportunityClass.NONE;
		 break;
	case BREAK_ANY:
		 return m_slots_BreakOpportunityClass.ANY;
		 break;
	case BREAK_ALL:
		 return m_slots_BreakOpportunityClass.ALL;
		 break;
	}
}

BreakOpportunityObject::BreakOpportunityObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
