#ifndef _AS3_DATAIO_H
#define _AS3_DATAIO_H

#include "../../CharSet.h"

#include "flashglobal.h"
#include "AS3DataMemory.h"

//Added By Xiepeifu

namespace avmshell
{
	enum 
	{
		AMF_OUTNAME=1,
		AMF_OUTVALUE=2,
		AMF_OUTCOUNT=4,
	};
	enum AMFTYPE
	{
		AMF_UNDEFINE=0,
		AMF_NULL=1,
		AMF_FALSE=2,
		AMF_TRUE=3,
		AMF_INT=4,
		AMF_DOUBLE=5,
		AMF_STRING=6,
		AMF_XMLDOC=7,
		AMF_DATE=8,
		AMF_ARRAY=9,
		AMF_OBJECT=10,
		AMF_XML=11,
		AMF_BYTEARRAY=12,
		AMF_VECTORINT = 13,
		AMF_VECTORUINT =14,
		AMF_VECTORDOUBLE = 15,
		AMF_VECTOROBJECT = 16,
		AMF_DICTIONARY=17//,Dictionary
	};
	enum AMFTYPE0
	{
		AMF0_NUMBER=0,
		AMF0_BOOLEAN=1,
		AMF0_STRING=2,
		AMF0_OBJECT=3,
		AMF0_MOVIECLIP=4,
		AMF0_NULL=5,
		AMF0_UNDEFINED=6,
		AMF0_REFERENCE=7,
		AMF0_ECMAARRAY=8,
		AMF0_OBJECTEND=9,
		AMF0_STRICTARRAY=0xa,
		AMF0_DATE=0xb,
		AMF0_LONGSTRING=0x0c,
		AMF0_UNSUPPORTED=0x0d,
		AMF0_RECORDSET=0x0e,
		AMF0_XMLDOC=0x0f,
		AMF0_TYPEDOBJECT=0x10,
		AMF0_AMF3=0x11,
	};

	enum ObjectEncoding {
		kAMF0 = 0,
		kAMF3 = 3,
		kEncodeDefault = kAMF3	
	};

	enum Endian
	{
		kBigEndian    = 0,
		kLittleEndian = 1
	};

	template <class T>
	inline void EndianSwap(T &a, T &b)
	{
		//T t = a;
		//a = b;
		//b = t;
		a = a ^ b;
		b = a ^ b;
		a = a ^ b;
	}

	inline void EndianFlipU16(uint16_t &value)
	{
		uint8_t *pValue = (uint8_t *)&value;
		EndianSwap(pValue[0], pValue[1]);
	}

	inline void	EndianFlipU32(uint32_t &value)
	{
		uint8_t *pValue = (uint8_t *)&value;
		EndianSwap(pValue[0], pValue[3]);
		EndianSwap(pValue[1], pValue[2]);
	}

	inline void	EndianFlipU64(uint64_t &value)
	{
		uint8_t *pValue = (uint8_t *)&value;
		EndianSwap(pValue[0], pValue[7]);
		EndianSwap(pValue[1], pValue[6]);
		EndianSwap(pValue[2], pValue[5]);
		EndianSwap(pValue[3], pValue[4]);
	}

	class DataIOBase
	{
	private:
		Endian m_endian;
		ObjectEncoding m_objectEncoding;

	protected:
		Toplevel* const m_pcToplevel;

	public:
		DataIOBase(Toplevel *pTopLevel) : m_pcToplevel(pTopLevel)
		{
			m_endian = kBigEndian;
			m_objectEncoding = kEncodeDefault;
		}
		~DataIOBase(){}

		Endian	GetEndian() const {return m_endian;}
		void	SetEndian(Endian endian){m_endian = endian;}

		ObjectEncoding	GetObjectEncoding() const {return m_objectEncoding;}
		void			SetObjectEncoding(ObjectEncoding objectEncoding);

		//ObjectEncoding	GetObjectEncoding(){return m_objectEncoding;}
		//void			SetObjectEncoding(ObjectEncoding objectEncoding){m_objectEncoding = objectEncoding;}

		Endian GetNativeEndian() const
		{
			#if defined(AVMPLUS_LITTLE_ENDIAN)
				return kLittleEndian;
			#elif defined(AVMPLUS_BIG_ENDIAN)
				return kBigEndian;
			#else
				#error
			#endif
		}

		void EndianAdjust16(uint16_t &value)
		{
			if (GetEndian() != GetNativeEndian())
			{
				EndianFlipU16(value);
			}
		}

		void EndianAdjust32(uint32_t &value)
		{
			if (GetEndian() != GetNativeEndian())
			{
				EndianFlipU32(value);
			}
		}

		void EndianAdjust64(uint64_t &value)
		{
			if (GetEndian() != GetNativeEndian())
			{
				EndianFlipU64(value);
			}
		}
	};

	class DataInput : virtual public DataIOBase
	{
	public:
//		DataInput(Toplevel *toplevel) : m_toplevel(toplevel) {}
		DataInput(Toplevel *pToplevel) : DataIOBase(pToplevel)
		{
//			m_objectEncoding = kEncodeDefault;
//			m_bytesAvailable = 0;
		}
		virtual ~DataInput(){}

		virtual uint32_t	BytesAvailable() = 0;

		virtual uint32_t	GetFilePointer() {return 0;};
		//virtual uint32_t	BytesAvailable()
		//{
		//	return m_bytesAvailable;
		//}
//		void	SetBytes

		virtual void		Read(void *data, uint32_t count) = 0;

		bool				ReadBoolean();
		uint32_t			ReadUnsignedByte();
		uint32_t			ReadUnsignedShort();
		uint32_t			ReadUnsignedInt();
		float				ReadFloat();
		double				ReadDouble();
		String*				ReadUTF(bool bLong=false);
		String*				ReadUTFBytes(uint32_t length);

		int32_t				ReadByte();
		int32_t				ReadShort();
		int32_t				ReadInt();
		void				ReadBytes(DataMemory &buffer, uint32_t offset, uint32_t length);
		Stringp				ReadMultiByte(uint32_t length, Stringp charSet);
		//Modify By Xie Peifu,for init...
		Atom				ReadObject0(Atom thisAtom,ArrayObject*refString,ArrayObject*refObject,ArrayObject*refTraits);
		Atom				ReadObject(Atom thisAtom,ArrayObject*refString,ArrayObject*refObject,ArrayObject*refTraits);
		void				ReadMembers0(Atom thisAtom,ScriptObject*pObj,ArrayObject*refString,ArrayObject*refObject,ArrayObject*refTraits,int iCount=0);
		void			    ReadScriptObject(Atom thisAtom,ScriptObject*pObj,int iCount,bool bHash,
											 ArrayObject*refString,ArrayObject*refObject,ArrayObject*refTraits,Atom classAtom);
		void				ReadScriptObjectTraits(Atom thisAtom,ScriptObject*pObj,
											 ArrayObject*refString,ArrayObject*refObject,ArrayObject*refTraits,int iRef);
		Stringp				ReadString(ArrayObject*refString);
		int					ReadU29();		
		
	private:
		Stringp	ReadData(uint32_t byteLen, char_set charSet);

		Stringp BuildUpUTF8(const char *pData, int byteLen);
		Stringp BuildUpUTF16(const wchar *pData, int byteLen);
		Stringp BuildUpMultiByte(const char *pData, int byteLen);

	public:
	//	void ThrowEOFError();
	//	void ThrowMemoryError();
		void CheckEOF(uint32_t count);
	};

	class DataOutput : virtual public DataIOBase
	{

	public:
		DataOutput(Toplevel *pToplevel) : DataIOBase(pToplevel){}
		virtual ~DataOutput(){}

		virtual void	Write(const void *data, uint32_t count) = 0;

		void		WriteBoolean(bool data);
		void		WriteByte(int32_t data);
		void		WriteBytes(DataMemory &data, uint32_t offset, uint32_t length);
		void		WriteDouble(double data);
		void		WriteFloat(float data);
		void		WriteShort(int32_t data);
		void		WriteInt(int32_t data);
		void		WriteUnsignedInt(uint32_t data);
		void		WriteMultiByte(Stringp data, Stringp charSet);
		//Modify By Xie Peifu
		inline int	FindObject(ArrayObject*pArray,Atom atom)
		{
			if(pArray==NULL) return 0;
			int i,iCount=pArray->getDenseLength();//get_length();
			for(i=0;i<iCount;i++)
			{
				if(pArray->getIntProperty(i)==atom) return i;
			}
////#if (__CORE_VERSION__>=0x02071000)
			return -1;
//#else
//			return 0;
//#endif
		}
		inline void WriteRef(uint32_t type,uint32_t ref)
		{
			WriteByte(type);
			ref<<=1;
			WriteByte(ref);
		}
		inline void WriteString(Stringp str)
		{
			if(str==NULL)
			{
				WriteU29(1);
				return;
			}
			StUTF8String s(str);
			int l=(s.length()<<1)|1;
			WriteU29(l);
			this->Write(s.c_str(),l>>1);
			//WriteUTFBytes(str);
		}
		void		WriteScriptObject(Atom thisAtom,ScriptObject*pObj,ArrayObject*refString,ArrayObject*refObject);
		void		WriteDictionary(Atom thisAtom,ScriptObject*pObj,ArrayObject*refString,ArrayObject*refObject);
		void		WriteScriptObjectEx(Atom thisAtom,ScriptObject*pObj,Stringp alias,ArrayObject*refString,ArrayObject*refObject);
		void		WriteArray(Atom thisAtom,ArrayObject*pObj,ArrayObject*refString,ArrayObject*refObject);
		int			WriteMembers(Atom thisAtom,ScriptObject*,int t,ArrayObject*refString,ArrayObject*refObject);
		int			WriteDictMembers(Atom thisAtom,ScriptObject*,ArrayObject*refString,ArrayObject*refObject,bool writeOut);
		int			WriteMembers0(ScriptObject*pObj,ArrayObject*refObjects,bool bNamed=true);
		void		WriteU29(int u);		
		void		WriteObject(Atom thisAtom,Atom atom,ArrayObject*refString,ArrayObject*refObject);
		void		WriteObject0(Atom atom,ArrayObject*refObject);
		void		WriteUTF(Stringp data,bool bLong=false);		
		void		WriteUTFBytes(Stringp data);
		//Additions
		void		WriteUnsignedByte(uint8_t data);
		void		WriteUnsignedShort(uint16_t data);

	private:
		void	WriteUTF8(Stringp data);
		void	WriteUTF16(Stringp data);
		void	WriteMultiByte(Stringp data, char_set charSet);
	//protected:
	//	Toplevel* const m_pcToplevel;
	};
}

#endif