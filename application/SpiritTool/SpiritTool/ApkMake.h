#pragma once

#include "PointerArray.h"
#include "SpiritData.h"

class CApkMake
{
public:
	class MakeListener
	{
	public:
		virtual void onListen(int type,unsigned long data)=0;
	};
public:
	static CApkMake* Singleton()
	{
		static CApkMake* sl = NULL;
		if(sl == NULL)
		{
			sl = new CApkMake();
		}
		return sl;
	}
public:
	CApkMake(void);
	~CApkMake(void);
public:
	//设置源码路径和sdk路径
	void setPath(const char* srcpath,const char* sdkpath);
	//开始打包
	void startMake();
	void stopMake();
	void setListener(MakeListener* listener);

	//打包器控制
	void start();
	void end();
	void setDataArray(CPointerArray<CSpiritData>* data);
	int getMakedCount();
	int getMakingId();
public:
	DWORD CallBack();
private:
	void ready();
	void go();
	void modulatepath(const char *packagename,char *path);	//调制路径
	void changestring(const char *appname,const char* ua,const char* channel,const char* fname);	//修改strings.xml
	void changeDesktop(bool bDesktop,const char* fname);
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParam); 
private:
	char *m_plOldPackageName;		//程序包名
	CPointerArray<CSpiritData>	*m_pArray;	//数据队列	
	int		m_nMakedCount;			//已打包数
	HANDLE	m_hthread;				//线程句柄
	HANDLE	m_hevent;				//事件对象
	HANDLE	m_hclose;				//关闭事件
	HANDLE	m_hclosed;				//以关闭事件
	MakeListener*	m_pListener;	//监听者
	char*	m_plSrcpath;			//源码路径
	char*	m_plSdkpath;			//sdk路径
};

