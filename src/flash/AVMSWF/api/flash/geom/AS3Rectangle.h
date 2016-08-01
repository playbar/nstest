#ifndef _AS3Rectangle_
#define _AS3Rectangle_

#define AS3RECTANGLEDATA avmplus::NativeID::RectangleObjectSlots

namespace avmplus{namespace NativeID{
class RectangleClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};

class RectangleObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	double left;
	double top;
	double width;
	double height;
	inline bool _containsRect(RectangleObjectSlots*p)
	{
		double right=left+width;
		double bottom=top+height;
		double r1=p->left+p->width;
		double b1=p->top+p->height;
		if(p->left>=left&&p->left<=right&&
		   p->top>=top&&p->top<=bottom&&
		   r1>=left&&r1<=right&&
		   b1>=top&&b1<=bottom )
		  return true;
		return false;

	}
	inline void _intersection(RectangleObjectSlots*pr,double&x,double&y,double&w,double&h)
	{
		double r=left+width;
		double b=top+height;
		double r1=pr->left+pr->width;
		double b1=pr->top+pr->height;
		x=XMAX(left,pr->left);
		y=XMAX(top,pr->top);
		r=XMIN(r,r1);
		b=XMIN(b,b1);
		
		w=r>x?r-x:0;
		h=b>y?b-y:0;
		if(w==0||h==0){x=0;y=0;w=0;h=0;}
		/*if(r>l&&b>t)
		{
			pRect->left=l;
			pRect->top=t;
			pRect->width=r-l;
			pRect->height=b-t;
		}*/
	}
	inline bool _intersects(RectangleObjectSlots*pr)
	{
		double r=left+width;
		double b=top+height;
		double r1=pr->left+pr->width;
		double b1=pr->top+pr->height;
		double l=XMAX(left,pr->left);
		double t=XMAX(top,pr->top);
		r=XMIN(r,r1);
		b=XMIN(b,b1);
		if(r>l&&b>t)
		   return true;
		return false;
	}
	inline void _union(RectangleObjectSlots*pr,double&x,double&y,double&w,double&h)
	{
		double r=left+width;
		double b=top+height;
		double r1=pr->left+pr->width;
		double b1=pr->top+pr->height;
		x=XMIN(left,pr->left);
		y=XMIN(top,pr->top);
		r=XMAX(r,r1);
		b=XMAX(b,b1);
		w=r-x;
		h=b-y;		
	}
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class RectangleObject;
	class RectangleClass : public ClassClosure
	{
	public:
		RectangleClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		RectangleObject* CreateRectangle(double l,double t,double w,int h);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::RectangleClassSlots m_slots_RectangleClass;
};
class RectangleObject : public ScriptObject
{
	public:
		RectangleObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		inline AS3RECTANGLEDATA&GetData(){return m_slots_RectangleObject;}
		inline void constructor(double x,double y,double w,double h)
		{
			m_slots_RectangleObject.left=x;
			m_slots_RectangleObject.top=y;
			m_slots_RectangleObject.width=w;
			m_slots_RectangleObject.height=h;
		}
		/*AvmBox AS3_bottom_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bottom_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bottomRight_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bottomRight_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_height_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_height_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_left_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_left_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_right_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_right_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_size_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_size_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_top_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_top_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_topLeft_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_topLeft_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_width_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_width_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_x_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_x_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_y_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_y_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_constructor(double l,double t,double w,double h);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_clone(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_contains(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_containsPoint(AvmMethodEnv env, uint32_t argc, AvmBox* argv);*/
		bool AS3_containsRect(RectangleObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_equals(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_inflate(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_inflatePoint(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		RectangleObject* AS3_intersection(RectangleObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		bool AS3_intersects(RectangleObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_isEmpty(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_offset(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_offsetPoint(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_setEmpty(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_toString();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		RectangleObject* AS3_union(RectangleObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		 //new adds 11
 		 void AS3_setTo(double xa, double ya, double widtha, double heighta);
 		 //new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::RectangleObjectSlots m_slots_RectangleObject;
};}
#endif