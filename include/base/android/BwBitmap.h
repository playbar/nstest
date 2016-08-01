#include "SkBitmap.h"
#include "android_runtime/AndroidRuntime.h"
#include "GraphicsJNI.h"
using namespace android;


class BwBitmap
{
protected:
	JavaVM* jvm;
	JNIEnv* env;

	SkBitmap *m_Bitmap;
	SkBitmap *m_tmp_Bitmap;

	int width;
	int height;
	int size;

	jclass bmClass;
	jmethodID bm_recycle_MID;
	jmethodID bm_createBitmap_MID;
	jmethodID bm_getWidth_MID;
	jmethodID bm_getHeight_MID;

	jclass bcClass;
	jmethodID bc_valueOf_MID;

	jobject jstr_Config;

	jobject bcObject;
	jobject bmObject;
	jobject bm_tmp_Object;

	jclass bfClass;
	jmethodID bf_decodeByteArray_MID;

	void myInit();
	void createBitmap();

public:
	BwBitmap(int w, int h);
	BwBitmap(int w, int h, void* data); // 非解码
	BwBitmap(void* src, int offset, int length); // 解码
	~BwBitmap();

	SkBitmap * getBitmap();
	void * getDataAddr();
	int getSize();
	int getWidth();
	int getHeight();



};
