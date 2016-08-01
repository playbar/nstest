#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FileFilter.h"
namespace avmshell{
FileFilterClass::FileFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	prototype = new (core()->GetGC(), cvtable->getExtraSize())FileFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* FileFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FileFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FileFilterObject* FileFilterClass::CreateFileFilterObject(Stringp description, Stringp extension, Stringp macType)
{
	FileFilterObject *pObj = (FileFilterObject *)createInstance(ivtable(), prototype);

	pObj->AS3_constructor(description, extension, macType);

	return pObj;
}

FileFilterObject::FileFilterObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp FileFilterObject::AS3_description_get()
{
	//Add your act code here...
	return m_description;
}

void FileFilterObject::AS3_description_set(Stringp description)
{
	//Add your act code here...
	m_description = description;
}

Stringp FileFilterObject::AS3_extension_get()
{
	//Add your act code here...
	return m_extension;
}

void FileFilterObject::AS3_extension_set(Stringp extension)
{
	//Add your act code here...
	m_extension = extension;
}

Stringp FileFilterObject::AS3_macType_get()
{
	//Add your act code here...
	return m_macType;
}

void FileFilterObject::AS3_macType_set(Stringp macType)
{
	//Add your act code here...
	m_macType = macType;
}

//AS3 contructor function..
void FileFilterObject::AS3_constructor(Stringp description, Stringp extension, Stringp macType)
{
	//Add your act code here...
	m_description = description;
	m_extension = extension;
	m_macType = macType;
}

}