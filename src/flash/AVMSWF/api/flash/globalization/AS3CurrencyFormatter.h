#ifndef _AS3CurrencyFormatter_
#define _AS3CurrencyFormatter_

#include "AS3CurrencyParseResult.h"

namespace avmplus{namespace NativeID{
class CurrencyFormatterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CurrencyFormatterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CurrencyFormatterClass : public ClassClosure
	{
	public:
		CurrencyFormatterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		ObjectVectorObject* AS3_getAvailableLocaleIDNames();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CurrencyFormatterClassSlots m_slots_CurrencyFormatterClass;
};
class CurrencyFormatterObject : public ScriptObject
{
	public:
		CurrencyFormatterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp AS3_actualLocaleIDName_get();
		Stringp AS3_currencyISOCode_get();
		Stringp AS3_currencySymbol_get();
		Stringp AS3_decimalSeparator_get();
		void AS3_decimalSeparator_set(Stringp decimalSeparator);
		uint32_t AS3_digitsType_get();
		void AS3_digitsType_set(uint32_t digitsType);
		int AS3_fractionalDigits_get();
		void AS3_fractionalDigits_set(int fractionalDigits);
		Stringp AS3_groupingPattern_get();
		void AS3_groupingPattern_set(Stringp groupingPattern);
		Stringp AS3_groupingSeparator_get();
		void AS3_groupingSeparator_set(Stringp groupingSeparator);
		Stringp AS3_lastOperationStatus_get();
		bool AS3_leadingZero_get();
		void AS3_leadingZero_set(bool leadingZero);
		uint32_t AS3_negativeCurrencyFormat_get();
		void AS3_negativeCurrencyFormat_set(uint32_t negativeCurrencyFormat);
		Stringp AS3_negativeSymbol_get();
		void AS3_negativeSymbol_set(Stringp negativeSymbol);
		uint32_t AS3_positiveCurrencyFormat_get();
		void AS3_positiveCurrencyFormat_set(uint32_t positiveCurrencyFormat);
		Stringp AS3_requestedLocaleIDName_get();
		bool AS3_trailingZeros_get();
		void AS3_trailingZeros_set(bool trailingZeros);
		bool AS3_useGrouping_get();
		void AS3_useGrouping_set(bool useGrouping);
		void AS3_constructor(Stringp requestedLocaleIDName);
		Stringp AS3_format(double value, bool withCurrencySymbol);
		bool AS3_formattingWithCurrencySymbolIsSafe(Stringp requestedISOCode);
		CurrencyParseResultObject* AS3_parse(Stringp inputString);
		void AS3_setCurrency(Stringp currencyISOCode, Stringp currencySymbol);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CurrencyFormatterObjectSlots m_slots_CurrencyFormatterObject;
};}
#endif