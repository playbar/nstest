#include "StdAfxflash.h"
#include "sobject.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextSnapshot.h"
namespace avmshell{
TextSnapshotClass::TextSnapshotClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextSnapshotObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextSnapshotClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextSnapshotObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

TextSnapshotObject* TextSnapshotClass::CreateObject(void*p)
{
	TextSnapshotObject*pObj=(TextSnapshotObject*)createInstance(ivtable(),prototype);
	pObj->m_pObject=(SObject*)p;
	//pObj->AS3_construtor();
	return pObj;
}

TextSnapshotObject::TextSnapshotObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
int TextSnapshotObject::AS3_charCount_get()
{
	//Add your act code here...
	return 0;//Modify this please!
}

int TextSnapshotObject::AS3_findText(int beginIndex, Stringp textToFind, bool caseSensitive)
{
	//Add your act code here...
	return 0;//Modify this please!
}

bool TextSnapshotObject::AS3_getSelected(int beginIndex, int endIndex)
{
	//Add your act code here...
	return false;//Modify this please!
}

Stringp TextSnapshotObject::AS3_getSelectedText(bool includeLineEndings)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp TextSnapshotObject::AS3_getText(int beginIndex, int endIndex, bool includeLineEndings)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

ArrayObject* TextSnapshotObject::AS3_getTextRunInfo(int beginIndex, int endIndex)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

double TextSnapshotObject::AS3_hitTestTextNearPos(double x, double y, double maxDistance)
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

void TextSnapshotObject::AS3_setSelectColor(uint32_t hexColor)
{
	//Add your act code here...
}

void TextSnapshotObject::AS3_setSelected(int beginIndex, int endIndex, bool select)
{
	//Add your act code here...
}

}