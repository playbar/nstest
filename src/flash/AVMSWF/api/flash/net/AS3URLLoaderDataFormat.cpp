#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3URLLoaderDataFormat.h"
namespace avmshell{
URLLoaderDataFormatClass::URLLoaderDataFormatClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())URLLoaderDataFormatObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* URLLoaderDataFormatClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) URLLoaderDataFormatObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

URLLoaderDataFormatObject *URLLoaderDataFormatClass::CreateURLLoaderDataFormat()
{
	URLLoaderDataFormatObject *pObj = (URLLoaderDataFormatObject *)createInstance(ivtable(), prototype);

	return pObj;
}

URLLoaderDataFormatObject::URLLoaderDataFormatObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}