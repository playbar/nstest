#include "StdAfxGGBase.h"
#include "XCamera.h"
#include "XCameraDevice.h"

XCamera::XCamera()
{
	m_pDevice=new XCameraDevice();
	m_nActiveLevel = -1;
	m_nMontionLevel = 50;
	m_nMontionTimeout = 2000;
	m_nBindWidth = 16384;
	m_nFPS = 15;
	m_nCurrentFPS = 0;
	m_nKeyFrame = 15;
	m_bLoopback = false;
	m_nQuality = 0;
}

XCamera::~XCamera()
{
	if(m_pDevice)
		delete m_pDevice;
}


int  XCamera::GetActiveLevel()
{
	return m_nActiveLevel;
}
void  XCamera::SetActiveLevel(int v)
{
	m_nActiveLevel = v;
}
int	  XCamera::GetMontionLevel()
{
	return m_nMontionLevel;
}
int   XCamera::GetMontionTimeout()
{
	return m_nMontionTimeout;
}
int	  XCamera::GetBindWidth()
{
	return m_nBindWidth;
}
int	  XCamera::GetCurrentFPS()
{
	return m_nCurrentFPS;
}
int	  XCamera::GetFPS()
{
	return m_nFPS;
}
int	  XCamera::GetIndex()
{
	return m_pDevice->m_nIndex;
}
int	  XCamera::GetKeyFrame()
{
	return m_nKeyFrame;
}
void  XCamera::SetLoopback(bool b)
{
	m_bLoopback =b;
}
bool  XCamera::GetLoopback()
{
	return m_bLoopback;
}
int	 XCamera::GetQuality()
{
	return m_nQuality;
}
void  XCamera::SetKeyFrame(int v)
{
	m_nKeyFrame = v;
}
void  XCamera::SetQuality(int bw,int qa)
{
	m_nQuality = qa;
	m_nBindWidth = bw;
}
void  XCamera::SetMotionLevel(int lv,int tm)
{
	m_nMontionLevel = lv;
}
XPCWSTR XCamera::GetName()
{
	return m_name;
}

int XCamera::GetDevices(XString8Array&list)
{	
	return XCameraDevice::GetDevices(list);
}

void XCamera::Update()
{
	XLock lock(&m_lock);
	if(lock.Lock(XSHORTTIME))
	{
		int i;
		for(i=0;i<m_attachs.GetSize();i++)
		{
			_CAMERAATTACH&c=m_attachs[i];
			if(c.call&&c.pArg)
				c.call(c.pArg);
		}
		lock.UnLock();
	}
}

bool XCamera::Start()
{
	return m_pDevice->Start();
}

bool XCamera::Stop()
{
	return m_pDevice->Stop();
}

XDib* XCamera::GetDib()
{
	return m_pDevice->GetDib();
}


bool XCamera::SetName(XPCWSTR name)
{
	m_name = name;
	m_pDevice->Init(name,this);
	return true;
}

bool  XCamera::Attach(_CAMERACALLBACK call,void*pArg)
{
	int iCount=0,i;
	bool bNew=true;
	for(i=0;i<m_attachs.GetSize();i++)
	{
		_CAMERAATTACH&c=m_attachs[i];
		if(!c.call||!c.pArg)
		{
			if(pArg)
			{
				c.call=call;
				c.pArg=pArg;
				iCount++;
				bNew=false;
			}
		}
		else
			iCount++;
	}
	if(bNew)
	{
		_CAMERAATTACH c={call,pArg};
		m_attachs.Add(c);
		iCount++;
	}
	if(iCount)
		m_pDevice->Start();
	return true;
}
bool  XCamera::UnAttach(void*pArg)
{
	int iCount=0,i;
	for(i=0;i<m_attachs.GetSize();i++)
	{
		_CAMERAATTACH&c=m_attachs[i];
		if(c.pArg==pArg)
		{
			c.pArg=XNULL;
			c.call=XNULL;
		}
		else if(c.pArg&&c.pArg)
			iCount++;
	}	
	if(!iCount)
		m_pDevice->Stop();
	return true;
}