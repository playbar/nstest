#ifndef _AS3GraphicElement_
#define _AS3GraphicElement_

#include "AS3DisplayObject.h"
#include "AS3ContentElement.h"

namespace avmplus{namespace NativeID{
class GraphicElementClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicElementObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextLineObject;
	class GraphicElementClass : public ClassClosure//ContentElementClass
	{
	public:
		GraphicElementClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicElementClassSlots m_slots_GraphicElementClass;
};
class GraphicElementObject : public ContentElementObject
{
	public:
		GraphicElementObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		int m_nWidth,m_nHeight;
		DRCWB(DisplayObjectObject*) m_pGraphic;
		DRCWB(TextLineObject*)		m_cacheLine;
		int							m_cacheX;
		void*					    m_cacheFormat;
	public:
		virtual bool CreateContext(TextLineObject*pLine,int nID);
		double AS3_elementHeight_get();
		void AS3_elementHeight_set(double elementHeight);
		double AS3_elementWidth_get();
		void AS3_elementWidth_set(double elementWidth);
		DisplayObjectObject* AS3_graphic_get();
		void AS3_graphic_set(DisplayObjectObject *pGraphic);
//		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicElementObjectSlots m_slots_GraphicElementObject;
};}
#endif