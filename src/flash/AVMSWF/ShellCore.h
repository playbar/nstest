/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef __avmshell_core__
#define __avmshell_core__

extern void* _pDebugP;

#define XCHAIN_MAX	100

#include "avmplus.h"
#include "AS3/TotalInclude.h"
#include "AS3EventDispatcher.h"
#include "DebugCLI.h"
class XSWFPlayer;
class ScriptPlayer;
class XDrawDib;
struct _DIBINFO;
using namespace avmplus;
using namespace MMgc;
#ifdef _DEBUG
    #if defined(__APPLE__)&&defined(__HDStackOverFlew)
        #define _STACKSIZE (32768<<3)
    #else
        #define _STACKSIZE (32768<<4)
    #endif
#else
	#ifdef WIN32
	#define _STACKSIZE (32768<<3)
	#elif defined __ANDROID__    // add by hgl 
	#define _STACKSIZE (32768<<3)
	#elif defined(__APPLE__)    // add by handong
        #ifdef __HDStackOverFlew
            #define _STACKSIZE (32768<<3)
        #else
            #define _STACKSIZE (32768<<4)
        #endif
	#else  
	#define  _STACKSIZE (32768<<3)
	#endif
#endif

#ifdef _DEBUG
	#define INTString(II) core()->intToString(II)
#else
	#define INTString(II) core()->kEmptyString
#endif


namespace avmshell
{	
#ifdef DEBUGGER
	class ShellToplevel;
	class Profiler : public avmplus::Profiler
	{
	public:
		Profiler(AvmCore*) {}
		
		void sendDebugFileURL(String * /*url*/) {}
		void sendLineTimestamp(int /*linenumber*/) {}
		void setEnabled(bool /*on*/) {}
		void sendFunctionEnter(MethodInfo* /*method*/) {}
		void sendFunctionExit() {}
		void sendCatch(MethodInfo* /*method*/) {}
		void addScriptObject(ScriptObject * /*scriptObject*/) {}
		void deleteScriptObject(ScriptObject * /*scriptObject*/) {}
		void sendHeapDump(String * /*heapDumpName*/) {}
		int computeStringSize(String * /*myString*/) { return 0; }		

		void sample(uint64_t /*time*/, Stringp /*stackTrace*/) {}
		void allocationSample(uint64_t /*time*/, Stringp /*stackTrace*/, uint64_t /*allocId*/, Stringp /*type*/) {}
		void deallocationSample(uint64_t /*time*/, uint64_t /*id*/) {}
		void getMembers() {}

	};
#endif
	class URLRequestObject;
	namespace ExtErrorContext
	{
		enum
		{
			kExtError_EOFError	= 2000,// 如果尝试读取的内容超出可用数据的末尾，则会引发 EOFError 异常。 
			kExtError_IllegalOperationError,// 当方法未实现或者实现中未涉及当前用法时，将引发 IllegalOperationError 异常。 
			kExtError_InvalidSWFError,// Flash,// Player 遇到损坏的 SWF 文件时，将引发此异常。 
			kExtError_IOError,// 某些类型的输入或输出失败时，将引发 IOError 异常。 
			kExtError_MemoryError,// 内存分配请求失败时，将引发 MemoryError 异常。 
			kExtError_ScriptTimeoutError,// 达到脚本超时间隔时，将引发 ScriptTimeoutError 异常。 
			kExtError_StackOverflowError
		};
	}
		class ShellCore;
		class ColorTransformClass;
		class PointClass;
		class RectangleClass;
		class MatrixClass;
		class EventClass;
		class BitmapDataClass;
		class ApplicationDomainClass;
		class SecurityDomainClass;
		class ByteArrayClass;
		class ContextMenuBuiltInItemsClass;
		class AccessibilityPropertiesClass;
		class TransformClass;
		class EventDispatcherClass;
		class URLRequestClass;
		class SoundTransformClass;
		class ResponderClass;
		class ID3InfoClass;
		class SoundLoaderContextClass;
		class TextFormatClass;
		class LoaderContextClass;
		class BitmapFilterClass;
		class TextSnapshotClass;
		class GraphicsClass;
		class SceneClass;
		class PrintJobOptionsClass;
		class AccessibilityClass;
		class ActionScriptVersionClass;
		class AntiAliasTypeClass;
		class argumentsClass;
		class BitmapDataChannelClass;
		class BitmapFilterQualityClass;
		class BitmapFilterTypeClass;
		class BlendModeClass;
		class CapabilitiesClass;
		class CapsStyleClass;
		class CSMSettingsClass;
		class CustomActionsClass;
		class DictionaryClass;
		class DisplacementMapFilterModeClass;
		class EndianClass;
		class EventPhaseClass;
		class FileFilterClass;
		class FontClass;
		class FontStyleClass;
		class FontTypeClass;
		class FrameLabelClass;
		class GradientTypeClass;
		class GridFitTypeClass;
		class IMEConversionModeClass;
		class InterpolationMethodClass;
		class JointStyleClass;
		class KeyboardClass;
		class KeyLocationClass;
		class LineScaleModeClass;
		class MouseClass;
		class ObjectEncodingClass;
		class PixelSnappingClass;
		class PrintJobOrientationClass;
		class ProxyClass;
		class SecurityClass;
		class SecurityPanelClass;
		class SharedObjectFlushStatusClass;
		class SoundMixerClass;
		class SpreadMethodClass;
		class StageAlignClass;
		class StageDisplayStateClass;
		class StageQualityClass;
		class StageScaleModeClass;
		class SWFVersionClass;
		class SystemClass;
		class TextColorTypeClass;
		class TextDisplayModeClass;
		class TextFieldAutoSizeClass;
		class TextFieldTypeClass;
		class TextFormatAlignClass;
		class TextLineMetricsClass;
		class TextRendererClass;
		class URLLoaderDataFormatClass;
		class URLRequestHeaderClass;
		class URLRequestMethodClass;
		class URLVariablesClass;
		class XMLNodeClass;
		class XMLNodeTypeClass;
		class ContextMenuClipboardItemsClass;
		class XMLUIClass;
		class IllegalOperationErrorClass;
		class IOErrorClass;
		class MemoryErrorClass;
		class ScriptTimeoutErrorClass;
		class StackOverflowErrorClass;
		class ActivityEventClass;
		class ContextMenuEventClass;
		class FocusEventClass;
		class HTTPStatusEventClass;
		class InvalidSWFErrorClass;
		class KeyboardEventClass;
		class MouseEventClass;
		class NetStatusEventClass;
		class ProgressEventClass;
		class StatusEventClass;
		class SyncEventClass;
		class TextEventClass;
		class TimerEventClass;
		class ContextMenuClass;
		class DisplayObjectClass;
		class IMEClass;
		class NetConnectionClass;
		class SoundChannelClass;
		class StyleSheetClass;
		class MicrophoneClass;
		class NetStreamClass;
		class CameraClass;
		class ContextMenuItemClass;
		class FileReferenceClass;
		class FileReferenceListClass;
		class LoaderInfoClass;
		class LocalConnectionClass;
		class PrintJobClass;
		class SharedObjectClass;
		class SocketClass;
		class SoundClass;
		class TimerClass;
		class URLLoaderClass;
		class URLStreamClass;
		class XMLSocketClass;
		class BlurFilterClass;
		class BevelFilterClass;
		class ColorMatrixFilterClass;
		class ConvolutionFilterClass;
		class DisplacementMapFilterClass;
		class DropShadowFilterClass;
		class GlowFilterClass;
		class GradientBevelFilterClass;
		class GradientGlowFilterClass;
		class XMLDocumentClass;
		class EOFErrorClass;
		class FullScreenEventClass;
		class DataEventClass;
		class ErrorEventClass;
		class IMEEventClass;
		class InteractiveObjectClass;
		class AVM1MovieClass;
		class BitmapClass;
		class MorphShapeClass;
		class ShapeClass;
		class StaticTextClass;
		class VideoClass;
		class AsyncErrorEventClass;
		class IOErrorEventClass;
		class SecurityErrorEventClass;
		class DisplayObjectContainerClass;
		class SimpleButtonClass;
		class TextFieldClass;
		class LoaderClass;
		class SpriteClass;
		class StageClass;
		class MovieClipClass;
		class ApplicationDomainObject;
		class SecurityDomainObject;
		class WorkerDomainObject;
		class LoaderObject;
		class LoaderInfoObject;
		class BitmapDataObject;
		class Matrix3DClass;
		////////////////////////////////////
		class WindowClass;
		class LocationClass;
		class NavigatorClass;

		class URLRequestDefaultsClass;
		class TouchscreenTypeClass;

		class TextBaselineClass;
		class BreakOpportunityClass;
		class DigitCaseClass;
		class CFFHintingClass;
		class RenderingModeClass;
		class FontLookupClass;
		class FontPostureClass;
		class KerningClass;
		class LigatureLevelClass;
		class TextRotationClass;
		class TypographicCaseClass;
		class FontMetricsClass;
		class GroupElementClass;
		class ContentElementClass;
		class TextElementClass;
		class TextLineValidityClass;
		class TextLineClass;
		class UncaughtErrorEventsClass;
		class PerspectiveProjectionClass;
		class ElementFormatClass;

		class Vector3DClass;

		class Stage3D;

		class BitmapEncodingColorSpaceClass;
		class NativeWindowRenderModeClass;
		class VideoCodecClass;
		class H264LevelClass;
		class H264ProfileClass;

		class ThrottleTypeClass;
		class TouchEventIntentClass;
		class AudioDecoderClass;
		class AudioPlaybackModeClass;
		class CameraPositionClass;
		class MessageChannelStateClass;
		class WorkerStateClass;
		class SoftKeyboardTypeClass;
		class ReturnKeyLabelClass;
		class AutoCapitalizeClass;
		class FontWeightClass;
		class NativeWindowInitOptionsClass;
		class DRMDeviceGroupClass;
		class StageTextInitOptionsClass;
		class DRMReturnVoucherCompleteEventClass;
		class MessageChannelClass;

	class ShellCodeContext : public CodeContext
	{
	public:
		Stringp	GetHost(AvmCore*core);
//#if (__CORE_VERSION__>=0x02070200)
//#else
//#ifdef _CODEPOST_
//#else
//		DWB(RCHashTable*)				m_eventsBinding;
//#endif
//#endif
		//struct ObjectChian*				m_eventChains[XCHAIN_MAX];
		DWB(Domain*)					m_domain;
		DWB(DomainEnv*)				    m_domainEnv;
		DRCWB(ApplicationDomainObject*)	m_pAppDomain;
		DRCWB(SecurityDomainObject*)	m_pSecurityDomain;
		DRCWB(WorkerDomainObject*)      m_pWorkerDomain;
		DRCWB(LoaderInfoObject*)		m_pLoaderInfo;
		//LoaderInfoObject*				m_pLoaderInfo;
		//LoaderInfoObject*				m_pLoaderInfo;
		//LoaderInfoObject*				m_pLoaderInfo;
		ShellCore*						m_pCore;
		ScriptPlayer*					m_pRoot;
		ShellCodeContext*			    m_pNext;
		bool						    m_bAccessiable,m_bLazy;
		virtual ~ShellCodeContext();
		ShellCodeContext(ShellCore*pCore,ScriptObject*pDump,MMgc::GC*gc);
		virtual DomainEnv *domainEnv() const { return m_domainEnv; }
//#if (__CORE_VERSION__>=0x02070200)
//#else
//#ifdef _CODEPOST_
//#else
//		//void	AddEvent(int type,ScriptObject*);
//		//void	RemoveEvent(ScriptObject*);
//		bool	PostEvent(class EventObject*,bool bMask);
//#endif
//#endif
		//ShellCodeContext* pNextContext;
	};

	/**
	 * Settings for ShellCore.  The command line parser uses this, but the initial state
	 * is set in ShellCore.cpp, and it's propagated throughout various parts of the
	 * system.  Partly duplicates the functionality in AvmCore::config, and some settings
	 * are copied from ShellCoreSettings into that structure when a new core is created.
	 *
	 * We try to avoid conditional compilation here, as there's only ever one of these
	 * structures floating around and the savings would be minuscule.
	 *
	 * FIXME: the naming of many fields could be much better, and they should be documented.
	 */
	class ShellCoreSettings 
	{
	public:
		//void* pTest;
		ShellCoreSettings();

		char** arguments;				// non-terminated array of argument values, never NULL
		int numargs;					// number of entries in 'arguments'
		bool nodebugger;		
		int  astrace_console;
		uint32_t do_verbose;            // copy to config
		bool enter_debugger_on_launch;
		bool interrupts;				// copy to config
		bool verifyall;					// copy to config
		bool sse2;						// copy to config
        bool fixed_esp;                 // copy to config
		bool greedy;					// copy to each GC
		bool nogc;						// copy to each GC
		bool incremental;				// copy to each GC
		int langID;						// copy to ShellCore?
		bool cseopt;					// copy to config
		bool jitordie;					// copy to config
		Runmode runmode;				// copy to config
		AvmCore::CacheSizes cacheSizes;	// Default to unlimited
		const char* st_component;
		const char* st_category;
		const char* st_name;
		uint32_t api;
		
		MMgc::GC::GCMode gcMode()
		{
			if (nogc)				return MMgc::GC::kDisableGC;
			else if (greedy)		return MMgc::GC::kGreedyGC;
			else if (incremental)	return MMgc::GC::kIncrementalGC;
			else					return MMgc::GC::kNonincrementalGC;
		}		
	};


	
	/**
	 * ShellCore provides typical housekeeping tasks around an AvmCore, lightweight
	 * embedders may prefer to use this rather than to subclass AvmCore themselves
	 * and do all the housekeeping that results.  The shell uses this; see the
	 * shell code for typical usage patterns.
	 */
	class ShellCore : public AvmCore
	{
	public:

		/*struct LazyPoolNode : public MMgc::GCRoot
		{
		public:
			LazyPoolNode* next;
			PoolObject* pool;
			ShellCodeContext*codeContext;
			LazyPoolNode(MMgc::GC* gc) : GCRoot(gc) 
			{
			}
			
		};
		LazyPoolNode* lazyPools,*lazyTail;
		void addLazyPool(PoolObject* pool,ShellCodeContext*code)
		{
			LazyPoolNode* node = new LazyPoolNode(GetGC());
			node->next=NULL;//lazyPools;
			//lazyPools=node;
			node->pool = pool;
			node->codeContext=code;
			if(lazyTail)
			{
				lazyTail->next=node;
				lazyTail=node;
			}
			else
			{
				lazyTail=lazyPools=node;
			}
		}*/
		/*struct POSTEVENT
		{
			EventDispatcherObject* pObject;
			POSTEVENT*	  pNext;
			XU32		  nCreateTime;
		};
		POSTEVENT* m_pPostEvents;//,*m_pLastEvents;
		XFastAlloc eventAlloc;
		void AddPost(EventDispatcherObject*pObject,XU32 nCreateTime)
		{
			POSTEVENT*pNew=(POSTEVENT*)eventAlloc.Alloc();
			if(!pNew) return;
			pNew->pNext=NULL;//m_pPostEvents;//NULL;
			pNew->nCreateTime=nCreateTime;
			pNew->pObject=pObject;
			POSTEVENT**root=&m_pPostEvents;
			for(;;)
			{
				POSTEVENT*pe=*root;
				if(pe==NULL)
				{
					*root=pNew;
					break;
				}
				else if(pe->nCreateTime>nCreateTime)
				{
					pNew->pNext=pe;
					*root=pNew;
					break;
				}
				root=&pe->pNext;
			}
			//m_pPostEvents=pNew;
			
			
		}
		void RemovePost(EventDispatcherObject*pObject)
		{
			POSTEVENT**root=&m_pPostEvents;
			for(;;)
			{
				POSTEVENT*pEvent=*root;
				if(!pEvent) break;
				if(pEvent->pObject==pObject)
				{
					*root=pEvent->pNext;
					eventAlloc.Free(pEvent);
				}
				root=&pEvent->pNext;
			}
		}
		bool PostEvent(EventObject*pEvent,bool bMask);*/
//#if (__CORE_VERSION__>=0x02070200)
		struct POSTCHAIN
		{
			ScriptObject* EventObject;
			POSTCHAIN*	  pNext;
		};
		XFastAlloc postAlloc;
		POSTCHAIN* m_pPostChain;
		bool PostEvent(class EventObject*,bool bMask);
		bool AddPostEvent(ScriptObject*);
		bool RemovePostEvent(ScriptObject*);
//#else
//#ifdef _CODEPOST_
//		//void	AddEvent(int type,ScriptObject*);
//		//void	RemoveEvent(ScriptObject*);
//		bool	PostEvent(class EventObject*,bool bMask);
//#endif
//#endif
		void doLazyPools(const Multiname&name,ShellCodeContext*codeContext);		
	public:
		void throwExtError(int type,Stringp message);	
#ifdef FEATURE_NANOJIT
		void Stop(){m_bBreakOutTime=true;}
#endif
		//void Stop(){m_stopEvent.SetEvent();}
		//virtual void interrupt(Toplevel *env, InterruptReason);
	protected:
		//XEvent m_stopEvent;
		//PoolObject* poolObject;
		//class ScriptPlayer*pRootPlayer;
		XSWFPlayer*m_pPlayer;
		//ContextEnter* m_pContextStack;
	public:
		virtual void ExportClass(Stringp name);
		ShellCodeContext*m_pContextList;
		void SaveContext(ShellCodeContext*pCnt)
		{
			ShellCodeContext*p=m_pContextList;
			while(p)
			{
				if(p==pCnt) return;
				p=p->m_pNext;
			}
			pCnt->m_pNext=m_pContextList;
			m_pContextList=pCnt;
		}
		void RemoveContext(ShellCodeContext*pCnt)
		{
			ShellCodeContext**pc=&m_pContextList;
			//ShellCodeContext*pc=*p;
			for(;;)
			{
				ShellCodeContext*obj=*pc;
				if(!obj) 
					break;
				if(obj==pCnt)
				{
					*pc=obj->m_pNext;
					//pc->m_pNext=pCnt->m_pNext;
					//pCnt->m_pNext=NULL;
					//*p=pc->m_pNext;
					break;
				}
				pc=&obj->m_pNext;
				//pc=pc->m_pNext;
				//p=&(pc->m_pNext);
			}
		}
	public:
		/*inline void PushContext(ContextEnter*pCnt)
		{
			//pCnt->pNextContext=m_pContextStack;
			//m_pContextStack=pCnt;
			pCnt->m_pNext=m_pContextStack;
			m_pContextStack=pCnt;
		}
		inline void PopContext()
		{
			AvmAssert(m_pContextStack!=NULL);
			m_pContextStack=m_pContextStack->m_pNext;

		}
		ScriptPlayer* GetScriptPlayer()
		{
			if(!m_pContextStack)
			{
				ShellCodeContext*c=(ShellCodeContext*)codeContext();
				if(c) return c->m_pRoot;
				return NULL;
			}
			return m_pContextStack->m_pContext->m_pRoot;
		}
		ShellCodeContext* GetCodeContext()
		{
			ShellCodeContext*pCode=(ShellCodeContext*)codeContext();
			if(pCode) return pCode;
			if(m_pContextStack) return m_pContextStack->m_pContext;
			
			return m_pShellContext;
		}*/
	public:
		virtual bool InitClassClosure(ClassClosure*c);
		DRC(Stringp) kCallProperty;
		DRC(Stringp) kDeleteProperty;
		DRC(Stringp) kGetDescendants;
		DRC(Stringp) kGetProperty;
		DRC(Stringp) kHasProperty;
		DRC(Stringp) kIsAttribute;
		DRC(Stringp) kNextName;
		DRC(Stringp) kNextNameIndex;
		DRC(Stringp) kNextValue;
		DRC(Stringp) kSetProperty;
		DRC(Stringp) kFlashProxy;
		DRC(Stringp) kAS3;
		DRC(Stringp) kObjectProxy;
		DRC(Stringp) kProtoHttp;
		DRC(Stringp) kProtoHttps;
		DRC(Stringp) kProtoNone;
		DRC(Stringp) kProtoConnect;
		DRC(Stringp) kWriteExternal;
		DRC(Stringp) kReadExternal;
		DRC(Stringp) kRemoveChild;

		DRC(Stringp) kSpaceString;
		DRC(Stringp) kDrawNow;
		DRC(Stringp) kHookUIInit;//createAccessibilityImplementation
		DRC(Stringp) kEqualString;
		DRC(Stringp) kQuotString;
		DRC(Stringp) kSpString;
		DRC(Stringp) kAMFContent;
		DRC(Stringp) kLeftQ;
		DRC(Stringp) kRightQ;
		DRC(Stringp) kLocalHost;
		DRC(Stringp) kCreateAccessibilityImplementation;
		DRC(Stringp) kOnResult;
		DRC(Stringp) kScript;
		DRC(Stringp) kJavaScriptPackage;
		DRC(FunctionObject*) hookUIFunction;
		DRC(Stringp) kApplication;
		DRC(Stringp) kJQNonamed;
		DRC(Stringp) kCreateModalWindowRequest;

		//dsx new adds
		DRC(Stringp) kEvenOddStr;
		DRC(Stringp) kNoneStr;
		DRC(Stringp) kEulerAnglesStr;
		DRC(Stringp) kSortingStr;
		DRC(Stringp) kLongStr;
		DRC(Stringp) kFullStr;
		DRC(Stringp) kStandaloneStr;
		DRC(Stringp) kUserinterfaceStr;
		DRC(Stringp) kXMethodDict;
		DRC(Stringp) kBRStr;
		DRC(Stringp) kStandardStr;
		DRC(Stringp) kDefaultStr;
		DRC(Stringp) kFloatStr;

//#if (__CORE_VERSION__>=0x02070100)
		DRC(Stringp) kUri;
		DRC(Stringp) kLocalName;
//#endif
		//dsx new adds end

		XSWFPlayer* GetPlayer(){return m_pPlayer;}		
	public:
//#if (__CORE_VERSION__>=0x02076000)
		uintptr_t nEnterStack;
		bool prevCheckStack(int nBits)
		{
			int dummy;
			uintptr_t nNowStack=(uintptr_t)&dummy;
			uintptr_t nMinStack=nEnterStack-(_STACKSIZE>>nBits);
			return nNowStack<nMinStack;
		}
//#endif
		virtual void setStackLimit()
		{
			int dummy;
//#if (__CORE_VERSION__>=0x02076000)
			nEnterStack=(uintptr_t)&dummy;
//#endif
			int nMinStack=(int)(&dummy)-_STACKSIZE;
			AvmCore::setStackLimit(nMinStack);
		}
		/*static bool IsProtoTypeOf(ClassClosure* p,ScriptObject*o)
		{
			//if (AvmCore::isNullOrUndefined(V))
			//return false;
			if(p==XNULL) return false;
			Atom thisAtom=p->prototype->atom();

			//ScriptObject* o = shell_toplevel->toPrototype(V);
			for (; o != NULL; o = o->getDelegate())
			{
				if (o->atom() == thisAtom)
					return true;
			}
			return false;
		}*/
		static bool IsTypeObject(ScriptObject*o,ClassClosure*p)
		{
			if(o==NULL||p==NULL) return false;
			Traits* itraits = p->traits()->itraits;
			//return o->traits()->subtypeof(itraits);
			return AvmCore::istype(o->atom(),itraits);
			//AvmCore::istypeAtom(a1, toplevel->toClassITraits(a2));
			//return AvmCore::istypeAtom(a1, toplevel->toClassITraits(a2));
		}

		static bool IsTypeClass(ScriptObject*o,ClassClosure*p)
		{
			if(o==NULL||p==NULL) return false;
			Traits* itraits = p->traits()->itraits;
			return o->traits()->subtypeof(itraits);
			//return AvmCore::istype(o->atom(),itraits);
			//AvmCore::istypeAtom(a1, toplevel->toClassITraits(a2));
			//return AvmCore::istypeAtom(a1, toplevel->toClassITraits(a2));
		}
		static bool IsSuperClassOf(ScriptObject*p,ScriptObject*o)
		{
			Traits*s=p->traits()->itraits;
			Traits*pc=o->traits()->itraits;
			while(pc)
			{
				if(pc==s) return true;
				pc=pc->base;
			}
			return false;
		}

		//int GetSlotsCount(ScriptObject*pObj,bool bWrite,bool bPublic,bool bAll);
		
		//new adds
		static void scanHardware();
		static void MMEndCommand(bool endStatus, Stringp pNotifyString);

		static void stopSampling();
		static void startSampling();
		static void setSamplerCallback(FunctionObject *pFun);
		static void sampleInternalAllocs(bool b);
		static void pauseSampling();
		static bool isGetterSetter(Atom obj, QNameObject *pQName);
		static double getSize(Atom o);
		static double getSetterInvocationCount(Atom obj, QNameObject *pQName);
		static Atom getSavedThis(FunctionObject *pObj);
		static Atom getSamples();
		static double getSampleCount();
		static Atom getMemberNames(Atom o, bool instanceNames/* = false*/);
		static Stringp getMasterString(Stringp pStr);
		static ArrayObject* getLexicalScopes(FunctionObject *pObj);
		static double getInvocationCount(Atom obj, QNameObject *pQName);
		static double getGetterInvocationCount(Atom obj, QNameObject *pQName);
		static void clearSamples();
		//new adds end

		//void forNative(){}
		//MultinameHashtable* GetEventsBinding(){return m_eventsBinding;}
		static bool _setProperty(ScriptObject*pObject,Atom name,Atom value,Namespacep publicNS,TraitsBindingsp t);
		ClassClosure* getAliasClass(Stringp);
		Stringp		  getClassAlias(ScriptObject*);
		Stringp		  getClassAliasClass(ClassClosure*cs);
		static ClassClosure* getClassByAlias(AvmMethodEnv env,Stringp alias);
		static void registerClassAlias(AvmMethodEnv,Stringp,ClassClosure*);
		static Stringp MMExecute(AvmMethodEnv,Stringp);
		static void showRedrawRegions(AvmMethodEnv,bool on,uint32_t color);
		static void nativeTrace(AvmMethodEnv,ArrayObject*pArguments);
		static void fscommand(AvmMethodEnv,Stringp,Stringp);
		static void clearInterval(AvmMethodEnv,uint32 id);//Cancels a specified setInterval() call. flash.utils 
		static void clearTimeout(AvmMethodEnv,uint32 id); //Cancels a specified setTimeout() call. flash.utils 
		static Atom describeTypeJSON(AvmMethodEnv env, Atom v, uint32_t flags);
		//static ScriptObject* describeType(AvmMethodEnv,Atom);//Produces an XML object that describes the ActionScript object named as the parameter of the method. flash.utils 
		static Stringp escapeMultiByte(AvmMethodEnv,ScriptObject*,Stringp); //Returns an escaped copy of the input string encoded as either UTF-8 or system code page, depending on the value of System.useCodePage. flash.utils 
		static Atom getDefinitionByName(AvmMethodEnv,ScriptObject*,Stringp); //Returns a reference to the class object of the class specified by the name parameter. flash.utils 
		static Stringp getQualifiedClassName(AvmMethodEnv,ScriptObject*,Atom); //Returns the fully qualified class name of an object. flash.utils 
		static Stringp getQualifiedSuperclassName(AvmMethodEnv,ScriptObject*,Atom); //Returns the fully qualified class name of the base class of the object specified by the value parameter. flash.utils 
		static int getTimer(AvmMethodEnv); //Returns the number of milliseconds that have elapsed since Flash Player was initialized, and is used to compute relative time. flash.utils 
		static uint32 nativeSetInterval(AvmMethodEnv env,FunctionObject*closure,double delay,ArrayObject*arguments);
		static uint32 nativeSetTimeout(AvmMethodEnv env,FunctionObject*closure,double delay,ArrayObject*arguments);
		static Stringp unescapeMultiByte(AvmMethodEnv env,ScriptObject*,Stringp value); //Returns an unescaped copy of the input string, which is decoded from either system code page page or UTF-8 depending on the value of System.useCodePage. 

		//void setStackLimit();
		/**
		 * Create a new core with the given GC (one gc per core).
		 *
		 * Requires: MMGC_ENTER and MMGC_GCENTER(gc) on the stack.
		 */
		ShellCore(XSWFPlayer*pPlayer,MMgc::GC *gc);
		void Release();

		/**
		 * Initialize the new core from the settings.  This creates a top-level
		 * environment and performs other housekeeping.
		 *
		 * Requires: MMGC_ENTER and MMGC_GCENTER(gc) on the stack.
		 */
		bool setup(ShellCoreSettings& settings);
		
		/**
		 * Load the contents from the file and run them in the context of this core's
		 * top level.  The file may contain ABC bytecode, a SWF containing a single DoABC
		 * tag, or (if the run-time compiler is present) ActionScript source code.
		 *
		 * Requires: MMGC_ENTER and MMGC_GCENTER(gc) on the stack.
		 */
		//int evaluateFile(ShellCoreSettings& settings, const char* filename);
		PoolObject* handleABC(const void*pCode,int nLength,int nFlags,ShellCodeContext*);
		static bool MultinameOf(Stringp name,Multiname&multiname,AvmCore*core);
		ScriptObject* FindClass(Multiname&name,ShellCodeContext*env,bool bMust);
		ScriptObject* FindClass(Stringp strValue,ShellCodeContext*env,bool bMust);
		inline ScriptObject* FindClass(const char* strName,ShellCodeContext*env,bool bMust)
		{
			Stringp name=newStringUTF8(strName);
			return FindClass(name,env,bMust);
			//return NULL;
		}
		//Toplevel*	  Toplevel(){return shell_toplevel;}
		//DomainEnv* DomainEnv(){return shell_toplevel->domainEnv();}
		//ScriptObject* FindClass(const char*strName);
#ifdef VMCFG_EVAL
		/**
		 * Load ActionScript source code from the string and run it in the 
		 * context of this core's top level.  Note the string must have been 
		 * created in the context of the GC in this ShellCore instance.
		 *
		 * Requires: MMGC_ENTER and MMGC_GCENTER(gc) on the stack.
		 */
		Atom evaluateString(String* input, bool record_time=false);
#endif
		
#ifdef AVMSHELL_PROJECTOR_SUPPORT
		int executeProjector(char *executablePath);
#endif

#ifdef AVMPLUS_SELFTEST
		void executeSelftest(ShellCoreSettings& settings);
#endif

		avmplus::PoolObject* getShellPool() { return shellPool; }
		
#ifdef AVMSHELL_PROJECTOR_SUPPORT
		static bool isValidProjectorFile(const char* filename);
#endif

#ifdef VMCFG_TEST_API_VERSIONING
		inline int32_t getDefaultAPI() { return ApiUtils::toAPI(this, defaultAPIVersion); }
#else
		inline int32_t getDefaultAPI() { return 0; }
#endif

	protected:
		
		
		virtual Toplevel* createToplevel(AbcEnv* abcEnv);
#ifdef DEBUGGER
		virtual avmplus::Debugger* createDebugger(int tracelevel) 
		{
			AvmAssert(allowDebugger >= 0);
			return allowDebugger ? new (GetGC()) DebugCLI(this, (avmplus::Debugger::TraceLevel)tracelevel) : NULL;
		}
		virtual avmplus::Profiler* createProfiler() { AvmAssert(allowDebugger >= 0); return allowDebugger ? new (GetGC()) Profiler(this) : NULL; }
#endif
#ifdef VMCFG_EVAL
		virtual String* readFileForEval(String* referencing_filename, String* filename)
		{
			return NULL;
		}
#endif
	public:
		Toplevel* initShellBuiltins();
		Atom	  DoJavaScript(Stringp name,ArrayObject*args);
	private:
		//static void interruptTimerCallback(void* data);
		
		
		void interrupt(Toplevel*, InterruptReason);
		void stackOverflow(Toplevel *toplevel);
		void setEnv(Toplevel *toplevel, int argc, char *argv[]);
		void initShellPool();
		//int handleArbitraryExecutableContent(ScriptBuffer& code, const char * filename);
		
#ifdef VMCFG_EVAL
		String* decodeBytesAsUTF16String(uint8_t* bytes, uint32_t nbytes, bool terminate=false);
#endif // VMCFG_EVAL
#ifdef DEBUGGER
		DebugCLI* debugCLI() { return (DebugCLI*)debugger(); }
#endif
		
		avmplus::PoolObject* shellPool;
//#if (__CORE_VERSION__>=0x02070200)
		avmplus::PoolObject* externPool;
//#endif
		OutputStream *consoleOutputStream;
		bool gracePeriod;
		bool inStackOverflow;
		int allowDebugger;
		Toplevel* shell_toplevel;
		Domain* shell_domain;
		DomainEnv* shell_domainEnv;
		//for JavaScript
		Domain*	   java_domain;
		DomainEnv* java_domainEnv;
		ShellCodeContext* m_pJavaContext;
		//ApplicationDomain* m_pDomain
		ShellCodeContext* m_pShellContext;
		uint32_t defaultAPIVersion;
		//MultinameHashtable* m_eventsBinding;

	public:
		Toplevel* GetToplevel(){return shell_toplevel;}
		ShellCodeContext* GetShellContext(){return m_pShellContext;}
		//DWB(MultinameHashtable*) m_eventsBinding;
	};

	

	class ShellToplevel : public Toplevel
	{
	public:
		//Delete JavaScript for BUGS
		//WindowClass* getWindowClass(){return (WindowClass*)getBuiltinExtensionClass(abcclass_Window);}
		//LocationClass* getLocationClass(){return (LocationClass*)getBuiltinExtensionClass(abcclass_Location);}
		//NavigatorClass* getNavigatorClass(){return (NavigatorClass*)getBuiltinExtensionClass(abcclass_Navigator);}
		////////////////////////////////////////////////////////////////////////
		ColorTransformClass* getColorTransformClass(){return (ColorTransformClass*)getBuiltinExtensionClass(abcclass_flash_geom_ColorTransform);}
		PointClass* getPointClass(){return (PointClass*)getBuiltinExtensionClass(abcclass_flash_geom_Point);}
		RectangleClass* getRectangleClass(){return (RectangleClass*)getBuiltinExtensionClass(abcclass_flash_geom_Rectangle);}
		MatrixClass* getMatrixClass(){return (MatrixClass*)getBuiltinExtensionClass(abcclass_flash_geom_Matrix);}
		EventClass* getEventClass(){return (EventClass*)getBuiltinExtensionClass(abcclass_flash_events_Event);}
		BitmapDataClass* getBitmapDataClass(){return (BitmapDataClass*)getBuiltinExtensionClass(abcclass_flash_display_BitmapData);}
		ApplicationDomainClass* getApplicationDomainClass(){return (ApplicationDomainClass*)getBuiltinExtensionClass(abcclass_flash_system_ApplicationDomain);}
		SecurityDomainClass* getSecurityDomainClass(){return (SecurityDomainClass*)getBuiltinExtensionClass(abcclass_flash_system_SecurityDomain);}
		ByteArrayClass* getByteArrayClass(bool bCreate=true){return (ByteArrayClass*)getBuiltinExtensionClass(abcclass_flash_utils_ByteArray,bCreate);}
		ContextMenuBuiltInItemsClass* getContextMenuBuiltInItemsClass(){return (ContextMenuBuiltInItemsClass*)getBuiltinExtensionClass(abcclass_flash_ui_ContextMenuBuiltInItems);}
		ContextMenuClipboardItemsClass* getContextMenuClipboardItemsClass(){return (ContextMenuClipboardItemsClass*)getBuiltinExtensionClass(abcclass_flash_ui_ContextMenuClipboardItems);}
		AccessibilityPropertiesClass* getAccessibilityPropertiesClass(){return (AccessibilityPropertiesClass*)getBuiltinExtensionClass(abcclass_flash_accessibility_AccessibilityProperties);}
		TransformClass* getTransformClass(){return (TransformClass*)getBuiltinExtensionClass(abcclass_flash_geom_Transform);}
		EventDispatcherClass* getEventDispatcherClass(){return (EventDispatcherClass*)getBuiltinExtensionClass(abcclass_flash_events_EventDispatcher);}
		URLRequestClass* getURLRequestClass(){return (URLRequestClass*)getBuiltinExtensionClass(abcclass_flash_net_URLRequest);}
		SoundTransformClass* getSoundTransformClass(){return (SoundTransformClass*)getBuiltinExtensionClass(abcclass_flash_media_SoundTransform);}
		ResponderClass* getResponderClass(){return (ResponderClass*)getBuiltinExtensionClass(abcclass_flash_net_Responder);}
		ID3InfoClass* getID3InfoClass(){return (ID3InfoClass*)getBuiltinExtensionClass(abcclass_flash_media_ID3Info);}
		SoundLoaderContextClass* getSoundLoaderContextClass(){return (SoundLoaderContextClass*)getBuiltinExtensionClass(abcclass_flash_media_SoundLoaderContext);}
		TextFormatClass* getTextFormatClass(){return (TextFormatClass*)getBuiltinExtensionClass(abcclass_flash_text_TextFormat);}
		LoaderContextClass* getLoaderContextClass(){return (LoaderContextClass*)getBuiltinExtensionClass(abcclass_flash_system_LoaderContext);}
		BitmapFilterClass* getBitmapFilterClass(){return (BitmapFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_BitmapFilter);}
		TextSnapshotClass* getTextSnapshotClass(){return (TextSnapshotClass*)getBuiltinExtensionClass(abcclass_flash_text_TextSnapshot);}
		GraphicsClass* getGraphicsClass(){return (GraphicsClass*)getBuiltinExtensionClass(abcclass_flash_display_Graphics);}
		SceneClass* getSceneClass(){return (SceneClass*)getBuiltinExtensionClass(abcclass_flash_display_Scene);}
		PrintJobOptionsClass* getPrintJobOptionsClass(){return (PrintJobOptionsClass*)getBuiltinExtensionClass(abcclass_flash_printing_PrintJobOptions);}
		AccessibilityClass* getAccessibilityClass(){return (AccessibilityClass*)getBuiltinExtensionClass(abcclass_flash_accessibility_Accessibility);}
		ActionScriptVersionClass* getActionScriptVersionClass(){return (ActionScriptVersionClass*)getBuiltinExtensionClass(abcclass_flash_display_ActionScriptVersion);}
		AntiAliasTypeClass* getAntiAliasTypeClass(){return (AntiAliasTypeClass*)getBuiltinExtensionClass(abcclass_flash_text_AntiAliasType);}
		//argumentsClass* getargumentsClass(){return (argumentsClass*)getBuiltinExtensionClass(abcclass_arguments);}
		BitmapDataChannelClass* getBitmapDataChannelClass(){return (BitmapDataChannelClass*)getBuiltinExtensionClass(abcclass_flash_display_BitmapDataChannel);}
		BitmapFilterQualityClass* getBitmapFilterQualityClass(){return (BitmapFilterQualityClass*)getBuiltinExtensionClass(abcclass_flash_filters_BitmapFilterQuality);}
		BitmapFilterTypeClass* getBitmapFilterTypeClass(){return (BitmapFilterTypeClass*)getBuiltinExtensionClass(abcclass_flash_filters_BitmapFilterType);}
		BlendModeClass* getBlendModeClass(){return (BlendModeClass*)getBuiltinExtensionClass(abcclass_flash_display_BlendMode);}
		CapabilitiesClass* getCapabilitiesClass(){return (CapabilitiesClass*)getBuiltinExtensionClass(abcclass_flash_system_Capabilities);}
		CapsStyleClass* getCapsStyleClass(){return (CapsStyleClass*)getBuiltinExtensionClass(abcclass_flash_display_CapsStyle);}
		CSMSettingsClass* getCSMSettingsClass(){return (CSMSettingsClass*)getBuiltinExtensionClass(abcclass_flash_text_CSMSettings);}
		CustomActionsClass* getCustomActionsClass(){return (CustomActionsClass*)getBuiltinExtensionClass(abcclass_adobe_utils_CustomActions);}
		DictionaryClass* getDictionaryClass(){return (DictionaryClass*)getBuiltinExtensionClass(abcclass_flash_utils_Dictionary);}
		DisplacementMapFilterModeClass* getDisplacementMapFilterModeClass(){return (DisplacementMapFilterModeClass*)getBuiltinExtensionClass(abcclass_flash_filters_DisplacementMapFilterMode);}
		EndianClass* getEndianClass(){return (EndianClass*)getBuiltinExtensionClass(abcclass_flash_utils_Endian);}
		EventPhaseClass* getEventPhaseClass(){return (EventPhaseClass*)getBuiltinExtensionClass(abcclass_flash_events_EventPhase);}
		FileFilterClass* getFileFilterClass(){return (FileFilterClass*)getBuiltinExtensionClass(abcclass_flash_net_FileFilter);}
		FontClass* getFontClass(){return (FontClass*)getBuiltinExtensionClass(abcclass_flash_text_Font);}
		FontStyleClass* getFontStyleClass(){return (FontStyleClass*)getBuiltinExtensionClass(abcclass_flash_text_FontStyle);}
		FontTypeClass* getFontTypeClass(){return (FontTypeClass*)getBuiltinExtensionClass(abcclass_flash_text_FontType);}
		FrameLabelClass* getFrameLabelClass(){return (FrameLabelClass*)getBuiltinExtensionClass(abcclass_flash_display_FrameLabel);}
		GradientTypeClass* getGradientTypeClass(){return (GradientTypeClass*)getBuiltinExtensionClass(abcclass_flash_display_GradientType);}
		GridFitTypeClass* getGridFitTypeClass(){return (GridFitTypeClass*)getBuiltinExtensionClass(abcclass_flash_text_GridFitType);}
		IMEConversionModeClass* getIMEConversionModeClass(){return (IMEConversionModeClass*)getBuiltinExtensionClass(abcclass_flash_system_IMEConversionMode);}
		InterpolationMethodClass* getInterpolationMethodClass(){return (InterpolationMethodClass*)getBuiltinExtensionClass(abcclass_flash_display_InterpolationMethod);}
		JointStyleClass* getJointStyleClass(){return (JointStyleClass*)getBuiltinExtensionClass(abcclass_flash_display_JointStyle);}
		KeyboardClass* getKeyboardClass(){return (KeyboardClass*)getBuiltinExtensionClass(abcclass_flash_ui_Keyboard);}
		KeyLocationClass* getKeyLocationClass(){return (KeyLocationClass*)getBuiltinExtensionClass(abcclass_flash_ui_KeyLocation);}
		LineScaleModeClass* getLineScaleModeClass(){return (LineScaleModeClass*)getBuiltinExtensionClass(abcclass_flash_display_LineScaleMode);}
		MouseClass* getMouseClass(){return (MouseClass*)getBuiltinExtensionClass(abcclass_flash_ui_Mouse);}
		ObjectEncodingClass* getObjectEncodingClass(){return (ObjectEncodingClass*)getBuiltinExtensionClass(abcclass_flash_net_ObjectEncoding);}
		PixelSnappingClass* getPixelSnappingClass(){return (PixelSnappingClass*)getBuiltinExtensionClass(abcclass_flash_display_PixelSnapping);}
		PrintJobOrientationClass* getPrintJobOrientationClass(){return (PrintJobOrientationClass*)getBuiltinExtensionClass(abcclass_flash_printing_PrintJobOrientation);}
		ProxyClass* getProxyClass(){return (ProxyClass*)getBuiltinExtensionClass(abcclass_flash_utils_Proxy);}
		SecurityClass* getSecurityClass(){return (SecurityClass*)getBuiltinExtensionClass(abcclass_flash_system_Security);}
		SecurityPanelClass* getSecurityPanelClass(){return (SecurityPanelClass*)getBuiltinExtensionClass(abcclass_flash_system_SecurityPanel);}
		SharedObjectFlushStatusClass* getSharedObjectFlushStatusClass(){return (SharedObjectFlushStatusClass*)getBuiltinExtensionClass(abcclass_flash_net_SharedObjectFlushStatus);}
		SoundMixerClass* getSoundMixerClass(){return (SoundMixerClass*)getBuiltinExtensionClass(abcclass_flash_media_SoundMixer);}
		SpreadMethodClass* getSpreadMethodClass(){return (SpreadMethodClass*)getBuiltinExtensionClass(abcclass_flash_display_SpreadMethod);}
		StageAlignClass* getStageAlignClass(){return (StageAlignClass*)getBuiltinExtensionClass(abcclass_flash_display_StageAlign);}
		StageDisplayStateClass* getStageDisplayStateClass(){return (StageDisplayStateClass*)getBuiltinExtensionClass(abcclass_flash_display_StageDisplayState);}
		StageQualityClass* getStageQualityClass(){return (StageQualityClass*)getBuiltinExtensionClass(abcclass_flash_display_StageQuality);}
		StageScaleModeClass* getStageScaleModeClass(){return (StageScaleModeClass*)getBuiltinExtensionClass(abcclass_flash_display_StageScaleMode);}
		SWFVersionClass* getSWFVersionClass(){return (SWFVersionClass*)getBuiltinExtensionClass(abcclass_flash_display_SWFVersion);}
		SystemClass* getSystemClass(){return (SystemClass*)getBuiltinExtensionClass(abcclass_flash_system_System);}
		TextColorTypeClass* getTextColorTypeClass(){return (TextColorTypeClass*)getBuiltinExtensionClass(abcclass_flash_text_TextColorType);}
		TextDisplayModeClass* getTextDisplayModeClass(){return (TextDisplayModeClass*)getBuiltinExtensionClass(abcclass_flash_text_TextDisplayMode);}
		TextFieldAutoSizeClass* getTextFieldAutoSizeClass(){return (TextFieldAutoSizeClass*)getBuiltinExtensionClass(abcclass_flash_text_TextFieldAutoSize);}
		TextFieldTypeClass* getTextFieldTypeClass(){return (TextFieldTypeClass*)getBuiltinExtensionClass(abcclass_flash_text_TextFieldType);}
		TextFormatAlignClass* getTextFormatAlignClass(){return (TextFormatAlignClass*)getBuiltinExtensionClass(abcclass_flash_text_TextFormatAlign);}
		TextLineMetricsClass* getTextLineMetricsClass(){return (TextLineMetricsClass*)getBuiltinExtensionClass(abcclass_flash_text_TextLineMetrics);}
		TextRendererClass* getTextRendererClass(){return (TextRendererClass*)getBuiltinExtensionClass(abcclass_flash_text_TextRenderer);}
		URLLoaderDataFormatClass* getURLLoaderDataFormatClass(){return (URLLoaderDataFormatClass*)getBuiltinExtensionClass(abcclass_flash_net_URLLoaderDataFormat);}
		URLRequestHeaderClass* getURLRequestHeaderClass(){return (URLRequestHeaderClass*)getBuiltinExtensionClass(abcclass_flash_net_URLRequestHeader);}
		URLRequestMethodClass* getURLRequestMethodClass(){return (URLRequestMethodClass*)getBuiltinExtensionClass(abcclass_flash_net_URLRequestMethod);}
		URLVariablesClass* getURLVariablesClass(){return (URLVariablesClass*)getBuiltinExtensionClass(abcclass_flash_net_URLVariables);}
		XMLNodeClass* getXMLNodeClass(){return (XMLNodeClass*)getBuiltinExtensionClass(abcclass_flash_xml_XMLNode);}
		XMLNodeTypeClass* getXMLNodeTypeClass(){return (XMLNodeTypeClass*)getBuiltinExtensionClass(abcclass_flash_xml_XMLNodeType);}
		XMLUIClass* getXMLUIClass(){return (XMLUIClass*)getBuiltinExtensionClass(abcclass_adobe_utils_XMLUI);}
		IllegalOperationErrorClass* getIllegalOperationErrorClass(){return (IllegalOperationErrorClass*)getBuiltinExtensionClass(abcclass_flash_errors_IllegalOperationError);}
		IOErrorClass* getIOErrorClass(){return (IOErrorClass*)getBuiltinExtensionClass(abcclass_flash_errors_IOError);}
		MemoryErrorClass* getMemoryErrorClass(){return (MemoryErrorClass*)getBuiltinExtensionClass(abcclass_flash_errors_MemoryError);}
		ScriptTimeoutErrorClass* getScriptTimeoutErrorClass(){return (ScriptTimeoutErrorClass*)getBuiltinExtensionClass(abcclass_flash_errors_ScriptTimeoutError);}
		StackOverflowErrorClass* getStackOverflowErrorClass(){return (StackOverflowErrorClass*)getBuiltinExtensionClass(abcclass_flash_errors_StackOverflowError);}
		ActivityEventClass* getActivityEventClass(){return (ActivityEventClass*)getBuiltinExtensionClass(abcclass_flash_events_ActivityEvent);}
		ContextMenuEventClass* getContextMenuEventClass(){return (ContextMenuEventClass*)getBuiltinExtensionClass(abcclass_flash_events_ContextMenuEvent);}
		FocusEventClass* getFocusEventClass(){return (FocusEventClass*)getBuiltinExtensionClass(abcclass_flash_events_FocusEvent);}
		HTTPStatusEventClass* getHTTPStatusEventClass(){return (HTTPStatusEventClass*)getBuiltinExtensionClass(abcclass_flash_events_HTTPStatusEvent);}
		InvalidSWFErrorClass* getInvalidSWFErrorClass(){return (InvalidSWFErrorClass*)getBuiltinExtensionClass(abcclass_flash_errors_InvalidSWFError);}
		KeyboardEventClass* getKeyboardEventClass(){return (KeyboardEventClass*)getBuiltinExtensionClass(abcclass_flash_events_KeyboardEvent);}
		MouseEventClass* getMouseEventClass(){return (MouseEventClass*)getBuiltinExtensionClass(abcclass_flash_events_MouseEvent);}
		NetStatusEventClass* getNetStatusEventClass(){return (NetStatusEventClass*)getBuiltinExtensionClass(abcclass_flash_events_NetStatusEvent);}
		ProgressEventClass* getProgressEventClass(){return (ProgressEventClass*)getBuiltinExtensionClass(abcclass_flash_events_ProgressEvent);}
		StatusEventClass* getStatusEventClass(){return (StatusEventClass*)getBuiltinExtensionClass(abcclass_flash_events_StatusEvent);}
		SyncEventClass* getSyncEventClass(){return (SyncEventClass*)getBuiltinExtensionClass(abcclass_flash_events_SyncEvent);}
		TextEventClass* getTextEventClass(){return (TextEventClass*)getBuiltinExtensionClass(abcclass_flash_events_TextEvent);}
		TimerEventClass* getTimerEventClass(){return (TimerEventClass*)getBuiltinExtensionClass(abcclass_flash_events_TimerEvent);}
		ContextMenuClass* getContextMenuClass(){return (ContextMenuClass*)getBuiltinExtensionClass(abcclass_flash_ui_ContextMenu);}
		DisplayObjectClass* getDisplayObjectClass(){return (DisplayObjectClass*)getBuiltinExtensionClass(abcclass_flash_display_DisplayObject);}
		IMEClass* getIMEClass(){return (IMEClass*)getBuiltinExtensionClass(abcclass_flash_system_IME);}
		NetConnectionClass* getNetConnectionClass(){return (NetConnectionClass*)getBuiltinExtensionClass(abcclass_flash_net_NetConnection);}
		SoundChannelClass* getSoundChannelClass(){return (SoundChannelClass*)getBuiltinExtensionClass(abcclass_flash_media_SoundChannel);}
		StyleSheetClass* getStyleSheetClass(){return (StyleSheetClass*)getBuiltinExtensionClass(abcclass_flash_text_StyleSheet);}
		MicrophoneClass* getMicrophoneClass(){return (MicrophoneClass*)getBuiltinExtensionClass(abcclass_flash_media_Microphone);}
		NetStreamClass* getNetStreamClass(){return (NetStreamClass*)getBuiltinExtensionClass(abcclass_flash_net_NetStream);}
		CameraClass* getCameraClass(){return (CameraClass*)getBuiltinExtensionClass(abcclass_flash_media_Camera);}
		ContextMenuItemClass* getContextMenuItemClass(){return (ContextMenuItemClass*)getBuiltinExtensionClass(abcclass_flash_ui_ContextMenuItem);}
		FileReferenceClass* getFileReferenceClass(){return (FileReferenceClass*)getBuiltinExtensionClass(abcclass_flash_net_FileReference);}
		FileReferenceListClass* getFileReferenceListClass(){return (FileReferenceListClass*)getBuiltinExtensionClass(abcclass_flash_net_FileReferenceList);}
		LoaderInfoClass* getLoaderInfoClass(){return (LoaderInfoClass*)getBuiltinExtensionClass(abcclass_flash_display_LoaderInfo);}
		LocalConnectionClass* getLocalConnectionClass(){return (LocalConnectionClass*)getBuiltinExtensionClass(abcclass_flash_net_LocalConnection);}
		PrintJobClass* getPrintJobClass(){return (PrintJobClass*)getBuiltinExtensionClass(abcclass_flash_printing_PrintJob);}
		SharedObjectClass* getSharedObjectClass(){return (SharedObjectClass*)getBuiltinExtensionClass(abcclass_flash_net_SharedObject);}
		SocketClass* getSocketClass(){return (SocketClass*)getBuiltinExtensionClass(abcclass_flash_net_Socket);}
		SoundClass* getSoundClass(){return (SoundClass*)getBuiltinExtensionClass(abcclass_flash_media_Sound);}
		TimerClass* getTimerClass(){return (TimerClass*)getBuiltinExtensionClass(abcclass_flash_utils_Timer);}
		URLLoaderClass* getURLLoaderClass(){return (URLLoaderClass*)getBuiltinExtensionClass(abcclass_flash_net_URLLoader);}
		URLStreamClass* getURLStreamClass(){return (URLStreamClass*)getBuiltinExtensionClass(abcclass_flash_net_URLStream);}
		XMLSocketClass* getXMLSocketClass(){return (XMLSocketClass*)getBuiltinExtensionClass(abcclass_flash_net_XMLSocket);}
		BlurFilterClass* getBlurFilterClass(){return (BlurFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_BlurFilter);}
		BevelFilterClass* getBevelFilterClass(){return (BevelFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_BevelFilter);}
		ColorMatrixFilterClass* getColorMatrixFilterClass(){return (ColorMatrixFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_ColorMatrixFilter);}
		ConvolutionFilterClass* getConvolutionFilterClass(){return (ConvolutionFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_ConvolutionFilter);}
		DisplacementMapFilterClass* getDisplacementMapFilterClass(){return (DisplacementMapFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_DisplacementMapFilter);}
		DropShadowFilterClass* getDropShadowFilterClass(){return (DropShadowFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_DropShadowFilter);}
		GlowFilterClass* getGlowFilterClass(){return (GlowFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_GlowFilter);}
		GradientBevelFilterClass* getGradientBevelFilterClass(){return (GradientBevelFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_GradientBevelFilter);}
		GradientGlowFilterClass* getGradientGlowFilterClass(){return (GradientGlowFilterClass*)getBuiltinExtensionClass(abcclass_flash_filters_GradientGlowFilter);}
		XMLDocumentClass* getXMLDocumentClass(bool bCreate=true){return (XMLDocumentClass*)getBuiltinExtensionClass(abcclass_flash_xml_XMLDocument,bCreate);}
		EOFErrorClass* getEOFErrorClass(){return (EOFErrorClass*)getBuiltinExtensionClass(abcclass_flash_errors_EOFError);}
		FullScreenEventClass* getFullScreenEventClass(){return (FullScreenEventClass*)getBuiltinExtensionClass(abcclass_flash_events_FullScreenEvent);}
		DataEventClass* getDataEventClass(){return (DataEventClass*)getBuiltinExtensionClass(abcclass_flash_events_DataEvent);}
		ErrorEventClass* getErrorEventClass(){return (ErrorEventClass*)getBuiltinExtensionClass(abcclass_flash_events_ErrorEvent);}
		IMEEventClass* getIMEEventClass(){return (IMEEventClass*)getBuiltinExtensionClass(abcclass_flash_events_IMEEvent);}
		InteractiveObjectClass* getInteractiveObjectClass(){return (InteractiveObjectClass*)getBuiltinExtensionClass(abcclass_flash_display_InteractiveObject);}
		AVM1MovieClass* getAVM1MovieClass(){return (AVM1MovieClass*)getBuiltinExtensionClass(abcclass_flash_display_AVM1Movie);}
		BitmapClass* getBitmapClass(){return (BitmapClass*)getBuiltinExtensionClass(abcclass_flash_display_Bitmap);}
		MorphShapeClass* getMorphShapeClass(){return (MorphShapeClass*)getBuiltinExtensionClass(abcclass_flash_display_MorphShape);}
		ShapeClass* getShapeClass(){return (ShapeClass*)getBuiltinExtensionClass(abcclass_flash_display_Shape);}
		StaticTextClass* getStaticTextClass(){return (StaticTextClass*)getBuiltinExtensionClass(abcclass_flash_text_StaticText);}
		VideoClass* getVideoClass(){return (VideoClass*)getBuiltinExtensionClass(abcclass_flash_media_Video);}
		AsyncErrorEventClass* getAsyncErrorEventClass(){return (AsyncErrorEventClass*)getBuiltinExtensionClass(abcclass_flash_events_AsyncErrorEvent);}
		IOErrorEventClass* getIOErrorEventClass(){return (IOErrorEventClass*)getBuiltinExtensionClass(abcclass_flash_events_IOErrorEvent);}
		SecurityErrorEventClass* getSecurityErrorEventClass(){return (SecurityErrorEventClass*)getBuiltinExtensionClass(abcclass_flash_events_SecurityErrorEvent);}
		DisplayObjectContainerClass* getDisplayObjectContainerClass(){return (DisplayObjectContainerClass*)getBuiltinExtensionClass(abcclass_flash_display_DisplayObjectContainer);}
		SimpleButtonClass* getSimpleButtonClass(){return (SimpleButtonClass*)getBuiltinExtensionClass(abcclass_flash_display_SimpleButton);}
		TextFieldClass* getTextFieldClass(){return (TextFieldClass*)getBuiltinExtensionClass(abcclass_flash_text_TextField);}
		LoaderClass* getLoaderClass(){return (LoaderClass*)getBuiltinExtensionClass(abcclass_flash_display_Loader);}
		SpriteClass* getSpriteClass(){return (SpriteClass*)getBuiltinExtensionClass(abcclass_flash_display_Sprite);}
		StageClass* getStageClass(){return (StageClass*)getBuiltinExtensionClass(abcclass_flash_display_Stage);}
		MovieClipClass* getMovieClipClass(){return (MovieClipClass*)getBuiltinExtensionClass(abcclass_flash_display_MovieClip);}
		Matrix3DClass* getMatrix3DClass(){return (Matrix3DClass*)getBuiltinExtensionClass(abcclass_flash_geom_Matrix3D);}

		//dsx new adds
		URLRequestDefaultsClass* getURLRequestDefaultsClass(){return (URLRequestDefaultsClass*)getBuiltinExtensionClass(abcclass_flash_net_URLRequestDefaults);}
		TouchscreenTypeClass* getTouchscreenTypeClass(){return (TouchscreenTypeClass*)getBuiltinExtensionClass(abcclass_flash_system_TouchscreenType);}
		TextBaselineClass* getTextBaselineClass(){return (TextBaselineClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_TextBaseline);}
		BreakOpportunityClass* getBreakOpportunityClass(){return (BreakOpportunityClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_BreakOpportunity);}
		DigitCaseClass* getDigitCaseClass(){return (DigitCaseClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_DigitCase);}


		CFFHintingClass* getCFFHintingClass(){return (CFFHintingClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_CFFHinting);}
		RenderingModeClass*getRenderingModeClass(){return (RenderingModeClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_RenderingMode);}
		FontLookupClass*getFontLookupClass(){return (FontLookupClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_FontLookup);}
		FontPostureClass*getFontPostureClass(){return (FontPostureClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_FontPosture);}
		KerningClass*getKerningClass(){return (KerningClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_Kerning);}
		LigatureLevelClass*getLigatureLevelClass(){return (LigatureLevelClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_LigatureLevel);}
		TextRotationClass*getTextRotationClass(){return (TextRotationClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_TextRotation);}
		TypographicCaseClass*getTypographicCaseClass(){return (TypographicCaseClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_TypographicCase);}
		FontMetricsClass*getFontMetricsClass(){return (FontMetricsClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_FontMetrics);}
		GroupElementClass*getGroupElementClass(){return (GroupElementClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_GroupElement);}
		ContentElementClass*getContentElementClass(){return (ContentElementClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_ContentElement);}
		TextElementClass*getTextElementClass(){return (TextElementClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_TextElement);}
		TextLineValidityClass*getTextLineValidityClass(){return (TextLineValidityClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_TextLineValidity);}
		TextLineClass*getTextLineClass(){return (TextLineClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_TextLine);}
		UncaughtErrorEventsClass*getUncaughtErrorEventsClass(){return (UncaughtErrorEventsClass*)getBuiltinExtensionClass(abcclass_flash_events_UncaughtErrorEvents);}

		PerspectiveProjectionClass*getPerspectiveProjectionClass(){return (PerspectiveProjectionClass*)getBuiltinExtensionClass(abcclass_flash_geom_PerspectiveProjection);}
		ElementFormatClass*getElementFormatClass(){return (ElementFormatClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_ElementFormat);}
		//dsx new adds end
		//Support new Class
		Vector3DClass* getVector3DClass(){return (Vector3DClass*)getBuiltinExtensionClass(abcclass_flash_geom_Vector3D);}
		//end
		
		//Support new 11 Class
		BitmapEncodingColorSpaceClass* getBitmapEncodingColorSpaceClass(){return (BitmapEncodingColorSpaceClass*)getBuiltinExtensionClass(abcclass_flash_display_BitmapEncodingColorSpace);}
		NativeWindowRenderModeClass* getNativeWindowRenderModeClass(){return (NativeWindowRenderModeClass*)getBuiltinExtensionClass(abcclass_flash_display_NativeWindowRenderMode);}
		VideoCodecClass* getVideoCodecClass(){return (VideoCodecClass*)getBuiltinExtensionClass(abcclass_flash_media_VideoCodec);}
		H264LevelClass* getH264LevelClass() {return (H264LevelClass*)getBuiltinExtensionClass(abcclass_flash_media_H264Level);}
		H264ProfileClass* getH264ProfileClass() {return (H264ProfileClass*)getBuiltinExtensionClass(abcclass_flash_media_H264Profile);}

		ThrottleTypeClass* getThrottleTypeClass(){return (ThrottleTypeClass*)getBuiltinExtensionClass(abcclass_flash_events_ThrottleType);}
		TouchEventIntentClass* getTouchEventIntentClass(){return (TouchEventIntentClass*)getBuiltinExtensionClass(abcclass_flash_events_TouchEventIntent);}
		AudioDecoderClass* getAudioDecoderClass() {return (AudioDecoderClass*)getBuiltinExtensionClass(abcclass_flash_media_AudioDecoder);}
		AudioPlaybackModeClass* getAudioPlaybackModeClass(){return (AudioPlaybackModeClass*)getBuiltinExtensionClass(abcclass_flash_media_AudioPlaybackMode);}
		CameraPositionClass* getCameraPositionClass(){return (CameraPositionClass*)getBuiltinExtensionClass(abcclass_flash_media_CameraPosition);}
		MessageChannelStateClass* getMessageChannelStateClass(){return (MessageChannelStateClass*)getBuiltinExtensionClass(abcclass_flash_system_MessageChannelState);}
		WorkerStateClass* getWorkerStateClass(){return (WorkerStateClass*)getBuiltinExtensionClass(abcclass_flash_system_WorkerState);}
		SoftKeyboardTypeClass* getSoftKeyboardTypeClass(){return (SoftKeyboardTypeClass*)getBuiltinExtensionClass(abcclass_flash_text_SoftKeyboardType);}
		ReturnKeyLabelClass* getReturnKeyLabelClass(){return (ReturnKeyLabelClass*)getBuiltinExtensionClass(abcclass_flash_text_ReturnKeyLabel);}
		AutoCapitalizeClass* getAutoCapitalizeClass(){return (AutoCapitalizeClass*)getBuiltinExtensionClass(abcclass_flash_text_AutoCapitalize);}
		FontWeightClass* getFontWeightClass(){return (FontWeightClass*)getBuiltinExtensionClass(abcclass_flash_text_engine_FontWeight);}
		NativeWindowInitOptionsClass* getNativeWindowInitOptionsClass(){return (NativeWindowInitOptionsClass*)getBuiltinExtensionClass(abcclass_flash_display_NativeWindowInitOptions);}
		DRMDeviceGroupClass* getDRMDeviceGroupClass(){return (DRMDeviceGroupClass*)getBuiltinExtensionClass(abcclass_flash_net_drm_DRMDeviceGroup);}
		StageTextInitOptionsClass* getStageTextInitOptionsClass(){return (StageTextInitOptionsClass*)getBuiltinExtensionClass(abcclass_flash_text_StageTextInitOptions);}
		DRMReturnVoucherCompleteEventClass* getDRMReturnVoucherCompleteEventClass(){return (DRMReturnVoucherCompleteEventClass*)getBuiltinExtensionClass(abcclass_flash_events_DRMReturnVoucherCompleteEvent);}
		MessageChannelClass* getMessageChannelClass(){return (MessageChannelClass*)getBuiltinExtensionClass(abcclass_flash_system_MessageChannel);}
	public:		
		ShellToplevel(AbcEnv*);
		virtual ~ShellToplevel()
		{
			if(pEventNames)
				delete pEventNames;
			pEventNames=XNULL;
		}
		ShellCore* core() const {
			return (ShellCore*)Toplevel::core();
		}

		void  SetActive(Stringp url);
		void* CreateImage(Stringp url);
		bool  RemoveImage(Stringp url,bool bPost);
		bool  Connect(Stringp url,LoaderObject*loader);
		bool  DisConnect(Stringp url,LoaderObject*loader);
//#if (__CORE_VERSION__>=0x02075000)
		int   GetRefCount(LoaderObject*loader,Stringp url);
//#endif
		virtual ClassClosure *getBuiltinExtensionClass(int class_id,bool bCreate=true) 
		{ 
			return shellClasses[class_id] ? shellClasses[class_id] : (bCreate?resolveShellClass(class_id):NULL);
		}
		void InitSystemEvents();
		int  MapSysEvent(Stringp strName);
		Stringp IdToEventType(int id){return pEventNames[id];}
		//MultinameHashtable* GetEventsBinding(){return m_eventsBinding;}
		inline void PushObject(ScriptObject*pObj)
		{
			//if(pObj==_pDebugP)
			//	int v=0;
			pRCObjects->put(pObj);
		}
		inline void RemoveObject(ScriptObject*pObj)
		{
			//if(pObj==_pDebugP)
			//	int v=0;
			pRCObjects->remove(pObj);
		}
		inline RCHashTable* GetRCHashTable(){return pRCObjects;}
		inline RCHashTable* GetReleaseObjects(){return pReleaseObjects;}
	private:
		ClassClosure* resolveShellClass(int class_id)
		{
			ClassClosure* cc = findClassInPool(class_id, core()->getShellPool());
			WBRC(core()->GetGC(), shellClasses, &shellClasses[class_id], cc);
			return cc;
		}
	public:
		void AddBitmap(BitmapDataObject*obj);
		void RemoveBitmap(BitmapDataObject*obj);
		bool RestoreBitmap(BitmapDataObject*obj);
		//PoolObject* ImportPool(Multiname*name);
		//void ExportPoolTraits(PoolObject*pool);
	public:
		DWB(RCHashTable*)		 m_bitmapCaches;
		//DWB(HeapHashtable*)		 m_traitsPool;
		DWB(HeapHashtable*)		 m_extFuns;
		DWB(HeapHashtable*)		 m_aliasClasses;
		DWB(HeapHashtable*)		 m_classAlias;
		DWB(HeapHashtable*)		 m_loadImagesTable;
		void*					 GetImage(Stringp url);
		bool					 LockImage(Stringp url);
		bool					 UnLockImage(Stringp url);
		void*					 Attach(Stringp url,void*pData,int l,bool bAttach,bool bImage,const char*msg);
		int						 ClearDeadImage(int id,XU32 t,XU32 tMax);//bool bForce);
		int						 ClearDeadImage(bool bForce);//bool bForce);
		bool					 TriggerComplete(Stringp url,LoaderObject*self);
#ifdef _ANALY
		DWB(RCHashTable*)			m_asBitmaps;
		void AddAsBitmap(XDrawDib*    pDib);
		void RemoveAsBitmap(XDrawDib* pDib);
		_DIBINFO* ShellToplevel::GetDibInfo(_DIBINFO*pRoot,XU32 nTime);
#endif
	public:
		DWB(ClassClosure**)		 shellClasses;
		DWB(MultinameHashtable*) pSystemEvents;
#ifdef _WINEMU
		//RCHashTable*			 pAllObjects;
		class XXObjects:public XSort			
		{
		public:
			struct OBJITEM
			{
				ScriptObject*pObj;
				XString8	strName;
			};
		public:
			virtual ~XXObjects()
			{
				RemoveAll();
			}
			void Add(ScriptObject*pObj)
			{
				int mi;
				int id=Index(pObj,mi);
				if(id>=0) 
					return;
				OBJITEM*item=new OBJITEM;
				item->pObj=pObj;
				Traits*t=pObj->traits();
				StUTF8String s(t->name());
				item->strName=s.c_str();
				objs.InsertAt(mi,item);
			}
			void Remove(ScriptObject*pObj)
			{
				int id=Index(pObj);
				if(id>=0)
				{
					delete objs[id];
					objs.RemoveAt(id);
				}
			}
			void RemoveAll()
			{
				for(int i=0;i<(int)objs.GetSize();i++)
					delete objs[i];
				objs.RemoveAll();
			}
			
			
		XU32 GetCount(){return objs.GetSize();}
		protected:
			XINT Compare(void*iSrt,void*pDst,XU8 /*nArg*/)
			{
				return CompareUInt((XU32)iSrt,(XU32)pDst);
			}
			void* GetData(XU32 iData){return objs[iData]->pObj;}
		public:
			XVector<OBJITEM*> objs;
		};
		static XXObjects m_allObjects;
#endif
		DRCWB(RCObject*)	 m_pDumpObject;
		DWB(RCHashTable*)		 pRCObjects;
		DWB(RCHashTable*)		 pReleaseObjects;
//#if (__CORE_VERSION__>=0x02080000)
		DWB(RCHashTable*)	m_pCallbackObjects;
		int CallLaterObjects()
		{
			int i,nc=m_pCallbackObjects->numQuads,nCount=0;
			for(i=0;i<nc;i++)
			{
				FlashObject*obj=(FlashObject*)m_pCallbackObjects->valueAt(i);
				if(obj==NULL||obj==m_pDumpObject)
					continue;
				obj->LaterCallback();
				nCount++;
			}
			m_pCallbackObjects->clear();
			return nCount;
		}
		void AddLaterObject(FlashObject*obj)
		{
			m_pCallbackObjects->put(obj);
		}
		void RemoveLaterObject(FlashObject*obj)
		{
			m_pCallbackObjects->remove(obj);
		}
//#endif


//#if (__CORE_VERSION__>=0x02070200)

//#else
//#ifdef _CODEPOST_
//		DWB(RCHashTable*)				m_eventsBinding;
//#endif
//#endif
		Stringp*				 pEventNames;
		//ArrayClass*)			arrayClass;
	};
	class ConsoleOutputStream : public OutputStream
	{
	public:
		ConsoleOutputStream(XSWFPlayer*player)
		{
			m_pPlayer=player;
		}
		
		virtual int write(const void *buffer, int count);
	public:
		XSWFPlayer* m_pPlayer;
		XString8	m_output;
	};
}

#endif /* __avmshell_core__ */
