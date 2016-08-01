#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MediaPromise.h"
namespace avmshell{
MediaPromiseClass::MediaPromiseClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* MediaPromiseClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MediaPromiseObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MediaPromiseObject::MediaPromiseObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
FileObject* MediaPromiseObject::AS3_file_get()
{
	return NULL;
}
bool MediaPromiseObject::AS3_isAsync_get()
{
	return false;
}
Stringp MediaPromiseObject::AS3_mediaType_get()
{
	return NULL;
}
Stringp MediaPromiseObject::AS3_relativePath_get()
{
	return NULL;
}

void MediaPromiseObject::AS3_close()
{

}
AvmBox MediaPromiseObject::AS3_open(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return kAvmThunkUndefined;
}
void MediaPromiseObject::AS3_reportError(ErrorEventObject *pEvent)
{

}

}