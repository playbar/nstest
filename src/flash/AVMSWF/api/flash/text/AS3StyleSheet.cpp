#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StyleSheet.h"
#include "AS3TextFormat.h"
#include "ShellCore.h"

namespace avmshell{
StyleSheetClass::StyleSheetClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())StyleSheetObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* StyleSheetClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StyleSheetObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StyleSheetObject::StyleSheetObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_css.m_bClass = true;
	m_pNames = toplevel()->arrayClass->newArray(0);
	m_pSheets = toplevel()->objectClass->newInstance();
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* StyleSheetObject::AS3_styleNames_get()
{
	//Add your act code here...
	ArrayObject* pNames=m_pNames;
	int i,iCount=m_pNames->get_length();//m_css.m_ids.GetCount();//GetAttribs(m_list.GetSize();
	ArrayObject*pArray=toplevel()->arrayClass->newArray(iCount);//newArray(iCount);
	AvmCore*c=core();
	for(i=0;i<iCount;i++)
	{
		//const char*name=XEAB::GetName(m_list[i].nID);
		//if(!name) continue;
		//int id=m_css.m_css[i].nToken;
		pArray->setIntProperty(i,pNames->getIntProperty(i));//c->newStringLatin1(m_css.m_ids.m_list[i]->strKey)->atom());
	}
	return pArray;
}

//AS3 contructor function..
void StyleSheetObject::AS3_constructor()
{
	//Add your act code here...
	EventDispatcherObject::AS3_constructor(NULL);
}

void StyleSheetObject::AS3_clear()
{
	//Add your act code here...
	//m_css.Clear();
	//m_css.m_ids.m_list.RemoveAll();
	//m_pNames = 
	m_pNames = toplevel()->arrayClass->newArray(0);
	m_pSheets = toplevel()->objectClass->newInstance();
}

void StyleSheetObject::AddSheet(XCSS&css)
{
	int i,iCount = css.m_ids.GetCount();
	ObjectClass*pClass=toplevel()->objectClass;
	AvmCore*c=core();
	for(i=0;i<iCount;i++)
	{
		XString8&name=css.m_ids.m_list[i]->strKey;
		name.MakeLower();
		bool bExist = false;
		ArrayObject*pNames=m_pNames;
		int j,jCount = pNames->get_length();
		for(j=0;j<jCount;j++)
		{
			Stringp en=(Stringp)pNames->getIntProperty(j);
			if(en->equalsLatin1(name))
			{
				bExist = true;
				break;
			}
		}
		if(bExist) continue;
		ScriptObject*pObj=pClass->createInstance(pClass->ivtable(),pClass->prototype);
		Stringp s = c->internStringUTF8(name);
		m_pNames->setIntProperty(m_pNames->get_length(),s->atom());
		XU16Array list;
		int nStatus=css.PhaseSelector(list,name);
		if(list.GetSize()>0)
		{
			int id=list[0];			
			if(id>=0)
			{				
				id=css.m_css.Index((void*)id);
				if(id>=0)
				{
					XCSS::XSelector*pSelect=css.m_css.m_entries[id];
					
					int ii,iiCount=pSelect->m_attribs.GetSize();
					for(ii=0;ii<iiCount;ii++)
					{
						XVar*v=pSelect->m_attribs[ii];
						const char*name=css.AttribName(*v);
							//XEAB::GetName(v->nID);
						if(!name) continue;
						Stringp namep=c->internStringLatin1(name);
						switch(v->nType)
						{
						case VLOGIC:
							 pObj->setAtomProperty(namep->atom(),v->iData?trueAtom:falseAtom);
							 break;
						case VINT:
							 pObj->setAtomProperty(namep->atom(),c->intToAtom(v->iData));
							 break;
						case VFLOAT:
							 pObj->setAtomProperty(namep->atom(),c->doubleToAtom(v->fData));
							 break;
						case VSTRING:
							 pObj->setAtomProperty(namep->atom(),c->newStringUTF8(v->strData)->atom());
							 break;
						}
					}
					//return pObj->atom();
				}
			}
			//return pObj->atom();
		}
		m_pSheets->setStringProperty(s,pObj->atom());
	}
}

Atom StyleSheetObject::AS3_getStyle(Stringp s)
{
	//Add your act code here...
	Atom atom=m_pSheets->getStringProperty(s);
	if(!AvmCore::isNullOrUndefined(atom))
	{
		return atom;
	}
	ObjectClass*pClass=toplevel()->objectClass;
	ScriptObject*pObj=pClass->createInstance(pClass->ivtable(),pClass->prototype);
	return pObj->atom();
	/*StUTF8String str(s);
	XString8 names(str.c_str());
	XU16Array list;
	int nStatus=m_css.PhaseSelector(list,names);
	if(list.GetSize()<=0) return pObj->atom();
	int id=list[0];
	if(id<0) return pObj->atom();
	else
	{
		AvmCore*c=core();
		id=m_css.m_css.Index((void*)id);
		if(id>=0)
		{
			XCSS::XSelector*pSelect=m_css.m_css.m_entries[id];
			
			int i,iCount=pSelect->m_attribs.GetSize();
			for(i=0;i<iCount;i++)
			{
				XVar*v=pSelect->m_attribs[i];
				const char*name=m_css.AttribName(*v);
					//XEAB::GetName(v->nID);
				if(!name) continue;
				Stringp namep=c->internStringLatin1(name);
				switch(v->nType)
				{
				case VLOGIC:
					 pObj->setAtomProperty(namep->atom(),v->iData?trueAtom:falseAtom);
					 break;
				case VINT:
					 pObj->setAtomProperty(namep->atom(),c->intToAtom(v->iData));
					 break;
				case VFLOAT:
					 pObj->setAtomProperty(namep->atom(),c->doubleToAtom(v->fData));
					 break;
				case VSTRING:
					 pObj->setAtomProperty(namep->atom(),c->newStringUTF8(v->strData)->atom());
					 break;
				}
			}
			return pObj->atom();
		}
	}
	return pObj->atom();*/
	//return kAvmThunkUndefined;//Modify this please!
}

void StyleSheetObject::AS3_parseCSS(Stringp s)
{
	//Add your act code here..
	if(s)
	{
		StUTF8String cssText(s);
		XString8 str(cssText.c_str());
		
		
		m_css.AddCSS(NULL,str);
		AddSheet(m_css);
	}
}

void StyleSheetObject::AS3_setStyle(Stringp name,Atom v)
{
	//Add your act code here...
	if(name==XNULL) return;
	//name->AS3_toLowerCase();
	m_pSheets->setStringProperty(name,v);
	if(!AvmCore::isUndefined(m_pSheets->getStringProperty(name)))
	{
		//Atom args[1]={v};
		m_pNames->setIntProperty(m_pNames->get_length(),v);//AS3_push(args,1);
	}
	StUTF8String s(name);
	AvmCore* c=core();
	const char* n=s.c_str();
	int id=m_css.CreateID(n,n[0]=='.'?XCSS::XCSS_CLASS:XCSS::XCSS_ID);
	if(!c->isNull(v)&&atomKind(v)==kObjectType) //return;
	{
		XString8 names(n);
		XU16Array list;
		int nStatus=m_css.PhaseSelector(list,names);
		XVarList vars;
		Traits* ts=c->atomToScriptObject(v)->vtable->traits;
		TraitsBindingsp b=ts->getTraitsBindings();
		int type;
		int index=0,iSet=0;
		for(;;)
		{
			index=b->next(index);
			if(index<=0) break;
			Stringp str=b->keyAt(index);
			StUTF8String k(str);
			int attribID=m_css.AttribID(k.c_str(),type);
			//int id=XEAB::Index
			if(attribID>=0)
			{
				Binding bd=b->valueAt(index);
				if(bd==BIND_NONE) continue;
				switch (AvmCore::bindingKind(bd))
				{
					case BKIND_VAR:
					case BKIND_CONST:
						 {
							Atom atom=getSlotAtom(AvmCore::bindingToSlotId(bd));
							switch(atomKind(atom))
							{
							case kStringType://    = 2;  // null=2
								{
									Stringp s=c->string(atom);
									StUTF8String ss(s);
									XVar var(attribID,ss.c_str());
									m_css.MakeVar(NULL,vars,var,type,ss.c_str());
									//vars.Add(XVar(attribID,ss.c_str()));
								}break;
							case kBooleanType:
								 vars.Add(XVar(attribID,c->boolean(atom)));
								 break;
							case kIntptrType:
								 vars.Add(XVar(attribID,c->integer_i(atom)));
								 break;
							case kDoubleType:
								 vars.Add(XVar(attribID,c->atomToDouble(atom)));
								 break;
							}
						}break;
					case BKIND_GET:
					case BKIND_GETSET:
						break;
						
				}
			}
		}
		if(ts->getHashtableOffset()!=0)
		{
			InlineHashtable*b=c->atomToScriptObject(v)->getTable();			
			if(b)
			{
				int index=0;
				for(;;)
				{
					index=b->next(index);
					if(index<=0) break;				
					Stringp str=c->string(b->keyAt(index));
					if(!str) continue;
					StUTF8String k(str);
					int attribID=m_css.AttribID(k.c_str(),type);
			//int id=XEAB::Index
					if(attribID>=0)
					{
						Atom atom=b->valueAt(index);
						switch(atomKind(atom))
						{
						case kStringType://    = 2;  // null=2
							{
								Stringp s=c->string(atom);
								//StUTF8String ss(s);
								//XXVar var(attribID,ss.c_str());
								//m_css.MakeStringVar(var);
								//vars.Add(var);//XVar(attribID,ss.c_str()));
								StUTF8String ss(s);
								XVar var(attribID,ss.c_str());
								m_css.MakeVar(NULL,vars,var,type,ss.c_str());
							}break;
						case kBooleanType:
							 vars.Add(XVar(attribID,c->boolean(atom)));
							 break;
						case kIntptrType:
							 vars.Add(XVar(attribID,c->integer_i(atom)));
							 break;
						case kDoubleType:
							 vars.Add(XVar(attribID,c->atomToDouble(atom)));
							 break;
						}
					}
					
				}				
			}
		}
		m_css.m_css.AddAttribs(list,list.GetSize()-1,vars,(XU8)nStatus);
	}
}

TextFormatObject* StyleSheetObject::AS3_transform(Atom atom)
{
	//Add your act code here...
	ShellToplevel*top=(ShellToplevel*)toplevel();
	TextFormatClass*pClass=top->getTextFormatClass();
	TextFormatObject*pObj=(TextFormatObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
	if(!AvmCore::isNullOrUndefined(atom))
	{
		ScriptObject*pFmt = core()->atomToScriptObject(atom);
		pObj->SetObjFormat(pFmt);
	}
	return pObj;
	//return NULL;
}

}