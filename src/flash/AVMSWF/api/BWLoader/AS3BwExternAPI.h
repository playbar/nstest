#ifndef _BwExternAPI_
#define _BwExternAPI_

class XDrawDib;
class SBitmapCore;
#include "AS3DisplayObject.h"

namespace avmplus
{
	namespace NativeID
	{
		class BwExternAPIClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
		private:
		};
		class BwExternAPIObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
		private:
		};
	}
}

namespace avmshell
{
	class RectangleObject;
	class PointObject;
	class BitmapFilterObject;
	class BwExternAPIObject;
	class ColorTransformObject;
	class MatrixObject;
	class ByteArrayObject;
	class DisplayObjectObject;

	class BwExternAPIClass : public ClassClosure
	{
	public:
		BwExternAPIClass(VTable *vtable);
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);
		BwExternAPIObject* AS3_GetBwExternAPI();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BwExternAPIClassSlots m_slots_BwExternAPIClass;
	};

	class BwExternAPIObject : public ScriptObject
	{
	public:
		BwExternAPIObject(VTable* vtable, ScriptObject* proto, int capacity);
		virtual void Release();
		virtual void SetObject(void*pObject);

	public:
		Stringp AS3_getDeviceType( );
		int		AS3_getHPixel();
		int		AS3_getVPixel();
		int		AS3_getDPI();

		int		AS3_SDKRegister( int type, Stringp username, Stringp passwd );
		int		AS3_SDKLogin( int type, Stringp username, Stringp passwd );
		int		AS3_SDKLogout( int type, Stringp username );
		int		AS3_SDKPayment( int type, Stringp username, Stringp passwd, int amount );
		int		AS3_SaveDisk( Stringp dataName, ByteArrayObject *dataString );
		ByteArrayObject* AS3_LoadDisk( Stringp dataName );

		int		AS3_Launch( int type, Stringp url );
		int		AS3_ShowToolbar( int type );
		void	AS3_CloseSwf();		

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BwExternAPIObjectSlots m_slots_BwExternAPIObject;
	};
}

#endif
