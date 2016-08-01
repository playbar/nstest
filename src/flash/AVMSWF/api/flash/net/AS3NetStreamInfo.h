#ifndef _AS3NetStreamInfo_
#define _AS3NetStreamInfo_
namespace avmplus{namespace NativeID{
class NetStreamInfoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NetStreamInfoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double   m_currentBytesPerSecond;
	double   m_byteCount;
	double   m_maxBytesPerSecond;
	double   m_audioBytesPerSecond;
	double   m_audioByteCount;
	double   m_videoBytesPerSecond;
	double   m_videoByteCount;
	double   m_dataBytesPerSecond;
	double   m_dataByteCount;
	double   m_playbackBytesPerSecond;
	double   m_droppedFrames;
	double   m_audioBufferByteLength;
	double   m_videoBufferByteLength;
	double   m_dataBufferByteLength;
	double   m_audioBufferLength;
	double   m_videoBufferLength;
	double   m_dataBufferLength;
	double   m_srtt;
	double   m_audioLossRate;
	double   m_videoLossRate;
	Atom	 m_metaData;	//Object *
	Atom	 m_xmpData;		//Object *
	Stringp  m_resourceName;
	Stringp  m_uri;
	bool	 m_isLive;
private:};
}}
namespace avmshell{
	class NetStreamInfoClass : public ClassClosure
	{
	public:
		NetStreamInfoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamInfoClassSlots m_slots_NetStreamInfoClass;
};
class NetStreamInfoObject : public ScriptObject
{
	public:
		NetStreamInfoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_audioBufferByteLength_get();
		//double AS3_audioBufferLength_get();
		//double AS3_audioByteCount_get();
		//double AS3_audioBytesPerSecond_get();
		//double AS3_audioLossRate_get();
		//double AS3_byteCount_get();
		//double AS3_currentBytesPerSecond_get();
		//double AS3_dataBufferByteLength_get();
		//double AS3_dataBufferLength_get();
		//double AS3_dataByteCount_get();
		//double AS3_dataBytesPerSecond_get();
		//double AS3_droppedFrames_get();
		//bool AS3_isLive_get();
		//double AS3_maxBytesPerSecond_get();
		//Atom AS3_metaData_get();
		//double AS3_playbackBytesPerSecond_get();
		//Stringp AS3_resourceName_get();
		//double AS3_SRTT_get();
		//Stringp AS3_uri_get();
		//double AS3_videoBufferByteLength_get();
		//double AS3_videoBufferLength_get();
		//double AS3_videoByteCount_get();
		//double AS3_videoBytesPerSecond_get();
		//double AS3_videoLossRate_get();
		//Atom AS3_xmpData_get();

		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamInfoObjectSlots m_slots_NetStreamInfoObject;
};}
#endif