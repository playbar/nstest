#ifndef _AS3BlendMode_
#define _AS3BlendMode_
namespace avmplus{namespace NativeID{
class BlendModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp ADD = "add";
//	Stringp ALPHA = "alpha";
//	Stringp DARKEN = "darken";
//	Stringp DIFFERENCE = "difference";
//	Stringp ERASE = "erase";
//
//	Stringp HARDLIGHT = "hardlight";
//	Stringp INVERT = "invert";
//	Stringp LAYER = "layer";
//	Stringp LIGHTEN = "lighten";
//	Stringp MULTIPLY = "multiply";
//
//	Stringp NORMAL = "normal";
//	Stringp OVERLAY = "overlay";
//	Stringp SCREEN = "screen";
//	Stringp SUBTRACT = "subtract";

//  SHADER : String = "shader"

	Stringp ADD;
	Stringp ALPHA;
	Stringp DARKEN;
	Stringp DIFFERENCE_FORSPACE;
	Stringp ERASE;

	Stringp HARDLIGHT;
	Stringp INVERT;
	Stringp LAYER;
	Stringp LIGHTEN;
	Stringp MULTIPLY;

	Stringp NORMAL;
	Stringp OVERLAY;
	Stringp SCREEN;
	Stringp SUBTRACT;

	//new adds
	Stringp SHADER;
	//new adds end
private:};
class BlendModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class BlendModeClass : public ClassClosure
	{
	public:
		BlendModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BlendModeClassSlots m_slots_BlendModeClass;
	public:
		enum {ADD=0,
			  ALPHA,
			  DARKEN,
			  DIFFERENCE_FORSPACE,
			  ERASE,
			  
			  HARDLIGHT,
			  INVERT,
			  LAYER,
			  LIGHTEN,
			  MULTIPLY,
			  NORMAL,
			  OVERLAY,
			  SCREEN,
			  SUBTRACT};
		Stringp* getSlots(){return (Stringp*)&m_slots_BlendModeClass.ADD;}
		Stringp getSlotADD(){return m_slots_BlendModeClass.ADD;}
		Stringp getSlotALPHA(){return m_slots_BlendModeClass.ALPHA;}
		Stringp getSlotDARKEN(){return m_slots_BlendModeClass.DARKEN;}
		Stringp getSlotDIFFERENCE_FORSPACE(){return m_slots_BlendModeClass.DIFFERENCE_FORSPACE;}
		Stringp getSlotERASE(){return m_slots_BlendModeClass.ERASE;}

		Stringp getSlotHARDLIGHT(){return m_slots_BlendModeClass.HARDLIGHT;}
		Stringp getSlotINVERT(){return m_slots_BlendModeClass.INVERT;}
		Stringp getSlotLAYER(){return m_slots_BlendModeClass.LAYER;}
		Stringp getSlotLIGHTEN(){ return m_slots_BlendModeClass.LIGHTEN;}
		Stringp getSlotMULTIPLY(){return m_slots_BlendModeClass.MULTIPLY;}

		Stringp getSlotNORMAL(){return m_slots_BlendModeClass.NORMAL;}
		Stringp getSlotOVERLAY(){return m_slots_BlendModeClass.OVERLAY;}
		Stringp getSlotSCREEN(){return m_slots_BlendModeClass.SCREEN;}
		Stringp getSlotSUBTRACT(){return m_slots_BlendModeClass.SUBTRACT;}

		//new adds
		Stringp getSlotSHADER(){return m_slots_BlendModeClass.SHADER;}
		//new adds end
		Stringp getModeNameOf(int i);
		int	    getModeIDOf(Stringp name);
};
class BlendModeObject : public ScriptObject
{
	public:
		BlendModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BlendModeObjectSlots m_slots_BlendModeObject;
};}
#endif