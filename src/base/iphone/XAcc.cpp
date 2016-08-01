#include "StdAfxGGBase.h"
#include "XAcc.h"
//#include <math.h>


static double _accX=0,_accY=0,_accZ=0;
static double _gAccX=0,_gAccY=0,_gAccZ=0;
static double _aAccX=0,_aAccY=0,_aAccZ=0;
static bool   _bFirst=true;
bool XAcc::Init()
{
	_bFirst = true;

	return true;
}
	//获取重力感应（综合）数据
double XAcc::getAccX()
{
	return _accX;
}

double XAcc::getAccY()
{
	return _accY;
}

double XAcc::getAccZ()
{
	return _accZ;
}
    //获取重力感应的重力分量
void XAcc::getGAcc(double&x,double&y,double&z)
{
	x = _gAccX;
	y = _gAccY;
	z = _gAccZ;
}
	//获取重力感应的加速度分量
void XAcc::getAAcc(double&x,double&y,double&z)
{
	x = _aAccX;
	y = _aAccY;
	z = _aAccZ;
}

void XAcc::SetAcc(double x,double y,double z)
{
	_accX = x;
	_accY = y;
	_accZ = z;
	if(_bFirst)
	{
		_bFirst = false;
		_gAccX = x;
		_gAccY = y;
		_gAccZ = z;
		_aAccX = 0;
		_aAccY = 0;
		_aAccZ = 0;
	}
	else
	{
		double gv=(x*x+y*y+z*z);
		if(gv>0.97&gv<1.03)
		{
			_gAccX = x;
			_gAccY = y;
			_gAccZ = z;
			_aAccX = 0;
			_aAccY = 0;
			_aAccZ = 0;
		}
		else
		{
			_aAccX = x - _gAccX;
			_aAccY = y - _gAccY;
			_aAccZ = z - _gAccZ;
		}
	}
}
