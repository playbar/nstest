#ifndef _AS3Transform_
#define _AS3Transform_

#include "AS3Matrix3D.h"
#include "AS3PerspectiveProjection.h"
#include "AS3DisplayObject.h"

namespace avmplus{namespace NativeID{
class TransformClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TransformObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DisplayObjectObject;
	class ColorTransformObject;
	class MatrixObject;
	class RectangleObject;
	class TransformClass : public ClassClosure
	{
	public:
		TransformClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Atom CreateTransform(DisplayObjectObject*);
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TransformClassSlots m_slots_TransformClass;
};
class TransformObject : public ScriptObject
{
	public:
		TransformObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(DisplayObjectObject*) m_pDisplay;//Set your data!!

		//new adds
		DRCWB(Matrix3DObject *) m_pMatrix3D;
		DRCWB(PerspectiveProjectionObject *) m_pPerspectiveProjection;
		//new adds end

		DisplayObjectObject* GetDisplayObject()
		{ return m_pDisplay; }
		ColorTransformObject* AS3_colorTransform_get();
		void AS3_colorTransform_set(ColorTransformObject*);
		ColorTransformObject* AS3_concatenatedColorTransform_get();
		MatrixObject* AS3_concatenatedMatrix_get();
		MatrixObject* AS3_matrix_get();
		void AS3_matrix_set(MatrixObject*);
		RectangleObject* AS3_pixelBounds_get();

		//new adds
		Matrix3DObject* AS3_matrix3D_get();
		void AS3_matrix3D_set(Matrix3DObject *pMatrix3D);
		PerspectiveProjectionObject* AS3_perspectiveProjection_get();
		void AS3_perspectiveProjection_set(PerspectiveProjectionObject *pPerspectiveProjection);

		Matrix3DObject* AS3_getRelativeMatrix3D(DisplayObjectObject *pRelativeTo);
		//new adds end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TransformObjectSlots m_slots_TransformObject;
};}
#endif