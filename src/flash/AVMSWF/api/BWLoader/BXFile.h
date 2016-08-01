#ifndef __BXFILE__
#define __BXFILE__

namespace avmplus
{
	namespace NativeID
	{
		class BXFileClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			int iSEEK_SET;
			int iSEEK_CUR;
			int iSEEK_END;
			int iREAD;
			int iWRITE;
			int iCREATE;
			int iAPPEND;
			int iSWF;
			int iNETGAME;
			int iWEB;
			int iAPP;
		};
		class BXFileObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
		private:
		};
	}
}

namespace avmshell
{

	class ByteArrayObject;

	class BXFileClass : public ClassClosure
	{
	public:
		BXFileClass(VTable *vtable);
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);
		bool AS3_deleteFile(Stringp file);
		bool AS3_copyFile(Stringp src,Stringp dst);
		bool AS3_exec(Stringp name,int type);
		bool AS3_createShortcutIcon(Stringp file,Stringp image,Stringp name);
		bool AS3_reload();
		bool AS3_isExist(Stringp file); 		
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXFileClassSlots m_slots_BXFileClass;
	};

	class BXFileObject : public ScriptObject
	{
	public:
		//XString8 m_strFile;
		virtual void Release();
		//int		 m_nTemp[3];
		XFile	 m_file;
		int		 m_nMode;
		BXFileObject(VTable* vtable, ScriptObject* proto, int capacity);
		
	public:
		bool AS3_open(Stringp file,int mode);
		bool AS3_close();
		int  AS3_getLength();
		int  AS3_seek(int pos,int type);
		int  AS3_write(ByteArrayObject*data,int pos,int len);
		int  AS3_read(ByteArrayObject*data,int pos,int len);		
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXFileObjectSlots m_slots_BXFileObject;
	};
}

#endif
