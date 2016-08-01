#ifndef _AS3CompressionAlgorithm_
#define _AS3CompressionAlgorithm_
namespace avmplus{namespace NativeID{
class CompressionAlgorithmClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//DEFLATE : String = "deflate"
	//ZLIB : String = "zlib"
	//LZMA:String = "lzma"
	Stringp DEFLATE;
	Stringp ZLIB;
	Stringp LZMA;
private:};
class CompressionAlgorithmObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CompressionAlgorithmClass : public ClassClosure
	{
	public:
		CompressionAlgorithmClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CompressionAlgorithmClassSlots m_slots_CompressionAlgorithmClass;
};
class CompressionAlgorithmObject : public ScriptObject
{
	public:
		CompressionAlgorithmObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CompressionAlgorithmObjectSlots m_slots_CompressionAlgorithmObject;
};}
#endif