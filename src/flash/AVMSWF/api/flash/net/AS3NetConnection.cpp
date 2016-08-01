#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3NetConnection.h"
#include "AS3ByteArray.h"
#include "AS3Responder.h"
#include "sobject.h"
#include "splayer.h"

namespace avmshell{
NetConnectionClass::NetConnectionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	m_nDefaultEncoding=kEncodeDefault;
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())NetConnectionObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* NetConnectionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetConnectionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

void NetConnectionObject::Release()
{
	//m_strHeaders.RemoveAll();
	//m_headers->();
	EventDispatcherObject::Release();
}

NetConnectionObject::NetConnectionObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	ShellToplevel*top=(ShellToplevel*)toplevel();
	ByteArrayClass*pClass=top->getByteArrayClass();
	NetConnectionClass*pNetClass=top->getNetConnectionClass();
	m_headers=top->arrayClass->newArray(0);
	m_inData=pClass->CreateArray(0);
	m_outData=pClass->CreateArray(0);
	m_inData->AS3_objectEncoding_set(kAMF0);//(ObjectEncoding)pNetClass->AS3_defaultObjectEncoding_get());
	m_outData->AS3_objectEncoding_set(kAMF0);//(ObjectEncoding)pNetClass->AS3_defaultObjectEncoding_get());
	m_resHash=new(core()->GetGC())RCHashTable((ScriptObject*)top->m_pDumpObject.value(),4,true);
	m_bConnected=0;
	m_bCalling=false;
	//m_pClient=nullObjectAtom;
	m_strProto=((ShellCore*)core())->kProtoHttp;
	m_pClient=NULL;
	m_pLoaderInfo=NULL;
//#if (__CORE_VERSION__>=0x02080000)
	m_bCalling=false;
//#endif
}
//////////////////////////////////////////////////////////
//Native Method start...
uint32_t NetConnectionClass::AS3_defaultObjectEncoding_get()
{
	return m_nDefaultEncoding;
	//return m_outData->AS3_defaultObjectEncoding_get();
}
void NetConnectionClass::AS3_defaultObjectEncoding_set(uint32_t v)
{
	m_nDefaultEncoding=(ObjectEncoding)v;
	//m_outData->AS3_defaultObjectEncoding_set(v);
	//m_inData->AS3_defaultObjectEncoding_set(v);
}
Atom NetConnectionObject::AS3_client_get()
{
	return m_pEventObject==NULL?nullObjectAtom:m_pEventObject->atom();
	//return m_pClient;
}
void NetConnectionObject::AS3_client_set(Atom atom)
{
	m_pEventObject=AvmCore::atomToScriptObject(atom);
}
bool NetConnectionObject::AS3_connected_get()
{
	return m_bConnected;
}
Stringp NetConnectionObject::AS3_connectedProxyType_get()
{
	return ((ShellCore*)core())->kProtoHttp;
}
uint32_t NetConnectionObject::AS3_objectEncoding_get()
{
	return m_outData->AS3_objectEncoding_get();
}
void NetConnectionObject::AS3_objectEncoding_set(uint32_t v)
{
	m_outData->AS3_objectEncoding_set(v);
	m_inData->AS3_objectEncoding_set(v);
}
Stringp NetConnectionObject::AS3_proxyType_get()
{
	return m_strProto;
}
void NetConnectionObject::AS3_proxyType_set(Stringp strProto)
{
	m_strProto=strProto;
}
Stringp NetConnectionObject::AS3_uri_get()
{
	return m_strURL;
}
bool NetConnectionObject::AS3_usingTLS_get()
{
	return false;
}
void NetConnectionObject::AS3_constructor()
{
}
void NetConnectionObject::AS3_addHeader(Stringp name,bool mustUnderStand,Atom param)//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
{

	int i,iCount=m_headers->getDenseLength();//m_headers->get_length();
	AvmCore* c=core();
	for(i=0;i<iCount;i+=3)
	{
		Stringp strKey=AvmCore::atomToString(m_headers->getIntProperty(i));
		if(strKey->equals(name))
		{
			m_headers->setIntProperty(i+1,mustUnderStand?trueAtom:falseAtom);
			m_headers->setIntProperty(i+2,param);
			return;
		}
	}
	Atom atoms[]={name->atom(),mustUnderStand?trueAtom:falseAtom,param};
	m_headers->push(atoms,3);

	/*if(m_hashTable==NULL)
		m_hashTable=new (core()->GetGC(), 0) HeapHashtable(core()->GetGC());
	if(m_hashTable)
		m_hashTable->add(name->atom(),param?param->atom():nullObjectAtom);*/
	/*Stringp tmp=param?core()->string(param->atom()):(Stringp)core()->kEmptyString;
	StUTF8String strKey(name);
	StUTF8String strValue(tmp);
	int i,iCount=m_strHeaders.GetSize();
	bool bSet=false;
	for(i=0;i<iCount;i+=2)
	{
		XString8&key=m_strHeaders[i];
		XString8&value=m_strHeaders[i+1];
		if(key.Compare(strKey.c_str(),true)==0)
		{
			value.SetString(strValue.c_str(),strValue.length());
			bSet=true;
			break;
		}
	}
	if(!bSet)
	{
		m_strHeaders.Add(strKey.c_str());
		m_strHeaders.Add(strValue.c_str());
	}*/

}

void NetConnectionObject::AS3_close()//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
{
	if(m_pClient)
	{
		((XClient*)m_pClient)->Cancel();
		m_pClient=NULL;
	}
	if(m_pLoaderInfo)
	{
		((ShellCore*)core())->GetPlayer()->ReleaseInfo(m_pLoaderInfo);
		m_pLoaderInfo=NULL;
	}
	this->m_resHash->clear();
	m_bConnected=false;
	m_bCalling=false;
}
void NetConnectionObject::AS3_connect(Stringp cmds,ArrayObject*args)//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
{
	//this->OnIOErrorEvent("not support");
	//ScriptObject*pObj=CreateStatusObject(NC_CONNECT_FAILED);////CreateStatus(toplevel()->objectClass->newInstance();
	//this->OnNetStatusEvent(pObj);
	m_strURL=cmds;
	m_bCalling=false;
	m_bConnected=true;
}

ScriptObject* NetConnectionObject::CreateStatusObject(int nCode)
{
	const char* _strStatus[]={"NetConnection.Call.BadVersion",// "error" �Բ���ʶ��ĸ�ʽ�������ݰ�
							  "NetConnection.Call.Failed",// "error" NetConnection.call �����޷����÷������˵ķ��������
							  "NetConnection.Call.Prohibited",// "error" Action Message Format (AMF) ������ȫԭ�����ֹ�� ������ AMF URL �� SWF ����ͬһ���򣬻����� AMF ������û������ SWF �ļ�����Ĳ����ļ���
							  "NetConnection.Connect.Closed",// "status" �ɹ��ر����ӡ�
							  "NetConnection.Connect.Failed",// "error" ���ӳ���ʧ�ܡ�
							  "NetConnection.Connect.Success",// "status" ���ӳ��Գɹ���
							  "NetConnection.Connect.Rejected",// "error" ���ӳ���û�з���Ӧ�ó����Ȩ�ޡ�
							  "NetConnection.Connect.AppShutdown",// "error" ���ڹر�ָ����Ӧ�ó���
							  "NetConnection.Connect.InvalidApp"};// "error" ����ʱָ����Ӧ�ó�������Ч��

	ScriptObject*pObj=toplevel()->objectClass->newInstance();
	AvmCore*c=core();
	Stringp name=c->internConstantStringLatin1("code");
	Stringp value=c->newConstantStringLatin1(_strStatus[nCode]);
	pObj->setStringProperty(name,value->atom());
	return pObj;
}

Stringp NetConnectionObject::registerResponse(ResponderObject*obj)
{

	Stringp res=core()->cachedChars[(int)'/'];//core()->newStringLatin1("/",1);
	XU32 nRunTime=((ShellCore*)core())->GetPlayer()->m_nRunTime;
	RCObject*dumpObj=((ShellToplevel*)toplevel())->m_pDumpObject.value();
	int i,iCount=m_resHash->numQuads,iSet=0;
	for(i=0;i<iCount;i++)
	{
		ResponderObject* obj=(ResponderObject*)m_resHash->valueAt(i);
		if(obj==NULL||obj==dumpObj) continue;
		if(obj->m_nCreateTime+6000<nRunTime)
		{
			m_resHash->Clear(i);
		}
		else iSet++;
	}
	if(!iSet) m_bCalling=false;
	if(m_bCalling) return NULL;
	if(obj!=NULL)
	{
		obj->m_nCreateTime=nRunTime;
		if(m_resHash->indexOf(obj)>=0) return NULL;
		int id=m_resHash->put(obj);
		Stringp numStr=core()->intToString(id);
		res=res->concatStrings(res,numStr);
	}
	return res;
}
ResponderObject* NetConnectionObject::callbackResponse(Stringp name,bool&isResult)
{
	isResult=true;
	Stringp strSp=core()->cachedChars[(int)'/'];
	int dsp0= name->indexOf(strSp);
	int dsp1= name->lastIndexOf(strSp);
	if((dsp1-dsp0)<=1) return NULL;
	Stringp strNum=name->substr(dsp0+1,dsp1-dsp0-1);
	double id=core()->number(strNum->atom());
	if(id==MathUtils::kNaN) return NULL;

	Stringp strResult=name->substr(dsp1+1);
	if(strResult->equals(((ShellCore*)core())->kOnResult))
		isResult=true;

	ResponderObject*obj=(ResponderObject*)m_resHash->valueAt(id);
	if(!obj||obj==((ShellToplevel*)toplevel())->m_pDumpObject.value()) return NULL;
	m_resHash->remove(obj);
	return obj;
}

void NetConnectionObject::AS3_call(Stringp cmds,ResponderObject*res,ArrayObject* args)//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
{
//#if (__CORE_VERSION__>=0x02080000)
	if(m_bDating)
	{
		m_callCmds=cmds;
		m_callRes=res;
		m_callArgs=args;
		int nType=kHttpLoadTarget|kHttpLoadData;
		//ShellCore*c=(ShellCore*)core();
		ShellToplevel*top=(ShellToplevel*)toplevel();
		top->AddLaterObject(this);
		return;
	}
//#endif
	if(!m_bConnected)
		return;
	Stringp resStr=registerResponse(res);
	if(resStr==NULL)
	{
		if(res&&res->m_pStatusCall)
		{
			Atom atom[]={res->m_pStatusCall->atom(),nullObjectAtom};
			res->m_pStatusCall->call(1,atom);
		}
		return;
	}
	m_bCalling=true;
	m_outData->SetLength(0);
	m_outData->Seek(0);
	m_outData->AS3_writeShort(3);//version
	int nHeaders=m_headers->getDenseLength();//get_length();
	m_outData->AS3_writeShort(nHeaders);//header count
	for(int i=0;i<nHeaders;i+=3)
	{
		Stringp name=AvmCore::atomToString(m_headers->getIntProperty(i));
		bool	bMust=AvmCore::boolean(m_headers->getIntProperty(i+1));
		Atom	value=m_headers->getIntProperty(i+2);
		m_outData->AS3_writeUTF(name);
		m_outData->AS3_writeByte(bMust);
		m_inData->SetLength(0);
		m_inData->Seek(0);
		m_inData->AS3_objectEncoding_set(kAMF0);
		m_inData->AS3_writeObject(value);
		m_outData->AS3_writeUnsignedInt(m_inData->GetLength());
		m_outData->AS3_writeBytes(m_inData,0,m_inData->GetLength());
	}
	m_outData->AS3_writeShort(1);//message count
	if(cmds==NULL)
		m_outData->AS3_writeUTF(core()->knull);
	else
		m_outData->AS3_writeUTF(cmds);//request
	m_outData->AS3_writeUTF(resStr);//responder

	if(args==NULL)
	{
		m_outData->AS3_writeUnsignedInt(5);
		m_outData->AS3_writeByte(0xa);
		m_outData->AS3_writeUnsignedInt(0);
	}
	else
	{
		//m_inData->AS3_objectEncoding_set(kAMF0);
		m_inData->SetLength(0);
		m_inData->Seek(0);
		//ByteArrayObject*pArray=((ShellToplevel*)toplevel())->getByteArrayClass()->CreateArray(0);
		m_inData->AS3_objectEncoding_set(kAMF0);
		m_inData->AS3_writeObject(args->atom());
		m_outData->AS3_writeUnsignedInt(m_inData->GetLength());
		m_outData->AS3_writeBytes(m_inData,0,m_inData->GetLength());

		/*XFile file;
		if(file.Open("E:\\aobj.dat",XFile::XBINARY|XFile::XCREATE|XFile::XWRITE))
		{
			file.Write(m_outData->GetByteArray().GetBuffer(),m_outData->GetLength());
			file.Close();
		}*/

	}
	m_inData->SetLength(0);
	m_inData->Seek(0);

	XU8Array datas;
	datas.Append(m_outData->GetByteArray().GetBuffer(),
					 m_outData->GetLength());

	if(m_pClient!=NULL)
	{
		XClient*pc=(XClient*)m_pClient;
		if(pc->SendRequest(datas))
			return;
		if(m_pLoaderInfo)
		{
			((ShellCore*)core())->GetPlayer()->ReleaseInfo(m_pLoaderInfo);
			m_pLoaderInfo=NULL;
		}
		pc->Cancel();
		pc->SetAlive(false);
		m_pClient=NULL;
	}
	if(m_pClient==NULL)
	{
		ShellCore*c=(ShellCore*)core();
		XSWFPlayer*player=c->GetPlayer();
		StUTF8String strURL(m_strURL);
		//if(str!=NULL)
		//	return;
		StUTF8String strType(c->kAMFContent);
		int nType=kHttpLoadTarget|kHttpSendUsePost|kHttpLoadRPC;
		//const char*context=NULL;
		//LoaderInfoObject*pEvent=m_pCodeContext->m_pLoaderInfo;
			//(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
		_XLoadInfo*pInfo=player->AddLoader(strURL.c_str(),NULL,nType,NULL,this,strType.c_str());
		if(m_codeContext->m_pRoot)
		{
			pInfo->strRefer=m_codeContext->m_pRoot->m_url;
			pInfo->strRefer.ToString(XTRUE);
		}
		pInfo->postData.Append(datas);
		//if(m_strHeaders.GetSize())
		//	pInfo->strHeaders.Append(m_strHeaders);
	}
	//m_inData->AS3_writeObject(args->atom());
}


bool  NetConnectionObject::StreamInNew(void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo)
{
	m_pClient=pClient;
	m_pLoaderInfo=loadInfo;
	if(pClient)
	{
		XClient*pc=(XClient*)pClient;
		pc->SetAlive(true);
		pc->SetNeedCache(true);
		if(nCode<100||nCode>=300)
		{
			this->OnIOErrorEvent(XEnumHttpStatus::GetErrorString(nCode));

			ScriptObject*pObj=CreateStatusObject(NC_CONNECT_FAILED);////CreateStatus(toplevel()->objectClass->newInstance();
			this->OnNetStatusEvent(pObj);
			XClient*pc=(XClient*)pClient;
			((ShellCore*)core())->GetPlayer()->RemoveClient(pClient);
			pc->SetWindow(NULL,0,0);
			m_bConnected=false;
		}
	}
	return true;
}
int   NetConnectionObject::StreamInWrite(void* buffer,int	  length,void*gcEnter,void*pClient)
{
	m_inData->WriteBytes((XU8*)buffer,length);
	return XSWFPlayer::LOAD_OK;
}
bool  NetConnectionObject::StreamInDestroy(XBOOL bOK,const char*msg,void*pClient)
{
	bool bRemove=false;
	if(pClient)
	{
		
		XClient*pc=(XClient*)pClient;


		if(!pc->IsConnecting())
		{
			//int id=NC_CONNECT_CLOSED;////CreateStatus(toplevel()->objectClass->newInstance();
			ScriptObject*pObj=CreateStatusObject(NC_CONNECT_CLOSED);////CreateStatus(toplevel()->objectClass->newInstance();
			this->OnNetStatusEvent(pObj);
			this->m_bConnected=false;
			m_pClient=NULL;
			bRemove=true;
			if(m_pLoaderInfo)
			{
				((ShellCore*)core())->GetPlayer()->ReleaseInfo(m_pLoaderInfo);
				m_pLoaderInfo=NULL;
			}
		}
/*#if (__CORE_VERSION__>=0x02080000)
		else
		{
			pc->Cancel();
			pc->SetWindow(NULL,0,0);
			((ShellCore*)core())->GetPlayer()->RemoveClient(pClient);
			m_bConnected=false;
			m_pClient=NULL;
			bRemove=true;
			if(m_pLoaderInfo)
			{
				((ShellCore*)core())->GetPlayer()->ReleaseInfo(m_pLoaderInfo);
				m_pLoaderInfo=NULL;
			}
		}
#endif*/
		if(!bOK&&m_bConnected)
		{
			ScriptObject*pObj=CreateStatusObject(NC_CONNECT_FAILED);////CreateStatus(toplevel()->objectClass->newInstance();
			this->OnNetStatusEvent(pObj);
			pc->Cancel();
			pc->SetWindow(NULL,0,0);
			((ShellCore*)core())->GetPlayer()->RemoveClient(pClient);
			m_bConnected=false;
			m_pClient=NULL;
			bRemove=true;
			if(m_pLoaderInfo)
			{
				((ShellCore*)core())->GetPlayer()->ReleaseInfo(m_pLoaderInfo);
				m_pLoaderInfo=NULL;
			}
		}
	}
	m_inData->Seek(0);
	ShellToplevel*top=(ShellToplevel*)toplevel();
	ByteArrayClass*pClass=top->getByteArrayClass();	
	ByteArrayObject* inData=pClass->CreateArray(0);
	m_inData->AS3_readBytes(inData,0,m_inData->GetLength());
	inData->Seek(0);

	int nVersion=inData->AS3_readShort();
	if(nVersion>3)
	{
		ScriptObject*pObj=CreateStatusObject(NC_CALL_BADVERSION);////CreateStatus(toplevel()->objectClass->newInstance();
		this->OnNetStatusEvent(pObj);
		return false;
	}
	m_bCalling=false;
	int nHeaderCount=inData->AS3_readShort();

	int i;
	for(i=0;i<nHeaderCount;i++)
	{
		Stringp name=inData->AS3_readUTF();
		bool	bMust=inData->AS3_readBoolean();
		int		nLength=inData->AS3_readUnsignedInt();
		inData->AS3_objectEncoding_set(kAMF0);
		Atom	value=inData->AS3_readObject();
	}
	int nMessageCount=inData->AS3_readShort();
	/*if(inData->GetLength()>=6280)
	{
		XFile file;
		file.Open("E:\\nc.dat",XFile::XBINARY|XFile::XCREATE|XFile::XWRITE);
		file.Write(inData->GetByteArray().GetBuffer(),
				   inData->GetLength());
		file.Close();
	}*/
//#if (__CORE_VERSION__>=0x02080000)
	m_bDating=true;
//#endif
	for(i=0;i<nMessageCount;i++)
	{
		Stringp resStr=inData->AS3_readUTF();
		Stringp target=inData->AS3_readUTF();
		int nLength=inData->AS3_readUnsignedInt();
		int pos=inData->GetFilePointer();
		inData->AS3_objectEncoding_set(kAMF0);
		Atom o[]={nullObjectAtom,inData->AS3_readObject()};
		bool isResult=true;
		ResponderObject*res=this->callbackResponse(resStr,isResult);
		if(res)
		{
			if(isResult)
			{
				if(res->m_pResultCall)
				{
					o[0]=res->m_pResultCall->atom();
					res->m_pResultCall->call(1,o);
				}
			}
			else if(res->m_pStatusCall)
			{
				o[0]=res->m_pResultCall->atom();
				res->m_pStatusCall->call(1,o);
			}
		}
		if(pos+nLength!=inData->GetFilePointer())
		{
			inData->Seek(pos);
		}
	}
//#if (__CORE_VERSION__>=0x02080000)
	m_bDating=false;
	
//#endif
	return bRemove;
}

//new adds
Stringp NetConnectionObject::AS3_farID_get()
{
	return m_farID;
}
Stringp NetConnectionObject::AS3_farNonce_get()
{
	return m_farNonce;
}
double NetConnectionObject::AS3_httpIdleTimeout_get()
{
	return m_httpIdleTimeout;
}
void NetConnectionObject::AS3_httpIdleTimeout_set(double httpIdleTimeout)
{
	m_httpIdleTimeout = httpIdleTimeout;
}
uint32_t NetConnectionObject::AS3_maxPeerConnections_get()
{
	return m_maxPeerConnections;
}
void NetConnectionObject::AS3_maxPeerConnections_set(uint32_t maxPeerConnections)
{
	m_maxPeerConnections = maxPeerConnections;
}
Stringp NetConnectionObject::AS3_nearID_get()
{
	return m_nearID;
}
Stringp NetConnectionObject::AS3_nearNonce_get()
{
	return m_nearNonce;
}
Stringp NetConnectionObject::AS3_protocol_get()
{
	return m_protocol;
}
ArrayObject* NetConnectionObject::AS3_unconnectedPeerStreams_get()
{
	return m_unconnectedPeerStreams;
}
//new adds end

}
