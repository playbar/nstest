#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ImageDecodingPolicy.h"
namespace avmshell{
ImageDecodingPolicyClass::ImageDecodingPolicyClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ImageDecodingPolicyClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ImageDecodingPolicyObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ImageDecodingPolicyObject::ImageDecodingPolicyObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}