#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "AS3JPEGXREncoderOptions.h"

namespace avmshell
{
	JPEGXREncoderOptionsClass::JPEGXREncoderOptionsClass(VTable* cvtable):ClassClosure(cvtable)
	{
		createVanillaPrototype();
	};
	ScriptObject* JPEGXREncoderOptionsClass::createInstance(VTable* ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) JPEGXREncoderOptionsObject(ivtable, prototype, 0);
	}

	JPEGXREncoderOptionsObject::JPEGXREncoderOptionsObject(VTable* vtable,ScriptObject* proto,int capacity):ScriptObject(vtable,proto,0)
	{
		m_ColorSpace = ((ShellToplevel*)toplevel())->getBitmapEncodingColorSpaceClass()->getSlotCOLORSPACE_AUTO();
		m_quantization = 20;
		m_trimFlexBits = 0;
	}

}