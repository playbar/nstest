#ifndef _AS3Point_
#define _AS3Point_
namespace avmplus{namespace NativeID{
class PointClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class PointObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	double x;
	double y;
//Declare your MEMBER AS3 slots here!!!
private:};
}}

#define AS3POINTDATA avmplus::NativeID::PointObjectSlots

namespace avmshell{
	class PointClass : public ClassClosure
	{
	public:
		PointClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Atom CreatePoint(double x,double y);
		AvmBox AS3_polar(double l,double r);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PointClassSlots m_slots_PointClass;
};
class PointObject : public ScriptObject
{
	public:
		PointObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		double l;
		bool   bModify;
	//public:
		//inline double GetX(){return x;}
		//inline double GetY(){return y;}
		//inline void SetX(double v){x=v;}
		//inline void SetY(double v){y=v;}
		inline void Modify(){bModify=true;}
		inline double GetLength()
		{
			if(!bModify) return l;
			else return AS3_length_get();
		}
		double AS3_distance(PointObject*p1,PointObject*p2);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_interpolate(PointObject*p1,PointObject*p2,double f);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		
		double AS3_length_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);		
		inline double getX()//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		{
			return m_slots_PointObject.x;
		}
		inline void setX(double x)//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		{
			if(m_slots_PointObject.x!=x)
			{
				m_slots_PointObject.x=x;
				bModify=true;
			}
		}
		inline double getY()//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		{
			return m_slots_PointObject.y;
		}
		inline void setY(double y)//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		{
			if(m_slots_PointObject.y!=y)
			{
				m_slots_PointObject.y=y;
				bModify=true;
			}
		}
		inline void constructor(double x,double y)//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		{
			m_slots_PointObject.x=x;
			m_slots_PointObject.y=y;
			bModify=true;
		}
		inline avmplus::NativeID::PointObjectSlots& GetData(){return m_slots_PointObject;}
		//AvmBox AS3_add(PointObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		PointObject* clone();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//bool AS3_equals(PointObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_normalize(double t);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//void AS3_offset(double,double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_subtract(PointObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_toString();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		//new adds 11
 		void AS3_copyFrom(PointObject* sourcePoint);
 		void AS3_setTo(double xa, double ya);
 		//new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PointObjectSlots m_slots_PointObject;
};}
#endif