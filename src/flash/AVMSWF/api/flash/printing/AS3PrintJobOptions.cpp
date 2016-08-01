#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3PrintJobOptions.h"
namespace avmshell{
PrintJobOptionsClass::PrintJobOptionsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())PrintJobOptionsObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* PrintJobOptionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) PrintJobOptionsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
PrintJobOptionsObject::PrintJobOptionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_bAsBitmap=false;
}
//////////////////////////////////////////////////////////
//Native Method start...

//new adds
double PrintJobOptionsObject::AS3_pixelsPerInch_get()
{
	return 0;
}
void PrintJobOptionsObject::AS3_pixelsPerInch_set(double pixelsPerInch)
{
}
Stringp PrintJobOptionsObject::AS3_printMethod_get()
{
	return NULL;
}
void PrintJobOptionsObject::AS3_printMethod_set(Stringp pPrintMethod)
{
}
//new adds end

}