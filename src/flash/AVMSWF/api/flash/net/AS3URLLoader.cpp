#include "StdAfxflash.h"
#include "ShellCore.h"
#include "splay.h"
#include "splayer.h"
#include "avm2.h"
using namespace avmplus;
#include "AS3URLLoader.h"
#include "AS3URLLoaderDataFormat.h"
#include "AS3ByteArray.h"
#include "AS3Event.h"
#include "AS3HTTPStatusEvent.h"
#include "AS3ProgressEvent.h"
#include "AS3IOErrorEvent.h"
#include "AS3URLVariables.h"

#include "XDom.h"
#include "XHTMLScript.h"
#include "XDomCard.h"

//#define _DOSCRIPT_

namespace avmshell{
URLLoaderClass::URLLoaderClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())URLLoaderObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* URLLoaderClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) URLLoaderObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

URLLoaderObject* URLLoaderClass::CreateURLLoader(URLRequestObject *pRequest)
{
	URLLoaderObject *pObj = (URLLoaderObject *)createInstance(ivtable(), prototype);

	pObj->AS3_constructor(pRequest);

	return pObj;
}

URLLoaderObject::URLLoaderObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_dataFormat=((ShellToplevel*)toplevel())->getURLLoaderDataFormatClass()->getSlotTEXT();
	m_nDataFormat=LD_TEXT;
	m_bCanceled=false;
	m_bLoading=false;
	m_nLoadID=0;
	m_pClient=XNULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
uint32_t URLLoaderObject::AS3_bytesLoaded_get()
{
	//Add your act code here...
	return m_bytesLoaded;
}

void URLLoaderObject::AS3_bytesLoaded_set(uint32_t bytesLoaded)
{
	//Add your act code here...
	m_bytesLoaded = bytesLoaded;
}

uint32_t URLLoaderObject::AS3_bytesTotal_get()
{
	//Add your act code here...
	return m_bytesTotal;
}

void URLLoaderObject::AS3_bytesTotal_set(uint32_t bytesTotal)
{
	//Add your act code here...
	m_bytesTotal = bytesTotal;
}

Atom URLLoaderObject::AS3_data_get()
{
	//Add your act code here...
	//ScriptObject*obj=(ScriptObject*)(m_pData&~7);
	//if(obj==NULL)
	//	int v=0;
	return m_pData;
}

void URLLoaderObject::AS3_data_set(Atom pData)
{
	//Add your act code here...
	m_pData = pData;
}

Stringp URLLoaderObject::AS3_dataFormat_get()
{
	//Add your act code here...
	return m_dataFormat;
}

void URLLoaderObject::AS3_dataFormat_set(Stringp dataFormat)
{
	//Add your act code here...
	m_dataFormat = dataFormat;
	URLLoaderDataFormatClass*pClass=((ShellToplevel*)toplevel())->getURLLoaderDataFormatClass();
	if(dataFormat->equals(pClass->getSlotVARIABLES()))
		m_nDataFormat=LD_VAR;
	else if(dataFormat->equals(pClass->getSlotBINARY()))
		m_nDataFormat=LD_BINARY;
	else
		m_nDataFormat=LD_TEXT;
}

//AS3 contructor function..
void URLLoaderObject::AS3_constructor(URLRequestObject *pRequest)
{
	//Add your act code here...
	if(pRequest)
		AS3_load(pRequest);
}

void URLLoaderObject::AS3_close()
{
	//Add your act code here...
//#if (__CORE_VERSION__>=0x02075000)
	if(!m_bLoading)
		return;
//#endif
	if(m_pClient)
	{
		((XClient*)m_pClient)->Cancel();
		((XClient*)m_pClient)->SetWindow(NULL,0,0);
		((ShellCore*)core())->GetPlayer()->CloseClient(m_pClient);
		m_pClient=NULL;
	}
//#if (__CORE_VERSION__>=0x02075000)
	else if(m_nLoadID)
	{
		((ShellCore*)core())->GetPlayer()->RemoveLoader(m_nLoadID);
		m_nLoadID = 0;
	}
//#endif
	m_bCanceled=true;
	m_bLoading=false;
}

void URLLoaderObject::AS3_load(URLRequestObject *url)
{
	if(m_bLoading) return;
	if(!url->m_strURL)
		toplevel()->throwArgumentError(kInvalidArgumentError,"null");

	//Add your act code here...
	/*ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	StUTF8String strURL(url->m_strURL);
	XU8Array datas;
	Stringp strType=url->GetPostData(datas,false);
	if(strType==NULL) strType=url->m_strContext;
	int nType=kHttpLoadTarget|kHttpLoadData;
	if(url->m_nMethod==URLRequestMethodClass::POST)
		nType|=kHttpSendUsePost;
	else
		nType|=kHttpSendUseGet;
	//const char*context=NULL;
	//LoaderInfoObject*pEvent=(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
	_XLoadInfo*pInfo=player->AddLoader(strURL.c_str(),NULL,nType,NULL,this,strType?StUTF8String(strType).c_str():NULL);
	if(datas.GetSize())
		pInfo->postData.Append(datas);*/
	
#ifdef _WINEMU

	//StUTF8String ss(url->m_strURL);
	//XString16 s;
	//s.FromInt((XINT)this,true);
	//XSWFPlayer::m_pInstance->Trace(s);
	//if(strstr(ss.c_str(),".9yur")!=NULL)
	//	int vv=0;
	/*if(strstr(ss.c_str(),"Function")!=NULL)
		int vv=0;*/

	m_strURL=url->m_strURL;


	

#endif

	AS3_close();

	m_buffer=NULL;
	m_pData=nullObjectAtom;
	m_bytesTotal=0;
	m_bytesLoaded=0;
	m_bLoading=true;
//#if (__CORE_VERSION__>=0x02075000)
	m_nLoadID=LoadURL(url,NULL);
//#else
//	LoadURL(url,NULL);
//#endif
}


bool URLLoaderObject::StreamInNew(void*sreamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo)
{
	//Atom atom=m_pData;
	AvmCore*c=core();
	ShellToplevel*top=(ShellToplevel*)toplevel();
	_XStreamData*stream=(_XStreamData*)sreamData;	// link the objects
	this->m_bytesTotal=stream->nTotalBytes;
	this->m_bytesLoaded=0;
	m_bCanceled=false;
	m_pClient=pClient;
	m_buffer=top->getByteArrayClass()->CreateArray(stream->nTotalBytes);
	
	m_pData=nullObjectAtom;
	
	if(m_nDataFormat==LD_VAR)
	{
		if(headList.GetSize())
		{
			for(int i=0;i<(int)headList.GetSize()-1;i+=2)
			{
				m_buffer->WriteBytes((XU8*)headList[i].GetData(),headList[i].GetLength());
				m_buffer->AS3_writeByte('=');
				m_buffer->WriteBytes((XU8*)headList[i+1].GetData(),headList[i+1].GetLength());
				m_buffer->AS3_writeByte('&');
				//Stringp s=c->newStringUTF8(headList[i],headList[i].GetLength());
				//XXVar name,txt;
				//name.SetString(headList[i],headList[i].GetLength());
				//txt.SetString(headList[i+1],headList[i+1].GetLength());

				//p->m_heads.Add(name);
				//p->m_heads.Add(txt);
			}
//#if (__CORE_VERSION__>=0x02079000)
			this->m_bytesLoaded=m_buffer->GetLength();
//#endif
		}
	}
	/*if(hasEvent(XAS3_EVENT_open))
	{
		EventObject*pObj=top->getEventClass()->CreateEventObject(XAS3_EVENT_open);
		pObj->pTarget=this;
		AS3_dispatchEvent(pObj);
	}*/
	//if(nCode!=200)
	//	int v=0;
	OnEvent(XAS3_EVENT_open,false);
	{
#ifdef _WINEMU
		XString16 ss;
		ss.FromInt(nCode);
		XString16 s("Stream In New Code:");
		s+=ss;
		XSWFPlayer::m_pInstance->Trace(s);
#endif
	}
	OnHTTPStatusEvent(nCode);
	/*if(hasEvent(XAS3_EVENT_httpStatus))
	{
		EventObject*pEvent=top->getHTTPStatusEventClass()->CreateEventObject(XAS3_EVENT_httpStatus,nCode);
		pEvent->pTarget=this;
		//pEvent->pTarget=(ScriptObject*)m_pLoaderInfo;
		AS3_dispatchEvent(pEvent);
	}*/
	//p->AS3OnOpen();
	//p->AS3OnHttpStatus(nCode);
	
	return true;
}
int URLLoaderObject::StreamInWrite(void* buffer,int  length,void*gcEnter,void*pClient)
{
	if(m_buffer)
	{
		this->m_bytesLoaded+=length;
		//m_buffer->SetLength(m_bytesLoaded);
		m_buffer->WriteBytes((XU8*)buffer,length);
	
		/*if(hasEvent(XAS3_EVENT_progress))
		{
			ProgressEventObject*pEvent=((ShellToplevel*)toplevel())->getProgressEventClass()->CreateEventObject(XAS3_EVENT_progress,
										 this->m_bytesLoaded,this->m_bytesTotal);
			pEvent->pTarget=this;
			AS3_dispatchEvent(pEvent);
		}*/
		OnProgressEvent(XAS3_EVENT_progress,this->m_bytesLoaded,this->m_bytesTotal);

		if(m_bCanceled)
			return XSWFPlayer::LOAD_BREAK;
		return XSWFPlayer::LOAD_OK;		
	}
	
	return XSWFPlayer::LOAD_BREAK;
}
bool URLLoaderObject::StreamInDestroy(XBOOL bOK,const char*msg,void*pClient)
{
	//return false;
	if(m_pClient&&m_pClient!=pClient)
		return true;
#ifdef _WINEMU
	//StUTF8String s(m_strURL);
	//if(strstr(s.c_str(),"GameUI")!=NULL)
	//	int v=0;
#endif
	m_pClient=NULL;
	m_bLoading=false;
//	if(this->m_bytesLoaded!=this->m_bytesTotal)
//		int v=0;
	if(m_buffer)
	{
				if(this->m_bytesLoaded!=m_buffer->GetLength())
					m_buffer->SetLength(this->m_bytesLoaded);
				m_buffer->Seek(0);
#ifdef _WINEMU_DEBUGFILE
				//if(l>20480)
				if(m_buffer->GetLength())
				{
					ShellCore*c=(ShellCore*)core();
					XString8 m_strFile=c->GetPlayer()->m_strWorkPath.strTxt;
					m_strFile+="Datas\\";
					
					XFile::CreateFolder(m_strFile);
					//int le;
					//const char*appName=c->GetPlayer()->player->GetAppName(le);
					StUTF8String url(m_strURL);
					const char*strurl=url.c_str();
					const char*appName=strrchr(strurl,'/');
					if(appName==NULL)
						appName=strurl;
					else 
					{
						int iSets = 1;
						while(appName>strurl)
						{
							char c=*appName;
							if(c=='\\'||c=='/')
							{
								iSets++;
								if(iSets>2) break;
							}
							else if(c=='.')
								break;
							appName--;
						}
						appName++;
					}
					const char*end=strrchr(strurl,'?');

					if(appName)
					{
						int ll=m_strFile.GetLength();
						//if(end==NULL)
							m_strFile+=appName;
						//else
						//	m_strFile+=XString8(appName,end-appName);
						char*ss=(char*)m_strFile.GetData()+ll;
						while(*ss)
						{
							switch(*ss)
							{
							case '?':
							case '\\':
							case '/':
								*ss='_';
								break;
							}
							ss++;
						}
						XFile file;
						//m_strFile+=".swf";
						XU8*gzdata=m_buffer->GetByteArray().GetBuffer();
						int l=m_buffer->GetByteArray().GetLength();
						file.Open(m_strFile,XFile::XCREATE|XFile::XWRITE|XFile::XBINARY);
						file.Write(gzdata,l);
						file.Close();
					}

				}
#endif
		AvmCore*c=core();
		switch(m_nDataFormat)
		{
		case LD_TEXT:
			{
				Stringp s=m_buffer->AS3_toString();
					//c->newStringUTF8((char*)m_buffer->GetByteArray().GetBuffer(),m_buffer->GetLength());
				
				m_pData=s->atom();
#ifdef		_DOSCRIPT_
				Stringp strSp=core()->cachedChars[(int)'/'];
				ShellCore* c=(ShellCore*)core();
				int dsp0= s->indexOf(c->cachedChars[(int)'<']);
				
				if(dsp0>=0)
				{
					while(s->charAt(dsp0)==' ') dsp0++;					
					int dsp1= s->indexOf(c->cachedChars[(int)'>'],dsp0+1);
					
					if(dsp1>dsp0+6)
					{
						StUTF8String htmlStr(s);				
						XDom dom;
						dom.Reset(XCHARSET_UTF8,XCNT::CTEXT,XCNT::TVND_WAP_WML,XNULL);
						dom.AddTag(new XDomNode(XDOM_WML));
						dom.AddTag(new XDomCard());
						dom.PhaseXML((XU8*)htmlStr.c_str(),htmlStr.length(),true,true);
						XDomList list;
						dom.GetList(list,XDOM_SCRIPT);
						ShellCore* c=(ShellCore*)core();
						int i,iCount=list.GetSize();
						for(i=0;i<iCount;i++)
						{
							XHTMLScript*item=(XHTMLScript*)list[i];
							Stringp input=c->newStringLatin1(item->m_strTxt,item->m_strTxt.GetLength());
							//input=String::concatStrings(c->kJavaScriptPackage,input);
							//input=String::concatStrings(input,c->cachedChars[(int)'}']);
							c->evaluateString(input,false);//(this,input->atom());
						}
					}
				}
#endif
			}break;
		case LD_BINARY:
			m_pData=m_buffer->atom();
			break;
		case LD_VAR:
			{
				Stringp s=m_buffer->AS3_toString();
					//c->newStringUTF8((char*)m_buffer->GetByteArray().GetBuffer(),m_buffer->GetLength());
				URLVariablesObject*pVar=((ShellToplevel*)toplevel())->getURLVariablesClass()->CreateVars(s);
				m_pData=pVar->atom();
			}
			 break;
		}
		m_buffer=NULL;
		if(bOK)
		{
			/*if(hasEvent(XAS3_EVENT_complete))
			{
				EventObject*pObj=((ShellToplevel*)toplevel())->getEventClass()->CreateEventObject(XAS3_EVENT_complete);
				pObj->pTarget=this;
				AS3_dispatchEvent(pObj);		
			}*/
			OnEvent(XAS3_EVENT_complete,false);
		}
		else
		{
			OnIOErrorEvent(msg);
			/*if(hasEvent(XAS3_EVENT_ioError))
			{
				Stringp s=core()->newStringLatin1(msg);
				EventObject*pObj=((ShellToplevel*)toplevel())->getIOErrorEventClass()->CreateEventObject(XAS3_EVENT_ioError,s);
				pObj->pTarget=this;
				AS3_dispatchEvent(pObj);		
			}*/
		}
	}
	else
	{		
		OnProgressEvent(XAS3_EVENT_progress,0,0);
		if(bOK)
		{
			OnEvent(XAS3_EVENT_complete,false);
		}
		else
		{
			if(pClient)
			{
				XClient*pc=(XClient*)pClient;
				int nCode=pc->GetResponseInfo()->nErrorCode;			
				if(nCode!=XHTTP_STATUS_OK)
				{
					OnHTTPStatusEvent(nCode);
				}
			}
			OnIOErrorEvent(msg);
		}
	}
	return true;
}

}
