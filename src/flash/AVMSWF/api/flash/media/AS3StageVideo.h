#ifndef _AS3StageVideo_
#define _AS3StageVideo_

#include "AS3EventDispatcher.h"

#include "AS3Point.h"
#include "AS3Rectangle.h"
#include "AS3NetStream.h"

namespace avmplus{namespace NativeID{
class StageVideoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class StageVideoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CameraObject;
	class StageVideoClass : public ClassClosure
	{
	public:
		StageVideoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageVideoClassSlots m_slots_StageVideoClass;
};
class StageVideoObject : public EventDispatcherObject
{
	public:
		StageVideoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		ObjectVectorObject* AS3_colorSpaces_get();
		int AS3_depth_get();
		void AS3_depth_set(int depth);
		PointObject* AS3_pan_get();
		void AS3_pan_set(PointObject *pPan);
		int AS3_videoHeight_get();
		int AS3_videoWidth_get();
		RectangleObject* AS3_viewPort_get();
		void AS3_viewPort_set(RectangleObject *pViewPort);
		PointObject* AS3_zoom_get();
		void AS3_zoom_set(PointObject *pZoom);

		void AS3_attachNetStream(NetStreamObject *pNetStream);

		//new adds 11
		void AS3_attachCamera(CameraObject* pTheCamera);
		//new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageVideoObjectSlots m_slots_StageVideoObject;
};}
#endif