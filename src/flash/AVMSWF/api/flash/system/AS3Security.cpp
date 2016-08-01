#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Security.h"
namespace avmshell{

int SecurityClass::m_nSandType=XAS3_REMOTE;
SecurityClass::SecurityClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	m_bExactSetting=true;
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SecurityObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SecurityClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SecurityObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SecurityObject::SecurityObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	
}
//////////////////////////////////////////////////////////
//Native Method start...

void SecurityClass::AS3_allowDomain(ArrayObject*)
{
	//Add your act code here...
}

void SecurityClass::AS3_allowInsecureDomain(ArrayObject*)
{
	//Add your act code here...
}

void SecurityClass::AS3_loadPolicyFile(Stringp)
{
	//Add your act code here...	
}

void SecurityClass::AS3_showSettings(Stringp)
{
	//Add your act code here...

}

//new adds
Stringp SecurityClass::AS3_pageDomain_get()
{
	return NULL;
}

//new adds end

}