#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
#include "sobject.h"
using namespace avmplus;
#include "AS3StageText.h"
namespace avmshell
{
	StageTextClass::StageTextClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* StageTextClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) StageTextObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	StageTextObject::StageTextObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
	{
		//Add your construct code here...
		//m_pObject = NULL;
		m_pOptions = NULL;

		InitObject();
		RectangleClass* pClass = ((ShellToplevel*)toplevel())->getRectangleClass();
		m_pViewPort = (RectangleObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		m_pViewPort->constructor(0.0, 0.0, 0.0, 0.0);
	}
	//////////////////////////////////////////////////////////



	void StageTextObject::InitObject()
	{
		TextFieldClass* pClass = ((ShellToplevel*)toplevel())->getTextFieldClass();
		m_pTextField = (TextFieldObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		m_pTextField->InitObject();
		m_pTextField->m_pObject->editText->m_pFormat->Init();
		if(m_pOptions)
			m_pTextField->AS3_multiline_set(m_pOptions->m_bMultiline);
		m_strAutoCapitalize = ((ShellToplevel*)toplevel())->getAutoCapitalizeClass()->getSlotNONE();
		m_bAutoCorrect = false;
		m_bEditable = true;
		m_strFontFamily = NULL;
		//m_strFontPosture = ((ShellToplevel*)toplevel())->getFontPostureClass()->getNORMAL();
		//m_iFontSize = 12;
		//m_strFontWeight = ((ShellToplevel*)toplevel())->getFontWeightClass()->getSlotNORMAL();
		m_strLocale = core()->newConstantStringLatin1("en");
		//m_strRestrict = NULL;
		m_strReturnKeyLabel = ((ShellToplevel*)toplevel())->getReturnKeyLabelClass()->getSlotDEFAULT();
		//m_iSelectionActiveIndex = 0;
		//m_iSelectionAnchorIndex = 0;
		m_strSoftKeyboardType = ((ShellToplevel*)toplevel())->getSoftKeyboardTypeClass()->getSlotDEFAULT();
		m_pStage = NULL;
		m_strTextAlign = ((ShellToplevel*)toplevel())->getTextFormatAlignClass()->getSlotSTART();
	}
	StageTextInitOptionsObject* StageTextObject::AS3GetInitOptions()
	{
		return m_pOptions;
	}
	void StageTextObject::AS3SetInitOptions(StageTextInitOptionsObject* pOption)
	{
		m_pOptions = pOption;
		m_pTextField->AS3_multiline_set(m_pOptions->m_bMultiline);
	}
	inline EditText* StageTextObject::GetEditText()
	{
		if(!m_pTextField || !m_pTextField->m_pObject) return NULL;  
		if(m_pTextField->m_pObject->character->type!=editTextChar&&
		  m_pTextField->m_pObject->character->type!=textLineChar) return NULL;
		return m_pTextField->m_pObject->editText;
		//return NULL;
	}
	Stringp StageTextObject::AS3_autoCapitalize_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strAutoCapitalize;//Stringp("none");
	}
	void StageTextObject::AS3_autoCapitalize_set(Stringp s)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_strAutoCapitalize = s;
		
	}
	
	bool StageTextObject::AS3_autoCorrect_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_bAutoCorrect;
	}
	void StageTextObject::AS3_autoCorrect_set(bool b)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_bAutoCorrect = b;
	}
	
	uint32_t StageTextObject::AS3_color_get()
	{
		//Add your construct code here...
		//GWHERE();
		if(m_pTextField)
			return m_pTextField->AS3_textColor_get();
		return 0;
	}
	void StageTextObject::AS3_color_set(uint32_t value)
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			return m_pTextField->AS3_textColor_set(value); 
	}

	bool StageTextObject::AS3_displayAsPassword_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		EditText*pEdit=GetEditText();
		if(pEdit)
			return pEdit->IsPassword();
		return false;
	}
	void StageTextObject::AS3_displayAsPassword_set(bool value)
	{
		//Add your construct code here...
		//LOGWHERE();
		EditText*pEdit=GetEditText();
		if(pEdit)
			pEdit->SetPassword(value);
	}

    bool StageTextObject::AS3_editable_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_bEditable;//false;
	}
	void StageTextObject::AS3_editable_set(bool bEditalbe)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_bEditable = bEditalbe;
	}

	Stringp StageTextObject::AS3_fontFamily_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strFontFamily?m_strFontFamily:core()->newConstantStringLatin1("Times New Roman"); //NULL
	}
	void StageTextObject::AS3_fontFamily_set(Stringp value)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_strFontFamily = value;
	}
    
	Stringp StageTextObject::AS3_fontPosture_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		FontPostureClass* pClass = ((ShellToplevel*)toplevel())->getFontPostureClass();
		EditText* pEdit = GetEditText();
		if(pEdit)
			if(pEdit->m_pFormat->italic)
				return pClass->getITALIC();
		return pClass->getNORMAL();//NativeID::FontPostureClassSlots::NORMAL;
	}
	void StageTextObject::AS3_fontPosture_set(Stringp s)
	{
		//Add your construct code here...
		//LOGWHERE();
		FontPostureClass* pClass = ((ShellToplevel*)toplevel())->getFontPostureClass();
		EditText* pEdit = GetEditText();
		if (s->equals(pClass->getITALIC()))
			pEdit->m_pFormat->italic = true;
		else 
			pEdit->m_pFormat->italic = false;
	}
    
	int StageTextObject::AS3_fontSize_get()
	{
		//Add your construct code here...
	//	LOGWHERE();
		EditText* pEdit = GetEditText();
		if(pEdit)
			return pEdit->m_pFormat->size?pEdit->m_pFormat->size:12;
		return 12;
	}
	void StageTextObject::AS3_fontSize_set(int value)
	{
		//Add your construct code here...
	//	LOGWHERE();
		EditText* pEdit = GetEditText();
		if(pEdit)
			pEdit->m_pFormat->size = value;
	}
    
	Stringp StageTextObject::AS3_fontWeight_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		FontWeightClass* pClass = ((ShellToplevel*)toplevel())->getFontWeightClass();
		EditText* pEdit = GetEditText();
		if(pEdit)
			if(pEdit->m_pFormat->Bold)
				return pClass->getSlotBOLD();
		return pClass->getSlotNORMAL();//NativeID::FontWeightClassSlots::NORMAL;
	}
	void StageTextObject::AS3_fontWeight_set(Stringp s)
	{
		//Add your construct code here...
		//LOGWHERE();
		FontWeightClass* pClass = ((ShellToplevel*)toplevel())->getFontWeightClass();
		EditText* pEdit = GetEditText();
		if(s->equals(pClass->getSlotBOLD()))
			pEdit->m_pFormat->Bold = true;
		else
			pEdit->m_pFormat->Bold = false;
	}
    
	Stringp StageTextObject::AS3_locale_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strLocale;     //   Stringp("en");
	}
	void StageTextObject::AS3_locale_set(Stringp s)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_strLocale = s;	
	}
    
	int StageTextObject::AS3_maxChars_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			return m_pTextField->AS3_maxChars_get();
		return 0;
	}
	void StageTextObject::AS3_maxChars_set(int value)
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			m_pTextField->AS3_maxChars_set(value);

	}
    
	bool StageTextObject::AS3_multiline_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pOptions)
			return m_pOptions->AS3_multiline_get();
		return false;
	}
    
	Stringp StageTextObject::AS3_restrict_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			return m_pTextField->AS3_restrict_get();
		return NULL;
	}
	void StageTextObject::AS3_restrict_set(Stringp restrict)
	{
		//Add your construct code here...
		if(m_pTextField)
			m_pTextField->AS3_restrict_set(restrict);
		//LOGWHERE();

	}
    
	Stringp StageTextObject::AS3_returnKeyLabel_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strReturnKeyLabel;//NativeID::ReturnKeyLabelClassSlots::DEFAULT;
	}
	void StageTextObject::AS3_returnKeyLabel_set(Stringp keyLabel)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_strReturnKeyLabel = keyLabel;
	}
    
	int StageTextObject::AS3_selectionActiveIndex_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			return m_pTextField->AS3_selectionBeginIndex_get();
		return 0;
	}
    
	int StageTextObject::AS3_selectionAnchorIndex_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			return m_pTextField->AS3_selectionEndIndex_get();
		return 0;
	}
    
	Stringp StageTextObject::AS3_softKeyboardType_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strSoftKeyboardType;//NativeID::SoftKeyboardTypeClassSlots::DEFAULT;
	}
	void StageTextObject::AS3_softKeyboardType_set(Stringp s)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_strSoftKeyboardType = s;
	}
    
	StageObject* StageTextObject::AS3_stage_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		//if(m_pTextField)
		//	return m_pTextField->AS3_stage_get();
		return m_pStage;
	}
	void StageTextObject::AS3_stage_set(StageObject* stageObj)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_pStage = stageObj;

	}
    
	Stringp StageTextObject::AS3_text_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			return m_pTextField->AS3_text_get();
		return core()->newConstantStringLatin1("");
	}
	void StageTextObject::AS3_text_set(Stringp text)
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			m_pTextField->AS3_text_set(text);
	}
    
	Stringp StageTextObject::AS3_textAlign_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		//EditText* pEdit = GetEditText();
		//TextFormatAlignClass* pClass = ((ShellToplevel*)toplevel())->getTextFormatAlignClass();
		//if(pEdit)
		//	return pClass->IDToString(pEdit->m_pFormat->align);
		return m_strTextAlign;//NativeID::TextFormatAlignClassSlots::START;
	}
	void StageTextObject::AS3_textAlign_set(Stringp textAlign)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_strTextAlign = textAlign;
		//TextFormatAlignClass* pClass = ((ShellToplevel*)toplevel())->getTextFormatAlignClass();
		//EditText* pEdit = GetEditText();
		//if(pEdit)
		//	pEdit->m_pFormat->align = pClass->StringToID(textAlign);
	}
    
	RectangleObject* StageTextObject::AS3_viewPort_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		
		return m_pViewPort; //(x=0, y=0, w=0, h=0)
	}
	void StageTextObject::AS3_viewPort_set(RectangleObject* rectangle)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_pViewPort = rectangle;
	}
    
	bool StageTextObject::AS3_visible_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			return m_pTextField->AS3_visible_get();//true;
		return true;
	}
	void StageTextObject::AS3_visible_set(bool bVisible)
	{
		//Add your construct code here...
		//GWHERE();
		if(m_pTextField)
			return m_pTextField->AS3_visible_set(bVisible);
	}


	void StageTextObject::AS3_assignFocus()
	{
		//Add your construct code here...
		//LOGWHERE();
		if(!m_bEditable)return;
		if(m_pTextField&&m_pTextField->m_pObject)
			m_pTextField->m_pObject->exflags|=_XSObject::XEN_FOCUSENABLE;
	}
	void StageTextObject::AS3_dispose()
	{
		//Add your construct code here...
		LOGWHERE();

	}
	void StageTextObject::AS3_drawViewPortToBitmapData(BitmapDataObject* bitmap)
	{
		//Add your construct code here...
		LOGWHERE();

	}
	void StageTextObject::AS3_selectRange(int anchorIndex, int activeIndex)
	{
		//Add your construct code here...
		//LOGWHERE();
		if(m_pTextField)
			m_pTextField->AS3_setSelection(anchorIndex, activeIndex);
	}
}