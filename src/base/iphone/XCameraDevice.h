// XCamera.h: interface for the XCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCAMERA_H__6B2852A6_51A4_4E34_997C_4A7B8193024B__INCLUDED_)
#define AFX_XCAMERA_H__6B2852A6_51A4_4E34_997C_4A7B8193024B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XString.h"
//#include <combase.h>
//#include <strmif.h>
//#include <uuids.h>
//#include <control.h>
//#include <wxlist.h>
//#include "SampleCGB.h"
//#include "crossbar.h"
struct IMoniker;
struct IFileSinkFilter;// *m_pSink;
struct IConfigAviMux;// *m_pConfigAviMux;
struct IBaseFilter;// *m_pRender;
struct IMediaEventEx;// *m_pME;
struct IAMDroppedFrames;// *m_pDF;
struct IVideoWindow;// *m_pVW;
struct IBaseFilter;// *m_pVCap, *m_pACap;	
struct IGraphBuilder;// *m_pFg;
struct IAMStreamConfig;// *m_pASC;      // for audio cap
struct IAMStreamConfig;//*m_pVSC;      // for video cap
struct IAMVideoCompression;// *m_pVC;
struct ICaptureGraphBuilder2;
struct IMediaControl;
struct ISampleGrabber;
class SampleGrabberCB;
class XDib;
class XCamera;
//class ISampleCaptureGraphBuilder;// *m_pBuilder;
//class CCrossbar;// m_pCrossbar;
/*typedef struct _CAPDEVICE
{
	IMoniker* pDevice;
	XString8  name;
}CAPDEVICE;*/
class XCameraDevice  
{
public:
    
	XCameraDevice();
	virtual ~XCameraDevice();
	int	m_nIndex;
public:
	static int GetDevices(XString8Array&list);
	bool Init(XPCWSTR name,XCamera*pCamera);
	void Release();
	bool Stop();
	bool Start();
	//void InitDrivers();
	XPCTSTR GetDeviceName(int id);
	/*{
		if(id>=m_devices.GetSize())
			return XNULL;
		return m_devices[0]->name;
	}*/
	XDib* GetDib();
protected:
	IMoniker* GetDeviceByName(XPCWSTR name);
	/*{
		int i;
		for(i=0;i<m_devices.GetSize();i++)
		{
			if(m_devices[i]->name==name)
				return m_devices[i]->pDevice;
		}
		return NULL;
	}*/
	//XArray<_CAPDEVICE*> m_devices;
	XU16 m_wachFriendlyName[120];
	//XString8Array m_devices;
	IMoniker *m_pVideo;
	IGraphBuilder *m_pGB;
	ICaptureGraphBuilder2* m_pCapture;
	IBaseFilter* m_pBF;
	IMediaControl* m_pMC;
	IVideoWindow* m_pVW;
	ISampleGrabber * m_pGrabber;
	SampleGrabberCB* m_pDib;
	//IAMStreamConfig *m_pConfig;
	//IFileSinkFilter *m_pSink;
	//IConfigAviMux *m_pConfigAviMux;
	//IBaseFilter *m_pRender;
	//IMediaEventEx *m_pME;
    //IAMDroppedFrames *m_pDF;
	//IVideoWindow *m_pVW;
	//IBaseFilter *m_pVCap, *m_pACap;	
	//IGraphBuilder *m_pFg;
	//IAMStreamConfig *m_pASC;      // for audio cap
    //IAMStreamConfig *m_pVSC;      // for video cap
	//IAMVideoCompression *m_pVC;
	//ISampleCaptureGraphBuilder *m_pBuilder;
	//CCrossbar* m_pCrossbar;
	bool m_bPreview;
	//HWND m_hWnd;
	//LONG m_NumberOfVideoInputs;
protected:
	bool ChooseDevice(IMoniker*pDevice);
	bool InitCaptureGraphBuilder();
	/*void NukeDownstream(IBaseFilter *pf);
	void TearDownGraph(void);
	void FreeCapFilters(void);
	BOOL InitCapFilters(void);
	void BuildPreviewGraph(void);
	void StartPreview(void);
	BOOL MakeBuilder();
	BOOL MakeGraph();*/
};

#endif // !defined(AFX_XCAMERA_H__6B2852A6_51A4_4E34_997C_4A7B8193024B__INCLUDED_)
