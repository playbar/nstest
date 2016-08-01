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
#include "AS3DuoMengSDKInterface.h"
#include "avm2.h"
#include "AvmCore.h"
#include "StdAfxggdom.h"


//BWSendMessageContentClass *g_WeiXinShareSessionSavedData = NULL;
//bool g_WeiXinAppIsInstalled = true;
//int g_WeiXinSendType = 0;

namespace avmshell
{
	DuoMengSDKInterfaceClass::DuoMengSDKInterfaceClass(VTable *vtable):ClassClosure( vtable )
	{
		createVanillaPrototype();
	}
	ScriptObject* DuoMengSDKInterfaceClass::createInstance(VTable* ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) DuoMengSDKInterfaceObject(ivtable, prototype, 0);
	}

	DuoMengSDKInterfaceObject* DuoMengSDKInterfaceClass::AS3_GetDuoMengSDKInterface()
	{
		DuoMengSDKInterfaceObject *pObj = (DuoMengSDKInterfaceObject *)createInstance( ivtable(), prototype );
		return pObj;
	}

	DuoMengSDKInterfaceObject::DuoMengSDKInterfaceObject(VTable* vtable, ScriptObject* proto, int capacity) : ScriptObject(vtable, proto, 0)
	{

	}
	
	void DuoMengSDKInterfaceObject::Release()
	{

	}
	
	void DuoMengSDKInterfaceObject::SetObject(void*pObject)
	{

	}

	//bool DuoMengSDKInterfaceObject::AS3_isDuoMengSDKInterfaceIstalled()
	//{
	//	return g_DuoMengSDKInterfaceAppIsInstalled;
	//}

		int DuoMengSDKInterfaceObject::AS3_presentOfferWall()
		{

			XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
            {
				pWnd->PostMsg(XWM_YMSHOWOFFERWALL,0,0,XMS_TOPMOST);
				return 1;
			}
			return 0;
		}

//////////////////////////////////////////////////////////////////////////

}
