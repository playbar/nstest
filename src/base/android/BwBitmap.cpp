#include "BwBitmap.h"

#include <stdio.h>
#include <stdlib.h>

long writeToFile(void* data, long size)
{
	char filename[] = "/sdcard/aa.tmp";
	FILE *fp = fopen(filename,"w+");
//	fseek(fp,0,SEEK_END);
//	int len = ftell(fp);
//	rewind(fp);

	LOGWHEREVAL(data);
	LOGWHEREVAL(size);
	LOGWHEREVAL(fp);

	long ret = fwrite(data,1,size,fp);

//	void * fData = malloc(len);
//	fread(fData,1,len,fp);
	fclose(fp);

	return ret;
}
void BwBitmap::getJavaEnv()
{
	if (env == 0)
	{
		env = AndroidRuntime::getJNIEnv();
		if (env == 0)
		{
			if (jvm == 0)
			{
				jvm = AndroidRuntime::getJavaVM();
			}
			jvm->AttachCurrentThread(&env, 0);
		}
	}
}
int BwBitmap::getSize()
{
//	size = getWidth() * getHeight() * 4;
	return size;
}
int BwBitmap::getWidth()
{
//	getJavaEnv();
//	width = env->CallIntMethod(bmObject,bm_getWidth_MID);
	return width;
}
int BwBitmap::getHeight()
{
//	getJavaEnv();
//	height = env->CallIntMethod(bmObject,bm_getHeight_MID);
	return height;
}

void * BwBitmap::getDataAddr()
{
	void * p = m_Bitmap->getAddr(0,0);
	return p;
}
void BwBitmap::myInit()
{
	jvm = 0;
	env = 0;

	getJavaEnv();
	jobject tmp;

	tmp = env->FindClass("android/graphics/Bitmap");
	bmClass = (jclass)env->NewGlobalRef(tmp);
	env->DeleteLocalRef(tmp);

	bm_recycle_MID = env->GetMethodID(bmClass, "recycle", "()V");
	bm_createBitmap_MID = env->GetStaticMethodID(bmClass, "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
	bm_getHeight_MID = env->GetMethodID(bmClass, "getHeight", "()I");
	bm_getWidth_MID = env->GetMethodID(bmClass, "getWidth", "()I");

	tmp = env->FindClass("android/graphics/Bitmap$Config");
	bcClass = (jclass)env->NewGlobalRef(tmp);
	env->DeleteLocalRef(tmp);

	bc_valueOf_MID = env->GetStaticMethodID(bcClass, "valueOf", "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");

	tmp = env->FindClass("android/graphics/BitmapFactory");
	bfClass = (jclass)env->NewGlobalRef(tmp);
	env->DeleteLocalRef(tmp);

	bf_decodeByteArray_MID = env->GetStaticMethodID(bfClass, "decodeByteArray", "([BII)Landroid/graphics/Bitmap;");
}
void BwBitmap::createBitmap()
{
	jobject tmp;

	tmp = env->NewStringUTF( "ARGB_8888");
	jstr_Config = env->NewGlobalRef(tmp);
	env->DeleteLocalRef(tmp);
	tmp = env->CallStaticObjectMethod(bcClass, bc_valueOf_MID, jstr_Config);
	bcObject = env->NewGlobalRef(tmp);
	env->DeleteLocalRef(tmp);

	tmp = env->CallStaticObjectMethod(bmClass, bm_createBitmap_MID, width, height, bcObject);
	bmObject = env->NewGlobalRef(tmp);
	env->DeleteLocalRef(tmp);
	m_Bitmap = GraphicsJNI::getNativeBitmap(env, bmObject);
//	m_Bitmap->eraseRGB( 0x0, 0x0, 0x0 );

	tmp = env->CallStaticObjectMethod(bmClass, bm_createBitmap_MID, width, height, bcObject);
	bm_tmp_Object = env->NewGlobalRef(tmp);
	env->DeleteLocalRef(tmp);
	m_tmp_Bitmap = GraphicsJNI::getNativeBitmap(env, bm_tmp_Object);
//	m_tmp_Bitmap->eraseRGB( 0x0, 0x0, 0x0 );

	m_Bitmap->copyTo(m_tmp_Bitmap,SkBitmap::kARGB_8888_Config);
	m_tmp_Bitmap->copyTo(m_Bitmap,SkBitmap::kARGB_8888_Config);

	void* p = m_Bitmap->getAddr(0,0);
	memset(p,0,size);

}
SkBitmap * BwBitmap::getBitmap()
{
//	getJavaEnv();
//	m_Bitmap = GraphicsJNI::getNativeBitmap(env, bmObject);
	return m_Bitmap;
}
BwBitmap::BwBitmap(void* src, int offset, int length)
{
	myInit();

	jbyteArray jba =  env->NewByteArray(length);
	env->SetByteArrayRegion(jba,0,length,(jbyte*)src);
	jobject tmp = env->CallStaticObjectMethod(bfClass, bf_decodeByteArray_MID, jba, offset, length);

	width = env->CallIntMethod(tmp,bm_getWidth_MID);
	height = env->CallIntMethod(tmp,bm_getHeight_MID);
	size = width*height*4;

	createBitmap();

	SkBitmap *skBMPTmp = GraphicsJNI::getNativeBitmap(env, tmp);
	skBMPTmp->copyTo(m_tmp_Bitmap,SkBitmap::kARGB_8888_Config);
	m_tmp_Bitmap->copyTo(skBMPTmp,SkBitmap::kARGB_8888_Config);

	void * data = skBMPTmp->getAddr(0,0);
	void * dst    = m_Bitmap->getAddr(0,0);
	memcpy(dst,data,size);

	env->DeleteLocalRef(jba);
	env->CallVoidMethod(tmp,bm_recycle_MID);
	env->DeleteLocalRef(tmp);
	jba = 0;
}
BwBitmap::BwBitmap(int w, int h, void* data)
{
	myInit();

	width = w;
	height = h;
	size = w*h*4;

	createBitmap();

	void *dst = m_Bitmap->getAddr(0,0);
	memcpy(dst,data,size);
}

BwBitmap::BwBitmap(int w, int h)
{
	myInit();

	width = w;
	height = h;
	size = w*h*4;

	createBitmap();
}
BwBitmap::~BwBitmap()
{
	getJavaEnv();
	if (bcClass != 0)
	{
		env->DeleteGlobalRef(bcClass);
		bcClass = 0;
	}
	if (bcObject != 0)
	{
		env->DeleteGlobalRef(bcObject);
		bcObject = 0;
	}
	if (jstr_Config != 0)
	{
		env->DeleteGlobalRef(jstr_Config);
		jstr_Config = 0;
	}
	if (bmClass != 0)
	{
		env->DeleteGlobalRef(bmClass);
		bmClass = 0;
	}
	if (bmObject != 0)
	{
		env->CallVoidMethod(bmObject,bm_recycle_MID);
		env->DeleteGlobalRef(bmObject);
		bmObject = 0;
		m_Bitmap = 0;
	}
	if (bm_tmp_Object != 0)
	{
		env->CallVoidMethod(bm_tmp_Object,bm_recycle_MID);
		env->DeleteGlobalRef(bm_tmp_Object);
		bm_tmp_Object = 0;
		m_tmp_Bitmap = 0;
	}

	bm_recycle_MID = 0;
	bm_createBitmap_MID =0;
	bm_getHeight_MID = 0;
	bm_getWidth_MID = 0;

	bc_valueOf_MID = 0;
	bf_decodeByteArray_MID = 0;

	width = 0;
	height = 0;
	size = 0;
	env = 0;

	if (jvm != 0)
	{
		jvm->DetachCurrentThread();
		jvm = 0;
	}
}






