#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextJustifier.h"
namespace avmshell{
TextJustifierClass::TextJustifierClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextJustifierClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextJustifierObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

/*TextJustifierObject* TextJustifierClass::CreateObject(Stringp strLocal,Stringp strLineJust)
{
	TextJustifierObject*pObj=(TextJustifierObject*)createInstance(ivtable(),prototype);
	Atom atoms[]={this->atom(),strLocal?strLocal->atom():nullStringAtom,
							  strLineJust?strLineJust->atom():nullStringAtom,
				};
	pObj->construct(2,atoms);
	return pObj;
}*/

TextJustifierObject::TextJustifierObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}


//////////////////////////////////////////////////////////
//Native Method start...
//AvmBox TextJustifierObject::AS3_getJustifierForLocale(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

Stringp TextJustifierObject::AS3_lineJustification_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void TextJustifierObject::AS3_lineJustification_set(Stringp lineJustification)
{
	//Add your act code here...
}

Stringp TextJustifierObject::AS3_locale_get()
{
	//Add your act code here...
	return m_strLocal;//Modify this please!
}

void TextJustifierObject::AS3_setLocal(Stringp str)
{
	m_strLocal=str;
}

//AS3 contructor function..
//AvmBox TextJustifierObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox TextJustifierObject::AS3_clone(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}