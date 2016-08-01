#ifndef _RCHASHTABLE_
#define _RCHASHTABLE_


#include "avmplus.h"
using namespace avmplus;
using namespace MMgc;

#define _CHAINEVENT

//extern void * _DebugWindow;

namespace avmshell
{
	
#ifdef _CHAINEVENT
	class ObjectChain:public MMgc::GCObject
	{
		//Stringp		  pName;
	public:
		ObjectChain()
		{
			pObject=NULL;
			pNext=NULL;
			nPpriority=0;
		}
		DRCWB(ScriptObject*)   pObject;
		XU32				   nPpriority;
		XU32				   nCreateTime;
		XU32				   bUseCapture;
		DWB(ObjectChain*)	   pNext;
	};
	
	REALLY_INLINE void FreeChain(ObjectChain*root)
	{
		while(root)
		{
			ObjectChain*pNext=root->pNext;
			root->pObject=NULL;
			root->pNext=NULL;
			delete root;
			root=pNext;
		}
	}
	
	REALLY_INLINE ObjectChain* RemoveChain(ObjectChain*root,ScriptObject*pObject)//,RCHashTable*pHash)
	{			
		//return ret;
		ObjectChain*ret=root;
		if(root->pObject==pObject)
		{
			ret=root->pNext;
			root->pObject=NULL;
			root->pNext=NULL;
			delete root;
		}
		else
		{
			//root=root->pNext;
			for(;;)
			{
				ObjectChain*pNext=root->pNext;
				if(!pNext) break;
				if(pNext->pObject==pObject)
				{					
					root->pNext=pNext->pNext;
					pNext->pObject=NULL;
					pNext->pNext=NULL;
					delete pNext;
					break;
				}
				root=pNext;	
			}
		}
		return ret;
	}

	REALLY_INLINE bool InChain(ObjectChain*root,ScriptObject*pObject)
	{
		while(root)
		{
			if(root->pObject==pObject) return true;
			root=root->pNext;
		}
		return false;
	}

	REALLY_INLINE ObjectChain* AddChainTail(ObjectChain*root,ScriptObject*pObject,XU32 np,XU32 t,XBOOL bCapture,MMgc::GC*gc)//,RCHashTable*pHash)
	{
		ObjectChain*pChain=new (gc)ObjectChain();
			//(ObjectChain*)chainAlloc.Alloc();
		//pChain->pName=NULL;
		//pObject->IncrementRef();
		//pHash->put(pObject);		
		pChain->pObject=pObject;
		pChain->nPpriority=np;
		pChain->pNext=NULL;
		pChain->nCreateTime=t;
		pChain->bUseCapture=bCapture;
		if(root==NULL) return pChain;
		ObjectChain*p=root;
		if(root->nPpriority==-1)
		{
			pChain->pNext=root;
			root=pChain;
		}
		else
		{
			if(np>root->nPpriority)
			{
				pChain->pNext = root;
				return pChain;
			}
			for(;;)
			{
				ObjectChain*pNext=p->pNext;
				if(!pNext||np>pNext->nPpriority)
				{
					pChain->pNext=pNext;
					p->pNext=pChain;
					break;
				}
				p=pNext;
			}
		}
		return root;

	}


class ChainArray : public MMgc::GCObject
	{		
	public:
		ChainArray(int capacity):
		  m_pChain(NULL),
		  numQuads(0)
		{
			Init(capacity);
		}
		void Init(int capacity)
		{
			if (capacity)
			{
				numQuads = capacity;//MathUtils::nextPowerOfTwo(capacity);

				MMgc::GC* gc = MMgc::GC::GetGC(this);

				//AvmAssert(numQuads > 0);
				MMGC_MEM_TYPE(this);
				ObjectChain** newAtoms = (ObjectChain**) gc->Alloc (sizeof(ObjectChain*) * numQuads, GC::kContainsPointers|GC::kZero);
				WB(gc, this, &m_pChain, newAtoms);
			}
		}
		inline void grow()
		{
			int capacity = numQuads*2;
			MMgc::GC* gc = MMgc::GC::GetGC(this);
			MMGC_MEM_TYPE(this);
			ObjectChain** newAtoms = (ObjectChain**) gc->Calloc (capacity,sizeof(ObjectChain*), GC::kContainsPointers|GC::kZero);
			//rehash(m_quads, numQuads, newAtoms, capacity);
			VMPI_memcpy(newAtoms,m_pChain,numQuads*sizeof(ObjectChain*));
			gc->Free(m_pChain);
			WB(gc, this, &m_pChain, newAtoms);
			numQuads = capacity;
		}
		inline void setAt(int i,ObjectChain*chain)
		{
			MMgc::GC* gc = MMgc::GC::GetGC(m_pChain);
			WB(gc,m_pChain,&m_pChain[i],chain);
			//WBRC(gc,quadbase,&cur->owner,NULL);
		}
		inline ObjectChain* getAt(int i)
		{	return m_pChain[i];	}
		inline int put(ObjectChain*chain)
		{
			int id=numQuads;
			grow();
			setAt(id,chain);
			return id;
		}
		inline int emptyID()
		{
			for(int i=0;i<numQuads;i++)
			{
				if(m_pChain[i]==NULL) return i;
			}
			return -1;
		}
		inline int add(ObjectChain*chain)
		{
			int id=emptyID();
			if(id<0)
				id=put(chain);
			else
				setAt(id,chain);
			return id;
		}
		inline void clear()
		{
			MMgc::GC* gc = MMgc::GC::GetGC(m_pChain);
			for(int i=0;i<numQuads;i++)
			{
				if(!m_pChain[i]) continue;
				FreeChain(m_pChain[i]);
				WB(gc,m_pChain,&m_pChain[i],NULL);
			}
		}
		inline int newObject(XBOOL bCapture,ScriptObject*pObj,int np,XU32 t)
		{
			int id=emptyID();
			if(id<0)
			{
				id=numQuads;
				grow();
				
			}
			addObject(id,bCapture,pObj,np,t);
			return id;//m_pChain[id];
		}
		inline void addObject(int i,XBOOL bCapture,ScriptObject*pObj,int np,XU32 t,bool noSame=true)
		{
			MMgc::GC* gc = MMgc::GC::GetGC(m_pChain);
			if(noSame)
			{
				if(InChain(m_pChain[i],pObj)) return;
			}
			ObjectChain*pChain=AddChainTail(m_pChain[i],pObj,np,t,bCapture,gc);
			setAt(i,pChain);
		}
		inline ObjectChain* removeObject(int i,ScriptObject*pObj)
		{
			if(!InChain(m_pChain[i],pObj)) return NULL;
			MMgc::GC* gc = MMgc::GC::GetGC(m_pChain);
			ObjectChain*pChain=RemoveChain(m_pChain[i],pObj);
			WB(gc,m_pChain,&m_pChain[i],pChain);
			return pChain;
		}
	public:
		ObjectChain**m_pChain;
		int numQuads;
	};
class ObjectArray : public MMgc::GCObject
	{		
	public:
		~ObjectArray()
		{			
			if(m_pObjects&&!gc->IsPointerToGCPage(this))
				gc->Free(m_pObjects);
		}
		ObjectArray(MMgc::GC*gc,int capacity):
		  m_pObjects(NULL),
		  numQuads(0),
		  size(0)
		{
			Init(gc,capacity);
		}
		void Init(MMgc::GC*gc,int capacity)
		{
			if (capacity)
			{
				numQuads = capacity;//MathUtils::nextPowerOfTwo(capacity);

				if(gc==NULL)
					gc = MMgc::GC::GetGC(this);
				this->gc=gc;
				//AvmAssert(numQuads > 0);
				//MMGC_MEM_TYPE(this);
				ScriptObject** newAtoms = (ScriptObject**) gc->Alloc (sizeof(ScriptObject*) * numQuads, GC::kContainsPointers|GC::kZero);
				if(gc->IsPointerToGCPage(this))
					WB(gc, this, &m_pObjects, newAtoms);
				else
				{
					//gc->FreeNotNull(m_pObjects);
					m_pObjects=newAtoms;
				}
			}
		}
		inline void grow()
		{
			int capacity = numQuads*2;
			//MMgc::GC* gc = MMgc::GC::GetGC(this);
			//MMGC_MEM_TYPE(this);
			ScriptObject** newAtoms = (ScriptObject**) gc->Calloc (capacity,sizeof(ScriptObject*), GC::kContainsPointers|GC::kZero);
			//rehash(m_quads, numQuads, newAtoms, capacity);
			VMPI_memcpy(newAtoms,m_pObjects,numQuads*sizeof(ObjectChain*));
			gc->Free(m_pObjects);
			if(gc->IsPointerToGCPage(this))
				WB(gc, this, &m_pObjects, newAtoms);
			else
				m_pObjects=newAtoms;
			numQuads = capacity;
		}
		inline void setAt(int i,ScriptObject*obj)
		{
			MMgc::GC* gc = MMgc::GC::GetGC(m_pObjects);
			WBRC(gc,m_pObjects,&m_pObjects[i],obj);
			if(size<i) size=i;
			//WBRC(gc,quadbase,&cur->owner,NULL);
		}
		inline ScriptObject* getAt(int i)
		{	return m_pObjects[i];	}
		inline int put(ScriptObject*obj)
		{
			int id=size;
			if(size>=numQuads)
				grow();
			setAt(id,obj);
			size++;
			return id;
		}
		
	public:
		MMgc::GC*	 gc;
		ScriptObject**m_pObjects;
		int numQuads,size;
	};
class ChainHashTable : public MMgc::GCObject
	{
		struct QUAD
		{
			ObjectChain*chain;
			int			key;
		};
	public:
		ChainHashTable(int capacity) : 
		m_quads(NULL),
		size(0),
		numQuads(0)
		{
			//m_pEmpty=new (MMgc::GC::GetGC(this))ObjectChain();
			Init(capacity);
		}
		void Init(int capacity)
		{
			if (capacity)
			{
				numQuads = MathUtils::nextPowerOfTwo(capacity);

				MMgc::GC* gc = MMgc::GC::GetGC(this);

				AvmAssert(numQuads > 0);
				MMGC_MEM_TYPE(this);
				QUAD* newAtoms = (QUAD*) gc->Alloc (sizeof(QUAD) * numQuads, GC::kContainsPointers|GC::kZero);
				VMPI_memset(newAtoms,0,sizeof(QUAD) * numQuads);
				WB(gc, this, &m_quads, newAtoms);
			}
		}
	protected:
		inline void grow()
		{
			int capacity = numQuads*2;
			MMgc::GC* gc = MMgc::GC::GetGC(this);
			MMGC_MEM_TYPE(this);
			QUAD* newAtoms = (QUAD*) gc->Calloc(capacity, sizeof(QUAD), GC::kContainsPointers|GC::kZero);
			VMPI_memset(newAtoms,0,sizeof(QUAD) * numQuads);
			rehash(m_quads, numQuads, newAtoms, capacity);
			gc->Free(m_quads);
			WB(gc, this, &m_quads, newAtoms);
			numQuads = capacity;
		}

		inline int find(int key,const QUAD* t, unsigned m)const
		{

			// this is a quadratic probe but we only hit every third slot since those hold keys.
			int n = 7;

			int bitmask = (m - 1);

			// Note: Mask off MSB to avoid negative indices.  Mask off bottom
			// 3 bits because it doesn't contribute to hash.  Quad it
			// because names, namespaces, and values are stored adjacently.
			unsigned i = ((0x7FFFFFF8 & (uintptr)key) >> 3) & bitmask;

			int k;
			while (((k=t[i].key) != key ) && k != 0 )
			{				
				i = (i + (n++)) & bitmask;			// quadratic probe
			}
			return i;
		}
		inline void rehash(QUAD* oldAtoms, int oldTriplet, QUAD* newAtoms, int newTriplet)
		{
			size=0;
			for (int i=0, n=oldTriplet; i < n; i++)
			{
				//if(oldAtoms[i].pObj)
				//{
				//Stringp oldName;
				//Stringp oldName;
				int oldKey;
				if ((oldKey=oldAtoms[i].key) != 0&&oldKey!=-1)
				{
					// inlined & simplified version of put()
					size++;
					int j = find(oldKey, newAtoms, newTriplet);
					// don't need WBRC/WB here because we are just moving pointers
					newAtoms[j].key = oldKey;
					newAtoms[j].chain= oldAtoms[i].chain;
				}
				//newAtoms[i]=oldAtoms[i];
				//}
			}
		}
		inline bool isFull() const
		{ 
			// 0.80 load factor
			return 5*(size+1) >= numQuads*4; 
		}
	public:
		inline int keyAt(int index)
		{
			return m_quads[index].key;
		}
		inline ObjectChain* chainAt(int index)
		{
			return m_quads[index].chain;
		}		

		inline ObjectChain* addObject(int key,XBOOL bCapture,ScriptObject*pObj,XU32 np,XU32 tm,bool noSame)
		{
			if(isFull()) grow();
			QUAD* t = m_quads;
			int i = find(key, t, numQuads);
			if(!t[i].key&&t[i].key!=-1) size++;
			t[i].key=key;
			if(!noSame||!InChain(t[i].chain,pObj))
			{
				GC* gc = GC::GetGC(m_quads);
				ObjectChain*pChain=AddChainTail(t[i].chain,pObj,np,tm,bCapture,gc);
				WB(gc,m_quads,&t[i].chain,pChain);
			}
			return t[i].chain;
		}

		inline ObjectChain* get(int key) const
		{
			const QUAD* t = m_quads;
			int i = find(key, t, numQuads);
			if (t[i].key == key)
			{
				const QUAD& tf = t[i];
				return tf.chain;
			}
			return NULL;
		}
		int clear()
		{
			GC* gc = GC::GetGC(m_quads);
			QUAD* cur;
			int iCount=0;
			QUAD* const quadbase = m_quads;
			for(int i=0;i<numQuads;i++)
			{
				cur=quadbase+i;
				if(!cur->chain) continue;
				size--;
				//WBRC(gc,quadbase,&cur->owner,NULL);
				cur->key=0;
				//ObjectChain*chain=cur->chain;
				FreeChain(cur->chain);
				//cur->=NULL;
				WB(gc,quadbase,&cur->chain,NULL);
				//WB(gc,quadbase,&cur->value,0);
			}
			size=0;
			return iCount;
		}
		void remove(int key)
		{
			AvmAssert(key>0);			
			GC* gc = GC::GetGC(m_quads);

			QUAD* cur;
			QUAD* const quadbase = m_quads;
			{
				int n = 7;
				int const bitmask = (numQuads - 1);
				unsigned i = ((0x7FFFFFF8 & (uintptr)key) >> 3) & bitmask;
				cur = quadbase + i;
				for (;;)
				{
					//Stringp probeName=cur->name;
					int probeKey=cur->key;
					if (!probeKey)
					{
						// found the hole.
						return;
					}

					if (probeKey == key )
					{
						// it's the one we're looking for, just update the value.
						break;
					}
					i = (i + (n++)) & bitmask;			// quadratic probe
					cur = quadbase + i;
				}
			}

			cur->key=-1;
			WB(gc,quadbase,&cur->chain,NULL);
			//size--;

		}
		
		bool put(int key,ObjectChain*chain)
		{
			bool bGrow=false;
			if(isFull()) {grow();bGrow=true;}
			//AvmAssert(!isFull());			
			GC* gc = GC::GetGC(m_quads);

			QUAD* cur;
			QUAD* const quadbase = m_quads;
			{
				int n = 7;
				int const bitmask = (numQuads - 1);
				unsigned i = ((0x7FFFFFF8 & (uintptr)key) >> 3) & bitmask;
				cur = quadbase + i;
				for (;;)
				{
					int probeName=cur->key;
					if (!probeName||probeName==-1)
					{
						// found the hole.
						break;
					}

					if (probeName == key )
					{
						// it's the one we're looking for, just update the value.						
							goto write_value;
					}
					i = (i + (n++)) & bitmask;			// quadratic probe
					cur = quadbase + i;
				}
			}
			
			// New table entry for this <name,ns> pair
			//if(newValue)
			{
				if(!cur->key)
					size++;
				cur->key=key;
				//WBRC(gc,quadbase,&cur->owner,owner);
				//cur->owner=owner;
				//cur->nRefs=1;
				//quads[i].name = name;
			}
			//else return;
	write_value:
			WB(gc,quadbase,&cur->chain,chain);
			//quads[i].value = value;
			//cur->value=value;
			//WB(gc, quadbase, &cur->value, value);
			//WBRC(gc, quadbase, cur->pObj, newValue);
			return bGrow;
		}
		
	private:
		QUAD* m_quads;		
	public:
		int			  numQuads;
		int			  size;
	};
#else
	REALLY_INLINE void FreeChain(ArrayObject*root)
	{
		int i,iCount=root->get_length();
		for(i=0;i<iCount;i++)
		{
			root->setUintProperty(i,nullObjectAtom);
		}
		/*while(root)
		{
			ObjectChain*pNext=root->pNext;
			root->pObject=NULL;
			root->pNext=NULL;
			delete root;
			root=pNext;
		}*/
	}
	
	REALLY_INLINE ArrayObject* RemoveChain(ArrayObject*root,ScriptObject*pObject)//,RCHashTable*pHash)
	{			
		//return ret;
		ArrayObject*ret=root;
		
		if(root==NULL) return NULL;
		int i,iSets=0,iCount=root->get_length();
		for(i=0;i<iCount;i++)
		{
			Atom atom=root->getUintProperty(i);
			if(AvmCore::isNullOrUndefined(atom)) continue;
			ScriptObject*obj=AvmCore::atomToScriptObject(atom);
			if(obj==pObject)
				root->setUintProperty(i,nullObjectAtom);
			else
				iSets++;
		}
		return iSets?root:NULL;
		/*if(root->pObject==pObject)
		{
			ret=root->pNext;
			root->pObject=NULL;
			root->pNext=NULL;
			delete root;
		}
		else
		{
			//root=root->pNext;
			for(;;)
			{
				ObjectChain*pNext=root->pNext;
				if(!pNext) break;
				if(pNext->pObject==pObject)
				{					
					root->pNext=pNext->pNext;
					pNext->pObject=NULL;
					pNext->pNext=NULL;
					delete pNext;
					break;
				}
				root=pNext;	
			}
		}
		return ret;*/
	}

	REALLY_INLINE bool InChain(ArrayObject*root,ScriptObject*pObject)
	{
		/*while(root)
		{
			if(root->pObject==pObject) return true;
			root=root->pNext;
		}
		return false;*/
		if(!root) return false;
		int i,iCount=root->get_length();
		for(i=0;i<iCount;i++)
		{
			Atom atom=root->getUintProperty(i);
			if(AvmCore::atomToScriptObject(atom)==pObject) return true;
		}
		return false;
	}

	REALLY_INLINE ArrayObject* AddChainTail(ArrayObject*root,ScriptObject*pObject,MMgc::GC*gc,Toplevel*top)//,RCHashTable*pHash)
	{
		if(root==NULL)
		{
			root=top->arrayClass->newArray(1);
			root->setUintProperty(0,pObject->atom());
			return root;
		}
		else
		{
			int i,iCount=root->get_length();
			for(i=0;i<iCount;i++)
			{
				Atom atom=root->getUintProperty(i);
				if(AvmCore::isNullOrUndefined(atom))
				{
					root->setUintProperty(i,pObject->atom());
					return root;
				}
			}
			Atom atom=pObject->atom();
			root->push(&atom,1);
		}
		return root;
	}
	class ArrayHashTable : public MMgc::GCObject
	{
		struct QUAD
		{
			Atom	chain;
			int		key;
		};
	public:
		ArrayHashTable(int capacity) : 
		m_quads(NULL),
		size(0),
		numQuads(0)
		{
			//m_pEmpty=new (MMgc::GC::GetGC(this))ObjectChain();
			Init(capacity);
		}
		void Init(int capacity)
		{
			if (capacity)
			{
				numQuads = MathUtils::nextPowerOfTwo(capacity);

				MMgc::GC* gc = MMgc::GC::GetGC(this);

				AvmAssert(numQuads > 0);
				MMGC_MEM_TYPE(this);
				QUAD* newAtoms = (QUAD*) gc->Alloc (sizeof(QUAD) * numQuads, GC::kContainsPointers|GC::kZero);
				VMPI_memset(newAtoms,0,sizeof(QUAD) * numQuads);
				for(int i=0;i<capacity;i++)
					WBATOM(gc,newAtoms,&newAtoms[i].chain,nullObjectAtom);
				WB(gc, this, &m_quads, newAtoms);
			}
		}
	protected:
		inline void grow()
		{
			int capacity = numQuads*2;
			MMgc::GC* gc = MMgc::GC::GetGC(this);
			MMGC_MEM_TYPE(this);
			QUAD* newAtoms = (QUAD*) gc->Calloc(capacity, sizeof(QUAD), GC::kContainsPointers|GC::kZero);
			VMPI_memset(newAtoms,0,sizeof(QUAD) * numQuads);
			for(int i=0;i<capacity;i++)
				WBATOM(gc,newAtoms,&newAtoms[i].chain,nullObjectAtom);
			rehash(m_quads, numQuads, newAtoms, capacity);
			gc->Free(m_quads);
			WB(gc, this, &m_quads, newAtoms);
			numQuads = capacity;
		}
		inline int find(int key,const QUAD* t, unsigned m)const
		{

			// this is a quadratic probe but we only hit every third slot since those hold keys.
			int n = 7;

			int bitmask = (m - 1);

			// Note: Mask off MSB to avoid negative indices.  Mask off bottom
			// 3 bits because it doesn't contribute to hash.  Quad it
			// because names, namespaces, and values are stored adjacently.
			unsigned i = ((0x7FFFFFF8 & (uintptr)key) >> 3) & bitmask;

			int k;
			while (((k=t[i].key) != key ) && k != 0 )
			{				
				i = (i + (n++)) & bitmask;			// quadratic probe
			}
			return i;
		}
		inline void rehash(QUAD* oldAtoms, int oldTriplet, QUAD* newAtoms, int newTriplet)
		{
			size=0;
			for (int i=0, n=oldTriplet; i < n; i++)
			{
				//if(oldAtoms[i].pObj)
				//{
				//Stringp oldName;
				//Stringp oldName;
				int oldKey;
				if ((oldKey=oldAtoms[i].key) != 0&&oldKey!=-1)
				{
					// inlined & simplified version of put()
					size++;
					int j = find(oldKey, newAtoms, newTriplet);
					// don't need WBRC/WB here because we are just moving pointers
					newAtoms[j].key = oldKey;
					newAtoms[j].chain= oldAtoms[i].chain;
				}
				//newAtoms[i]=oldAtoms[i];
				//}
			}
		}
		inline bool isFull() const
		{ 
			// 0.80 load factor
			return 5*(size+1) >= numQuads*4; 
		}
	public:
		inline int keyAt(int index)
		{
			return m_quads[index].key;
		}
		inline ArrayObject* chainAt(int index)
		{
			return (ArrayObject*)AvmCore::atomToScriptObject(m_quads[index].chain);
		}		

		inline ArrayObject* addObject(int key,ScriptObject*pObj,bool noSame,Toplevel*top)
		{
			if(isFull()) grow();
			QUAD* t = m_quads;
			int i = find(key, t, numQuads);
			if(!t[i].key&&t[i].key!=-1) size++;
			t[i].key=key;
			ArrayObject*chains=(ArrayObject*)AvmCore::atomToScriptObject(t[i].chain);
			if(!noSame||!InChain(chains,pObj))
			{
				GC* gc = GC::GetGC(m_quads);
				ArrayObject*pChain=AddChainTail(chains,pObj,gc,top);
				WBATOM(gc,m_quads,&t[i].chain,pChain->atom());
			}
			return (ArrayObject*)AvmCore::atomToScriptObject(t[i].chain);
		}

		inline ArrayObject* get(int key) const
		{
			const QUAD* t = m_quads;
			int i = find(key, t, numQuads);
			if (t[i].key == key)
			{
				const QUAD& tf = t[i];
				return (ArrayObject*)AvmCore::atomToScriptObject(tf.chain);
			}
			return NULL;
		}
		int clear()
		{
			GC* gc = GC::GetGC(m_quads);
			QUAD* cur;
			int iCount=0;
			QUAD* const quadbase = m_quads;
			for(int i=0;i<numQuads;i++)
			{
				cur=quadbase+i;
				if(AvmCore::isNullOrUndefined(cur->chain)) continue;
				size--;
				//WBRC(gc,quadbase,&cur->owner,NULL);
				cur->key=0;
				//ObjectChain*chain=cur->chain;
				FreeChain((ArrayObject*)AvmCore::atomToScriptObject(cur->chain));
				//cur->=NULL;
				WBATOM(gc,quadbase,&cur->chain,nullObjectAtom);
				//WB(gc,quadbase,&cur->value,0);
			}
			size=0;
			return iCount;
		}
		void remove(int key)
		{
			AvmAssert(key>0);			
			GC* gc = GC::GetGC(m_quads);

			QUAD* cur;
			QUAD* const quadbase = m_quads;
			{
				int n = 7;
				int const bitmask = (numQuads - 1);
				unsigned i = ((0x7FFFFFF8 & (uintptr)key) >> 3) & bitmask;
				cur = quadbase + i;
				for (;;)
				{
					//Stringp probeName=cur->name;
					int probeKey=cur->key;
					if (!probeKey)
					{
						// found the hole.
						return;
					}

					if (probeKey == key )
					{
						// it's the one we're looking for, just update the value.
						break;
					}
					i = (i + (n++)) & bitmask;			// quadratic probe
					cur = quadbase + i;
				}
			}

			cur->key=-1;
			WBATOM(gc,quadbase,&cur->chain,nullObjectAtom);
			//size--;

		}
		
		bool put(int key,ArrayObject*chain)
		{
			bool bGrow=false;
			if(isFull()) {grow();bGrow=true;}
			//AvmAssert(!isFull());			
			GC* gc = GC::GetGC(m_quads);

			QUAD* cur;
			QUAD* const quadbase = m_quads;
			{
				int n = 7;
				int const bitmask = (numQuads - 1);
				unsigned i = ((0x7FFFFFF8 & (uintptr)key) >> 3) & bitmask;
				cur = quadbase + i;
				for (;;)
				{
					int probeName=cur->key;
					if (!probeName||probeName==-1)
					{
						// found the hole.
						break;
					}

					if (probeName == key )
					{
						// it's the one we're looking for, just update the value.						
							goto write_value;
					}
					i = (i + (n++)) & bitmask;			// quadratic probe
					cur = quadbase + i;
				}
			}
			
			// New table entry for this <name,ns> pair
			//if(newValue)
			{
				if(!cur->key)
					size++;
				cur->key=key;
				//WBRC(gc,quadbase,&cur->owner,owner);
				//cur->owner=owner;
				//cur->nRefs=1;
				//quads[i].name = name;
			}
			//else return;
	write_value:
			WBATOM(gc,quadbase,&cur->chain,chain->atom());
			//quads[i].value = value;
			//cur->value=value;
			//WB(gc, quadbase, &cur->value, value);
			//WBRC(gc, quadbase, cur->pObj, newValue);
			return bGrow;
		}
		
	private:
		QUAD* m_quads;		
	public:
		int			  numQuads;
		int			  size;
	};
#endif
class EventHashTable : public MMgc::GCObject
	{
		struct QUAD
		{
			ScriptObject*owner;
			Stringp		name;
			Binding		value;
		};
	public:
		EventHashTable(int capacity) : 
		m_quads(NULL),
		size(0),
		numQuads(0)
		{
			Init(capacity);
		}
		void Init(int capacity)
		{
			if (capacity)
			{
				numQuads = MathUtils::nextPowerOfTwo(capacity);

				MMgc::GC* gc = MMgc::GC::GetGC(this);

				AvmAssert(numQuads > 0);
				MMGC_MEM_TYPE(this);
				QUAD* newAtoms = (QUAD*) gc->Alloc (sizeof(QUAD) * numQuads, GC::kContainsPointers|GC::kZero);
				WB(gc, this, &m_quads, newAtoms);
			}
		}
	protected:
		inline void grow()
		{
			int capacity = numQuads*2;
			MMgc::GC* gc = MMgc::GC::GetGC(this);
			MMGC_MEM_TYPE(this);
			QUAD* newAtoms = (QUAD*) gc->Calloc(capacity, sizeof(QUAD), GC::kContainsPointers|GC::kZero);
			rehash(m_quads, numQuads, newAtoms, capacity);
			gc->Free(m_quads);
			WB(gc, this, &m_quads, newAtoms);
			numQuads = capacity;
		}
		inline int find(Stringp name,ScriptObject*owner,const QUAD* t, unsigned m)const
		{

			// this is a quadratic probe but we only hit every third slot since those hold keys.
			int n = 7;

			int bitmask = (m - 1);

			// Note: Mask off MSB to avoid negative indices.  Mask off bottom
			// 3 bits because it doesn't contribute to hash.  Quad it
			// because names, namespaces, and values are stored adjacently.
			unsigned i = ((0x7FFFFFF8 & (uintptr)name) >> 3) & bitmask;

			Stringp k;
			while (((k=t[i].name) != name || t[i].owner!=owner) && k != NULL)
			{
				
				i = (i + (n++)) & bitmask;			// quadratic probe
			}
			return i;
		}
		inline void rehash(QUAD* oldAtoms, int oldTriplet, QUAD* newAtoms, int newTriplet)
		{
			size=0;
			for (int i=0, n=oldTriplet; i < n; i++)
			{
				//if(oldAtoms[i].pObj)
				//{
				//Stringp oldName;
				Stringp oldName;
				if ((oldName=oldAtoms[i].name) != NULL&&oldAtoms[i].value)
				{
					// inlined & simplified version of put()
					size++;
					int j = find(oldName,oldAtoms[i].owner, newAtoms, newTriplet);
					// don't need WBRC/WB here because we are just moving pointers
					newAtoms[j].name = oldName;
					newAtoms[j].owner= oldAtoms[i].owner;
					newAtoms[j].value= oldAtoms[i].value;
				}
				//newAtoms[i]=oldAtoms[i];
				//}
			}
		}
		inline bool isFull() const
		{ 
			// 0.80 load factor
			return 5*(size+1) >= numQuads*4; 
		}
	public:
		inline Binding valueAt(int index)
		{
			return m_quads[index].value;
		}
		inline ScriptObject* ownerAt(int index)
		{
			return m_quads[index].owner;
		}
		inline Stringp nameAt(int index)
		{
			return m_quads[index].name;
		}
		inline Binding get(Stringp name, ScriptObject*owner) const
		{
			const QUAD* t = m_quads;
			int i = find(name, owner, t, numQuads);
			if (t[i].name == name)
			{
				const QUAD& tf = t[i];
				AvmAssert(tf.owner==owner);
				return tf.value;
			}
			return BIND_NONE;
		}
		int clear(ScriptObject*owner)
		{
			GC* gc = GC::GetGC(m_quads);
			QUAD* cur;
			int iCount=0;
			QUAD* const quadbase = m_quads;
			for(int i=0;i<numQuads;i++)
			{
				cur=quadbase+i;
				if(!cur->name||cur->owner!=owner) continue;
				size--;
				//WBRC(gc,quadbase,&cur->owner,NULL);
				cur->value=NULL;
				cur->owner=NULL;
				//WBRC(gc,quadbase,&cur->name,NULL);
				//WB(gc,quadbase,&cur->value,0);
			}
			return iCount;
		}
		void remove(Stringp name,ScriptObject* owner)
		{
			//AvmAssert(!isFull());			
			GC* gc = GC::GetGC(m_quads);

			QUAD* cur;
			QUAD* const quadbase = m_quads;
			{
				int n = 7;
				int const bitmask = (numQuads - 1);
				unsigned i = ((0x7FFFFFF8 & (uintptr)name) >> 3) & bitmask;
				cur = quadbase + i;
				for (;;)
				{
					Stringp probeName=cur->name;
					if (!probeName)
					{
						// found the hole.
						return;
					}

					if (probeName == name&&cur->owner==owner)
					{
						// it's the one we're looking for, just update the value.
						break;
					}
					i = (i + (n++)) & bitmask;			// quadratic probe
					cur = quadbase + i;
				}
			}

						//else return;
			//WBRC(gc,quadbase,&cur->owner,NULL);
			cur->owner=0;
			//size--;
			//WBRC(gc,quadbase,&cur->name,NULL);
			cur->value=0;
			//WB(gc,quadbase,&cur->value,0);

			//WBRC(gc, quadbase, cur->pObj, newValue);
		}
		bool put(Stringp name,ScriptObject* owner,Binding value)
		{
			bool bGrow=false;
			if(isFull()) {grow();bGrow=true;}
			//AvmAssert(!isFull());			
			GC* gc = GC::GetGC(m_quads);

			QUAD* cur;
			QUAD* const quadbase = m_quads;
			{
				int n = 7;
				int const bitmask = (numQuads - 1);
				unsigned i = ((0x7FFFFFF8 & (uintptr)name) >> 3) & bitmask;
				cur = quadbase + i;
				for (;;)
				{
					Stringp probeName=cur->name;
					if (!probeName||!cur->value)
					{
						// found the hole.
						break;
					}


					if (probeName == name )
					{
						// it's the one we're looking for, just update the value.
						if(cur->owner==owner)
							goto write_value;
					}
					i = (i + (n++)) & bitmask;			// quadratic probe
					cur = quadbase + i;
				}
			}
			
			// New table entry for this <name,ns> pair
			//if(newValue)
			{
				if(cur->name==NULL)
					size++;
				WBRC(gc,quadbase,&cur->name,name);
				//WBRC(gc,quadbase,&cur->owner,owner);
				
				//cur->nRefs=1;
				//quads[i].name = name;
			}
			//else return;
	write_value:
			//quads[i].value = value;
			cur->owner=owner;
			cur->value=value;
			//WB(gc, quadbase, &cur->value, value);
			//WBRC(gc, quadbase, cur->pObj, newValue);
			return bGrow;
		}
		
	private:
		QUAD* m_quads;
	public:
		int			  numQuads;
		int			  size;
	};

class RCHashTable : public MMgc::GCObject
	{
		struct QUAD
		{
			ScriptObject*pObj;
			int			 nRefs;
		};
	public:
		int indexOf(ScriptObject*value)
		{
			int i=find(value,this->m_quads,this->numQuads);
			if(m_quads[i].pObj==value) return i;
			return -1;
		}
		ScriptObject* m_pDump;
		//DRCWB(ScriptObject*) m_pRCDump;
	public:
		inline ScriptObject* valueAt(int i)
		{	return m_quads[i].pObj;	}
		RCHashTable(ScriptObject*pDump,int capacity,bool bRef) : 
		m_quads(NULL),
		size(0),
		numQuads(0)
		{
			bRefrence=bRef;
			m_pDump=pDump;
			Init(capacity);
			
		}
		void Init(int capacity)
		{
			if (capacity)
			{
				numQuads = MathUtils::nextPowerOfTwo(capacity);

				MMgc::GC* gc = MMgc::GC::GetGC(this);

				AvmAssert(numQuads > 0);
				MMGC_MEM_TYPE(this);
				QUAD* newAtoms = (QUAD*) gc->Alloc (sizeof(QUAD) * numQuads, GC::kContainsPointers|GC::kZero);
				WB(gc, this, &m_quads, newAtoms);
			}
		}
	protected:
		inline void grow()
		{
			int capacity = numQuads*2;
			MMgc::GC* gc = MMgc::GC::GetGC(this);
			MMGC_MEM_TYPE(this);
			QUAD* newAtoms = (QUAD*) gc->Calloc(capacity, sizeof(QUAD), GC::kContainsPointers|GC::kZero);
			rehash(m_quads, numQuads, newAtoms, capacity);
			gc->Free(m_quads);
			WB(gc, this, &m_quads, newAtoms);
			numQuads = capacity;
		}
		
		inline int find(ScriptObject*obj, const QUAD* t, unsigned m)
		{

			// this is a quadratic probe but we only hit every third slot since those hold keys.
			int n = 7;

			int bitmask = (m - 1);

			// Note: Mask off MSB to avoid negative indices.  Mask off bottom
			// 3 bits because it doesn't contribute to hash.  Quad it
			// because names, namespaces, and values are stored adjacently.
			unsigned i = ((0x7FFFFFF8 & (uintptr)obj) >> 3) & bitmask;

			ScriptObject* k;
			while (((k=t[i].pObj) != obj ) && k != NULL)
			{
				
				i = (i + (n++)) & bitmask;			// quadratic probe
			}
			return i;
		}
		inline void rehash(QUAD* oldAtoms, int oldTriplet, QUAD* newAtoms, int newTriplet)
		{
			size=0;
			for (int i=0, n=oldTriplet; i < n; i++)
			{
				//if(oldAtoms[i].pObj)
				//{
				//Stringp oldName;
				ScriptObject*oldObj;
				if ((oldObj=oldAtoms[i].pObj) != NULL && oldObj!=m_pDump)
				{
					// inlined & simplified version of put()
					size++;
					int j = find(oldObj, newAtoms, newTriplet);
					// don't need WBRC/WB here because we are just moving pointers
					newAtoms[j].pObj = oldObj;
					newAtoms[j].nRefs = oldAtoms[i].nRefs;
				}
				//newAtoms[i]=oldAtoms[i];
				//}
			}
		}
		inline bool isFull() const
		{ 
			// 0.80 load factor
			return 5*(size+1) >= numQuads*4; 
		}
	public:
		//inline ScriptObject* valueAt(int i){return m_quads[i].pObj;}
		void clear()
		{
			if(!bRefrence)
			{
				VMPI_memset(m_quads,0,numQuads*sizeof(QUAD));
			}
			else
			{
				GC* gc = GC::GetGC(m_quads);
				QUAD* const quadbase = m_quads;
				QUAD* cur;
				for(int i=0;i<numQuads;i++)
				{
					cur=quadbase+i;
					if(!cur->pObj) continue;
					WBRC(gc,quadbase,&cur->pObj,NULL);
					cur->nRefs=0;
				}
			}
		}
		
		int put(ScriptObject* value)
		{
			//if(value==_DebugWindow)
			//	int v=0;
			bool bGrow=false;
			int index=-1;
			if(isFull()) {grow();bGrow=true;}
			//AvmAssert(!isFull());			
			GC* gc = GC::GetGC(m_quads);

			QUAD* cur;
			QUAD* const quadbase = m_quads;
			{
				int n = 7;
				int const bitmask = (numQuads - 1);
				unsigned i = ((0x7FFFFFF8 & (uintptr)value) >> 3) & bitmask;
				cur = quadbase + i;
				for (;;)
				{
					ScriptObject*probeObject=cur->pObj;					
					if (!probeObject||probeObject==m_pDump)
					{
						// found the hole.
						index=i;
						break;
					}

					if (probeObject == value)
					{
						// it's the one we're looking for, just update the value.
						goto write_value;
					}
					i = (i + (n++)) & bitmask;			// quadratic probe
					index=i;
					cur = quadbase + i;
				}
			}
			
			// New table entry for this <name,ns> pair
			//if(newValue)
			{
				if(cur->pObj!=m_pDump)
					size++;
				if(bRefrence)
					WBRC(gc,quadbase,&cur->pObj,value);
				else
					cur->pObj=value;
				cur->nRefs=0;
				//cur->nRefs=1;
				//quads[i].name = name;
			}
			//else return;
	write_value:
			//quads[i].value = value;
			cur->nRefs++;
			return index;
			//WBRC(gc, quadbase, cur->pObj, newValue);
		}
		int GetRefIndex(ScriptObject*value)
		{
			int id=this->find(value,this->m_quads,this->numQuads);
			if(m_quads[id].pObj!=value) return -1;
			return id;
		}
		void Clear(int i)
		{
			GC* gc = GC::GetGC(m_quads);
			QUAD* const quadbase = m_quads;
			QUAD*cur=quadbase+i;
			cur->nRefs=0;
			if(bRefrence)
				WBRC(gc,quadbase,&cur->pObj,m_pDump);
			else
				cur->pObj=m_pDump;
			
		}
		void remove(ScriptObject* value,bool bForce=false)
		{
			//AvmAssert(!isFull());			
			GC* gc = GC::GetGC(m_quads);

			QUAD* cur;
			QUAD* const quadbase = m_quads;
			{
				int n = 7;
				int const bitmask = (numQuads - 1);
				unsigned i = ((0x7FFFFFF8 & (uintptr)value) >> 3) & bitmask;
				cur = quadbase + i;
				for (;;)
				{
					ScriptObject*probeObject=cur->pObj;					
					if (!probeObject)
					{
						// found the hole.
						return;
					}

					if (probeObject == value)
					{
						// it's the one we're looking for, just update the value.
						break;
					}
					i = (i + (n++)) & bitmask;			// quadratic probe
					cur = quadbase + i;
				}
			}
						//else return;
			if(bForce)
			{
				cur->nRefs=0;
				if(bRefrence)
						WBRC(gc,quadbase,&cur->pObj,m_pDump);
				else
						cur->pObj=m_pDump;
			}
			else
			{
				AvmAssert(cur->nRefs);
				cur->nRefs--;
				if(!cur->nRefs)
				{
					if(bRefrence)
						WBRC(gc,quadbase,&cur->pObj,m_pDump);
					else
						cur->pObj=m_pDump;
				}
			}
			//WBRC(gc, quadbase, cur->pObj, newValue);
		}
	private:
		QUAD* m_quads;
	public:
		int			  numQuads;
		int			  size;
		bool		  bRefrence;
	};

}
#endif