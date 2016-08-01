#include "StdAfxflash.h"
#include "avmplus.h"
#include "sobject.h"
using namespace avmplus;
#include "AS3GraphicElement.h"
#include "AS3DisplayObjectContainer.h"
#include "AS3TextLine.h"

namespace avmshell{
GraphicElementClass::GraphicElementClass(VTable* cvtable):ClassClosure(cvtable)//ContentElementClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicElementClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicElementObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicElementObject::GraphicElementObject(VTable *vtable, ScriptObject* proto, int capacity): ContentElementObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_nWidth=15;
	m_nHeight=15;
	m_nContentType=TYPE_GRAPHIC;
	const wchar chars[]={65007,0};
	m_strRawText=core()->newStringUTF16(chars,1);
	m_cacheFormat=NULL;
	//const wchar chars[]={65007,0};
	//core()->newStringUTF16(chars,1);
	//m_strRawText=core()->kEmptyString;//core()->newStringUTF16(chars,1);
}

bool GraphicElementObject::CreateContext(TextLineObject*pLine,int nID)
{
	m_cacheLine=NULL;
	m_cacheFormat=NULL;
	if(!m_pGraphic)
	{
		pLine->AppendObject(this);
		ContentElementObject*next=GetNextElement();
		if(next)
			return next->CreateContext(pLine,nID);
		else
		{
			pLine->m_pElement=NULL;
		}
	}
	else
	{
		pLine->m_pElement=this;
		pLine->m_nElementID=0;
		if(pLine->m_nTextWidth+m_nWidth>pLine->m_nMaxWidth)
			return false;
		else
		{
			pLine->AppendObject(this);
			ContentElementObject*next=GetNextElement();
			if(next)
				return next->CreateContext(pLine,0);
			pLine->m_pElement=NULL;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////
//Native Method start...
double GraphicElementObject::AS3_elementHeight_get()
{
	//Add your act code here...
	return m_nHeight;//Modify this please!
}

void GraphicElementObject::AS3_elementHeight_set(double elementHeight)
{
	//Add your act code here...
	m_nHeight=elementHeight;
}

double GraphicElementObject::AS3_elementWidth_get()
{
	//Add your act code here...
	return m_nWidth;//Modify this please!
}

void GraphicElementObject::AS3_elementWidth_set(double elementWidth)
{
	//Add your act code here...
	m_nWidth=elementWidth;
}

DisplayObjectObject* GraphicElementObject::AS3_graphic_get()
{
	//Add your act code here...
	return m_pGraphic;//Modify this please!
}

void GraphicElementObject::AS3_graphic_set(DisplayObjectObject *pGraphic)
{
	//Add your act code here...
	/*if(pGraphic==NULL)
		m_strRawText=core()->kEmptyString;
	else if(m_strRawText->isEmpty()&&pGraphic)
	{
		const wchar chars[]={65007,0};
		core()->newStringUTF16(chars,1);
	}*/
	m_pGraphic=pGraphic;
	if(pGraphic&&pGraphic->m_pObject)
	{
		if(pGraphic->m_pObject->parent)
		{
			if(pGraphic->m_pObject->parent->pASObject)
			{
				DisplayObjectContainerObject*pCon=(DisplayObjectContainerObject*)pGraphic->m_pObject->parent->pASObject;
				pCon->AS3_removeChild(pGraphic);
			}
		}
		if(m_cacheLine)
		{
			m_cacheLine->AppendDisplay(this);
			m_cacheLine=NULL;
			m_cacheFormat=NULL;
		}
	}
}

//AS3 contructor function..
//AvmBox GraphicElementObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}
