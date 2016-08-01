#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3WaveStream.h"
#include "AS3Microphone.h"
#include "avm2.h"
#include "XMicrophone.h"
#include "XAcc.h"

#define MINDIVSIZE 192
#define SCALEONE 1024
namespace avmshell{
BXWaveStreamClass::BXWaveStreamClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BXWaveStreamObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BXWaveStreamClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BXWaveStreamObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

//#undef _BACKLAYER
void BXWaveStreamObject::Release()
{
	if(IsRunning()&&m_pMicrophone!=XNULL)
	{
		m_pMicrophone->UnAttachWaveStream(this);
	}
}

BXWaveStreamObject::BXWaveStreamObject(VTable* _vtable, ScriptObject* _delegate, int capacity):EventDispatcherObject(_vtable, _delegate, 0)
{
	ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
	m_pData=pClass->CreateArray(MAXWAVEBLOCKSIZE);
	//m_pData->GetByteArray().SetEndian(m_pData->GetByteArray().GetNativeEndian());
}

void BXWaveStreamObject::AS3_constructor(MicrophoneObject*mp)
{
	m_pMicrophone=mp;
	if(mp)
		mp->AttachWaveStream(this);
}

bool BXWaveStreamObject::AS3_start()
{
	MicrophoneObject*pm=m_pMicrophone;
	return pm->m_pMicrophone->Start();
}
bool BXWaveStreamObject::AS3_stop()
{
	MicrophoneObject*pm=m_pMicrophone;
	return pm->m_pMicrophone->Stop();
}

MicrophoneObject* BXWaveStreamObject::AS3_getMicrophone()
{
	return m_pMicrophone;
}

void BXWaveStreamObject::AS3_getDCTData(ByteArrayObject*pObj)
{
	if(pObj==XNULL||m_pMicrophone==NULL) return;
	MicrophoneObject*pm=m_pMicrophone;
	DataFile&data=pObj->GetByteArray();
	data.Seek(0);
	int i,l;
	XS32* pData=pm->m_pMicrophone->GetDCTData(l);
	if(pData)
	{
		for(i=0;i<l;i++)		
			data.WriteInt(pData[i]);		
	}
	data.Seek(0);
}

void BXWaveStreamObject::WaveIn(void*pData,int l)
{
	/*BXWaveEventClass*pClass=(BXWaveEventClass*)((ShellToplevel*)toplevel())->getBuiltinExtensionClass(abcclass_BWLoader_BXWaveEvent,false);
	if(pClass==NULL) return;
	if(!hasEvent(pClass->getSlotWAVE_IN(),bBubbles)) return;
	//EventObject*pEvent=((ShellToplevel*)toplevel())->getEventClass()->CreateEventObject(nType);
		
	MicrophoneObject*pm=m_pMicrophone;
	int nbits=pm->m_pMicrophone->GetBitsPerSample();
	int nc=pm->m_pMicrophone->GetChannels();
	DataFile&data=m_pData->GetByteArray();
	data.Seek(0);
	if(nbits==8)
	{
		XS8* p=(XS8*)pData;
		XS8* pEnd=((XS8*)pData)+l;
		while(p<pEnd)
		{
			data.WriteShort(p[0]<<8);
			p++;
			if(nc==2)
			{
				data.WriteShort(p[0]<<8);
				p++;
			}
		}
	}
	else
	{
		XS16* p=(XS16*)pData;
		XS16* pEnd=(XS16)(((XS8*)pData)+l);
		while(p<pEnd)
		{
			data.WriteShort(p[0]);
			p++;
			if(nc==2)
			{
				data.WriteShort(p[0]);
				p++;
			}
		}
	}
	data.Seek(0);
	
	BXWaveEventObject*pEvent=pClass->createEvent(m_pData,pm->m_pMicrophone->GetChannels);
	pEvent->bBubbles=false;
	if(bLater)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		//if(!ShellCore::IsTypeClass(pTarget?pTarget:this,top->getDisplayObjectClass()))
		//	int v=0;
		return ((ShellCore*)core())->GetPlayer()->OnAS3Event(this,pEvent,pTarget?pTarget:this);
	}
	else
	{
		pEvent->pTarget=pTarget?pTarget:this;
		return DispatchEvent(pEvent);
	}*/
	//m_pData->GetByteArray().GetBuffer();
}

///////////////////////////////////////////////////////////////////////////////////////////////
BXWaveEventClass::BXWaveEventClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BXWaveEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};

BXWaveEventObject* BXWaveEventClass::createEvent(ByteArrayObject*obj,int channels)
{
	BXWaveEventObject*pObj=(BXWaveEventObject*)createInstance(ivtable(),prototype);
	pObj->AS3_constructor(this->getSlotWAVE_IN(),false,false);
	pObj->nEventID=-1;
	pObj->m_nChannels=channels;
	pObj->m_pData=obj;
	return pObj;
}

ScriptObject* BXWaveEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BXWaveEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

//#undef _BACKLAYER

BXWaveEventObject::BXWaveEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity):EventObject(vtable, _delegate, 0)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////
BXAccClass::BXAccClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BXAccObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BXAccClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BXAccObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

//#undef _BACKLAYER

BXAccObject::BXAccObject(VTable* _vtable, ScriptObject* _delegate, int capacity):ScriptObject(vtable, _delegate, 0)
{

}

double BXAccClass::AS3_getX()
{
	return XAcc::getAccX();
}

double BXAccClass::AS3_getY()
{
	return XAcc::getAccY();
}

double BXAccClass::AS3_getZ()
{
	return XAcc::getAccZ();
}

Atom BXAccClass::AS3_aObject_get()
{
	ScriptObject*pObj=toplevel()->objectClass->newInstance();
	//pObj->setStringProperty(
	double x,y,z;
	XAcc::getAAcc(x,y,z);
	AvmCore* c=core();
	pObj->setStringProperty(c->newConstantStringLatin1("x"),c->doubleToAtom(x));
	pObj->setStringProperty(c->newConstantStringLatin1("y"),c->doubleToAtom(y));
	pObj->setStringProperty(c->newConstantStringLatin1("z"),c->doubleToAtom(z));
	return pObj->atom();
}
Atom BXAccClass::AS3_gObject_get()
{
	ScriptObject*pObj=toplevel()->objectClass->newInstance();
	double x,y,z;
	XAcc::getGAcc(x,y,z);
	AvmCore* c=core();
	pObj->setStringProperty(c->newConstantStringLatin1("x"),c->doubleToAtom(x));
	pObj->setStringProperty(c->newConstantStringLatin1("y"),c->doubleToAtom(y));
	pObj->setStringProperty(c->newConstantStringLatin1("z"),c->doubleToAtom(z));
	return pObj->atom();
}

bool BXAccClass::AS3_init()
{
	return XAcc::Init();
}

}
