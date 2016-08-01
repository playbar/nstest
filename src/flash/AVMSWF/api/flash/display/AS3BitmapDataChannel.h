#ifndef _AS3BitmapDataChannel_
#define _AS3BitmapDataChannel_
namespace avmplus{namespace NativeID{
class BitmapDataChannelClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
		//uint32_t ALPHA = 8;
		//uint32_t BLUE= 4;
		//uint32_t GREEN = 2;
		//uint32_t RED = 1;

		uint32_t ALPHA;
		uint32_t BLUE;
		uint32_t GREEN;
		uint32_t RED;
private:};
class BitmapDataChannelObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class BitmapDataChannelClass : public ClassClosure
	{
	public:
		BitmapDataChannelClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapDataChannelClassSlots m_slots_BitmapDataChannelClass;
	public:
		inline uint32_t getSlotALPHA(){return m_slots_BitmapDataChannelClass.ALPHA;}
		inline uint32_t getSlotBLUE(){return m_slots_BitmapDataChannelClass.BLUE;}
		inline uint32_t getSlotGREEN(){return m_slots_BitmapDataChannelClass.GREEN;}
		inline uint32_t getSlotRED(){return m_slots_BitmapDataChannelClass.RED;}
};
class BitmapDataChannelObject : public ScriptObject
{
	public:
		BitmapDataChannelObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapDataChannelObjectSlots m_slots_BitmapDataChannelObject;
};}
#endif