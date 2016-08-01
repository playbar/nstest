#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3PaperSize.h"
namespace avmshell{
PaperSizeClass::PaperSizeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* PaperSizeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) PaperSizeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
PaperSizeObject::PaperSizeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}