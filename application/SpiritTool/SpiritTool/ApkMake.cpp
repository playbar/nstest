#include "StdAfx.h"
#include "ApkMake.h"
#include "StaticUtil.h"
#include "global.h"
#include "message.h"


CApkMake::CApkMake(void)
{
	m_plOldPackageName = NULL;		//程序包名
	m_hthread = NULL;				//线程句柄
	m_hevent = NULL;				//事件对象
	m_hclose = NULL;				//关闭事件
	m_hclosed = NULL;				//以关闭事件
	m_nMakedCount = 0;
	m_pArray = NULL;
	m_pListener = NULL;
	m_plSrcpath = NULL;
	m_plSdkpath = NULL;
}


CApkMake::~CApkMake(void)
{
	end();
	if(m_plSrcpath != NULL) delete m_plSrcpath;
	if(m_plSdkpath != NULL) delete m_plSdkpath;
	if(m_plOldPackageName != NULL) delete m_plOldPackageName;
}

void CApkMake::setPath(const char* srcpath,const char* sdkpath)
{
	//源码路径
	if(srcpath == NULL || sdkpath == NULL) return;
	if(m_plSrcpath)
		delete m_plSrcpath;
	int len = strlen(srcpath);
	m_plSrcpath = new char[len+1];
	strcpy(m_plSrcpath,srcpath);

	//sdk路径
	if(m_plSdkpath)
		delete m_plSdkpath;
	len = strlen(sdkpath);
	m_plSdkpath = new char[len+1];
	strcpy(m_plSdkpath,sdkpath);

	//获取包名
	char fpath[256];
	strcpy(fpath,m_plSrcpath);
	strcat(fpath,"\\AndroidManifest.xml");
	FILE *fl;
	fl = fopen(fpath,"rb");
	if(fl)
	{
		int len;
		char buf[512];
		fread(buf,1,512,fl);
		buf[511] = 0;
		char* temp = buf;
		len = CStaticUtil::findstr(temp,"package=");
		temp+=len+9;
		len = CStaticUtil::findstr(temp,"\"");
		if(m_plOldPackageName) delete m_plOldPackageName;
		m_plOldPackageName = new char[len+1];
		memcpy(m_plOldPackageName,temp,len);
		m_plOldPackageName[len] = 0;
		fclose(fl);
	}
	m_nMakedCount=0;
}

//开始打包
void CApkMake::startMake()
{
	if(m_pArray == NULL || m_plSrcpath == NULL || m_plSdkpath == NULL) return;
	::SetEvent(m_hevent);
	::ResumeThread(m_hthread);
}

void CApkMake::stopMake()
{
	SetEvent(m_hclose);
	SetEvent(m_hevent);
	while(::WaitForSingleObject(m_hclosed,1000000)!=WAIT_OBJECT_0)
	{
		::SetEvent(m_hclose);
	}
}

void CApkMake::setListener(MakeListener* listener)
{
	m_pListener = listener;
}

//打包器控制
void CApkMake::start()
{
	if(m_hthread != NULL) return;
	m_hevent = ::CreateEvent(NULL,true,false,NULL);
	m_hclose = ::CreateEvent(NULL,true,false,NULL);
	m_hclosed = CreateEvent(NULL,true,false,NULL);
	DWORD threadId;
	m_hthread = CreateThread(NULL,0,ThreadProc,this,0,&threadId);
}
void CApkMake::end()
{
	stopMake();
#define R(M) {if(M) CloseHandle(M); M=NULL;}
	R(m_hthread);
	R(m_hevent);
	R(m_hclose);
	R(m_hclosed);
#undef R
}
void CApkMake::setDataArray(CPointerArray<CSpiritData>* data)
{
	m_pArray = data;
	m_nMakedCount = 0;
}
int CApkMake::getMakedCount()
{
	return m_nMakedCount;
}
int CApkMake::getMakingId()
{
	return 0;
}

DWORD CApkMake::CallBack()
{
	while(true)
	{
		while(::WaitForSingleObject(m_hevent,1000000)!=WAIT_OBJECT_0);
		if(WaitForSingleObject(m_hclose,0)==WAIT_OBJECT_0)
		{
			::SetEvent(m_hclosed);
			break;
		}
		//线程操作
		ready();
		go();
		delete m_plOldPackageName;
		m_plOldPackageName = new char[strlen(m_pArray->getData(m_nMakedCount)->m_hPackagename)+1];
		strcpy(m_plOldPackageName,m_pArray->getData(m_nMakedCount)->m_hPackagename);
		m_nMakedCount++;
		if(m_pListener)
			m_pListener->onListen(Kcomplete,m_nMakedCount);
		if(m_nMakedCount>=m_pArray->getSize())
		{
			if(m_pListener)
				m_pListener->onListen(Kover,0);
			::ResetEvent(m_hevent);
		}

	}
	return 0;
}

void CApkMake::ready()
{
	//将资源文件拷入指定文件夹，修改文件中指定字段
	CSpiritData* data = m_pArray->getData(m_nMakedCount);
	if(!data) return;
	//调制路径批处理
	char oldpath[256];
	char newpath[256];
	modulatepath(m_plOldPackageName,oldpath);
	modulatepath(data->m_hPackagename,newpath);
	FILE *fl;
	fl = fopen(ResourceConform,"w");
	if(fl)
	{
		fprintf(fl,"@echo off\r\n");
		fprintf(fl,"mkdir \"%s\"\r\n",newpath);
		fprintf(fl,"move \"%s\\*java\" \"%s\"\r\n",oldpath,newpath);
		fprintf(fl,"rd \"%s\"\r\n",oldpath);
		int len_src(0);
		len_src = CStaticUtil::findstr(oldpath,"src",0,false);
		len_src+=3;
		int len(0);
		int lenp(strlen(oldpath));
		int offset(0);
		char path[256];
		while(1)
		{
			len = CStaticUtil::findstr(oldpath,"\\",offset,false);
			if(len<=len_src) break;
			offset=lenp-len;
			memcpy(path,oldpath,len);
			path[len] = 0;
			fprintf(fl,"rd \"%s\"\r\n",path);
		}
		fprintf(fl,"copy \"%s\" \"%s\\res\\drawable\\ic_launcher.png\"\r\n",data->m_hIconpath,m_plSrcpath);
		fprintf(fl,"copy \"%s\" \"%s\\res\\drawable\\img_a.png\"\r\n",data->m_hImagepath1,m_plSrcpath);
		fprintf(fl,"copy \"%s\" \"%s\\res\\drawable\\img_b.png\"\r\n",data->m_hImagepath2,m_plSrcpath);
		fprintf(fl,"copy \"%s\" \"%s\\assets\\game.gif\"\r\n",data->m_hGifpath,m_plSrcpath);
		if(data->m_hBWpath)
			fprintf(fl,"copy \"%s\" \"%s\\assets\\ggclient.apk\"\r\n",data->m_hBWpath,m_plSrcpath);
		else
			fprintf(fl,"del \"%s\\assets\\ggclient.apk\"\r\n",m_plSrcpath);
		fprintf(fl,"@echo on\r\n");
		fclose(fl);
	}
	//修改文件
	strcat(oldpath,"\\GhostActivity.java");
	CStaticUtil::replaceFileContent(oldpath,this->m_plOldPackageName,data->m_hPackagename);
	::Sleep(50);

	strcpy(oldpath,m_plSrcpath);
	strcat(oldpath,"\\AndroidManifest.xml");
	CStaticUtil::replaceFileContent(oldpath,this->m_plOldPackageName,data->m_hPackagename);
	::Sleep(50);

	strcpy(oldpath,m_plSrcpath);
	strcat(oldpath,"\\src\\my\\global\\imageAdapter.java");
	CStaticUtil::replaceFileContent(oldpath,this->m_plOldPackageName,data->m_hPackagename);
	::Sleep(50);

	strcpy(oldpath,m_plSrcpath);
	strcat(oldpath,"\\src\\my\\global\\PackageReceiver.java");
	CStaticUtil::replaceFileContent(oldpath,this->m_plOldPackageName,data->m_hPackagename);
	::Sleep(50);

	strcpy(oldpath,m_plSrcpath);
	strcat(oldpath,"\\res\\values\\strings.xml");
	changestring(data->m_hAppname,data->m_hSpiritUA,data->m_hChannelId,oldpath);
	::Sleep(50);

	strcpy(oldpath,m_plSrcpath);
	strcat(oldpath,"\\src\\my\\global\\GlobalUtil.java");
	changeDesktop(data->m_bDesktop,oldpath);
	::Sleep(50);

	
	//生成资源文件批处理，包括R.java
	fl = fopen(MakeR,"w");
	if(fl)
	{
		
		//%PLATFORMS_TOOL%\aapt package -f -M %WORK_PATH%\AndroidManifest.xml -S %WORK_PATH%\res -A %WORK_PATH%\assets -I %PLATFORMS% -F %WORK_PATH%\bin\resources.ap_

		//%PLATFORMS_TOOL%\aapt package -f -m -J %WORK_PATH%\gen -S %WORK_PATH%\res -I %PLATFORMS% -M %WORK_PATH%\AndroidManifest.xml
		fprintf(fl,"@echo off\r\n");
		fprintf(fl,
			"%s\\platform-tools\\aapt package -f -M %s\\AndroidManifest.xml -S %s\\res -A %s\\assets -I %s\\platforms\\android-7\\android.jar -F %s\\bin\\resources.ap_\r\n",
			m_plSdkpath,
			m_plSrcpath,
			m_plSrcpath,
			m_plSrcpath,
			m_plSdkpath,
			m_plSrcpath);
		fprintf(fl,"%s\\platform-tools\\aapt package -f -m -J %s\\gen -S %s\\res -I %s\\platforms\\android-7\\android.jar -M %s\\AndroidManifest.xml\r\n",
			m_plSdkpath,
			m_plSrcpath,
			m_plSrcpath,
			m_plSdkpath,
			m_plSrcpath);
		fprintf(fl,"@echo on\r\n");
		fclose(fl);
	}
	//编译，并生成。dex文件
	fl = fopen(Build,"w");
	if(fl)
	{
		strcpy(oldpath,m_plSrcpath);
		strcat(oldpath,"\\gen\\");
		char* temp = oldpath+strlen(oldpath);
		int len = strlen(data->m_hPackagename);
		for(int i=0;i<len;i++)
		{
			if(data->m_hPackagename[i] == '.')
				temp[i] = '\\';
			else
				temp[i] = data->m_hPackagename[i];
		}
		temp[len] = 0;
		modulatepath(data->m_hPackagename,newpath);
		fprintf(fl,"@echo off\r\n");
		fprintf(fl,"javac -encoding UTF-8 -bootclasspath %s\\platforms\\android-7\\android.jar -d %s\\bin\\classes %s\\*.java %s\\R.java %s\\src\\my\\global\\*.java\r\n",
			m_plSdkpath,
			m_plSrcpath,
			newpath,
			oldpath,
			m_plSrcpath
			);
		fprintf(fl,"%s\\platform-tools\\dx --dex --output=%s\\bin\\classes.dex %s\\bin\\classes\r\n",m_plSdkpath,m_plSrcpath,m_plSrcpath);
		fprintf(fl,"@echo on\r\n");
		fclose(fl);
	}
	//生成打包批处理
	fl = fopen(MakePackage,"w");
	if(fl)
	{
		fprintf(fl,"@echo off\r\n");
		fprintf(fl,"%s\\tools\\apkbuilder %s\\bin\\test.apk -u -z %s\\bin\\resources.ap_ -f %s\\bin\\classes.dex -rf %s\\src\r\n",
			m_plSdkpath,
			m_plSrcpath,
			m_plSrcpath,
			m_plSrcpath,
			m_plSrcpath);
		fprintf(fl,"@echo on\r\n");
		fclose(fl);
	}
	//给包签名
	fl = fopen(Signature,"w");
	if(fl)
	{
		fprintf(fl,"@echo off\r\n");
		fprintf(fl,"jarsigner -keystore %s\\bin\\spirit.keystore -storepass spirit001 -keypass spirit007 -signedjar %s\\bin\\testkey.apk %s\\bin\\test.apk test\r\n",
			m_plSrcpath,
			m_plSrcpath,
			m_plSrcpath);
		fprintf(fl,"copy \"%s\\bin\\testkey.apk\" \"%s\\%s\"\r\n",m_plSrcpath,data->m_hOutpath,data->m_hApkname);
		fprintf(fl,"@echo on\r\n");
		fclose(fl);
	}
}
void CApkMake::go()
{
	//开始执行脚本
	//资源文件
	::system(ResourceConform);
	//生成资源文件
	system(MakeR);
	//编译
	system(Build);
	//打包
	system(MakePackage);
	//签名
	system(Signature);
}

void CApkMake::modulatepath(const char *packagename,char *path)	//调制路径
{
	strcpy(path,m_plSrcpath);
	strcat(path,"\\src");
	int len(0),lens(0);
	char *temp = (char*)packagename;
	lens = strlen(path);
	while(1)
	{
		len = CStaticUtil::findstr(temp,".",1);
		if(len<0) break;
		strcat(path,"\\");
		lens+=1;
		memcpy(path+lens,temp,len);
		lens+=len;
		path[lens] = 0;
		temp+=len+1;
	}
	strcat(path,"\\");
	strcat(path,temp);
	
}

void CApkMake::changestring(const char *appname,const char* ua,const char* channel,const char* fname)	//修改strings.xml
{
	FILE *fl;
	fl = fopen(fname,"rb+");
	if(!fl) return;
		::fseek(fl,0,SEEK_END);
		int flen(ftell(fl));
		fseek(fl,0,SEEK_SET);
		char *buf = new char[flen];
		char *temp = buf;
		fread(temp,1,flen,fl);
	fclose(fl);
	fl = fopen(fname,"wb+");
		int len1(0),len2(0);
		//处理appname
		len1 = CStaticUtil::findstr(temp,"app_name");
		len1+=8+2;
		::fwrite(temp,1,len1,fl);
		temp+=len1;
		len2 = CStaticUtil::findstr(temp,"</");
		char* appnameutf8=NULL;
		len1 = CStaticUtil::GbkToUtf8(appnameutf8,appname);
		fwrite(appnameutf8,1,strlen(appnameutf8),fl);
		delete appnameutf8;
		temp+=len2;
		//处理UA
		len1 = CStaticUtil::findstr(temp,"spirit_code_value");
		len1+=17+2;
		fwrite(temp,1,len1,fl);
		temp+=len1;
		len2 = CStaticUtil::findstr(temp,"</");
		fprintf(fl,"%s",ua);
		temp+=len2;
		//处理渠道号
		len1 = CStaticUtil::findstr(temp,"channel_id_value");
		len1+=16+2;
		fwrite(temp,1,len1,fl);
		temp+=len1;
		len2 = CStaticUtil::findstr(temp,"</");
		fprintf(fl,"%s",channel);
		temp+=len2;
		fwrite(temp,1,flen-(temp-buf),fl);
	fclose(fl);
	delete buf;
}

void CApkMake::changeDesktop(bool bDesktop,const char* fname)
{
	FILE *fl;
	fl = fopen(fname,"rb+");
	if(!fl) return;
		::fseek(fl,0,SEEK_END);
		int flen(ftell(fl));
		fseek(fl,0,SEEK_SET);
		char *buf = new char[flen];
		char *temp = buf;
		fread(temp,1,flen,fl);
	fclose(fl);

	fl = fopen(fname,"wb+");
		int len1(0),len2(0);
		len1 = CStaticUtil::findstr(temp,"m_bDesktop = ");
		len1+=13;
		fwrite(temp,1,len1,fl);
		temp+=len1;
		len2 = CStaticUtil::findstr(temp,";");
		if(bDesktop)
			fprintf(fl,"true");
		else
			fprintf(fl,"false");
		temp+=len2;
		fwrite(temp,1,flen-(temp-buf),fl);
	fclose(fl);
	delete buf;
}

DWORD WINAPI CApkMake::ThreadProc(LPVOID lpParam)
{
	return ((CApkMake*)lpParam)->CallBack();
}