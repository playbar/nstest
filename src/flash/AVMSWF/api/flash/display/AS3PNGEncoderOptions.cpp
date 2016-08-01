#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3PNGEncoderOptions.h"
namespace avmshell{
	PNGEncoderOptionsClass::PNGEncoderOptionsClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* PNGEncoderOptionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) PNGEncoderOptionsObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	PNGEncoderOptionsObject::PNGEncoderOptionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_fastCompression = false;
	}
}