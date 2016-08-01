#include "StdAfxflash.h"
#include "avmplus.h"
#include "avm2.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Camera.h"
#include "AS3Video.h"
#include "AS3StatusEvent.h"
#include "XCamera.h"
#include "XDibSrv.h"
namespace avmshell{
CameraClass::CameraClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())CameraObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* CameraClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CameraObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CameraObject::CameraObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	//m_nWidth = 0;
	//m_nHeight = 0;
	m_bFirst = true;
	m_pDib = XNULL;
	m_strPosition = ((ShellToplevel*)toplevel())->getCameraPositionClass()->getSlotUNKNOWN();
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* CameraClass::AS3_names_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	ArrayObject*pArray=toplevel()->arrayClass->newArray(0);
	XString8Array list;
	int i,iCount=XCamera::GetDevices(list);
	AvmCore*core=this->core();
	for(i=0;i<iCount;i++)
	{		
		Stringp str=core->newStringUTF8(list[i]);
		Atom atoms[]={str->atom()};
		pArray->push(atoms,1);
	}
	return pArray;
}


CameraObject* CameraClass::AS3_getCamera(Stringp name)
{
	//Add your act code here...
	//XCamera*pCamera=XCamera::
	//StUTF8String ss(s)
	//XPCWSTR strName=NULL;
	//
	/*XCamera*pDevice=NULL;
	if(name==NULL)
		pDevice=XCamera::GetCamera(XNULL);
	else
	{
		StUTF16String s(name);
		pDevice=XCamera::GetCamera(s.c_str());
	}*/
	CameraObject*pObj=(CameraObject*)createInstance(ivtable(),prototype);
	if(name)
	{
		StUTF16String s(name);
		pObj->InitCamera(s.c_str());
	}
	else
	{
		
		pObj->InitCamera(XNULL);
	}
	return pObj;
}


XDib* CameraObject::GetDib()
{
	if(m_pDib) return m_pDib;
	return m_camera.GetDib();
}

double CameraObject::AS3_activityLevel_get()
{
	//Add your act code here...
	return m_camera.GetActiveLevel();
}

int CameraObject::AS3_bandwidth_get()
{
	//Add your act code here...
	return m_camera.GetBindWidth();
}

double CameraObject::AS3_currentFPS_get()
{
	//Add your act code here...
	return m_camera.GetCurrentFPS();
}

double CameraObject::AS3_fps_get()
{
	//Add your act code here...
	return m_camera.GetFPS();
}

int CameraObject::AS3_height_get()
{
	//Add your act code here...
	return m_pDib?m_pDib->Height():m_camera.GetDib()->Width();
}

int CameraObject::AS3_index_get()
{
	//Add your act code here...
	return m_camera.GetIndex();
}

int CameraObject::AS3_keyFrameInterval_get()
{
	//Add your act code here...
	return m_camera.GetKeyFrame();
}

bool CameraObject::AS3_loopback_get()
{
	//Add your act code here...
	return m_camera.GetLoopback();
}

int CameraObject::AS3_motionLevel_get()
{
	//Add your act code here...
	return m_camera.GetMontionLevel();
}

int CameraObject::AS3_motionTimeout_get()
{
	//Add your act code here...
	return m_camera.GetMontionTimeout();
}

bool CameraObject::AS3_muted_get()
{
	//Add your act code here...
	return false;
}

Stringp CameraObject::AS3_name_get()
{
	//Add your act code here...
	XPCWSTR name=m_camera.GetName();
	return core()->newStringUTF16(name);
	//return core()->kEmptyString;
}

int CameraObject::AS3_quality_get()
{
	//Add your act code here...
	return m_camera.GetQuality();
}

int CameraObject::AS3_width_get()
{
	//Add your act code here...
	return m_pDib?m_pDib->Width():m_camera.GetDib()->Width();
}

void CameraObject::AS3_setKeyFrameInterval(int v)
{
	//Add your act code here...	
	m_camera.SetKeyFrame(v);
}

void CameraObject::AS3_setLoopback(bool b)
{
	//Add your act code here...
	m_camera.SetLoopback(b);
	
}

void CameraObject::AS3_setMode(int w,int h,double,bool)
{
	//Add your act code here...	
	XDib*pDib=m_camera.GetDib();
    if (!pDib) {
        return;
    }
	if(pDib->Width()!=w||pDib->Height()!=h)
	{
		if(!m_pDib)
			m_pDib=new XDib();
		m_pDib->ResetSize(w,h);
		CopyDib();
	}
	/*else
	{
		if(m_pDib)
		{
			delete m_pDib;
			m_pDib = NULL;
		}
	}*/
}

void CameraObject::AS3_setMotionLevel(int v,int t)
{
	//Add your act code here...
	m_camera.SetMotionLevel(v,t);
}

void CameraObject::AS3_setQuality(int bw,int q)
{
	//Add your act code here...	
	m_camera.SetQuality(bw,q);
}

void CameraObject::Release()
{
	//if(IsRunning())
		m_camera.Stop();
	if(m_pDib)
		delete m_pDib;
	EventDispatcherObject::Release();
}

bool CameraObject::InitCamera(XPCWSTR name)
{
	return m_camera.SetName(name);
	//return true;
}

void CameraObject::CopyDib()
{
	if(!m_pDib) return;
	XDib*pDst=m_pDib;
	XDib*pSrc=m_camera.GetDib();
	XDibSrv* srv=XDibSrv::GetInst();
	SRECT draw={0,(SCOORD)pDst->Width(),0,(SCOORD)pDst->Height()};
	if(pDst->Width()!=pSrc->Width()||pDst->Height()!=pSrc->Height())
	{
		//XDibSrv* srv=XDibSrv::instance;
		SRECT draw={0,(SCOORD)pDst->Width(),0,(SCOORD)pDst->Height()};
		MATRIX mat;
		::MatrixIdentity(&mat);
		int sx=fixed_1*pSrc->Width()/pDst->Width();
		int sy=fixed_1*pSrc->Height()/pDst->Height();
		int ss=XMIN(sx,sy);
		mat.a=ss;
		mat.d=ss;
		mat.tx=(pSrc->Width()-pDst->Width()*ss/fixed_1)/2;
		mat.ty=(pSrc->Height()-pDst->Height()*ss/fixed_1)/2;
		srv->ScaleTo(*pDst,*pSrc,draw,mat,SE_SCALE_COPY);
	}
	else
	{
		SPOINT pt={0,0};
		srv->Copy(*pDst,*pSrc,pt,draw);	
	}
}

static void _attachBack(void*pArg)
{
	if(pArg)
	{
		VideoObject*pObj=(VideoObject*)pArg;
		CameraObject*pCamera=pObj->GetAttachCamera();
		if(pCamera->m_pDib)
		{
			pCamera->CopyDib();
			//srv->ScaleTo(*pCamera->m_pDib,*pCamera->m_camera.GetDib(),
		}
		pObj->Update();
	}
}

bool CameraObject::AttachVideo(VideoObject*obj)
{
	bool b=m_camera.Attach(_attachBack,obj);
	if(b&&!m_bFirst)
	{
		/*if(!m_camera.m_->IsStarted())
		{
			Stringp str=core()->newConstantStringLatin1("Microphone.error");
			this->OnStatusEvent(str,StatusEventObject::LV_ERROR);
		}
		else if(m_bFirst)*/
		{
			m_bFirst = false;
			Stringp str=core()->newConstantStringLatin1("Camera.unmuted");
			this->OnStatusEvent(str,StatusEventObject::LV_STATUS);
		}
	}
	return b;
}
bool CameraObject::UnAttachVideo(VideoObject*obj)
{
	return m_camera.UnAttach(obj);
}

//new adds 11

Stringp CameraObject::AS3_position_get()
{
//	LOGWHERE();
	return m_strPosition;
}
void CameraObject::AS3_copyToByteArray(RectangleObject* pRect, ByteArrayObject* destination)
{
	LOGWHERE();
}
void CameraObject::AS3_copyToVector(RectangleObject* pRect, IntVectorObject* destination)
{
	LOGWHERE();
}
void CameraObject::AS3_drawToBitmapData(BitmapDataObject* pDestination)
{
	LOGWHERE();
}
//new adds 11 end
}