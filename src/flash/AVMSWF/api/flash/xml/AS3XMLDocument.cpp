#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3XMLDocument.h"
#include "AS3XMLNodeType.h"

namespace avmshell{
XMLDocumentClass::XMLDocumentClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	((ShellToplevel*)toplevel())->shellClasses[abcclass_flash_xml_XMLDocument]=this;
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())XMLDocumentObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* XMLDocumentClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) XMLDocumentObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

XMLDocumentObject* XMLDocumentClass::CreateXML(Stringp s)
{
	XMLDocumentObject*pObj=(XMLDocumentObject*)createInstance(ivtable(),prototype);
	if(s)
	{
		Atom atoms[]={pObj->atom(),s->atom()};
		pObj->construct(1,atoms);
	}
	return pObj;
}

XMLDocumentObject::XMLDocumentObject(VTable *vtable, ScriptObject* proto, int capacity): XMLNodeObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_xmlDecl=nullObjectAtom;
	m_docTypeDecl=nullObjectAtom;
	m_bIgnoreWhite=false;
}
//////////////////////////////////////////////////////////
//Native Method start...

Atom XMLDocumentObject::AS3_idMap_get()
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

void XMLDocumentObject::AS3_idMap_set(Atom a)
{
	//Add your act code here...
	
}


void XMLDocumentObject::AS3_parseXML(Stringp s)
{
	//Add your act code here...	
	XMLClass*pClass=toplevel()->xmlClass();
	XMLObject*pObj=new (core()->GetGC())XMLObject(pClass,s,core()->getPublicNamespace(0));
	if(pObj->m_xmlDecl)
		this->m_xmlDecl=pObj->m_xmlDecl->atom();
	if(pObj->m_docTypeDecl)
		this->m_docTypeDecl=pObj->m_docTypeDecl->atom();
	//this->m_slots_XMLNodeObject.nodeType=XELEMENT_NODE;
	setNodeType(XELEMENT_NODE);
	this->CreateFromNode(pObj->getNode());
	//XMLNodeClass*type=((ShellToplevel*)toplevel())->getXMLNodeClass();
	//XMLNodeObject*pNode=new(core()->GetGC(),type->ivtable()->getExtraSize())XMLNodeObject(type,pObj->getNode());
	//AS3_appendChild(pNode);
}


}