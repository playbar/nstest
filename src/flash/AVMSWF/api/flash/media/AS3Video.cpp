#include "StdAfxflash.h"
#include "sobject.h"
#include "avmplus.h"
#include "avm2.h"
using namespace avmplus;
#include "AS3Video.h"
#include "AS3Camera.h"
#include "XDibSrv.h"
#include "SPlayer.h"
namespace avmshell{
VideoClass::VideoClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())VideoObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* VideoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) VideoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
VideoObject::VideoObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
void VideoObject::ReleaseObject()
{
	if(IsRunning())
	{
		CameraObject*pCamera=m_pCamera;
		if(pCamera)
			pCamera->UnAttachVideo(this);
	}
	DisplayObjectObject::ReleaseObject();
}

int VideoObject::AS3_deblocking_get()
{
	//Add your act code here...
	return 0;
}

void VideoObject::AS3_deblocking_set(int)
{
	//Add your act code here...
	
}

bool VideoObject::AS3_smoothing_get()
{
	//Add your act code here...
	return true;
}

void VideoObject::AS3_smoothing_set(bool)
{
	//Add your act code here...
	
}

int VideoObject::AS3_videoHeight_get()
{
	//Add your act code here...
	//return m_dib.Width();
	if(m_pObject)
	{
		return m_pObject->character->bits.height;
	}
	CameraObject*pCamera=m_pCamera;
	if(pCamera)
	{
		return pCamera->AS3_height_get();
	}
	return 0;
}

int VideoObject::AS3_videoWidth_get()
{
	//Add your act code here...
	//return m_dib.Height();
	if(m_pObject)
	{
		return m_pObject->character->bits.width;
	}
	CameraObject*pCamera=m_pCamera;
	if(pCamera)
	{
		return pCamera->AS3_width_get();
	}
	return 0;
}

//AS3 contructor function..
void VideoObject::AS3_constructor(int w,int h)
{
	//Add your act code here...
	if(m_pObject==NULL)
	{
		DisplayObjectObject::CreateNewObject(videoEmptyChar);
		SCharacter*ch=m_pObject->character;
		ch->rbounds.xmin=0;
		ch->rbounds.ymin=0;
		ch->rbounds.xmax=w*20;
		ch->rbounds.ymax=h*20;
		ch->bits.PIInit();
		//ch->frame = new FrameTexture();
		//ch->frame->nImgDiv = 0;
		if(w>0&&h>0)
			//ch->frame->init(w,h,false);
			ch->bits.PICreate(w,h,false);
		//ch->bits.LockBits();
		//ch->bits.m_nLockTime = XSWFPlayer::m_pInstance->m_nRunTime;
			//m_dib.Create(w,h);
	}
	DisplayObjectObject::AS3_constructor();
}
//指定要在应用程序中 Video 对象的边界内显示的来自摄像头的视频流。
//camera:Camera — 正在捕获视频数据的 Camera 对象。 若要切断与该 Video 对象的连接，请传递 null。 
//XU8 tt=0;
void VideoObject::Update()
{
	//return;
	CameraObject*pCamera=m_pCamera;
	SBitmapCore* bits = &m_pObject->character->bits;
	if(pCamera&&bits->HasBits())
	{
		XDib*pDib=pCamera->GetDib();
		
		if(bits->width*bits->height*4==pDib->m_nDataLength)
			memcpy(bits->baseAddr,pDib->m_pDib,pDib->m_nDataLength);
		else
		{
				//int l=XMIN(m_dib.m_nDataLength,pDib->m_nDataLength);
				//memcpy(m_dib.m_pDib,pDib->m_pDib,l);//pDib->m_nDataLength);
				//memset(m_dib.m_pDib,tt,l);
				XDibSrv* pSrv=XDibSrv::GetInst();
				SRECT draw={0,(SCOORD)bits->width,0,(SCOORD)bits->height};
				MATRIX mat;
				::MatrixIdentity(&mat);
				mat.a=fixed_1*pDib->Width()/bits->width;
				mat.d=fixed_1*pDib->Height()/bits->height;
				XDrawDib m_dib;
				m_dib.CreateFrom(&m_pObject->character->bits,true);
				pSrv->ScaleTo(m_dib,*pDib,draw,mat,SE_SCALE_COPY);
		}
			
		
		/*m_pObject->character->record.ReleaseAll();
		if(m_pObject->character->bits.bitmapID)
			RasterGL::sharedRasterGL()->DeleteTexture(m_pObject->character->bits.bitmapID);
		m_pObject->character->bits.m_nLockTime = XSWFPlayer::m_pInstance->m_nRunTime;*/
	}
	//m_dib.ResetAlpha(0xff);
	//tt++;
	if(m_pObject)
		m_pObject->Modify();
}

void VideoObject::AS3_attachCamera(CameraObject*obj)
{
	//Add your act code here...
	CameraObject*pCamera=m_pCamera;
	if(pCamera&&pCamera!=obj)
	{
		pCamera->UnAttachVideo(this);
	}
	m_pCamera=obj;
	if(obj)
	{
		obj->AttachVideo(this);
		if(!m_pObject->character->bits.HasBits())
		{
			XDib*pDib=obj->GetDib();
			//m_dib.ResetSize(pDib->m_nWidth,pDib->m_nHeight);
			
			
			if(m_pObject)
			{
				
				SCharacter*ch=m_pObject->character;
				if(ch->bits.bitmapID)
					RasterGL::sharedRasterGL()->DeleteTexture(ch->bits.bitmapID);
				ch->record.ReleaseAll();
				ch->bits.PIFree();
				ch->bits.PICreate(pDib->m_nWidth,pDib->m_nHeight,false);
				//ch->bits.m_nLockTime = XSWFPlayer::m_pInstance->m_nRunTime;

				ch->rbounds.xmin=0;
				ch->rbounds.ymin=0;
				ch->rbounds.xmax=pDib->m_nWidth*20;
				ch->rbounds.ymax=pDib->m_nHeight*20;
			}
		}
	}
	if(m_pObject)
		m_pObject->Modify();
}

void VideoObject::AS3_attachNetStream(NetStreamObject*)
{
	//Add your act code here...	
}

void VideoObject::AS3_clear()
{
	//Add your act code here...	
}

}