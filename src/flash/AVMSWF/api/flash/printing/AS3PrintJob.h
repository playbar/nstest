#ifndef _AS3PrintJob_
#define _AS3PrintJob_

#include "AS3PrintUIOptions.h"

namespace avmplus{namespace NativeID{
class PrintJobClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	uint32_t	kGetPaperHeight;
	uint32_t	ukGetPaperWidth;
	uint32_t	kGetPageHeight;
	uint32_t	kGetPageWidth;
	uint32_t	kGetOrientation;
	uint32_t	kStart;
	uint32_t	kAddPage;
	uint32_t	kSend;
private:};
class PrintJobObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SpriteObject;
	class RectangleObject;
	class PrintJobOptionObject;
	class PrintJobClass : public ClassClosure
	{
	public:
		PrintJobClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		//new adds
		bool AS3_active_get();
		ObjectVectorObject* AS3_printers_get();
		bool AS3_supportsPageSetupDialog_get();
		//new adds end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintJobClassSlots m_slots_PrintJobClass;
};
class PrintJobObject : public ScriptObject
{
	public:
		PrintJobObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		DRCWB(Stringp) m_strOrient;
		int m_nPageWidth,m_nPageHeight;
		int m_nPaperWidth,m_nPaperHeight;
		//Stringp AS3_orientation_get(){return m_strOrient;}
		//int AS3_pageHeight_get(){return m_nPageHeight;}
		//int AS3_pageWidth_get(){return m_nPageWidth;}
		//int AS3_paperHeight_get(){return m_nPaperHeight;}
		//int AS3_paperWidth_get(){return m_nPaperWidth;}
		//void AS3_constructor(){}
		//void AS3_addPage(SpriteObject*,RectangleObject*,PrintJobOptionObject*,int);
		//void AS3_send();
		//bool AS3_start();

		bool AS3_invoke(uint32_t index, ArrayObject *pArgs){return false;}
		bool AS3_invoke1(Atom index, ArrayObject *pArgs){return false;}

		//new adds
		int AS3_copies_get();
		void AS3_copies_set(int copies);
		int AS3_firstPage_get();
		bool AS3_isColor_get();
//		bool AS3_isSupported_get();
		Stringp AS3_jobName_get();
		void AS3_jobName_set(Stringp pJobName);
		int AS3_lastPage_get();
		double AS3_maxPixelsPerInch_get();
		RectangleObject* AS3_paperArea_get();
		RectangleObject* AS3_printableArea_get();
		Stringp AS3_printer_get();
		void AS3_printer_set(Stringp pPrinter);

		void AS3_selectPaperSize(Stringp pPaperSize);
		bool AS3_showPageSetupDialog();
		bool AS3_start2(PrintUIOptionsObject *uiOptions, bool showPrintDialog);
		void AS3_terminate();
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PrintJobObjectSlots m_slots_PrintJobObject;
};}
#endif