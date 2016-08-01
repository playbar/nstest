#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3PrintJob.h"
#include "AS3PrintJobOrientation.h"
namespace avmshell{

PrintJobClass::PrintJobClass(VTable*vtable):ClassClosure(vtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())PrintJobObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* PrintJobClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) PrintJobObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

//new adds
bool PrintJobClass::AS3_active_get()
{
	return false;
}
ObjectVectorObject* PrintJobClass::AS3_printers_get()
{
	return NULL;
}
bool PrintJobClass::AS3_supportsPageSetupDialog_get()
{
	return false;
}
//new adds end

PrintJobObject::PrintJobObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	ShellToplevel*t=(ShellToplevel*)toplevel();
	m_strOrient=t->getPrintJobOrientationClass()->getSlotLANDSCAPE();
	m_nPageWidth=12;
	m_nPageHeight=8;
	m_nPaperWidth=12;
	m_nPaperHeight=8;
}
//////////////////////////////////////////////////////////
//Native Method start...

//void PrintJobObject::AS3_addPage(SpriteObject*,RectangleObject*,PrintJobOptionObject*,int)
//{
//	//Add your act code here...
//}
//
//void PrintJobObject::AS3_send()
//{
//	//Add your act code here...
//}
//
//bool PrintJobObject::AS3_start()
//{
//	//Add your act code here...
//	return false;
//}

//new adds
int PrintJobObject::AS3_copies_get()
{
	return 0;
}
void PrintJobObject::AS3_copies_set(int copies)
{

}
int PrintJobObject::AS3_firstPage_get()
{
	return 0;
}
bool PrintJobObject::AS3_isColor_get()
{
	return false;
}
//bool PrintJobObject::AS3_isSupported_get()
//{
//	return false;
//}
Stringp PrintJobObject::AS3_jobName_get()
{
	return NULL;
}
void PrintJobObject::AS3_jobName_set(Stringp pJobName)
{

}
int PrintJobObject::AS3_lastPage_get()
{
	return 0;
}
double PrintJobObject::AS3_maxPixelsPerInch_get()
{
	return 0;
}
RectangleObject* PrintJobObject::AS3_paperArea_get()
{
	return NULL;
}
RectangleObject* PrintJobObject::AS3_printableArea_get()
{
	return NULL;
}
Stringp PrintJobObject::AS3_printer_get()
{
	return NULL;
}
void PrintJobObject::AS3_printer_set(Stringp pPrinter)
{

}

void PrintJobObject::AS3_selectPaperSize(Stringp pPaperSize)
{
	
}
bool PrintJobObject::AS3_showPageSetupDialog()
{
	return false;
}
bool PrintJobObject::AS3_start2(PrintUIOptionsObject *uiOptions, bool showPrintDialog)
{
	return false;
}
void PrintJobObject::AS3_terminate()
{

}
//new adds end

}