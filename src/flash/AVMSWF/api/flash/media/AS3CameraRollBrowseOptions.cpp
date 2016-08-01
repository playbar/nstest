#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3CameraRollBrowseOptions.h"
namespace avmshell
{
	CameraRollBrowseOptionsClass::CameraRollBrowseOptionsClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* CameraRollBrowseOptionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) CameraRollBrowseOptionsObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	CameraRollBrowseOptionsObject::CameraRollBrowseOptionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_height = 0.0;
		RectangleClass* pClass = ((ShellToplevel*)toplevel())->getRectangleClass();
		m_origin = (RectangleObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		m_origin->constructor( 0.0, 0.0, 0.0, 0.0 );
		m_width  = 0.0;
	}
	//////////////////////////////////////////////////////////
	double CameraRollBrowseOptionsObject::AS3_height_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_height;
	}
	void CameraRollBrowseOptionsObject::AS3_height_set(double height)
	{
		//Add your construct code here...
		//LOGWHERE();
		if(height<0)return;
		m_height = height;
	}
    
	RectangleObject* CameraRollBrowseOptionsObject::AS3_origin_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_origin;//NULL;//(x=0, y=0, w=0, h=0)
	}
	void CameraRollBrowseOptionsObject::AS3_origin_set(RectangleObject* origin)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_origin = origin;
	}
    
	double CameraRollBrowseOptionsObject::AS3_width_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_width;//0;
	}
	void CameraRollBrowseOptionsObject::AS3_width_set(double width)
	{
		//Add your construct code here...
		//LOGWHERE();
		if(width<0)return;
		m_width = width;
	}

}