#ifndef _AS3HTMLPDFCapability_
#define _AS3HTMLPDFCapability_
namespace avmplus{namespace NativeID{
class HTMLPDFCapabilityClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ERROR_CANNOT_LOAD_READER : int = 3204
//ERROR_INSTALLED_READER_NOT_FOUND : int = 3201
//ERROR_INSTALLED_READER_TOO_OLD : int = 3202
//ERROR_PREFERRED_READER_TOO_OLD : int = 3203

	int ERROR_CANNOT_LOAD_READER;
	int ERROR_INSTALLED_READER_NOT_FOUND;
	int ERROR_INSTALLED_READER_TOO_OLD;
	int ERROR_PREFERRED_READER_TOO_OLD;

private:};
class HTMLPDFCapabilityObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class HTMLPDFCapabilityClass : public ClassClosure
	{
	public:
		HTMLPDFCapabilityClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLPDFCapabilityClassSlots m_slots_HTMLPDFCapabilityClass;
};
class HTMLPDFCapabilityObject : public ScriptObject
{
	public:
		HTMLPDFCapabilityObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HTMLPDFCapabilityObjectSlots m_slots_HTMLPDFCapabilityObject;
};}
#endif