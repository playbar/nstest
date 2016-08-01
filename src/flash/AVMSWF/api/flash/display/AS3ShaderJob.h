#ifndef _AS3ShaderJob_
#define _AS3ShaderJob_

#include "AS3EventDispatcher.h"

#include "AS3Shader.h"

namespace avmplus{namespace NativeID{
class ShaderJobClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ShaderJobObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ShaderJobClass : public ClassClosure
	{
	public:
		ShaderJobClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderJobClassSlots m_slots_ShaderJobClass;
};
class ShaderJobObject : public EventDispatcherObject
{
	public:
		ShaderJobObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		int AS3_height_get();
		void AS3_height_set(int height);
		double AS3_progress_get();
		ShaderObject* AS3_shader_get();
		void AS3_shader_set(ShaderObject *pShader);
		Atom AS3_target_get();
		void AS3_target_set(Atom target);
		int AS3_width_get();
		void AS3_width_set(int width);

//		void AS3_constructor(ShaderObject *pShader, Atom target, int width, int height);
		void AS3_cancel();
		void AS3_start(bool waitForCompletion);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderJobObjectSlots m_slots_ShaderJobObject;
};}
#endif