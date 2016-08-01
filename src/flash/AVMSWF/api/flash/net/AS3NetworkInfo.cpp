#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetworkInfo.h"
namespace avmshell{
	NetworkInfoClass::NetworkInfoClass(VTable* cvtable):ClassClosure(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* NetworkInfoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) NetworkInfoObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	NetworkInfoObject::NetworkInfoObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////
	//Native Method start...
	bool NetworkInfoObject::AS3_isSupported_get()
	{
		//Add your act code here...
		return false;//Modify this please!
	}

	NetworkInfoObject* NetworkInfoObject::AS3_networkInfo_get()
	{
		//Add your act code here...
		return NULL;//Modify this please!
	}

	AvmBox NetworkInfoObject::AS3_findInterfaces(AvmMethodEnv, uint32_t, AvmBox* argv)
	{
		//Add your act code here...
		return kAvmThunkUndefined;//Modify this please!
	}

}