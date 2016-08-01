#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3LoaderContext.h"
namespace avmshell{
LoaderContextClass::LoaderContextClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())LoaderContextObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* LoaderContextClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LoaderContextObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
LoaderContextObject::LoaderContextObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	
}

//new adds
bool LoaderContextObject::AS3_allowCodeImport_get()
{
	return false;
}
void LoaderContextObject::AS3_allowCodeImport_set(bool allowCodeImport)
{

}
bool LoaderContextObject::AS3_allowLoadBytesCodeExecution_get()
{
	return false;
}
void LoaderContextObject::AS3_allowLoadBytesCodeExecution_set(bool allowLoadBytesCodeExecution)
{

}
Stringp LoaderContextObject::AS3_imageDecodingPolicy_get()
{
	return NULL;
}
void LoaderContextObject::AS3_imageDecodingPolicy_set(Stringp pImageDecodingPolicy)
{

}
Atom LoaderContextObject::AS3_parameters_get()
{
	return kAvmThunkUndefined;
}
void LoaderContextObject::AS3_parameters_set(Atom parameters)
{

}
DisplayObjectContainerObject* LoaderContextObject::AS3_requestedContentParent_get()
{
	return NULL;
}
void LoaderContextObject::AS3_requestedContentParent_set(DisplayObjectContainerObject *pRequestedContentParent)
{

}
//new adds end

}