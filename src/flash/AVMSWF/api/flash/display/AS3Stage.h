#ifndef _AS3Stage_
#define _AS3Stage_

#include "AS3NativeWindow.h"
#include "AS3DisplayObjectContainer.h"
#include "AS3Rectangle.h"

namespace avmplus{namespace NativeID{
class StageClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class StageObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DisplayObjectObject;
	class StageObject;
	class LoaderInfoObject;
	class InteractiveObjectObject;
	class StageClass : public ClassClosure
	{
	public:
		StageClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		StageObject* CreateStage(DisplayObjectObject*);

		//new adds
		
		bool AS3_supportsOrientationChange_get();
		//new adds end
	private:
		bool m_bSupportsOrientationChange;

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageClassSlots m_slots_StageClass;
};
class NativeWindowObject;
class StageObject : public DisplayObjectContainerObject
{
	public:
		StageObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual EventDispatcherObject* GetParentObject(bool bSeek){return NULL;}
	public:		
		DisplayObjectObject* AS3_root_get();
		LoaderInfoObject* AS3_loaderinfo_get();
		DRCWB(Stringp) m_strAlign;
		Stringp AS3_align_get()
		{	return m_strAlign;	}
		void AS3_align_set(Stringp s){m_strAlign=s;}
		Stringp AS3_displayState_get();
		void AS3_displayState_set(Stringp);
		InteractiveObjectObject* AS3_focus_get();
		void AS3_focus_set(InteractiveObjectObject*);
		double AS3_frameRate_get();
		void AS3_frameRate_set(double);
		double AS3_height_get();
		void AS3_height_set(double);
		bool AS3_mouseChildren_get();
		void AS3_mouseChildren_set(bool);
		int AS3_numChildren_get();
		Stringp AS3_quality_get();
		void AS3_quality_set(Stringp);
		Stringp AS3_scaleMode_get();
		void AS3_scaleMode_set(Stringp);
		bool AS3_showDefaultContextMenu_get();
		void AS3_showDefaultContextMenu_set(bool);
		bool AS3_stageFocusRect_get();
		void AS3_stageFocusRect_set(bool);
		int AS3_stageHeight_get();
		void AS3_stageHeight_set(int);
		int AS3_stageWidth_get();
		void AS3_stageWidth_set(int);
		bool AS3_tabChildren_get();
		void AS3_tabChildren_set(bool);
		TextSnapshotObject* AS3_textSnapshot_get();
		double AS3_width_get()
		{
			return DisplayObjectContainerObject::AS3_width_get();
		}
		void AS3_width_set(double v)
		{
			DisplayObjectContainerObject::AS3_width_set(v);
		}
		DisplayObjectObject* AS3_addChild(DisplayObjectObject*obj)
		{
			return DisplayObjectContainerObject::AS3_addChild(obj);
		}
		DisplayObjectObject* AS3_addChildAt(DisplayObjectObject*obj,int v)
		{
			return DisplayObjectContainerObject::AS3_addChildAt(obj,v);
		}
		void AS3_addEventListener(Stringp type,FunctionObject*fun,bool uc,int pi,bool uw)
		{
			return DisplayObjectContainerObject::AS3_addEventListener(type,fun,uc,pi,uw);
		}
		void AS3_dispatchEvent(EventObject*pEvent)
		{
			DisplayObjectContainerObject::AS3_dispatchEvent(pEvent);
		}
		bool AS3_hasEventListener(Stringp s)
		{
			return DisplayObjectContainerObject::AS3_hasEventListener(s);
		}
		void AS3_invalidate();
		bool AS3_isFocusInaccessible(){return true;}
		DisplayObjectObject* AS3_removeChildAt(int index)
		{
			return DisplayObjectContainerObject::AS3_removeChildAt(index);
		}
		void AS3_setChildIndex(DisplayObjectObject*obj,int index)
		{
			DisplayObjectContainerObject::AS3_setChildIndex(obj,index);
		}
		void AS3_swapChildrenAt(int i1,int i2)
		{
			DisplayObjectContainerObject::AS3_swapChildrenAt(i1,i2);
		}
		bool AS3_willTrigger(Stringp s)
		{
			return DisplayObjectContainerObject::AS3_willTrigger(s);
		}

		//new adds
		bool AS3_allowsFullScreen_get();
		bool AS3_autoOrients_get();
		void AS3_autoOrients_set(bool autoOrients);
		uint32_t AS3_color_get();
		void AS3_color_set(uint32_t color);
		Stringp AS3_colorCorrection_get();
		void AS3_colorCorrection_set(Stringp pColorCorrection);
		Stringp AS3_colorCorrectionSupport_get();
		Stringp AS3_deviceOrientation_get();
		uint32_t AS3_fullScreenHeight_get();
		RectangleObject* AS3_fullScreenSourceRect_get();
		void AS3_fullScreenSourceRect_set(RectangleObject *pFullScreenSourceRect);
		uint32_t AS3_fullScreenWidth_get();
		NativeWindowObject* AS3_nativeWindow_get();
		Stringp AS3_orientation_get();
		RectangleObject* AS3_softKeyboardRect_get();
		ObjectVectorObject* AS3_stageVideos_get();
		ObjectVectorObject* AS3_supportedOrientations_get();
		bool AS3_wmodeGPU_get();

		void AS3_assignFocus(InteractiveObjectObject pObjectToFocus, Stringp pDirection);
		void AS3_setAspectRatio(Stringp pNewAspectRatio);
		void AS3_setOrientation(Stringp pNewOrientation);
		//new adds end

		//new adds 11
		 bool AS3_allowsFullScreenInteractive_get();
		 
		 void* AS3_constructor_get();
		 void AS3_constructor_set(void* value);
		 
		 double AS3_contentsScaleFactor_get();
		 
		 bool AS3_mouseLock_get();
		 void AS3_mouseLock_set(bool bLock);
		 
		 DRCWB(ObjectVectorObject*) m_stage3Ds;
		 ObjectVectorObject* AS3_stage3Ds_get();
		 //new adds 11 end
	private:
		bool m_bAllowsFullScreenInteractive;
		void* m_pConstructor;
		double m_numContentsScaleFactor;
		bool m_bMouseLock;

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageObjectSlots m_slots_StageObject;
};}
#endif