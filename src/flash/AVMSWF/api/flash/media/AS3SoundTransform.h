#ifndef _AS3SoundTransform_
#define _AS3SoundTransform_
namespace avmplus{namespace NativeID{
class SoundTransformClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SoundTransformObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SoundTransformObject;
	class SoundTransformClass : public ClassClosure
	{
	public:
		int m_nOffset;
		SoundTransformClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		SoundTransformObject* GetObjectOf(char*address)
		{
			return (SoundTransformObject*)(address-m_nOffset);
		}
		SoundTransformObject* CreateTransform();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundTransformClassSlots m_slots_SoundTransformClass;
};
class SoundTransformObject : public ScriptObject
{
	public:
		SoundTransformObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:		
		XSoundTransform m_trans;
		double AS3_leftToLeft_get()
		{ return m_trans.m_dLeftToLeft/256.0;}
		void AS3_leftToLeft_set(double v){m_trans.m_dLeftToLeft=v*256;}
		double AS3_leftToRight_get()
		{  return m_trans.m_dLeftToRight/256.0;}
		void AS3_leftToRight_set(double v){m_trans.m_dLeftToRight=v*256;}
		double AS3_pan_get(){return m_trans.m_dPan/256.0;}
		void AS3_pan_set(double v){m_trans.m_dPan=v*256.0;}
		double AS3_rightToLeft_get(){return m_trans.m_dRightToLeft/256.0;}
		void AS3_rightToLeft_set(double v){m_trans.m_dRightToLeft=v*256;}
		double AS3_rightToRight_get(){return m_trans.m_dRightToRight/256.0;}
		void AS3_rightToRight_set(double v){m_trans.m_dRightToRight=v*256.0;}
		double AS3_volume_get()
		{
			return m_trans.m_dVolume/256.0;
		}
		void AS3_volume_set(double v)
		{
			m_trans.m_dVolume=v*256;
		}
		void AS3_constructor(double vol,double pan)
		{
			m_trans.m_dVolume=vol*256;
			m_trans.m_dPan=pan*256;
		}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundTransformObjectSlots m_slots_SoundTransformObject;
};}
#endif