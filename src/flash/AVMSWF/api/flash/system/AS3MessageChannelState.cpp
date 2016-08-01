#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MessageChannelState.h"
namespace avmshell
{
	MessageChannelStateClass::MessageChannelStateClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* MessageChannelStateClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) MessageChannelStateObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	MessageChannelStateObject::MessageChannelStateObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}