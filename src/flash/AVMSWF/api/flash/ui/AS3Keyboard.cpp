#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Keyboard.h"
namespace avmshell{
KeyboardClass::KeyboardClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())KeyboardObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* KeyboardClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) KeyboardObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
KeyboardObject::KeyboardObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}

bool KeyboardClass::AS3_isAccessible()
{
	//Add your act code here...
	ShellCodeContext*cnt=((ShellCore*)core())->GetShellContext();
		//core()->codeContext();

		//m_pCodeContext;//((ShellCore*)core())->GetCodeContext();
		//(ShellCodeContext*)core()->codeContext();
	return cnt->m_bAccessiable;
	//return kAvmThunkUndefined;//Modify this please!
}

}