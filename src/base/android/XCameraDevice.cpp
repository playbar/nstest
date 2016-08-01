// XCamera.cpp: implementation of the XCamera class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxGGBase.h"

#include "XCameraDevice.h"
#include "XDib.h"
#include "XCamera.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define SAFE_RELEASE(x) {if(x) x->Release();x=NULL;}

XCameraDevice::XCameraDevice()
{
	m_pVideo=NULL;
	m_bPreview = true;
	m_nIndex = 0;
	//m_hWnd = NULL;
	//int er=GetLastError();

		/*CreateWindowEx(0,
                            MAKEINTATOM(6),    // Class name
                            _T("Capture"),             // Caption
                            // Style bits
                            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                            CW_USEDEFAULT, 0,       // Position
                            320,300,                // Size
                            (HWND)NULL,             // Parent window (no parent)
                            (HMENU)NULL,            // use class menu
                            NULL,                  // handle to window instance
                            (LPSTR)NULL             // no params to pass on
                            );*/
	/*m_pSink =  NULL;
	m_pConfigAviMux = NULL;
	m_pRender = NULL;
	m_pME = NULL;
	m_pDF = NULL;
	m_pVW = NULL;
	m_pVCap = NULL;
	m_pACap = NULL;
	m_pBuilder = NULL;
	m_pFg = NULL;
	m_pVSC = NULL;
	m_pASC = NULL;
	m_pVC = NULL;
	m_pCrossbar = NULL;
	m_NumberOfVideoInputs = NULL;*/
	m_pVW = NULL;
	m_pMC = NULL;
	m_pGB = NULL;
	m_pCapture = NULL;
	m_pGrabber = NULL;
	//m_pConfig = NULL;
	
}

XCameraDevice::~XCameraDevice()
{
	//int i;
	Release();
	if(m_pDib)
		delete m_pDib;
	m_pDib=NULL;
	//m_pDib = NULL;
	//::DestroyWindow(m_hWnd);
}


bool XCameraDevice::Stop()
{
	
	return true;
}

void XCameraDevice::Release()
{
	
	
	
}


XDib* XCameraDevice::GetDib()
{
	return NULL;
}

bool XCameraDevice::Init(XPCWSTR name,XCamera*pCamera)
{
	return true;
}

bool XCameraDevice::Start()
{
	return true;
}

bool XCameraDevice::ChooseDevice(IMoniker*pDevice)
{
	return true;
}

bool XCameraDevice::InitCaptureGraphBuilder()
{
	return true;

}

#define MAXDEVICES 12
//XU16 _cameraDeviceNames[12][120];

int XCameraDevice::GetDevices(XString8Array&list)
{
    return 0;
}