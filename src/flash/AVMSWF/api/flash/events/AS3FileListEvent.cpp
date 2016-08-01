#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FileListEvent.h"
namespace avmshell{
FileListEventClass::FileListEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* FileListEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FileListEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FileListEventObject::FileListEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* FileListEventObject::AS3_files_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void FileListEventObject::AS3_files_set(ArrayObject *pFiles)
{
	//Add your act code here...
}

//AS3 contructor function..
void FileListEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, ArrayObject *pFiles)
{
	//Add your act code here...
}

}