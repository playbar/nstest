#ifndef _BwPayAPI_
#define _BwPayAPI_

namespace avmplus
{
	namespace NativeID
	{
		class BwPayAPIClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
		private:
		};
		class BwPayAPIObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
		private:
		};
	}
}

namespace avmshell
{

	class BwPayAPIObject;

	class BwPayAPIClass : public ClassClosure
	{
	public:
		BwPayAPIClass(VTable *vtable);
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);
		BwPayAPIObject* AS3_GetBwPayAPI();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BwPayAPIClassSlots m_slots_BwPayAPIClass;
	};

	class BwPayAPIObject : public ScriptObject
	{
	public:
		BwPayAPIObject(VTable* vtable, ScriptObject* proto, int capacity);
	public:
		void AS3_payAlipay( Stringp url );
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BwPayAPIObjectSlots m_slots_BwPayAPIObject;
	};
}

#endif
