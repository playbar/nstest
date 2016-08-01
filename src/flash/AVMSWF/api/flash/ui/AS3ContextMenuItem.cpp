#include "StdAfxflash.h"
#include "ShellCore.h"
#include "avm2.h"
#include "splayer.h"

using namespace avmplus;
#include "AS3ContextMenuItem.h"
#include "AS3ContextMenuEvent.h"
namespace avmshell{
ContextMenuItemClass::ContextMenuItemClass(VTable* cvtable):ClassClosure(cvtable)
{
	//createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ContextMenuItemObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ContextMenuItemClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ContextMenuItemObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ContextMenuItemObject::ContextMenuItemObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	mbVisible = true;
	//Add your construct code here...
}

/*int ContextMenuItemObject::MapSystemEvent(Stringp strName,int&nMax)
{
	nMax=XAS3_EVENT_MAX;
	ContextMenuEventClass*pClass=((ShellToplevel*)toplevel())->getContextMenuEventClass();
	if(strName==pClass->getSlotMENU_ITEM_SELECT())
		return XAS3_EVENT_menuItemSelect;
	return EventDispatcherObject::MapSystemEvent(strName,nMax);
}*/


//new adds
ContextMenuItemObject* ContextMenuItemClass::AS3_systemClearMenuItem()
{
	return NULL;
}
ContextMenuItemObject* ContextMenuItemClass::AS3_systemCopyLinkMenuItem()
{
	return NULL;
}
ContextMenuItemObject* ContextMenuItemClass::AS3_systemCopyMenuItem()
{
	return NULL;
}
ContextMenuItemObject* ContextMenuItemClass::AS3_systemCutMenuItem()
{
	return NULL;
}
ContextMenuItemObject* ContextMenuItemClass::AS3_systemOpenLinkMenuItem()
{
	return NULL;
}
ContextMenuItemObject* ContextMenuItemClass::AS3_systemPasteMenuItem()
{
	return NULL;
}
ContextMenuItemObject* ContextMenuItemClass::AS3_systemSelectAllMenuItem()
{
	return NULL;
}
//new adds end

bool ContextMenuItemObject::AS3_visible_get()
{
	return mbVisible;
}

void ContextMenuItemObject::AS3_visible_set( bool b )
{
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	player->pWindow->PostMsg(XWM_XSHOWHIDEMENU, b, 0 );
	mbVisible = b;
}

}