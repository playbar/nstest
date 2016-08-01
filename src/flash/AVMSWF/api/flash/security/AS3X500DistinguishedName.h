#ifndef _AS3X500DistinguishedName_
#define _AS3X500DistinguishedName_
namespace avmplus{namespace NativeID{
class X500DistinguishedNameClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class X500DistinguishedNameObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class X500DistinguishedNameClass : public ClassClosure
	{
	public:
		X500DistinguishedNameClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::X500DistinguishedNameClassSlots m_slots_X500DistinguishedNameClass;
};
class X500DistinguishedNameObject : public ScriptObject
{
	public:
		X500DistinguishedNameObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp AS3_commonName_get();
		Stringp AS3_organizationName_get();
		Stringp AS3_organizationalUnitName_get();
		Stringp AS3_localityName_get();
		Stringp AS3_stateOrProvinceName_get();
		Stringp AS3_countryName_get();
		Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::X500DistinguishedNameObjectSlots m_slots_X500DistinguishedNameObject;
};}
#endif