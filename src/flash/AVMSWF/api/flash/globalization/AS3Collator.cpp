#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Collator.h"
namespace avmshell{
CollatorClass::CollatorClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* CollatorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CollatorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CollatorObject::CollatorObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ObjectVectorObject* CollatorClass::AS3_getAvailableLocaleIDNames()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp CollatorObject::AS3_actualLocaleIDName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

bool CollatorObject::AS3_ignoreCase_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void CollatorObject::AS3_ignoreCase_set(bool ignoreCase)
{
	//Add your act code here...
}

bool CollatorObject::AS3_ignoreCharacterWidth_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void CollatorObject::AS3_ignoreCharacterWidth_set(bool ignoreCharacterWidth)
{
	//Add your act code here...
}

bool CollatorObject::AS3_ignoreDiacritics_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void CollatorObject::AS3_ignoreDiacritics_set(bool ignoreDiacritics)
{
	//Add your act code here...
}

bool CollatorObject::AS3_ignoreKanaType_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void CollatorObject::AS3_ignoreKanaType_set(bool ignoreKanaType)
{
	//Add your act code here...
}

bool CollatorObject::AS3_ignoreSymbols_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void CollatorObject::AS3_ignoreSymbols_set(bool ignoreSymbols)
{
	//Add your act code here...
}

Stringp CollatorObject::AS3_lastOperationStatus_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

bool CollatorObject::AS3_numericComparison_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void CollatorObject::AS3_numericComparison_set(bool numericComparison)
{
	//Add your act code here...
}

Stringp CollatorObject::AS3_requestedLocaleIDName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

//AS3 contructor function..
void CollatorObject::AS3_constructor(Stringp requestedLocaleIDName, Stringp initialMode)
{
	//Add your act code here...
}

int CollatorObject::AS3_compare(Stringp string1, Stringp string2)
{
	//Add your act code here...
	return 0;//Modify this please!
}

bool CollatorObject::AS3_equals(Stringp string1, Stringp string2)
{
	//Add your act code here...
	return false;//Modify this please!
}

}
