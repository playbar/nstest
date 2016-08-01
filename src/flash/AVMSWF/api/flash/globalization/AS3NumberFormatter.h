#ifndef _AS3NumberFormatter_
#define _AS3NumberFormatter_

#include "AS3NumberParseResult.h"

namespace avmplus{namespace NativeID{
class NumberFormatterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NumberFormatterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NumberFormatterClass : public ClassClosure
	{
	public:
		NumberFormatterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NumberFormatterClassSlots m_slots_NumberFormatterClass;
};
class NumberFormatterObject : public ScriptObject
{
	public:
		NumberFormatterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp	AS3_actualLocaleIDName_get();

		Stringp	AS3_decimalSeparator_get();
		void	AS3_decimalSeparator_set(Stringp decimalSeparator);

		uint32_t	AS3_digitsType_get();
		void	AS3_digitsType_set(uint32_t digitsType);

		int	AS3_fractionalDigits_get();
		void	AS3_fractionalDigits_set(int fractionalDigits);

		Stringp	AS3_groupingPattern_get();
		void	AS3_groupingPattern_set(Stringp groupingPattern);

		Stringp	AS3_groupingSeparator_get();
		void	AS3_groupingSeparator_set(Stringp groupingSeparator);

		Stringp	AS3_lastOperationStatus_get();

		bool	AS3_leadingZero_get();
		void	AS3_leadingZero_set(bool leadingZero);

		uint32_t	AS3_negativeNumberFormat_get();
		void	AS3_negativeNumberFormat_set(uint32_t negativeNumberFormat);

		Stringp	AS3_negativeSymbol_get();
		void	AS3_negativeSymbol_set(Stringp negativeSymbol);

		Stringp	AS3_requestedLocaleIDName_get();

		bool	AS3_trailingZeros_get();
		void 	AS3_trailingZeros_set(bool trailingZeros);

		bool	AS3_useGrouping_get();
		void	AS3_useGrouping_set(bool useGrouping);

		void	AS3_constructor(Stringp requestedLocaleIDName);

		Stringp	AS3_formatInt(int value);
		Stringp	AS3_formatNumber(double value);
		Stringp	AS3_formatUint(uint32_t value);
		ObjectVectorObject*	AS3_getAvailableLocaleIDNames();
		NumberParseResultObject*	AS3_parse(Stringp parseString);
		double	AS3_parseNumber(Stringp parseString);

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NumberFormatterObjectSlots m_slots_NumberFormatterObject;
};}
#endif