#ifndef _AS3ID3Info_
#define _AS3ID3Info_

#include "AS3Sound.h"
#define AS3ID3DATA avmplus::NativeID::ID3InfoObjectSlots

namespace avmplus{namespace NativeID{
class ID3InfoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ID3InfoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	Stringp m_strAlbum;
	Stringp m_strArtist;
	Stringp m_strComment;
	Stringp m_strGenre;
	Stringp m_strSongName;
	Stringp m_strTrack;
	Stringp m_strYear;
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	extern const char* _strSongClass[];
	class ID3InfoObject;
	class ID3InfoClass : public ClassClosure
	{
	public:
		ID3InfoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ID3InfoObject* CreateID3(ID3Header*pHeader);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ID3InfoClassSlots m_slots_ID3InfoClass;
};
class ID3InfoObject : public ScriptObject
{
	public:
		ID3InfoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AS3ID3DATA& GetData(){return m_slots_ID3InfoObject;}
		/*DRCWB(Stringp) m_strAlbum;
		DRCWB(Stringp) m_strArtist;
		DRCWB(Stringp) m_strComment;
		DRCWB(Stringp) m_strGenre;
		DRCWB(Stringp) m_strSongName;
		DRCWB(Stringp) m_strTrack;
		DRCWB(Stringp) m_strYear;
		AvmBox AS3_album_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_album_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_artist_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_artist_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_comment_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_comment_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_genre_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_genre_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_songName_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_songName_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_track_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_track_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_year_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_year_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);*/
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ID3InfoObjectSlots m_slots_ID3InfoObject;
};}
#endif