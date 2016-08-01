#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3VideoCodec.h"
namespace avmshell
{
	VideoCodecClass::VideoCodecClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* VideoCodecClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) VideoCodecObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	VideoCodecObject::VideoCodecObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}