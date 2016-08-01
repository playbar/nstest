#include "StdAfxflash.h"
#include "sobject.h"
#include "splayer.h"
#include "ShellCore.h"
using namespace avmplus;
#include "avm2.h"
#include "AS3Loader.h"
#include "AS3LoaderInfo.h"
#include "AS3URLRequest.h"
#include "AS3URLRequestMethod.h"
#include "AS3ByteArray.h"
#include "AS3LoaderContext.h"
#include "AS3ApplicationDomain.h"


namespace avmshell
{
    
LoaderClass::LoaderClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	AvmCore* c=core();
	ShellToplevel*top=(ShellToplevel*)toplevel();
	this->m_pByteObjects=new (c->GetGC())RCHashTable((ScriptObject*)top->m_pDumpObject.value(),4,false);
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())LoaderObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* LoaderClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LoaderObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
	
}


LoaderObject* LoaderClass::CreateLoader()
{
	LoaderObject*pObj=(LoaderObject*)createInstance(ivtable(),prototype);
	pObj->AS3_constructor();
	return pObj;
}

LoaderObject::LoaderObject(VTable *vtable, ScriptObject* proto, int capacity):  DisplayObjectContainerObject(vtable, proto, 0)
{
	m_pPlayer=XNULL;
	m_pClient=XNULL;
	m_bImage=false;	
	m_pStream=XNULL;
	m_bCache=false;
	m_bFirst=false;
	m_bLoading=false;
	m_bConnecting=false;
	m_bPush=false;
	m_bStoped=XFALSE;
	m_nLoadID=0;
}


DisplayObjectObject* LoaderObject::AS3_root_get()
{
	return DisplayObjectObject::AS3_root_get();
}

void LoaderObject::Release()
{
	if(m_pStream)
		delete m_pStream;
	if(IsRunning())
	{
		//DisplayObject*pObj=this->AS3_content_get();
		
		if(m_bCache)
			((ShellToplevel*)toplevel())->DisConnect(m_url,this);			
		
		if(m_pContentLoaderInfo)
		{
			m_pContentLoaderInfo->ClearEventInfo();
			m_pContentLoaderInfo->m_pLoader=NULL;
		}
		if(m_pPlayer)
		{			
			m_pPlayer=NULL;
		}
	}
	m_bCache=false;
	m_bFirst=true;
	DisplayObjectContainerObject::Release();
}

ScriptPlayer* LoaderObject::GetPlayer()
{
	//if(m_pObject)
	//	return m_pObject->character->player;
	//return NULL;
	return m_pPlayer;
}


bool LoaderObject::StreamInNew(void*sreamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo)
{
	m_pClient=pClient;	
	m_bFirst=true;
//m_bContent=true;
	
	if(m_pPlayer)
	{
		ScriptPlayer*p=m_pPlayer;//m_pObject->character->player;
		if(p)
		{
			p->m_bCanceled=false;
			//p->AS3Unload();
			p->stream = (_XStreamData*)sreamData;	// link the objects
			p->m_url=streamUrl;
			
			p->stream->scriptPlayer=p;
			p->m_url.ToString(XTRUE);
			p->m_loadUrl=p->m_url;
			p->m_contentType=contentType;
			p->m_contentType.ToString(XTRUE);
			/*XContentType type;
			type.SetType(contentType);
			if(type.m_nClass==XCNT::CIMAGE)
			{	
				m_bImage=true;
				if(m_pStream==NULL)
					m_pStream=new XStream();
				if(m_pStream)
					m_pStream->SetBuffer(p->stream->nTotalBytes+1);
			}*/
			//else
			//	((ShellToplevel*)toplevel())->RemoveImage(m_url);
#ifdef _WINEMU_DEBUGFILE
			char*host=strstr(streamUrl,"://");
			if(host)
			{
				//((ShellToplevel*)toplevel())->PushObject(m_pLoaderInfo);	
				
				char* id=strchr(streamUrl,'?');
				char* start=NULL;//strchr(host+3,'/');	
				if(id)
				{
					start=id-1;
					while(start>streamUrl)
					{
						if(*start=='/') break;
						start--;
					}
				}
				else
					start=strrchr(host,'/');

				if(start)
				{
					//start+=3;
					char*s=streamUrl;
					int iCode=0;
					while(s<start)
					{
						if(*s=='-')
							iCode+='_';
						else
							iCode+=*s;
						s++;
					}
					//ShellCodeContext*code=(ShellCodeContext*)core()->codeContext();
					ScriptPlayer*player=p;//character->player;
						//code->m_pRoot;
					XString8 strPath=player->splayer->m_strWorkPath.strTxt;
					strPath+="Download\\";
					XFile::CreateFolder(strPath);
					//int is=strPath.GetLength(),ie;
					XString8 strInt;
					strInt.FromInt(iCode);
					strPath+=strInt;
					strPath+="_";
					if(id)
						strPath.AddString(start+1,id-start-1);
					else
						strPath.AddString(start+1);
					
					/*ie=strPath.GetLength();
					char*buf=strPath.GetData();
					for(int ii=is;ii<ie;ii++)
					{
						switch(buf[ii])
						{
						case '/':
						case '\\':
						//case '.':
							 buf[ii]='_';break;
						}
					}*/
					//strPath+=".swf";
					m_file.Open(strPath,XFile::XCREATE|XFile::XBINARY|XFile::XWRITE);
				}
				//int id=strchr(streamUrl,"."
			}
#endif

			//p->scriptLen=p->stream->nTotalBytes;
			//p->len=0;
			for(int i=0;i<(int)headList.GetSize()-1;i+=2)
			{
				XXVar name,txt;
				name.SetString(headList[i],headList[i].GetLength());
				txt.SetString(headList[i+1],headList[i+1].GetLength());

				p->m_heads.Add(name);
				p->m_heads.Add(txt);
			}
			
			p->AS3_OnOpen();
			p->AS3_OnHttpStatus(nCode);
			return true;
		}
	}
	return false;
}
int LoaderObject::StreamInWrite(void* buffer,int  length,void*gcEnter,void*pClient)
{
	//if(!m_bContent) 
	//	return XSWFPlayer::LOAD_BREAK;
	//if(m_pPlayer)
	//{
		
		ScriptPlayer*p=m_pPlayer;//m_pObject->character->player;
		if(p)
		{
			//p->stream->nLoadedBytes+=length;
#ifdef _WINEMU_DEBUGFILE
			if(m_file.IsValid())
				m_file.Write(buffer,length);
#endif
			if(m_bFirst)
			{
				m_bFirst=false;
				if(!m_bImage)
				{
					switch(((char*)buffer)[0])
					{
//#if (__CORE_VERSION__>=0x02070200)
					case 0x30:
						 buffer=((char*)buffer)+68;
						 length-=68;
						 break;
//#endif
					case 'Z':
					case 'C':					
					case 'F':
					case 'S':
						break;

					default:
						{
							switch(((char*)buffer)[0])
							{
								case 'p':
									p->m_contentType="image/png";
									//player->AS3InitImageLoader();
									//player->scriptLen=l;
									break;
								case 'j':
									p->m_contentType="image/jpg";
									//player->AS3InitImageLoader();
									//player->scriptLen=l;
									break;
								case 'g':
									p->m_contentType="image/gif";
									//player->AS3InitImageLoader();
									//player->scriptLen=l;
									break;
							}
						}
						m_bImage=true;
						p->scriptLen=p->stream->nTotalBytes;
						if(m_pStream==NULL)
							m_pStream=new XStream();
						if(m_pStream)
							m_pStream->SetBuffer(p->stream->nTotalBytes+1);
						break;
					}
				}
			}
			if(m_bImage)
			{
				if(m_pStream)
					m_pStream->WriteData(buffer,length);
				p->len=m_pStream->GetSize();
				p->AS3_OnProgress(p->len);
				//m_pByteData->WriteBytes((XU8*)buffer,length);
			}
			else
			{
				PushData(buffer,length,gcEnter,false,true);
				/*int nCode=p->PushData((U8*)buffer, length,gcEnter);			
				if ( (p->pASSuper||(p->m_bInitOK&&!p->bAS3))&&!p->m_bLoadedOK)// && (this->numFramesComplete>0||m_pAVM2)) {
				{
					nCode=p->DrawFrame(0, true,false);
					//////////////////////////////////////////////////////////////////for AS3
					if (  nCode == ScriptPlayer::playOK|| p->ScriptComplete()) 
					{
						//if(IsAS3()&&!m_bLoadedOK)					
							p->m_bLoadedOK=true;
							if(p->bAS3)
							{
								SObject*pObj=p->rootObject;
								if(!pObj->pASObject)
									p->pASSuper=pObj->AS3CreateInst(p->pASSuper,XTRUE);
								{
									//pObj->AS3CreateAndConstruct(splayer->player->pASSuper,XTRUE);
									p->rootObject->AS3ConstructInst(p->pASSuper);
									//CreateLoader();
									p->AS3_OnInit();

									p->m_pAVM2->OnEvent(XAS3_EVENT_added,false,p->rootObject->pASObject,p->m_pCodeContext);
									if(p->display->root.IsParentOf(p->rootObject))
										p->m_pAVM2->OnEvent(XAS3_EVENT_addedToStage,false,p->rootObject->pASObject,p->m_pCodeContext);

									pObj->CallFrame(0);
									pObj->thread->m_bAS3Frame0=false;
									
								}
							}
					}
				}
				//return length;
				if(nCode==ScriptThread::notSupport)
				{
					p->stream->scriptPlayer=XNULL;
					p->m_bCanceled=true;
				}*/
			}
			//p->AS3_OnProgress(m_bImage?m_pStream->GetSize():0);
			if(p->len>=p->scriptLen)
			{
#ifdef _WINEMU_DEBUGFILE
				//if(m_file.IsValid())
				//	m_file.Close();
#endif
				//p->AS3_OnComplete();
				p->m_bLoadedOK=true;
				m_bConnecting=false;
			}
			if(p->m_bCanceled)
				return XSWFPlayer::LOAD_BREAK;
			return XSWFPlayer::LOAD_OK;
		}
	//}
	return XSWFPlayer::LOAD_BREAK;
}
bool LoaderObject::StreamInDestroy(XBOOL bOK,const char*msg,void*pClient)
{
	
	//if(m_url)
	//{
	//	StUTF8String ss(m_url);
	//	if(strstr(ss.c_str(),"dailyRewardBtn.png"))
	//	{
	//		int v=0;
	//	}
	//	//	int v=0;
	//}
	//if(!bOK)
	//		int v=0;
#ifdef _WINEMU_DEBUGFILE
	/*if(m_url)
	{
		StUTF8String ss(m_url);
		if(strstr(ss.c_str(),"icon_001.png")!=NULL)
			int v=0;
	}*/
	if(m_file.IsValid())
		m_file.Close();
#endif
	//if(!m_bContent) return false;
	if(m_pClient&&m_pClient!=pClient)
		return true;
	m_bConnecting=false;
//#if (__CORE_VERSION__>=0x02077000)
	m_nLoadID=0;
//#endif
//#if (__CORE_VERSION__>=0x02070200)
	m_bLoading=false;
//#endif
	m_pClient=NULL;
	if(m_pPlayer)//&&this->m_bContent)
	{
		CheckAS3();
		ScriptPlayer*p=m_pPlayer;//m_pObject->character->player;
		//if(p)
		{
			ShellToplevel*top=(ShellToplevel*)toplevel();
			if(m_bImage)
			{
				//StUTF8String s(this->m_url);
				//if(strstr(s.c_str(),"100102.jpg")!=0)
				//	int v=0;
				if(m_pStream)
				{
					p->m_bLoadedOK=true;
					//ShellToplevel*top=(ShellToplevel*)toplevel();
					U8*pData=m_pStream->GetData();//m_pByteData->GetByteArray().GetBuffer();
					int l=m_pStream->GetDataSize();//m_pByteData->GetGetLength();
					m_pPlayer->scriptLen=l;
					SCharacter*ch=(SCharacter*)top->Attach(m_url,pData,l,false,true,bOK?NULL:msg);
					
					//if(ch==NULL)
					//	ch=(SCharacter*)top->Attach(m_url,pData,l,false);
					if(ch)
					{
						ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
						ByteArrayObject*pObj=pClass->CreateArray(0);
						pObj->Attach(pData,l);
						p->m_pLoaderInfo->m_bytes=pObj;
						m_pStream->DataTo();
						//if(p->rootObject->pASObject)
						//	RemoveChildRef(p->rootObject->pASObject);
						p->AS3AttachBitmap(ch,p->m_pCodeContext);
						if(m_pObject)
							m_pObject->Modify();
					}
					//delete m_pStream;
				}
			}
			else
			{
				ShellToplevel*top=(ShellToplevel*)toplevel();
				SCharacter*ch=(SCharacter*)top->Attach(m_url,p->script,p->scriptLen,false,false,bOK?NULL:msg);
				if(ch)
				{
					ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
					ByteArrayObject*pObj=pClass->CreateArray(0);
					pObj->Attach(p->script,p->scriptLen);
					p->m_pLoaderInfo->m_bytes=pObj;

					top->LockImage(m_url);
					p->bAttach=true;	
					if(p->m_nShapes)
						XSWFPlayer::m_pInstance->m_newDatas.Add(p);
				}
			}
			top->DisConnect(m_url,this);
			p->AS3Complete(bOK!=0);
			if(p->stream)
			{
				p->stream->scriptPlayer=XNULL;
		   		p->stream = 0;	// disconnect the stream
			}

			//if(bOK)
			//	p->AS3_OnComplete();
			//else
			
			if(!bOK||!p->m_bLoadedOK)//||!p->rootObject->pASObject)
				p->AS3_OnIOError(msg);
//#if (__CORE_VERSION__>=0x02070200)
			else if(!m_bStoped)		
//#else
//			else
//#endif
			{
				if(m_bImage)
					p->AS3_OnInit();
				p->AS3_OnComplete();
			}
			return true;
		}
		
	}
	else if(!m_bCache)
	{

	}

	return true;
}
//////////////////////////////////////////////////////////
//Native Method start...
DisplayObjectObject* LoaderObject::AS3_content_get()
{
	//Add your act code here...
	if(m_pPlayer)
	{
		//if(m_bConnecting)
		//	return NULL;
		if(m_pPlayer->m_bHold)
		{
			m_pPlayer->m_bHold=false;
			m_pPlayer->HoldInit();
		}
		DisplayObjectObject*obj=(DisplayObjectObject*)m_pPlayer->rootObject->pASObject;
		m_pContent = obj;
		/*if(!obj)
		{
			//m_pPlayer->InitRoot();
			m_pPlayer->rootObject->AS3CreateInst(m_pPlayer->pASSuper,XTRUE);
			m_pPlayer->rootObject->AS3ConstructInst(m_pPlayer->pASSuper);
			obj=(DisplayObjectObject*)m_pPlayer->rootObject->pASObject;
		}*/
		//if(obj)
		//	obj->m_pLoaderInfo=this->m_pContentLoaderInfo;
		//Maybe error!!!!
		//m_bConnecting=false;
		/*if(!obj)
		{
			_XSObject*objs=m_pPlayer->GetBitmapContent();
			if(objs)
				obj=(DisplayObjectObject*)objs->pASObject;
		}
		if(obj)
		{
			obj->m_pLoadInfo=this->m_pLoaderInfo;
		}*/
		//if(!obj)
		//	int v=0;

		return obj;
	}
	return NULL;
	//return kAvmThunkUndefined;//Modify this please!
}

LoaderInfoObject* LoaderObject::AS3_contentLoaderInfo_get()
{
	//Add your act code here...
	//if(m_pContentLoaderInfo==NULL)
	//	int v=0;
	return m_pContentLoaderInfo;
}

//AS3 contructor function..
void LoaderObject::AS3_constructor()
{
	//Add your act code here...
	CreateObject();
	DisplayObjectContainerObject::AS3_constructor();

	//return kAvmThunkUndefined;//Modify this please!
}

void LoaderObject::InitObject()
{
	CreateObject();
}

void LoaderObject::Close()
{
	//if(m_url)
	//{
	//	StUTF8String ss(m_url);
	//	if(strstr(ss.c_str(),"dailyRewardBtn.png"))
	//	{
	//		int v=0;
	//	}
	//	//	int v=0;
	//}
	if(m_pClient)
	{
		((XClient*)m_pClient)->Cancel();
		((XClient*)m_pClient)->SetWindow(NULL,0,0);
		if(((ShellCore*)core())->GetPlayer()->CloseClient(m_pClient))
		{
			((ShellToplevel*)toplevel())->RemoveObject(this);
		}
		m_pClient=NULL;
	}
//#if (__CORE_VERSION__>=0x02077000)
	else if(this->m_nLoadID)
	{
		((ShellCore*)core())->GetPlayer()->RemoveLoader(m_nLoadID);
		m_nLoadID=0;
	}
	
//#endif
	if(m_pPlayer)
	{
		m_pPlayer->m_bCanceled=true;//character->player->m_bCanceled=true;
//#if (__CORE_VERSION__>=0x02070200)
		if(m_url&&m_bLoading)
//#else
//		if(m_bConnecting&&m_url)
//#endif
		{
			ShellToplevel*top=(ShellToplevel*)toplevel();
			top->DisConnect(m_url,this);
			if(!m_bCache)
			{
				StUTF8String strURL(m_url);
				if(((ShellCore*)core())->GetPlayer()->RemoveLoader(strURL.c_str()))
				{
					top->RemoveImage(m_url,true);
				}				
			}
			m_pPlayer=NULL;
			//m_bConnecting=false;
		}

	}
	m_bConnecting=false;
//#if (__CORE_VERSION__>=0x02070200)
	m_bLoading=false;
//#endif
}

void LoaderObject::AS3_close()
{
	//if(m_url)
	//{
	//	StUTF8String ss(m_url);
	//	if(strstr(ss.c_str(),"dailyRewardBtn.png"))
	//	{
	//		int v=0;
	//	}
	//	//	int v=0;
	//}
	//Add your act code here...
	/*if(m_url)
	{
		StUTF8String ss(m_url);
		if(strstr(ss.c_str(),"/camp/1.png"))
		{
			//XSWFPlayer::m_pInstance->Trace(XString16("-------------------------CLOSE IMAGE"));
			int v=0;
		}
		//	int v=0;
	}*/
	//return;
	//[Note X] -------
//#if (__CORE_VERSION__>=0x02075000)
	ShellToplevel*top=(ShellToplevel*)toplevel();
	if(top->GetRefCount(this,m_url)==0)
	{
		if(m_pClient)
		{
			((XClient*)m_pClient)->Cancel();
			((XClient*)m_pClient)->SetWindow(NULL,0,0);
			if(((ShellCore*)core())->GetPlayer()->CloseClient(m_pClient))
			{
				((ShellToplevel*)toplevel())->RemoveObject(this);
			}
			m_pClient=NULL;
		}
/*#if (__CORE_VERSION__>=0x02077000)
		else if(this->m_nLoadID)
		{
			((ShellCore*)core())->GetPlayer()->RemoveLoader(m_nLoadID);			
			m_nLoadID=0;
		}
		
#endif*/
		if(m_pPlayer)
		{
			m_pPlayer->m_bCanceled=true;//character->player->m_bCanceled=true;
			if(m_bConnecting&&m_url)
			{			
				top->DisConnect(m_url,this);
				if(!m_bCache)
				{
					if(((ShellCore*)core())->GetPlayer()->RemoveLoader(m_nLoadID))
					{
						top->RemoveImage(m_url,true);
					}	
					//this->m_msg="stoped";
					top->TriggerComplete(m_url,this);
				}
				m_pPlayer=NULL;
				//m_bConnecting=false;
			}

		}

		m_bConnecting=false;
		m_bStoped=XFALSE;
		return;
	}
//#endif
//#if (__CORE_VERSION__>=0x02070200)
	m_bStoped=XTRUE;
	//Close();  

	/*if(m_pClient)
	{
		if(((ShellCore*)core())->GetPlayer()->CloseClient(m_pClient))
		{
			((ShellToplevel*)toplevel())->RemoveObject(this);
		}
		m_pClient=NULL;
	}
	if(m_pPlayer)
	{
		m_pPlayer->m_bCanceled=true;//character->player->m_bCanceled=true;
		if(m_bConnecting&&m_url)
		{
			ShellToplevel*top=(ShellToplevel*)toplevel();
			top->DisConnect(m_url,this);
			if(!m_bCache)
			{
				StUTF8String strURL(m_url);
				if(((ShellCore*)core())->GetPlayer()->RemoveLoader(strURL.c_str()))
				{
					top->RemoveImage(m_url,true);
				}				
			}
			m_pPlayer=NULL;
			//m_bConnecting=false;
		}

	}
	m_bConnecting=false;*/
//#endif
	//return kAvmThunkUndefined;//Modify this please!
}

void LoaderObject::AS3_load(URLRequestObject*url,LoaderContextObject*pContext)
{	
	//Add your act code here...
	//return;
	//if(m_pPlayer)
//#ifdef _WINEMU
//	if(url)
//	{
//		StUTF8String ss(url->m_strURL);
//		if(strstr(ss.c_str(),"dailyRewardBtn.png"))
//		{
//			int v=0;
//		}
//		//	int v=0;
//	}
//#endif
	bool bTest=false;

	if(!url->m_strURL)
		toplevel()->throwArgumentError(kInvalidArgumentError,"null");
	{
		if(m_bConnecting&&m_url)
		{
			if(m_url->equals(url->m_strURL))
				return;
		}		
//#if (__CORE_VERSION__>=0x02070200)
	//Close();  
		if(m_bConnecting)
		{
			Close();
			
		}
		m_bStoped=XFALSE;
//#endif
//#if (__CORE_VERSION__>=0x02075000)
		m_nLoadID=0;
//#endif
		AS3_unload();
		CreateContent(pContext);
		if(m_pPlayer)
		{
			StUTF8String s(url->m_strURL);
			m_pPlayer->m_url=s.c_str();
			m_pPlayer->m_url.ToString(XTRUE);
			m_pPlayer->m_loadUrl=m_pPlayer->m_url;
			//if(strstr(s.c_str(),"AssetsChange"))
			//	int v=0;
		}
		
		m_url=core()->internString(url->m_strURL);


		//StUTF8String ss(url->m_strURL);
		//if(strstr(ss.c_str(),"revenue/2.png"))
		//	int v=0;

		//StUTF8String ss(m_url);
		//if(strstr(ss.c_str(),"info_bg")!=NULL)
		//	int v=0;

		ShellToplevel*top=(ShellToplevel*)toplevel();
		//m_pCache=top->GetImage(m_url);
		if(top->Connect(m_url,this))//m_pCache)
		{
			m_bCache=true;
			ShellCore*c=(ShellCore*)core();
			XSWFPlayer*player=c->GetPlayer();
			player->AddLoader("",NULL,kHttpLoadTarget|kHttpLoadData,NULL,this,NULL);
			m_bLoading=true;

			/*XString16 e("***********Load From cache:");
			e+=XString16(StUTF16String(m_url).c_str());
			XString16 strInt;
			strInt.FromInt((XU32)this,XTRUE);
			e+=strInt;
			XSWFPlayer::m_pInstance->Trace(e);*/
			//if(bTest)
			//	XSWFPlayer::m_pInstance->Trace(XString16("-------------IMAGE CACHED"));
		}
		else
		{
			m_bFirst=true;
			m_bConnecting=true;
//#if (__CORE_VERSION__>=0x02070200)
			m_bLoading=false;
//#endif
			top->CreateImage(m_url);
//#if (__CORE_VERSION__>=0x02075000)
			m_nLoadID=LoadURL(url,pContext);
//#else
//			LoadURL(url,pContext);
//#endif
		}
	}
}

void LoaderObject::AS3_loadBytes(ByteArrayObject*bytes,LoaderContextObject*pContext)
{
	if(bytes->GetLength()<=0)
		return;

	//if(bytes->GetLength()==13406)
	//	int v=0;

	ShellToplevel*top=(ShellToplevel*)toplevel();
	LoaderClass*pClass=top->getLoaderClass();
		//int v=0;
	AS3_unload();
	CreateContent(pContext);
/*#if (__CORE_VERSION__>=0x02075000)
	XU32 nAddr=(XU32)bytes->GetByteArray().GetBuffer();
	MD5 md5;
	char md5data[16];
	md5.MakeMD5((char*)nAddr,bytes->GetLength(),md5data);
	for(int i=0;i<16;i++)
	{
		nAddr|=(md5data[i]<<((i&3)*8));
	}
	if(pClass->m_pByteObjects->GetRefIndex((ScriptObject*)nAddr)>=0)
	{
		m_pPlayer->SetDummy();
	}
	else
		pClass->m_pByteObjects->put((ScriptObject*)nAddr);
#else*/
	XU32 nAddr=(XU32)bytes->GetByteArray().GetBuffer();
	if(pClass->m_pByteObjects->GetRefIndex((ScriptObject*)nAddr)>=0)
	{
		m_pPlayer->SetDummy();
	}
	else
		pClass->m_pByteObjects->put((ScriptObject*)bytes->GetByteArray().GetBuffer());
//#endif
	if(m_codeContext&&this->m_pContentLoaderInfo&&m_codeContext->m_pRoot)
	{
		m_pContentLoaderInfo->m_loadBy=m_codeContext->m_pRoot->m_pLoaderInfo;
		/*if(m_codeContext->m_pRoot&&m_codeContext->m_pRoot->m_pLoaderInfo)
		{
			m_pContentLoaderInfo->m_params=
				AvmCore::atomToScriptObject(m_codeContext->m_pRoot->m_pLoaderInfo->AS3_parameters_get());
		}*/
	}
	ShellCore*c=(ShellCore*)core();
	//m_url=c->internStringLatin1((const char*)&bytes,4);
	
	XSWFPlayer*player=c->GetPlayer();
	if(player->m_pDomFilter&&
	   player->m_pDomFilter->bHold)
	   this->m_pPlayer->m_bHold=XTRUE;
	//m_pCache=top->GetImage(m_url);
	/*if(top->Connect(m_url,this))//m_pCache)
	{	
		m_bCache=true;
		player->AddLoader("",NULL,kHttpLoadTarget|kHttpLoadData,NULL,this,NULL);
	}
	else*/
	{
		//m_pLoaderContext=pContext;
		//top->CreateImage(m_url);
		//ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
		//pClass
		m_pByteData=bytes->Clone();
			//bytes;
		
		//if(strType==NULL) strType=url->m_strContext;
		int nType=kHttpLoadTarget|kHttpLoadData;
		
		//LoaderInfoObject*pEvent=this->m_pLoaderInfo;//m_pCodeContext->m_pLoaderInfo;
			//(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
		//bytes->Lock();
		_XLoadInfo*pInfo=player->AddLoader("",NULL,nType,NULL,this,NULL);
	}
}


void LoaderObject::CacheComplete(void*c,const char*msg)
{

	/*if(m_url)
	{
		StUTF8String ss(m_url);
		if(strstr(ss.c_str(),"info_bg")!=NULL)
			int v=0;
	}*/
	/*StUTF8String ss(this->m_url);
	if(strstr(ss.c_str(),"16.jpg")!=NULL)
	{
		//XString16 url("CCCCCCCCCC------->");
		//url+=XString16(ss.c_str());
		//XSWFPlayer::m_pInstance->Trace(url);
		int v=0;
	}*/
	/*XString16 e("**************Data From cache:");
	e+=XString16(StUTF16String(m_url).c_str());
	XString16 strInt;
	strInt.FromInt((XU32)this,XTRUE);
			e+=strInt;
	XSWFPlayer::m_pInstance->Trace(e);*/


	if(!m_bCache||!m_pPlayer||!m_bLoading) return;


	ScriptPlayer*player=m_pPlayer;
	ShellToplevel*top=(ShellToplevel*)toplevel();
//#if (__CORE_VERSION__>=0x02076000)
	top->DisConnect(m_url,this);
	m_bConnecting=false;
	m_bCache=false;
	m_bLoading=false;
//#endif

	if(m_bPush)
	{
		top->RemoveObject(this);
		m_bPush=false;
	}
	if(player)
	{
		SCharacter*ch=(SCharacter*)c;
		if(ch->type==loaderSWFChar)
		{
			player->SetDummy();
			//player->AS3Unload();
			void*gzdata=ch->data;
			int l=ch->length;
			
				//toplevel()->throwArgumentError(kInvalidArgumentError,INTString(l));
			if(l>0)
			{
				ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
				ByteArrayObject*pObj=pClass->CreateArray(0);
				pObj->Attach((XU8*)gzdata,l);
				player->m_pLoaderInfo->m_bytes=pObj;
				_XStreamData stream;
				stream.nLoadedBytes=l;
				stream.nTotalBytes=l;
				stream.pLoader=this;
				stream.resize=false;
				stream.scriptPlayer=player;
				player->stream=&stream;			
				PushData(gzdata,l,NULL,true,false);
				player->AS3Complete(true);			
			}
			if(msg||!player->m_bLoadedOK||!player->rootObject->pASObject)
				player->AS3_OnIOError(msg);
			else
			{
				//player->AS3_OnInit();
				player->AS3_OnComplete();
			}
			player->stream=NULL;
			player->m_lockURL=m_url;
			top->LockImage(m_url);
			CheckAS3();
		}
		else
		{

			m_pPlayer->len=ch->length;
			m_pPlayer->scriptLen=ch->length;
			m_pPlayer->AS3AttachBitmap(ch,m_pPlayer->m_pCodeContext);
			player->AS3Complete(true);
			player->AS3_OnProgress();
			player->AS3_OnInit();
			if(msg)
				player->AS3_OnIOError(msg);
			else
				player->AS3_OnComplete();
			if(m_pObject)
				m_pObject->Modify();
		}


		/*if(player->rootObject==NULL)
		{
			XString16 e("Error!!!!!!!!!!!!!!!!!!!!!!!!");
			XSWFPlayer::m_pInstance->Trace(e);
		}*/
	}
//#if (__CORE_VERSION__>=0x02076000)
//#else
//	top->DisConnect(m_url,this);
//	m_bConnecting=false;
//	m_bCache=false;
//#if (__CORE_VERSION__>=0x02070200)
//	m_bLoading=false;
//#endif
//#endif

	/*{
		XString16 ee("************Data From cache OKK:");
		ee+=XString16(StUTF16String(m_url).c_str());
		XString16 strInt;
		strInt.FromInt((XU32)this,XTRUE);
		ee+=strInt;
		ee+=XString16(msg);
		XSWFPlayer::m_pInstance->Trace(ee);
	}*/
}

void LoaderObject::PushData(void*pData,int l,void*gcEnter,bool bAttach,bool bStream)
{
	if(!m_pPlayer) return;
	ScriptPlayer*player=m_pPlayer;
	//if(player->m_url.strTxt&&strstr(player->m_url.strTxt,"AssetsChange"))
	//	int v=0;
	int nCode=player->PushData((XU8*)pData,l,gcEnter,bAttach);
	player->AS3_OnProgress(m_bImage?m_pStream->GetSize():0);
	if(player->rootObject&&!player->m_bHold)
	{
		if (!player->m_bLoadedOK&&(player->pASSuper||player->m_bInitOK))//!bStream||((player->pASSuper||(player->m_bInitOK&&!player->bAS3))&&!player->m_bLoadedOK))
		{
			void*pSuper=NULL;
			if(player->pASSuper||player->ScriptComplete())
				pSuper=player->rootObject->AS3CreateInst(player->pASSuper,XTRUE);
			if(player->DrawFrame(0,false,false)==ScriptPlayer::playOK&&player->bAS3)
			{
				player->m_bLoadedOK=true;
				//if(player->pASSuper||(player->m_bInitOK&&!player->bAS3))
				{
					
					player->rootObject->AS3ConstructInst(pSuper);
					if(player->rootObject->pASObject)
						AddChildRef((ScriptObject*)player->rootObject->pASObject);
					if(!player->m_bInitSet)
					{
						player->AS3_OnInit();

						player->m_pAVM2->OnEvent(XAS3_EVENT_added,false,player->rootObject->pASObject,player->m_pCodeContext);
						if(player->display->root.IsParentOf(player->rootObject))
							player->m_pAVM2->OnEvent(XAS3_EVENT_addedToStage,false,player->rootObject->pASObject,player->m_pCodeContext);
						player->m_pAVM2->OnEvent(XAS3_EVENT_complete,false,player->rootObject->pASObject,player->m_pCodeContext);
						//this->AS3_OnAddToStage();
					}
					player->m_bAS3Frame0=false;
					player->m_bNeedCallFrame=false;
					player->rootObject->CallFrame(0);
				}
			}				
		}
		//player->rootObject->AS3CreateInstance(NULL,XTRUE);
	}
	if(nCode==ScriptThread::notSupport)
	{
		player->stream->scriptPlayer=XNULL;
		player->m_bCanceled=true;
	}
}

void LoaderObject::LoadBytes()//AS3_loadBytes(ByteArrayObject*bytes,LoaderContextObject*pContext)
{
	//Add your act code here...
	//XSWFPlayer::m_pInstance->Trace(XString16("---Load Bytes 111"));
	if(m_bCache)
	{

		/*{
			XString16 ee("************Data From cache CACHE Begin:");
			ee+=XString16(StUTF16String(m_url).c_str());
			XString16 strInt;
			strInt.FromInt((XU32)this,XTRUE);
			ee+=strInt;
			XSWFPlayer::m_pInstance->Trace(ee);
		}*/

		ShellToplevel*top=(ShellToplevel*)toplevel();
		if(!((ShellToplevel*)toplevel())->TriggerComplete(m_url,this))
		{
			m_bPush=true;
			top->PushObject(this);
		}
		//ShellToplevel*top=(ShellToplevel*)toplevel();
		//void*ch=top->GetImage(m_url);
		//this->CacheComplete(ch);
		return;
	}
	if(!m_pByteData) return;
//m_bContent=true;
	ByteArrayObject*bytes=this->m_pByteData;
	m_pByteData=NULL;
	if(m_pPlayer)
	{
		ScriptPlayer*player=m_pPlayer;//m_pObject->character->player;
		if(player)
		{
			//player->AS3Unload();
			int l=bytes->GetLength();
			if(l<=0)
				return;
				//toplevel()->throwArgumentError(kInvalidArgumentError,INTString(l));
			_XStreamData stream;
			stream.nLoadedBytes=l;
			stream.nTotalBytes=l;
			stream.pLoader=this;
			stream.resize=false;
			stream.scriptPlayer=player;
			player->stream=&stream;
			
			{
				uint8_t *gzdata = bytes->GetByteArray().GetBuffer();
#ifdef _WINEMU_DEBUGFILE
				//if(l>20480)
				{
					ShellCore*c=(ShellCore*)core();
					XString8 m_strFile=c->GetPlayer()->m_strWorkPath.strTxt;
					m_strFile+="NO_AD\\";
					
					XFile::CreateFolder(m_strFile);
					//if(l<20480)
						m_strFile+=l;
					int le=0; 
					const char*appName=c->GetPlayer()->player->GetAppName(le);
					if(appName)
					{
						m_strFile+=XString8(appName,le+4);
						//m_strFile+=".swf";
						m_file.Open(m_strFile,XFile::XCREATE|XFile::XWRITE|XFile::XBINARY);
						m_file.Write(gzdata,l);
						m_file.Close();
						player->m_savedUrl=m_strFile;
						player->m_savedUrl.ToString(XTRUE);
					}

				}
#endif
					//mmfx_new_array( uint8_t, l );
				//bytes->ReadBytes(gzdata,l);
				switch(gzdata[0])
				{
//#if (__CORE_VERSION__>=0x02070200)
				case 0x30:
						 gzdata+=68;
						 l-=68;
						 break;
//#endif
				case 'Z':
				case 'C':
				case 'F':
				case 'S':
					//((ShellToplevel*)toplevel())->RemoveImage(m_url);
					//((ShellToplevel*)toplevel())->RemoveImage(m_url);
					player->m_contentType="application/x-shockwave-flash";
						//XContentType::_strTypeList[XContentType::
						//XET::_strTypes[XET::CANY
						//application/x-shockwave-flash
					break;
				case 'p':
					player->m_contentType="image/png";
					player->AS3InitImageLoader();
					player->scriptLen=l;
					break;
				case 'j':
					player->m_contentType="image/jpg";
					player->AS3InitImageLoader();
					player->scriptLen=l;
					break;
				case 'g':
					player->m_contentType="image/gif";
					player->AS3InitImageLoader();
					player->scriptLen=l;
					break;
				default:
					player->AS3InitImageLoader();
					player->scriptLen=l;
					break;
				}
				PushData(gzdata,l,NULL,bytes->IsAttach(),false);
				/*player->PushData(gzdata,l,NULL,bytes->IsAttach());
				if(player->rootObject)
				{
					if(player->DrawFrame(0,false,false)==ScriptPlayer::playOK&&player->bAS3)
					{
						player->m_bLoadedOK=true;
						void*pSuper=player->rootObject->AS3CreateInst(player->pASSuper,XTRUE);
						player->rootObject->AS3ConstructInst(pSuper);
						if(!player->m_bInitSet)
						{
							player->AS3_OnInit();

							player->m_pAVM2->OnEvent(XAS3_EVENT_added,false,player->rootObject->pASObject,player->m_pCodeContext);
							if(player->display->root.IsParentOf(player->rootObject))
								player->m_pAVM2->OnEvent(XAS3_EVENT_addedToStage,false,player->rootObject->pASObject,player->m_pCodeContext);
							player->m_pAVM2->OnEvent(XAS3_EVENT_complete,false,player->rootObject->pASObject,player->m_pCodeContext);
							//this->AS3_OnAddToStage();
						}
						player->m_bAS3Frame0=false;
						player->rootObject->CallFrame(0);
					}				
					//player->rootObject->AS3CreateInstance(NULL,XTRUE);
				}*/
				//player->AS3_OnProgress(m_bImage?m_pStream->GetSize():0);
				//XSWFPlayer::m_pInstance->Trace(XString16("---Load Bytes 222"));
				player->m_pLoaderInfo->m_bytes=bytes;

				player->AS3Complete(true);
				if(player->IsImage())
					player->AS3_OnInit();
				//XSWFPlayer::m_pInstance->Trace(XString16("---Load Bytes 333"));
				player->AS3_OnComplete();
				//XSWFPlayer::m_pInstance->Trace(XString16("---Load Bytes 444"));
				
				//XSWFPlayer::m_pInstance->Trace(XString16("---Load Bytes 444"));
				//if(player->rootObject->pASObject)
				//	AddChildRef(player->rootObject->pASObject);
				//mmfx_delete_array(gzdata);
			}
			player->stream=NULL;
		}
		
		CheckAS3();
	}
	bytes->UnLock();
	//XSWFPlayer::m_pInstance->Trace(XString16("---Load Bytes 444"));
}

void LoaderObject::CheckAS3()
{
	if(m_pPlayer&&!m_pPlayer->bAS3)
	{
		if(m_pPlayer->bScript)
		{
			m_pPlayer->rootObject->FreeChildren();
			m_pPlayer->StopPlay();
		}
	}
}

bool LoaderObject::CreateObject()
{
	if(!m_pObject)
	{
		this->CreateNewObject(loaderChar);
		
	}
	if(m_pContentLoaderInfo==NULL)
	{
		LoaderInfoClass*pClass=((ShellToplevel*)toplevel())->getLoaderInfoClass();
		m_pContentLoaderInfo=pClass->CreateLoaderInfo(m_pPlayer,m_codeContext);
		m_pContentLoaderInfo->m_pLoader=this;
		//((ShellToplevel*)toplevel())->PushObject(m_pContentLoaderInfo);
	}
	return true;
}

bool LoaderObject::CreateContent(LoaderContextObject*code)
{
	//code=NULL;
	if(!m_pPlayer)
	{
		ShellCore*c=(ShellCore*)core();
		XSWFPlayer*pl=c->GetPlayer();
		ScriptPlayer*pRoot=new ScriptPlayer(XTRUE,XFALSE);
		pRoot->splayer=pl;
		pRoot->display=&pl->display;
		pRoot->player=pRoot;
		
		//ShellCodeContext*pContext=NULL;
		void* pApp=NULL;
		if(code&&code->GetApplicationDomain())
		{
			pApp=code->GetApplicationDomain();
			//pDomainEnv=code->GetApplicationDomain()->m_pDomainEnv;
			//pContext=code->GetApplicationDomain()->m_pCodeContext;
		}
		else
		{
			
			ShellCodeContext*codeContext=(ShellCodeContext*)c->codeContext();
			if(codeContext)
			{
				pApp=((ShellToplevel*)toplevel())->getApplicationDomainClass()->CreateAppDomain(codeContext->m_pAppDomain,NULL,toplevel());
				((ApplicationDomainObject*)pApp)->AS3_constructor(codeContext->m_pAppDomain);
				if(code)
				code->SetApplication((ApplicationDomainObject*)pApp);
			}
			
			//	pApp=codeContext->m_pAppDomain;
			//pApp=c->GetShellContext()->m_pAppDomain;
		}
		if(this->m_codeContext&&this->m_codeContext->m_pRoot)
		{
			pRoot->m_loadUrl=m_codeContext->m_pRoot->m_loadUrl;
		}
		else
			pRoot->m_loadUrl=pl->strURL;
		pRoot->InitAS3(NULL,this,pApp,m_pContentLoaderInfo);
		pRoot->InitRoot(false);
			//((avmplus::ScriptObject*)pInstance)->Inc
			//pRoot->rootObject->pASObject=pInstance;
		pRoot->SetDisplay(&pl->display,false);
		//m_pCodeContext=(ShellCodeContext*)m_pPlayer->m_pContext;
		//ShellToplevel*top=(ShellToplevel*)toplevel();
		//m_pLoaderInfo=top->getLoaderInfoClass()->CreateLoaderInfo(m_pPlayer,m_pCodeContext);
		//top->PushObject(m_pLoaderInfo);
		//if(m_pCodeContext)
		//{
		//	m_pAppDomain=m_pCodeContext->m_pAppDomain;
		//	m_pSecurityDomain=m_pCodeContext->m_pSecurityDomain;
			//m_pLoaderInfo=m_pCodeContext->m_pLoaderInfo;
		//}
		m_pPlayer=pRoot;
		//m_codeContext=(ShellCodeContext*)m_pPlayer->m_pContext;
		c->GetPlayer()->AS3PushPlayer(m_pPlayer);
		//((ShellToplevel*)toplevel())->PushObject(m_pLoaderInfo);
		m_pContentLoaderInfo->SetCodeContext(pRoot->m_pCodeContext);//m_pCodeContext=pRoot->m_pCodeContext;
		//pRoot->m_pCodeContext->m_pLoaderInfo=m_pContentLoaderInfo;
		pl->display.AS3PlaceObject(m_pObject,m_pPlayer->rootObject,-1);
		m_pContentLoaderInfo->PushPlayer(m_pPlayer);
	}
	//m_pContentLoaderInfo->m_pPlayer=m_pPlayer;
	return true;
}


void LoaderObject::AS3_unload()
{
	unload(true);
}
void LoaderObject::unload(bool bAS3)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	//if(m_url)
	//{
	//	StUTF8String ss(m_url);
	//	if(strstr(ss.c_str(),"dailyRewardBtn.png"))
	//	{
	//		int v=0;
	//	}
	//	//	int v=0;
	//}
	//XSWFPlayer::m_pInstance->Trace(XString16("---unload begin"));
	//if(m_bCache) return;
	/*XString16 ee("************Unload Cache Loader:");
	ee+=XString16(StUTF16String(m_url).c_str());
	XString16 strInt;
	strInt.FromInt((XU32)this,XTRUE);
	ee+=strInt;
	XSWFPlayer::m_pInstance->Trace(ee);*/
	if(m_bPush)
	{
		((ShellToplevel*)toplevel())->RemoveObject(this);
		m_bPush=false;
	}
	m_pContent = NULL;
	//this->AS3_close();
//#if (__CORE_VERSION__>=0x02077000)
	int nLoadID=m_nLoadID;
	m_nLoadID=0;
	Close();
	m_nLoadID=nLoadID;
//#else
//	Close();
//#endif
	if(m_bCache)
	{
		
		((ShellToplevel*)toplevel())->DisConnect(m_url,this);
	}
	m_pByteData=NULL;
	m_url=NULL;
	m_bImage=false;
	m_bCache=false;
	m_bFirst=true;
	if(m_pPlayer&&m_pPlayer->rootObject)//m_pObject&&m_bContent)
	{
		/*_XSObject*parent=m_pPlayer->rootObject->parent;
		if(parent&&parent!=m_pObject)//&&m_pObject->parent!=NULL)
			//toplevel()->throwArgumentError(kInvalidArgumentError,core()->knull);
		{
			//m_pContentLoaderInfo->m_pLoader=NULL;
			//m_pPlayer->AS3Unload(false);
			m_pContentLoaderInfo=NULL;
			m_pPlayer=NULL;
		}
		else*/
		{
			/*if(m_pPlayer->rootObject->pASObject)
			{
				((DisplayObjectObject*)m_pPlayer->rootObject->pASObject)->m_pLoaderInfo=m_pContentLoaderInfo;
			}*/
			if(m_pChildren!=NULL)
				m_pChildren->clear();
			if(m_pObject)
				m_pObject->FreeChildren(false);
			//if(bAS3)
			//if(!bAS3)
				m_pContentLoaderInfo->Reset(!bAS3);
			/*else
			{
				m_pContentLoaderInfo->Reset(!bAS3);
				m_pContentLoaderInfo->ClearEventInfo();
				m_pContentLoaderInfo->m_pLoader=NULL;
				m_pContentLoaderInfo = NULL;
				LoaderInfoClass*pClass=((ShellToplevel*)toplevel())->getLoaderInfoClass();
				m_pContentLoaderInfo=pClass->CreateLoaderInfo(m_pPlayer,m_codeContext);
				m_pContentLoaderInfo->m_pLoader=this;
			}*/
			//m_pPlayer->AS3Unload(false);
			//if(!bAS3)
			m_pPlayer=NULL;
		}
		/*DisplayObjectObject*obj=(DisplayObjectObject*)m_pObject->pASObject;
				
		if(obj)
		{
			((ShellToplevel*)toplevel())->RemoveObject(obj);
			//obj->m_pLoadInfo=this->m_pLoaderInfo;
		}*/
		/*if(m_pPlayer->IsImage())
		{
			//m_pPlayer->AS3_OnUnload();
			m_pLoaderInfo->OnEvent(avmshell::XAS3_EVENT_unload,false);
			m_pPlayer->AS3Unload(true);
			//toplevel()->throwArgumentError(kInvalidArgumentError,core()->kEmptyString);
			return;
		}*/
		
		
		/*m_pLoaderInfo->OnEvent(avmshell::XAS3_EVENT_unload,false);
		//m_pPlayer->AS3_OnUnload();
		m_pObject=NULL;
		this->m_bContent=false;
		m_pPlayer=NULL;
		//m_pCodeContext=NULL;
		m_pAppDomain=NULL;//m_pCodeContext->m_pAppDomain;
		m_pSecurityDomain=NULL;//m_pCodeContext->m_pSecurityDomain;
		m_pLoaderInfo=NULL;//m_pCodeContext->m_pLoaderInfo;
		CreateLoaderContext();*/
		/*if(m_pObject==NULL)
		{
			DisplayObjectObject::CreateNewObject(rootChar);
			//this->IncrementRef();
			m_pPlayer=m_pObject->character->player;
			m_pCodeContext=(ShellCodeContext*)m_pPlayer->m_pContext;
			m_pAppDomain=m_pCodeContext->m_pAppDomain;
			m_pSecurityDomain=m_pCodeContext->m_pSecurityDomain;
			m_pLoaderInfo=m_pCodeContext->m_pLoaderInfo;
			//m_codeContext=(ShellCodeContext*)m_pPlayer->m_pContext;
			((ShellCore*)core())->GetPlayer()->AS3PushPlayer(m_pPlayer);
			((ShellToplevel*)toplevel())->PushObject(m_pLoaderInfo);
		}*/
		//m_pObject->character->player->AS3Unload();
	}
	else //if(m_pContentLoaderInfo!=null)
	{
		m_pContentLoaderInfo->Reset(!bAS3);
	}

	
}

//new adds
UncaughtErrorEventsObject* LoaderObject::AS3_uncaughtErrorEvents_get()
{
	if(m_pErrorEvents==XNULL)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		UncaughtErrorEventsClass*pClass=(UncaughtErrorEventsClass*)top->getBuiltinExtensionClass(abcclass_flash_events_UncaughtErrorEvents);
		m_pErrorEvents = pClass->CreateObject();
	}
	return m_pErrorEvents;
}

void LoaderObject::AS3_loadFilePromise(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{

}
//void LoaderObject::AS3_unloadAndStop(bool gc)
//{
//
//}
//new adds end


}