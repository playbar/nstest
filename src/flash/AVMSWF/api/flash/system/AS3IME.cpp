#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3IME.h"
#include "AS3IMEConversionMode.h"
namespace avmshell{
IMEClass::IMEClass(VTable* cvtable):ClassClosure(cvtable)
{
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())IMEObject(cvtable, toplevel()->objectClass->prototype, 0);
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IMEClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IMEObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

void IMEClass::SetComposition(int id)
{
	m_strComposition=((ShellToplevel*)toplevel())->getIMEConversionModeClass()->NameOf(id);
}

IMEObject::IMEObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}


//////////////////////////////////////////////////////////
//Native Method start...
void IMEClass::AS3_doConversion()
{
	//Add your act code here...	
}

void IMEClass::AS3_setCompositionString(Stringp s)
{
	//Add your act code here...
	m_strComposition=s;
}

Stringp IMEClass::AS3_conversionMode_get()
{
	//Add your act code here...
	return m_strComposition;
}

bool IMEClass::AS3_enabled_get()
{
	//Add your act code here...
	return true;
}

//new adds
//bool IMEClass::AS3_isSupported_get()
//{
//	return false;
//}

void IMEClass::AS3_compositionAbandoned()
{
}
void IMEClass::AS3_compositionSelectionChanged(int start, int end)
{
}
//new adds end

}