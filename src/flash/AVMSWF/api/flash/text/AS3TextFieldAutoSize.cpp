#include "StdAfxflash.h"
#include "edittext.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextFieldAutoSize.h"
namespace avmshell{
TextFieldAutoSizeClass::TextFieldAutoSizeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextFieldAutoSizeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextFieldAutoSizeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextFieldAutoSizeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextFieldAutoSizeObject::TextFieldAutoSizeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
int TextFieldAutoSizeClass::ToID(Stringp a)
{
	if(a->equals(getSlotLEFT()))
	{
		//pEdit->AutoSize(XTRUE);
		 return stextAlignLeft;
	}
	else if(a->equals(getSlotRIGHT()))
	{
		//pEdit->AutoSize(XTRUE);
		return stextAlignRight;
	}
	else if(a->equals(getSlotCENTER()))
	{
		//pEdit->AutoSize(XTRUE);
		return stextAlignCenter;
	}
//#if (__CORE_VERSION__>=0x02075000)
	else if(a->equals(this->getSlotNONE()))
	{
		return stextAlignJustify;
	}
	else
		return -1;
//#else
//	else
//	{
//		//pEdit->AutoSize(FALSE);
//		return stextAlignJustify;
//	}
//#endif
}
Stringp TextFieldAutoSizeClass::ToString(int id)
{
	switch(id)
	{
	case stextAlignLeft:
		 return getSlotLEFT();
	case stextAlignRight:
		 return getSlotRIGHT();
	case stextAlignCenter:
		 return getSlotCENTER();
	default:
		 return getSlotNONE();
	}
}
}
