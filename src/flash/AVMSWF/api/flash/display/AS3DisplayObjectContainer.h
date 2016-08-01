#ifndef _AS3DisplayObjectContainer_
#define _AS3DisplayObjectContainer_

#include "AS3InteractiveObject.h"

namespace avmplus{namespace NativeID{
class DisplayObjectContainerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DisplayObjectContainerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextSnapshotObject;
	class PointObject;
	class DisplayObjectObject;
	class DisplayObjectContainerClass : public ClassClosure
	{
	public:
		DisplayObjectContainerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DisplayObjectContainerClassSlots m_slots_DisplayObjectContainerClass;
};
class DisplayObjectContainerObject : public  InteractiveObjectObject
{
	private:
		bool CheckAddObject(DisplayObjectObject*pObj);
	public:
		DWB(RCHashTable*) m_pChildren;
		DisplayObjectContainerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		//void OnEventAll(int id,_XSObject*pObj);
	public:
		void AddChildRef(ScriptObject*obj);
		void RemoveChildRef(ScriptObject*obj);
		MethodEnv* getRemoveMethod(ScriptObject*);
		void FillMembers();
		//void* pData;//Set your data!!
		bool CreateObject();
		bool AS3_mouseChildren_get();
		void AS3_mouseChildren_set(bool);
		int AS3_numChildren_get();
		bool AS3_tabChildren_get();
		void AS3_tabChildren_set(bool);
		TextSnapshotObject* AS3_textSnapshot_get();
		void AS3_constructor();
		DisplayObjectObject* AS3_addChild(DisplayObjectObject*);
		DisplayObjectObject* AS3_addChildAt(DisplayObjectObject*,int i);
		bool AS3_areInaccessibleObjectsUnderPoint(PointObject*);
		XBOOL AS3_contains(DisplayObjectObject*);
		DisplayObjectObject* AS3_getChildAt(int);
		DisplayObjectObject* AS3_getChildByName(Stringp strName);
		int AS3_getChildIndex(DisplayObjectObject*);
		ArrayObject* AS3_getObjectsUnderPoint(PointObject*);
		DisplayObjectObject* AS3_removeChild(DisplayObjectObject*);
		DisplayObjectObject* AS3_removeChildAt(int);
		void AS3_setChildIndex(DisplayObjectObject*,int);
		void AS3_swapChildren(DisplayObjectObject*,DisplayObjectObject*);
		void AS3_swapChildrenAt(int,int);
		ScriptObject* m_pAutoRemoved;

		 //new adds 11
 		 void AS3_removeChildren(int beginIndex = 0, int endIndex = 0x7fffffff);
 		 void AS3_stopAllMovieClips();
 		 //new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DisplayObjectContainerObjectSlots m_slots_DisplayObjectContainerObject;
};}
#endif