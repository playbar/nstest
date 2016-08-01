#ifndef _AS3SoundLoaderContext_
#define _AS3SoundLoaderContext_
namespace avmplus{namespace NativeID{
class SoundLoaderContextClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SoundLoaderContextObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	double buffTime;
	int	   checkPolicyFile;
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SoundLoaderContextClass : public ClassClosure
	{
	public:
		SoundLoaderContextClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundLoaderContextClassSlots m_slots_SoundLoaderContextClass;
};
class SoundLoaderContextObject : public ScriptObject
{
	public:
		SoundLoaderContextObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		int getBufferTime(){return m_slots_SoundLoaderContextObject.buffTime;}
		bool getCheckPolicyFile(){return m_slots_SoundLoaderContextObject.checkPolicyFile!=0;}
		/*void* pData;//Set your data!!
		AvmBox AS3_bufferTime_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bufferTime_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_checkPolicyFile_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_checkPolicyFile_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);*/
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundLoaderContextObjectSlots m_slots_SoundLoaderContextObject;
};}
#endif