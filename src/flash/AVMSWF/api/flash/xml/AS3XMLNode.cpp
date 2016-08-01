#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3XMLNode.h"
#include "AS3XMLNodeType.h"

namespace avmshell{

XMLNodeClass::XMLNodeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())XMLNodeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* XMLNodeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) XMLNodeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}


Atom XMLNodeObject::AS3_attributes_get()
{
	if(m_attributes==NULL)
	{
		m_attributes=toplevel()->objectClass->newInstance();
		//return m_attributes?m_attributes->atom():nullObjectAtom;
	}
	return m_attributes->atom();
}

XMLNodeObject::XMLNodeObject(XMLNodeClass*type,E4XNode *node)
		: ScriptObject(type->ivtable(), type->prototype)

{
	//Add your construct code here...
	this->publicNS = core()->findPublicNamespace();
	this->m_childNodes=toplevel()->arrayClass->newArray(0);
	m_namespaces=NULL;
	m_attributes=NULL;
	CreateFromNode(node);
	
}

void XMLNodeObject::CreateFromNode(E4XNode*node)
{
//	m_slots_XMLNodeObject.nodeType=
	m_nodeType=
		node->getClass()==E4XNode::kText?XTEXT_NODE:XELEMENT_NODE;
	Multiname name;
	if(node->getQName(&name,publicNS))
		//m_slots_XMLNodeObject.nodeName=name.getName();//node->getQName(
		m_nodeName=name.getName();
	//m_slots_XMLNodeObject.nodeValue=node->getValue();
	m_nodeValue=node->getValue();
	//m_slots_XMLNodeObject.localName=m_slots_XMLNodeObject.nodeName;
	m_localName=m_nodeName;//
	m_namespaces=node->getNamespaces();
		//GetNamespaces(node);
	GetAttributes(node);
	GetChildren(node);
}

void XMLNodeObject::GetAttributes(E4XNode*node)
{
	int i,iCount=node->numAttributes();//ats->getLength();
	if(!iCount) return;
	ScriptObject*pObj=toplevel()->objectClass->newInstance();
	m_attributes=pObj;
	for(i=0;i<iCount;i++)
	{
		E4XNode*n=node->getAttribute(i);
		if(n)
		{
			Multiname nm;
			if(!n->getQName(&nm,publicNS)) continue;
			Stringp value=n->getValue();
			pObj->setStringProperty(nm.getName(),value?value->atom():nullObjectAtom);
		}
	}
}

void XMLNodeObject::GetChildren(E4XNode*node)
{
	int i,iCount=node->numChildren();
	if(!iCount) return;
	AvmCore*c=core();
	XMLNodeClass*pClass=((ShellToplevel*)toplevel())->getXMLNodeClass();
	for(i=0;i<iCount;i++)
	{
		E4XNode*child=node->_getAt(i);
		XMLNodeObject*pNew=new (c->GetGC(),pClass->ivtable()->getExtraSize())XMLNodeObject(pClass,child);
		AS3_appendChild(pNew);
	}
}

/*ArrayObject* XMLNodeObject::GetNamespaces(E4XNode*node)
{
		AvmCore *core = this->core();
		ArrayObject *a = toplevel()->arrayClass->newArray();

		if (node->getClass() & (E4XNode::kText | E4XNode::kComment | E4XNode::kProcessingInstruction | E4XNode::kAttribute | E4XNode::kCDATA))
			return a; 

		// step 7+8+9+10
		for (uint32 i = 0; i < node->numNamespaces(); i++)
		{
			Namespace *ns = AvmCore::atomToNamespace (node->getNamespaces()->getAt(i));
			a->setUintProperty (i, ns->atom());
		}

		return a;
}*/

XMLNodeObject::XMLNodeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	this->publicNS = core()->findPublicNamespace();
	//pArray=
		//s.childNodes=pArray;//pArray->atom();
	this->m_childNodes=toplevel()->arrayClass->newArray(0);
	//m_node=NULL;
	//m_attributes=NULL;
}

Stringp XMLNodeObject::AS3_prefix_get()
{
	Stringp s=m_nodeName;//m_slots_XMLNodeObject.nodeName;
	if(!s) return NULL;
	int dot = s->lastIndexOf(core()->cachedChars[(int)':']);
	if(dot>=0)
	{
		return s->substring(0, dot);
	}
	else return s;
	return NULL;
}

Stringp XMLNodeObject::AS3_getPrefixForNamespace(Stringp p)
{
	AvmCore*c=core();
	if(m_namespaces&&m_namespaces->getLength())
	{
		int i,iCount=m_namespaces->getLength();
		for(i=0;i<iCount;i++)
		{
			Namespacep ns=c->atomToNamespace(m_namespaces->getAt(i));
			Stringp url=ns->getURI();
			if(p==NULL||p->isEmpty()) return url;
			int dot=url->lastIndexOf(p);
			if(dot>=0)
				return url->substring(dot);
		}
	}
	if(m_parentNode)//m_slots_XMLNodeObject.parentNode)
		return m_parentNode->AS3_getPrefixForNamespace(p);
		//return m_slots_XMLNodeObject.parentNode->AS3_getPrefixForNamespace(p);
	return NULL;
}

Stringp XMLNodeObject::AS3_getNamespaceForPrefix(Stringp p)
{
	AvmCore*c=core();
	if(m_namespaces&&m_namespaces->getLength())
	{
		int i,iCount=m_namespaces->getLength();
		for(i=0;i<iCount;i++)
		{
			Namespacep ns=c->atomToNamespace(m_namespaces->getAt(i));
			Stringp url=ns->getURI();
			if(p==NULL||p->isEmpty()) return url;
			int dot=url->lastIndexOf(p);
			if(dot>=p->get_length())
				return url->substring(0,dot-p->get_length());
		}
	}
	if(m_parentNode)//m_slots_XMLNodeObject.parentNode)
		return m_parentNode->AS3_getNamespaceForPrefix(p);
		//return m_slots_XMLNodeObject.parentNode->AS3_getNamespaceForPrefix(p);
	return NULL;
}

Stringp XMLNodeObject::AS3_namespaceURI_get()
{
	if(m_namespaces&&m_namespaces->getLength())
	{
		//int i,iCount=m_namespaces->get_length();
		Namespacep ns=core()->atomToNamespace(m_namespaces->getAt(0));
		return ns->getURI();
	}
	return NULL;
}

void XMLNodeObject::AS3_appendChild(XMLNodeObject*pNode)
{
	//avmplus::NativeID::XMLNodeObjectSlots&s=m_slots_XMLNodeObject;
	Atom atom=pNode->atom();
	ArrayObject*pArray=this->m_childNodes;//s.childNodes;//(ArrayObject*)core()->atomToScriptObject(s.childNodes);
	if(pArray==NULL)
	{
		pArray=toplevel()->arrayClass->newArray(0);
		//s.childNodes=pArray;//pArray->atom();
		this->m_childNodes=pArray;
	}
	pArray->push(&atom,1);
	
	//avmplus::NativeID::XMLNodeObjectSlots&c=pNode->m_slots_XMLNodeObject;	
	pNode->m_parentNode=this;
	//c.parentNode=this;
	pNode->m_previousSibling=this->m_lastChild;
	//c.previousSibling=s.lastChild;
	pNode->m_nextSibling=NULL;
	//c.nextSibling=NULL;
	if(this->m_firstChild==NULL)//s.firstChild==NULL)
		this->m_firstChild=pNode;
		//s.firstChild=pNode;
	this->m_lastChild=pNode;
	//s.lastChild=pNode;
}
//////////////////////////////////////////////////////////
//Native Method start...

/*

AvmBox XMLNodeObject::AS3_nextSibling_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_nextSibling_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

Stringp XMLNodeObject::AS3_nodeName_get()
{
	//Add your act code here...
	AvmCore*core=this->core();
	if(!m_node)
		toplevel()->throwTypeError(kNullArgumentError);
	Multiname m;
	if (!m_node->getQName(&m, publicNS))
		return NULL;
	return m.getName();
	//return (new (core->GetGC(), toplevel()->qnameClass()->ivtable()->getExtraSize()) QNameObject(toplevel()->qnameClass(), m));

}

void XMLNodeObject::AS3_nodeName_set(Stringp name)
{
	//Add your act code here...
	if(!m_node)
		toplevel()->throwTypeError(kNullArgumentError);
	AvmCore *core = this->core();

	if (m_node->getClass() & (E4XNode::kText | E4XNode::kComment | E4XNode::kCDATA))
			return;

	m_node->setQName(core,name,publicNS);
}

uint32 XMLNodeObject::AS3_nodeType_get()
{
	//Add your act code here...
	if(!m_node)
		toplevel()->throwTypeError(kNullArgumentError);
	if(getClass()&(E4XNode::kText))
		return XTEXT_NODE;
	else
		return XELEMENT_NODE;
}

void XMLNodeObject::AS3_nodeType_set(uint32 t)
{
	//Add your act code here...
	if(!m_node)
		toplevel()->throwTypeError(kNullArgumentError);
}

Stringp XMLNodeObject::AS3_nodeValue_get()
{
	//Add your act code here...
	XMLObject*xml=m_slots_XMLNodeObject.xml;
	if(xml==NULL) 
		toplevel()->throwTypeError(kNullArgumentError);
	E4XNode* node=xml->getNode();
	return node->getValue();
	//return kAvmThunkUndefined;//Modify this please!
}

void XMLNodeObject::AS3_nodeValue_set(Stringp s)
{
	//Add your act code here...
	XMLObject*xml=m_slots_XMLNodeObject.xml;
	if(xml==NULL) 
		toplevel()->throwTypeError(kNullArgumentError);
	E4XNode* node=xml->getNode();
	node->setValue(s);
}

void XMLNodeObject::AS3_addAttrib(Stringp name,Atom object)
{
	XMLObject*xml=m_slots_XMLNodeObject.xml;
	if(xml==NULL) 
		toplevel()->throwTypeError(kNullArgumentError);
	E4XNode* node=xml->getNode();
	AvmCore* core=this->core();
	E4XNode* attr=new(core->GetGC())AttributeE4XNode(node,core->string(object));
	
	attr->setQName(core,name,publicNS);
	node->addAttribute(attr);
}

AvmBox XMLNodeObject::AS3_parentNode_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_parentNode_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_prefix_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_previousSibling_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_previousSibling_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}



//AS3 contructor function..
void XMLNodeObject::AS3_constructor(uint32 type,Stringp s)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	MMgc::GC*gc=core()->GetGC();
	switch(type)
	{
	default:
	case XTEXT_NODE:
		 setNode(new (gc) TextE4XNode(0,s) ); 		 
		 m_node->setValue(s);
		 break;
	case XELEMENT_NODE:
		 setNode(new (gc) ElementE4XNode (0));
		 m_node->setQName(core(),s,publicNS);
		 break;
	}
}

AvmBox XMLNodeObject::AS3_appendChild(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_cloneNode(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_getNamespaceForPrefix(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_getPrefixForNamespace(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_hasChildNodes(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_insertBefore(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox XMLNodeObject::AS3_removeNode(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

Stringp XMLNodeObject::AS3_toString()
{
	//Add your act code here...
	AvmCore *core = this->core();
	
	XMLObject*xml=new (core->GetGC())XMLObject(toplevel()->xmlClass(),m_node);
	return xml->AS3_toXMLString();
}*/
}