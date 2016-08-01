#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageWebView.h"
namespace avmshell{
StageWebViewClass::StageWebViewClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StageWebViewClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageWebViewObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageWebViewObject::StageWebViewObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool StageWebViewObject::AS3_isSupported_get()
{
	return false;
}
bool StageWebViewObject::AS3_isHistoryBackEnabled_get()
{
	return false;
}
bool StageWebViewObject::AS3_isHistoryForwardEnabled_get()
{
	return false;
}
Stringp StageWebViewObject::AS3_location_get()
{
	return NULL;
}
StageObject* StageWebViewObject::AS3_stage_get()
{
	return NULL;
}
void StageWebViewObject::AS3_stage_set(StageObject *pStage)
{

}
Stringp StageWebViewObject::AS3_title_get()
{
	return NULL;
}
RectangleObject* StageWebViewObject::AS3_viewPort_get()
{
	return NULL;
}
void StageWebViewObject::AS3_viewPort_set(RectangleObject *pViewPort)
{

}

void StageWebViewObject::AS3_constructor()
{

}
void StageWebViewObject::AS3_assignFocus(Stringp pDirection)
{

}
void StageWebViewObject::AS3_dispose()
{

}
void StageWebViewObject::AS3_drawViewPortToBitmapData(BitmapDataObject *pBitmap)
{

}
void StageWebViewObject::AS3_historyBack()
{

}
void StageWebViewObject::AS3_historyForward()
{

}
void StageWebViewObject::AS3_loadString(Stringp pText, Stringp pMimeType)
{

}
void StageWebViewObject::AS3_loadURL(Stringp pURL)
{

}
void StageWebViewObject::AS3_reload()
{

}
void StageWebViewObject::AS3_stop()
{

}

}