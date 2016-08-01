#ifndef _AS3Stage3D_
#define _AS3Stage3D_

//#include "AS3Context3D.h"
//#include "AS3EventDispatcher.h"

namespace avmplus
{
	namespace nativeID
	{
		class Stage3DClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
		private:
		};
		class Stage3DObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}

namespace avmshell
{
	class Stage3DClass : public ClassClosure
	{
	public:
		Stage3DClass (VTable * vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::nativeID::Stage3DClassSlots m_slots_Stage3DClass;
	};
	class Stage3DObject : public EventDispatcherObject 
	{
	public:
		Stage3DObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//		DRCWB(Context3DObject*) m_context3D;
		bool                    m_visible;
		double                  m_x,m_y;
	public:
		//		Context3DObject* AS3_context3D_get();

		bool AS3_visible_get();
		void AS3_visible_set(bool visible);

		double AS3_x_get();
		void AS3_x_set(double x);

		double AS3_y_get();
		void AS3_y_set(double y);

		//		void AS3_requestContext3D(String context3DRenderMode, String profile);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::nativeID::Stage3DObjectSlots m_slots_Stage3DObject;
	};
}

#endif