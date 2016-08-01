#ifndef _AS3ObjectEncoding_
#define _AS3ObjectEncoding_
namespace avmplus{namespace NativeID{
class ObjectEncodingClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//uint32_t AMF0 = 0;
	//uint32_t AMF3 = 3;
	//uint32_t DEFAULT = 3;

	uint32_t AMF0;
	uint32_t AMF3;
	uint32_t DEFAULT;
private:};
class ObjectEncodingObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	uint32_t AMF0;
	uint32_t AMF3;
	uint32_t DEFAULT;
private:};
}}
namespace avmshell{
	class ObjectEncodingClass : public ClassClosure
	{
	public:
		ObjectEncodingClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ObjectEncodingClassSlots m_slots_ObjectEncodingClass;
};
class ObjectEncodingObject : public ScriptObject
{
	public:
		ObjectEncodingObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_dynamicPropertyWriter_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_dynamicPropertyWriter_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ObjectEncodingObjectSlots m_slots_ObjectEncodingObject;
};}
#endif