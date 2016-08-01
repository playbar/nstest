#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Proxy.h"
namespace avmshell{
ProxyClass::ProxyClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//this->ivtable()->traits->isDictionary=true;
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ProxyObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ProxyClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	//ivtable->traits->isDictionary=true;
	return new (core()->GetGC(), ivtable->getExtraSize()) ProxyObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ProxyObject::ProxyObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	ShellCore*c=(ShellCore*)core();
	m_pNamespace=//c->getPublicNamespace(0);
		c->internNamespace(c->newNamespace(c->kFlashProxy, Namespace::NS_Public, c->getAPI(NULL)));
	/*Stringp s=m_pNamespace->getURI();
	
	m_hashTable=new (core()->GetGC(), 0) HeapHashtable(core()->GetGC());	
	AvmAssert(vtable->traits->getHashtableOffset() != 0);
	union {
		uint8_t* p;
		InlineHashtable* iht;
		HeapHashtable** hht;
	};
	p = (uint8_t*)this + vtable->traits->getHashtableOffset();
	*hht=m_hashTable;*/
	//int v=0;
}

MethodEnv* ProxyObject::getMethod(Stringp name)const
{
	//Atom atom=this->getAtomProperty(name->atom());
	//if(AvmCore::isNullOrUndefined(atom)) return NULL;
	//return AvmCore::atomToScriptObject(atom);
	Multiname m;
	m.setName(name);	
	m.setNamespace(m_pNamespace);
	Binding b = avmplus::getBinding(toplevel(), vtable, &m);
	if(AvmCore::bindingKind(b)!=BKIND_METHOD) return NULL;
	return vtable->methods[AvmCore::bindingToMethodId(b)];
}
//#if (__CORE_VERSION__>=0x02070100)
Atom ProxyObject::createLocalObject(Atom name)const
{
	Toplevel*top=toplevel();
	QNameClass*pClass=top->qnameClass();
	Atom names[]={NULL,name};
	QNameObject*pObject=(QNameObject*)AvmCore::atomToScriptObject(pClass->construct(1,names));
	//pObject->setAtomProperty(pClass->kUri,core()->kEmptyString->atom());
	//pObject->setAtomProperty(pClass->kLocalName,name);
	//pObject->set
	return pObject->atom();
}
//#endif
Atom  ProxyObject::callFunction(MethodEnv* fun,const Stringp name,int argc,Atom*argv)const
{
	//Atom*data=(Atom*)mmfx_alloc((argc+1)*sizeof(Atom));
	//data[0]=argv[0];
	//data[1]=name->atom();
	//for(int i=1;i<=argc;i++)
	//	data[i+1]=argv[i];
	//Atom base=this->atom();
	Atom ret=undefinedAtom;
//#if (__CORE_VERSION__>=0x02070100)
	argv[0]=createLocalObject(name->atom());
//#else
//	argv[0]=name->atom();
//#endif
	TRY(core(),kCatchAction_ReportAsError)
	{
		ret=fun->coerceEnter(atom(),argc+1,argv);	
	}
	CATCH(Exception *exception)
	{
		return undefinedAtom;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY

		//toplevel()->callproperty(,m,argc,argv,vtable);
		//fun->call(argc+1,data);
	//mmfx_free(data);
	return ret;
}

Atom ProxyObject::callProperty(const Multiname* name, int argc, Atom* argv)
{
	ShellCore*c=(ShellCore*)core();

	//Namespacep ns=vtable->traits->_ns
	TRY(c,kCatchAction_ReportAsError)
	{

		MethodEnv*env=getMethod(c->kCallProperty);
		if(env==NULL) return ScriptObject::callProperty(name,argc,argv);	
		return callFunction(env,name->getName(),argc,argv);
	}
	CATCH(Exception *exception)
	{
		return 0;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY

}


bool ProxyObject::deleteAtomProperty(Atom name)
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{
		
		//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kDeleteProperty);
		if(env==NULL) return ScriptObject::deleteAtomProperty(name);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			name=createLocalObject(name);
//#endif
			Atom atoms[]={atom(),name};
			return c->boolean(env->coerceEnter(1,atoms))!=0;
		}
	}
	CATCH(Exception *exception)
	{
		return false;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY

}

Atom ProxyObject::getDescendants(const Multiname* name) const
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{

	//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kGetDescendants);
		if(env==NULL) return ScriptObject::getDescendants(name);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			Atom atoms[]={atom(),createLocalObject(name->getName()->atom())};
//#else
//			Atom atoms[]={atom(),name->getName()->atom()};
//#endif
			return env->coerceEnter(1,atoms);
		}
	}
	CATCH(Exception *exception)
	{
		return undefinedAtom;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY

}

Atom ProxyObject::getUintProperty(uint32_t index) const
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{

	//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kGetProperty);
		if(env==NULL) return ScriptObject::getUintProperty(index);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			Atom atoms[]={atom(),createLocalObject(c->intToAtom(index))};
//#else
//			Atom atoms[]={atom(),c->intToAtom(index)};
//#endif
			return env->coerceEnter(1,atoms);
		}
		
	}
	CATCH(Exception *exception)
	{
		return undefinedAtom;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY

}
void ProxyObject::setUintProperty(uint32_t index,Atom value)
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{
		
	//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kSetProperty);
		if(env==NULL) ScriptObject::setUintProperty(index,value);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			Atom atoms[]={atom(),createLocalObject(c->intToAtom(index)),value};
/*#else
			Atom atoms[]={atom(),c->intToAtom(index),value};
#endif	*/		
			env->coerceEnter(2,atoms);
		}

	}
	CATCH(Exception *exception)
	{
	//AvmAssert(false);
	}
	END_CATCH
	END_TRY
}
bool ProxyObject::hasUintProperty(uint32_t index) const
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{
	//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kHasProperty);
		if(env==NULL) return ScriptObject::hasUintProperty(index);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			Atom atoms[]={atom(),createLocalObject(c->intToAtom(index))};
//#else
//			Atom atoms[]={atom(),c->intToAtom(index)};
//#endif
			return c->boolean(env->coerceEnter(1,atoms))!=0;
		}
		
	}
	CATCH(Exception *exception)
	{
		return false;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY

}

Atom ProxyObject::getAtomProperty(Atom name) const
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{
		//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kGetProperty);
		if(env==NULL) return ScriptObject::getAtomProperty(name);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			name=createLocalObject(name);
//#endif
			Atom atoms[]={atom(),name};
			return env->coerceEnter(1,atoms);
		}
	}
	CATCH(Exception *exception)
	{
		return undefinedAtom;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY

}
bool ProxyObject::hasAtomProperty(Atom name) const
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{

	//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kHasProperty);
		if(env==NULL) return ScriptObject::hasAtomProperty(name);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			name=createLocalObject(name);
//#endif
			Atom atoms[]={atom(),name};
			return c->boolean(env->coerceEnter(1,atoms))!=0;
		}
		
	}
	CATCH(Exception *exception)
	{
		return false;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY

}
Atom ProxyObject::nextValue(int index)
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{

		//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kNextValue);
		if(env==NULL) return ScriptObject::nextValue(index);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			Atom atoms[]={atom(),createLocalObject(c->intToAtom(index))};
//#else
//			Atom atoms[]={atom(),c->intToAtom(index)};
//#endif
			return env->coerceEnter(1,atoms);
		}
	}
	CATCH(Exception *exception)
	{
		return undefinedAtom;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY
}
int	 ProxyObject::nextNameIndex(int index)
{
	ShellCore*c=(ShellCore*)core();

	TRY(c,kCatchAction_ReportAsError)
	{			
	//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kNextNameIndex);
		if(env==NULL) 
		{			
			return ScriptObject::nextNameIndex(index);
		}
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			Atom atoms[]={atom(),createLocalObject(c->intToAtom(index))};
//#else
//			Atom atoms[]={atom(),c->intToAtom(index)};
//#endif
			return c->integer(env->coerceEnter(1,atoms));
		}
	
	}
	CATCH(Exception *exception)
	{
		return 0;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY
}
Atom ProxyObject::nextName(int index)
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{
		//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kNextName);
		if(env==NULL) return ScriptObject::nextName(index);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			Atom atoms[]={atom(),createLocalObject(c->intToAtom(index))};
//#else
//			Atom atoms[]={atom(),c->intToAtom(index)};
//#endif
			return env->coerceEnter(1,atoms);
		}
	}
	CATCH(Exception *exception)
	{
		return nullStringAtom;
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY
}
void ProxyObject::setAtomProperty(Atom name,Atom value)
{
	ShellCore*c=(ShellCore*)core();
	TRY(c,kCatchAction_ReportAsError)
	{
		//Namespacep ns=vtable->traits->_ns
		MethodEnv*env=getMethod(c->kSetProperty);
		if(env==NULL) ScriptObject::setAtomProperty(name,value);	
		else
		{
//#if (__CORE_VERSION__>=0x02070100)
			name=createLocalObject(name);
//#endif
			Atom atoms[]={atom(),name,value};
			env->coerceEnter(2,atoms);
		}
	}
	CATCH(Exception *exception)
	{
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY
}

Atom ProxyObject::getMultinameProperty(const Multiname* name) const
{
	return this->getAtomProperty(name->getName()->atom());
}
void ProxyObject::setMultinameProperty(const Multiname* name, Atom value)
{
	this->setAtomProperty(name->getName()->atom(),value);
}
bool ProxyObject::deleteMultinameProperty(const Multiname* name)
{
	return this->deleteAtomProperty(name->getName()->atom());
}
bool ProxyObject::hasMultinameProperty(const Multiname* name) const
{
	return this->hasAtomProperty(name->getName()->atom());
}

//////////////////////////////////////////////////////////
//Native Method start...
/*AvmBox ProxyObject::AS3_callProperty(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ProxyObject::AS3_deleteProperty(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ProxyObject::AS3_getDescendants(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ProxyObject::AS3_getProperty(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ProxyObject::AS3_hasProperty(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ProxyObject::AS3_isAttribute(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ProxyObject::AS3_nextName(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ProxyObject::AS3_nextNameIndex(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ProxyObject::AS3_nextValue(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ProxyObject::AS3_setProperty(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}*/

}