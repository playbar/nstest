#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3AVDictionaryDataEvent.h"
namespace avmshell{
AVDictionaryDataEventClass::AVDictionaryDataEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* AVDictionaryDataEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) AVDictionaryDataEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
AVDictionaryDataEventObject::AVDictionaryDataEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...


}