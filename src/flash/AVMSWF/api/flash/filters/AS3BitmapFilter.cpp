#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "XFilter.h"
#include "AS3BitmapFilter.h"
#include "AS3BevelFilter.h"// 可使用 BevelFilter 类对显示对象添加斜角效果。 
//  BitmapFilter BitmapFilter 类是所有图像滤镜效果的基类。 
#include "AS3BitmapFilterQuality.h"// BitmapFilterQuality 类中包含的值用于设置 BitmapFilter 对象的呈现品质。 
#include "AS3BitmapFilterType.h"// BitmapFilterType 类中包含的值可用于设置 BitmapFilter 的类型。 
#include "AS3BlurFilter.h"// 可使用 BlurFilter 类将模糊视觉效果应用于显示对象。  
#include "AS3ColorMatrixFilter.h"// 使用 ColorMatrixFilter 类可以将 4 x 5 矩阵转换应用于输入图像上的每个像素的 RGBA 颜色和 Alpha 值，以生成具有一组新的 RGBA 颜色和 Alpha 值的结果。  
#include "AS3ConvolutionFilter.h" //ConvolutionFilter 类应用矩阵盘绕滤镜效果。 
#include "AS3DisplacementMapFilter.h"// DisplacementMapFilter 类使用指定的 BitmapData 对象（称为置换图图像）的像素值执行对象置换。 
#include "AS3DisplacementMapFilterMode.h"// DisplacementMapFilterMode 类为 DisplacementMapFilter 类的 mode 属性提供值。 
#include "AS3DropShadowFilter.h"// 可使用 DropShadowFilter 类向显示对象添加投影。  
#include "AS3GlowFilter.h"// 使用 GlowFilter 类可以对显示对象应用发光效果。  
#include "AS3GradientBevelFilter.h"// 使用 GradientBevelFilter 类可以对显示对象应用渐变斜角效果。  
#include "AS3GradientGlowFilter.h" 

namespace avmshell{
BitmapFilterClass::BitmapFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BitmapFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BitmapFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BitmapFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
BitmapFilterObject::BitmapFilterObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}

BitmapFilterObject* BitmapFilterClass::CreateFilter(void*data)
{
	BitmapFilterObject*pFilter=NULL;
	ShellToplevel*top=(ShellToplevel*)toplevel();
	switch(((_XFilter*)data)->m_nFilterID)
	{
	case XFilterDropShadow:
		{
			DropShadowFilterClass*pClass=top->getDropShadowFilterClass();
			pFilter=pClass->CreateFilter(data);
		}break;
	case XFilterBlur:
		{
			BlurFilterClass*pClass=top->getBlurFilterClass();
			pFilter=pClass->CreateFilter(data);
		}break;
	case XFilterGlow:
		{
			GlowFilterClass*pClass=top->getGlowFilterClass();
			pFilter=pClass->CreateFilter(data);
		}break;
	case XFilterBevel:
		{
			BevelFilterClass*pClass=top->getBevelFilterClass();
			pFilter=pClass->CreateFilter(data);
		}break;
	case XFilterGradientGlow:
		{
			GradientGlowFilterClass*pClass=top->getGradientGlowFilterClass();
			pFilter=pClass->CreateFilter(data);
		}break;
	case XFilterConvolution:
		{
			ConvolutionFilterClass*pClass=top->getConvolutionFilterClass();
			pFilter=pClass->CreateFilter(data);
		}break;
	case XFilterColorMatrix:
		{
			ColorMatrixFilterClass*pClass=top->getColorMatrixFilterClass();
			pFilter=pClass->CreateFilter(data);
		}break;
	case XFilterGradientBevel:
		{
			GradientBevelFilterClass*pClass=top->getGradientBevelFilterClass();
			pFilter=pClass->CreateFilter(data);
		}break;
	default:
		break;
	}
	return pFilter;
}
//////////////////////////////////////////////////////////
//Native Method start...

}