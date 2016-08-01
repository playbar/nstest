#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3ExternalInterface.h"
#include "splayer.h"
#include "XFlashView.h"

//#define _JAVASCRIPT_

namespace avmshell{
ExternalInterfaceClass::ExternalInterfaceClass(VTable* cvtable):ClassClosure(cvtable)
{
	
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ExternalInterfaceClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	
	return new (core()->GetGC(), ivtable->getExtraSize()) ExternalInterfaceObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ExternalInterfaceObject::ExternalInterfaceObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	
}
//////////////////////////////////////////////////////////
//Native Method start...
void ExternalInterfaceClass::AS3_addCallback(Stringp name,FunctionObject*fun)
{
	//Add your act code here...
	if(!name||!fun)
		toplevel()->throwArgumentError(kInvalidArgumentError,"null");
	HeapHashtable*extFuns=((ShellToplevel*)toplevel())->m_extFuns;
	//if(extFuns==NULL)
	//	extFuns=new (core()->GetGC())HeapHashtable(core()->GetGC());
	if(extFuns)
		extFuns->add(name->atom(),fun->atom());
}

static const char*TrimLeft(const char*ch,int l)
{
	while(*ch)
	{
		if(!l) break;
		if(*ch>' ') return ch;
		ch++;
		l--;
	}
	return NULL;
}

static const char*TrimLeft(const char*ch)
{
	while(*ch)
	{
		if(*ch>' ') return ch;
		ch++;
	}
	return NULL;
}

static int TrimRight(const char*cn,int l)
{
	while(l)
	{
		if(cn[l-1]>' ')
			return l;
		l--;
	}
	return 0;
}

static const char* EndOf(const char*cnt,int&l,char c)
{
	l--;
	while(l>=0)
	{
		if(cnt[l]==c)
		{
			//l++;
			return cnt+l;
		}
		l--;
	}
	return NULL;
}

static const char* StartOf(const char*cnt,int&l,char c)
{
	for(int i=0;i<l;i++)
	{
		if(cnt[i]==c)
		{
			l=i;
			return cnt+i;
		}
	}
	return NULL;
}

static int NextOf(const char*&cnt,int l)
{
	int i;
	for(i=0;i<l;i++)
	{
		if(cnt[i]==',') break;
		if(cnt[i]=='[')
		{
			int iStack=1;
			while(i<l)
			{
				if(cnt[i]==']')
				{
					iStack--;
					if(!iStack)
						break;
				}
			}
		}
	}
	cnt+=i+1;
	//i=TrimRight(cnt);
	//cnt=TrimLeft(cnt,i);
	
	return i;
}




Atom ExternalInterfaceClass::SystemVar(XString8&strName,ArrayObject*args)
{
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	XFlashView*pView=(XFlashView*)player->pWindow;
	XString8 strValue;
	XString8Array strArgs;
	if(args)
	{
		for(int i=0;i<args->getDenseLength();i++)
		{
			Stringp s=c->string(args->getIntProperty(i));
			StUTF8String ss(s);
			strArgs.Add(ss.c_str());
		}
	}
	if(pView->GetVarOf(strName,strArgs,strValue))
	{
		Stringp s=c->newStringUTF8(strValue,strValue.GetLength());
		return s->atom();
	}
	Stringp s=c->newStringUTF8(strName,strName.GetLength());
	return s->atom();
	//return nullObjectAtom;
}


/*static const char* _strSysFuns[]=
{
	"cookieof",
	"cookies",
};

enum 
{
	XSF_COOKIEOF,
	XSF_COOKIES,
};*/

/*Atom ExternalInterfaceClass::SystemExec(XString8&strKey,ArrayObject*args)
{
	XSortConstStr sort(_strSysFuns,sizeof(_strSysFuns)/sizeof(XPCTSTR));
	int id=sort.Index(strKey,XSS_NOCASE);
	ShellCore*c=(ShellCore*)core();
	if(id>=0)
	{
		
		XSWFPlayer*player=c->GetPlayer();
		XFlashView*pView=(XFlashView*)player->pWindow;
		switch(id)
		{
		case XSF_COOKIEOF:
			if(args&&args->getDenseLength())
			{
				Stringp str=c->string(args->getUintProperty(0));
				StUTF8String ss(str);
				XString8 name(ss.c_str(),ss.length());
				XString8 strCookie;
				if(pView->GetCookieOf(name,strCookie))
				{
					Stringp cookie=c->newStringUTF8(strCookie,strCookie.GetLength());
					return cookie->atom();
				}
			}break;
		case XSF_COOKIES:
			{
				XString8 strCookie;
				if(pView->GetCookies(strCookie))
				{
					Stringp cookie=c->newStringUTF8(strCookie,strCookie.GetLength());
					return cookie->atom();
				}
			}break;
		}
	}
	Stringp s=c->newStringUTF8(strKey,strKey.GetLength());
	return s->atom();
	//return nullObjectAtom;
}*/

Atom ExternalInterfaceClass::SystemCall(XString8&strCall,ArrayObject*callargs)
{
	HeapHashtable*extFuns=((ShellToplevel*)toplevel())->m_extFuns;
	if(extFuns==NULL) return undefinedAtom;	
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	XFlashView*pView=(XFlashView*)player->pWindow;
	int id=strCall.Find(':');
	if(id>=0)
	{
		int idb=strCall.Find('(',id+1),iOff=0;
		XString8 strMethod;
		ArrayObject* callArgs=((ShellToplevel*)toplevel())->arrayClass->newArray(0);
		Atom atom;
		//callArgs->push(&atom,1);
		if(idb>id)
		{
			strCall.Mid(strMethod,id+1,idb-id-1);
			XString8 strArgs;
			int iEnd=strCall.ReverseFind(')');
			if(iEnd>idb+1)
			{
				strCall.Mid(strArgs,idb+1,iEnd-idb-1);
			}
			//strArgs.TrimRight(')');
			if(!strArgs.IsEmpty())
			{
				XString8Array args;
				args.SplitString(strArgs,',');
				for(int i=0;i<args.GetSize();i++)
				{
					XString8&arg=args[i];
					strArgs.Empty();
					Stringp s=NULL;
					arg.TrimLeft();
					XString8 strTmp=arg;
					if(arg[0]=='&')
					{
						arg.Delete(0);
						int nIndex=arg.ToInt();
						if(nIndex>=0&&nIndex<=callargs->getDenseLength())
						{
							atom=callargs->getIntProperty(nIndex);
							
						}
						else
							atom=undefinedAtom;

						callArgs->push(&atom,1);
					}
					else if(pView->GetVarOf(strTmp,args,strArgs))
						s=core()->newStringUTF8(strArgs,strArgs.GetLength());
					else
						s=core()->newStringUTF8(arg,arg.GetLength());
					atom=s->atom();
					callArgs->push(&atom,1);
				}
			}
		}
		else
			strCall.Mid(strMethod,id+1);
		strMethod.TrimLeft();
		if(strMethod[0]=='#'&&idb<id)
		{			
			iOff=1;
			XString8Array args;
			XString8 strArgs,strName=strMethod;
			pView->GetVarOf(strName,args,strArgs);
			args.SplitString(strArgs,',');
			for(int i=0;i<args.GetSize();i++)
			{
				XString8&strTmp=args[i];
				strTmp.TrimLeft('\'');
				strTmp.TrimLeft('"');
				strTmp.TrimRight('\'');
				strTmp.TrimRight('"');
				Stringp s=c->newStringUTF8(strTmp,strTmp.GetLength());
				atom=s->atom();
				callArgs->push(&atom,1);
			}
		}
		idb=strMethod.Find('@');
		Stringp strCall=NULL;
		if(idb<=0)
			strCall=c->newStringUTF8(strMethod.GetData()+iOff,strMethod.GetLength()-iOff);
		else
			strCall=c->newStringUTF8(strMethod.GetData()+iOff,idb-iOff);
		strCall=c->internString(strCall);
		HeapHashtable*extFuns=((ShellToplevel*)toplevel())->m_extFuns;
		atom=extFuns->get(strCall->atom());
		if(!AvmCore::isNullOrUndefined(atom))
		{
			FunctionObject*fun=(FunctionObject*)AvmCore::atomToScriptObject(atom);
			if(callArgs->getDenseLength())
				return fun->AS3_apply(atom,callArgs->atom());
			else
				return fun->AS3_apply(atom,nullObjectAtom);
		}	
	}
	return undefinedAtom;
}

Atom ExternalInterfaceClass::StringAtom(const char*cnt,int l,ArrayObject*args)
{
	enum {ST_NUMBER,ST_STRING,ST_ARRAY};
	if(l==0)
		l=strlen(cnt);

	const char*begin=NULL;
	char  spChar=0,isEmpty=1;
	int iType=ST_NUMBER;
	for(int i=0;i<l;i++)
	{
		
		switch(cnt[i])
		{
		default:
			if(cnt[i]>' ') isEmpty=0;
			break;
		case '@':
		case '#':
		case '$':
			{
				XString8 str(cnt+i,l-i);
				if(str[1]=='X'&&str.GetLength()>10)
				{
					if(str.Compare("$XVAR_CALL",0,10)==0)
						return SystemCall(str,args);
				}
				return SystemVar(str,args);
			}break;
		/*case '@':
			{
				XString8 str(cnt+i+1,l-i-1);
				return SystemExec(str,args);
			}
			break;*/
		case '"':
			iType=ST_STRING;
			begin=cnt+i+1;
			spChar='"';
			l-=(i+1);
			break;
		case '\'':
			iType=ST_STRING;
			begin=cnt+i+1;
			l-=(i+1);
			spChar='\'';
			break;
		case '[':
			iType=ST_ARRAY;
			begin=cnt+i+1;
			l-=(i+1);
			spChar=']';
			break;
		}
		if(spChar) break;
	}	
	AvmCore*c=core();
	switch(iType)
	{
	default:
		return c->newStringUTF8(cnt,l)->atom();
		break;
	case ST_NUMBER:
		{
			Stringp s=c->newStringUTF8(cnt,l);
			double value=MathUtils::convertStringToNumber(s);
			double nan=MathUtils::kNaN;
			if(MathUtils::isNaN(value))//value==nan)
				return s->atom();
			return c->doubleToAtom(value);
		}break;
	case ST_STRING:
		{
			int l2=l;
			const char*end=EndOf(begin,l2,spChar);
			if(end)
			{
				if(end<=begin)
					return c->kEmptyString->atom();
				else
				{
					if(XString8::Compare(begin,"true",0,XTRUE)==0)
						return trueAtom;
					else if(XString8::Compare(begin,"false",0,XTRUE)==0)
						return falseAtom;
					return c->newStringUTF8(begin,l2)->atom();
				}
			}
			else
				return c->newStringUTF8(begin,l)->atom();
				//return c->kEmptyString->atom();
		}break;	
	case ST_ARRAY:
		{
			const char*end=EndOf(begin,l,spChar);
			ArrayObject*pArray=toplevel()->arrayClass->newArray(0);
			if(end&&end>begin)
			{
				for(;;)
				{
					int cl=NextOf(begin,l);
					int nl=TrimRight(begin,cl);
					const char*cnt=TrimLeft(begin,nl);

					Atom atom=nullObjectAtom;
					if(nl==0||!cnt) 
						atom=c->intToAtom(0);
					else
						atom=StringAtom(cnt,nl,NULL);
					pArray->push(&atom,1);
					begin+=(cl+1);
					l-=(cl+1);
					if(l<=0) break;
				}
			}
			return pArray->atom();
		}break;
	}
	/*Stringp strValue=core()->newStringUTF8(cnt,l);
	double value;

		if (inStr->length() == 0) { // toNumber("") should be 0, not NaN
			value = 0;
		} else if (MathUtils::convertStringToDouble(inStr, &value, true)) {
			// nothing to do, value set by side effect.
		} else  {
			value = MathUtils::parseInt(inStr, 0,true); // 0 radix means figure it out from the string.
		}
		return value;*/

	//Stringp str=core()->String
	/*const char*begin=strchr(cnt,'[');
	if(begin)
	{
		const char*end=strchr(begin+1,']');
		if(end)
		{
			XString8Array list;

			list.SplitString(
		}
	}*/
	return c->kEmptyString->atom();
}

static int getLine(const char*ch)
{
	int l=0;
	for(;;)
	{
		switch(ch[l])
		{
		
		case '\r':
		case '\n':
			l++;
		case 0:
			return l;
		}
		l++;
	}
	return l;
}



Atom ExternalInterfaceClass::AS3_call(Stringp name,ArrayObject*args)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(!name)
		toplevel()->throwArgumentError(kInvalidArgumentError,"null");
	HeapHashtable*extFuns=((ShellToplevel*)toplevel())->m_extFuns;
	if(extFuns!=NULL)
	{
		Atom atom=extFuns->get(name->atom());
		if(!AvmCore::isNullOrUndefined(atom))
		{
			FunctionObject*fun=(FunctionObject*)AvmCore::atomToScriptObject(atom);
			if(args)
				return fun->AS3_apply(atom,args->atom());
			else
				return fun->AS3_apply(atom,nullObjectAtom);
		}
	}
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	const char* ret=NULL;
	StUTF8String cmd(name);
//#if (__CORE_VERSION__>=0x02079000)
	XDOMFILTER*pFilter=player->m_pDomFilter;
	if(pFilter)
	{
		
		int i,nc=pFilter->strExtFuns3.GetSize();
		for(i=0;i<nc;i+=2)
		{
			if(pFilter->strExtFuns3[i]==(XPCTSTR)cmd.c_str())
			{
				//m_pDomFilter->strExtFuns2[i+1];
				if(player->pWindow)
				{					
					XString8 strData=pFilter->strExtFuns3[i+1].GetData(),strValue;
					XFlashView*pView=(XFlashView*)player->pWindow;
					XString8Array callArgs;
					if(args&&args->getDenseLength())
					{
						int iSet,iCount=args->getDenseLength();
						for(iSet=0;iSet<iCount;iSet++)
						{
							Stringp s=c->string(args->getUintProperty(iSet));
							StUTF8String us(s);
							callArgs.Add(us.c_str());
						}
					}
					pView->GetVarOf(strData,callArgs,strValue);
					pView->Handle(XCMD_FLASHCOMMAND,XFLASH_POSTDATA,(XU32)strValue.GetData());
				}
				return undefinedAtom;
			}
		}
	}
//#endif
	if(args&&args->getDenseLength())
	{
		Stringp s=c->string(args->atom());
		StUTF8String str(s);
		
		ret=player->ProcessFSCommand((char*)cmd.c_str(),(char*)str.c_str());
	}
	else
		ret=player->ProcessFSCommand((char*)cmd.c_str(),NULL);
	//return undefinedAtom;
	if(ret)
	{
		if(strchr(ret,'\r')!=NULL||strchr(ret,'\n')!=NULL)
		{
			ScriptObject*obj=toplevel()->objectClass->newInstance();
			const char*pData=TrimLeft(ret);
			while(pData)
			{
				int l=getLine(pData);
				int nl=TrimRight(pData,l);
				if(!nl) break;		
				const char*cnt=TrimLeft(pData,nl);
				int l2=nl;
				const char*sp=StartOf(pData,l2,'=');//strchr(pData),'=');
				if(sp)
				{
					int ll=TrimRight(pData,l2);
					int lDot=ll;
					const char*pDot=StartOf(pData,lDot,'.');
					if(pDot)
					{
						//int lDot=pDot-pData;
						Stringp key=c->internString(c->newStringUTF8(pDot+1,ll-lDot-1));
						Stringp keyObj=c->internString(c->newStringUTF8(pData,lDot));
						Atom subAtom=obj->getStringProperty(keyObj);
						ScriptObject*subObj=NULL;
						if(AvmCore::isNullOrUndefined(subAtom))
						{
							subObj=toplevel()->objectClass->newInstance();
							subAtom=subObj->atom();
							obj->setStringProperty(keyObj,subAtom);
						}
						else
							subObj=AvmCore::atomToScriptObject(subAtom);
						Atom	atom=StringAtom(sp+1,nl-l2-1,args);
						subObj->setStringProperty(key,atom);
					}
					else
					{

						Stringp key=c->internString(c->newStringUTF8(pData,ll));
						//ll=nl-l2-1;
						//sp=TrimLeft(sp+1,ll);
						Atom	atom=StringAtom(sp+1,nl-l2-1,args);
						//Stringp value=c->newStringUTF8(sp+1);
						obj->setStringProperty(key,atom);
					}
				}
				else
					StringAtom(pData,l2,args);
				pData+=l;
				pData=TrimLeft(pData);
			}
			/*XString8Array list;
			XString8 str(ret,STRING_ATTACH);
			list.SplitStringS(str,"\r\n");
			int i,nCount=list.GetSize();
			for(i=0;i<nCount;i++)
			{
				XString8&strLine=list[i];
				if(strLine.IsEmpty()) continue;
				char*sv=strLine.GetData();
				char*sp=strchr(strLine.GetData(),'=');
				if(sp)
				{
					Stringp key=c->internString(c->newStringUTF8(sv,sp-sv));
					Atom	atom=StringAtom(sp+1,0);					
					//Stringp value=c->newStringUTF8(sp+1);
					obj->setStringProperty(key,atom);
				}
			}
			str.DataTo();*/
			return obj->atom();
		}
		else
		{
			return StringAtom(ret,0,args);
			//Stringp s=c->newStringUTF8(ret);
			//return s->atom();
		}
	}
	else 
	{
#ifdef _JAVASCRIPT_
	#ifdef _WINEMU
			XString16 str("[Note] External call:\r\n");
			str+=XString16(cmd.c_str());
			((ShellCore*)c)->GetPlayer()->Trace(str);
	#endif
			return c->DoJavaScript(name,args);
#else

	#ifdef _WINEMU
			XString16 str("[Note] Missed external call:");
			str+=XString16(cmd.c_str());
			((ShellCore*)c)->GetPlayer()->Trace(str);
	#endif
			return undefinedAtom;//c->kundefined->atom();
#endif
	}
}

bool ExternalInterfaceClass::AS3_available_get()
{
	//Add your act code here...
	return true;
}

void ExternalInterfaceClass::AS3_available_set(bool)
{
	//Add your act code here...	
}

Stringp ExternalInterfaceClass::AS3_objectID_get()
{
	//Add your act code here...
	return core()->kEmptyString;
}

void ExternalInterfaceClass::AS3_objectID_set(Stringp id)
{
	//Add your act code here...
	
}

//new adds
bool ExternalInterfaceClass::AS3_marshallExceptions_get()
{
	return false;
}
void ExternalInterfaceClass::AS3_marshallExceptions_set(bool marshallExceptions)
{
}
//new adds end


}