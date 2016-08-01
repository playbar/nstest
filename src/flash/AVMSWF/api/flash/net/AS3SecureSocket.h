#ifndef _AS3SecureSocket_
#define _AS3SecureSocket_

#include "AS3Socket.h"

namespace avmplus{namespace NativeID{
class SecureSocketClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SecureSocketObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ByteArrayObject;
	class X509CertificateObject;
	class SecureSocketClass : public ClassClosure
	{
	public:
		SecureSocketClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		bool AS3_isSupported_get();

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecureSocketClassSlots m_slots_SecureSocketClass;
};
class SecureSocketObject : public SocketObject
{
	public:
		SecureSocketObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		Stringp AS3_serverCertificateStatus_get();
		X509CertificateObject* AS3_serverCertificate_get(); //new add
		void AS3_constructor();
		void AS3_connect(Stringp pHost, int port);
		void AS3_addBinaryChainBuildingCertificate(ByteArrayObject* pCertificate, bool trusted);  //new add
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecureSocketObjectSlots m_slots_SecureSocketObject;
};}
#endif