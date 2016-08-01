#ifndef _AS3MovieClip_
#define _AS3MovieClip_
namespace avmplus{namespace NativeID{
class MovieClipClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MovieClipObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}

#include "AS3Sprite.h"

namespace avmshell{
	class SceneObject;
	class MovieClipClass : public ClassClosure
	{
	public:
		MovieClipClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MovieClipClassSlots m_slots_MovieClipClass;
};

#define _FMETHODSIZE(COUNT) (sizeof(FrameInfo)+COUNT*sizeof(Atom))

class MovieClipObject : public SpriteObject
{
	private:
		int m_nTag;
		//ObjectChain* m_pChain;
		//typedef List<DRCWB(ObjectChain*)> FRAMEINFOS;
		//DWB(FRAMEINFOS*)m_pFrameInfo;
		//DWB(EventHashTable*) 
#ifdef _CHAINEVENT
		DWB(ChainHashTable*) m_pFrameInfo;
#else
		DWB(ArrayHashTable*) m_pFrameInfo;
#endif
		//ObjectChain** m_pFrameInfo;
		/*class FrameInfo:public MMgc::GCObject
		{
		public:
			List<ObjectChain*> chains;
			int				   nFrame;//cache
			ObjectChain*	   pChain;
		};
		DWB(FrameInfo*) m_pFrameInfo;*/
		//ObjectChain** m_pFrameInfo;
		//
		//DWB(FRAMEINFOS*)m_pFrameInfo;
		/*struct FrameInfo
		{
			FrameInfo(int v=0)
			{
				iFrame=v;
				pChain=NULL;
			}			
			int iFrame;
			ObjectChain* pChain;
		};
		typedef List<FrameInfo> FRAMEINFOS;
		//DWB(FRAMEINFOS*) m_pFrameInfo;
		FRAMEINFOS* m_pFrameInfo;

		class XSortInfo:public XSort
		{
		public:
			XSortInfo(FRAMEINFOS*pInfo){m_pFrameInfo=pInfo;}
			XU32 GetCount(){return m_pFrameInfo->size()-1;}
			XINT Compare(void*iSrt,void*iDst,XU8)
			{	return ((int)iDst)-((int)iSrt);	}
			void* GetData(XU32 i)
			{
				FrameInfo*pInfo=(FrameInfo*)m_pFrameInfo->getData();
				return (void*)pInfo[i+1].iFrame;
			}
			inline void AddFrameInfo(int iFrame,ScriptObject*pObject)
			{
				XINT index,mi;
				index=Index((void*)iFrame,mi,XSS_NORMAL);
				if(index>=0)
				{
					index++;
					FrameInfo*pInfo=(FrameInfo*)m_pFrameInfo->getData();
					if(!InChain(pInfo[index].pChain,pObject))
						AddChainTail(&pInfo[index].pChain,pObject);
					//int v=0;
				}
				else
				{
					FrameInfo vi;
					vi.iFrame=iFrame;
					AddChainHead(&vi.pChain,pObject);
					mi++;
					m_pFrameInfo->insert(mi,vi);
				}
			}
			inline ObjectChain* FindFrameInfo(int iFrame)
			{
				FrameInfo*pInfo=(FrameInfo*)m_pFrameInfo->getData();
				if(pInfo[0].iFrame==iFrame)
					return pInfo[0].pChain;
				int index=XSort::Index((void*)iFrame);
				if(index>=0)
				{
					index++;
					pInfo[0].iFrame=iFrame;
					pInfo[0].pChain=pInfo[index].pChain;
					return pInfo[index].pChain;
				}
				else
				{
					return NULL;
				}
			}
			FRAMEINFOS*m_pFrameInfo;
		};

		inline void AddFrameInfo(int iFrame,ScriptObject*pObject)
		{
			int size=m_pFrameInfo->size();			
			if(size==0)
			{
				m_pFrameInfo->add(FrameInfo(0));
				m_pFrameInfo->add(FrameInfo(0));
				FrameInfo*pInfo=(FrameInfo*)m_pFrameInfo->getData();
				pInfo[0].iFrame=iFrame;
				
				//pInfo[0].methods=pObject;
				pInfo[1].iFrame=iFrame;
				AddChainHead(&pInfo[1].pChain,pObject);
				pInfo[0].pChain=pInfo[1].pChain;
				//pInfo[1].methods=pObject;
				return;
			}
			XSortInfo sort(m_pFrameInfo);
			sort.AddFrameInfo(iFrame,pObject);
		}
		inline ObjectChain* FindFrameInfo(int iFrame)
		{
			XSortInfo sort(m_pFrameInfo);
			return sort.FindFrameInfo(iFrame);
			//ScriptObject*pObject=NULL;
			//return pObject;
		}*/
		//FrameInfo* m_pFrameInfo;		
	public:
		//DRCWB(ArrayObject*) m_pFrames;
		DRCWB(ArrayObject*) m_pScenes;
		//int nTag;
	public:
		inline SceneObject* GetCurrentScene(int&i);
		inline int GetFrame(Atom frame,Stringp scene);
		inline SceneObject* FindScene(Stringp scene);
		inline int FindFrame(Stringp frame,SceneObject*pScene);
		inline int FindFrame(int iFrame,SceneObject*pScene);
		//int GetFrame(Stringp frame,
		MovieClipObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		virtual bool CallFrame(int iFrame);
		virtual void SetEnter(bool bEnter){m_bEnter=bEnter;}
		virtual bool IsEnter(){return m_bEnter;}
	public:
#ifdef _ANALY
		virtual AS3PROPITEM* GetChildren();
#endif
		int AS3_currentFrame_get();
		Stringp AS3_currentLabel_get();
		Stringp AS3_currentFrameLabel_get();
		ArrayObject* AS3_currentLabels_get();
		SceneObject* AS3_currentScene_get();
		bool AS3_enabled_get();
		void AS3_enabled_set(bool b);
		int AS3_framesLoaded_get();
		ArrayObject* AS3_scenes_get();
		int AS3_totalFrames_get();
		bool AS3_trackAsMenu_get();
		void AS3_trackAsMenu_set(bool b);
		void AS3_constructor();
		void AS3_gotoAndPlay(Atom frame,Stringp scense);
		void AS3_gotoAndStop(Atom frame,Stringp scense);
		void AS3_nextFrame();
		void AS3_nextScene();
		void AS3_play();
		void AS3_prevFrame();
		void AS3_prevScene();
		void AS3_stop();
		void AS3_addFrameScript(ArrayObject*args);//uint32 iFrame,FunctionObject*);
		//new adds 11
		bool AS3_isPlaying_get();
		 
		//new adds 11 end
	private:
		bool m_bEnter;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MovieClipObjectSlots m_slots_MovieClipObject;
};}
#endif