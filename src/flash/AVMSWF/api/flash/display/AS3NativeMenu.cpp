#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NativeMenu.h"
namespace avmshell{
NativeMenuClass::NativeMenuClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeMenuClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeMenuObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeMenuObject::NativeMenuObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//bool NativeMenuObject::AS3_isSupported_get()
//{
//	return false;
//}
//ArrayObject* NativeMenuObject::AS3_items_get()
//{
//	return NULL;
//}
//void NativeMenuObject::AS3_items_set(ArrayObject *pItems)
//{
//	
//}
//int NativeMenuObject::AS3_numItems_get()
//{
//	return 0;
//}
//NativeMenuObject* NativeMenuObject::AS3_parent_get()
//{
//	return NULL;
//}
//
//void NativeMenuObject::AS3_constructor()
//{
//
//}
//NativeMenuItemObject* NativeMenuObject::AS3_addItem(NativeMenuItemObject *pItem)
//{
//	return NULL;
//}
//NativeMenuItemObject* NativeMenuObject::AS3_addItemAt(NativeMenuItemObject *pItem, int index)
//{
//	return NULL;
//}
//NativeMenuItemObject* NativeMenuObject::AS3_addSubmenu(NativeMenuObject *pSubmenu, Stringp pLabel)
//{
//	return NULL;
//}
//NativeMenuItemObject* NativeMenuObject::AS3_addSubmenuAt(NativeMenuObject *pSubmenu, int index, Stringp pLabel)
//{
//	return NULL;
//}
//NativeMenuObject* NativeMenuObject::AS3_clone()
//{
//	return NULL;
//}
//bool NativeMenuObject::AS3_containsItem(NativeMenuItemObject *pItem)
//{
//	return false;
//}
//void NativeMenuObject::AS3_display(StageObject *pStage, double stageX, double stageY)
//{
//
//}
//NativeMenuItemObject* NativeMenuObject::AS3_getItemAt(int index)
//{
//	return NULL;
//}
//NativeMenuItemObject* NativeMenuObject::AS3_getItemByName(Stringp pName)
//{
//	return NULL;
//}
//int NativeMenuObject::AS3_getItemIndex(NativeMenuItemObject *pItem)
//{
//	return 0;
//}
//void NativeMenuObject::AS3_removeAllItems()
//{
//
//}
//NativeMenuItemObject* NativeMenuObject::AS3_removeItem(NativeMenuItemObject *pItem)
//{
//	return NULL;
//}
//NativeMenuItemObject* NativeMenuObject::AS3_removeItemAt(int index)
//{
//	return NULL;
//}
//void NativeMenuObject::AS3_setItemIndex(NativeMenuItemObject *pItem, int index)
//{
//
//}

}