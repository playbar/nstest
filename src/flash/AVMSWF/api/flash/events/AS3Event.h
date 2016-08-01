#ifndef _AS3Event_
#define _AS3Event_
namespace avmplus{namespace NativeID{
class EventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp ACTIVATE = "activate";
	//Stringp ADDED = "added";
	//Stringp ADDED_TO_STAGE = "addedToStage"; 
	//Stringp CANCEL = "cancel";
	//Stringp CHANGE = "change";

	//Stringp CLOSE = "close";
	//Stringp COMPLETE = "complete";
	//Stringp CONNECT = "connect";
	//Stringp DEACTIVATE = "deactivate"; 
	//Stringp ENTER_FRAME = "enterFrame"; 

	//Stringp FULLSCREEN = "fullScreen";
	//Stringp ID3 = "id3";
	//Stringp INIT = "init"; 
	//Stringp MOUSE_LEAVE = "mouseLeave";
	//Stringp OPEN = "open";

	//Stringp REMOVED = "removed";
	//Stringp REMOVED_FROM_STAGE = "removedFromStage";
	//Stringp RENDER = "render";
	//Stringp RESIZE = "resize";
	//Stringp SCROLL = "scroll";

	//Stringp SELECT = "select";
	//Stringp SOUND_COMPLETE = "soundComplete";
	//Stringp TAB_CHILDREN_CHANGE = "tabChildrenChange";
	//Stringp TAB_ENABLED_CHANGE = "tabEnabledChange";
	//Stringp TAB_INDEX_CHANGE = "tabIndexChange";
	//Stringp UNLOAD = "unload";

	//CLEAR : String = "clear"
	//CLOSING : String = "closing"
	//COPY : String = "copy"
	//CUT : String = "cut"
	//DISPLAYING : String = "displaying"
	//EXIT_FRAME : String = "exitFrame"
	//EXITING : String = "exiting"
	//FRAME_CONSTRUCTED : String = "frameConstructed"
	//HTML_BOUNDS_CHANGE : String = "htmlBoundsChange"
	//HTML_DOM_INITIALIZE : String = "htmlDOMInitialize"
	//HTML_RENDER : String = "htmlRender"
	//LOCATION_CHANGE : String = "locationChange"
	//NETWORK_CHANGE : String = "networkChange"
	//PASTE : String = "paste"
	//PREPARING : String = "preparing"
	//SELECT_ALL : String = "selectAll"
	//STANDARD_ERROR_CLOSE : String = "standardErrorClose"
	//STANDARD_INPUT_CLOSE : String = "standardInputClose"
	//STANDARD_OUTPUT_CLOSE : String = "standardOutputClose"
	//TEXT_INTERACTION_MODE_CHANGE : String = "textInteractionModeChange"
	//USER_IDLE : String = "userIdle"
	//USER_PRESENT : String = "userPresent"

	//new adds 11
		//CHANNEL_MESSAGE:String = "channelMessage"
		//CHANNEL_STATE:String = "channelState"
		//FRAME_LABEL:String = "frameLabel"
		//SUSPEND:String = "suspend"
		//TEXTURE_READY:String = "textureReady"
		//VIDEO_FRAME:String = "videoFrame"
		//WORKER_STATE:String = "workerState"
	//new adds 11 end 

	Stringp ACTIVATE;
	Stringp ADDED;
	Stringp ADDED_TO_STAGE; 
	Stringp CANCEL;
	Stringp CHANGE;

	Stringp CLOSE;
	Stringp COMPLETE;
	Stringp CONNECT;
	Stringp DEACTIVATE; 
	Stringp ENTER_FRAME; 

	Stringp FULLSCREEN;
	Stringp ID3;
	Stringp INIT; 
	Stringp MOUSE_LEAVE;
	Stringp OPEN;

	Stringp REMOVED;
	Stringp REMOVED_FROM_STAGE;
	Stringp RENDER;
	Stringp RESIZE;
	Stringp SCROLL;

	Stringp SELECT;
	Stringp SOUND_COMPLETE;
	Stringp TAB_CHILDREN_CHANGE;
	Stringp TAB_ENABLED_CHANGE;
	Stringp TAB_INDEX_CHANGE;
	Stringp UNLOAD;
	Stringp EXIT_FRAME;

	//new adds
	Stringp CLEAR;
	Stringp CLOSING;
	Stringp COPY;
	Stringp CUT;
	Stringp DISPLAYING;
	Stringp EXITING;
	Stringp FRAME_CONSTRUCTED;
	Stringp HTML_BOUNDS_CHANGE;
	Stringp HTML_DOM_INITIALIZE;
	Stringp HTML_RENDER;
	Stringp LOCATION_CHANGE;
	Stringp NETWORK_CHANGE;
	Stringp PASTE;
	Stringp PREPARING;
	Stringp SELECT_ALL;
	Stringp STANDARD_ERROR_CLOSE;
	Stringp STANDARD_INPUT_CLOSE;
	Stringp STANDARD_OUTPUT_CLOSE;
	Stringp TEXT_INTERACTION_MODE_CHANGE;
	Stringp USER_IDLE;
	Stringp USER_PRESENT;
	//new adds end

	//Check adds
	Stringp	CONTEXT3D_CREATE;
	//Check adds end

	//new adds 11
	Stringp		CHANNEL_MESSAGE;
	Stringp		CHANNEL_STATE;
	Stringp		FRAME_LABEL;
	Stringp		SUSPEND;
	Stringp		TEXTURE_READY;
	Stringp		VIDEO_FRAME;
	Stringp		WORKER_STATE;
	//new adds 11 end
private:};
class EventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}



namespace avmshell{

	const XPCTSTR _strLeftQ="[";
	const XPCTSTR _strRightQ="]";
	const XPCTSTR _strQT="\"";
	const XPCTSTR _strType="type";
	const XPCTSTR _strBubbles="bubbles";
	const XPCTSTR _strCancelable="cancelable";
	const XPCTSTR _strEventPhase="eventPhase";
	
	class EventObject;

	class EventClass : public ClassClosure
	{
	public:		
		EventObject* CreateEventObject(int id);
		EventObject* CreateEventObject(Stringp strType);
		Stringp		 IDToStringType(int id);
	public:
		EventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		//EventObject*  CreateEventObject(int id);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EventClassSlots m_slots_EventClass;
public:
		inline Stringp getSlotACTIVATE(){return m_slots_EventClass.ACTIVATE;}
		inline Stringp getSlotADDED(){return m_slots_EventClass.ADDED;}
		inline Stringp getSlotADDED_TO_STAGE(){return m_slots_EventClass.ADDED_TO_STAGE;}
		inline Stringp getSlotCANCEL(){return m_slots_EventClass.CANCEL;}
		inline Stringp getSlotCHANGE(){return m_slots_EventClass.CHANGE;}
		inline Stringp getSlotCLOSE(){return m_slots_EventClass.CLOSE;}
		inline Stringp getSlotCOMPLETE(){return m_slots_EventClass.COMPLETE;}
		inline Stringp getSlotCONNECT(){return m_slots_EventClass.CONNECT;}
		inline Stringp getSlotDEACTIVATE(){return m_slots_EventClass.DEACTIVATE;}
		inline Stringp getSlotENTER_FRAME(){return m_slots_EventClass.ENTER_FRAME;}
		inline Stringp getSlotFULLSCREEN(){return m_slots_EventClass.FULLSCREEN;}
		inline Stringp getSlotID3(){return m_slots_EventClass.ID3;}
		inline Stringp getSlotINIT(){return m_slots_EventClass.INIT;}
		inline Stringp getSlotMOUSE_LEAVE(){return m_slots_EventClass.MOUSE_LEAVE;}
		inline Stringp getSlotOPEN(){return m_slots_EventClass.OPEN;}
		inline Stringp getSlotREMOVED(){return m_slots_EventClass.REMOVED;}
		inline Stringp getSlotREMOVED_FROM_STAGE(){return m_slots_EventClass.REMOVED_FROM_STAGE;}
		inline Stringp getSlotRENDER(){return m_slots_EventClass.RENDER;}
		inline Stringp getSlotRESIZE(){return m_slots_EventClass.RESIZE;}
		inline Stringp getSlotSCROLL(){return m_slots_EventClass.SCROLL;}
		inline Stringp getSlotSELECT(){return m_slots_EventClass.SELECT;}
		inline Stringp getSlotSOUND_COMPLETE(){return m_slots_EventClass.SOUND_COMPLETE;}
		inline Stringp getSlotTAB_CHILDREN_CHANGE(){return m_slots_EventClass.TAB_CHILDREN_CHANGE;}
		inline Stringp getSlotTAB_ENABLED_CHANGE(){return m_slots_EventClass.TAB_ENABLED_CHANGE;}
		inline Stringp getSlotTAB_INDEX_CHANGE(){return m_slots_EventClass.TAB_INDEX_CHANGE;}
		inline Stringp getSlotUNLOAD(){return m_slots_EventClass.UNLOAD;}
		inline Stringp getSlotEXIT_FRAME(){return m_slots_EventClass.EXIT_FRAME;}

		//new adds
		inline Stringp getSlotCLEAR(){return m_slots_EventClass.CLEAR;}
		inline Stringp getSlotCLOSING(){return m_slots_EventClass.CLOSING;}
		inline Stringp getSlotCOPY(){return m_slots_EventClass.COPY;}
		inline Stringp getSlotCUT(){return m_slots_EventClass.CUT;}
		inline Stringp getSlotDISPLAYING(){return m_slots_EventClass.DISPLAYING;}
		inline Stringp getSlotEXITING(){return m_slots_EventClass.EXITING;}
		inline Stringp getSlotFRAME_CONSTRUCTED(){return m_slots_EventClass.FRAME_CONSTRUCTED;}
		inline Stringp getSlotHTML_BOUNDS_CHANGE(){return m_slots_EventClass.HTML_BOUNDS_CHANGE;}
		inline Stringp getSlotHTML_DOM_INITIALIZE(){return m_slots_EventClass.HTML_DOM_INITIALIZE;}
		inline Stringp getSlotHTML_RENDER(){return m_slots_EventClass.HTML_RENDER;}
		inline Stringp getSlotLOCATION_CHANGE(){return m_slots_EventClass.LOCATION_CHANGE;}
		inline Stringp getSlotNETWORK_CHANGE(){return m_slots_EventClass.NETWORK_CHANGE;}
		inline Stringp getSlotPASTE(){return m_slots_EventClass.PASTE;}
		inline Stringp getSlotPREPARING(){return m_slots_EventClass.PREPARING;}
		inline Stringp getSlotSELECT_ALL(){return m_slots_EventClass.SELECT_ALL;}
		inline Stringp getSlotSTANDARD_ERROR_CLOSE(){return m_slots_EventClass.STANDARD_ERROR_CLOSE;}
		inline Stringp getSlotSTANDARD_INPUT_CLOSE(){return m_slots_EventClass.STANDARD_INPUT_CLOSE;}
		inline Stringp getSlotSTANDARD_OUTPUT_CLOSE(){return m_slots_EventClass.STANDARD_OUTPUT_CLOSE;}
		inline Stringp getSlotTEXT_INTERACTION_MODE_CHANGE(){return m_slots_EventClass.TEXT_INTERACTION_MODE_CHANGE;}
		inline Stringp getSlotUSER_IDLE(){return m_slots_EventClass.USER_IDLE;}
		inline Stringp getSlotUSER_PRESENT(){return m_slots_EventClass.USER_PRESENT;}
		//new adds end
		inline Stringp getSlotCONTEXT3D_CREATE(){return m_slots_EventClass.CONTEXT3D_CREATE;}
		//new adds 11
		inline Stringp getSlotCHANNEL_MESSAGE(){return m_slots_EventClass.CHANNEL_MESSAGE;}
		inline Stringp getSlotCHANNEL_STATE(){return m_slots_EventClass.CHANNEL_STATE;}
		inline Stringp getSlotFRAME_LABEL(){return m_slots_EventClass.FRAME_LABEL;}
		inline Stringp getSlotSUSPEND(){return m_slots_EventClass.SUSPEND;}
		inline Stringp getSlotTEXTURE_READY(){return m_slots_EventClass.TEXTURE_READY;}
		inline Stringp getSlotVIDEO_FRAME(){return m_slots_EventClass.VIDEO_FRAME;}
		inline Stringp getSlotWORKER_STATE(){return m_slots_EventClass.WORKER_STATE;}

		//new adds 11 end
};
class EventObject : public ScriptObject
{
	//friend class EventDispatcherObject;
	public:
		 bool bBubbles;//[read-only] 指示事件是否为冒泡事件。  Event 
		 bool bCancelable;//[read-only] 指示是否可以阻止与事件相关联的行为。  Event 
		 DRCWB(ScriptObject*) pCurrentTarget;//[read-only] 当前正在使用某个事件侦听器处理 Event 对象的对象。 Event 
		 uint32 uEventPhase;//[read-only] 事件流中的当前阶段。  Event 
		 DRCWB(ScriptObject*) pTarget;//[read-only] 事件目标。  Event 
		 DRCWB(ScriptObject*) pStopObj;
		 DRCWB(Stringp)	   strType;//[read-only] 事件的类型。
		 bool bStoped,bDefaultPrevented,bStopedLater,bWithTarget;
		 int  nEventID,nCallStack;
		 XU32	nCreateTime;

	public:
		EventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual EventObject* Clone(EventObject*pObj);
	public:
		//void* pData;//Set your data!!
		bool AS3_bubbles_get();
		bool AS3_cancelable_get();
		AvmBox AS3_currentTarget_get();
		uint32 AS3_eventPhase_get();
		AvmBox AS3_target_get();
		Stringp AS3_type_get();
		void AS3_constructor(Stringp, bool, bool);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		EventObject* AS3_clone();
		Stringp AS3_formatToString(Stringp,ArrayObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		bool AS3_isDefaultPrevented();
		void AS3_preventDefault(bool);
		void AS3_stopImmediatePropagation();
		void AS3_stopPropagation();
		Stringp AS3_toString();		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EventObjectSlots m_slots_EventObject;	
};}
#endif