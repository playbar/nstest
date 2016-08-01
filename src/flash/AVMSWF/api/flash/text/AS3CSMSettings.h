#ifndef _AS3CSMSettings_
#define _AS3CSMSettings_
namespace avmplus{namespace NativeID{
class CSMSettingsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CSMSettingsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double fontSize;
	double insideCutoff;
	double outsideCutoff;
private:};
}}
namespace avmshell{
	class CSMSettingsClass : public ClassClosure
	{
	public:
		CSMSettingsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CSMSettingsClassSlots m_slots_CSMSettingsClass;
};
class CSMSettingsObject : public ScriptObject
{
	public:
		CSMSettingsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		inline double getFontSize(){return m_slots_CSMSettingsObject.fontSize;}
		inline double getInsideSize(){return m_slots_CSMSettingsObject.insideCutoff;}
		inline double getOutsideSize(){return m_slots_CSMSettingsObject.outsideCutoff;}
		inline void setFontSize(double s){m_slots_CSMSettingsObject.fontSize=s;}
		inline void setInsideSize(double s){m_slots_CSMSettingsObject.insideCutoff=s;}
		inline void setOutsideSize(double s){m_slots_CSMSettingsObject.outsideCutoff=s;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CSMSettingsObjectSlots m_slots_CSMSettingsObject;
};}
#endif