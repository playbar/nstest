#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3PrintUIOptions.h"
namespace avmshell{
PrintUIOptionsClass::PrintUIOptionsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* PrintUIOptionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) PrintUIOptionsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
PrintUIOptionsObject::PrintUIOptionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool PrintUIOptionsObject::AS3_disablePageRange_get()
{
	return false;
}
void PrintUIOptionsObject::AS3_disablePageRange_set(bool disablePageRange)
{

}
uint32_t PrintUIOptionsObject::AS3_maxPage_get()
{
	return 0;
}
void PrintUIOptionsObject::AS3_maxPage_set(uint32_t maxPage)
{

}
uint32_t PrintUIOptionsObject::AS3_minPage_get()
{
	return 0;
}
void PrintUIOptionsObject::AS3_minPage_set(uint32_t minPage)
{

}

void PrintUIOptionsObject::AS3_constructor()
{

}

}