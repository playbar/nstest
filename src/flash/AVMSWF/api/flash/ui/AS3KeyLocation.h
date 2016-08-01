#ifndef _AS3KeyLocation_
#define _AS3KeyLocation_
namespace avmplus{namespace NativeID{
class KeyLocationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//uint32_t LEFT = 1;
	//uint32_t NUM_PAD = 3; 
	//uint32_t RIGHT = 2; 
	//uint32_t STANDARD = 0; 
	//D_PAD : uint = 4;

	uint32_t LEFT;
	uint32_t NUM_PAD; 
	uint32_t RIGHT; 
	uint32_t STANDARD;

	//new adds
	uint32_t D_PAD;
	//new adds end
private:};
class KeyLocationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class KeyLocationClass : public ClassClosure
	{
	public:
		KeyLocationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		inline uint32_t getSlotLEFT(){return m_slots_KeyLocationClass.LEFT;}
		inline uint32_t getSlotNUM_PAD(){return m_slots_KeyLocationClass.NUM_PAD;}
		inline uint32_t getSlotRIGHT(){return m_slots_KeyLocationClass.RIGHT;}
		inline uint32_t getSlotSTANDARD(){return m_slots_KeyLocationClass.STANDARD;}

		//new adds
		inline uint32_t getSlotD_PAD(){return m_slots_KeyLocationClass.D_PAD;}
		//new adds end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KeyLocationClassSlots m_slots_KeyLocationClass;
};
class KeyLocationObject : public ScriptObject
{
	public:
		KeyLocationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KeyLocationObjectSlots m_slots_KeyLocationObject;
};}
#endif