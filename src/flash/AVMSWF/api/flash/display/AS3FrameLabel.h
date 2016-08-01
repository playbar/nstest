#ifndef _AS3FrameLabel_
#define _AS3FrameLabel_
namespace avmplus{namespace NativeID{
class FrameLabelClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class FrameLabelObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	//int frame;
	//Stringp name;
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FrameLabelObject;
	class FrameLabelClass : public ClassClosure
	{
	public:
		FrameLabelClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		FrameLabelObject* CreateFrameLabel(int frame,Stringp name);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FrameLabelClassSlots m_slots_FrameLabelClass;
};
class FrameLabelObject : public ScriptObject
{
	public:
		FrameLabelObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		int			   m_nFrame;
		DRCWB(Stringp) m_strName;
		int			   m_iLabel;
		void MakeILabel();
		inline Stringp AS3_name_get(){return m_strName;}
		inline void setName(Stringp name) {m_strName=name;MakeILabel();}//m_slots_FrameLabelObject.name=name;}
		inline int AS3_frame_get(){return m_nFrame;}
		inline void setFrame(int i){m_nFrame=i;};//m_slots_FrameLabelObject.frame=i;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FrameLabelObjectSlots m_slots_FrameLabelObject;
};}
#endif