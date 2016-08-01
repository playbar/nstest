#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Multitouch.h"
namespace avmshell{
MultitouchClass::MultitouchClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* MultitouchClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MultitouchObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MultitouchObject::MultitouchObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp MultitouchClass::AS3_inputMode_get()
{
	return NULL;
}
void MultitouchClass::AS3_inputMode_set(Stringp pInputMode)
{

}
int MultitouchClass::AS3_maxTouchPoints_get()
{
	return 0;
}
ObjectVectorObject* MultitouchClass::AS3_supportedGestures_get()
{
	return NULL;
}
bool MultitouchClass::AS3_supportsGestureEvents_get()
{
	return false;
}
bool MultitouchClass::AS3_supportsTouchEvents_get()
{
	return false;
}

}