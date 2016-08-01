#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ContextMenuClipboardItems.h"
namespace avmshell{
ContextMenuClipboardItemsClass::ContextMenuClipboardItemsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ContextMenuClipboardItemsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ContextMenuClipboardItemsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ContextMenuClipboardItemsObject* ContextMenuClipboardItemsClass::CreateObject()
{
	return (ContextMenuClipboardItemsObject*)createInstance(this->ivtable(),prototype);
}

ContextMenuClipboardItemsObject::ContextMenuClipboardItemsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//bool ContextMenuClipboardItemsObject::AS3_clear_get()
//{
//	return false;
//}
//void ContextMenuClipboardItemsObject::AS3_clear_set(bool clear)
//{
//
//}
//bool ContextMenuClipboardItemsObject::AS3_copy_get()
//{
//	return false;
//}
//void ContextMenuClipboardItemsObject::AS3_copy_set(bool copy)
//{
//
//}
//bool ContextMenuClipboardItemsObject::AS3_cut_get()
//{
//	return false;
//}
//void ContextMenuClipboardItemsObject::AS3_cut_set(bool cut)
//{
//
//}
//bool ContextMenuClipboardItemsObject::AS3_paste_get()
//{
//	return false;
//}
//void ContextMenuClipboardItemsObject::AS3_paste_set(bool paste)
//{
//
//}
//bool ContextMenuClipboardItemsObject::AS3_selectAll_get()
//{
//	return false;
//}
//void ContextMenuClipboardItemsObject::AS3_selectAll_set(bool selectAll)
//{
//
//}
//
//void ContextMenuClipboardItemsObject::AS3_constructor()
//{
//
//}

}