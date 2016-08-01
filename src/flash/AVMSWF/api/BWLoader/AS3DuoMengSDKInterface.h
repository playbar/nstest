#ifndef _AS3DuoMengSDKInterface_
#define _AS3DuoMengSDKInterface_

class XDrawDib;
class SBitmapCore;
#include "AS3DisplayObject.h"

namespace avmplus
{
	namespace NativeID
	{
		class DuoMengSDKInterfaceClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
		private:
		};
		class DuoMengSDKInterfaceObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
		private:
		};
	}
}

namespace avmshell
{
	class RectangleObject;
	class PointObject;
	class BitmapFilterObject;
	class DuoMengSDKInterfaceObject;
	class ColorTransformObject;
	class MatrixObject;
	class ByteArrayObject;
	class BitmapDataObject;
	class DisplayObjectObject;

	class DuoMengSDKInterfaceClass : public ClassClosure
	{
	public:
		DuoMengSDKInterfaceClass(VTable *vtable);
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);
		DuoMengSDKInterfaceObject* AS3_GetDuoMengSDKInterface();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DuoMengSDKInterfaceClassSlots m_slots_DuoMengSDKInterfaceClass;
	};

	class DuoMengSDKInterfaceObject : public ScriptObject
	{
	public:
		DuoMengSDKInterfaceObject(VTable* vtable, ScriptObject* proto, int capacity);
		virtual void Release();
		virtual void SetObject(void*pObject);

	public:
		
		 //调用积分墙列表
 		 int AS3_presentOfferWall();
 		 

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DuoMengSDKInterfaceObjectSlots m_slots_DuoMengSDKInterfaceObject;
	};
}

#endif
