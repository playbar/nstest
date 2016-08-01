#ifndef _AS3DRMPlaybackTimeWindow_
#define _AS3DRMPlaybackTimeWindow_
namespace avmplus{namespace NativeID{
class DRMPlaybackTimeWindowClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DRMPlaybackTimeWindowObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	uint32_t	m_length;
	DateObject	*m_startDate;
	DateObject	*m_endDate;
private:};
}}
namespace avmshell{
	class DRMPlaybackTimeWindowClass : public ClassClosure
	{
	public:
		DRMPlaybackTimeWindowClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMPlaybackTimeWindowClassSlots m_slots_DRMPlaybackTimeWindowClass;
};
class DRMPlaybackTimeWindowObject : public ScriptObject
{
	public:
		DRMPlaybackTimeWindowObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//AvmBox AS3_endDate_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_period_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_startDate_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMPlaybackTimeWindowObjectSlots m_slots_DRMPlaybackTimeWindowObject;
};}
#endif