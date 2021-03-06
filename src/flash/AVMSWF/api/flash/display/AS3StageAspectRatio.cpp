#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageAspectRatio.h"
namespace avmshell{
StageAspectRatioClass::StageAspectRatioClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StageAspectRatioClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageAspectRatioObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageAspectRatioObject::StageAspectRatioObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}