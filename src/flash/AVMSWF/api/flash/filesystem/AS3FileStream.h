#ifndef _AS3FileStream_
#define _AS3FileStream_
namespace avmplus{namespace NativeID{
class FileStreamClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class FileStreamObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FileStreamClass : public ClassClosure
	{
	public:
		FileStreamClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileStreamClassSlots m_slots_FileStreamClass;
};
class FileStreamObject : public ScriptObject
{
	public:
		FileStreamObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_bytesAvailable_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_endian_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_endian_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_objectEncoding_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_objectEncoding_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_position_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_position_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readAhead_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readAhead_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_close(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_open(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_openAsync(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readBoolean(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readByte(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readBytes(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readDouble(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readFloat(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readInt(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readMultiByte(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readObject(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readShort(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readUnsignedByte(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readUnsignedInt(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readUnsignedShort(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readUTF(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_readUTFBytes(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_truncate(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeBoolean(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeByte(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeBytes(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeDouble(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeFloat(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeInt(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeMultiByte(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeObject(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeShort(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeUnsignedInt(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeUTF(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_writeUTFBytes(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileStreamObjectSlots m_slots_FileStreamObject;
};}
#endif