#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3AccessibilityProperties.h"
namespace avmshell{
AccessibilityPropertiesClass::AccessibilityPropertiesClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())AccessibilityPropertiesObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* AccessibilityPropertiesClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) AccessibilityPropertiesObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
AccessibilityPropertiesObject::AccessibilityPropertiesObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	m_strDescription=NULL;
	m_bForceSimple=false;
//#if (__CORE_VERSION__>=0x02074000)
	m_strName=core()->kEmptyString;
//#else
//	m_strName=NULL;
//#endif
	m_bNoAutoLabeling=false;
	m_strShortcut=NULL;
	m_bSilent=false;
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp AccessibilityPropertiesObject::AS3_description_get()
{
	//Add your act code here...
	return m_strDescription;
	//return kAvmThunkUndefined;//Modify this please!
}

void AccessibilityPropertiesObject::AS3_description_set(Stringp str)
{

	//Add your act code here...
	m_strDescription=str;
	//return kAvmThunkUndefined;//Modify this please!
}

bool AccessibilityPropertiesObject::AS3_forceSimple_get()
{
	//Add your act code here...
	return m_bForceSimple;
	//return kAvmThunkUndefined;//Modify this please!
}

void AccessibilityPropertiesObject::AS3_forceSimple_set(bool b)
{
	//Add your act code here...
	m_bForceSimple=b;
	//return kAvmThunkUndefined;//Modify this please!
}

Stringp AccessibilityPropertiesObject::AS3_name_get()
{
	//Add your act code here...
	return m_strName;
	//return kAvmThunkUndefined;//Modify this please!
}

void AccessibilityPropertiesObject::AS3_name_set(Stringp strName)
{
	//Add your act code here...
	m_strName=strName;
	//return kAvmThunkUndefined;//Modify this please!
}

bool AccessibilityPropertiesObject::AS3_noAutoLabeling_get()
{
	//Add your act code here...
	return m_bNoAutoLabeling;
	//return kAvmThunkUndefined;//Modify this please!
}

void AccessibilityPropertiesObject::AS3_noAutoLabeling_set(bool b)
{
	//Add your act code here...
	m_bNoAutoLabeling=b;
	//return kAvmThunkUndefined;//Modify this please!
}

Stringp AccessibilityPropertiesObject::AS3_shortcut_get()
{
	//Add your act code here...
	return m_strShortcut;
	//return kAvmThunkUndefined;//Modify this please!
}

void AccessibilityPropertiesObject::AS3_shortcut_set(Stringp str)
{
	//Add your act code here...
	m_strShortcut=str;
	//return kAvmThunkUndefined;//Modify this please!
}

bool AccessibilityPropertiesObject::AS3_silent_get()
{
	//Add your act code here...
	return m_bSilent;
	//return kAvmThunkUndefined;//Modify this please!
}

void AccessibilityPropertiesObject::AS3_silent_set(bool b)
{
	//Add your act code here...
	m_bSilent=b;
	//return kAvmThunkUndefined;//Modify this please!
}

//AS3 contructor function..
void AccessibilityPropertiesObject::AS3_constructor()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

}