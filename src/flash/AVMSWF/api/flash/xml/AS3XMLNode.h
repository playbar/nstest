#ifndef _AS3XMLNode_
#define _AS3XMLNode_

namespace avmshell
{
	class XMLNodeObject;
};

namespace avmplus{namespace NativeID{
class XMLNodeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class XMLNodeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
    //Atom attributes;
	//uint32_t nodeType;
	//ArrayObject* childNodes;
	//avmshell::XMLNodeObject* firstChild;
	//avmshell::XMLNodeObject* lastChild;
	//Stringp localName;
	//avmshell::XMLNodeObject* nextSibling;
	//Stringp nodeName;	
	//Stringp nodeValue;
	//avmshell::XMLNodeObject* parentNode;
	//avmshell::XMLNodeObject* previousSibling;
	
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class XMLNodeObject;
	class XMLNodeClass : public ClassClosure
	{
	public:
		XMLNodeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLNodeClassSlots m_slots_XMLNodeClass;
};
class XMLNodeObject : public ScriptObject
{
	public:

		//public var nodeType:uint;
		 //public var childNodes:Array;
		 //public var firstChild:XMLNode;
		 //public var lastChild:XMLNode;
		 //public var localName:String;
		 //public var nextSibling:XMLNode;
		 //public var nodeName:String;		 
		 //public var nodeValue:String;
		 //public var parentNode:XMLNode;
		XU32 m_nodeType;
		DRCWB(ArrayObject*) m_childNodes;
		DRCWB(XMLNodeObject*) m_firstChild;
		DRCWB(XMLNodeObject*) m_lastChild;
		DRCWB(Stringp)		  m_localName;
		DRCWB(XMLNodeObject*) m_nextSibling;
		DRCWB(Stringp)		  m_nodeName;
		DRCWB(Stringp)		  m_nodeValue;
		DRCWB(XMLNodeObject*) m_parentNode;
		DRCWB(XMLNodeObject*) m_previousSibling;

		XU32 AS3_nodeType_get(){return m_nodeType;}		
		ArrayObject* AS3_childNodes_get(){return m_childNodes;}
		XMLNodeObject* AS3_firstChild_get(){return m_firstChild;}
		XMLNodeObject* AS3_lastChild_get(){return m_lastChild;}
		Stringp		   AS3_localName_get(){return m_localName;}
		XMLNodeObject* AS3_nextSibling_get(){return m_nextSibling;}
		Stringp		   AS3_nodeName_get(){return m_nodeName;}
		Stringp		   AS3_nodeValue_get(){return m_nodeValue;}
		XMLNodeObject* AS3_parentNode_get(){return m_parentNode;}
		XMLNodeObject* AS3_previousSibling_get(){return m_previousSibling;}
		
		void AS3_nodeType_set(XU32 type){m_nodeType=type;}
		void AS3_childNodes_set(ArrayObject*value){m_childNodes=value;}
		void AS3_firstChild_set(XMLNodeObject*value){m_firstChild=value;}
		void AS3_lastChild_set(XMLNodeObject*value){m_lastChild=value;}
		void AS3_localName_set(Stringp value){m_localName=value;}
		void AS3_nextSibling_set(XMLNodeObject*value){m_nextSibling=value;}
		void AS3_nodeName_set(Stringp value){m_nodeName=value;}
		void AS3_nodeValue_set(Stringp value){m_nodeValue=value;}
		void AS3_parentNode_set(XMLNodeObject* value){m_parentNode=value;}
		void AS3_previousSibling_set(XMLNodeObject* value){m_previousSibling=value;}


		DWB(AtomArray*) m_namespaces;
		//DWB(E4XNode*) m_node;
		DRCWB(ScriptObject*)m_attributes;
		XMLNodeObject(XMLNodeClass *type, E4XNode *node);
		XMLNodeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		void CreateFromNode(E4XNode*node);
		void setNodeType(int t)
		{
			m_nodeType=t;
			//m_slots_XMLNodeObject.nodeType=t;
		}
		//ArrayObject* GetNamespaces(E4XNode*node);
		void GetAttributes(E4XNode*node);
		void GetChildren(E4XNode*node);
		//inline void setNode(E4XNode*node){m_node=node;}
		//inline int  getClass(){return m_node->getClass();}
	public:		
		Atom AS3_attributes_get();//{return m_attributes?m_attributes->atom():nullObjectAtom;}
		void AS3_attributes_set(Atom a){m_attributes=AvmCore::atomToScriptObject(a);}
		//ArrayObject* AS3_childNodes_get();
		//AvmBox AS3_firstChild_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_firstChild_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_lastChild_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_lastChild_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_localName_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_namespaceURI_get();
		//AvmBox AS3_nextSibling_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_nextSibling_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//Stringp AS3_nodeName_get();
		//void AS3_nodeName_set(Stringp);
		//uint32 AS3_nodeType_get();
		//void AS3_nodeType_set(uint32);
		//Stringp AS3_nodeValue_get();
		//void AS3_nodeValue_set(Stringp s);
		//AvmBox AS3_parentNode_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_parentNode_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_prefix_get();
		//AvmBox AS3_previousSibling_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_previousSibling_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//void AS3_constructor(uint32 type,Stringp str);
		void AS3_appendChild(XMLNodeObject*pNode);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_cloneNode(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_getNamespaceForPrefix(Stringp);
		Stringp AS3_getPrefixForNamespace(Stringp);
		//AvmBox AS3_hasChildNodes(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_insertBefore(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_removeNode(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//Stringp AS3_toString();
		//void AS3_addAttrib(Stringp name,Atom object);
	private:
		Namespacep publicNS;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLNodeObjectSlots m_slots_XMLNodeObject;
};}
#endif