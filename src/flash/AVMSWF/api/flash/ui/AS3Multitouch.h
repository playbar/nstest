#ifndef _AS3Multitouch_
#define _AS3Multitouch_
namespace avmplus{namespace NativeID{
class MultitouchClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MultitouchObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MultitouchClass : public ClassClosure
	{
	public:
		MultitouchClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		Stringp AS3_inputMode_get();
		void AS3_inputMode_set(Stringp pInputMode);
		int AS3_maxTouchPoints_get();
		ObjectVectorObject* AS3_supportedGestures_get();
		bool AS3_supportsGestureEvents_get();
		bool AS3_supportsTouchEvents_get();

		bool AS3_mapTouchToMouse_get(){return false;}
		void AS3_mapTouchToMouse_set(bool mapTouchToMouse){}

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MultitouchClassSlots m_slots_MultitouchClass;
};
class MultitouchObject : public ScriptObject
{
	public:
		MultitouchObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
//		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MultitouchObjectSlots m_slots_MultitouchObject;
};}
#endif