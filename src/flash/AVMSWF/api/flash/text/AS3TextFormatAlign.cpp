#include "StdAfxflash.h"
#include "avmplus.h"
#include "stags.h"
using namespace avmplus;
#include "AS3TextFormatAlign.h"
namespace avmshell{
TextFormatAlignClass::TextFormatAlignClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextFormatAlignObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextFormatAlignClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextFormatAlignObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextFormatAlignObject::TextFormatAlignObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...

Stringp TextFormatAlignClass::IDToString(short id)
{
	switch(id)
	{
		case stextAlignLeft:
			return getSlotLEFT();
		case stextAlignRight:
			return getSlotRIGHT();
		case stextAlignCenter:
			return getSlotCENTER();
		case stextAlignJustify:
			return getSlotJUSTIFY();
		case stextAlignEnd:
			return getSlotEND();
		case stextAlignStart:
			return getSlotSTART();
		default:
			return getSlotSTART();
	}
}

short   TextFormatAlignClass::StringToID(Stringp s)
{
	if(s->equals(getSlotLEFT()))
		return stextAlignLeft;
	else if(s->equals(getSlotRIGHT()))
		return stextAlignRight;
	else if(s->equals(getSlotCENTER()))
		return stextAlignCenter;
	else if(s->equals(getSlotJUSTIFY()))
		return stextAlignJustify;
	else if(s->equals(getSlotEND()))
		return stextAlignEnd;
	else 
		return stextAlignStart;
}
}

