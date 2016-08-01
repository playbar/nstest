#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;

#include "AS3EventDispatcher.h"
#include "AS3Stage3D.h"

namespace avmshell{
	Stage3DClass::Stage3DClass (VTable * cvtable):ClassClosure(cvtable)
	{
		createVanillaPrototype();
		//prototype = new (core()->GetGC(), cvtable->getExtraSize())BitmapObject(cvtable, toplevel()->objectClass->prototype, 0);
		//Add your construct code here...
	};
	ScriptObject *Stage3DClass::createInstance(VTable *ivtable, ScriptObject *prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) Stage3DObject(ivtable, prototype,0);
		//Add your construct code here...
	}


	
	Stage3DObject::Stage3DObject(VTable* vtable, ScriptObject* proto, int capacity):EventDispatcherObject(vtable,proto,0)
	{
		m_visible = true;
		m_x       = 0;
		m_y       = 0;
	}

//	Context3DObject* Stage3DObject::AS3_context3D_get()
//	{
//		return NULL;
//	}

	bool Stage3DObject::AS3_visible_get()
	{
		//LOGWHERE();
		return m_visible;
	}
	void Stage3DObject::AS3_visible_set(bool visible)
	{
		//LOGWHERE();
		m_visible  = visible;
	}

	double Stage3DObject::AS3_x_get()
	{
		//LOGWHERE();
		//XString16 s("x:");
		//XWindow::Trace(s);
		return m_x;
	}
	void Stage3DObject::AS3_x_set(double x)
	{
		//LOGWHERE();
		m_x = x;
	}

	double Stage3DObject::AS3_y_get()
	{
		//LOGWHERE();
		return m_y;
	}
	void Stage3DObject::AS3_y_set(double y)
	{
		//LOGWHERE();
		m_y = y;
	}

//	void Stage3DObject::AS3_requestContext3D(String context3DRenderMode, String profile)
//	{
	
//	}
}