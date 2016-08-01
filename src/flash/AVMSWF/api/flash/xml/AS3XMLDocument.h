#ifndef _AS3XMLDocument_
#define _AS3XMLDocument_

#include "AS3XMLNode.h"

namespace avmplus{namespace NativeID{
class XMLDocumentClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class XMLDocumentObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class XMLDocumentObject;
	class XMLDocumentClass : public ClassClosure
	{
	public:
		XMLDocumentClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		XMLDocumentObject* CreateXML(Stringp s);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLDocumentClassSlots m_slots_XMLDocumentClass;
};
class XMLDocumentObject : public XMLNodeObject
{
	public:
		XMLDocumentObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(Atom) m_xmlDecl;
		DRCWB(Atom) m_docTypeDecl;
		DRCWB(Atom) m_namespaces;
		bool		m_bIgnoreWhite;
		Atom AS3_docTypeDecl_get(){return m_docTypeDecl;}		
		void AS3_docTypeDecl_set(Atom a)
		{	m_docTypeDecl=a;	}
		Atom AS3_idMap_get();
		void AS3_idMap_set(Atom a);
		bool AS3_ignoreWhite_get(){return m_bIgnoreWhite;}
		void AS3_ignoreWhite_set(bool b){m_bIgnoreWhite=b;}
		Atom AS3_xmlDecl_get(){return m_xmlDecl;}
		void AS3_xmlDecl_set(Atom a){m_xmlDecl=a;}
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_createElement(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_createTextNode(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_parseXML(Stringp s);
		//AvmBox AS3_toString(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLDocumentObjectSlots m_slots_XMLDocumentObject;
};}
#endif