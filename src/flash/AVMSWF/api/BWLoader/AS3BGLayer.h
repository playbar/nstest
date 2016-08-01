#ifndef _AS3BXBGLayer_
#define _AS3BXBGLayer_
class XSWFPlayer;
class XDrawDib;
class SBitmapCore;
#include "AS3Sprite.h"
namespace avmplus{namespace NativeID{
class BXBGLayerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//void* instance;
private:};
class BXBGLayerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class RectangleObject;
	class PointObject;
	class BitmapFilterObject;
	class BXBGLayerObject;
	class ColorTransformObject;
	class MatrixObject;
	class ByteArrayObject;
	class DisplayObjectObject;
	class BXBGLayerClass : public ClassClosure
	{
	public:
		BXBGLayerClass(VTable *vtable);
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXBGLayerClassSlots m_slots_BXBGLayerClass;
};
class BXBGLayerObject : public SpriteObject//ScriptObject
{
	public:
		//MATRIX m_totalMatrix;
		bool m_bScrollMode;
		BXBGLayerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		XSWFPlayer* m_pPlayer;
	public:
		//double m_offx,m_offy;
		void AS3_scrollMode_set(bool bs);//{m_bScrollMode=bs;}
		void AS3_x_set(double v);
		double AS3_x_get();
		void AS3_y_set(double v);
		double AS3_y_get();
		void AS3_showBG();
		void AS3_hideBG();
		//void AS3_release();
		//void AS3_create();
		void AS3_constructor();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXBGLayerObjectSlots m_slots_BXBGLayerObject;
};}
#endif
