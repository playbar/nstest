#include "StdAfxflash.h"
#include "ShellCore.h"
#include "sobject.h"
//#if (__CORE_VERSION__>=0x02070200)
#include "sdisplay.h"
//#endif
#include "flashgeom.h"
using namespace avmplus;
#include "AS3Transform.h"
#include "AS3DisplayObject.h"
#include "AS3ColorTransform.h"
#include "AS3Matrix.h"
#include "AS3Rectangle.h"

namespace avmshell{
TransformClass::TransformClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TransformObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TransformClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TransformObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

Atom TransformClass::CreateTransform(DisplayObjectObject*pDisplay)
{
	//Atom atom=Atom();
	TransformObject*pObj=(TransformObject*)createInstance(ivtable(),prototype);
		//(TransformObject*)AvmCore::atomToScriptObject(
		//construct(0,&atom));
	pObj->m_pDisplay=pDisplay;
	return pObj->atom();
}

TransformObject::TransformObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	m_pDisplay=NULL;
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ColorTransformObject* TransformObject::AS3_colorTransform_get()
{
	//Add your act code here...
	if(m_pDisplay&&m_pDisplay->m_pObject)
	{
		ShellToplevel*t=(ShellToplevel*)toplevel();
		ColorTransformObject*pTrans=t->getColorTransformClass()->CreateColorTransform
			(m_pDisplay->m_pObject->xform.cxform);
		return pTrans;
	}
	return NULL;
	//return kAvmThunkUndefined;//Modify this please!
}

void TransformObject::AS3_colorTransform_set(ColorTransformObject*pForm)
{
	//Add your act code here...
	if(m_pDisplay&&m_pDisplay->m_pObject)
	{
		XCXForm form;
		form.Clear();
		pForm->GetCXForm(form);
		m_pDisplay->m_pObject->SetCXForm(form);
	}
	//return kAvmThunkUndefined;//Modify this please!
}

ColorTransformObject* TransformObject::AS3_concatenatedColorTransform_get()
{
	//Add your act code here...
	if(m_pDisplay&&m_pDisplay->m_pObject)
	{
		XCXForm form;
		m_pDisplay->m_pObject->GetTotalCXForm(form);
		ShellToplevel*t=(ShellToplevel*)toplevel();
		return t->getColorTransformClass()->CreateColorTransform(form);
	}
	return NULL;//kAvmThunkUndefined;//Modify this please!
}

MatrixObject* TransformObject::AS3_concatenatedMatrix_get()
{
	//Add your act code here...
	if(m_pDisplay&&m_pDisplay->m_pObject)
	{
		MATRIX mat;
		m_pDisplay->m_pObject->GetTotalMatrix(mat,XFALSE);
		ShellToplevel*t=(ShellToplevel*)toplevel();
		return t->getMatrixClass()->CreateMatrix(&mat);
	}
	return NULL;
	//return kAvmThunkUndefined;//Modify this please!
}

MatrixObject* TransformObject::AS3_matrix_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pDisplay&&m_pDisplay->m_pObject)
	{
		ShellToplevel*t=(ShellToplevel*)toplevel();
		return t->getMatrixClass()->CreateMatrix
			(&m_pDisplay->m_pObject->xform.mat);
	}
	return NULL;
}

void TransformObject::AS3_matrix_set(MatrixObject*pMatrix)
{
	//Add your act code here...
	if(m_pDisplay&&m_pDisplay->m_pObject)
	{
		MATRIX mat;
		pMatrix->GetMatrix(&mat,true);
		m_pDisplay->m_pObject->SetMatrix(mat);
	}
}

RectangleObject* TransformObject::AS3_pixelBounds_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pDisplay&&m_pDisplay->m_pObject)
	{
		SRECT rect;
		m_pDisplay->m_pObject->GetBoundBox(rect,XTRUE);
//#if (__CORE_VERSION__>=0x02070200)
		//m_pDisplay->m_pObject->display->ToPixel(rect);
//#endif
		ShellToplevel*t=(ShellToplevel*)toplevel();
		return t->getRectangleClass()->CreateRectangle(rect.xmin,rect.ymin,rect.xmax-rect.xmin,rect.ymax-rect.ymin);
		//m_pClip->GetPixelRect((SRECT&)rect);
	}
	return NULL;
}


//new adds
Matrix3DObject* TransformObject::AS3_matrix3D_get()
{
	return m_pMatrix3D;
}
void TransformObject::AS3_matrix3D_set(Matrix3DObject *pMatrix3D)
{
	m_pMatrix3D = pMatrix3D;
//#if (__CORE_VERSION__>=0x02078000)
	if(m_pDisplay&&m_pDisplay->m_pObject&&pMatrix3D)
	{
		MATRIX mat;
		MatrixIdentity(&mat);
		double*pData=pMatrix3D->m_dData;
		mat.a=pData[0]*fixed_1;
		mat.b=pData[1]*fixed_1;
		mat.c=pData[4]*fixed_1;
		mat.d=pData[5]*fixed_1;
		mat.tx=pData[12]*20;
		mat.ty=pData[13]*20;
		m_pDisplay->m_pObject->SetMatrix(mat);
	}
//#endif
}
PerspectiveProjectionObject* TransformObject::AS3_perspectiveProjection_get()
{
	if(m_pPerspectiveProjection==NULL)
	{
		PerspectiveProjectionClass*pClass=((ShellToplevel*)toplevel())->getPerspectiveProjectionClass();
		m_pPerspectiveProjection=pClass->CreateObject();
	}
	return m_pPerspectiveProjection;
}
void TransformObject::AS3_perspectiveProjection_set(PerspectiveProjectionObject *pPerspectiveProjection)
{
	m_pPerspectiveProjection = pPerspectiveProjection;
}

Matrix3DObject* TransformObject::AS3_getRelativeMatrix3D(DisplayObjectObject *pRelativeTo)
{
	return NULL;
}
//new adds end

}