#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"

using namespace avmplus;
#include "AS3GroupElement.h"
#include "AS3TextLine.h"

namespace avmshell{
GroupElementClass::GroupElementClass(VTable* cvtable):ClassClosure(cvtable)//ContentElementClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GroupElementClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GroupElementObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GroupElementObject::GroupElementObject(VTable *vtable, ScriptObject* proto, int capacity): ContentElementObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_nContentType=TYPE_GROUP;
}



GroupElementObject* GroupElementClass::CreateObject(ObjectVectorObject*pVec,GroupElementObject*oldObj)
{
	GroupElementObject*pObj=(GroupElementObject*)createInstance(ivtable(),prototype);
	pObj->AS3_setElements(pVec);
	if(oldObj)
	{
		if(oldObj->m_pElementFormat)
			pObj->AS3_elementFormat_set(oldObj->m_pElementFormat);
		if(oldObj->m_pMirrorEvent)
			//pObj->AS3_
			pObj->m_pMirrorEvent=oldObj->m_pMirrorEvent;
		if(oldObj->m_strRotation)
			pObj->AS3_textRotation_set(oldObj->m_strRotation);
	}
	//Atom atoms[]={pEClass->atom(),
	//			  pObj->atom(),
	//			  m_pElementFormat?this->m_pElementFormat->atom():nullObjectAtom,
	//			  this->m_pMirrorEvent?m_pMirrorEvent->atom():nullObjectAtom,
	//			  this->m_strRotation?m_strRotation->atom():nullStringAtom};
	return pObj;
}
//////////////////////////////////////////////////////////
//Native Method start...
int GroupElementObject::AS3_elementCount_get()
{
	//Add your act code here...
	if(m_pElements==NULL)
		return 0;
	return m_pElements->get_length();//>get_length();//Modify this please!
}

//AS3 contructor function..
//AvmBox GroupElementObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

bool GroupElementObject::CreateContext(TextLineObject*pLine,int nID)
{
	if(!m_pElements||m_pElements->get_length()==0)
	{
		ContentElementObject*next=GetNextElement();
		if(!next)
		{
			pLine->m_pElement=NULL;
			return false;
		}
		return next->CreateContext(pLine,nID);
	}
	Atom atom=m_pElements->getUintProperty(0);
	if(AvmCore::isNullOrUndefined(atom))
	{
		ContentElementObject*next=GetNextElement();
		if(!next) 
		{
			pLine->m_pElement=NULL;
			return false;
		}
		next->CreateContext(pLine,nID);
	}
	ContentElementObject*obj=(ContentElementObject*)AvmCore::atomToScriptObject(atom);
	return obj->CreateContext(pLine,nID);
}

ContentElementObject* GroupElementObject::GetNext(ContentElementObject*obj)
{
	if(m_pElements==NULL)
		return this->GetNextElement();
	int i,nc=m_pElements->get_length();
	Atom findAtom=obj->atom();
	for(i=0;i<nc-1;i++)
	{
		Atom atom=m_pElements->getUintProperty(i);
		if(atom==findAtom)
		{
			atom=m_pElements->getUintProperty(i+1);
			if(!AvmCore::isNullOrUndefined(atom))
			{
				ContentElementObject*nextObj=(ContentElementObject*)AvmCore::atomToScriptObject(atom);
				return nextObj;
			}
		}
	}
	return this->GetNextElement();
}

void GroupElementObject::ReleaseCreationData()
{
	ContentElementObject::ReleaseCreationData();
	if(m_pElements)
	{
		int i,nc=m_pElements->get_length();
		for(i=0;i<nc;i++)
		{
			Atom atom=m_pElements->getUintProperty(i);
			if(AvmCore::isNullOrUndefined(atom)) continue;
			ContentElementObject*pObj=(ContentElementObject*)AvmCore::atomToScriptObject(atom);
			pObj->ReleaseCreationData();
		}
	}
}


ContentElementObject* GroupElementObject::AS3_getElementAt(int index)
{
	//Add your act code here...
	if(m_pElements==NULL)
		return NULL;
	if(m_pElements==NULL||index<0||index>=m_pElements->get_length())
		toplevel()->throwRangeError(kInvalidRangeError,INTString(index),INTString(m_pElements?m_pElements->get_length():0));
	if(m_pElements)
	{
		Atom atom=m_pElements->getUintProperty(index);
		if(AvmCore::isNullOrUndefined(atom)) return NULL;
		return (ContentElementObject*)AvmCore::atomToScriptObject(atom);
	}
	return NULL;//Modify this please!
}

ContentElementObject* GroupElementObject::AS3_getElementAtCharIndex(int charIndex)
{
	//Add your act code here...
	if(m_strRawText==NULL||charIndex<0||charIndex>=m_strRawText->get_length())
		toplevel()->throwRangeError(kInvalidRangeError,INTString(charIndex),INTString(m_strRawText?m_strRawText->get_length():0));
	if(m_pElements)
	{
		int i,nc=m_pElements->get_length();
		int nTotal=0;
		for(i=0;i<nc;i++)
		{
			Atom atom=m_pElements->getUintProperty(i);
			if(!AvmCore::isNullOrUndefined(atom))
			{
				ContentElementObject*pObj=(ContentElementObject*)AvmCore::atomToScriptObject(atom);
				nTotal+=pObj->m_strRawText->get_length();
				if(charIndex<nTotal)
					return pObj;
			}
		}
	}
	return NULL;//Modify this please!
}

//AvmBox GroupElementObject::AS3_getElementIndex(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

GroupElementObject* GroupElementObject::AS3_groupElements(int beginIndex, int endIndex)
{
	//Add your act code here...
	if(m_pElements==NULL)
		return NULL;
	if(m_pElements==NULL||beginIndex<0||endIndex>m_pElements->get_length()||endIndex<beginIndex)
		toplevel()->throwRangeError(kInvalidRangeError,INTString(beginIndex),INTString(endIndex));	
	ShellToplevel*top=(ShellToplevel*)toplevel();
	ContentElementClass*pClass=top->getContentElementClass();
	int i,nCount=endIndex-beginIndex;//,newCount=m_pElements->get_length();
	if(nCount<0) nCount=0;
	ObjectVectorObject*pObj=top->vectorClass->newVector(pClass,nCount);
	for(i=0;i<nCount;i++)
	{
		Atom atom=m_pElements->getUintProperty(i+beginIndex);
		pObj->setUintProperty(i,atom);
		//m_pElements->
	}
	//m_pElements->deleteAtomProperty(
	
	GroupElementClass*pEClass=top->getGroupElementClass();	
	GroupElementObject*pNew=pEClass->CreateObject(pObj,this);
	pNew->m_pGroup=this;
	int nNewCount=m_pElements->get_length()+1-nCount;

	ObjectVectorObject*pNewVec=top->vectorClass->newVector(pClass,nNewCount);
	for(i=0;i<beginIndex;i++)
		pNewVec->setUintProperty(i,m_pElements->getUintProperty(i));
	pNewVec->setUintProperty(beginIndex,pNew->atom());
	for(i=beginIndex+nCount;i<m_pElements->get_length();i++)
	{
		pNewVec->setUintProperty(i-nCount+1,m_pElements->getUintProperty(i));
	}
	m_pElements=pNewVec;
	MakeRowText();

	return pNew;
	//return pObj;
	//return NULL;//Modify this please!
}

TextElementObject* GroupElementObject::AS3_mergeTextElements(int beginIndex, int endIndex)
{
	//Add your act code here...
	if(m_pElements==NULL)
		return NULL;
	if(m_pElements==NULL||beginIndex<0||endIndex>m_pElements->get_length()||endIndex<beginIndex)
		toplevel()->throwRangeError(kInvalidRangeError,INTString(beginIndex),INTString(endIndex));
	TextElementObject*pBegin=NULL;
	
	int i,nDecCount=0;
	for(i=beginIndex;i<endIndex;i++)
	{
		Atom atom=m_pElements->getUintProperty(i);
		if(AvmCore::isNullOrUndefined(atom)) continue;
		GroupElementObject*pObj=(GroupElementObject*)AvmCore::atomToScriptObject(atom);
		if(pObj->m_nContentType!=GroupElementObject::TYPE_TEXT) continue;
		TextElementObject*pTxt=(TextElementObject*)pObj;
		if(pBegin==NULL)
			pBegin=pTxt;
		else
		{
			nDecCount++;
			pBegin->CombineText(pTxt);
			pTxt->m_pGroup=NULL;
		}
	}
	if(nDecCount)
	{
		int nNewCount=m_pElements->get_length()-nDecCount;
		ShellToplevel*top=(ShellToplevel*)toplevel();
		ContentElementClass*pClass=top->getContentElementClass();
		ObjectVectorObject*pNewVec=top->vectorClass->newVector(pClass,nNewCount);
		int iIndex=0;
		for(i=0;i<beginIndex;i++)
		{
			pNewVec->setUintProperty(iIndex,m_pElements->getUintProperty(i));
			iIndex++;
		}
		pBegin=NULL;
		for(i=beginIndex;i<endIndex;i++)
		{
			Atom atom=m_pElements->getUintProperty(i);
			if(AvmCore::isNullOrUndefined(atom)) continue;
			GroupElementObject*pObj=(GroupElementObject*)AvmCore::atomToScriptObject(atom);
			if(pObj->m_nContentType!=GroupElementObject::TYPE_TEXT) 
			{
				pNewVec->setUintProperty(iIndex,atom);
				iIndex++;
			}
			else if(pBegin==NULL)
			{
				pNewVec->setUintProperty(iIndex,atom);
				pBegin=(TextElementObject*)pObj;
				iIndex++;
			}
		}
		for(i=endIndex;i<m_pElements->get_length();i++)
		{
			pNewVec->setUintProperty(iIndex,m_pElements->getUintProperty(i));
			iIndex++;
		}
		m_pElements=pNewVec;
		MakeRowText();
	}

	return pBegin;//Modify this please!
}

ObjectVectorObject* GroupElementObject::AS3_replaceElements(int beginIndex, int endIndex, ObjectVectorObject *pNewElements)
{
	//Add your act code here...

	//if(beginIndex==endIndex&&(beginIndex==4||beginIndex==5||beginIndex==6))
	//	int v=0;

	if(m_pElements==NULL) 
	{
		if(pNewElements==NULL)
			return NULL;
		m_pElements=pNewElements;
		SetItemGroup(this);
		MakeRowText();
		return m_pElements;
	}

	if(m_pElements==NULL||beginIndex<0||endIndex>m_pElements->get_length()||endIndex<beginIndex)
		toplevel()->throwRangeError(kInvalidRangeError,INTString(beginIndex),INTString(endIndex));
	int nNewCount=m_pElements->get_length()-(endIndex-beginIndex);
	if(pNewElements)
		nNewCount+=pNewElements->get_length();
	ShellToplevel*top=(ShellToplevel*)toplevel();
	ContentElementClass*pClass=top->getContentElementClass();
	ObjectVectorObject*pDelVec=top->vectorClass->newVector(pClass,endIndex-beginIndex);

	if(beginIndex!=endIndex||
	   (pNewElements&&pNewElements->get_length()))
	{
		ObjectVectorObject*pNewVec=top->vectorClass->newVector(pClass,nNewCount);
		int i,iIndex=0;
		for(i=0;i<beginIndex;i++)
		{
			pNewVec->setUintProperty(iIndex,m_pElements->getUintProperty(i));
			iIndex++;
		}
		for(i=beginIndex;i<endIndex;i++)
		{
			Atom atom=m_pElements->getUintProperty(i);
			if(!AvmCore::isNullOrUndefined(atom))
			{
				ContentElementObject*pObj=(ContentElementObject*)AvmCore::atomToScriptObject(atom);
				pObj->m_pGroup=NULL;
			}
			pDelVec->setUintProperty(i-beginIndex,atom);
		}
		if(pNewElements)
		{
			for(i=0;i<pNewElements->get_length();i++)
			{
				pNewVec->setUintProperty(iIndex,pNewElements->getUintProperty(i));
				iIndex++;
			}
		}
		for(i=endIndex;i<m_pElements->get_length();i++)
		{
			pNewVec->setUintProperty(iIndex,m_pElements->getUintProperty(i));
			iIndex++;
		}
		m_pElements=pNewVec;
		SetItemGroup(this);
		MakeRowText();
	}

	return pDelVec;//Modify this please!
}

void GroupElementObject::AS3_setElements(ObjectVectorObject *pValue)
{
	//Add your act code here...
	if(m_pElements)
		SetItemGroup(NULL);
	//if(pValue&&pValue->get_length()==7)
	//	int v=0;
	m_pElements=pValue;
	SetItemGroup(this);
	MakeRowText();
}

TextElementObject* GroupElementObject::AS3_splitTextElement(int elementIndex, int splitIndex)
{
	//Add your act code here...
	if(m_pElements==NULL)
		return NULL;
	if(m_pElements==NULL||elementIndex<0||elementIndex>=m_pElements->get_length())
		toplevel()->throwRangeError(kInvalidRangeError,INTString(elementIndex),INTString(splitIndex));
	Atom atom=m_pElements->getUintProperty(elementIndex);
	if(AvmCore::isNullOrUndefined(atom))
		toplevel()->throwArgumentError(kInvalidArgumentError,"Object(null)");
	ContentElementObject*pObj=(ContentElementObject*)AvmCore::atomToScriptObject(atom);
	if(pObj->m_nContentType!=ContentElementObject::TYPE_TEXT)
		toplevel()->throwArgumentError(kInvalidArgumentError,"NOT TextElement");
	TextElementObject*pTxt=(TextElementObject*)pObj;
	TextElementObject*pNew=pTxt->Split(splitIndex);
	pNew->m_pGroup=this;
	int nCount=m_pElements->get_length();
	m_pElements->set_length(nCount+1);
	int i;
	for(i=nCount;i>elementIndex+1;i--)
		m_pElements->setUintProperty(i,m_pElements->getUintProperty(i-1));
	m_pElements->setUintProperty(elementIndex+1,pNew->atom());
	return pNew;//Modify this please!
}

void GroupElementObject::AS3_ungroupElements(int groupIndex)
{
	//Add your act code here...
	if(m_pElements==NULL)
		return;
	if(m_pElements==NULL||groupIndex<0||groupIndex>=m_pElements->get_length())
		toplevel()->throwRangeError(kInvalidRangeError,INTString(groupIndex),INTString(groupIndex));
	Atom atom=m_pElements->getUintProperty(groupIndex);
	if(AvmCore::isNullOrUndefined(atom))
		toplevel()->throwArgumentError(kInvalidArgumentError,"Object(null)");
	ContentElementObject*pObj=(ContentElementObject*)AvmCore::atomToScriptObject(atom);
	if(pObj->m_nContentType!=ContentElementObject::TYPE_GROUP)
		toplevel()->throwArgumentError(kInvalidArgumentError,"NOT GroupElement");
	GroupElementObject*pGroup=(GroupElementObject*)pObj;
	pGroup->m_pGroup=NULL;
	//if(pGroup->m_pElements&&pGroup->m_pElements->get_length())
	ShellToplevel*top=(ShellToplevel*)toplevel();
	ContentElementClass*pClass=top->getContentElementClass();
	if(pGroup->m_pElements&&pGroup->m_pElements->get_length()==1)
	{
		m_pElements->setUintProperty(groupIndex,pGroup->m_pElements->getUintProperty(0));
		pGroup->m_pElements=top->vectorClass->newVector(pClass,0);
	}
	else
	{
		
		int i,iIndex=0,nNewCount=m_pElements->get_length()+pGroup->m_pElements->get_length()-1;
		ObjectVectorObject*pNewVec=top->vectorClass->newVector(pClass,nNewCount);
		pGroup->m_pElements=top->vectorClass->newVector(pClass,0);
		for(i=0;i<groupIndex;i++)
		{
			pNewVec->setUintProperty(iIndex,m_pElements->getUintProperty(i));
			iIndex++;
		}
		if(pGroup->m_pElements)
		{
			for(i=0;i<pGroup->m_pElements->get_length();i++)
			{
				pNewVec->setUintProperty(iIndex,pGroup->m_pElements->getUintProperty(i));
				iIndex++;
			}
		}
		for(i=groupIndex+1;i<m_pElements->get_length();i++)
		{
			pNewVec->setUintProperty(iIndex,m_pElements->getUintProperty(i));
			iIndex++;
		}
		m_pElements=pNewVec;
	}
	SetItemGroup(this);
}

void GroupElementObject::SetItemGroup(GroupElementObject*item)
{
	if(m_pElements)
	{
		int i,nc=m_pElements->get_length();
		m_strRawText=NULL;
		for(i=0;i<nc;i++)
		{
			Atom atom=m_pElements->getUintProperty(i);
			if(!AvmCore::isNullOrUndefined(atom))
			{
				ContentElementObject*pObj=(ContentElementObject*)AvmCore::atomToScriptObject(atom);
				pObj->m_pGroup=item;
			}
		}
	}
}

void GroupElementObject::MakeRowText()
{
	//if(m_pElements&&m_pElements->get_length()==7)
	//	int v=0;
	if(m_pElements==NULL||m_pElements->get_length()==0)
		m_strRawText=core()->kEmptyString;
	else
	{
		int i,nc=m_pElements->get_length();
		m_strRawText=NULL;
		for(i=0;i<nc;i++)
		{
			Atom atom=m_pElements->getUintProperty(i);
			if(!AvmCore::isNullOrUndefined(atom))
			{
				ContentElementObject*pObj=(ContentElementObject*)AvmCore::atomToScriptObject(atom);
				if(pObj->m_strRawText==NULL) continue;
				if(m_strRawText==NULL)
					m_strRawText=pObj->m_strRawText;
				else
					m_strRawText=String::concatStrings(m_strRawText,pObj->m_strRawText);
			}
		}
	}
}

}