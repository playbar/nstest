#ifndef _AS3SecurityPanel_
#define _AS3SecurityPanel_
namespace avmplus{namespace NativeID{
class SecurityPanelClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp CAMERA = "camera";
	//Stringp DEFAULT = "default"; 
	//Stringp LOCAL_STORAGE = "localStorage";
	//Stringp MICROPHONE = "microphone";
	//Stringp PRIVACY = "privacy";
	//Stringp SETTINGS_MANAGER = "settingsManager";

	//DISPLAY : String = "display"

	Stringp CAMERA;
	Stringp DEFAULT; 
	Stringp LOCAL_STORAGE;
	Stringp MICROPHONE;
	Stringp PRIVACY;
	Stringp SETTINGS_MANAGER;

	//new adds
	Stringp DISPLAY;
	//new adds end
private:};
class SecurityPanelObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	
private:};
}}
namespace avmshell{
	class SecurityPanelClass : public ClassClosure
	{
	public:
		SecurityPanelClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		inline Stringp getSlotCAMERA(){return m_slots_SecurityPanelClass.CAMERA;}
		inline Stringp getSlotDEFAULT(){return m_slots_SecurityPanelClass.DEFAULT; }
		inline Stringp getSlotLOCAL_STORAGE(){return m_slots_SecurityPanelClass.LOCAL_STORAGE;}
		inline Stringp getSlotMICROPHONE(){return m_slots_SecurityPanelClass.MICROPHONE;}
		inline Stringp getSlotPRIVACY(){return m_slots_SecurityPanelClass.PRIVACY;}
		inline Stringp getSlotSETTINGS_MANAGER(){return m_slots_SecurityPanelClass.SETTINGS_MANAGER;}
		
		//new adds
		inline Stringp getSlotDISPLAY(){return m_slots_SecurityPanelClass.DISPLAY;}
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecurityPanelClassSlots m_slots_SecurityPanelClass;
};
class SecurityPanelObject : public ScriptObject
{
	public:
		SecurityPanelObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecurityPanelObjectSlots m_slots_SecurityPanelObject;
};}
#endif