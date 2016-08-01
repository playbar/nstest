#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;

#include "AS3JPEGEncoderOptions.h"

namespace avmshell
{
	JPEGEncoderOptionsClass::JPEGEncoderOptionsClass(VTable* cvtable):ClassClosure(cvtable)
	{
		createVanillaPrototype();
		//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextFieldAutoSizeObject(cvtable, toplevel()->objectClass->prototype, 0);
		//Add your construct code here...
	};
	ScriptObject* JPEGEncoderOptionsClass::createInstance(VTable* ivtable, ScriptObject* prototype)
	{
		return new(core()->GetGC(), ivtable->getExtraSize()) JPEGEncoderOptionsObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}

	JPEGEncoderOptionsObject::JPEGEncoderOptionsObject(VTable* vtable, ScriptObject* proto, int capacity):ScriptObject(vtable,proto,0)
	{
		m_quality = 80;
	}
}