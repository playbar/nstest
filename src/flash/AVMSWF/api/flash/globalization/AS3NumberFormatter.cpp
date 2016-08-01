#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NumberFormatter.h"
namespace avmshell{
NumberFormatterClass::NumberFormatterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NumberFormatterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NumberFormatterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NumberFormatterObject::NumberFormatterObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp	NumberFormatterObject::AS3_actualLocaleIDName_get()
{
	return NULL;
}

Stringp	NumberFormatterObject::AS3_decimalSeparator_get()
{
	return NULL;
}
void	NumberFormatterObject::AS3_decimalSeparator_set(Stringp decimalSeparator)
{

}

uint32_t	NumberFormatterObject::AS3_digitsType_get()
{
	return 0;
}
void	NumberFormatterObject::AS3_digitsType_set(uint32_t digitsType)
{

}

int	NumberFormatterObject::AS3_fractionalDigits_get()
{
	return 0;
}
void	NumberFormatterObject::AS3_fractionalDigits_set(int fractionalDigits)
{

}

Stringp	NumberFormatterObject::AS3_groupingPattern_get()
{
	return NULL;
}
void	NumberFormatterObject::AS3_groupingPattern_set(Stringp groupingPattern)
{

}

Stringp	NumberFormatterObject::AS3_groupingSeparator_get()
{
	return NULL;
}
void	NumberFormatterObject::AS3_groupingSeparator_set(Stringp groupingSeparator)
{

}

Stringp	NumberFormatterObject::AS3_lastOperationStatus_get()
{
	return NULL;
}

bool	NumberFormatterObject::AS3_leadingZero_get()
{
	return false;
}
void	NumberFormatterObject::AS3_leadingZero_set(bool leadingZero)
{

}

uint32_t	NumberFormatterObject::AS3_negativeNumberFormat_get()
{
	return 0;
}
void	NumberFormatterObject::AS3_negativeNumberFormat_set(uint32_t negativeNumberFormat)
{

}

Stringp	NumberFormatterObject::AS3_negativeSymbol_get()
{
	return NULL;
}
void	NumberFormatterObject::AS3_negativeSymbol_set(Stringp negativeSymbol)
{

}

Stringp	NumberFormatterObject::AS3_requestedLocaleIDName_get()
{
	return NULL;
}

bool	NumberFormatterObject::AS3_trailingZeros_get()
{
	return false;
}
void 	NumberFormatterObject::AS3_trailingZeros_set(bool trailingZeros)
{

}

bool	NumberFormatterObject::AS3_useGrouping_get()
{
	return false;
}
void	NumberFormatterObject::AS3_useGrouping_set(bool useGrouping)
{

}

void	NumberFormatterObject::AS3_constructor(Stringp requestedLocaleIDName)
{

}

Stringp	NumberFormatterObject::AS3_formatInt(int value)
{
	return NULL;
}
Stringp	NumberFormatterObject::AS3_formatNumber(double value)
{
	return NULL;
}
Stringp	NumberFormatterObject::AS3_formatUint(uint32_t value)
{
	return NULL;
}
ObjectVectorObject*	NumberFormatterObject::AS3_getAvailableLocaleIDNames()
{
	return NULL;
}
NumberParseResultObject*	NumberFormatterObject::AS3_parse(Stringp parseString)
{
	return NULL;
}
double	NumberFormatterObject::AS3_parseNumber(Stringp parseString)
{
	return 0.0;
}

}