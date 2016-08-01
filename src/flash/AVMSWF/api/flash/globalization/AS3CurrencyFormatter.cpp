#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CurrencyFormatter.h"
namespace avmshell{
CurrencyFormatterClass::CurrencyFormatterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* CurrencyFormatterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CurrencyFormatterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CurrencyFormatterObject::CurrencyFormatterObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ObjectVectorObject* CurrencyFormatterClass::AS3_getAvailableLocaleIDNames()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp CurrencyFormatterObject::AS3_actualLocaleIDName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp CurrencyFormatterObject::AS3_currencyISOCode_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp CurrencyFormatterObject::AS3_currencySymbol_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp CurrencyFormatterObject::AS3_decimalSeparator_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void CurrencyFormatterObject::AS3_decimalSeparator_set(Stringp decimalSeparator)
{
	//Add your act code here...
}

uint32_t CurrencyFormatterObject::AS3_digitsType_get()
{
	//Add your act code here...
	return 0;//Modify this please!
}

void CurrencyFormatterObject::AS3_digitsType_set(uint32_t digitsType)
{
	//Add your act code here...
}

int CurrencyFormatterObject::AS3_fractionalDigits_get()
{
	//Add your act code here...
	return 0;//Modify this please!
}

void CurrencyFormatterObject::AS3_fractionalDigits_set(int fractionalDigits)
{
	//Add your act code here...
}

Stringp CurrencyFormatterObject::AS3_groupingPattern_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void CurrencyFormatterObject::AS3_groupingPattern_set(Stringp groupingPattern)
{
	//Add your act code here...
}

Stringp CurrencyFormatterObject::AS3_groupingSeparator_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void CurrencyFormatterObject::AS3_groupingSeparator_set(Stringp groupingSeparator)
{
	//Add your act code here...
}

Stringp CurrencyFormatterObject::AS3_lastOperationStatus_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

bool CurrencyFormatterObject::AS3_leadingZero_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void CurrencyFormatterObject::AS3_leadingZero_set(bool leadingZero)
{
	//Add your act code here...
}

uint32_t CurrencyFormatterObject::AS3_negativeCurrencyFormat_get()
{
	//Add your act code here...
	return 0;//Modify this please!
}

void CurrencyFormatterObject::AS3_negativeCurrencyFormat_set(uint32_t negativeCurrencyFormat)
{
	//Add your act code here...
}

Stringp CurrencyFormatterObject::AS3_negativeSymbol_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void CurrencyFormatterObject::AS3_negativeSymbol_set(Stringp negativeSymbol)
{
	//Add your act code here...
}

uint32_t CurrencyFormatterObject::AS3_positiveCurrencyFormat_get()
{
	//Add your act code here...
	return 0;//Modify this please!
}

void CurrencyFormatterObject::AS3_positiveCurrencyFormat_set(uint32_t positiveCurrencyFormat)
{
	//Add your act code here...
}

Stringp CurrencyFormatterObject::AS3_requestedLocaleIDName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

bool CurrencyFormatterObject::AS3_trailingZeros_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void CurrencyFormatterObject::AS3_trailingZeros_set(bool trailingZeros)
{
	//Add your act code here...
}

bool CurrencyFormatterObject::AS3_useGrouping_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

void CurrencyFormatterObject::AS3_useGrouping_set(bool useGrouping)
{
	//Add your act code here...
}

//AS3 contructor function..
void CurrencyFormatterObject::AS3_constructor(Stringp requestedLocaleIDName)
{
	//Add your act code here...
}

Stringp CurrencyFormatterObject::AS3_format(double value, bool withCurrencySymbol)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

bool CurrencyFormatterObject::AS3_formattingWithCurrencySymbolIsSafe(Stringp requestedISOCode)
{
	//Add your act code here...
	return false;//Modify this please!
}

CurrencyParseResultObject* CurrencyFormatterObject::AS3_parse(Stringp inputString)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void CurrencyFormatterObject::AS3_setCurrency(Stringp currencyISOCode, Stringp currencySymbol)
{
	//Add your act code here...
}

}