#include "StdAfxflash.h"


#include "ShellCore.h"
using namespace avmplus;
#include "avm2.h"
#include "AvmCore.h"
#include "AS3BwPayAPI.h"
#include "StdAfxggdom.h"

namespace avmshell
{
	BwPayAPIClass::BwPayAPIClass(VTable *vtable):ClassClosure( vtable )
	{
		createVanillaPrototype();
	}
	ScriptObject* BwPayAPIClass::createInstance(VTable* ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) BwPayAPIObject(ivtable, prototype, 0);
	}

	BwPayAPIObject* BwPayAPIClass::AS3_GetBwPayAPI()
	{
		BwPayAPIObject *pObj = ( BwPayAPIObject *)createInstance( ivtable(), prototype );
		return pObj;
	}


	BwPayAPIObject::BwPayAPIObject(VTable* vtable, ScriptObject* proto, int capacity) : ScriptObject(vtable, proto, 0)
	{

	}

	void BwPayAPIObject::AS3_payAlipay( Stringp url )
	{
		XWindow*pWnd=XWindow::m_pMainWnd;
			if(pWnd)
		        {
				pWnd->PostMsg(XWM_BWPAYURL,(XU32)url,0,XMS_TOPMOST);
			}
		return;
	}

	//////////////////////////////////////////////////////////////////////////

}
