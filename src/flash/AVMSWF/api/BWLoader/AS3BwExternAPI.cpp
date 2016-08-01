#include "StdAfxflash.h"
#include "XDrawDib.h"
#include "Raster_gl.h"
#include "ShellCore.h"
#include "sobject.h"
#include "splay.h"
#include "splayer.h"
using namespace avmplus;
#include "AS3BwExternAPI.h"
#include "AS3Rectangle.h"
#include "AS3Point.h"
#include "AS3Matrix.h"
#include "AS3ColorTransform.h"
#include "AS3DisplayObject.h"
#include "AS3ByteArray.h"
#include "AS3ByteArray.h"
#include "AS3Bitmap.h"
#include "AS3BlendMode.h"
#include "avm2.h"
#include "AvmCore.h"

namespace avmshell
{
	BwExternAPIClass::BwExternAPIClass(VTable *vtable):ClassClosure( vtable )
	{
		createVanillaPrototype();
	}
	ScriptObject* BwExternAPIClass::createInstance(VTable* ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) BwExternAPIObject(ivtable, prototype, 0);
	}

	BwExternAPIObject* BwExternAPIClass::AS3_GetBwExternAPI()
	{
		BwExternAPIObject *pObj = ( BwExternAPIObject *)createInstance( ivtable(), prototype );
		return pObj;
	}


	BwExternAPIObject::BwExternAPIObject(VTable* vtable, ScriptObject* proto, int capacity) : ScriptObject(vtable, proto, 0)
	{

	}
	
	void BwExternAPIObject::Release()
	{

	}
	
	void BwExternAPIObject::SetObject(void*pObject)
	{

	}

	Stringp BwExternAPIObject::AS3_getDeviceType( )
	{
		ShellCore*c=(ShellCore*)core();
		Stringp buffer = c->newConstantStringLatin1("Error #");
		return buffer;
	}

	int	BwExternAPIObject::AS3_getHPixel()
	{
		return 0;
	}

	int	BwExternAPIObject::AS3_getVPixel()
	{
		return 0;
	}

	int	BwExternAPIObject::AS3_getDPI()
	{
		return 0;
	}

	int	BwExternAPIObject::AS3_SDKRegister( int type, Stringp username, Stringp passwd )
	{
		return 0;
	}

	int	BwExternAPIObject::AS3_SDKLogin( int type, Stringp username, Stringp passwd )
	{
		return 0;
	}

	int	BwExternAPIObject::AS3_SDKLogout( int type, Stringp username )
	{
		return 0;
	}

	int	BwExternAPIObject::AS3_SDKPayment( int type, Stringp username, Stringp passwd, int amount )
	{
		return 0;
	}

	int	BwExternAPIObject::AS3_SaveDisk( Stringp dataName, ByteArrayObject *dataString )
	{
		return 0;
	}

	ByteArrayObject* BwExternAPIObject::AS3_LoadDisk( Stringp dataName )
	{
		ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
		ByteArrayObject*pObj=pClass->CreateArray(0);
		return pObj;
	}

	int	BwExternAPIObject::AS3_Launch( int type, Stringp url )
	{
		return 0;
	}

	int	BwExternAPIObject::AS3_ShowToolbar( int type )
	{
		return 0;
	}

	void BwExternAPIObject::AS3_CloseSwf()
	{

	}

	//////////////////////////////////////////////////////////////////////////

}
