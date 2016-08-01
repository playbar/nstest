#ifndef _AS3Endian_
#define _AS3Endian_

namespace avmplus{namespace NativeID{
class EndianClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp BIG_ENDIAN = "bigEndian";
	//Stringp LITTLE_ENDIAN = "littleEndian"; 

#ifdef __APPLE__
    Stringp BIG_ENDIAN_APPLE;
    Stringp LITTLE_ENDIAN_APPLE;
#elif defined(_SYMBIAN)
    Stringp BIG_ENDIAN_SYMBIAN;
    Stringp LITTLE_ENDIAN_SYMBIAN;
#else
#ifdef __ANDROID__
#undef BIG_ENDIAN
#undef LITTLE_ENDIAN
#endif
	Stringp BIG_ENDIAN_0;
	Stringp LITTLE_ENDIAN_0;
#endif
private:};
class EndianObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class EndianClass : public ClassClosure
	{
	public:
		EndianClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EndianClassSlots m_slots_EndianClass;
	public:
#ifdef __APPLE__
        inline Stringp getSlotBIG_ENDIAN(){return m_slots_EndianClass.BIG_ENDIAN_APPLE;}
		inline Stringp getSlotLITTLE_ENDIAN(){return m_slots_EndianClass.LITTLE_ENDIAN_APPLE;}
#elif defined(_SYMBIAN)
        inline Stringp getSlotBIG_ENDIAN(){return m_slots_EndianClass.BIG_ENDIAN_SYMBIAN;}
		inline Stringp getSlotLITTLE_ENDIAN(){return m_slots_EndianClass.LITTLE_ENDIAN_SYMBIAN;}
#else
		inline Stringp getSlotBIG_ENDIAN(){return m_slots_EndianClass.BIG_ENDIAN_0;}
		inline Stringp getSlotLITTLE_ENDIAN(){return m_slots_EndianClass.LITTLE_ENDIAN_0;}
#endif
};
class EndianObject : public ScriptObject
{
	public:
		EndianObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EndianObjectSlots m_slots_EndianObject;
};}
#endif
