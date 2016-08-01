#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CurrencyParseResult.h"
namespace avmshell{
CurrencyParseResultClass::CurrencyParseResultClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* CurrencyParseResultClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CurrencyParseResultObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CurrencyParseResultObject::CurrencyParseResultObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp CurrencyParseResultObject::AS3_currencyString_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

double CurrencyParseResultObject::AS3_value_get()
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

//AS3 contructor function..
void CurrencyParseResultObject::AS3_constructor(double value, Stringp symbol)
{
	//Add your act code here...
}

}