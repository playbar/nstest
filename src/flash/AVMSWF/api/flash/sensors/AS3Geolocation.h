#ifndef _AS3Geolocation_
#define _AS3Geolocation_
namespace avmplus{namespace NativeID{
class GeolocationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GeolocationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GeolocationClass : public ClassClosure
	{
	public:
		GeolocationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GeolocationClassSlots m_slots_GeolocationClass;
};
class GeolocationObject : public ScriptObject
{
	public:
		GeolocationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		bool AS3_isSupported_get();
		bool AS3_muted_get();
//		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_setRequestedUpdateInterval(double interval);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GeolocationObjectSlots m_slots_GeolocationObject;
};}
#endif