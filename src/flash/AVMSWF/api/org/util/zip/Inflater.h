#ifndef _AS3Inflater_
#define _AS3Inflater_
namespace avmplus{namespace NativeID{
class InflaterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class InflaterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ByteArrayObject;
	class InflaterClass : public ClassClosure
	{
	public:
		InflaterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InflaterClassSlots m_slots_InflaterClass;
};
class InflaterObject : public ScriptObject
{
	public:
		InflaterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		
		uint32_t AS3_inflate(ByteArrayObject*pOutput);
        void AS3_setInput(ByteArrayObject*pInput);
	private:

		//DRCWB(ScriptObject*) lencode;
		DRCWB(ByteArrayObject*) inbuf;
#ifdef __USEEXT_INFLATE__
		int bitbuf;
		//DRCWB(ScriptObject*) distcode;
		DRCWB(ArrayObject*) lencodeCount;
		DRCWB(ArrayObject*) lencodeSymbol;
		DRCWB(ArrayObject*) distcodeCount;
		DRCWB(ArrayObject*) distcodeSymbol;
		uint32_t incnt;
		int      bitcnt;        
		void throwError(const char*buf,int id);
		int bits(int param1);
		int construct(ArrayObject*count,ArrayObject*symbol, int*param2, int param3);
		void constructFixedTables();
		int constructDynamicTables();
		int codes(ByteArrayObject*);
		void stored(ByteArrayObject* param1);
		int decode(ArrayObject*count,ArrayObject*symbol);
		inline void SetValue(ArrayObject*a,int index,int value)
		{
			Atom v=core()->intToAtom(value);
			a->setUintProperty(index,v);
		}
		int  GetValue(ArrayObject*a,int index)
		{
			Atom v=a->getUintProperty(index);
			return core()->integer(v);
		}
		inline void PushByte(ByteArrayObject*obj,int v)
		{
			obj->AS3_writeByte(v);
		}
		inline int GetByteValue(ByteArrayObject*obj,int index)
		{
			if(index<0||index>=obj->GetLength())
			{
				throwError("Range error!",0);
				return 0;
			}
			return obj->GetByteArray().GetBuffer()[index];
		}
#endif
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InflaterObjectSlots m_slots_InflaterObject;
};}
#endif