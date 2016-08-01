#include "StdAfxflash.h"
#include "XSystem.h"
#include "ShellCore.h"

using namespace avmplus;
#include "AS3System.h"
#include "AS3IME.h"
namespace avmshell{
SystemClass::SystemClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	m_pIME=(IMEObject*)(((ShellToplevel*)toplevel())->getIMEClass()->newInstance());
	m_bUseCodePage=true;
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SystemObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SystemClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SystemObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SystemObject::SystemObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...

void SystemClass::AS3_gc()
{
	//((ShellToplevel*)toplevel())->ClearDeadImage(true);
}
uint32 SystemClass::AS3_totalMemory_get()
{
	//Add your act code here...
	return XSystem::GetUsedMemory();
}



void SystemClass::AS3_setClipboard(Stringp text)
{
	//Add your act code here...
	StUTF16String s(text);
	XString16 t(s.c_str(),s.length());
	XSystem::SetClipText(t);
	//XSystem::SetClipText(t);
}
//new adds
double SystemClass::AS3_freeMemory_get()
{
	return 0;
}
double SystemClass::AS3_privateMemory_get()
{
	return 0;
}
double SystemClass::AS3_totalMemoryNumber_get()
{
	return 0;
}

void SystemClass::AS3_disposeXML(XMLObject *pNode)
{

}
void SystemClass::AS3_exit(uint32_t code)
{

}
void SystemClass::AS3_pause()
{

}
void SystemClass::AS3_resume()
{

}
//new adds end

//new adds 11
void SystemClass::AS3_pauseForGCIfCollectionImminent(double imminence)
{
	LOGWHERE();
}
//new adds 11 end
}