#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ID3Info.h"
namespace avmshell{

const char* _strSongClass[]=
	{"Blues","ClassicRock","Country","Dance","Disco","Funk","Grunge","Hip-Hop","Jazz","Metal","NewAge",
	 "Oldies","Other","Pop","R&B","Rap","Reggae","Rock","Techno","Industrial","Alternative",
	 "Ska","DeathMetal","Pranks","Soundtrack","Euro-Techno","Ambient","Trip-Hop","Vocal","Jazz+Funk","Fusion",
	 "Trance","Classical","Instrumental","Acid","House","Game","SoundClip","Gospel","Noise","AlternRock",
	 "Bass","Soul","Punk","Space","Meditative","InstrumentalPop","InstrumentalRock","Ethnic","Gothic","Darkwave",
	 "Techno-Industrial","Electronic","Pop-Folk","Eurodance","Dream","SouthernRock","Comedy","Cult","Gangsta","Top40",
	 "ChristianRap","Pop/Funk","Jungle","NativeAmerican","Cabaret","NewWave","Psychadelic","Rave","Showtunes","Trailer",
	 "Lo-Fi","Tribal","AcidPunk","AcidJazz","Polka","Retro","Musical","Rock&Roll","HardRock",/* Extended genres */
	 "Folk","Folk-Rock","NationalFolk","Swing","FastFusion","Bebob","Latin","Revival","Celtic","Bluegrass","Avantgarde",
	 "GothicRock","ProgessiveRock","PsychedelicRock","SymphonicRock","SlowRock","BigBand","Chorus","EasyListening","Acoustic","Humour",
	 "Speech","Chanson","Opera","ChamberMusic","Sonata","Symphony","BootyBass","Primus","PornGroove","Satire",
	 "SlowJam","Club","Tango","Samba","Folklore","Ballad","PowerBallad","RhythmicSoul","Freestyle","Duet",
	 "PunkRock","DrumSolo","Acapella","Euro-House","DanceHall","Goa","Drum&Bass","Club-House","Hardcore",
	 "Terror","Indie","BritPop","Negerpunk","PolskPunk","Beat","ChristianGangstaRap","HeavyMetal","BlackMetal","Crossover",
	 "ContemporaryChristian","ChristianRock","Merengue","Salsa","TrashMetal","Anime","JPop","Synthpop"
	};

ID3InfoClass::ID3InfoClass(VTable* cvtable):ClassClosure(cvtable)
{
	prototype = new (core()->GetGC(), cvtable->getExtraSize())ID3InfoObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ID3InfoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ID3InfoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ID3InfoObject::ID3InfoObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}

ID3InfoObject* ID3InfoClass::CreateID3(ID3Header*pHeader)
{
	ID3InfoObject*pObj=(ID3InfoObject*)this->newInstance();
	AS3ID3DATA& data=pObj->GetData();
	AvmCore*c=core();
	data.m_strAlbum=c->newStringLatin1(pHeader->className,30);
	data.m_strArtist=c->newStringLatin1(pHeader->authorName,30);
	data.m_strComment=c->newStringLatin1(pHeader->echoName,30);
	data.m_strGenre=c->newStringLatin1(_strSongClass[pHeader->nClass&0x7f]);
	data.m_strSongName=c->newStringLatin1(pHeader->songName,30);
	data.m_strTrack=c->kEmptyString;
	XTime time(pHeader->nTime);
	XString8 strTime;
	time.GetTime(strTime);
	data.m_strYear=c->newStringLatin1(strTime);
	return pObj;
}
//////////////////////////////////////////////////////////
//Native Method start...
/*AvmBox ID3InfoObject::AS3_album_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_album_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_artist_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_artist_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_comment_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_comment_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_genre_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_genre_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_songName_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_songName_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_track_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_track_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_year_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox ID3InfoObject::AS3_year_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}*/

}