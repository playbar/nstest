#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Orientation3D.h"
namespace avmshell{
Orientation3DClass::Orientation3DClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* Orientation3DClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) Orientation3DObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
Orientation3DObject::Orientation3DObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}