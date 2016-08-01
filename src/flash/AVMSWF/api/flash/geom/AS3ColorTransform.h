#ifndef _AS3ColorTransform_
#define _AS3ColorTransform_
namespace avmplus
{
	namespace NativeID
	{
		class ColorTransformClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

		private:
		};
		class ColorTransformObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
			double aa;//与 Alpha 透明度通道值相乘的十进制值。 ColorTransform 
			double ab;//-255 到 255 之间的数字，它先与 alphaMultiplier 值相乘，再与 Alpha 透明度通道值相加。 ColorTransform 
			double ba;//与蓝色通道值相乘的十进制值。 ColorTransform 
			double bb;//-255 到 255 之间的数字，它先与 blueMultiplier 值相乘，再与蓝色通道值相加。 ColorTransform 
			double ga;//与绿色通道值相乘的十进制值。 ColorTransform 
			double gb;//-255 到 255 之间的数字，它先与 greenMultiplier 值相乘，再与绿色通道值相加。 ColorTransform 
			double ra;//与红色通道值相乘的十进制值。 ColorTransform 
			double rb;
			inline void _concat(ColorTransformObjectSlots*cx) {
				ab += (aa * cx->ab);// alpha
				aa = (aa * cx->aa);

				rb += (ra * cx->rb);// red
				ra = (ra * cx->ra);

				gb += (ga * cx->gb);// green
				ga = (ga * cx->ga);

				bb += (ba * cx->bb);// blue
				ba = (ba * cx->ba);

			}
			inline uint32 _getColor() {
				return (((XU8) rb) << 16) | (((XU8) gb) << 8) | ((XU8) bb);
			}
			inline void _setColor(uint32 c) {
				ga = ba = ra = 0;
				rb = (c >> 16) &0xff;
				gb = (c >> 8) & 0xff;
				bb = (c & 0xff);
			}
		private:
		};
	}
}

#define AS3COLORTRANSFORMDATA avmplus::NativeID::ColorTransformObjectSlots

namespace avmshell {
	class ColorTransformObject;
	class ColorTransformClass: public ClassClosure
	{
	public:
		ColorTransformClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ColorTransformObject* CreateColorTransform(class XCXForm&form);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ColorTransformClassSlots m_slots_ColorTransformClass;
	};
	class ColorTransformObject: public ScriptObject
	{
	public:
		ColorTransformObject(VTable* _vtable, ScriptObject* _delegate,
			int capacity);
	public:
		XU32 m_nColor;
		void SetCXForm(class XCXForm&form);
		void GetCXForm(class XCXForm&form);
		uint32 AS3_color_get();
		void AS3_color_set(uint32);
		void AS3_constructor(double ra, double ga, double ba, double aa, double rb,
			double gb, double bb, double ab);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_concat(ColorTransformObject*);
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ColorTransformObjectSlots m_slots_ColorTransformObject;
	};
}
#endif
