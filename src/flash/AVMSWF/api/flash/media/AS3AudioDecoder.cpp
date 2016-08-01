#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3AudioDecoder.h"
namespace avmshell
{
	AudioDecoderClass::AudioDecoderClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* AudioDecoderClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) AudioDecoderObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	AudioDecoderObject::AudioDecoderObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}