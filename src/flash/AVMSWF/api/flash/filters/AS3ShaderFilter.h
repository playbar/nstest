#ifndef _AS3ShaderFilter_
#define _AS3ShaderFilter_

#include "AS3BitmapFilter.h"

#include "AS3Shader.h"

namespace avmplus{namespace NativeID{
class ShaderFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ShaderFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ShaderFilterClass : public ClassClosure
	{
	public:
		ShaderFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderFilterClassSlots m_slots_ShaderFilterClass;
};
class ShaderFilterObject : public BitmapFilterObject
{
	public:
		ShaderFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		int AS3_bottomExtension_get();
		void AS3_bottomExtension_set(int bottomExtension);
		int AS3_leftExtension_get();
		void AS3_leftExtension_set(int leftExtension);
		int AS3_rightExtension_get();
		void AS3_rightExtension_set(int rightExtension);
		ShaderObject* AS3_shader_get();
		void AS3_shader_set(ShaderObject *pShader);
		int AS3_topExtension_get();
		void AS3_topExtension_set(int topExtension);

		void AS3_constructor(ShaderObject *pShader);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderFilterObjectSlots m_slots_ShaderFilterObject;
};}
#endif