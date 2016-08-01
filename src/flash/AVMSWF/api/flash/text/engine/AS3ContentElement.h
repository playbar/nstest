#ifndef _AS3ContentElement_
#define _AS3ContentElement_

#include "AS3ElementFormat.h"
#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class ContentElementClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//GRAPHIC_ELEMENT : uint = 0xFDEF

	uint32_t GRAPHIC_ELEMENT;

private:};
class ContentElementObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	ScriptObject *userData;
private:};
}}
namespace avmshell{
class GroupElementObject;
class TextBlockObject;
class TextLineObject;

	class ContentElementClass : public ClassClosure
	{
	public:
		ContentElementClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContentElementClassSlots m_slots_ContentElementClass;
};
class ContentElementObject : public ScriptObject
{
	public:
		struct _CHARINFO
		{
			XU16 charCode;
			XU16 charGraph;
			XU16 charWidth;
		};
		struct _CONTENTINFO
		{
			int		m_nLength;
			_CHARINFO* pCharsInfo;
			struct _XTEXTFORMAT* pFormat;
		};
	public:
		enum {TYPE_NONE,TYPE_TEXT,TYPE_GRAPHIC,TYPE_GROUP};
		ContentElementObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
	public:
		int GetLineWidth(int nStart,int nEnd);
		int FindEndPos(int nStart,int& nWidth);
		int m_nContentType,m_nBeginIndex;
		DRCWB(ElementFormatObject*) m_pElementFormat;
		DRCWB(EventDispatcherObject*) m_pMirrorEvent;
		DRCWB(GroupElementObject*) m_pGroup;
		DRCWB(TextBlockObject*)	   m_pBlock;
		_CONTENTINFO*			   m_pCreationInfo;
		DRC(Stringp) m_strText;
		DRC(Stringp) m_strRawText;
		DRC(Stringp) m_strRotation;
	public:
		virtual void ReleaseCreationData();
		virtual void InitCreationData();
		virtual bool CreateContext(TextLineObject*pLine,int nID);
		//virtual ContentElementObject* GetNext(ContentElementObject*obj)=0;
		ContentElementObject* GetNextElement();
		ElementFormatObject* AS3_elementFormat_get();
		void AS3_elementFormat_set(ElementFormatObject *pElementFormat);
		EventDispatcherObject* AS3_eventMirror_get();
		void AS3_eventMirror_set(EventDispatcherObject *pEventMirror);
		GroupElementObject* AS3_groupElement_get();
		Stringp AS3_rawText_get();
		Stringp AS3_text_get();
		TextBlockObject* AS3_textBlock_get();
		int AS3_textBlockBeginIndex_get();
		Stringp AS3_textRotation_get();
		void AS3_textRotation_set(Stringp textRotation);
		//AvmBox AS3_userData_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_userData_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContentElementObjectSlots m_slots_ContentElementObject;
};}
#endif