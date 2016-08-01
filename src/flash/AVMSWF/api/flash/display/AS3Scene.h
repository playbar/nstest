#ifndef _AS3Scene_
#define _AS3Scene_
namespace avmplus{namespace NativeID{
class SceneClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SceneObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SceneObject;
	class SceneClass : public ClassClosure
	{
	public:
		SceneClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		SceneObject* CreateScene(Stringp name,int nFrames,int offset);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SceneClassSlots m_slots_SceneClass;
};
class SceneObject : public ScriptObject
{
	public:
		SceneObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual ~SceneObject()
		{

		}
	public:
		DRCWB(ArrayObject*) m_pLabels;
		DRCWB(Stringp)	    m_strName;
		int				    m_nFrames;
		int					m_nOffset;
	//public:
	//	inline void SetLabels(ArrayObject*pArray)
	//	{	m_pLabels=pArray;	}

		ArrayObject* AS3_labels_get(){return m_pLabels;}
		Stringp AS3_name_get(){return m_strName;}
		int AS3_numFrames_get(){return m_nFrames;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SceneObjectSlots m_slots_SceneObject;
};}
#endif