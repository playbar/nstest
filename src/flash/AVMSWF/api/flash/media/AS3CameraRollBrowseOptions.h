#ifndef _AS3CameraRollBrowseOptions_
#define _AS3CameraRollBrowseOptions_

#include "AS3Rectangle.h"

namespace avmplus
{
	namespace NativeID
	{
		class CameraRollBrowseOptionsClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!


		private:
		};
		class CameraRollBrowseOptionsObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class CameraRollBrowseOptionsClass : public ClassClosure//EventClass
	{
	public:
		CameraRollBrowseOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraRollBrowseOptionsClassSlots m_slots_CameraRollBrowseOptionsClass;
	};
	class CameraRollBrowseOptionsObject : public ScriptObject
	{
	public:
		CameraRollBrowseOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

		double m_height, m_width;
		DRCWB(RectangleObject*) m_origin;
		double AS3_height_get();
		void AS3_height_set(double height);
	    
		RectangleObject* AS3_origin_get();
		void AS3_origin_set(RectangleObject* origin);
	    
		double AS3_width_get();
		void AS3_width_set(double width);


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraRollBrowseOptionsObjectSlots m_slots_CameraRollBrowseOptionsObject;
	};
}
#endif