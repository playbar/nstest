// XCamera.cpp: implementation of the XCamera class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfxGGBase.h"
#include <streams.h>
#include <windows.h>
#include <dbt.h>
#include <mmreg.h>
#include <msacm.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <commdlg.h>
#include <atlbase.h>
#include <strsafe.h>
#pragma include_alias( "dxtrans.h", "qedit.h" ) 
#define __IDxtCompositor_INTERFACE_DEFINED__ 
#define __IDxtAlphaSetter_INTERFACE_DEFINED__ 
#define __IDxtJpeg_INTERFACE_DEFINED__ 
#define __IDxtKey_INTERFACE_DEFINED__ 
#include <qedit.h>
#include "XCameraDevice.h"
#include "XDib.h"
#include "XCamera.h"

//#include "SampleCGB.h"
//#include "crossbar.h"
HWND XCameraDevice::m_hWnd = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define SAFE_RELEASE(x) {if(x) x->Release();x=NULL;}
class SampleGrabberCB :
	public ISampleGrabberCB
{
protected:
	XDib m_dib;		
public:
	bool m_bCapture;
	XCamera* m_pCamera;
	XDib* GetDib(){return &m_dib;}
	XBOOL Create(int w,int h)
	{
		return m_dib.Create(w,h,32,0);
	}
	XBOOL Resize(int w,int h)
	{
		m_dib.ResetSize(w,h);
		return XTRUE;
	}
	SampleGrabberCB(void)
	{
		m_pCamera = NULL;
	}
	~SampleGrabberCB(void)
	{
	}
	STDMETHODIMP_(ULONG) AddRef() { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)\
	{
	  if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown )
	  {
		  *ppv = static_cast<void*>(this);//0;//static_cast(this);
		  return NOERROR;
	  }
	  return E_NOINTERFACE;
 }
 STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
  {
 		return 0;
  }
  STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
  {
 	  //if( !bOneShot )return 0;
 	  if (!pBuffer||!m_bCapture)return E_POINTER;

	  int iLine=m_dib.m_nLinePitch;
	  BYTE*pDst=m_dib.m_pDib+(m_dib.m_nHeight-1)*iLine;
	  BYTE*pSrc=pBuffer;
	  while(lBufferSize)
	  {
		  memcpy(pDst,pSrc,iLine);
		  for(int i=3;i<iLine;i+=4)
			  pDst[i]=0xff;
		  pDst-=iLine;
		  pSrc+=iLine;
		  lBufferSize-=iLine;
	  }

	  //memcpy(m_dib.m_pDib,pBuffer,lBufferSize);
	  /*if(m_dib.m_pDrawWnd)
	  {
		  m_dib.m_pDrawWnd->Invalidate();
	  }*/
 	  //SaveBitmap(pBuffer, lBufferSize);
 	  //bOneShot = FALSE;
	  if(m_pCamera)
		  m_pCamera->Update();
 	  return 0;
	}

};


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
	m_pDib = new SampleGrabberCB();
	//CoCreateInstance(
	CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_ISampleGrabber, (void**)&m_pGrabber ); 
	//CoCreateInstance( CLSID_SampleGrabberCB, NULL, CLSCTX_INPROC_SERVER, IID_ISampleGrabberCB, (void**)&m_pDib ); 

	//InitDrivers();
	//m_capWnd.Create();
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
	if(!m_pMC) return false;
	m_pMC->Stop();	
	if(m_pDib)
		m_pDib->m_bCapture = false;
	return true;
}

void XCameraDevice::Release()
{
	if(m_pVideo)
		m_pVideo->Release();
	m_pVideo=NULL;
	/*for(i=0;i<m_devices.GetSize();i++)
	{
		m_devices[i]->pDevice->Release();
		delete m_devices[i];
	}
	m_devices.RemoveAll();*/
	SAFE_RELEASE(m_pVW);
	SAFE_RELEASE(m_pMC);
	SAFE_RELEASE(m_pGB);
	SAFE_RELEASE(m_pCapture);
	SAFE_RELEASE(m_pGrabber);
	SAFE_RELEASE(m_pGrabber);
	//SAFE_RELEASE(m_pConfig);
	
}


IMoniker* XCameraDevice::GetDeviceByName(XPCWSTR name)
{
  // enumerate all video capture devices

    ICreateDevEnum *pCreateDevEnum=0;
    int hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
                          IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if(hr != NOERROR)
    {
        //ErrMsg(TEXT("Error Creating Device Enumerator"));
        return NULL;
    }

    IEnumMoniker *pEm=0;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if(hr != NOERROR)
    {
        //ErrMsg(TEXT("Sorry, you have no video capture hardware.\r\n\r\n")
        //       TEXT("Video capture will not function properly."));
        //goto EnumAudio;
		return NULL;
    }

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
	IMoniker *pSelect=NULL;
	IMoniker *pFirst=NULL;
	int iCount=0;
	m_nIndex = 0;
    while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
    {
        IPropertyBag *pBag=0;

        hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if(SUCCEEDED(hr))
        {
            VARIANT var;
            var.vt = VT_BSTR;
            hr = pBag->Read(L"FriendlyName", &var, NULL);
            if(hr == NOERROR)
            {
				if(name==XNULL||wcscmp(name,var.bstrVal)==0)
				{
					pSelect=pM;	
					m_nIndex = iCount;
				}
                SysFreeString(var.bstrVal);

                //pM->AddRef();
            }
            pBag->Release();
        }
		if(!pFirst)
			pFirst=pM;
		if(!name) break;
        pM->Release();
		iCount ++;
        //uIndex++;
    }
    pEm->Release();
	if(pSelect==NULL)
		pSelect=pFirst;
	if(pSelect)
		pSelect->AddRef();
	return pSelect;
}

XDib* XCameraDevice::GetDib()
{
	return m_pDib->GetDib();
}

bool XCameraDevice::Init(XPCWSTR name,XCamera*pCamera)
{
	//InitDrivers();
	if(m_pMC) return false;
	IMoniker* pDevice=GetDeviceByName(name);
	if(pDevice==NULL) return false;
	if(!this->InitCaptureGraphBuilder()) return false;
	ChooseDevice(pDevice);

	

	HRESULT hr = m_pGB->AddFilter(m_pBF, L"Capture Filter");

	
	 

 // hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, 
 // m_pBF, NULL, NULL);
 // create a sample grabber
	hr = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_ISampleGrabber, (void**)&m_pGrabber ); 
	if( !m_pGrabber ){
	  //AfxMessageBox("Fail to create SampleGrabber, maybe qedit.dll is not registered?");
	  return false;
	}
	 CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabBase( m_pGrabber );
	 //设置视频格式
	 CMediaType mt;
	 //AM_MEDIA_TYPE mt; 
	 //ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	 mt.SetType(&MEDIATYPE_Video);
	 mt.SetSubtype(&MEDIASUBTYPE_RGB32);
	
	 /*{
		   IAMStreamConfig *pConfig = NULL;
			hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, 0, // Any media type.
			   m_pBF, // Pointer to the capture filter.
			   IID_IAMStreamConfig, (void**)&pConfig);

			AM_MEDIA_TYPE *pmt;
			hr   =   pConfig ->GetFormat(&pmt); 

				//ZeroMemory(pmt, sizeof(AM_MEDIA_TYPE));
			//pmt->majortype = MEDIATYPE_Video;
			//pmt->subtype = MEDIASUBTYPE_RGB32;
			//pmt->formattype = FORMAT_VideoInfo;
			VIDEOINFOHEADER   *pvi   =   (VIDEOINFOHEADER*)pmt->pbFormat;   
			pvi->bmiHeader.biWidth=640;   
			pvi->bmiHeader.biHeight   =480;
			pvi->bmiHeader.biBitCount = 32;
			pvi->bmiHeader.biSizeImage=640*480*4;   //设置帧大小，而不是仅仅改变预览窗口大小
			//hr   =   pConfig->SetFormat(pmt);   
			mt.SetFormatType(&FORMAT_VideoInfo);  
			mt.AllocFormatBuffer(pmt->cbFormat);
			mt.SetFormat(pmt->pbFormat,pmt->cbFormat);
			FreeMediaType(*pmt);  
			pConfig->Release();

	}*/

	 //mt.majortype = MEDIATYPE_Video;
	 //mt.subtype = MEDIASUBTYPE_RGB32;
	 hr = m_pGrabber->SetMediaType(&mt);
	 if( FAILED( hr ) ){
	  //AfxMessageBox("Fail to set media type!");
	  return false;
	 }
	
	

	 hr = m_pGB->AddFilter( pGrabBase, L"Grabber" );
	 if( FAILED( hr ) ){
	  //AfxMessageBox("Fail to put sample grabber in graph");
	  return false;
	 }
	 // try to render preview/capture pin
	 hr = m_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,m_pBF,pGrabBase,NULL);
	 if( FAILED( hr ) )
	  hr = m_pCapture->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,m_pBF,pGrabBase,NULL);
	  if( FAILED( hr ) ){
	   //AfxMessageBox("Can’t build the graph");
	   return false;
	  }
	  
	 
    /*  {
		CComPtr<IPin> pPin;
		m_pBF->FindPin(L"0", &pPin); 

		CComQIPtr<IAMStreamConfig> pConfig(pPin);
		AM_MEDIA_TYPE* pmt = NULL;
		hr = pConfig->GetFormat(&pmt);// ---->Succeeded!
		if (SUCCEEDED(hr) && pmt->formattype == FORMAT_VideoInfo)
		{
		VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)pmt->pbFormat;
		pvi->bmiHeader.biWidth = 320;
		pvi->bmiHeader.biHeight = 240;
		pvi->bmiHeader.biSizeImage = DIBSIZE(pvi->bmiHeader);
		pmt->subtype = MEDIASUBTYPE_RGB24;
		hr = pConfig->SetFormat(pmt);// ----> Failed! hr = -2147467259.
		//ASSERT(SUCCEEDED(hr));

		DeleteMediaType(pmt);
		}
		pConfig.Release();


	 }*/

	 hr = m_pGrabber->GetConnectedMediaType( &mt );
	 if ( FAILED( hr) ){
	  //AfxMessageBox("Failt to read the connected media type");
	  return false;
	 }
	 VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mt.pbFormat;
	 
	

	 //mCB.lWidth = vih->bmiHeader.biWidth;
	 //mCB.lHeight = vih->bmiHeader.biHeight;
	 m_pDib->Create(vih->bmiHeader.biWidth,vih->bmiHeader.biHeight);
	 FreeMediaType(mt);
	 hr = m_pGrabber->SetBufferSamples( FALSE );
	 hr = m_pGrabber->SetOneShot( FALSE );
	 hr = m_pGrabber->SetCallback( m_pDib, 1 );
	 //设置视频捕捉窗口
	 //m_hWnd = hWnd ; 
	 //SetupVideoWindow();
	 hr = m_pVW->put_Owner((OAHWND)m_hWnd);//m_pDib->GetDib()->m_pDrawWnd->m_hWnd);

	 //m_pDib->GetDib()->m_pDrawWnd->ShowWindow(SW_HIDE);
	 //ShowWindow(m_hWnd,SW_HIDE);
	 hr = m_pVW->put_Visible( FALSE );
	 hr = m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
	
	 if(m_pDib)
	 {
		 m_pDib->m_pCamera=pCamera;
		 m_pDib->m_bCapture = true;
	 }

	 //hr = m_pMC->Run();//开始视频捕捉
	 if(FAILED(hr)){return false;}

	/* {
		   IAMStreamConfig *pConfig = NULL;
			hr = m_pCapture->FindInterface(&PIN_CATEGORY_CAPTURE, 0, // Any media type.
			   m_pBF, // Pointer to the capture filter.
			   IID_IAMStreamConfig, (void**)&pConfig);

			AM_MEDIA_TYPE *pmt;
			hr   =   pConfig ->GetFormat(&pmt); 

				//ZeroMemory(pmt, sizeof(AM_MEDIA_TYPE));
			//pmt->majortype = MEDIATYPE_Video;
			//pmt->subtype = MEDIASUBTYPE_RGB32;
			pmt->formattype = FORMAT_VideoInfo;
			VIDEOINFOHEADER   *pvi   =   (VIDEOINFOHEADER   *)pmt->pbFormat;   
			pvi->bmiHeader.biWidth=320;   
			pvi->bmiHeader.biHeight   =200;   
			pvi->bmiHeader.biSizeImage=320*200*3;   //设置帧大小，而不是仅仅改变预览窗口大小
			hr   =   pConfig->SetFormat(pmt);   
			//mt.SetFormatType(&FORMAT_VideoInfo);  
			//mt.AllocFormatBuffer(pmt->cbFormat);
			//mt.SetFormat(pmt->pbFormat,pmt->cbFormat);
			FreeMediaType(*pmt);  
			pConfig->Release();

	}*/
	//hr = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_ISampleGrabber, (void**)&m_pGrabber );
	return true;
}

bool XCameraDevice::Start()
{
	if(!m_pMC) return false;
	if(m_pDib)
		m_pDib->m_bCapture = true;
	HRESULT hr = m_pMC->Run();//开始视频捕捉
	if(FAILED(hr)){return false;}
	return true;
}

bool XCameraDevice::ChooseDevice(IMoniker*pDevice)
{
	#define VERSIZE 40
#define DESCSIZE 80

    USES_CONVERSION;
    int versize = VERSIZE;
    int descsize = DESCSIZE;
    WCHAR wachVer[VERSIZE]={0}, wachDesc[DESCSIZE]={0};
    TCHAR tachStatus[VERSIZE + DESCSIZE + 5]={0};


    // they chose a new device. rebuild the graphs
    if(m_pVideo!=pDevice)
    {
        if(pDevice)
        {
            pDevice->AddRef();
        }
		if(m_pVideo)
		{
			m_pVideo->Release();
		}
		m_pVideo=pDevice;
		m_wachFriendlyName[0]=0;
		if(m_pVideo)
		{
			IPropertyBag *pBag;
 			HRESULT hr = m_pVideo->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
 			if(SUCCEEDED(hr)) 
 			{
 				  VARIANT var;
 				  var.vt = VT_BSTR;
 				  hr = pBag->Read(L"FriendlyName", &var, NULL);
 				  if (hr == NOERROR) 
 				  {
						StringCchCopyW(m_wachFriendlyName,sizeof(m_wachFriendlyName),var.bstrVal);
						m_pVideo->BindToObject(0, 0, IID_IBaseFilter, (void**)&m_pBF);
 						SysFreeString(var.bstrVal);
 				  }
 				  pBag->Release();
			}
		}

        //if(m_fPreviewing)
        //    StopPreview();
        //if(m_fCaptureGraphBuilt || m_fPreviewGraphBuilt)
        /*TearDownGraph();

        FreeCapFilters();
        InitCapFilters();

        if(m_bPreview)//m_fWantPreview)   // were we previewing?
        {
            BuildPreviewGraph();
            StartPreview();
        }*/

        //MakeMenuOptions();      // the UI choices change per device
    }

    // Since the GetInfo method failed (or the interface did not exist),
    // display the device's friendly name.
    //statusUpdateStatus(ghwndStatus, W2T(m_wachFriendlyName));
	return true;
}

bool XCameraDevice::InitCaptureGraphBuilder()
{
	 HRESULT hr;
	 // 创建IGraphBuilder接口
	 hr=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGB);
	 // 创建ICaptureGraphBuilder2接口
	 hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,IID_ICaptureGraphBuilder2, (void **) &m_pCapture);
	 if (FAILED(hr))
		 return false;
	 m_pCapture->SetFiltergraph(m_pGB);
	 hr = m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC);
	 if (FAILED(hr))
		 return false;
	 hr = m_pGB->QueryInterface(IID_IVideoWindow, (LPVOID *) &m_pVW);
	 if (FAILED(hr))
		 return false;
	 return SUCCEEDED(hr);

}

/*void XCameraDevice::TearDownGraph(void)
{
	SAFE_RELEASE(m_pSink);
    SAFE_RELEASE(m_pConfigAviMux);
    SAFE_RELEASE(m_pRender);
    SAFE_RELEASE(m_pME);
    SAFE_RELEASE(m_pDF);

    if(m_pVW)
    {
        // stop drawing in our window, or we may get wierd repaint effects
        m_pVW->put_Owner(NULL);
        m_pVW->put_Visible(OAFALSE);
        m_pVW->Release();
        m_pVW = NULL;
    }

    // destroy the graph downstream of our capture filters
    if(m_pVCap)
        NukeDownstream(m_pVCap);
    if(m_pACap)
        NukeDownstream(m_pACap);
    if(m_pBuilder)
        m_pBuilder->ReleaseFilters();

    // potential debug output - what the graph looks like
    // if (m_pFg) DumpGraph(m_pFg, 1);

#ifdef REGISTER_FILTERGRAPH
    // Remove filter graph from the running object table
    if(g_dwGraphRegister)
    {
        RemoveGraphFromRot(g_dwGraphRegister);
        g_dwGraphRegister = 0;
    }
#endif

    //m_fCaptureGraphBuilt = FALSE;
    //m_fPreviewGraphBuilt = FALSE;
    //m_fPreviewFaked = FALSE;
}

void XCameraDevice::FreeCapFilters(void)
{
	SAFE_RELEASE(m_pFg);
    if( m_pBuilder )
    {
        m_pBuilder;
        m_pBuilder = NULL;
    }
    SAFE_RELEASE(m_pVCap);
    SAFE_RELEASE(m_pACap);
    SAFE_RELEASE(m_pASC);
    SAFE_RELEASE(m_pVSC);
    SAFE_RELEASE(m_pVC);
    //SAFE_RELEASE(m_pDlg);

}

BOOL XCameraDevice::MakeBuilder()
{
	if(m_pBuilder)
        return TRUE;

    m_pBuilder = new ISampleCaptureGraphBuilder( );
    if( NULL == m_pBuilder )
    {
        return FALSE;
    }

    return TRUE;
}

BOOL XCameraDevice::MakeGraph()
{
    // we have one already
    if(m_pFg)
        return TRUE;

    HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
                                  IID_IGraphBuilder, (LPVOID *)&m_pFg);

    return (hr == NOERROR) ? TRUE : FALSE;
}

BOOL XCameraDevice::InitCapFilters(void)
{
    HRESULT hr=S_OK;
    BOOL f;

    //m_fCCAvail = FALSE;  // assume no closed captioning support

    f = MakeBuilder();
    if(!f)
    {
        //ErrMsg(TEXT("Cannot instantiate graph builder"));
        return FALSE;
    }

    //
    // First, we need a Video Capture filter, and some interfaces
    //
    m_pVCap = NULL;

    if(m_pVideo != 0)
    {
        IPropertyBag *pBag;
        m_wachFriendlyName[0] = 0;

        hr = m_pVideo->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if(SUCCEEDED(hr))
        {
            VARIANT var;
            var.vt = VT_BSTR;

            hr = pBag->Read(L"FriendlyName", &var, NULL);
            if(hr == NOERROR)
            {
                hr = StringCchCopyW(m_wachFriendlyName, sizeof(m_wachFriendlyName) / sizeof(XU16), var.bstrVal);
                SysFreeString(var.bstrVal);
            }

            pBag->Release();
        }

        hr = m_pVideo->BindToObject(0, 0, IID_IBaseFilter, (void**)&m_pVCap);
    }

    if(m_pVCap == NULL)
    {
        //ErrMsg(TEXT("Error %x: Cannot create video capture filter"), hr);
        goto InitCapFiltersFail;
    }

    //
    // make a filtergraph, give it to the graph builder and put the video
    // capture filter in the graph
    //

    f = MakeGraph();
    if(!f)
    {
        //ErrMsg(TEXT("Cannot instantiate filtergraph"));
        goto InitCapFiltersFail;
    }

    hr = m_pBuilder->SetFiltergraph(m_pFg);
    if(hr != NOERROR)
    {
        //ErrMsg(TEXT("Cannot give graph to builder"));
        goto InitCapFiltersFail;
    }

    // Add the video capture filter to the graph with its friendly name
    hr = m_pFg->AddFilter(m_pVCap, m_wachFriendlyName);
    if(hr != NOERROR)
    {
        //ErrMsg(TEXT("Error %x: Cannot add vidcap to filtergraph"), hr);
        goto InitCapFiltersFail;
    }

    // Calling FindInterface below will result in building the upstream
    // section of the capture graph (any WDM TVTuners or Crossbars we might
    // need).

    // we use this interface to get the name of the driver
    // Don't worry if it doesn't work:  This interface may not be available
    // until the pin is connected, or it may not be available at all.
    // (eg: interface may not be available for some DV capture)
    hr = m_pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
                                      &MEDIATYPE_Interleaved, m_pVCap,
                                      IID_IAMVideoCompression, (void **)&m_pVC);
    if(hr != S_OK)
    {
        hr = m_pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
                                          &MEDIATYPE_Video, m_pVCap,
                                          IID_IAMVideoCompression, (void **)&m_pVC);
    }

    // !!! What if this interface isn't supported?
    // we use this interface to set the frame rate and get the capture size
    hr = m_pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
                                      &MEDIATYPE_Interleaved,
                                      m_pVCap, IID_IAMStreamConfig, (void **)&m_pVSC);

    if(hr != NOERROR)
    {
        hr = m_pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
                                          &MEDIATYPE_Video, m_pVCap,
                                          IID_IAMStreamConfig, (void **)&m_pVSC);
        if(hr != NOERROR)
        {
            // this means we can't set frame rate (non-DV only)
            //ErrMsg(TEXT("Error %x: Cannot find VCapture:IAMStreamConfig"), hr);
        }
    }

    //m_fCapAudioIsRelevant = TRUE;



    // we use this interface to bring up the 3 dialogs
    // NOTE:  Only the VfW capture filter supports this.  This app only brings
    // up dialogs for legacy VfW capture drivers, since only those have dialogs
    //hr = m_pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
    //                                  &MEDIATYPE_Video, m_pVCap,
    //                                  IID_IAMVfwCaptureDialogs, (void **)&m_pDlg);

    // Use the crossbar class to help us sort out all the possible video inputs
    // The class needs to be given the capture filters ANALOGVIDEO input pin
    {
        IPin        *pP = 0;
        IEnumPins   *pins=0;
        ULONG        n;
        PIN_INFO     pinInfo;
        BOOL         Found = FALSE;
        IKsPropertySet *pKs=0;
        GUID guid;
        DWORD dw;
        BOOL fMatch = FALSE;

        //m_pCrossbar = NULL;

        if(SUCCEEDED(m_pVCap->EnumPins(&pins)))
        {
            while(!Found && (S_OK == pins->Next(1, &pP, &n)))
            {
                if(S_OK == pP->QueryPinInfo(&pinInfo))
                {
                    if(pinInfo.dir == PINDIR_INPUT)
                    {
                        // is this pin an ANALOGVIDEOIN input pin?
                        if(pP->QueryInterface(IID_IKsPropertySet,
                            (void **)&pKs) == S_OK)
                        {
                            if(pKs->Get(AMPROPSETID_Pin,
                                AMPROPERTY_PIN_CATEGORY, NULL, 0,
                                &guid, sizeof(GUID), &dw) == S_OK)
                            {
                                if(guid == PIN_CATEGORY_ANALOGVIDEOIN)
                                    fMatch = TRUE;
                            }
                            pKs->Release();
                        }

                        if(fMatch)
                        {
                            HRESULT hrCreate=S_OK;
                            m_pCrossbar = new CCrossbar(pP, &hrCreate);
                            if (!m_pCrossbar || FAILED(hrCreate))
                                break;

                            hr = m_pCrossbar->GetInputCount(&m_NumberOfVideoInputs);
                            Found = TRUE;
                        }
                    }
                    pinInfo.pFilter->Release();
                }
                pP->Release();
            }
            pins->Release();
        }
    }

    // there's no point making an audio capture filter


SkipAudio:

    // Can this filter do closed captioning?
    IPin *pPin;
    hr = m_pBuilder->FindPin(m_pVCap, PINDIR_OUTPUT, &PIN_CATEGORY_VBI,
                                NULL, FALSE, 0, &pPin);
    if(hr != S_OK)
        hr = m_pBuilder->FindPin(m_pVCap, PINDIR_OUTPUT, &PIN_CATEGORY_CC,
                                    NULL, FALSE, 0, &pPin);
    if(hr == S_OK)
    {
        pPin->Release();
        //m_fCCAvail = TRUE;
    }
    //else
    //{
//        m_fCapCC = FALSE;    // can't capture it, then
  //  }

    // potential debug output - what the graph looks like
    // DumpGraph(m_pFg, 1);

    return TRUE;

InitCapFiltersFail:
    FreeCapFilters();
    return FALSE;
}

void XCameraDevice::BuildPreviewGraph(void)
{
}

void XCameraDevice::StartPreview(void)
{
}

void XCameraDevice::NukeDownstream(IBaseFilter *pf)
{
    IPin *pP=0, *pTo=0;
    ULONG u;
    IEnumPins *pins = NULL;
    PIN_INFO pininfo;

    if (!pf)
        return;

    HRESULT hr = pf->EnumPins(&pins);
    pins->Reset();

    while(hr == NOERROR)
    {
        hr = pins->Next(1, &pP, &u);
        if(hr == S_OK && pP)
        {
            pP->ConnectedTo(&pTo);
            if(pTo)
            {
                hr = pTo->QueryPinInfo(&pininfo);
                if(hr == NOERROR)
                {
                    if(pininfo.dir == PINDIR_INPUT)
                    {
                        NukeDownstream(pininfo.pFilter);
                        m_pFg->Disconnect(pTo);
                        m_pFg->Disconnect(pP);
                        m_pFg->RemoveFilter(pininfo.pFilter);
                    }
                    pininfo.pFilter->Release();
                }
                pTo->Release();
            }
            pP->Release();
        }
    }

    if(pins)
        pins->Release();
}*/
#define MAXDEVICES 12
//XU16 _cameraDeviceNames[12][120];

int XCameraDevice::GetDevices(XString8Array&list)
{
  // enumerate all video capture devices
	int iCount=0;
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    ICreateDevEnum *pCreateDevEnum=0;
    int hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
                          IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if(hr != NOERROR)
    {
        //ErrMsg(TEXT("Error Creating Device Enumerator"));
        return NULL;
    }

    IEnumMoniker *pEm=0;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if(hr != NOERROR)
    {
		return NULL;
    }

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
    {
        IPropertyBag *pBag=0;

        hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if(SUCCEEDED(hr))
        {
            VARIANT var;
            var.vt = VT_BSTR;
            hr = pBag->Read(L"FriendlyName", &var, NULL);
            if(hr == NOERROR)
            {
				//if(wcscmp(name,var.bstrVal)==0)
				//memset(_cameraDeviceNames[iCount],0,sizeof(_deviceNames[iCount]));
				//memcpy(_cameraDeviceNames[iCount],var.bstrVal,wcslen(var.bstrVal)*2);
				XString8 str(var.bstrVal);
				list.Add(str);
				iCount++;
                SysFreeString(var.bstrVal);

                //pM->AddRef();
            }
            pBag->Release();
        }
        pM->Release();
		if(iCount>=MAXDEVICES)
			break;
        //uIndex++;
    }
    pEm->Release();
	return iCount;
	//return _deviceNames;
}