#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ProductManager.h"
namespace avmshell{
ProductManagerClass::ProductManagerClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ProductManagerClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ProductManagerObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ProductManagerObject::ProductManagerObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void ProductManagerObject::AS3_constructor(Stringp name, bool shared)
{
	//Add your act code here...
}

bool ProductManagerObject::AS3_running_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

bool ProductManagerObject::AS3_installed_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

bool ProductManagerObject::AS3_launch(Stringp parameters)
{
	//Add your act code here...
	return false;//Modify this please!
}

Stringp ProductManagerObject::AS3_installedVersion_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

bool ProductManagerObject::AS3_doSelfUpgrade(Stringp os)
{
	//Add your act code here...
	return false;//Modify this please!
}

}
