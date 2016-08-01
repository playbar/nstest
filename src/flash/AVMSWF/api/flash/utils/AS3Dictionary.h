#ifndef _AS3Dictionary_
#define _AS3Dictionary_
namespace avmplus{namespace NativeID{
class DictionaryClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DictionaryObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DictionaryObject;
	class DictionaryClass : public ClassClosure
	{
	public:
		DictionaryClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		DictionaryObject* CreateDictionary(bool bWeek=true);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DictionaryClassSlots m_slots_DictionaryClass;
};
class DictionaryObject : public ScriptObject
{
	public:
		DictionaryObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		/*inline bool isObject(Atom atom)const
		{
			if(atomKind(atom)==kObjectType&&((atom&~7)!=0))
			   return true;
			return false;
		}
		static Atom atomToKey(MMgc::GC*gc,Atom atom,bool bWeak)
		{
			if(bWeak)
			{
				if(atomKind(atom)==kObjectType&&((atom&~7)!=0))
				{
					atom&=~7;
					MMgc::GCWeakRef*ref=gc->GetWeakRef((const void*)atom);
					atom=((Atom)ref)|kSpecialType;
					//atom|=kSpecialType;
				}
			}
			return atom;
		}
		static Atom keyToAtom(Atom key,bool bWeak)
		{
			if(bWeak)
			{
				if(atomKind(key)==kSpecialType&&((key&~7)!=0))
				{
					key&=~7;
					MMgc::GCWeakRef*ref=(MMgc::GCWeakRef*)key;
					key=((Atom)ref->get())|kObjectType;
					//key|=kObjectType;
				}
			}
			return key;
		}*/
		bool	m_bWeekKeys;
		virtual void Release();
		void AS3_constructor(bool bWeakRef);
		virtual bool deleteAtomProperty(Atom name);
		virtual bool hasAtomProperty(Atom name) const;
		virtual void setAtomProperty(Atom name, Atom value);
		virtual Atom nextName(int index);
		virtual int	 nextNameIndex(int index);
		
		/*virtual bool deleteAtomProperty(Atom name)
		{
			if(atomKind(name)==kObjectType)
			//if (traits()->needsHashtable())
			{
				//Stringp s = core()->atomToString(name);
				//AvmAssert(s->isInterned());
				//Atom ival = s->getIntAtom();
				//if (ival)
				//{
				//	name = ival;
				//}

				getTable()->remove(name);
				return true;
			}
			return ScriptObject::deleteAtomProperty(name);
		}*/
		virtual Atom getAtomProperty(Atom name) const;

		//new adds 11
		Atom AS3_toJSON(Stringp k);
		//new adds 11 end
	private:
		DWB(HeapHashtable*) m_hashTable;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DictionaryObjectSlots m_slots_DictionaryObject;
};}
#endif