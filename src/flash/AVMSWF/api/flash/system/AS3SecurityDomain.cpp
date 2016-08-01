#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3SecurityDomain.h"
namespace avmshell{
SecurityDomainClass::SecurityDomainClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SecurityDomainObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SecurityDomainClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SecurityDomainObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SecurityDomainObject::SecurityDomainObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
SecurityDomainObject* SecurityDomainClass::AS3_currentDomain_get()
{
	//Add your act code here...
	//ShellCodeContext*p=((ShellCore*)core())->GetCodeContext();
		//(ShellCodeContext*)core()->codeContext();
	ShellCodeContext*p=(ShellCodeContext*)core()->codeContext();
	if(p)
	{
		return p->m_pSecurityDomain;
	}
	return NULL;
	//return p->m_pSecurityDomain;
}

}