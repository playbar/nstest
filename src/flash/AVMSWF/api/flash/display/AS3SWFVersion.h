#ifndef _AS3SWFVersion_
#define _AS3SWFVersion_
namespace avmplus{namespace NativeID{
class SWFVersionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//uint32_t FLASH1 = 1;
	//uint32_t FLASH2 = 2;
	//uint32_t FLASH3 = 3;
	//uint32_t FLASH4 = 4;
	//uint32_t FLASH5 = 5;
	//uint32_t FLASH6 = 6;
	//uint32_t FLASH7 = 7;
	//uint32_t FLASH8 = 8;
	//uint32_t FLASH9 = 9;

	uint32_t FLASH1;
	uint32_t FLASH2;
	uint32_t FLASH3;
	uint32_t FLASH4;
	uint32_t FLASH5;
	uint32_t FLASH6;
	uint32_t FLASH7;
	uint32_t FLASH8;
	uint32_t FLASH9;
	uint32_t FLASH10;
private:};
class SWFVersionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SWFVersionClass : public ClassClosure
	{
	public:
		SWFVersionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SWFVersionClassSlots m_slots_SWFVersionClass;
};
class SWFVersionObject : public ScriptObject
{
	public:
		SWFVersionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SWFVersionObjectSlots m_slots_SWFVersionObject;
};}
#endif