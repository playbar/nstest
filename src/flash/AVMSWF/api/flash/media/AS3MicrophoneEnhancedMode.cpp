#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MicrophoneEnhancedMode.h"
namespace avmshell{
MicrophoneEnhancedModeClass::MicrophoneEnhancedModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* MicrophoneEnhancedModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MicrophoneEnhancedModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MicrophoneEnhancedModeObject::MicrophoneEnhancedModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}