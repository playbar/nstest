#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DateTimeFormatter.h"
namespace avmshell{
DateTimeFormatterClass::DateTimeFormatterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DateTimeFormatterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DateTimeFormatterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DateTimeFormatterObject::DateTimeFormatterObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ObjectVectorObject* DateTimeFormatterClass::AS3_getAvailableLocaleIDNames()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp DateTimeFormatterObject::AS3_actualLocaleIDName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp DateTimeFormatterObject::AS3_lastOperationStatus_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp DateTimeFormatterObject::AS3_requestedLocaleIDName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

//AS3 contructor function..
void DateTimeFormatterObject::AS3_constructor(Stringp requestedLocaleIDName, Stringp dateStyle, Stringp timeStyle)
{
	//Add your act code here...
}

Stringp DateTimeFormatterObject::AS3_format(DateObject *pDateTime)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp DateTimeFormatterObject::AS3_formatUTC(DateObject *pDateTime)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp DateTimeFormatterObject::AS3_getDateStyle()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp DateTimeFormatterObject::AS3_getDateTimePattern()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

int DateTimeFormatterObject::AS3_getFirstWeekday()
{
	//Add your act code here...
	return 0;//Modify this please!
}

ObjectVectorObject* DateTimeFormatterObject::AS3_getMonthNames(Stringp nameStyle, Stringp context)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp DateTimeFormatterObject::AS3_getTimeStyle()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

ObjectVectorObject* DateTimeFormatterObject::AS3_getWeekdayNames(Stringp nameStyle, Stringp context)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void DateTimeFormatterObject::AS3_setDateTimePattern(Stringp pattern)
{
	//Add your act code here...
}

void DateTimeFormatterObject::AS3_setDateTimeStyles(Stringp dateStyle, Stringp timeStyle)
{
	//Add your act code here...
}

}