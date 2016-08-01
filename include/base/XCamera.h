#pragma once

#include "GGType.h"	// Added by ClassView
#include "XString.h"
#include "XMutex.h"
class XCameraDevice;
class XDib;

typedef void  (*_CAMERACALLBACK)(void*);
typedef struct _CAMERAATTACH
{
	_CAMERACALLBACK call;
	void*		    pArg;
}CAMERAATTACH;

class _XEXT_CLASS XCamera
{
public:
	XCamera();
	~XCamera();
	//获得所有摄像头的名称
	static int GetDevices(XString8Array&list);	
	void Update();
	bool Stop();
	bool Start();
	bool SetName(XPCWSTR name);
	XDib* GetDib();
	bool  Attach(_CAMERACALLBACK call,void*pArg);
	bool  UnAttach(void*pArg);
	int   GetActiveLevel();
	void  SetActiveLevel(int v);
	int	  GetMontionLevel();
	int   GetMontionTimeout();
	int	  GetBindWidth();
	int	  GetCurrentFPS();
	int	  GetFPS();
	int	  GetIndex();
	int	  GetKeyFrame();
	void  SetLoopback(bool b);
	bool  GetLoopback();
	int	  GetQuality();
	void  SetKeyFrame(int v);
	void  SetQuality(int bw,int qa);
	void  SetMotionLevel(int lv,int tm);
	XPCWSTR GetName();
protected:
	int		  m_nActiveLevel;
	int		  m_nBindWidth;
	int		  m_nMontionLevel;
	int		  m_nMontionTimeout;
	int		  m_nFPS;
	int		  m_nCurrentFPS;
	int		  m_nKeyFrame;
	bool      m_bLoopback;
	int		  m_nQuality;
	XMutex	  m_lock;
	XString16 m_name;
	XCameraDevice* m_pDevice;
	XArray<_CAMERAATTACH> m_attachs;
};