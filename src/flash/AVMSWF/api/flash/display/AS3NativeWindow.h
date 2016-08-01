#ifndef _AS3NativeWindow_
#define _AS3NativeWindow_

#include "AS3EventDispatcher.h"

#include "AS3Stage.h"
#include "AS3Point.h"
#include "AS3Rectangle.h"
#include "AS3NativeMenu.h"
#include "AS3NativeWindowInitOptions.h"
#include "AS3NativeWindowRenderMode.h"

namespace avmplus{namespace NativeID{
class NativeWindowClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NativeWindowObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeWindowClass : public ClassClosure
	{
	public:
		NativeWindowClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowClassSlots m_slots_NativeWindowClass;
};
class StageObject;
class NativeMenuObject;
class NativeWindowInitOptionsObject;
class NativeWindowObject : public EventDispatcherObject
{
	public:
		NativeWindowObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		DRCWB(NativeWindowInitOptionsObject*) m_pInitOptions;
		bool AS3_isSupported_get();
		bool AS3_supportsMenu_get();
		bool AS3_supportsNotification_get();
		bool AS3_supportsTransparency_get();
		PointObject* AS3_systemMaxSize_get();
		PointObject* AS3_systemMinSize_get();
		bool AS3_active_get();
		bool AS3_alwaysInFront_get();
		void AS3_alwaysInFront_set(bool alwaysInFront);
		RectangleObject* AS3_bounds_get();
		void AS3_bounds_set(RectangleObject *pBounds);
		bool AS3_closed_get();
		Stringp AS3_displayState_get();
		double AS3_height_get();
		void AS3_height_set(double height);
		bool AS3_maximizable_get();
		PointObject* AS3_maxSize_get();
		void AS3_maxSize_set(PointObject *pMaxSize);
		NativeMenuObject* AS3_menu_get();
		void AS3_menu_set(NativeMenuObject *pMenu);
		bool AS3_minimizable_get();
		PointObject* AS3_minSize_get();
		void AS3_minSize_set(PointObject *pMinSize);
		NativeWindowObject* AS3_owner_get();
		bool AS3_resizable_get();
		StageObject* AS3_stage_get();
		Stringp AS3_systemChrome_get();
		Stringp AS3_title_get();
		void AS3_title_set(Stringp pTitle);
		bool AS3_transparent_get();
		Stringp AS3_type_get();
		bool AS3_visible_get();
		void AS3_visible_set(bool visible);
		double AS3_width_get();
		void AS3_width_set(double width);
		double AS3_x_get();
		void AS3_x_set(double x);
		double AS3_y_get();
		void AS3_y_set(double y);

		void AS3_constructor(NativeWindowInitOptionsObject *pInitOptions);
		void AS3_activate();
		void AS3_close();
		PointObject* AS3_globalToScreen(PointObject *pGlobalPoint);
		AvmBox AS3_listOwnedWindows(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_maximize();
		void AS3_minimize();
		void AS3_notifyUser(Stringp pType);
		bool AS3_orderInBackOf(NativeWindowObject *pWindow);
		bool AS3_orderInFrontOf(NativeWindowObject *pWindow);
		bool AS3_orderToBack();
		bool AS3_orderToFront();
		void AS3_restore();
		bool AS3_startMove();
		bool AS3_startResize(Stringp pEdgeOrCorner);

		//new adds 11
		Stringp AS3_renderMode_get();
	
		//new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowObjectSlots m_slots_NativeWindowObject;
};}
#endif