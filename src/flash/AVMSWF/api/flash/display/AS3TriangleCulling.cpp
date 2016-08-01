#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TriangleCulling.h"
namespace avmshell{
TriangleCullingClass::TriangleCullingClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TriangleCullingClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TriangleCullingObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TriangleCullingObject::TriangleCullingObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}