#ifndef _AS3Accelerometer_
#define _AS3Accelerometer_
namespace avmplus{namespace NativeID{
class AccelerometerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class AccelerometerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class AccelerometerClass : public ClassClosure
	{
	public:
		AccelerometerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccelerometerClassSlots m_slots_AccelerometerClass;
};
class AccelerometerObject : public ScriptObject
{
	public:
		AccelerometerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		bool AS3_isSupported_get();
		bool AS3_muted_get();
//		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_setRequestedUpdateInterval(double interval);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AccelerometerObjectSlots m_slots_AccelerometerObject;
};}
#endif