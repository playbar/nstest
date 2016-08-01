#ifndef _AS3LastOperationStatus_
#define _AS3LastOperationStatus_
namespace avmplus{namespace NativeID{
class LastOperationStatusClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//BUFFER_OVERFLOW_ERROR : Stringp = "bufferOverflowError"
//ERROR_CODE_UNKNOWN : Stringp = "errorCodeUnknown"
//ILLEGAL_ARGUMENT_ERROR : Stringp = "illegalArgumentError"
//INDEX_OUT_OF_BOUNDS_ERROR : Stringp = "indexOutOfBoundsError"
//INVALID_ATTR_VALUE : Stringp = "invalidAttrValue"
//INVALID_CHAR_FOUND : Stringp = "invalidCharFound"
//MEMORY_ALLOCATION_ERROR : Stringp = "memoryAllocationError"
//NO_ERROR : Stringp = "noError"
//NUMBER_OVERFLOW_ERROR : Stringp = "numberOverflowError"
//PARSE_ERROR : Stringp = "parseError"
//PATTERN_SYNTAX_ERROR : Stringp = "patternSyntaxError"
//PLATFORM_API_FAILED : Stringp = "platformAPIFailed"
//TRUNCATED_CHAR_FOUND : Stringp = "truncatedCharFound"
//UNEXPECTED_TOKEN : Stringp = "unexpectedToken"
//UNSUPPORTED_ERROR : Stringp = "unsupportedError"
//USING_DEFAULT_WARNING : Stringp = "usingDefaultWarning"
//USING_FALLBACK_WARNING : Stringp = "usingFallbackWarning"

	Stringp BUFFER_OVERFLOW_ERROR;
	Stringp ERROR_CODE_UNKNOWN;
	Stringp ILLEGAL_ARGUMENT_ERROR;
	Stringp INDEX_OUT_OF_BOUNDS_ERROR;
	Stringp INVALID_ATTR_VALUE;
	Stringp INVALID_CHAR_FOUND;
	Stringp MEMORY_ALLOCATION_ERROR;
	Stringp NO_ERROR_C;
	Stringp NUMBER_OVERFLOW_ERROR;
	Stringp PARSE_ERROR;
	Stringp PATTERN_SYNTAX_ERROR;
	Stringp PLATFORM_API_FAILED;
	Stringp TRUNCATED_CHAR_FOUND;
	Stringp UNEXPECTED_TOKEN;
	Stringp UNSUPPORTED_ERROR;
	Stringp USING_DEFAULT_WARNING;
	Stringp USING_FALLBACK_WARNING;

private:};
class LastOperationStatusObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class LastOperationStatusClass : public ClassClosure
	{
	public:
		LastOperationStatusClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LastOperationStatusClassSlots m_slots_LastOperationStatusClass;
};
class LastOperationStatusObject : public ScriptObject
{
	public:
		LastOperationStatusObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LastOperationStatusObjectSlots m_slots_LastOperationStatusObject;
};}
#endif