#ifndef _AS3JSON_
#define _AS3JSON_

namespace avmplus
{
	namespace NativeID
	{
		class JSONParseErrorClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

		private:
		};

		class JSONParseErrorObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			int _location;
			Stringp _text;
			//Declare your MEMBER AS3 slots here!!!
		private:
		};

		class JSONClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

		private:
		};
		class JSONObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell
{
	class JSONParseErrorClass : public ClassClosure//EventClass
	{
	public:
		JSONParseErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		void ThrowError(Stringp str);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JSONParseErrorClassSlots m_slots_JSONParseErrorClass;
	};

	class JSONParseErrorObject : public ErrorObject
	{
	public:
		JSONParseErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);


	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JSONParseErrorObjectSlots m_slots_JSONParseErrorObject;
	};


	class JSONClass : public ClassClosure//EventClass
	{
	public:
		JSONClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		//XFile file;		
	public:
		void ThrowError(char* str);
		void ThrowError(char* str,char c);
		void ThrowError(char* str,const char*str1);
		JSONParseErrorClass* m_pErrorClass;
		Atom AS3_parse(Stringp text, FunctionObject* reviver = NULL);
		Stringp AS3_stringify(ScriptObject* value, void* replacer = NULL, void* space = NULL );

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JSONClassSlots m_slots_JSONClass;
	};

	class JSONObject : public ScriptObject
	{
	public:
		JSONObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JSONObjectSlots m_slots_JSONObject;
	};
}
#endif