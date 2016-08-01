#pragma once

#include "GGType.h"	// Added by ClassView
#include "XString.h"
#include "XMutex.h"

#define ACC_CMD_INIT 3999

class _XEXT_CLASS XAcc
{
public:
	XAcc();
	~XAcc();
	static bool Init();
	//获取重力感应（综合）数据
	static double getAccX();
	static double getAccY();
	static double getAccZ();
    //获取重力感应的重力分量
	static void getGAcc(double&x,double&y,double&z);
	//获取重力感应的加速度分量
	static void getAAcc(double&x,double&y,double&z);	
#ifdef _WIN32
	static HWND m_hMainWnd;
#endif
    static void SetAcc(double x,double y,double z);
};