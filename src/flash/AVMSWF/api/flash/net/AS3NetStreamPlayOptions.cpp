#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetStreamPlayOptions.h"
namespace avmshell{
NetStreamPlayOptionsClass::NetStreamPlayOptionsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetStreamPlayOptionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetStreamPlayOptionsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetStreamPlayOptionsObject::NetStreamPlayOptionsObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double NetStreamPlayOptionsObject::AS3_len_get()
//{
//	return 0;
//}
//void NetStreamPlayOptionsObject::AS3_len_set(double len)
//{
//
//}
//double NetStreamPlayOptionsObject::AS3_offset_get()
//{
//	return 0;
//}
//void NetStreamPlayOptionsObject::AS3_offset_set(double offset)
//{
//
//}
//Stringp NetStreamPlayOptionsObject::AS3_oldStreamName_get()
//{
//	return NULL;
//}
//void NetStreamPlayOptionsObject::AS3_oldStreamName_set(Stringp pOldStreamName)
//{
//
//}
//double NetStreamPlayOptionsObject::AS3_start_get()
//{
//	return 0;
//}
//void NetStreamPlayOptionsObject::AS3_start_set(double start)
//{
//
//}
//Stringp NetStreamPlayOptionsObject::AS3_streamName_get()
//{
//	return NULL;
//}
//void NetStreamPlayOptionsObject::AS3_streamName_set(Stringp pStreamName)
//{
//
//}
//Stringp NetStreamPlayOptionsObject::AS3_transition_get()
//{
//	return NULL;
//}
//void NetStreamPlayOptionsObject::AS3_transition_set(Stringp pTransition)
//{
//
//}
//
//void NetStreamPlayOptionsObject::AS3_constructor()
//{
//
//}
}