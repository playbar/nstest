#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FileReferenceList.h"
namespace avmshell{
FileReferenceListClass::FileReferenceListClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())FileReferenceListObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* FileReferenceListClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FileReferenceListObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FileReferenceListObject* FileReferenceListClass::CreateFileReferenceList()
{
	FileReferenceListObject *pObj = (FileReferenceListObject *)createInstance(ivtable(), prototype);

	pObj->AS3_constructor();

	return pObj;
}

FileReferenceListObject::FileReferenceListObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* FileReferenceListObject::AS3_fileList_get()
{
	//Add your act code here...
	return m_pFileList;
}

//AS3 contructor function..
void FileReferenceListObject::AS3_constructor()
{
	//Add your act code here...
}

bool FileReferenceListObject::AS3_browse(ArrayObject *pTypeFilter)
{
	//Add your act code here...
	return false;
}

}