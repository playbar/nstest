#ifndef _AS3HashAlgorithms_
#define _AS3HashAlgorithms_
namespace avmplus{namespace NativeID{
class HashAlgorithmsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class HashAlgorithmsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};

class SwfEnDeUtilClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	Stringp keyCode;
private:};
class SwfEnDeUtilObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};

class ClassUtilClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	Stringp keyCode;
private:};
class ClassUtilObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};

class MD5ClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	Stringp keyCode;
private:};
class MD5ObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};


class SamePropertyObjectUtilClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	Stringp keyCode;
private:};
class SamePropertyObjectUtilObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};

}}
namespace avmshell{
	class ByteArrayObject;
	class HashAlgorithmsClass : public ClassClosure
	{
	public:
		HashAlgorithmsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		int AS3_fnvHash(ByteArrayObject*pObj);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HashAlgorithmsClassSlots m_slots_HashAlgorithmsClass;
};

class HashAlgorithmsObject : public ScriptObject
{
	public:
		HashAlgorithmsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
	
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::HashAlgorithmsObjectSlots m_slots_HashAlgorithmsObject;
};

class SwfEnDeUtilClass : public ClassClosure
	{
	public:
		SwfEnDeUtilClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		void unCompress(ByteArrayObject*dat,Stringp key);
		Atom AS3_unCompress(ByteArrayObject* dat,Stringp key);
        Atom AS3_getDecryptDataByByteArray(ByteArrayObject*dat,Stringp key,Stringp info);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SwfEnDeUtilClassSlots m_slots_SwfEnDeUtilClass;
};
class SwfEnDeUtilObject : public ScriptObject
{
	public:
		SwfEnDeUtilObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SwfEnDeUtilObjectSlots m_slots_SwfEnDeUtilObject;
};


class ClassUtilClass : public ClassClosure
	{
	public:
		ClassUtilClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Atom AS3_clone(Atom obj);
        Atom AS3_createNewObject(Atom obj);
        Atom AS3_samePropertiesCopyValue(Atom obj1, Atom obj2);
        void AS3_objectSamePropertyCopyValueByXML(Atom Obj1,Atom Obj2);
		Atom BooleanAtom(Atom atom);
		void CopyObject(ScriptObject* pObj,ScriptObject* pObj2,bool isObject);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ClassUtilClassSlots m_slots_ClassUtilClass;
};
class ClassUtilObject : public ScriptObject
{
	public:
		ClassUtilObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ClassUtilObjectSlots m_slots_ClassUtilObject;
};

class MD5Class : public ClassClosure
	{
	public:
		MD5Class(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp AS3_hashBinary(ByteArrayObject*pData);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MD5ClassSlots m_slots_MD5Class;
};
class MD5Object : public ScriptObject
{
	public:
		MD5Object(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MD5ObjectSlots m_slots_MD5Object;
};

class SamePropertyObjectUtilClass : public ClassClosure
	{
	public:
		SamePropertyObjectUtilClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		void AS3_copySamePropertyValue(Atom atom1,Atom atom2);
	private:
		void CopySamePropertyValue(ScriptObject*obj1,ScriptObject*obj2);
		inline bool IsList(ScriptObject*obj)
		{
			return IsArray(obj)||IsVector(obj);
		}
		bool IsListAtom(Atom atom);		
		bool IsArray(ScriptObject*obj);
		bool IsVector(ScriptObject*obj);
		bool IsObject(ScriptObject*obj);
		Atom GetPropety(ScriptObject*obj,Atom name);
		void CopyDirectList(ScriptObject*src,ScriptObject*target);
		Atom CopyValue(Atom src,Atom target,bool isSrcObj,bool isTarObj);
		bool IsComplex(ClassClosure*pClass);
		bool IsComplex(Atom atom);
		//void CopyDirectList(ScriptObject*obj1,ScriptObj*obj2);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SamePropertyObjectUtilClassSlots m_slots_SamePropertyObjectUtilClass;
};
class SamePropertyObjectUtilObject : public ScriptObject
{
	public:
		SamePropertyObjectUtilObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SamePropertyObjectUtilObjectSlots m_slots_SamePropertyObjectUtilObject;
};

}
#endif