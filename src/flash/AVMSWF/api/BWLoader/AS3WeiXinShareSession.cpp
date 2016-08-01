#include "StdAfxflash.h"
#include "XDrawDib.h"
#include "Raster_gl.h"
#include "ShellCore.h"
#include "sobject.h"
#include "splay.h"
#include "splayer.h"
using namespace avmplus;
#include "AS3Rectangle.h"
#include "AS3Point.h"
#include "AS3Matrix.h"
#include "AS3ColorTransform.h"
#include "AS3DisplayObject.h"
#include "AS3ByteArray.h"
#include "AS3ByteArray.h"
#include "AS3Bitmap.h" 
#include "AS3WeiXinShareSession.h"
#include "avm2.h"
#include "AvmCore.h"
#include "StdAfxggdom.h"
#include "XBWSendMessageContent.h"

BWSendMessageContentClass *g_WeiXinShareSessionSavedData = NULL;
bool g_WeiXinAppIsInstalled = true;
int g_WeiXinSendType = 0;

namespace avmshell
{
	WeiXinShareSessionClass::WeiXinShareSessionClass(VTable *vtable):ClassClosure( vtable )
	{
		createVanillaPrototype();
	}
	ScriptObject* WeiXinShareSessionClass::createInstance(VTable* ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) WeiXinShareSessionObject(ivtable, prototype, 0);
	}

	WeiXinShareSessionObject* WeiXinShareSessionClass::AS3_GetWeiXinShareSession()
	{
		WeiXinShareSessionObject *pObj = ( WeiXinShareSessionObject *)createInstance( ivtable(), prototype );
		return pObj;
	}

	WeiXinShareSessionObject::WeiXinShareSessionObject(VTable* vtable, ScriptObject* proto, int capacity) : ScriptObject(vtable, proto, 0)
	{

	}
	
	void WeiXinShareSessionObject::Release()
	{

	}
	
	void WeiXinShareSessionObject::SetObject(void*pObject)
	{

	}

	bool WeiXinShareSessionObject::AS3_isWeiXinIstalled()
	{
		return g_WeiXinAppIsInstalled;
	}

		int WeiXinShareSessionObject::AS3_sendText(int sendType, Stringp text)
		{
			if(!g_WeiXinAppIsInstalled)
				return 0;
            sendType < 2 ? (g_WeiXinSendType = sendType) : (g_WeiXinSendType = 0);
			if(!g_WeiXinShareSessionSavedData)
                g_WeiXinShareSessionSavedData = new BWSendMessageContentClass;
            XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
            StUTF8String str(text);
            if(!g_WeiXinShareSessionSavedData->parameter1)
            {
                g_WeiXinShareSessionSavedData->parameter1 = malloc(str.length() + 1);
                g_WeiXinShareSessionSavedData->parameter1Len = str.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter1, str.c_str(), str.length() + 1);
            }
            lock.UnLock();
			XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_WXSENDTEXT,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}

		int  WeiXinShareSessionObject::AS3_sendPhoto(int sendType, BitmapDataObject *picData)
		{
			if(picData == XNULL) return 0;
            if(!g_WeiXinAppIsInstalled)
				return 0;
            sendType < 2 ? (g_WeiXinSendType = sendType) : (g_WeiXinSendType = 0);
            if(!g_WeiXinShareSessionSavedData)
                g_WeiXinShareSessionSavedData = new BWSendMessageContentClass;
            XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
            //DataFile datafile;// = picData->GetByteArray();
			//XU8Array datafile;
			//picData->Encode(datafile);
            if(!g_WeiXinShareSessionSavedData->parameter1)
            {
				//bool bRelease = false;
				XDrawDib dib;
				//SBitmapCore* bits = &picData->m_pChar->bits;
				if(picData->GetDib(dib))
				{
					int width = dib.Width();//bits->width;
					int height = dib.Height();//bits->height;
					int length = 4 * width * height;
					g_WeiXinShareSessionSavedData->parameter1Width = width;
					g_WeiXinShareSessionSavedData->parameter1Height = height;
					g_WeiXinShareSessionSavedData->parameter1 = malloc(length);
					g_WeiXinShareSessionSavedData->parameter1Len = length;
					//memcpy(g_WeiXinShareSessionSavedData->parameter1, datafile.c_str(), datafile.length() + 1);
					memcpy(g_WeiXinShareSessionSavedData->parameter1, (void *)dib.GetAddress(), length);
					//if(bRelease)
					//	picData->CreateTexture();
				}
            }
            lock.UnLock();
            XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_WXSENDPHOTO,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}

		int  WeiXinShareSessionObject::AS3_sendLink(int sendType, Stringp title, Stringp description, BitmapDataObject * thumbPicData, Stringp webpageUrl)
		{
			//if(thumbPicData == XNULL) return 0;
			if(!g_WeiXinAppIsInstalled)
				return 0;
            sendType < 2 ? (g_WeiXinSendType = sendType) : (g_WeiXinSendType = 0);
            if(!g_WeiXinShareSessionSavedData)
                g_WeiXinShareSessionSavedData = new BWSendMessageContentClass;
            XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
            StUTF8String titleUTF8(title);
            if(!g_WeiXinShareSessionSavedData->parameter1)
            {
                g_WeiXinShareSessionSavedData->parameter1 = malloc(titleUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter1Len = titleUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter1, titleUTF8.c_str(), titleUTF8.length() + 1);
            }
            StUTF8String descriptionUTF8(description);
            if(!g_WeiXinShareSessionSavedData->parameter2)
            {
                g_WeiXinShareSessionSavedData->parameter2 = malloc(descriptionUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter2Len = descriptionUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter2, descriptionUTF8.c_str(), descriptionUTF8.length() + 1);
            }
            //DataFile datafile = thumbPicData->GetByteArray();
			//XU8Array data;
			//thumbPicData->Encode(data);
            if(!g_WeiXinShareSessionSavedData->parameter3)
            {
				XDrawDib dib;
				if(thumbPicData->GetDib(dib))
				{
					int width = dib.Width();//bits->width;//thumbPicData->m_pDib->Width();
					int height = dib.Height();//bits->height;//thumbPicData->m_pDib->Height();
					int length = 4 * width * height;
					g_WeiXinShareSessionSavedData->parameter3Width = width;
					g_WeiXinShareSessionSavedData->parameter3Height = height;
					//g_WeiXinShareSessionSavedData->parameter3 = malloc(data.GetSize());//thumbPicData->GetLength());
					g_WeiXinShareSessionSavedData->parameter3 = malloc(length);
					//g_WeiXinShareSessionSavedData->parameter3Len = data.GetSize();//thumbPicData->GetLength();
					g_WeiXinShareSessionSavedData->parameter3Len = length;
					//memcpy(g_WeiXinShareSessionSavedData->parameter3, (void *)data.GetData(), data.GetSize());//thumbPicData->GetLength());
					memcpy(g_WeiXinShareSessionSavedData->parameter3, dib.GetAddress(), length);
					//if(bRelease)
					//	thumbPicData->CreateTexture();
				}
            }
            StUTF8String webpageUrlUTF8(webpageUrl);
            if(!g_WeiXinShareSessionSavedData->parameter4)
            {
                g_WeiXinShareSessionSavedData->parameter4 = malloc(webpageUrlUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter4Len = webpageUrlUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter4, webpageUrlUTF8.c_str(), webpageUrlUTF8.length() + 1);
            }
            lock.UnLock();
            XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_WXSENDLINK,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}

		int  WeiXinShareSessionObject::AS3_sendMusic(int sendType, Stringp title, Stringp description, BitmapDataObject * thumbPicData, Stringp musicUrl, Stringp musicDataUrl)
		{
			if(!g_WeiXinAppIsInstalled)
				return 0;
            sendType < 2 ? (g_WeiXinSendType = sendType) : (g_WeiXinSendType = 0);
            if(!g_WeiXinShareSessionSavedData)
                g_WeiXinShareSessionSavedData = new BWSendMessageContentClass;
            XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
            StUTF8String titleUTF8(title);
            if(!g_WeiXinShareSessionSavedData->parameter1)
            {
                g_WeiXinShareSessionSavedData->parameter1 = malloc(titleUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter1Len = titleUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter1, titleUTF8.c_str(), titleUTF8.length() + 1);
            }
            StUTF8String descriptionUTF8(description);
            if(!g_WeiXinShareSessionSavedData->parameter2)
            {
                g_WeiXinShareSessionSavedData->parameter2 = malloc(descriptionUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter2Len = descriptionUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter2, descriptionUTF8.c_str(), descriptionUTF8.length() + 1);
            }
           // DataFile datafile = thumbPicData->GetByteArray();
			//XU8Array data;
			//thumbPicData->Encode(data);
            if(!g_WeiXinShareSessionSavedData->parameter3)
            {
				XDrawDib dib;
				if(thumbPicData->GetDib(dib))
				{
				
					int width = dib.Width();//bits->width;
					int height = dib.Height();//bits->height;
					int length = 4 * width * height;
					g_WeiXinShareSessionSavedData->parameter3Width = width;
					g_WeiXinShareSessionSavedData->parameter3Height = height;
					g_WeiXinShareSessionSavedData->parameter3Len = length;
					// g_WeiXinShareSessionSavedData->parameter3 = malloc(data.GetSize());//thumbPicData->GetLength());
					g_WeiXinShareSessionSavedData->parameter3 = malloc(length);
					//g_WeiXinShareSessionSavedData->parameter3Len = data.GetSize();//thumbPicData->GetLength();
					g_WeiXinShareSessionSavedData->parameter3Len = length;
					//memcpy(g_WeiXinShareSessionSavedData->parameter3, data.GetData(),data.GetSize());//(void *)datafile.GetBuffer(), thumbPicData->GetLength());
					//memcpy(g_WeiXinShareSessionSavedData->parameter3, data.GetData(),data.GetSize());
					memcpy(g_WeiXinShareSessionSavedData->parameter3, dib.GetAddress(), length);
				}
            }
            
            StUTF8String musicUrlUTF8(musicUrl);
            if(!g_WeiXinShareSessionSavedData->parameter4)
            {
                g_WeiXinShareSessionSavedData->parameter4 = malloc(musicUrlUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter4Len = musicUrlUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter4, musicUrlUTF8.c_str(), musicUrlUTF8.length() + 1);
            }
            StUTF8String musicDataUrlUTF8(musicDataUrl);
            if(!g_WeiXinShareSessionSavedData->parameter5)
            {
                g_WeiXinShareSessionSavedData->parameter5 = malloc(musicDataUrlUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter5Len = musicDataUrlUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter5, musicDataUrlUTF8.c_str(), musicDataUrlUTF8.length() + 1);
            }
            lock.UnLock();
            XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_WXSENDMUSIC,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}

		int  WeiXinShareSessionObject::AS3_sendVedio(int sendType, Stringp title, Stringp description, BitmapDataObject * thumbPicData, Stringp videoUrl)
		{
			if(!g_WeiXinAppIsInstalled)
				return 0;
            sendType < 2 ? (g_WeiXinSendType = sendType) : (g_WeiXinSendType = 0);
            if(!g_WeiXinShareSessionSavedData)
                g_WeiXinShareSessionSavedData = new BWSendMessageContentClass;
            XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
            StUTF8String titleUTF8(title);
            if(!g_WeiXinShareSessionSavedData->parameter1)
            {
                g_WeiXinShareSessionSavedData->parameter1 = malloc(titleUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter1Len = titleUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter1, titleUTF8.c_str(), titleUTF8.length() + 1);
            }
            StUTF8String descriptionUTF8(description);
            if(!g_WeiXinShareSessionSavedData->parameter2)
            {
                g_WeiXinShareSessionSavedData->parameter2 = malloc(descriptionUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter2Len = descriptionUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter2, descriptionUTF8.c_str(), descriptionUTF8.length() + 1);
            }
            //DataFile datafile = thumbPicData->GetByteArray();
			//XU8Array data;
			//thumbPicData->Encode(data);
            if(!g_WeiXinShareSessionSavedData->parameter3)
            {
				XDrawDib dib;
				if(thumbPicData->GetDib(dib))
				{
					int width = dib.Width();//bits->width;
					int height = dib.Height();//bits->height;
					int length = 4 * width * height;
					g_WeiXinShareSessionSavedData->parameter3Width = width;
					g_WeiXinShareSessionSavedData->parameter3Height = height;
					g_WeiXinShareSessionSavedData->parameter3Len = length;
					//g_WeiXinShareSessionSavedData->parameter3 = malloc(data.GetSize());//thumbPicData->GetLength());
					g_WeiXinShareSessionSavedData->parameter3 = malloc(length);
					//g_WeiXinShareSessionSavedData->parameter3Len = data.GetSize();//thumbPicData->GetLength();
					g_WeiXinShareSessionSavedData->parameter3Len = length;
					//memcpy(g_WeiXinShareSessionSavedData->parameter3, data.GetData(),data.GetSize());//(void *)datafile.GetBuffer(), thumbPicData->GetLength());
					memcpy(g_WeiXinShareSessionSavedData->parameter3, dib.GetAddress(), length);
				}
            }
            StUTF8String videoUrlUTF8(videoUrl);
            if(!g_WeiXinShareSessionSavedData->parameter4)
            {
                g_WeiXinShareSessionSavedData->parameter4 = malloc(videoUrlUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter4Len = videoUrlUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter4, videoUrlUTF8.c_str(), videoUrlUTF8.length() + 1);
            }
            lock.UnLock();
            XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_WXSENDVEDIO,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}

		int  WeiXinShareSessionObject::AS3_sendAppMessage(int sendType, Stringp title, Stringp description, BitmapDataObject * thumbPicData, Stringp extInfo, Stringp url, ByteArrayObject * fileData)
		{
			if(!g_WeiXinAppIsInstalled)
				return 0;
            sendType < 2 ? (g_WeiXinSendType = sendType) : (g_WeiXinSendType = 0);
            if(!g_WeiXinShareSessionSavedData)
                g_WeiXinShareSessionSavedData = new BWSendMessageContentClass;
            XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
            StUTF8String titleUTF8(title);
            if(!g_WeiXinShareSessionSavedData->parameter1)
            {
                g_WeiXinShareSessionSavedData->parameter1 = malloc(titleUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter1Len = titleUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter1, titleUTF8.c_str(), titleUTF8.length() + 1);
            }
            StUTF8String descriptionUTF8(description);
            if(!g_WeiXinShareSessionSavedData->parameter2)
            {
                g_WeiXinShareSessionSavedData->parameter2 = malloc(descriptionUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter2Len = descriptionUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter2, descriptionUTF8.c_str(), descriptionUTF8.length() + 1);
            }
            //DataFile datafile = thumbPicData->GetByteArray();
			//XU8Array data;
			//thumbPicData->Encode(data);
            if(!g_WeiXinShareSessionSavedData->parameter3)
            {
				XDrawDib dib;
				if(thumbPicData->GetDib(dib))
				{
					int width = dib.Width();//bits->width;
					int height = dib.Height();//bits->height;
					int length = 4 * width * height;
					g_WeiXinShareSessionSavedData->parameter3Width = width;
					g_WeiXinShareSessionSavedData->parameter3Height = height;
					g_WeiXinShareSessionSavedData->parameter3Len = length;
					//g_WeiXinShareSessionSavedData->parameter3 = malloc(data.GetSize());//thumbPicData->GetLength());
					g_WeiXinShareSessionSavedData->parameter3 = malloc(length);
					//g_WeiXinShareSessionSavedData->parameter3Len = data.GetSize();//thumbPicData->GetLength();
					//memcpy(g_WeiXinShareSessionSavedData->parameter3, data.GetData(),data.GetSize());//(void *)datafile.GetBuffer(), thumbPicData->GetLength());
					memcpy(g_WeiXinShareSessionSavedData->parameter3, dib.GetAddress(), length);
				}
            }
            
            StUTF8String extInfoUTF8(extInfo);
            if(!g_WeiXinShareSessionSavedData->parameter4)
            {
                g_WeiXinShareSessionSavedData->parameter4 = malloc(extInfoUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter4Len = extInfoUTF8. length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter4, extInfoUTF8.c_str(), extInfoUTF8.length() + 1);
            }
            StUTF8String urlUTF8(url);
            if(!g_WeiXinShareSessionSavedData->parameter5)
            {
                g_WeiXinShareSessionSavedData->parameter5 = malloc(urlUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter5Len = urlUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter5, urlUTF8.c_str(), urlUTF8.length() + 1);
            }
            DataFile datafile1 = fileData->GetByteArray();
            if(!g_WeiXinShareSessionSavedData->parameter6)
            {
                g_WeiXinShareSessionSavedData->parameter6 = malloc(fileData->GetLength());
                g_WeiXinShareSessionSavedData->parameter6Len = fileData->GetLength();
                memcpy(g_WeiXinShareSessionSavedData->parameter6, (void *)datafile1.GetBuffer(), fileData->GetLength());
            }
            lock.UnLock();
            XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_WXSENDAPPMESSAGE,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}

		int  WeiXinShareSessionObject::AS3_sendNotGifContent(int sendType, BitmapDataObject *thumbPicData,ByteArrayObject *emotionData)
		{
			if(!g_WeiXinAppIsInstalled)
				return 0;
            sendType < 2 ? (g_WeiXinSendType = sendType) : (g_WeiXinSendType = 0);
            if(!g_WeiXinShareSessionSavedData)
                g_WeiXinShareSessionSavedData = new BWSendMessageContentClass;
            XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
            //DataFile datafile = thumbPicData->GetByteArray();
			//XU8Array data;
			//thumbPicData->Encode(data);
            if(!g_WeiXinShareSessionSavedData->parameter1)
            {
				XDrawDib dib;
				if(thumbPicData->GetDib(dib))
				{
					int width = dib.Width();//bits->width;
					int height = dib.Height();//bits->height;
					int length = 4 * width * height;
					g_WeiXinShareSessionSavedData->parameter1Width = width;
					g_WeiXinShareSessionSavedData->parameter1Height = height;
					g_WeiXinShareSessionSavedData->parameter1Len = length;
					//g_WeiXinShareSessionSavedData->parameter1 = malloc(data.GetSize());//thumbPicData->GetLength());
					g_WeiXinShareSessionSavedData->parameter1 = malloc(length);
					//g_WeiXinShareSessionSavedData->parameter1Len = data.GetSize();//thumbPicData->GetLength();
					//memcpy(g_WeiXinShareSessionSavedData->parameter1, data.GetData(),data.GetSize());//(void *)datafile.GetBuffer(), thumbPicData->GetLength());
					memcpy(g_WeiXinShareSessionSavedData->parameter1, dib.GetAddress(), length);
				}
            }
            DataFile datafile1 = emotionData->GetByteArray();
            if(!g_WeiXinShareSessionSavedData->parameter2)
            {
                g_WeiXinShareSessionSavedData->parameter2 = malloc(emotionData->GetLength());
                g_WeiXinShareSessionSavedData->parameter2Len = emotionData->GetLength();
                memcpy(g_WeiXinShareSessionSavedData->parameter2, (void *)datafile1.GetBuffer(), emotionData->GetLength());
            }
            lock.UnLock();
            XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_WXSENDNOTGIFCONTENT,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}

		int  WeiXinShareSessionObject::AS3_sendGifContent(int sendType, BitmapDataObject *thumbPicData,ByteArrayObject * emotionData)
		{
			if(!g_WeiXinAppIsInstalled)
				return 0;
            sendType < 2 ? (g_WeiXinSendType = sendType) : (g_WeiXinSendType = 0);
            if(!g_WeiXinShareSessionSavedData)
                g_WeiXinShareSessionSavedData = new BWSendMessageContentClass;
            XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
            //DataFile datafile = thumbPicData->GetByteArray();
			//XU8Array data;
			//thumbPicData->Encode(data);
            if(!g_WeiXinShareSessionSavedData->parameter1)
            {
				XDrawDib dib;
				if(thumbPicData->GetDib(dib))
				{
					int width = dib.Width();//bits->width;
					int height = dib.Height();//bits->height;
					int length = 4 * width * height;
					g_WeiXinShareSessionSavedData->parameter1Width = width;
					g_WeiXinShareSessionSavedData->parameter1Height = height;
					g_WeiXinShareSessionSavedData->parameter1Len = length;
					//g_WeiXinShareSessionSavedData->parameter1 = malloc(data.GetSize());//thumbPicData->GetLength());
					g_WeiXinShareSessionSavedData->parameter1 = malloc(length);
					//g_WeiXinShareSessionSavedData->parameter1Len = data.GetSize();//thumbPicData->GetLength();
					//memcpy(g_WeiXinShareSessionSavedData->parameter1, data.GetData(),data.GetSize());//(void *)datafile.GetBuffer(), thumbPicData->GetLength());
					memcpy(g_WeiXinShareSessionSavedData->parameter1, dib.GetAddress(), length);
				}
            }
            DataFile datafile1 = emotionData->GetByteArray();
            if(!g_WeiXinShareSessionSavedData->parameter2)
            {
                g_WeiXinShareSessionSavedData->parameter2 = malloc(emotionData->GetLength());
                g_WeiXinShareSessionSavedData->parameter2Len = emotionData->GetLength();
                memcpy(g_WeiXinShareSessionSavedData->parameter2, (void *)datafile1.GetBuffer(), emotionData->GetLength());
            }
            lock.UnLock();
            XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_WXSENDNOTGIFCONTENT,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}

		int  WeiXinShareSessionObject::AS3_sendFileContent(int sendType, Stringp title, Stringp description, BitmapDataObject *thumbPicData, ByteArrayObject *fileData, Stringp fileExt)
		{
			if(!g_WeiXinAppIsInstalled)
				return 0;
            sendType < 2 ? (g_WeiXinSendType = sendType) : (g_WeiXinSendType = 0);
            if(!g_WeiXinShareSessionSavedData)
                g_WeiXinShareSessionSavedData = new BWSendMessageContentClass;
            XLock lock(&(g_WeiXinShareSessionSavedData->m_lock));
            StUTF8String titleUTF8(title);
            if(!g_WeiXinShareSessionSavedData->parameter1)
            {
                g_WeiXinShareSessionSavedData->parameter1 = malloc(titleUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter1Len = titleUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter1, titleUTF8.c_str(), titleUTF8.length() + 1);
            }
            StUTF8String descriptionUTF8(description);
            if(!g_WeiXinShareSessionSavedData->parameter2)
            {
                g_WeiXinShareSessionSavedData->parameter2 = malloc(descriptionUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter2Len = descriptionUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter2, descriptionUTF8.c_str(), descriptionUTF8.length() + 1);
            }
            //DataFile datafile = thumbPicData->GetByteArray();
			//XU8Array data;
			//thumbPicData->Encode(data);
            if(!g_WeiXinShareSessionSavedData->parameter3)
            {
				XDrawDib dib;
				if(thumbPicData->GetDib(dib))
				{
					int width = dib.Width();//bits->width;
					int height = dib.Height();//bits->height;
					int length = 4 * width * height;
					g_WeiXinShareSessionSavedData->parameter3Width = width;
					g_WeiXinShareSessionSavedData->parameter3Height = height;
					g_WeiXinShareSessionSavedData->parameter3Len = length;
					//g_WeiXinShareSessionSavedData->parameter3 = malloc(data.GetSize());//thumbPicData->GetLength());
					g_WeiXinShareSessionSavedData->parameter3 = malloc(length);
					//g_WeiXinShareSessionSavedData->parameter3Len = data.GetSize();//thumbPicData->GetLength();
					//memcpy(g_WeiXinShareSessionSavedData->parameter3, data.GetData(),data.GetSize());//(void *)datafile.GetBuffer(), thumbPicData->GetLength());
					memcpy(g_WeiXinShareSessionSavedData->parameter3, dib.GetAddress(), length);
				}
            }
            DataFile datafile1 = fileData->GetByteArray();
            if(!g_WeiXinShareSessionSavedData->parameter4)
            {
                g_WeiXinShareSessionSavedData->parameter4 = malloc(fileData->GetLength());
                g_WeiXinShareSessionSavedData->parameter4Len = fileData->GetLength();
                memcpy(g_WeiXinShareSessionSavedData->parameter4, (void *)datafile1.GetBuffer(), fileData->GetLength());
            }
            StUTF8String fileExtUTF8(fileExt);
            if(!g_WeiXinShareSessionSavedData->parameter5)
            {
                g_WeiXinShareSessionSavedData->parameter5 = malloc(fileExtUTF8.length() + 1);
                g_WeiXinShareSessionSavedData->parameter5Len = fileExtUTF8.length() + 1;
                memcpy(g_WeiXinShareSessionSavedData->parameter5, fileExtUTF8.c_str(), fileExtUTF8.length() + 1);
            }
            lock.UnLock();
            XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_WXSENDFILECONTENT,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}
	//////////////////////////////////////////////////////////////////////////

}
