#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3H264VideoStreamSettings.h"
namespace avmshell
{
	H264VideoStreamSettingsClass::H264VideoStreamSettingsClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* H264VideoStreamSettingsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) H264VideoStreamSettingsObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	H264VideoStreamSettingsObject::H264VideoStreamSettingsObject(VTable *vtable, ScriptObject* proto, int capacity): VideoStreamSettingsObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_strCodec = ((ShellToplevel*)toplevel())->getVideoCodecClass()->getSlotH264AVC();
		m_strLevel = ((ShellToplevel*)toplevel())->getH264LevelClass()->getSlotLEVEL_2_1();
		m_strProfile = ((ShellToplevel*)toplevel())->getH264ProfileClass()->getSlotBASELINE();
	}
	//////////////////////////////////////////////////////////

//new adds 11

	Stringp H264VideoStreamSettingsObject::AS3_codec_get()
	{
		//LOGWHERE();
		return m_strCodec;//Stringp("H264Avc");
	}
		
	Stringp H264VideoStreamSettingsObject::AS3_level_get()
	{
		//LOGWHERE();
		return m_strLevel;//Stringp("2.1");
	}
		
	Stringp H264VideoStreamSettingsObject::AS3_profile_get()
	{
		//LOGWHERE();
		return m_strProfile;//Stringp("baseline");
	}

	void H264VideoStreamSettingsObject::AS3_setProfileLevel(Stringp profile, Stringp level)
	{
		//LOGWHERE();
		m_strProfile=profile;
		m_strLevel = level;
	}
//new adds 11 end
}