#ifndef _AS3Vector3D_
#define _AS3Vector3D_

//namespace avmshell
//{
//	class Vector3DObject;
//}

namespace avmplus{namespace NativeID{

class Vector3DObject;

class Vector3DClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

Vector3DObject *X_AXIS;
Vector3DObject *Y_AXIS;
Vector3DObject *Z_AXIS;

private:};
class Vector3DObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double	x;
	double	y;
	double	z;
	double	w;
private:};
}}
namespace avmshell{
	class Vector3DObject;
	class Vector3DClass : public ClassClosure
	{
	public:
		Vector3DClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Vector3DObject* CreateObject(double x,double y,double z);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::Vector3DClassSlots m_slots_Vector3DClass;
};
class Vector3DObject : public ScriptObject
{
	public:
		Vector3DObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		inline double GetX(){return m_slots_Vector3DObject.x;}
		inline double GetY(){return m_slots_Vector3DObject.y;}
		inline double GetZ(){return m_slots_Vector3DObject.z;}
		inline double GetW(){return m_slots_Vector3DObject.w;}
		inline void SetX(double x)
		{
			m_slots_Vector3DObject.x=x;	
		}
		inline void SetY(double x)
		{
			m_slots_Vector3DObject.y=x;	
		}
		inline void SetZ(double x)
		{
			m_slots_Vector3DObject.z=x;	
		}
		inline void SetW(double x)
		{
			m_slots_Vector3DObject.w=x;	
		}


		//avmplus::NativeID::Vector3DObjectSlots* GetData(){return &m_slots_Vector3DObject;}
		//void* pData;//Set your data!!
		//AvmBox AS3_angleBetween(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_distance(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_length_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_lengthSquared_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_w_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_w_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_x_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_x_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_y_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_y_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_z_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_z_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_add(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_clone(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_crossProduct(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_decrementBy(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_dotProduct(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_equals(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_incrementBy(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_nearEquals(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_negate(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_normalize(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_project(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_scaleBy(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_subtract(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_toString(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		//new adds 11
        void AS3_copyFrom(Vector3DObject*sourceVector3D);
        void AS3_setTo(double xa, double ya, double za);
        //new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::Vector3DObjectSlots m_slots_Vector3DObject;
};}
#endif