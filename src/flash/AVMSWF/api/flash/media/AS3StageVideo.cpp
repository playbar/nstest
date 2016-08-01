#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageVideo.h"
namespace avmshell{
StageVideoClass::StageVideoClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StageVideoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageVideoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageVideoObject::StageVideoObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ObjectVectorObject* StageVideoObject::AS3_colorSpaces_get()
{
	return NULL;
}
int StageVideoObject::AS3_depth_get()
{
	return 0;
}
void StageVideoObject::AS3_depth_set(int depth)
{

}
PointObject* StageVideoObject::AS3_pan_get()
{
	return NULL;
}
void StageVideoObject::AS3_pan_set(PointObject *pPan)
{

}
int StageVideoObject::AS3_videoHeight_get()
{
	return 0;
}
int StageVideoObject::AS3_videoWidth_get()
{
	return 0;
}
RectangleObject* StageVideoObject::AS3_viewPort_get()
{
	return NULL;
}
void StageVideoObject::AS3_viewPort_set(RectangleObject *pViewPort)
{

}
PointObject* StageVideoObject::AS3_zoom_get()
{
	return NULL;
}
void StageVideoObject::AS3_zoom_set(PointObject *pZoom)
{

}

void StageVideoObject::AS3_attachNetStream(NetStreamObject *pNetStream)
{

}
//new adds 11
void StageVideoObject::AS3_attachCamera(CameraObject* pTheCamera)
{
	LOGWHERE();
}
//new adds 11 end
}