#ifndef _AS3StageText_
#define _AS3StageText_

#include "AS3EventDispatcher.h"
#include "AS3AutoCapitalize.h"
#include "AS3FontPosture.h"
#include "AS3FontWeight.h"
#include "AS3StageTextInitOptions.h"
#include "AS3ReturnKeyLabel.h"
#include "AS3SoftKeyboardType.h"
#include "AS3TextFormatAlign.h"
#include "AS3Rectangle.h"
#include "AS3BitmapData.h"
#include "AS3TextField.h"
#include "edittext.h"
#include "AS3StageTextInitOptions.h"

namespace avmplus
{
	namespace NativeID
	{
		class StageTextClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

		private:
		};
		class StageTextObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class StageTextClass : public ClassClosure//EventClass
	{
	public:
		StageTextClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageTextClassSlots m_slots_StageTextClass;
	};
	class StageTextObject : public EventDispatcherObject
	{
	public:
		StageTextObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	private:
		inline EditText* GetEditText();

	public:
		void InitObject();
		StageTextInitOptionsObject* AS3GetInitOptions();
		void AS3SetInitOptions(StageTextInitOptionsObject*);

		//struct _XSObject* m_pObject;//Set your data!!
		DRCWB(TextFieldObject*) m_pTextField;
		DRCWB(StageTextInitOptionsObject*) m_pOptions;
		Stringp m_strAutoCapitalize;
		bool m_bAutoCorrect;
		bool m_bEditable;
		Stringp m_strFontFamily;
		//Stringp m_strFontPosture;
		//int m_iFontSize;
		//Stringp m_strFontWeight;
		Stringp m_strLocale;
		//DRCWB(Stringp) m_strRestrict;
		Stringp m_strReturnKeyLabel;
		//int m_iSelectionActiveIndex;
		//int m_iSelectionAnchorIndex;
		Stringp m_strSoftKeyboardType;
		DRCWB(StageObject*) m_pStage;
		Stringp m_strTextAlign;
		DRCWB(RectangleObject*) m_pViewPort;
		

		Stringp AS3_autoCapitalize_get();
		void AS3_autoCapitalize_set(Stringp s);
		
		bool AS3_autoCorrect_get();
		void AS3_autoCorrect_set(bool b);
		
		uint32_t AS3_color_get();
		void AS3_color_set(uint32_t value);
    
		bool AS3_displayAsPassword_get();
		void AS3_displayAsPassword_set(bool value);
    
        bool AS3_editable_get();
		void AS3_editable_set(bool value);
    
		Stringp AS3_fontFamily_get();
		void AS3_fontFamily_set(Stringp value);
	    
		Stringp AS3_fontPosture_get();
		void AS3_fontPosture_set(Stringp value);
	    
		int AS3_fontSize_get();
		void AS3_fontSize_set(int value);
	    
		Stringp AS3_fontWeight_get();
		void AS3_fontWeight_set(Stringp value);
	    
		Stringp AS3_locale_get();
		void AS3_locale_set(Stringp value);
	    
		int AS3_maxChars_get();
		void AS3_maxChars_set(int value);
	    
		bool AS3_multiline_get();
	    
		Stringp AS3_restrict_get();
		void AS3_restrict_set(Stringp value);
	    
		Stringp AS3_returnKeyLabel_get();
		void AS3_returnKeyLabel_set(Stringp keyLabel);
	    
		int AS3_selectionActiveIndex_get();
	    
		int AS3_selectionAnchorIndex_get();
	    
		Stringp AS3_softKeyboardType_get();
		void AS3_softKeyboardType_set(Stringp sof);
	    
		StageObject* AS3_stage_get();
		void AS3_stage_set(StageObject* stageObj);
	    
		Stringp AS3_text_get();
		void AS3_text_set(Stringp text);
	    
		Stringp AS3_textAlign_get();
		void AS3_textAlign_set(Stringp textAlign);
	    
		RectangleObject* AS3_viewPort_get();
		void AS3_viewPort_set(RectangleObject* rectangle);
	    
		bool AS3_visible_get();
		void AS3_visible_set(bool bVisible);
    

		void AS3_assignFocus();
		void AS3_dispose();
		void AS3_drawViewPortToBitmapData(BitmapDataObject* bitmap);
		void AS3_selectRange(int anchorIndex, int activeIndex);


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageTextObjectSlots m_slots_StageTextObject;
	};
}
#endif