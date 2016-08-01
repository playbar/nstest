#include "StdAfxflash.h"
#include "sobject.h"
#include "splay.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Private.h"
namespace avmshell
{
DomainClass::DomainClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BitmapObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* DomainClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DomainObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}


DomainObject::DomainObject(VTable* _vtable, ScriptObject* proto, int capacity):ScriptObject(vtable, proto, 0)
{
	m_pDomain=NULL;
}

void DomainObject::AS3_constructor()
{
	m_pDomain=toplevel()->domainEnv()->domain();
}

void DomainObject::AS3_domainMemory_set(ByteArrayObject*)
{
}
ByteArrayObject* DomainObject::AS3_domainMemory_get()
{
	return NULL;
}

}
