#ifndef _AS3ByteArray_
#define _AS3ByteArray_

#include "../../../flash/core/MultinameHashtable.h"
#include "../CharSet.h"

#include "AS3DataIO.h"
#include "AS3DataFile.h"

namespace avmplus{namespace NativeID{
class ByteArrayClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	
private:};
class ByteArrayObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ByteArrayObject;
	class ByteArrayClass : public ClassClosure
	{
	private:
		DWB(MultinameHashtable*) m_pCharSetMap;
		Namespacep m_pCharSetNS;	
	private:
		void AddCharSetMap(Stringp charSet, char_set charSetID);
		void BuildCharSetMap();
		ObjectEncoding m_nDefaultEncoding;
	public:
		uint32_t	AS3_defaultObjectEncoding_get();
		void		AS3_defaultObjectEncoding_set(uint32_t defaultObjectEncoding);
		ByteArrayClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ByteArrayObject* CreateArray(int nSize);
	public:
//		void AddCharSetMap(Stringp charSet){}
		int GetCharSetFormMap(Stringp charSet);

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ByteArrayClassSlots m_slots_ByteArrayClass;
	};
	class ByteArrayObject : public ScriptObject
	{
	public:
		//Self Implement
		//ObjectEncoding m_defaultObjectEncoding;
//		ObjectEncoding m_objectEncoding;
	private:
		bool		   m_bLocked,m_bCleard;
		ByteArrayClass *m_pByteArrayClass;
	public:
		ByteArrayObject* Clone();
		void Lock()
		{
			m_bLocked = true;
		}
		void UnLock()
		{
			if(!m_bLocked) return;
			m_bLocked = false;
			if(m_bCleard)
			{
				m_bCleard = false;
				this->m_byteArray.Clear();
			}
		}
	public:
		DRCWB(ArrayObject*) refString;
		DRCWB(ArrayObject*) refTraits;
		DRCWB(ArrayObject*) refObjects;
		//Copys
//		void Fill(const void *b, uint32_t len);
		void CheckNull(void *instance, const char *name);
		inline bool IsAttach(){return m_byteArray.IsAttach();}
		inline DataFile& GetByteArray() {return m_byteArray;}
		/*virtual*/
		GlobalMemoryProvider* getGlobalMemoryProvider() {return &m_byteArray;}

		uint32_t	GetLength() { return m_byteArray.GetLength(); }
		void		SetLength(uint32_t newLength){m_byteArray.SetLength(newLength);}

		//Modify By Xie.
		virtual bool hasAtomProperty(Atom name) const;
		virtual void setAtomProperty(Atom name, Atom value);
		virtual Atom getAtomProperty(Atom name) const;
		virtual Atom getUintProperty(uint32_t i) const;
		virtual void setUintProperty(uint32_t i, Atom value);

		//inline int Available() { return get_bytesAvailable(); }
		inline int GetFilePointer() { return m_byteArray.GetFilePointer(); }
		inline void Seek(int offset) { if (offset >= 0) {m_byteArray.Seek(offset);} }

	public:
		ByteArrayObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:		
		
		uint32_t	AS3_bytesAvailable_get();
		Stringp		AS3_endian_get();
		void		AS3_endian_set(Stringp endianSet);
		uint32_t	AS3_length_get();
		void		AS3_length_set(uint32_t length);
		uint32_t	AS3_objectEncoding_get();
		void		AS3_objectEncoding_set(uint32_t objectEncoding);
		uint32_t	AS3_position_get();
		void		AS3_position_set(uint32_t position);

		void		AS3_constructor();

		void		AS3_compress();
		void		AS3_uncompress();
		void		AS3_inflate();
		void		AS3_deflate();

		bool		AS3_readBoolean();
		int32_t		AS3_readByte();
		void		AS3_readBytes(ByteArrayObject* byteArray, uint32_t offset, uint32_t length);
		double		AS3_readDouble();
		double		AS3_readFloat();
		int32_t		AS3_readInt();
		Stringp		AS3_readMultiByte(uint32_t length, Stringp charSet);
		//Modify By Xie Peifu
		Atom		AS3_readObject();
		int32_t		AS3_readShort();
		uint32_t	AS3_readUnsignedByte();
		uint32_t	AS3_readUnsignedInt();
		uint32_t	AS3_readUnsignedShort();
		Stringp		AS3_readUTF();
		Stringp		AS3_readUTFBytes(uint32_t length);

		Stringp		AS3_toString();

		void		AS3_writeBoolean(bool data);
		void		AS3_writeByte(int32_t data);
		void		AS3_writeBytes(ByteArrayObject *data, uint32_t offset, uint32_t length);
		void		AS3_writeDouble(double data);
		void		AS3_writeFloat(double data);
		void		AS3_writeInt(int32_t data);
		void		AS3_writeMultiByte(Stringp data, Stringp charSet);
		//Modify By Xie
		void		AS3_writeObject(Atom);
		void		AS3_writeShort(int32_t data);
		void		AS3_writeUnsignedInt(uint32_t data);
		void		AS3_writeUTF(Stringp data);
		void		AS3_writeUTFBytes(Stringp data);
		void		AS3_clear();

		//new adds 11
		bool m_bShareable;
		bool AS3_shareable_get();
        void AS3_shareable_set(bool b);
         
        int AS3_atomicCompareAndSwapIntAt(int byteIndex, int expectedValue, int newValue);
		int AS3_atomicCompareAndSwapLength(int expectedLength, int newLength);
		Atom AS3_toJSON(Stringp k);
		//new adds 11 end
		/////////////////////////////////////////////////
		// Add By Xie. for Init...
		void		Attach(uint8_t* buffer,int l)
		{
			m_byteArray.Attach(buffer,l);
		}
		void		WriteBytes(XU8*pData,int l)
		{
			m_byteArray.Write(pData,l);
		}
		void		ReadBytes(XU8*pData,int l)
		{
			m_byteArray.Read(pData,l);
		}
	//Add by dsx for Read&Write the MulityByte
	private:
		Stringp	ReadData(uint32_t byteLen, char_set charSet);

		void	WriteUTF8(Stringp data);
		void	WriteUTF16(Stringp data);
		void	WriteMultiByte(Stringp data, char_set charSet);

		Stringp BuildUpUTF8(const char *pData, int byteLen);
		Stringp BuildUpUTF16(const wchar *pData, int byteLen);
		Stringp BuildUpMultiByte(const char *pData, int byteLen);

	private:
		MMgc::Cleaner	m_cleaner;
		DataFile		m_byteArray;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ByteArrayObjectSlots m_slots_ByteArrayObject;

		//Copys
		
};}
#endif