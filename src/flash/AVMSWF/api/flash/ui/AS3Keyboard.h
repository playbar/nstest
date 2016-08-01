#ifndef _AS3Keyboard_
#define _AS3Keyboard_
namespace avmplus{namespace NativeID{
class KeyboardClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//uint32_t BACKSPACE = 8;
	//uint32_t CAPS_LOCK = 20; 
	//uint32_t CONTROL = 17;
	//uint32_t DELETE = 46;
	//uint32_t DOWN = 40; 

	//uint32_t END = 35;
	//uint32_t ENTER = 13; 
	//uint32_t ESCAPE = 27; 
	//uint32_t F1 = 112; 
	//uint32_t F10 = 121; 

	//uint32_t F11 = 122;
	//uint32_t F12 = 123;
	//uint32_t F13 = 124;
	//uint32_t F14 = 125;
	//uint32_t F15 = 126;

	//uint32_t F2 = 113;
	//uint32_t F3 = 114;
	//uint32_t F4 = 115;
	//uint32_t F5 = 116;
	//uint32_t F6 = 117;

	//uint32_t F7 = 118;
	//uint32_t F8 = 119;
	//uint32_t F9 = 120;
	//uint32_t HOME = 36; 
	//uint32_t INSERT = 45; 

	//uint32_t LEFT = 37;
	//uint32_t NUMPAD_0 = 96; 
	//uint32_t NUMPAD_1 = 97;
	//uint32_t NUMPAD_2 = 98;
	//uint32_t NUMPAD_3 = 99;

	//uint32_t NUMPAD_4 = 100; 
	//uint32_t NUMPAD_5 = 101;
	//uint32_t NUMPAD_6 = 102;
	//uint32_t NUMPAD_7 = 103;
	//uint32_t NUMPAD_8 = 104;

	//uint32_t NUMPAD_9 = 105; 
	//uint32_t NUMPAD_ADD = 107;
	//uint32_t NUMPAD_DECIMAL = 110; 
	//uint32_t NUMPAD_DIVIDE = 111;
	//uint32_t NUMPAD_ENTER = 108;

	//uint32_t NUMPAD_MULTIPLY = 106; 
	//uint32_t NUMPAD_SUBTRACT = 109;
	//uint32_t PAGE_DOWN = 34;
	//uint32_t PAGE_UP = 33;
	//uint32_t RIGHT = 39;

	//uint32_t SHIFT = 16;
	//uint32_t SPACE = 32;
	//uint32_t TAB = 9;
	//uint32_t UP = 38; 

	uint32_t BACKSPACE;
	uint32_t CAPS_LOCK; 
	uint32_t CONTROL;
	uint32_t DELETE_FORSPACE;
	uint32_t DOWN; 

	uint32_t END;
	uint32_t ENTER; 
	uint32_t ESCAPE; 
	uint32_t F1; 
	uint32_t F10; 

	uint32_t F11;
	uint32_t F12;
	uint32_t F13;
	uint32_t F14;
	uint32_t F15;

	uint32_t F2;
	uint32_t F3;
	uint32_t F4;
	uint32_t F5;
	uint32_t F6;

	uint32_t F7;
	uint32_t F8;
	uint32_t F9;
	uint32_t HOME; 
	uint32_t INSERT; 

	uint32_t LEFT;
	uint32_t NUMPAD_0; 
	uint32_t NUMPAD_1;
	uint32_t NUMPAD_2;
	uint32_t NUMPAD_3;

	uint32_t NUMPAD_4; 
	uint32_t NUMPAD_5;
	uint32_t NUMPAD_6;
	uint32_t NUMPAD_7;
	uint32_t NUMPAD_8;

	uint32_t NUMPAD_9; 
	uint32_t NUMPAD_ADD;
	uint32_t NUMPAD_DECIMAL; 
	uint32_t NUMPAD_DIVIDE;
	uint32_t NUMPAD_ENTER;

	uint32_t NUMPAD_MULTIPLY; 
	uint32_t NUMPAD_SUBTRACT;
	uint32_t PAGE_DOWN;
	uint32_t PAGE_UP;
	uint32_t RIGHT;

	uint32_t SHIFT;
	uint32_t SPACE;
	uint32_t TAB;
	uint32_t UP; 
	int		 capsLock;
	int		 numLock;

	//new adds
	uint32_t A;
	uint32_t ALTERNATE_C;
	uint32_t AUDIO;
	uint32_t B;
	uint32_t BACK;
	uint32_t BACKQUOTE;
	uint32_t BACKSLASH;
	uint32_t BLUE;
	uint32_t C;
	uint32_t CHANNEL_DOWN;
	uint32_t CHANNEL_UP;
	ArrayObject *CharCodeStringps;
	uint32_t COMMA;
	uint32_t COMMAND;
	uint32_t D;
	uint32_t DVR;
	uint32_t E;
	uint32_t EQUAL;
	uint32_t EXIT;
	uint32_t F;
	uint32_t FAST_FORWARD;
	uint32_t G;
	uint32_t GREEN;
	uint32_t GUIDE;
	uint32_t H;
	uint32_t HELP;
	uint32_t I;
	uint32_t INFO;
	uint32_t INPUT;
	uint32_t J;
	uint32_t K;
	Stringp KEYNAME_BEGIN;
	Stringp KEYNAME_BREAK;
	Stringp KEYNAME_CLEARDISPLAY;
	Stringp KEYNAME_CLEARLINE;
	Stringp KEYNAME_DELETE;
	Stringp KEYNAME_DELETECHAR;
	Stringp KEYNAME_DELETELINE;
	Stringp KEYNAME_DOWNARROW;
	Stringp KEYNAME_END;
	Stringp KEYNAME_EXECUTE;
	Stringp KEYNAME_F1;
	Stringp KEYNAME_F10;
	Stringp KEYNAME_F11;
	Stringp KEYNAME_F12;
	Stringp KEYNAME_F13;
	Stringp KEYNAME_F14;
	Stringp KEYNAME_F15;
	Stringp KEYNAME_F16;
	Stringp KEYNAME_F17;
	Stringp KEYNAME_F18;
	Stringp KEYNAME_F19;
	Stringp KEYNAME_F2;
	Stringp KEYNAME_F20;
	Stringp KEYNAME_F21;
	Stringp KEYNAME_F22;
	Stringp KEYNAME_F23;
	Stringp KEYNAME_F24;
	Stringp KEYNAME_F25;
	Stringp KEYNAME_F26;
	Stringp KEYNAME_F27;
	Stringp KEYNAME_F28;
	Stringp KEYNAME_F29;
	Stringp KEYNAME_F3;
	Stringp KEYNAME_F30;
	Stringp KEYNAME_F31;
	Stringp KEYNAME_F32;
	Stringp KEYNAME_F33;
	Stringp KEYNAME_F34;
	Stringp KEYNAME_F35;
	Stringp KEYNAME_F4;
	Stringp KEYNAME_F5;
	Stringp KEYNAME_F6;
	Stringp KEYNAME_F7;
	Stringp KEYNAME_F8;
	Stringp KEYNAME_F9;
	Stringp KEYNAME_FIND;
	Stringp KEYNAME_HELP;
	Stringp KEYNAME_HOME;
	Stringp KEYNAME_INSERT;
	Stringp KEYNAME_INSERTCHAR;
	Stringp KEYNAME_INSERTLINE;
	Stringp KEYNAME_LEFTARROW;
	Stringp KEYNAME_MENU;
	Stringp KEYNAME_MODESWITCH;
	Stringp KEYNAME_NEXT;
	Stringp KEYNAME_PAGEDOWN;
	Stringp KEYNAME_PAGEUP;
	Stringp KEYNAME_PAUSE;
	Stringp KEYNAME_PREV;
	Stringp KEYNAME_PRINT;
	Stringp KEYNAME_PRINTSCREEN;
	Stringp KEYNAME_REDO;
	Stringp KEYNAME_RESET;
	Stringp KEYNAME_RIGHTARROW;
	Stringp KEYNAME_SCROLLLOCK;
	Stringp KEYNAME_SELECT;
	Stringp KEYNAME_STOP;
	Stringp KEYNAME_SYSREQ;
	Stringp KEYNAME_SYSTEM;
	Stringp KEYNAME_UNDO;
	Stringp KEYNAME_UPARROW;
	Stringp KEYNAME_USER;
	uint32_t L;
	uint32_t LAST;
	uint32_t LEFTBRACKET;
	uint32_t LIVE;
	uint32_t M;
	uint32_t MASTER_SHELL;
	uint32_t MENU;
	uint32_t MINUS;
	uint32_t N;
	uint32_t NEXT;
	uint32_t NUMBER_0;
	uint32_t NUMBER_1;
	uint32_t NUMBER_2;
	uint32_t NUMBER_3;
	uint32_t NUMBER_4;
	uint32_t NUMBER_5;
	uint32_t NUMBER_6;
	uint32_t NUMBER_7;
	uint32_t NUMBER_8;
	uint32_t NUMBER_9;
	uint32_t NUMPAD;
	uint32_t O;
	uint32_t P;
	uint32_t PAUSE;
	uint32_t PERIOD;
	uint32_t PLAY;
	uint32_t PREVIOUS;
	uint32_t Q;
	uint32_t QUOTE;
	uint32_t R;
	uint32_t RECORD;
	uint32_t RED;
	uint32_t REWIND;
	uint32_t RIGHTBRACKET;
	uint32_t S;
	uint32_t SEARCH;
	uint32_t SEMICOLON;
	uint32_t SETUP;
	uint32_t SKIP_BACKWARD;
	uint32_t SKIP_FORWARD;
	uint32_t SLASH;
	uint32_t STOP;
	Stringp Stringp_BEGIN;
	Stringp Stringp_BREAK;
	Stringp Stringp_CLEARDISPLAY;
	Stringp Stringp_CLEARLINE;
	Stringp Stringp_DELETE;
	Stringp Stringp_DELETECHAR;
	Stringp Stringp_DELETELINE;
	Stringp Stringp_DOWNARROW;
	Stringp Stringp_END;
	Stringp Stringp_EXECUTE;
	Stringp Stringp_F1;
	Stringp Stringp_F10;
	Stringp Stringp_F11;
	Stringp Stringp_F12;
	Stringp Stringp_F13;
	Stringp Stringp_F14;
	Stringp Stringp_F15;
	Stringp Stringp_F16;
	Stringp Stringp_F17;
	Stringp Stringp_F18;
	Stringp Stringp_F19;
	Stringp Stringp_F2;
	Stringp Stringp_F20;
	Stringp Stringp_F21;
	Stringp Stringp_F22;
	Stringp Stringp_F23;
	Stringp Stringp_F24;
	Stringp Stringp_F25;
	Stringp Stringp_F26;
	Stringp Stringp_F27;
	Stringp Stringp_F28;
	Stringp Stringp_F29;
	Stringp Stringp_F3;
	Stringp Stringp_F30;
	Stringp Stringp_F31;
	Stringp Stringp_F32;
	Stringp Stringp_F33;
	Stringp Stringp_F34;
	Stringp Stringp_F35;
	Stringp Stringp_F4;
	Stringp Stringp_F5;
	Stringp Stringp_F6;
	Stringp Stringp_F7;
	Stringp Stringp_F8;
	Stringp Stringp_F9;
	Stringp Stringp_FIND;
	Stringp Stringp_HELP;
	Stringp Stringp_HOME;
	Stringp Stringp_INSERT;
	Stringp Stringp_INSERTCHAR;
	Stringp Stringp_INSERTLINE;
	Stringp Stringp_LEFTARROW;
	Stringp Stringp_MENU;
	Stringp Stringp_MODESWITCH;
	Stringp Stringp_NEXT;
	Stringp Stringp_PAGEDOWN;
	Stringp Stringp_PAGEUP;
	Stringp Stringp_PAUSE;
	Stringp Stringp_PREV;
	Stringp Stringp_PRINT;
	Stringp Stringp_PRINTSCREEN;
	Stringp Stringp_REDO;
	Stringp Stringp_RESET;
	Stringp Stringp_RIGHTARROW;
	Stringp Stringp_SCROLLLOCK;
	Stringp Stringp_SELECT;
	Stringp Stringp_STOP;
	Stringp Stringp_SYSREQ;
	Stringp Stringp_SYSTEM;
	Stringp Stringp_UNDO;
	Stringp Stringp_UPARROW;
	Stringp Stringp_USER;
	uint32_t SUBTITLE;
	uint32_t T;
	uint32_t U;
	uint32_t V;
	uint32_t VOD;
	uint32_t W;
	uint32_t X;
	uint32_t Y;
	uint32_t YELLOW;
	uint32_t Z;
	//new adds end

private:};
class KeyboardObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class KeyboardClass : public ClassClosure
	{
	public:
		KeyboardClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		bool AS3_isAccessible();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KeyboardClassSlots m_slots_KeyboardClass;
		
	public:
		inline int GetCapsLock() {return m_slots_KeyboardClass.capsLock;}
		inline int GetNumLock(){return m_slots_KeyboardClass.numLock;}
		inline void SetCapsLock(int b){m_slots_KeyboardClass.capsLock=b;}
		inline void SetNumLock(int b){m_slots_KeyboardClass.numLock=b;}
			inline uint32_t getSlotBACKSPACE(){return m_slots_KeyboardClass.BACKSPACE;}
			inline uint32_t getSlotCAPS_LOCK(){return m_slots_KeyboardClass.CAPS_LOCK;}
			inline uint32_t getSlotCONTROL(){return m_slots_KeyboardClass.CONTROL;}
			inline uint32_t getSlotDELETE_FORSPACE(){return m_slots_KeyboardClass.DELETE_FORSPACE;}
			inline uint32_t getSlotDOWN(){return m_slots_KeyboardClass.DOWN;}

			inline uint32_t getSlotEND(){return m_slots_KeyboardClass.END;}
			inline uint32_t getSlotENTER(){return m_slots_KeyboardClass.ENTER;}
			inline uint32_t getSlotESCAPE(){return m_slots_KeyboardClass.ESCAPE;}
			inline uint32_t getSlotF1(){return m_slots_KeyboardClass.F1;}
			inline uint32_t getSlotF10(){return m_slots_KeyboardClass.F10;}

			inline uint32_t getSlotF11(){return m_slots_KeyboardClass.F11;}
			inline uint32_t getSlotF12(){return m_slots_KeyboardClass.F12;}
			inline uint32_t getSlotF13(){return m_slots_KeyboardClass.F13;}
			inline uint32_t getSlotF14(){return m_slots_KeyboardClass.F14;}
			inline uint32_t getSlotF15(){return m_slots_KeyboardClass.F15;}

			inline uint32_t getSlotF2(){return m_slots_KeyboardClass.F2;}
			inline uint32_t getSlotF3(){return m_slots_KeyboardClass.F3;}
			inline uint32_t getSlotF4(){return m_slots_KeyboardClass.F4;}
			inline uint32_t getSlotF5(){return m_slots_KeyboardClass.F5;}
			inline uint32_t getSlotF6(){return m_slots_KeyboardClass.F6;}

			inline uint32_t getSlotF7(){return m_slots_KeyboardClass.F7;}
			inline uint32_t getSlotF8(){return m_slots_KeyboardClass.F8;}
			inline uint32_t getSlotF9(){return m_slots_KeyboardClass.F9;}
			inline uint32_t getSlotHOME(){return m_slots_KeyboardClass.HOME;}
			inline uint32_t getSlotINSERT(){return m_slots_KeyboardClass.INSERT;}

			inline uint32_t getSlotLEFT(){return m_slots_KeyboardClass.LEFT;}
			inline uint32_t getSlotNUMPAD_0(){return m_slots_KeyboardClass.NUMPAD_0;}
			inline uint32_t getSlotNUMPAD_1(){return m_slots_KeyboardClass.NUMPAD_1;}
			inline uint32_t getSlotNUMPAD_2(){return m_slots_KeyboardClass.NUMPAD_2;}
			inline uint32_t getSlotNUMPAD_3(){return m_slots_KeyboardClass.NUMPAD_3;}

			inline uint32_t getSlotNUMPAD_4(){return m_slots_KeyboardClass.NUMPAD_4;}
			inline uint32_t getSlotNUMPAD_5(){return m_slots_KeyboardClass.NUMPAD_5;}
			inline uint32_t getSlotNUMPAD_6(){return m_slots_KeyboardClass.NUMPAD_6;}
			inline uint32_t getSlotNUMPAD_7(){return m_slots_KeyboardClass.NUMPAD_7;}
			inline uint32_t getSlotNUMPAD_8(){return m_slots_KeyboardClass.NUMPAD_8;}

			inline uint32_t getSlotNUMPAD_9(){return m_slots_KeyboardClass.NUMPAD_9;}
			inline uint32_t getSlotNUMPAD_ADD(){return m_slots_KeyboardClass.NUMPAD_ADD;}
			inline uint32_t getSlotNUMPAD_DECIMAL(){return m_slots_KeyboardClass.NUMPAD_DECIMAL;}
			inline uint32_t getSlotNUMPAD_DIVIDE(){return m_slots_KeyboardClass.NUMPAD_DIVIDE;}
			inline uint32_t getSlotNUMPAD_ENTER(){return m_slots_KeyboardClass.NUMPAD_ENTER;}

			inline uint32_t getSlotNUMPAD_MULTIPLY(){return m_slots_KeyboardClass.NUMPAD_MULTIPLY;} 
			inline uint32_t getSlotNUMPAD_SUBTRACT(){return m_slots_KeyboardClass.NUMPAD_SUBTRACT;}
			inline uint32_t getSlotPAGE_DOWN(){return m_slots_KeyboardClass.PAGE_DOWN;}
			inline uint32_t getSlotPAGE_UP(){return m_slots_KeyboardClass.PAGE_UP;}
			inline uint32_t getSlotRIGHT(){return m_slots_KeyboardClass.RIGHT;}

			inline uint32_t getSlotSHIFT(){return m_slots_KeyboardClass.SHIFT;}
			inline uint32_t getSlotSPACE(){return m_slots_KeyboardClass.SPACE;}
			inline uint32_t getSlotTAB(){return m_slots_KeyboardClass.TAB;}
			inline uint32_t getSlotUP(){return m_slots_KeyboardClass.UP;}
};
class KeyboardObject : public ScriptObject
{
	public:
		KeyboardObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		//AvmBox AS3_capsLock_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_numLock_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::KeyboardObjectSlots m_slots_KeyboardObject;
};}
#endif