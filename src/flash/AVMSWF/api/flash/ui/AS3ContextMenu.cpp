#include "StdAfxflash.h"
#include "ShellCore.h"
#include "avm2.h"

using namespace avmplus;
#include "AS3ContextMenuBuiltInItems.h"
#include "AS3ContextMenu.h"
#include "AS3ContextMenuEvent.h"

namespace avmshell{
ContextMenuClass::ContextMenuClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ContextMenuObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ContextMenuClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ContextMenuObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ContextMenuObject::ContextMenuObject(VTable *vtable, ScriptObject* proto, int capacity): NativeMenuObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pBuiltInItems=NULL;
	m_pCustoms=NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
ContextMenuBuiltInItemsObject* ContextMenuObject::AS3_builtInItems_get()
{
	//Add your act code here...
	return m_pBuiltInItems;
	//return kAvmThunkUndefined;//Modify this please!
}

void ContextMenuObject::AS3_builtInItems_set(ContextMenuBuiltInItemsObject*p)
{
	//Add your act code here...
	m_pBuiltInItems=p;
	//return kAvmThunkUndefined;//Modify this please!
}

ArrayObject* ContextMenuObject::AS3_customItems_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	return m_pCustoms;
}

void ContextMenuObject::AS3_customItems_set(ArrayObject*p)
{
	//Add your act code here...
	m_pCustoms=p;
}

//AS3 contructor function..
void ContextMenuObject::AS3_constructor()
{
	//Add your act code here...
	ShellToplevel*t=(ShellToplevel*)toplevel();
	m_pCustoms=t->arrayClass->newArray(0);
	m_pBuiltInItems=t->getContextMenuBuiltInItemsClass()->CreateItems();
	//return kAvmThunkUndefined;//Modify this please!
}

ContextMenuObject* ContextMenuObject::AS3_clone()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	ShellToplevel*t=(ShellToplevel*)toplevel();
	ContextMenuClass*pClass=t->getContextMenuClass();
	ContextMenuObject*pObj=(ContextMenuObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
	pObj->m_pBuiltInItems=t->getContextMenuBuiltInItemsClass()->CreateItems();
	if(m_pCustoms)
	{
		int i,nCount=m_pCustoms->getDenseLength();//get_length();
		pObj->m_pCustoms=t->arrayClass->newArray(nCount);
		for(i=0;i<nCount;i++)
		{
			Atom atom=m_pCustoms->getIntProperty(i);
			pObj->m_pCustoms->setIntProperty(i,atom);//=t->arrayClass->newArray(
		}
		pObj->m_pBuiltInItems->SetData(m_pBuiltInItems);
			//t->arrayClass->newarray(m_pCustoms->
	}
	else
		pObj->m_pCustoms=t->arrayClass->newArray(0);
	return pObj;
	//return kAvmThunkUndefined;//Modify this please!
}

void ContextMenuObject::AS3_hideBuiltInItems()
{
	//Add your act code here...
	if(m_pBuiltInItems)
	{
		m_pBuiltInItems->ResetData();
	}
	//return kAvmThunkUndefined;//Modify this please!
}

/*int ContextMenuObject::MapSystemEvent(Stringp strName,int&nMax)
{
	nMax=XAS3_EVENT_MAX;
	ContextMenuEventClass*pClass=((ShellToplevel*)toplevel())->getContextMenuEventClass();
	if(strName==pClass->getSlotMENU_SELECT())
		return XAS3_EVENT_menuSelect;
	return EventDispatcherObject::MapSystemEvent(strName,nMax);
	//return idd;
}*/

//new adds
ContextMenuClipboardItemsObject* ContextMenuObject::AS3_clipboardItems_get()
{
	if(m_pClipboardItems==NULL)
	{
		ContextMenuClipboardItemsClass*pClass=((ShellToplevel*)toplevel())->getContextMenuClipboardItemsClass();
												//getContextMenuBuiltInItemsClass();
		m_pClipboardItems=pClass->CreateObject();
	}
	return m_pClipboardItems;
}
void ContextMenuObject::AS3_clipboardItems_set(ContextMenuClipboardItemsObject *pClipboardItems)
{
	m_pClipboardItems = pClipboardItems;
}
bool ContextMenuObject::AS3_clipboardMenu_get()
{
	
	return m_clipboardMenu;
}
void ContextMenuObject::AS3_clipboardMenu_set(bool clipboardMenu)
{
	m_clipboardMenu = clipboardMenu;
}
//bool ContextMenuObject::AS3_isSupported_get()
//{
//	return false;
//}
ArrayObject* ContextMenuObject::AS3_items_get()
{
	return m_pItems;
}
void ContextMenuObject::AS3_items_set(ArrayObject *items)
{
	m_pItems = items;
}
URLRequestObject* ContextMenuObject::AS3_link_get()
{
	return m_pLink;
}
void ContextMenuObject::AS3_link_set(URLRequestObject *pLink)
{
	m_pLink = pLink;
}
int ContextMenuObject::AS3_numItems_get()
{
	return 0;
}
void ContextMenuObject::AS3_numItems_set(int numItems)
{

}

NativeMenuItemObject* ContextMenuObject::AS3_addItemAt(NativeMenuItemObject *pItem, int index)
{
	return NULL;
}
bool ContextMenuObject::AS3_containsItem(NativeMenuItemObject *pItem)
{
	return false;
}
void ContextMenuObject::AS3_display(StageObject *pStage, double stageX, double stageY)
{
	
}
NativeMenuItemObject* ContextMenuObject::AS3_getItemAt(int index)
{
	return NULL;
}
int ContextMenuObject::AS3_getItemIndex(NativeMenuItemObject *pItem)
{
	return 0;
}
void ContextMenuObject::AS3_removeAllItems()
{

}
NativeMenuItemObject* ContextMenuObject::AS3_removeItemAt(int index)
{
	return NULL;
}
//new adds end

}