#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ColorCorrection.h"
namespace avmshell{
ColorCorrectionClass::ColorCorrectionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ColorCorrectionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ColorCorrectionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ColorCorrectionObject::ColorCorrectionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
