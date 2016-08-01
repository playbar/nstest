#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetStreamInfo.h"
namespace avmshell{
NetStreamInfoClass::NetStreamInfoClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetStreamInfoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetStreamInfoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetStreamInfoObject::NetStreamInfoObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double NetStreamInfoObject::AS3_audioBufferByteLength_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_audioBufferLength_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_audioByteCount_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_audioBytesPerSecond_get()
//{	
//	return 0;
//}
//double NetStreamInfoObject::AS3_audioLossRate_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_byteCount_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_currentBytesPerSecond_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_dataBufferByteLength_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_dataBufferLength_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_dataByteCount_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_dataBytesPerSecond_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_droppedFrames_get()
//{
//	return 0;
//}
//bool NetStreamInfoObject::AS3_isLive_get()
//{
//	return false;
//}
//double NetStreamInfoObject::AS3_maxBytesPerSecond_get()
//{
//	return 0;
//}
//Atom NetStreamInfoObject::AS3_metaData_get()
//{
//	return kAvmThunkUndefined;
//}
//double NetStreamInfoObject::AS3_playbackBytesPerSecond_get()
//{
//	return 0;
//}
//Stringp NetStreamInfoObject::AS3_resourceName_get()
//{
//	return NULL;
//}
//double NetStreamInfoObject::AS3_SRTT_get()
//{
//	return 0;
//}
//Stringp NetStreamInfoObject::AS3_uri_get()
//{
//	return NULL;
//}
//double NetStreamInfoObject::AS3_videoBufferByteLength_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_videoBufferLength_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_videoByteCount_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_videoBytesPerSecond_get()
//{
//	return 0;
//}
//double NetStreamInfoObject::AS3_videoLossRate_get()
//{
//	return 0;
//}
//Atom NetStreamInfoObject::AS3_xmpData_get()
//{
//	return kAvmThunkUndefined;
//}
//
//Stringp NetStreamInfoObject::AS3_toString()
//{
//	return NULL;
//}

}