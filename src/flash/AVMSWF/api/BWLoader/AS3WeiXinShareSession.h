#ifndef _AS3WeiXinShareSession_
#define _AS3WeiXinShareSession_

class XDrawDib;
class SBitmapCore;
#include "AS3DisplayObject.h"

namespace avmplus
{
	namespace NativeID
	{
		class WeiXinShareSessionClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
		private:
		};
		class WeiXinShareSessionObjectSlots
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
	class WeiXinShareSessionObject;
	class ColorTransformObject;
	class MatrixObject;
	class ByteArrayObject;
	class BitmapDataObject;
	class DisplayObjectObject;

	class WeiXinShareSessionClass : public ClassClosure
	{
	public:
		WeiXinShareSessionClass(VTable *vtable);
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);
		WeiXinShareSessionObject* AS3_GetWeiXinShareSession();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WeiXinShareSessionClassSlots m_slots_WeiXinShareSessionClass;
	};

	class WeiXinShareSessionObject : public ScriptObject
	{
	public:
		WeiXinShareSessionObject(VTable* vtable, ScriptObject* proto, int capacity);
		virtual void Release();
		virtual void SetObject(void*pObject);

	public:
		bool AS3_isWeiXinIstalled();
		 //发送text消息给息给微信
 		 int AS3_sendText(int sendType, Stringp text);
 		 //发送photo消息给息给微信
 		 int AS3_sendPhoto(int sendType, BitmapDataObject *picData);
 		 //发送link消息给息给微信
 		 int AS3_sendLink(int sendType, Stringp title, Stringp description, BitmapDataObject * thumbPicData, Stringp webpageUrl);
 		 //发送music消息给息给微信
 		 int AS3_sendMusic(int sendType, Stringp title, Stringp description, BitmapDataObject * thumbPicData, Stringp musicUrl, Stringp musicDataUrl);
 		 //发送video消消息给息给微信
 		 int AS3_sendVedio(int sendType, Stringp title, Stringp description, BitmapDataObject * thumbPicData, Stringp videoUrl);
 		 //发送app消息给息给微信
 		 int AS3_sendAppMessage(int sendType, Stringp title, Stringp description, BitmapDataObject * thumbPicData, Stringp extInfo, Stringp url, ByteArrayObject * fileData);
 		 //发送非gif消息给息给微信,emotionData为非gif数据
 		 int AS3_sendNotGifContent(int sendType, BitmapDataObject *thumbPicData,ByteArrayObject *emotionData);
 		 //发送gif消息给息给微信,emotionData为gif数据
 		 int AS3_sendGifContent(int sendType, BitmapDataObject *thumbPicData,ByteArrayObject * emotionData);
 		 //发送文件消息给息给微信
 		 int AS3_sendFileContent(int sendType, Stringp title, Stringp description, BitmapDataObject *thumbPicData, ByteArrayObject *fileData, Stringp fileExt);

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WeiXinShareSessionObjectSlots m_slots_WeiXinShareSessionObject;
	};
}

#endif
