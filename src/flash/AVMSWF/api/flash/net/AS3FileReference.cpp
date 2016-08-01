#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FileReference.h"
namespace avmshell{
FileReferenceClass::FileReferenceClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())FileReferenceObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* FileReferenceClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FileReferenceObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FileReferenceObject* FileReferenceClass::CreateFileReference()
{
	FileReferenceObject *pObj = (FileReferenceObject *)createInstance(ivtable(), prototype);

	pObj->AS3_constructor();

	return pObj;
}

FileReferenceObject::FileReferenceObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
DateObject* FileReferenceObject::AS3_creationDate_get()
{
	//Add your act code here...
	return m_pCreationDate;
}

Stringp FileReferenceObject::AS3_creator_get()
{
	//Add your act code here...
	return m_creator;
}

DateObject* FileReferenceObject::AS3_modificationDate_get()
{
	//Add your act code here...
	return m_pModificationDate;
}

Stringp FileReferenceObject::AS3_name_get()
{
	//Add your act code here...
	return m_name;
}

uint32_t FileReferenceObject::AS3_size_get()
{
	//Add your act code here...
	return m_size;
}

Stringp FileReferenceObject::AS3_type_get()
{
	//Add your act code here...
	return m_type;
}

//AS3 contructor function..
void FileReferenceObject::AS3_constructor()
{
	//Add your act code here...
}

bool FileReferenceObject::AS3_browse(ArrayObject *pTypeFilter)
{
	//Add your act code here...
	return false;
}

void FileReferenceObject::AS3_cancel()
{
	//Add your act code here...
}

void FileReferenceObject::AS3_download(URLRequestObject *pRequest, Stringp defaultFileName)
{
	//Add your act code here...
	
}

void FileReferenceObject::AS3_upload(URLRequestObject *pRequest, Stringp uploadDataFieldName, bool testUpload)
{
	//Add your act code here...
}

//new adds
ByteArrayObject* FileReferenceObject::AS3_data_get()
{
	return NULL;
}
Stringp FileReferenceObject::AS3_extension_get()
{
	return NULL;
}

void FileReferenceObject::AS3_load()
{

}
void FileReferenceObject::AS3_save(Atom data, Stringp pDefaultFileName)
{

}
void FileReferenceObject::AS3_uploadUnencoded(URLRequestObject *pRequest)
{

}
//new adds end

}