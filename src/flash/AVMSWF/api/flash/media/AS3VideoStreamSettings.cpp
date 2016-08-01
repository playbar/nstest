#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3VideoStreamSettings.h"
namespace avmshell
{
	VideoStreamSettingsClass::VideoStreamSettingsClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* VideoStreamSettingsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) VideoStreamSettingsObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	VideoStreamSettingsObject::VideoStreamSettingsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_iBandwidth = -1;
		m_strCodec = ((ShellToplevel*)toplevel())->getVideoCodecClass()->getSlotSORENSON();
		m_nFps = -1;
		m_iHeight = -1;
		m_iKeyFrameInterval = -1;
		m_iQuality = -1;
		m_iWidth = -1;
		m_pCamera = NULL;
	}
	//////////////////////////////////////////////////////////
	CameraObject* VideoStreamSettingsObject::GetCamera()
	{
		return ((ShellToplevel*)toplevel())->getCameraClass()->AS3_getCamera(NULL);
	}
	int VideoStreamSettingsObject::AS3_bandwidth_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		//if(m_pCamera == NULL)
		//	m_pCamera=GetCamera();
		//if(m_pCamera)
		//	return m_pCamera->AS3_bandwidth_get();
		return m_iBandwidth;
	}
	
	Stringp VideoStreamSettingsObject::AS3_codec_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strCodec;//Stringp("Sorenson");
	}
	
	double VideoStreamSettingsObject::AS3_fps_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_nFps;
	}
	
	int VideoStreamSettingsObject::AS3_height_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_iHeight;
	}
	
	int VideoStreamSettingsObject::AS3_keyFrameInterval_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		//if(m_pCamera == NULL)
		//	m_pCamera=GetCamera();
		//if(m_pCamera)
		//	return m_pCamera->AS3_keyFrameInterval_get();
		return m_iKeyFrameInterval;
	}
	
	int VideoStreamSettingsObject::AS3_quality_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		//if(m_pCamera == NULL)
		//	m_pCamera=GetCamera();
		//if(m_pCamera)
		//	return m_pCamera->AS3_quality_get();
		return m_iQuality;
	}
	
	int VideoStreamSettingsObject::AS3_width_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		//if(m_pCamera == NULL)
		//	m_pCamera=GetCamera();
		//if(m_pCamera)
		//	return m_pCamera->AS3_width_get();
		return m_iWidth;
	}
	
	
	void VideoStreamSettingsObject::AS3_setKeyFrameInterval(int keyFrameInterval)
	{
		//LOGWHERE();
		//Add your construct code here...
		//if(m_pCamera == NULL)
		//	m_pCamera=GetCamera();
		//if(m_pCamera)
		//	m_pCamera->AS3_setKeyFrameInterval(keyFrameInterval);
		if(keyFrameInterval<1)
			m_iKeyFrameInterval = 1;
		else if(keyFrameInterval>300)
			m_iKeyFrameInterval = 300;
		else 
			m_iKeyFrameInterval = keyFrameInterval;
	}
	void VideoStreamSettingsObject::AS3_setMode(int width, int height, double fps)
	{
		//LOGWHERE();
		//Add your construct code here...
		//if(m_pCamera == NULL)
		//	m_pCamera=GetCamera();
		if (width<1)
			m_iWidth = 1;
		else if (width>10000)
			m_iWidth = 10000;
		else
			m_iWidth = width;

		if (height<1)
			m_iHeight = 1;
		else if (height>10000)
			m_iHeight = 10000;
		else
			m_iHeight = height;

		if (fps<0)
			m_nFps = 0;
		else if (fps>10000)
			m_nFps = 10000;
		else
			m_nFps = fps;
		//if(m_pCamera)
		//	m_pCamera->AS3_setMode(width, height, fps, true);
	}
	void VideoStreamSettingsObject::AS3_setQuality(int bandwidth, int quality)
	{
		//LOGWHERE();
		//Add your construct code here...
		//if(m_pCamera == NULL)
		//	m_pCamera=GetCamera();
		if (bandwidth<0)
			m_iBandwidth = 0;
		else 
			m_iBandwidth = bandwidth;
		if (quality<0)
			m_iQuality = 0;
		else if (quality>100)
			m_iQuality = 100;
		else 
			m_iQuality = quality;
		//if(m_pCamera)
		//	m_pCamera->AS3_setQuality(bandwidth, quality);
	}
}