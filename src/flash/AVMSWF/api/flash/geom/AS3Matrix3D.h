#ifndef _AS3Matrix3D_
#define _AS3Matrix3D_

#include "AS3Vector3D.h"

namespace avmplus{namespace NativeID{
class Matrix3DClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class Matrix3DObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
class Matrix3DObject;

	class Matrix3DClass : public ClassClosure
	{
	public:
		Matrix3DClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		Matrix3DObject*	AS3_interpolate(Matrix3DObject *pThisMat, Matrix3DObject *pToMat, double percent);
		Matrix3DObject* CreateObject();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::Matrix3DClassSlots m_slots_Matrix3DClass;
};
class Matrix3DObject : public ScriptObject
{
	public:
		Matrix3DObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void AppendData(double dData[16]);
		void PrependData(double dData[16]);
		double m_dData[16];
		//Members
		//DRCWB(DoubleVectorObject*)	m_pRawData;
		//DRCWB(Vector3DObject*)		m_pPosition;
		double						m_determinant;

		double	AS3_determinant_get();

		Vector3DObject*	AS3_position_get();
		void	AS3_position_set(Vector3DObject *pPosition);

		DoubleVectorObject*	AS3_rawData_get();
		void	AS3_rawData_set(DoubleVectorObject *pRawData);

		void	AS3_constructor(DoubleVectorObject *pV);
		void	AS3_append(Matrix3DObject *pLhs);
		void	AS3_appendRotation(double degrees, Vector3DObject *pAxis, Vector3DObject *pPivotPoint);
		void	AS3_appendScale(double xScale, double yScale, double zScale);
		void	AS3_appendTranslation(double x, double y, double z);

		Matrix3DObject*	AS3_clone();
		ObjectVectorObject*	AS3_decompose(Stringp orientationStyle);
		Vector3DObject*	AS3_deltaTransformVector(Vector3DObject *pV);
		void	AS3_identity();

		void	AS3_interpolateTo(Matrix3DObject *pToMat, double percent);
		bool	AS3_invert();
		void	AS3_pointAt(Vector3DObject *pPos, Vector3DObject *pAt, Vector3DObject *pUp);
		void	AS3_prepend(Matrix3DObject *pRhs);
		void	AS3_prependRotation(double degrees, Vector3DObject *pAxis, Vector3DObject *pPivotPoint);

		void	AS3_prependScale(double xScale, double yScale, double zScale);
		void	AS3_prependTranslation(double x, double y, double z);
		bool	AS3_recompose(ObjectVectorObject *pComponents, Stringp orientationStyle);
		Vector3DObject*	AS3_transformVector(Vector3DObject *pV);
		void	AS3_transformVectors(ObjectVectorObject* pVin, ObjectVectorObject *pVout);
		void	AS3_transpose(){}

		//new adds 11
		void AS3_copyColumnFrom(uint32_t column, Vector3DObject* vector3D);
		void AS3_copyColumnTo(uint32_t column, Vector3DObject* vector3D);
		void AS3_copyFrom(Matrix3DObject* sourceMatrix3D);
		void AS3_copyRawDataFrom(DoubleVectorObject* vector, uint32_t index = 0, bool transpose = false);
		void AS3_copyRawDataTo(DoubleVectorObject* vector, uint32_t index = 0, bool transpose = false);
		void AS3_copyRowFrom(uint32_t column, Vector3DObject* vector3D);
		void AS3_copyRowTo(uint32_t column, Vector3DObject* vector3D);
		void AS3_copyToMatrix3D(Matrix3DObject*dest);
		//new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::Matrix3DObjectSlots m_slots_Matrix3DObject;
};}
#endif