#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NativeMenuItem.h"
namespace avmshell{
NativeMenuItemClass::NativeMenuItemClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeMenuItemClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeMenuItemObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeMenuItemObject::NativeMenuItemObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool NativeMenuItemObject::AS3_checked_get()
{
	return false;
}
void NativeMenuItemObject::AS3_checked_set(bool checked)
{

}
Atom NativeMenuItemObject::AS3_data_get()
{
	return kAvmThunkUndefined;
}
void NativeMenuItemObject::AS3_data_set(Atom pData)
{

}
bool NativeMenuItemObject::AS3_enabled_get()
{
	return false;
}
void NativeMenuItemObject::AS3_enabled_set(bool enabled)
{

}
bool NativeMenuItemObject::AS3_isSeparator_get()
{
	return false;
}
Stringp NativeMenuItemObject::AS3_keyEquivalent_get()
{
	return NULL;
}
void NativeMenuItemObject::AS3_keyEquivalent_set(Stringp pKeyEquivalent)
{

}
ArrayObject* NativeMenuItemObject::AS3_keyEquivalentModifiers_get()
{
	return NULL;
}
void NativeMenuItemObject::AS3_keyEquivalentModifiers_set(ArrayObject *pKeyEquivalentModifiers)
{

}
Stringp NativeMenuItemObject::AS3_label_get()
{
	return NULL;
}
void NativeMenuItemObject::AS3_label_set(Stringp pLabel)
{

}
NativeMenuObject* NativeMenuItemObject::AS3_menu_get()
{
	return NULL;
}
int NativeMenuItemObject::AS3_mnemonicIndex_get()
{
	return 0;
}
void NativeMenuItemObject::AS3_mnemonicIndex_set(int mnemonicIndex)
{

}
Stringp NativeMenuItemObject::AS3_name_get()
{
	return NULL;
}
void NativeMenuItemObject::AS3_name_set(Stringp pName)
{

}
NativeMenuObject* NativeMenuItemObject::AS3_submenu_get()
{
	return NULL;
}
void NativeMenuItemObject::AS3_submenu_set(NativeMenuObject *pSubmenu)
{

}

//void NativeMenuItemObject::AS3_constructor(Stringp pLabel, bool isSeparator)
//{
//
//}

NativeMenuItemObject* NativeMenuItemObject::AS3_clone()
{
	return NULL;
}
Stringp NativeMenuItemObject::AS3_toString()
{
	return NULL;
}

}