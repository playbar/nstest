#ifndef _AS3ContextMenuBuiltInItems_
#define _AS3ContextMenuBuiltInItems_
namespace avmplus{namespace NativeID{
class ContextMenuBuiltInItemsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ContextMenuBuiltInItemsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	int forwardAndBack;//允许用户在运行时在 SWF 文件中前进或后退一帧（单帧 SWF 文件不显示此属性）。 ContextMenuBuiltInItems 
	int loop;//允许用户将 SWF 文件设置为在到达最后一帧后自动重新开始（单帧 SWF 文件不显示此属性）。 ContextMenuBuiltInItems 
	int play;//允许用户启动暂停的 SWF 文件（单帧 SWF 文件不显示此属性）。 ContextMenuBuiltInItems 
	int print;//允许用户将显示的帧图像发送至打印机。 ContextMenuBuiltInItems 
	int quality;//允许用户在运行时设置 SWF 文件的分辨率。 ContextMenuBuiltInItems 
	int rewind;//允许用户将 SWF 文件设置为随时从选定的第一帧开始播放（单帧 SWF 文件不显示此属性）。 ContextMenuBuiltInItems 
	int save;//允许安装了 Shockmachine 的用户保存 SWF 文件。 ContextMenuBuiltInItems 
	int zoom;//允许用户在运行时放大和缩小 SWF 文件。 
	inline void SetData(ContextMenuBuiltInItemsObjectSlots&d)
	{
		VMPI_memcpy(this,&d,sizeof(ContextMenuBuiltInItemsObjectSlots));
	}
//Declare your MEMBER AS3 slots here!!!
private:};
}}

namespace avmshell{
	class ContextMenuBuiltInItemsObject;
	class ContextMenuBuiltInItemsClass : public ClassClosure
	{
	public:
		ContextMenuBuiltInItemsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ContextMenuBuiltInItemsObject* CreateItems();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuBuiltInItemsClassSlots m_slots_ContextMenuBuiltInItemsClass;
};
class ContextMenuBuiltInItemsObject : public ScriptObject
{
	public:
		ContextMenuBuiltInItemsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		inline bool getForwardAndBack(){return m_slots_ContextMenuBuiltInItemsObject.forwardAndBack!=0;}
		inline bool getLoop(){return m_slots_ContextMenuBuiltInItemsObject.loop!=0;}
		inline bool getPlay(){return m_slots_ContextMenuBuiltInItemsObject.play!=0;}
		inline bool getPrint(){return m_slots_ContextMenuBuiltInItemsObject.print!=0;}
		inline bool getQuality(){return m_slots_ContextMenuBuiltInItemsObject.quality!=0;}
		inline bool getRewind(){return m_slots_ContextMenuBuiltInItemsObject.rewind!=0;}
		inline bool getSave(){return m_slots_ContextMenuBuiltInItemsObject.save!=0;}
		inline bool getZoom(){return m_slots_ContextMenuBuiltInItemsObject.zoom!=0;}
		inline void getForwardAndBack(bool b){m_slots_ContextMenuBuiltInItemsObject.forwardAndBack=b;}
		inline void setLoop(bool b){m_slots_ContextMenuBuiltInItemsObject.loop=b;}
		inline void setPlay(bool b){m_slots_ContextMenuBuiltInItemsObject.play=b;}
		inline void setPrint(bool b){m_slots_ContextMenuBuiltInItemsObject.print=b;}
		inline void setQuality(bool b){m_slots_ContextMenuBuiltInItemsObject.quality=b;}
		inline void setRewind(bool b){m_slots_ContextMenuBuiltInItemsObject.rewind=b;}
		inline void setSave(bool b){m_slots_ContextMenuBuiltInItemsObject.save=b;}
		inline void setZoom(bool b){m_slots_ContextMenuBuiltInItemsObject.zoom=b;}
		inline void SetData(ContextMenuBuiltInItemsObject*p)
		{
			m_slots_ContextMenuBuiltInItemsObject.SetData(p->m_slots_ContextMenuBuiltInItemsObject);
		}
		inline void ResetData()
		{
			VMPI_memset(&m_slots_ContextMenuBuiltInItemsObject,0,sizeof(avmplus::NativeID::ContextMenuBuiltInItemsObjectSlots));
		}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ContextMenuBuiltInItemsObjectSlots m_slots_ContextMenuBuiltInItemsObject;
};}
#endif