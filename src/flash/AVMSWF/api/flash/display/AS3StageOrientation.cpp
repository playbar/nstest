#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageOrientation.h"
namespace avmshell{
StageOrientationClass::StageOrientationClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StageOrientationClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageOrientationObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageOrientationObject::StageOrientationObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}