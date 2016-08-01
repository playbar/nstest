#ifndef _AS3Sprite_
#define _AS3Sprite_

#include "AS3DisplayObjectContainer.h"
#include "AS3Rectangle.h"

namespace avmplus{namespace NativeID{
class SpriteClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SpriteObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DisplayObjectObject;
	class SpriteObject;
	class SoundTransformObject;
	class RectangleObject;
	class SpriteClass : public ClassClosure
	{
	public:
		SpriteClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SpriteClassSlots m_slots_SpriteClass;
};
class SpriteObject : public DisplayObjectContainerObject
{
	public:
		SpriteObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual GraphicsObject* GetGraphics(){return m_pGraphics;}
		virtual void InitObject();
	public:
		DRCWB(GraphicsObject*) m_pGraphics;
		DRCWB(SpriteObject*)  m_hitArea;
		//DRCWB(ScriptObject*)  m_pDummyObject;
		DRCWB(SoundTransformObject*) m_pSoundTransformObject;
		bool AS3_buttonMode_get();
		void AS3_buttonMode_set(bool);
		DisplayObjectObject* AS3_dropTarget_get();
		GraphicsObject* AS3_graphics_get();
		SpriteObject* AS3_hitArea_get();
		void AS3_hitArea_set(SpriteObject*);
		SoundTransformObject* AS3_soundTransform_get();
		void AS3_soundTransform_set(SoundTransformObject*);
		bool AS3_useHandCursor_get();
		void AS3_useHandCursor_set(bool);
		void AS3_constructor();
		void AS3_startDrag(bool,RectangleObject*);
		void AS3_stopDrag();

		//new adds
		void AS3_startTouchDrag(int touchPointID, bool lockCenter, RectangleObject *pBounds);
		void AS3_stopTouchDrag(int touchPointID);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SpriteObjectSlots m_slots_SpriteObject;
};}
#endif