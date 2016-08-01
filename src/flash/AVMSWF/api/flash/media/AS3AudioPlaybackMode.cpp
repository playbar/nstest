#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3AudioPlaybackMode.h"
namespace avmshell
{
	AudioPlaybackModeClass::AudioPlaybackModeClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* AudioPlaybackModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) AudioPlaybackModeObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	AudioPlaybackModeObject::AudioPlaybackModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}