#ifndef _AS3Shape_
#define _AS3Shape_

#include "AS3DisplayObject.h"

namespace avmplus{namespace NativeID{
class ShapeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ShapeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GraphicsObject;
	class ShapeObject;
	class ShapeClass : public ClassClosure
	{
	public:
		
		ShapeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ShapeObject* CreateObject(void*pObject);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShapeClassSlots m_slots_ShapeClass;
};
class ShapeObject : public DisplayObjectObject
{
	public:
		ShapeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		
	public:
		DRCWB(GraphicsObject*) m_pGraphics;
		GraphicsObject* AS3_graphics_get();
		void AS3_constructor();
		virtual GraphicsObject* GetGraphics(){return m_pGraphics;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShapeObjectSlots m_slots_ShapeObject;
};}
#endif