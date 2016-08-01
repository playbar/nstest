#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NativeDragEvent.h"
namespace avmshell{
NativeDragEventClass::NativeDragEventClass(VTable* cvtable):ClassClosure(cvtable)//MouseEventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeDragEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeDragEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeDragEventObject::NativeDragEventObject(VTable *vtable, ScriptObject* proto, int capacity): MouseEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
NativeDragOptionsObject* NativeDragEventObject::AS3_allowedActions_get()
{
	return NULL;
}
void NativeDragEventObject::AS3_allowedActions_set(NativeDragOptionsObject *pAllowedActions)
{

}
ClipboardObject* NativeDragEventObject::AS3_clipboard_get()
{
	return NULL;
}
void NativeDragEventObject::AS3_clipboard_set(ClipboardObject *pClipboard)
{

}
Stringp NativeDragEventObject::AS3_dropAction_get()
{
	return NULL;
}
void NativeDragEventObject::AS3_dropAction_set(Stringp pDropAction)
{

}

void NativeDragEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
											double localX, double localY, InteractiveObjectObject *pRelatedObject, 
											ClipboardObject *pClipboard, NativeDragOptionsObject *pAllowedActions, Stringp pDropAction, 
											bool controlKey, bool altKey, bool shiftKey, bool commandKey)
{

}
EventObject* NativeDragEventObject::AS3_clone()
{
	return NULL;
}
Stringp NativeDragEventObject::AS3_toString()
{
	return NULL;
}

}