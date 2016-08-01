#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3H264Profile.h"
namespace avmshell
{
	H264ProfileClass::H264ProfileClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* H264ProfileClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) H264ProfileObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	H264ProfileObject::H264ProfileObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}