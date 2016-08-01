#ifndef _AS3StageWebView_
#define _AS3StageWebView_

#include "AS3EventDispatcher.h"

#include "AS3Stage.h"
#include "AS3BitmapData.h"

namespace avmplus{namespace NativeID{
class StageWebViewClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class StageWebViewObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StageWebViewClass : public ClassClosure
	{
	public:
		StageWebViewClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageWebViewClassSlots m_slots_StageWebViewClass;
};
class StageWebViewObject : public EventDispatcherObject
{
	public:
		StageWebViewObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_isSupported_get();
		bool AS3_isHistoryBackEnabled_get();
		bool AS3_isHistoryForwardEnabled_get();
		Stringp AS3_location_get();
		StageObject* AS3_stage_get();
		void AS3_stage_set(StageObject *pStage);
		Stringp AS3_title_get();
		RectangleObject* AS3_viewPort_get();
		void AS3_viewPort_set(RectangleObject *pViewPort);

		void AS3_constructor();
		void AS3_assignFocus(Stringp pDirection);
		void AS3_dispose();
		void AS3_drawViewPortToBitmapData(BitmapDataObject *pBitmap);
		void AS3_historyBack();
		void AS3_historyForward();
		void AS3_loadString(Stringp pText, Stringp pMimeType);
		void AS3_loadURL(Stringp pURL);
		void AS3_reload();
		void AS3_stop();

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageWebViewObjectSlots m_slots_StageWebViewObject;
};}
#endif