#ifndef __AVM2__
#define __AVM2__

#include "PROPITEM.h"

class XDrawDib;
class ScriptPlayer;
struct _XCOMMAND;
struct _XSObject;
#if defined AVMPLUS_MAC || defined AVMPLUS_UNIX
  // Support for the -workers switch and distributing files across
  // multiple AvmCores on multiple threads.  Only the code in the
  // shell depends on this setting; the AVM core code works out of
  // the box.
  //
  // Requires pthreads.
  #define VMCFG_WORKERTHREADS
#endif

#if defined VMCFG_TESTING
  // We simulate Flash's use of API versioning for testing only.
  #define VMCFG_TEST_VERSIONING
#endif

#if !defined(UNDER_CE) && !defined(AVM_SHELL_NO_PROJECTOR)
#  define AVMSHELL_PROJECTOR_SUPPORT
#endif

#define INT32_T_MAX		0x7FFFFFFF	//max value for a 32-bit integer
#define UINT32_T_MAX	0xFFFFFFFF	//max value for a 32-bit unsigned integer

#ifdef _MSC_VER
#pragma warning(disable:4996)		// 'scanf' was declared deprecated
#endif

#ifdef VMCFG_AOT
#include "../aot/AOTCompiler.h"
#endif

namespace avmplus
{
	class ClassClosure;
	class ScriptObject;
	class Exception;
	class PoolObject;
};

namespace avmshell
{
	class ShellCodeContext;
	class ShellCore;
	class ShellToplevel;
	class LoaderInfoObject;
	class EventDispatcherObject;
}

namespace MMgc
{
	class GCHeapConfig;
	class EnterFrame;
	class GC;
	class GCAutoEnter;
};
//#include "ShellCore.h"
#ifdef _WINEMU
	#define _TraceOut(V,P1,P2) TraceOut(m_pCore,V,P1,P2)
	#define _TraceInt(V,P1,P2) TraceInt(m_pCore,V,P1,P2)	
#else
	#define _TraceOut(V,P1,P2)
	#define _TraceInt(V,P1,P2)
#endif

namespace avmshell
{ 
#ifdef _WINEMU
		
		extern void TraceOut(ShellCore*pCore,avmplus::Exception*e,const char*s1,const char*s2);
		extern void TraceInt(ShellCore*pCore,avmplus::Exception*e,const char*s1,int i2);
#endif
		enum {XAS3_BUT_REMOVE,XAS3_BUT_ADD,XAS3_BUT_HIT};
		enum
		{
			XAS3CON_LOCAL=0,
			XAS3CON_NET=1,
		};
		enum 
		{
			XAS3EVENT_EVENT,
			XAS3EVENT_MOUSE,
			XAS3EVENT_ACTIVITY,
			XAS3EVENT_ASYNCERROR,
			XAS3EVENT_CONTEXTMENU,
			XAS3EVENT_DATA,
			XAS3EVENT_ERROR,
			XAS3EVENT_FOCUS,
			XAS3EVENT_FULLSCREEN,
			XAS3EVENT_HTTPSTATUS,
			XAS3EVENT_IME,
			XAS3EVENT_IOERROR,
			XAS3EVENT_KEYBOARD,
			XAS3EVENT_NETSTATUS,
			XAS3EVENT_PROGRESS,
			XAS3EVENT_SECURITYERROR,
			XAS3EVENT_STATUS,
			XAS3EVENT_SYNC,
			XAS3EVENT_TEXT,
			XAS3EVENT_TIMER
		};
		enum{
			XAS3_EVENT_none	  =0,
			XAS3_EVENT_activate=1, //Flash Player ��ò���ϵͳ���㲢��Ϊ�״̬ʱ���ȡ�
			XAS3_EVENT_deactivate,//Flash Player ʧȥ����ϵͳ���㲢��Ϊ�ǻ״̬ʱ���ȡ�
			XAS3_EVENT_motionEnd,// �ڶ���������ɺ���ȣ������ǲ��ŵ�ĩβ�����ɵ��� stop() �� end() �������ж϶����� Animator 
			XAS3_EVENT_motionStart,//��������ʼ����ʱ���ȡ� Animator 
			XAS3_EVENT_motionUpdate,//�������Ѹ�Ĳ�����Ļ�Ѹ���ʱ���ȡ� Animator 
			XAS3_EVENT_timeChange,//�� Animator �� time ֵ�Ѹ�ģ�������Ļ��δ����ʱ���� motionUpdate �¼������ȡ�  
			XAS3_EVENT_activity,// ������ͷ��ʼ�����Ựʱ���ȡ� Camera 
		    XAS3_EVENT_status,//  ������ͷ������״̬ʱ���ȡ� 
			XAS3_EVENT_menuSelect,//  ���û��״���������Ĳ˵�����δ��ʾ�����Ĳ˵�����ʱ���ȡ� 
			XAS3_EVENT_menuItemSelect,//  �û��������Ĳ˵���ѡ����ʱ���ȡ� 
			XAS3_EVENT_dataChange,//  �ڸ�����֮����ȡ� DataProvider 
			XAS3_EVENT_preDataChange,//  �ڸ�����֮ǰ���ȡ� 
			XAS3_EVENT_added,//  ����ʾ������ӵ���ʾ�б���ʱ���ȡ� DisplayObject 
			XAS3_EVENT_addedToStage,//  �ڽ���ʾ����ֱ����ӵ���̨��ʾ�б�򽫰���ʾ����������������̨��ʾ�б���ʱ���ȡ� DisplayObject 
			XAS3_EVENT_enterFrame,//  ����ͷ������֡ʱ���ȡ� DisplayObject 
			XAS3_EVENT_removed,//  ��Ҫ����ʾ�б���ɾ����ʾ����ʱ���ȡ� DisplayObject 
			XAS3_EVENT_removedFromStage,//  �ڴ���ʾ�б���ֱ��ɾ����ʾ�����ɾ�����ʾ���������ʱ���ȡ� DisplayObject 
			XAS3_EVENT_render,//��Ҫ���ºͳ�����ʾ�б�ʱ���ȡ�
			XAS3_EVENT_cancel,//  ���û�ͨ���ļ�����Ի���ȡ���ļ����ػ�����ʱ���ȡ� FileReference 
			XAS3_EVENT_complete,//  �����ز�����ɻ����ز������ HTTP ״̬���� 200 ʱ���ȡ� FileReference 
			XAS3_EVENT_httpStatus,//  ������ʧ�ܲ��Ҵ��ڿ���������ʧ�ܵ� HTTP ״̬����ʱ���ȡ� FileReference 
			XAS3_EVENT_httpResponseStatus,
			XAS3_EVENT_ioError,//  �����ػ�����ʧ��ʱ���ȡ� FileReference 
			XAS3_EVENT_open,//  �����ػ����ز�����ʼʱ���ȡ� FileReference 
			XAS3_EVENT_progress,//  ���ļ����ػ����ز����ڼ䶨�ڵ��ȡ� FileReference 
			XAS3_EVENT_select,//  ���û����ļ�����Ի���ѡ��Ҫ���ػ����ص��ļ�ʱ���ȡ� FileReference 
			XAS3_EVENT_uploadCompleteData,//  �ɹ����ز��ӷ������������֮����ȡ� 
			XAS3_EVENT_imeComposition,//���û�������뷨�༭�� (IME) �������Ҷ�ȡ�ַ����ʱ���е��ȡ� 
			XAS3_EVENT_init,//  �Ѽ��ص� SWF �ļ������Ժͷ����ɷ���ʱ���ȡ� LoaderInfo 
			XAS3_EVENT_unload,//  ÿ��ʹ�� Loader ����� unload() ����ɾ���Ѽ��ض���ʱ�����ߵ�ͬһ Loader ����ִ�еڶ��μ��ز����ڼ��ؿ�ʼ֮ǰɾ����ԭʼ����ʱ���� LoaderInfo ������ȡ� 
			XAS3_EVENT_asyncError,//  ���첽���쳣�������Ա����첽���룩ʱ���ȡ� LocalConnection 
			XAS3_EVENT_securityError,//  ���� LocalConnection.send() �ĵ��ó�������һ����ȫɳ�䷢����ݣ�����е��ȡ� LocalConnection 
			XAS3_EVENT_netStatus,//  �� NetConnection ���󱨸���״̬���������ʱ���ȡ� NetConnection 
			XAS3_EVENT_onCuePoint,//  �ڲ��� FLV �ļ��ڼ䵽��Ƕ�����ʾ��ʱ���е��á� NetStream 
			XAS3_EVENT_onMetaData,//  �� Flash Player ��������ŵ� FLV �ļ���Ƕ�����������Ϣʱ���ȡ� NetStream 
			XAS3_EVENT_onPlayStatus,//  �� NetStream ������ȫ������������ȡ� 
			XAS3_EVENT_change,//  �����ڵ�ѡ�� RadioButton ʵ����仯ʱ���ȡ� RadioButtonGroup 
			XAS3_EVENT_click,//  ������ RadioButton ʵ��ʱ���ȡ� 
			XAS3_EVENT_sync,//  �ڷ�����������Զ�̹������(SharedObject)����ȡ� 
			XAS3_EVENT_close,//  �ڷ������ر��׽�������ʱ���ȡ� Socket 
			XAS3_EVENT_connect,//  �ڽ����������Ӻ���ȡ� Socket 
			XAS3_EVENT_socketData,//  ���׽��ֽ��յ���ݺ���ȡ� 
			XAS3_EVENT_id3,//  �ڴ��ڿ����� MP3 ������ ID3 ���ʱ�� Sound ������ȡ� Sound 
			XAS3_EVENT_soundComplete ,//��������ɲ��ź���ȡ�			
			XAS3_EVENT_fullscreen,
			XAS3_EVENT_fullScreenInteractiveAccepted,
			XAS3_EVENT_mouseLeave,			
			XAS3_EVENT_resize,
			XAS3_EVENT_scroll,
			XAS3_EVENT_tabChildrenChange,
			XAS3_EVENT_tabEnabledChange,
			XAS3_EVENT_tabIndexChange,
			XAS3_EVENT_focusIn,
			XAS3_EVENT_focusOut,
			XAS3_EVENT_keyFocusChange,
			XAS3_EVENT_mouseFocusChange,
			XAS3_EVENT_keyDown,
			XAS3_EVENT_keyUp,
			XAS3_EVENT_doubleClick,
			XAS3_EVENT_mouseDown,
			XAS3_EVENT_mouseMove,
			XAS3_EVENT_mouseOut,
			XAS3_EVENT_mouseOver,
			XAS3_EVENT_mouseUp,
			XAS3_EVENT_mouseWheel,
			XAS3_EVENT_rollOut,
			XAS3_EVENT_rollOver,
			XAS3_EVENT_contextMenu,
			XAS3_EVENT_middleClick,
			XAS3_EVENT_middleMouseDown,
			XAS3_EVENT_middleMouseUp,
			XAS3_EVENT_rightClick,
			XAS3_EVENT_rightMouseDown,
			XAS3_EVENT_rightMouseUp,
			XAS3_EVENT_releaseOutside,
			XAS3_EVENT_link,
			XAS3_EVENT_textInput,
			XAS3_EVENT_data,
			XAS3_EVENT_error,
			XAS3_EVENT_timer,
			XAS3_EVENT_timerComplete,
			XAS3_EVENT_exitFrame,
			XAS3_EVENT_clear,
			XAS3_EVENT_closing,
			XAS3_EVENT_copy,
			XAS3_EVENT_cut,
			XAS3_EVENT_displaying,
			XAS3_EVENT_exiting,
			XAS3_EVENT_frameConstructed,
			XAS3_EVENT_htmlBundsChange,
			XAS3_EVENT_htmlDomInitialize,
			XAS3_EVENT_htmlRender,
			XAS3_EVENT_locationChange,
			XAS3_EVENT_networkChange,
			XAS3_EVENT_paste,
			XAS3_EVENT_preparing,
			XAS3_EVENT_selectAll,
			XAS3_EVENT_standardErrorClose,
			XAS3_EVENT_standardInputClose,
			XAS3_EVENT_standardOutputClose,
			XAS3_EVENT_textInteractionModeChange,
			XAS3_EVENT_userIdle,
			XAS3_EVENT_userPresent,
			XAS3_EVENT_context3DCreate,
			XAS3_EVENT_channelMessage,
			XAS3_EVENT_channelState,
			XAS3_EVENT_frameLabel,
			XAS3_EVENT_suspend,
			XAS3_EVENT_textureReady,
			XAS3_EVENT_videoFrame,
			XAS3_EVENT_workerState,
			
			XAS3_EVENT_MAX

		};
	enum
	{
		XAS3_CLASS_button,
		XAS3_CLASS_edittext,
		XAS3_CLASS_movieclip,
		XAS3_CLASS_shape,
		XAS3_CLASS_bitmap,
		XAS3_CLASS_statictext,
		XAS3_CLASS_morphshape,
		XAS3_CLASS_video,
	};
	// The stack margin is the amount of stack that should be available to native
	// code that does not itself check for stack overflow.  The execution engines
	// in the VM will reserve this amount of stack.
	
#ifdef AVMPLUS_64BIT
	const size_t kStackMargin = 262144;
#elif (defined AVMPLUS_WIN32 && defined UNDER_CE) || defined AVMPLUS_SYMBIAN
	// FIXME: all embedded platforms, but we have no way of expressing that now
	const size_t kStackMargin = 32768;
#else
	const size_t kStackMargin = 131072;
#endif
	
	// The fallback stack size is probably not safe but is used by the shell code
	// if it fails to obtain the stack size from the operating system.
	// 512KB is the traditional value.

	const size_t kStackSizeFallbackValue = 512*1024;

	// exit codes
	enum {
		OUT_OF_MEMORY = 128,
	};
	


#ifdef VMCFG_AOT
	// AOT support, impl code in aot.cpp
    void handleAOT(AvmCore*, Domain*, DomainEnv*, Toplevel*&, CodeContext*);
#endif	
	class XAVM2
	{		
	public:
		void Interrupt(bool bExt);
		//MMgc::GC* GetGC();//{return m_pCore->GetGC();}
		bool CheckRCObjects(_XSObject*root);
		int InitAVM2(class XSWFPlayer*player,int nFlex);//bool bFlex);
		//void GCEnter();
		//void GCLeave();
		void ReleaseData();
		void ReleaseAVM2(void);
		int HandleABC(const void*pCode,int nLength,int nFlags,void*);
		avmplus::ScriptObject* FindClass(const char* strName,void*,bool bMust=false);
		int  RemoveAllObject(void*player);
		bool CreateAVM1(_XSObject*s,void*pContext,void**instance,void*parent);
		bool CreateInstance(_XSObject*s,avmplus::ScriptObject*,void**instance,void*pContext,void*parent);//,void*pObj,void*pContext);
#ifdef _WINEMU64
		bool ConstructInstance(_XSObject*s,void*pc,void*instance,void*codeContext,int argc,_int64*args);
#else
		bool ConstructInstance(_XSObject*s,void*pc,void*instance,void*codeContext,int argc,int*args);
#endif
		void ReleaseInstance(void**instance,void*pContext,bool,void*);
		bool CallFrame(void*instance,int iFrame);
		XAVM2();
		~XAVM2();
		ShellCore*	GetCore(){return m_pCore;}
		//bool HasEvent(int nType,void*pInstance);
		//bool DispatchEvent(void*pEvent,void*pObject);
		bool PostEvent(void*pEvent,void*pContext,bool bMask);	
		avmplus::ScriptObject* GetDefaultClass(int id);
		bool SetMember(void*,const char*,void*);
		bool ClearMember(void*,const char*);
		void RemoveSystem(void*);
		bool AddStringToArray(void*pArray,const char* pStr);
		LoaderInfoObject* InitContext(void*player,void*pContext,void*pLoader,void*pLoaderInfo);
		void ReleaseContext(void*,bool bDelete);
		//void* CreateLoaderInfo(void*,void*pContext);
		//void* CreateLoader(void*,void*);
		void* CreatePostEventObject(int id,int type,bool bBubbles,int argc,int*args);
		void ResetEvent(void*pEvent,int id,void*pTarget);
		bool OnKeyDown(int nKey);
		bool OnKeyUp(int nKey);
		bool OnTimer(int nTime,void*pInstance,void*pContext);
		void DispatchEvent(void*pTarget,void*pEvent);
		bool OnEvent(int nType,bool b,void*pInstance,void*pContext,bool bLater=false);
		bool OnCreateModul(void*pInstance);
		bool OnMouseEvent(int nType,MOUSEEVENTDATA&data,void*pInstance,void*pContext);
		bool OnFocusEvent(int nType,void*pout,void*pin,int nKey,bool bShift,void*pContext);
		bool OnKeyEvent(int nType,KEYEVENTDATA&data,void*pInstance,void*pContext);
		bool OnTextEvent(int nType,XU16*s16,XU8*s8,void*pInstance,void*pContext);
		bool OnHTTPStatusEvent(int nCode,void*pInstance,void*pContext);
		bool OnIOErrorEvent(const char*msg,void*pInstance,void*pContext);
		bool OnErrorEvent(const char*msg,void*pInstance,void*pContext);
		bool OnProgressEvent(int nType,int loaded,int total,void*pInstance,void*pContext);
		/*bool OnSyncEvent(ArrayObject*objs);
		bool OnStatusEvent(const char*code,int level);
		bool OnSecurityErrorEvent();
		
		bool OnNetStatusEvent(ScriptObject*obj);
		
		bool OnIMEEvent();
		bool OnHTTPStatusEvent(int nCode);
		bool OnFullScreenEvent(bool b);
		bool OnErrorEvent();
		bool OnDataEvent(int nType,const char*data);
		bool OnContextMenuEvent(int nType,InteractiveObjectObject*owner,InteractiveObjectObject*mouseTarget);
		bool OnAsyncErrorEvent(ErrorObject*obj);
		bool OnActivityEvent(bool);*/
		XU32 CallProperty(void*pObject,const char*strFun,void*pContext);
		XU32 Call(void*pFunction,void*pArguments,void*pContext);
		//void DrawDib(void*pDib,void*m,void*r,void*pClip);
		//XDrawDib* GetDrawDib(void*,void**);
		//XDrawDib* GetDrawDibData(void*,void**);
		void	  SetActive(void*ch);
		//XDrawDib* GetVideoDib(void*,void**);
		_XCOMMAND* GetCommands(void*);
		void	  ReleaseImage(const char*url);
		_XSObject* GetButtonObject(void*pInstance,int state,int act);
		_XSObject* GetButtonState(void*pInstance,int state);
		void FormatButtonState(void*pInstance);
		//* SetButtonstate(void*pInstance,int state,void*pState);
		class BitmapObject* CreateBitmap(XImage*pImage,ShellCodeContext*context,void*obj,void*parent);
		class BitmapObject* CreateSBitmap(void*ch,ShellCodeContext*context,void*obj,void*parent);
		void SetStateObject(void*pInstance,void*pObject,int state);
		bool SetBinaryData(void*pClass,void*pData,int length);
		bool HasEvent(void*pObj,int id);
		void GetName(void*pObj,XString16&name);
		bool SendMethod(void*pObj,const char*name,const char*domain,const char*method,void*args,int t,void*pContext);
		MMgc::GC* GetGC(){return m_pGC;}
		ShellToplevel* GetToplevel();
		void PushObject(void*pObject);
		void RemoveObject(void*pObject);
	public:
		bool  StreamInNew(EventDispatcherObject*,void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loaderInfo);	
		int StreamInWrite(EventDispatcherObject*,void* buffer,int	  length,void*gcEnter,void*pClient);
		bool StreamInDestroy(EventDispatcherObject*,XBOOL bOK,const char*msg,void*pClient);
		void ReleaseIntern(XU8* buffer,int l);
		void GetClassTree(XString8&strTree,void*pObject);		
	protected:
		
		//MMgc::GCAutoEnter*	m_pGCEnter;
		avmplus::PoolObject*	m_pPoolObj;
		class ShellCoreSettings*  m_pSettings;
		MMgc::GCHeapConfig*  m_pConfig;
		MMgc::EnterFrame*	m_pFrame;
		MMgc::GC*		    m_pGC;
		//MMgc::GCAutoEnter*  m_pEnter;
		
		ShellCore*			m_pCore;
	public:
		ScriptPlayer*		m_flexPlayer;
	};
	extern bool m_bRunning;
	extern bool m_bDestroying;
	inline bool IsRunning(){return m_bRunning;}
	XAVM2* InitAVM2(void*player,bool bFlex);//bool bAVM2);
	void ReleaseAVM2(XAVM2*pAVM);
	//void HandleABC(const void*pCode,int nLength,int nFlags)
}

#endif
