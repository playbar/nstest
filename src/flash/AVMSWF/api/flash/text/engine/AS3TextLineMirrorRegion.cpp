#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextLineMirrorRegion.h"
namespace avmshell{
TextLineMirrorRegionClass::TextLineMirrorRegionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextLineMirrorRegionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextLineMirrorRegionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextLineMirrorRegionObject::TextLineMirrorRegionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
RectangleObject* TextLineMirrorRegionObject::AS3_bounds_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

ContentElementObject* TextLineMirrorRegionObject::AS3_element_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

EventDispatcherObject* TextLineMirrorRegionObject::AS3_mirror_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

TextLineMirrorRegionObject* TextLineMirrorRegionObject::AS3_nextRegion_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

TextLineMirrorRegionObject* TextLineMirrorRegionObject::AS3_previousRegion_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

TextLineObject* TextLineMirrorRegionObject::AS3_textLine_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

}