#ifndef _AS3Matrix_
#define _AS3Matrix_
#include "AS3Vector3D.h"
#define AS3MATRIXDATA avmplus::NativeID::MatrixObjectSlots

namespace avmplus{namespace NativeID{
class MatrixClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MatrixObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	double a;
	double b;
	double c;
	double d;
	double tx;
	double ty;
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class PointObject;
	class MatrixObject;
	class MatrixClass : public ClassClosure
	{
	public:
		MatrixClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		MatrixObject* CreateMatrix(void*m);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MatrixClassSlots m_slots_MatrixClass;
};
class MatrixObject : public ScriptObject
{
	public:
		MatrixObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		inline AS3MATRIXDATA& GetData(){return m_slots_MatrixObject;}
		void GetMatrix(void*m,bool bTrip);
		void SetMatrix(void*m);
		//AvmBox AS3_clone();
		//void AS3_concat(MatrixObject*pMatrix);
		//void AS3_createBox(double w,double h,double r,double tx,double ty);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//void AS3_createGradientBox(double w,double h,double r,double tx,double ty);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_deltaTransformPoint(PointObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//void AS3_identity();
		void AS3_invert();
		//void AS3_rotate(double r);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//void AS3_scale(double,double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_toString();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_transformPoint(PointObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//void AS3_translate(double dx,double dy);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		//new adds 11
 		void AS3_copyColumnFrom(uint32_t column, Vector3DObject* vector3D);
 		void AS3_copyColumnTo(uint32_t column, Vector3DObject* vector3D);
 		void AS3_copyFrom(MatrixObject* sourceMatrix);
 		void AS3_copyRowFrom(uint32_t row, Vector3DObject* vector3D);
 		void AS3_copyRowTo(uint32_t row, Vector3DObject* vector3D);
 		void AS3_setTo(double aa, double ba, double ca, double da, double txa, double tya);
 		//new adds 11 end	
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MatrixObjectSlots m_slots_MatrixObject;
};}
#endif