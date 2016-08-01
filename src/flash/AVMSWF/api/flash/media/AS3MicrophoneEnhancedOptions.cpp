#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MicrophoneEnhancedOptions.h"
namespace avmshell{
MicrophoneEnhancedOptionsClass::MicrophoneEnhancedOptionsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* MicrophoneEnhancedOptionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MicrophoneEnhancedOptionsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MicrophoneEnhancedOptionsObject::MicrophoneEnhancedOptionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//int MicrophoneEnhancedOptionsObject::AS3_echoPath_get()
//{
//	return 0;
//}
//void MicrophoneEnhancedOptionsObject::AS3_echoPath_set(int echoPath)
//{
//
//}
//int MicrophoneEnhancedOptionsObject::AS3_isVoiceDetected_get()
//{
//	return 0;
//}
//Stringp MicrophoneEnhancedOptionsObject::AS3_mode_get()
//{
//	return NULL;
//}
//void MicrophoneEnhancedOptionsObject::AS3_mode_set(Stringp pMode)
//{
//
//}
//bool MicrophoneEnhancedOptionsObject::AS3_nonLinearProcessing_get()
//{
//	return false;
//}
//void MicrophoneEnhancedOptionsObject::AS3_nonLinearProcessing_set(bool pNonLinearProcessing)
//{
//
//}

}