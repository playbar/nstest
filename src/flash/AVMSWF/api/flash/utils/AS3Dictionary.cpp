#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "splayer.h"
#include "AS3Dictionary.h"
#include "avm2.h"

namespace avmshell{
DictionaryClass::DictionaryClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	this->ivtable()->traits->isDictionary=true;
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())DictionaryObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* DictionaryClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	ivtable->traits->isDictionary=true;
	return new (core()->GetGC(), ivtable->getExtraSize()) DictionaryObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

DictionaryObject* DictionaryClass::CreateDictionary(bool bWeek)
{
	DictionaryObject*obj=(DictionaryObject*)createInstance(ivtable(),prototype);
	obj->AS3_constructor(bWeek);
	return obj;
}



void DictionaryObject::Release()
{
	/*if(m_hashTable&&IsRunning()&&m_bWeekKeys)
	{
		InlineHashtable*hash=m_hashTable->get_ht();
		int i=0;
		Atom* atoms=hash->getAtoms();
		MMgc::GC *gc = MMgc::GC::GetGC(atoms);
		MMGC_MEM_TYPE(this);
		for(;;)
		{
			i=hash->next(i);
			if(!i) break;
			Atom atom=hash->keyAt(i);//hash->remove
			if(atomKind(atom)!=kSpecialType) continue;
			if((atom&~7)==0) continue;
			MMgc::GCWeakRef*ref=(MMgc::GCWeakRef*)(atom&~7);
			if(ref->get()!=NULL)
				gc->ClearWeakRef(ref->get());
		}
		//m_hashTable=NULL;
		MMGC_MEM_TYPE(NULL);
	}*/
}

void DictionaryObject::AS3_constructor(bool bWeakRef)
{
	m_bWeekKeys=bWeakRef;
	if(m_bWeekKeys) 
		m_hashTable=new (core()->GetGC(), 0) WeakKeyHashtable(core()->GetGC());	
	else
		m_hashTable=new (core()->GetGC(), 0) HeapHashtable(core()->GetGC());	

	AvmAssert(vtable->traits->getHashtableOffset() != 0);
	union {
		uint8_t* p;
		InlineHashtable* iht;
		HeapHashtable** hht;
	};
	p = (uint8_t*)this + vtable->traits->getHashtableOffset();
	*hht=m_hashTable;
}

DictionaryObject::DictionaryObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	
	
	/*if(!vtable->traits->isDictionary)
	{
		if (iht->getCapacity() == 0)
			const_cast<ScriptObject*>(this)->initHashtable(); 
		return iht;
	}
	else
	{
	
		//DictionaryObjects store pointer to HeapHashtable at
		//the hashtable offset
		return (*hht)->get_ht();
	}*/
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
bool DictionaryObject::hasAtomProperty(Atom name)const
{
	//if(atomKind(name)==kObjectType)
	if(AvmCore::isObject(name))
	{
		//name=atomToKey(gc(),name,m_bWeekKeys);
		/*getKey(
		//m_hashTable->add(name,value);
		InlineHashtable *table = m_hashTable->get_ht();//getTable();
		const Atom* atoms = table->getAtoms();
		int i = table->find(name, atoms, table->getCapacity());
		if (atoms[i] != InlineHashtable::EMPTY&&atoms[i]!=InlineHashtable::DELETED)
				return true;
		return false;*/
		return m_hashTable->contains(name);
	}
	return ScriptObject::hasAtomProperty(name);
}

bool DictionaryObject::deleteAtomProperty(Atom name)
{
	if(AvmCore::isObject(name))
	{
		//Atom key=name;
		//name=atomToKey(gc(),name,m_bWeekKeys);
		m_hashTable->remove(name);
		//if(m_bWeekKeys)
		//	gc()->ClearWeakRef((void*)(name&~7));
		return true;
	}
	return ScriptObject::deleteAtomProperty(name);
}

int DictionaryObject::nextNameIndex(int index)
{
	index=ScriptObject::nextNameIndex(index);
	while(index>0)
	{
		Atom atom=m_hashTable->keyAt(index);
		if(atomKind(atom)==kDoubleType)
		{
			MMgc::GCWeakRef*ref=(MMgc::GCWeakRef*)(atom&~7);
			if(!ref||!ref->get())
			{
				//m_hashTable->remove(atom);
				index=ScriptObject::nextNameIndex(index);
			}
			else break;
		}
		else
			break;
	}
	return index;
}

Atom DictionaryObject::nextName(int index)
{
	//index=m_hashTable->next(index);
	//if(index<=0) return undefinedAtom;
	//Atom atom=m_hashTable->keyAt(index);
	Atom atom=ScriptObject::nextName(index);
	//atomToKey(gc(),atom,m_bWeekKeys);
	if(atomKind(atom)==kDoubleType)
	{
		MMgc::GCWeakRef*ref=(MMgc::GCWeakRef*)(atom&~7);
		if(!ref) 
			return nullObjectAtom;
		return ((Atom)ref->get())|kObjectType;
	}
	return atom;

	//atom=keyToAtom(atom,m_bWeekKeys);
	//return atom;
	
}

static Atom _DebugAtom=0;

void DictionaryObject::setAtomProperty(Atom name, Atom value)
{
#ifdef _WINEMU
	/*if(atomKind(name)==kStringType)
	{
		StUTF8String s(AvmCore::atomToString(name));
		XString16 ss(s.c_str());
		//XSWFPlayer::m_pInstance->Trace(ss);
		if(strstr(s.c_str(),"020501")!=0)
		{
			Stringp str=(Stringp)(value&~7);
				//int v=0;
			_DebugAtom=value;
		}
			//StUTF16String ss(AvmCore::atomToString(name))
	}
	if(name==_DebugAtom)
	{
		ScriptObject*obj=(ScriptObject*)(value&~7);
		Stringp as=core()->string(value);
		StUTF16String os(as);
		XSWFPlayer::m_pInstance->Trace(os.c_str());
		int v=0;
	}*/
#endif
	if(AvmCore::isObject(name))
	{
		//name=atomToKey(gc(),name,m_bWeekKeys);
		
		m_hashTable->add(name,value);
		return;
	}

	ScriptObject::setAtomProperty(name,value);
}
Atom DictionaryObject::getAtomProperty(Atom name)const
{
	/*if(atomKind(name)==kStringType)
		{
			StUTF8String s(AvmCore::atomToString(name));
			if(strstr(s.c_str(),"com/assets/t1ViewUI/movie.swf")!=0)
				int v=0;
		}*/
	if(AvmCore::isObject(name))
	{
		//name=atomToKey(gc(),name,m_bWeekKeys);
		
		return m_hashTable->get(name);
	}
	
	return ScriptObject::getAtomProperty(name);
	//Stringp s=core()->string(a);
	//return a;
}

//new adds 11
Atom DictionaryObject::AS3_toJSON(Stringp k)
{
	LOGWHERE();
	return undefinedAtom;
}
//new adds 11 end

}
