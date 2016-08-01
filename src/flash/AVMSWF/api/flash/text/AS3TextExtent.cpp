#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextExtent.h"
namespace avmshell{
TextExtentClass::TextExtentClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextExtentClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextExtentObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextExtentObject::TextExtentObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double TextExtentObject::AS3_ascent_get()
//{
//	return 0;
//}
//void TextExtentObject::AS3_ascent_set(double ascent)
//{
//
//}
//double TextExtentObject::AS3_descent_get()
//{
//	return 0;
//}
//void TextExtentObject::AS3_descent_set(double descent)
//{
//
//}
//double TextExtentObject::AS3_height_get()
//{
//	return 0;
//}
//void TextExtentObject::AS3_height_set(double height)
//{
//
//}
//double TextExtentObject::AS3_textFieldHeight_get()
//{
//	return 0;
//}
//void TextExtentObject::AS3_textFieldHeight_set(double textFieldHeight)
//{
//
//}
//double TextExtentObject::AS3_textFieldWidth_get()
//{
//	return 0;
//}
//void TextExtentObject::AS3_textFieldWidth_set(double textFieldWidth)
//{
//
//}
//double TextExtentObject::AS3_width_get()
//{
//	return 0;
//}
//void TextExtentObject::AS3_width_set(double width)
//{
//
//}
//
//void TextExtentObject::AS3_constructor(double width, double height, double textFieldWidth, double textFieldHeight, double ascent, double descent)
//{
//
//}

}