#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NumberParseResult.h"
namespace avmshell{
NumberParseResultClass::NumberParseResultClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NumberParseResultClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NumberParseResultObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NumberParseResultObject::NumberParseResultObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
int NumberParseResultObject::AS3_endIndex_get()
{
	//Add your act code here...
	return 0;//Modify this please!
}

int NumberParseResultObject::AS3_startIndex_get()
{
	//Add your act code here...
	return 0;//Modify this please!
}

double NumberParseResultObject::AS3_value_get()
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

//AS3 contructor function..
void NumberParseResultObject::AS3_constructor(double value, int startIndex, int endIndex)
{
	//Add your act code here...
}

}