#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SoundCodec.h"
namespace avmshell{
SoundCodecClass::SoundCodecClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SoundCodecClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SoundCodecObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SoundCodecObject::SoundCodecObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}