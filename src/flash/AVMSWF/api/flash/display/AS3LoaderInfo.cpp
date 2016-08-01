#include "StdAfxflash.h"
#include "splay.h"
#include "splayer.h"
#include "ShellCore.h"
using namespace avmplus;
#include "avm2.h"

#include "AS3LoaderInfo.h"
#include "AS3ApplicationDomain.h"
#include "AS3Loader.h"
#include "AS3UncaughtErrorEvents.h"

namespace avmshell{
LoaderInfoClass::LoaderInfoClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())LoaderInfoObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* LoaderInfoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LoaderInfoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

void LoaderInfoObject::PushPlayer(ScriptPlayer*player)
{
	//player->pNextInfo=m_pInfoPlayer;
	//if(player == NULL)return;
	//AvmAssertMsg(m_pInfoPlayer==NULL||m_pInfoPlayer==player,"Multi set player!");
	m_pInfoPlayer=player;
}

void LoaderInfoObject::Reset(bool bClear)
{
	if(m_pInfoPlayer)
	{
		if(bClear)
		{
			if(m_pInfoPlayer->IsImage()||!m_pInfoPlayer->bScript||!m_pInfoPlayer->rootObject)
				((ShellCore*)core())->GetPlayer()->AS3ReleasePlayer(m_pInfoPlayer);
			else if(!m_pInfoPlayer->rootObject->pASObject)
			{
				((ShellCore*)core())->GetPlayer()->AS3ReleasePlayer(m_pInfoPlayer);
				//int v=0;
			}
			else if(m_pInfoPlayer->m_bDummy)//>m_nClasses<=1)
			{
				DisplayObjectObject*pDisp=(DisplayObjectObject*)m_pInfoPlayer->rootObject->pASObject;
				pDisp->m_pFreePlayer = m_pInfoPlayer;
			}
		}
		else
		{
			//if(m_pInfoPlayer->IsImage()||!m_pInfoPlayer->bScript||!m_pInfoPlayer->rootObject)

			//	((ShellCore*)core())->GetPlayer()->AS3ReleasePlayer(m_pInfoPlayer);
			/*if(!m_pInfoPlayer->rootObject||!m_pInfoPlayer->rootObject->pASObject)
			{
				//((ShellCore*)core())->GetPlayer()->AS3ReleasePlayer(m_pInfoPlayer);
				//int v=0;
			}
			else if(m_pInfoPlayer->m_bDummy||m_pInfoPlayer->IsImage()||!m_pInfoPlayer->bScript)//>m_nClasses<=1)
			{
				DisplayObjectObject*pDisp=(DisplayObjectObject*)m_pInfoPlayer->rootObject->pASObject;
				pDisp->m_pFreePlayer = m_pInfoPlayer;
			}*/
			/*else
			{
				//m_pInfoPlayer->m_pAVM2->PushObject(this);
				((ShellToplevel*)toplevel())->PushObject(this);
				//pAVM2->PushObject(player->m_pLoaderInfo)
			}*/
		}
		//if(m_pInfoPlayer->
	}
	m_pInfoPlayer = NULL;
	m_params = NULL;
	m_loadBy = NULL;
}

void LoaderInfoObject::Release()
{
	if(m_pInfoPlayer)
	{
		if(IsRunning())
		{
			if(m_pInfoPlayer)
			{
				//ScriptPlayer*next=m_pInfoPlayer->pNextInfo;
/*#if (__CORE_VERSION__>=0x02075000)
				if(m_pInfoPlayer->m_nABCID<=0)
					((ShellCore*)core())->GetPlayer()->AS3ReleasePlayer(m_pInfoPlayer);
#else*/
				((ShellCore*)core())->GetPlayer()->AS3ReleasePlayer(m_pInfoPlayer);
//#endif
				//m_pInfoPlayer=next;
			}
			//m_pPlayer->m_bCanceled=XTRUE;
			//((ShellCore*)core())->GetPlayer()->AS3ReleasePlayer(m_pPlayer);
		}
		m_pInfoPlayer=NULL;
	}
	EventDispatcherObject::Release();
}

LoaderInfoObject* LoaderInfoClass::CreateLoaderInfo(void*player,ShellCodeContext*codeContext)
{
	//Atom a=atom();
	LoaderInfoObject*pObj=(LoaderInfoObject*)createInstance(ivtable(),prototype);
		//AvmCore::atomToScriptObject(construct(0,&a));
	//if(player)
	//	((ScriptPlayer*)player)->pNextInfo=pObj->m_pInfoPlayer;

	pObj->m_pInfoPlayer=(ScriptPlayer*)player;
	//if(player)
	//	pObj->m_pInfoPlayer->pNextInfo=NULL;
	pObj->SetCodeContext(codeContext);
	//pObj->m_pCodeContext=codeContext;
	//pObj->m_pRoot=(ScriptPlayer*)player;
	return pObj;
}

LoaderInfoObject::LoaderInfoObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pInfoPlayer=NULL;
	//((ShellToplevel*)toplevel())->PushObject(this);
}


//////////////////////////////////////////////////////////
//Native Method start...
uint32 LoaderInfoObject::AS3_actionScriptVersion_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	return 3;
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

ByteArrayObject* LoaderInfoObject::AS3_bytes_get()
{

	/*if(m_bytes!=NULL)
	{
		int l=m_bytes->GetLength();
		if(l>=400000&&l<430000)
			int v=0;
	}*/
	return m_bytes;
}

void LoaderInfoObject::ClearEventInfo()
{
	if(this->m_pEventInfo)
		EventDispatcherObject::FreeEventInfo();
}

ApplicationDomainObject* LoaderInfoObject::AS3_applicationDomain_get()
{
	//Add your act code here...

	/*ShellCodeContext*pContext=XNULL;
	if(this->m_pPlayer)
		pContext=(ShellCodeContext*)m_pPlayer->m_pContext;
	else
		pContext=((ShellCore*)core())->GetCodeContext();
		//(ShellCodeContext*)core()->codeContext();
	return ((ScriptObject*)(ApplicationDomainObject*)pContext->m_pAppDomain)->atom();*/

	//if(m_pCodeContext)
	//	return m_pCo
	/*if(m_pPlayer)
	{
		return ((ShellCodeContext*)m_pPlayer->m_pContext)->m_pAppDomain;
	}*/
	return m_codeContext->m_pAppDomain;

	//return kAvmThunkUndefined;//Modify this please!
}

uint32 LoaderInfoObject::AS3_bytesLoaded_get()
{
	//Add your act code here...
	//return 10;

	if(m_pInfoPlayer)
	{
		uint32 t=m_pInfoPlayer->len;
		if(t>=m_pInfoPlayer->scriptLen&&
		   m_pInfoPlayer->m_strWaitClass.strTxt)
		   t=m_pInfoPlayer->scriptLen-1;
		return t;
	}
	return kAvmThunkUndefined;//Modify this please!
}

uint32 LoaderInfoObject::AS3_bytesTotal_get()
{
	//Add your act code here...
	if(m_pInfoPlayer)
	{
		//if(m_pInfoPlayer->scriptLen!=m_pInfoPlayer->len)
		//	int v=0;
		return m_pInfoPlayer->scriptLen;
	}
	return kAvmThunkUndefined;//Modify this please!
}

bool LoaderInfoObject::AS3_childAllowsParent_get()
{
	//Add your act code here...
	return true;
	//return kAvmThunkUndefined;//Modify this please!
}

DisplayObjectObject* LoaderInfoObject::AS3_content_get()
{
	//Add your act code here...
	//return true;
	if(m_pInfoPlayer)
	{
		return (DisplayObjectObject*)m_pInfoPlayer->rootObject->pASObject;
		//return p->atom();
	}
	return NULL;//Modify this please!
}

Stringp LoaderInfoObject::AS3_contentType_get()
{
	//Add your act code here...
	if(m_pInfoPlayer)
	{
		//ScriptObject*pObj=(ScriptObject*)
		//	m_pPlayer->rootObject->pASObject;
		//return pObj->atom();
		return core()->newStringLatin1(m_pInfoPlayer->m_contentType.strTxt);
	}
	return core()->kEmptyString;//Modify this please!
}

double LoaderInfoObject::AS3_frameRate_get()
{
	//Add your act code here...
	if(m_pInfoPlayer)
	{
		return (double)(m_pInfoPlayer->frameRate>>16);
	}
	return kAvmThunkUndefined;//Modify this please!
}

int LoaderInfoObject::AS3_height_get()
{
	//Add your act code here...
	if(m_pInfoPlayer)
	{
		if(m_pInfoPlayer->IsImage()&&m_pInfoPlayer->rootObject)
		{
			if(m_pInfoPlayer->rootObject->character)
				return RectHeight(&m_pInfoPlayer->rootObject->character->rbounds)/20;
				//m_pInfoPlayer->rootObject->character->type==loadImageChar)
			//return RectWidth(m_pInfoPlayer->rootObject->character->rbounds)/20;
		}
		
		{
			return RectHeight(&m_pInfoPlayer->frame)/20;
		}
	}
	return 0;
}

LoaderObject* LoaderInfoObject::AS3_loader_get()
{
	//Add your act code here..
	if(m_pLoader==NULL)
	{
		LoaderClass*pClass=((ShellToplevel*)this->toplevel())->getLoaderClass();
		m_pLoader=pClass->CreateLoader();
	}
	//	int v=0;
	return m_pLoader;
}

Stringp LoaderInfoObject::AS3_loaderURL_get()
{
	//Add your act code here...
	if(this->m_codeContext)//m_pPlayer)
	{
		
		ScriptPlayer*player=m_codeContext->m_pRoot;//this->m_pPlayer;//m_pCodeContext->m_pRoot;
		const char*url=player?player->m_loadUrl.strTxt:((ShellCore*)core())->GetPlayer()->strURLBase.strTxt;
		//if(m_strLoaderURL==XNULL)
		{
			XString8 s;
			s.ConvertFrom(url,XCHARSET_GB2312);
			if(s.Find("://",0)<0)
			{
				int i,il=s.GetLength();
				char*buf=s.GetData();
				for(i=0;i<il;i++)
				{
					if(buf[i]=='/') buf[i]='\\';
				}
				Stringp fileurl=core()->newConstantStringLatin1("file://");
				Stringp url=core()->newStringUTF8(s);
				return String::concatStrings(fileurl,url);
			}
			else
				return core()->newStringUTF8(s);
		}
		//return m_strLoaderURL;
	}
	//return m_strLoaderURL;
	return core()->kEmptyString;//kAvmThunkUndefined;//Modify this please!
}

AvmBox LoaderInfoObject::AS3_parameters_get()
{
	//Add your act code here...
	const char* _iStrList[]=
	{
		"align",
		"allowScriptAccess",
		"base",
		"bgcolor",
		"contextType",
		"height",
		"loop",
		"play",
		"pluginsage",
		"quality",
		"type",
		"width",
	};
	if(m_pInfoPlayer&&!m_params)
	{
//		if(m_pInfoPlayer!=((ShellCore*)core())->GetPlayer()->player)
//			int v=0;
		if(m_loadBy)
		{
			m_params=AvmCore::atomToScriptObject(m_loadBy->AS3_parameters_get());
		}
		else
		{
			ObjectClass*pClass=toplevel()->objectClass;
			ScriptObject*pObj=pClass->construct();
			int i,nSize=m_pInfoPlayer->m_heads.GetSize()-1;		
			AvmCore*cc=core();
			XArray<XXVar>& hs=m_pInfoPlayer->m_heads;
			XSortConstStr sorts(_iStrList,sizeof(_iStrList)/sizeof(char*));
			for(i=0;i<nSize;i+=2)
			{
				//if(strcmp(hs[i].strTxt,"contextType")==0)
				//	continue;
				if(sorts.Index(hs[i].strTxt,XSS_NOCASE)>=0)
					continue;
				Stringp strKey=cc->internStringUTF8(hs[i].strTxt);
				Stringp strValue=cc->newStringUTF8(hs[i+1].strTxt);
				pObj->setStringProperty(strKey,strValue->atom());
			}
				//pObj->setStringProperty(cc->internStringUTF8("debug"),cc->intToAtom(1));
			m_params=pObj;
			return pObj->atom();		
		}
	}
	if(m_params) return m_params->atom();
	return kAvmThunkUndefined;//Modify this please!
}

bool LoaderInfoObject::AS3_parentAllowsChild_get()
{
	//Add your act code here...
	return true;
	//return kAvmThunkUndefined;//Modify this please!
}

bool LoaderInfoObject::AS3_sameDomain_get()
{
	//Add your act code here...
	return true;
	//return kAvmThunkUndefined;//Modify this please!
}

EventDispatcherObject* LoaderInfoObject::AS3_sharedEvents_get()
{
	//Add your act code here...
	//if(m_pPlayer)
	{
		return this;//(EventDispatcherObject*)m_pPlayer->rootObject->pASObject;
		//return p->atom();
	}
	return NULL; 
		//nullObjectAtom;//Modify this please!
}

uint32 LoaderInfoObject::AS3_swfVersion_get()
{
	//Add your act code here...
	if(m_pInfoPlayer)
		return m_pInfoPlayer->version;
	return 0;//Modify this please!
}

Stringp LoaderInfoObject::AS3_url_get()
{
	//Add your act code here...
	//if(m_pInfoPlayer)
	{
		
		ScriptPlayer*player=m_pInfoPlayer;//m_pCodeContext->m_pRoot;
//#if (__CORE_VERSION__>=0x02071000)
		const char*url=player?(player->m_url.IsNull()?player->m_loadUrl.strTxt:player->m_url.strTxt):((ShellCore*)core())->GetPlayer()->strURLBase.strTxt;
		if(url==NULL||url[0]==0)
			url=((ShellCore*)core())->GetPlayer()->strURLBase.strTxt;
//#else
//		const char*url=player?player->m_url.strTxt:((ShellCore*)core())->GetPlayer()->strURLBase.strTxt;
//#endif
		//if(m_strURL==XNULL)
		{
			XString8 s;
			s.ConvertFrom(url,XCHARSET_GB2312);
			if(s.Find("://",0)<0)
			{
				Stringp strURL=core()->newConstantStringLatin1("file://");
				int i,il=s.GetLength();
				char*buf=s.GetData();
				for(i=0;i<il;i++)
				{
					if(buf[i]=='/') buf[i]='\\';
				}
				Stringp url=core()->newStringUTF8(s);
				return String::concatStrings(strURL,url);
			}
			else
				return core()->newStringUTF8(s);
		}
	}	
		//return core()->newStringLatin1(m_pPlayer->m_url.strTxt);
	return core()->kEmptyString;
	//return kAvmThunkUndefined;//Modify this please!
}

int LoaderInfoObject::AS3_width_get()
{
	//Add your act code here...
	if(m_pInfoPlayer)
	{
		if(m_pInfoPlayer->IsImage()&&m_pInfoPlayer->rootObject)
		{
			if(m_pInfoPlayer->rootObject->character)
				return RectWidth(&m_pInfoPlayer->rootObject->character->rbounds)/20;
				//m_pInfoPlayer->rootObject->character->type==loadImageChar)
			//return RectWidth(m_pInfoPlayer->rootObject->character->rbounds)/20;
		}
		//else//if(m_pInfoPlayer)
		{
			return RectWidth(&m_pInfoPlayer->frame)/20;
		}
	}
	return 0;//Modify this please!
}

EventDispatcherObject* LoaderInfoObject::GetParentObject(bool bSeek)
{
	if(!m_pInfoPlayer) return NULL;
	SObject*m_pObject=m_pInfoPlayer->rootObject;
	if(!m_pObject||!m_pObject->parent) return NULL;
	SObject*p=m_pObject->parent;
	if(!bSeek) return (EventDispatcherObject*)p->pASObject;
	while(p)
	{
		if(p->pASObject) return (EventDispatcherObject*)p->pASObject;
		p=p->parent;
		
	}
	return NULL;
}

//new adds
LoaderInfoObject* LoaderInfoClass::AS3_getLoaderInfoByDefinition(Atom object)
{
	return NULL;
}
//new adds end

//new adds
//ByteArrayObject* LoaderInfoObject::AS3_bytes_get()
//{
//	return NULL;
//}
Atom LoaderInfoObject::AS3_childSandboxBridge_get()
{
	return kAvmThunkUndefined;
}
void LoaderInfoObject::AS3_childSandboxBridge_set(Atom childSandboxBridge)
{

}
bool LoaderInfoObject::AS3_isURLInaccessible_get()
{
	return false;
}
Atom LoaderInfoObject::AS3_parentSandboxBridge_get()
{
	return kAvmThunkUndefined;
}
void LoaderInfoObject::AS3_parentSandboxBridge_set(Atom parentSandboxBridge)
{

}
UncaughtErrorEventsObject* LoaderInfoObject::AS3_uncaughtErrorEvents_get()
{
	if(!m_pErrorEvent)
	{
		UncaughtErrorEventsClass*pClass=((ShellToplevel*)toplevel())->getUncaughtErrorEventsClass();
		m_pErrorEvent=pClass->CreateObject();
	}
	return m_pErrorEvent;
}
//new adds end


}
