// XEnumData.cpp: implementation of the XEnumAlia class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include <math.h>
#include "XEnumData.h"
#include "XVar.h"
#include "XCSS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XEnumAttribs::_ATTR XEnumAttribs::_attribs[]=
{
	{"abbr",XEAB::TSTRING},
	{"accept-charset",/*XEAB::ACCEPT_CHARSET,*/XEAB::TCHARSET},
	{"accesskey",/*XEAB::ACCESSKEY,*/XEAB::TCHAR},
	{"accspeed",XEAB::TROTATE},
	{"action",/*XEAB::ACTION,*/XEAB::TURL},
	{"align",/*XEAB::ALIGN,*/XEAB::TALIGN},
	{"alink",/*XEAB::ALINK,*/XEAB::TCOLOR},
	{"allowscriptaccess",XEAB::TSTRING},
	{"alt",/*XEAB::ALT,*/XEAB::TSTRING},
	{"autotag",XEAB::TSTRING},
	{"axis",XEAB::TSTRING},
	{"background",/*XEAB::BACKGROUND,*/XEAB::TURL},

	{"bgcolor",/*XEAB::BGCOLOR,*/XEAB::TCOLOR},
	{"blockindent",XEAB::TINT},
	{"border",/*XEAB::BORDER,*/XEAB::TINT},
	{"button_hor_pos",XEAB::TINT},
	{"button_long_press",XEAB::TLOGIC},
	{"button_long_press_lock_time",XEAB::TINT},
	{"button_map_to",XEAB::TINT},
	{"button_name",XEAB::TMOUSEACT},
	{"button_ver_pos",XEAB::TINT},
	{"cache-control",/*XEAB::CACHE_CONTROL,*/XEAB::TSTRING},
	{"cellpadding",XEAB::TPECENT},
	{"cellspacing",XEAB::TPECENT},
	{"char",XEAB::TCHAR},
	{"charoff",XEAB::TPECENT},
	{"charset",/*XEAB::CHARSET,*/XEAB::TCHARSET},
	{"checked",/*XEAB::CHECKED,*/XEAB::TMARK},
	{"chunked",XEAB::TMARK},
	{"class",/*XEAB::CLASS,*/XEAB::TCLASS},
	{"color",/*XEAB::COLOR,*/XEAB::TCOLOR},
	{"cols",/*XEAB::COLS,*/XEAB::TINT},
	{"colspan",/*XEAB::COLSPAN,*/XEAB::TINT},
	{"columns",/*XEAB::COLUMNS,*/XEAB::TINT},
	{"content",/*XEAB::CONTENT,*/XEAB::TSTRING},
	{"content_id",XEAB::TSTRING},
	{"content_level",XEAB::TINT},
	{"coords",XEAB::TSTRING},
	{"data",XEAB::TSTRING},
	{"default_button_float_align",XEAB::ALIGN},
	{"default_button_float_side",XEAB::ALIGN},
	{"default_button_position",XEAB::TPTYPE},
	{"default_long_press_lock_time",XEAB::TINT},
	{"default_mouse_move_pace",XEAB::TINT},
	{"default_operate_mode",XEAB::TSTRING},
	{"direction",XEAB::TACCDIR},
	{"disabled",/*XEAB::DISABLED,*/XEAB::TMARK},
	{"domain",/*XEAB::DOMAIN,*/XEAB::TURL},
	{"emptyok",/*XEAB::EMPTYOK,*/XEAB::TLOGIC},
	{"encoding",/*XEAB::ENCODING,*/XEAB::TCHARSET},
	{"enctype",/*XEAB::ENCTYPE,*/XEAB::TSTRING},
	{"event",XEAB::TSTRING},
	{"face",XEAB::TSTRING},
	{"flashvars",XEAB::TSTRING},
	{"format",/*XEAB::FORMAT,*/XEAB::TFORMAT},
	{"forua",/*XEAB::FORUA,*/XEAB::TSTRING},
	{"frame",XEAB::TFRAME},
	{"headers",XEAB::TSTRING},
	{"height",/*XEAB::HEIGHT,*/XEAB::TPECENT},
	{"hotkey_for_zoom",XEAB::TINT},
	{"href",/*XEAB::HREF,*/XEAB::TURL},
	{"hspace",/*XEAB::HSPACE,*/XEAB::TPECENT},
	{"http-equiv",/*XEAB::HTTP_EQUIV,*/XEAB::TSTRING},
	{"id",/*XEAB::ID,*/XEAB::TID},
	{"iname",/*XEAB::INAME,*/XEAB::TSTRING},
	{"indent",XEAB::TINT},
	{"interface",XEAB::TSTRING},
	{"ivalue",/*XEAB::IVALUE,*/XEAB::TINT},
	{"key_map_to",XEAB::TINT},
	{"key_pressed",XEAB::TKEY},
	{"label",/*XEAB::LABEL,*/XEAB::TSTRING},
	{"leading",XEAB::TINT},
	{"leftmargin",XEAB::TINT},
	{"letterspacing",XEAB::TINT},
	{"link",/*XEAB::LINK,*/XEAB::TCOLOR},
	{"localsrc",/*XEAB::LOCALSRC,*/XEAB::TURL},
	{"loop",XEAB::TSTRING},
	{"long_press",XEAB::TLOGIC},
	{"long_press_lock_time",XEAB::TINT},
	{"maxlength",/*XEAB::MAXLENGTH,*/XEAB::TINT},
	{"md5",XEAB::TLOGIC},
	{"method",/*XEAB::METHOD,*/XEAB::TMETHOD},
	{"mode",/*XEAB::MODE,*/XEAB::TALIGN},
	{"mouse_act",XEAB::TMOUSEACT},
	{"multiple",/*XEAB::MULTIPLE,*/XEAB::TLOGIC},
	{"mute",XEAB::TLOGIC},
	{"name",/*XEAB::NAME,*/XEAB::TSTRING},
	{"newcontext",/*XEAB::NEWCONTEXT,*/XEAB::TLOGIC},
	{"noshade",/*XEAB::NOSHADE,*/XEAB::TLOGIC},
	{"nowrap",XEAB::TMARK},
	{"onclick",/*XEAB::ONCLICK,*/XEAB::TURL},//ONCLICK,
	{"onenterbackward",/*XEAB::ONENTERBACKWARD,*/XEAB::TURL},//ONENTERBACKWARD,
	{"onenterforward",/*XEAB::ONENTERFORWARD,*/XEAB::TURL},//ONENTERFORWARD,
	{"onkeydown",/*XEAB::ONKEYDOWN,*/XEAB::TURL},//ONKEYDOWN,
	{"onkeypress",/*XEAB::ONKEYPRESS,*/XEAB::TURL},//ONKEYPRESS,
	{"onkeyup",/*XEAB::ONKEYUP,*/XEAB::TURL},//ONKEYUP,
	{"onload",/*XEAB::ONLOAD,*/XEAB::TURL},//ONLOAD,
	{"onlyone",				   XEAB::TLOGIC},
	{"onmousedown",/*XEAB::ONMOUSEDOWN,*/XEAB::TURL},//ONMOUSEDOWN,
	{"onmouseout",/*XEAB::ONMOUSEOUT,*/XEAB::TURL},//ONMOUSEOUT,
	{"onmouseover",/*XEAB::ONMOUSEOVER,*/XEAB::TURL},//ONMOUSEOVER,
	{"onmouseup",/*XEAB::ONMOUSEUP,*/XEAB::TURL},//ONMOUSEUP,
	{"onpick",/*XEAB::ONPICK,*/XEAB::TURL},//ONPICK,
	{"ontimer",/*XEAB::ONTIMER,*/XEAB::TURL},//ONTIMER,
	{"onload",/*XEAB::ONLOAD,*/XEAB::TURL},//ONUNLOAD,
	{"optional",/*XEAB::OPTIONAL_X,*/XEAB::TLOGIC},
	{"ordered",/*XEAB::ORDERED,*/XEAB::TLOGIC},
	{"org",						 XEAB::TINT},
	{"path",/*XEAB::PATH,*/XEAB::TSTRING},
	{"play",XEAB::TSTRING},
	{"pluginspage",XEAB::TSTRING},
	{"quality",XEAB::TQUALITY},
	{"readonly",/*XEAB::READONLY,*/XEAB::TMARK},
	{"rightmargin",XEAB::TINT},
	{"rows",/*XEAB::ROWS,*/XEAB::TINT},
	{"rowspan",/*XEAB::ROWSPAN,*/XEAB::TINT},
	{"rules",XEAB::TFRAME},
	{"scheme",/*XEAB::SCHEME,*/XEAB::TINT},
	{"score_type",XEAB::TSCORETYPE},
	{"score_var_name",XEAB::TSTRING},
	{"screen_direction",XEAB::TSCDIR},
	{"sendreferer",/*XEAB::SENDREFERER,*/XEAB::TLOGIC},
	{"shape",XEAB::TSHAPE},
	{"size",/*XEAB::SIZE,*/XEAB::TINT},
	{"sound_device",XEAB::TSOUNDDEVICE},
	{"src",/*XEAB::SRC,*/XEAB::TURL},
	{"step",			 XEAB::TINT},
	{"style",			 XEAB::TSTRING},
	{"submit_url",		 XEAB::TURL},
	{"tabindex",/*XEAB::TABINDEX,*/XEAB::TINT},
	{"tabstops",XEAB::TSTRING},
	{"target",XEAB::TSTRING},
	{"text",/*XEAB::TEXT,*/XEAB::TCOLOR},
	{"title",/*XEAB::TITLE,*/XEAB::TSTRING},
	{"topscore_url",XEAB::TURL},
	{"touchsize",XEAB::TINT},
	{"type",/*XEAB::TYPE,*/XEAB::TINPUT},
	{"usemap",XEAB::TMAPID},
	{"valign",XEAB::TALIGN},
	{"value",/*XEAB::VALUE,*/XEAB::TSTRING},
	{"version",/*XEAB::VERSION,*/XEAB::TSTRING},
	{"virtual_key",XEAB::TSTRING},
	{"vlink",/*XEAB::VLINK,*/XEAB::TCOLOR},
	{"volume",XEAB::TINT},
	{"vspace",/*XEAB::VSPACE,*/XEAB::TPECENT},
	{"width",/*XEAB::WIDTH,*/XEAB::TPECENT},
	{"wmode",XEAB::TSTRING},
	{"xml:lang",/*XEAB::LANG,*/XEAB::TLAN},
	{"xml:space",/*XEAB::SPACE,*/XEAB::TSTRING},
	{"zacc",XEAB::TLOGIC},
	{"zadpos",XEAB::TINT},
	{"zadurl",XEAB::TSTRING},	
	{"zalign",XEAB::TINT},
	{"zbutton",XEAB::TLOGIC},
	{"zcontent_type",XEAB::TSWFTYPE},
	{"zmouse_pos",XEAB::TLOGIC},
	{"zoffx",XEAB::TINT},
	{"zoffy",XEAB::TINT},
	{"zoom",XEAB::TINT},
	{"zoom_size",XEAB::TZOOM},
	{"ztags",XEAB::TSTRING},
	{"zwaitfor",XEAB::TSTRING},
};
struct _KMAPData
{
	XPCTSTR strName;
	int		nValue;
};
static _KMAPData _KMAPTable[]=
{ 
	{"L_FUNC",XK_LEFTSOFT},
	{"R_FUNC",XK_RIGHTSOFT},
	{"CALL",XK_CALL},
	{"END",XK_CLOSE},
	{"MENU",XK_MENU},
	{"CLR",XK_CLEAR},
	{"LEFT",XK_LEFT},
	{"RIGHT",XK_RIGHT},
	{"UP",XK_UP},
	{"DOWN",XK_DOWN},
	{"OK",XK_RETURN},
	{"NUM0",XK_NUM0},
	{"NUM1",XK_NUM1},
	{"NUM2",XK_NUM2},
	{"NUM3",XK_NUM3},
	{"NUM4",XK_NUM4},
	{"NUM5",XK_NUM5},
	{"NUM6",XK_NUM6},
	{"NUM7",XK_NUM7},
	{"NUM8",XK_NUM8},
	{"NUM9",XK_NUM9},
	{"STAR",XK_STAR},
	{"WELL",XK_WELL},
	{"SPACE",XK_SPACE},
	{"BS",	XK_BACKSPACE},
};

#define ATTRCOUNT sizeof(XEnumAttribs::_attribs)/sizeof(XPCTSTR)
//static XSortStr _attrFind(XEnumAttribs::_attribs,ATTRCOUNT);

XU32 XEnumAttribs::GetCount(){return ATTRCOUNT;}
void* XEnumAttribs::GetData(XU32 id)
{
	if(id<0||id>=ATTRCOUNT)
		return XNULL;
	return (void*)_attribs[id].strName;
}

/*XINT XEnumAttribs::IndexOf(XPCTSTR strName)
{
XINT id=Index(strName,XSS_NOCASE);
if(id<0) return id;
return _attribs[id].nID;
}
XPCTSTR XEnumAttribs::GetName(int id)
{
if(id<0||id>=ATTRCOUNT) return XNULL;
return _attribs[id].strName;
}
XINT XEnumAttribs::GetID(int id)
{
if(id<0||id>=ATTRCOUNT) return -1;
return _attribs[id].nID;
}*/
XU8 XEnumAttribs::GetType(int id)
{
	if(id<0||id>=ATTRCOUNT) return TSTRING;
	return _attribs[id].nType;
}


const char* XEnumAttribs::GetName(int id)
{
	if(id<0||id>=ATTRCOUNT) return XNULL;
	return _attribs[id].strName;
}

#define VALUEOF(LIST,INDEX) (INDEX<sizeof(LIST)/sizeof(XPCTSTR)?LIST[INDEX]:LIST[0])
//#define PECENT(STRING) (STRING[0]==XPEC?-STRING.ToInt():STRING.ToInt())

void XEnumAttribs::FormatVar(XVar&var,XCSS*pCss)
{
	//return;
	if(var.nID<0||var.nID>=ATTRCOUNT) return;
	if(var.nType!=VSTRING) return;
	switch(_attribs[var.nID].nType)
	{
		/*case TCLASS:
		if(var.nType==VSTRING)
		var=pCss->CreateID(var.strData,XCSS::XCSS_CLASS);
		break;*/
	//case TSWFTYPE:
	//	var.ToString();
	//	switch(var.strData[0])
	//	{
	//	case 'G':
	//	case 'g':
	//		var=XConfig::GAME;break;
	//	default:
	//		var=XConfig::MEDIA;break;
	//	}break;
	case TROTATE:
		var.ToString();
		if(strchr(var.strData,'d')!=XNULL)
		{
			var=(int)(10000*sin(var.ToFloat()*3.1415926/180));
		}
		else
			var.ToInt();
		break;
	case TACCDIR:
		var.ToString();
		switch(var.strData[0])
		{
		case '-':
		case 'r':
		case 'R':
			switch(var.strData[1])
			{
			default:
			case 'x':
			case 'X':
				var=_ACCMAP::RX;
				break;
			case 'y':
			case 'Y':
				var=_ACCMAP::RY;
				break;
			case 'z':
			case 'Z':
				var=_ACCMAP::RZ;
				break;
			}
			break;
		default:
		case 'x':
		case 'X':
			var=_ACCMAP::X;
			break;
		case 'y':
		case 'Y':
			var=_ACCMAP::Y;
			break;
		case 'z':
		case 'Z':
			var=_ACCMAP::Z;
			break;
		}
		break;
	case TQUALITY:
		//var.ToString();
		//switch(var.strData[0])
		//{
		//default:
		//case 'a':
		//case 'A':
		//	var=XConfig::AUTO;
		//	break;
		//case 'l':
		//case 'L':
		//	var=XConfig::LOW;
		//	break;
		//case 'H':
		//case 'h':
		//	var=XConfig::HIGH;
		//	break;
		//}
		var = 0;
		break;
	case TKEY:
		var.ToString();
		switch(var.strData[1])
		{
		case 0:
			var=(int)var.strData[0];
			break;
		default:
			{
				int i,nc=sizeof(_KMAPTable)/sizeof(_KMAPData);
				XString8 str(var.strData);
				str.MakeUpper();
				XBOOL bOK=XFALSE;
				for(i=0;i<nc;i++)
				{
					if(str==_KMAPTable[i].strName)
					{
						var=_KMAPTable[i].nValue;
						bOK=XTRUE;
					}
				}
				if(!bOK)
					var=str.ToInt();
			}break;
		}
		break;
	case TSCORETYPE:
		var.ToString();
		switch(var.strData[0])
		{
		default:
			var=XConfig::SCOREVAR;
			break;
		case 'O':
		case 'o':
			var=XConfig::SCOREOBJ;
			break;
		}
		break;
	case TSCDIR:
		var.ToString();
		switch(var.strData[0])
		{
		case 'a':
		case 'A':
			var=TAUTO;
			break;
		case 'v':
		case 'V':
			var=TVERT;
			break;
		default:
			var=THORZ;
			break;
		}break;
	case TZOOM:
		var.ToString();
		switch(var.strData[0])
		{
		case 'f':
        case 'F':
                var = 2;
                break;
		case 'o':
		case 'O':
//			var=TORIGINAL;
//			break;
        default:
            var=TFIT;
            break;
		}break;
	case TMAPID:
		var=pCss->CreateID(var.strData+1,XCSS::XCSS_ID);
		break;
	case TSHAPE:
		var=XEnumListType::GetShapeType(var.strData);
		break;
	case TSOUNDDEVICE:
		var.ToString();
		switch(var.strData[0])
		{
		default:
		case 's':
		case 'S':
			var=TSPEAKER;
			break;
		case 'p':
		case 'P':
			var=TPHONE;
			break;
		case 'E':
		case 'e':
			var=TEARSET;break;
		}
		break;
	case TID:
		//if(var.nType==VSTRING)
		var=pCss->CreateID(var.strData,XCSS::XCSS_ID);
		break;
	case TFRAME:
		var=XEnumFrame().Index(var.strData);break;
	case TSTRING:var.ToString();break;
	case TPTYPE:
		var.ToString();
		if(!var.strData[0])
			var=TP_FLOAT;
		else
		{
			switch(var.strData[1])
			{
			case 'l':
			case 'L':
			default:
				var=TP_FLOAT;break;
			case 'i':
			case 'I':
				var=TP_FIXED;break;
			}
		}
		break;
	case TINT:
		var.ToInt();
		break;
	case TPECENT:
		{
			XString8 str(var.DataTo(),STRING_ATTACH);
			//var.DataTo(str);
			var=str.ToInt(10,XTRUE);//PECENT(str);
		}break;
		var=XEnumColor().ColorOf(var.strData,XTRUE);
		break;
	case TCOLOR:
		var=XEnumColor().ColorOf(var.strData);
		break;
	case TMOUSEACT:
		var=XEnumMouseAct().Index(var.strData,XString8::XLS_NOCASE);
		break;
	case TALIGN:
		var=XEnumAlign().Index(var.strData,XString8::XLS_NOCASE);
		break;
	case TINPUT:
		{
			int v=XEIT().Index(var.strData);
			if(v<0) v=XEnumListType::GetType(var.strData);
			var=v;
		}
		break;
	case TLAN:
		var=XEnumLan().Index(var.strData);
		break;
	case TCHARSET:
		{
			XString8 str(var.DataTo(),STRING_ATTACH);
			//var.DataTo(str);
			var=XEnumCharset::Index(str);
		}break;
	case TCHAR:
		if(var.strData!=XNULL)
			var=(XWCHAR)var.strData[0];
		else
			var=0;
		break;
	case TMARK:
		var=XTRUE;break;
	case TLOGIC:
		var.ToLogic();
		break;
	case TFORMAT:
		if(var.strData!=XNULL)
		{
			char c=var.strData[0];
			//for(XU32 i=1;i<strlen(p->strData);i++)
			int i=0;
			while(c)
			{
				if(c<'0'||c>'9') break;
				i++;
				c=var.strData[i];
			}
			XU8 nType=0;//m_edit.GetEditStyle()&~XEDS_ANY;
			switch(c)
			{
			case 'A':nType|=XEDS_UCHAR|XEDS_SIGN;break;
			case 'a':nType|=XEDS_LCHAR|XEDS_SIGN;break;
			case 'N':nType|=XEDS_NUMBER;break;
			case 'X':nType|=XEDS_UCHAR|XEDS_WCHAR;break;
			case 'x':nType|=XEDS_LCHAR|XEDS_WCHAR;break;
			default:
			case 'M':
			case 'm':nType=XEDS_ANY;break;
			}
			var=nType;
		}
		else
			var=XEDS_ANY;
		break;
	case TMETHOD:
		{
			XString8 str(var.DataTo(),STRING_ATTACH);
			//var.DataTo(str);
			var=XEM::Index(str);
		}break;
	}
}


XPCTSTR XEnumHeader::_strHeaders[]=
{
	"accept",//0
	"accept-application",//1.20x32
	"accept-charset",//1
	"accept-encoding",//2
	"accept-language",//3
	"accept-ranges",//4
	"age",//5
	"allow",//6
	"appdesc",
	"appstatus",
	"authorization",//7
	"bearer-indication",//1.20x33
	"cache-control",//8
	"cfg_url",
	"channel",
	"client_id",
	"connection",//9
	"content-base",//10
	"content-disposition",//1.10x2E
	"content-encoding",//11
	"content-id",//1.30x40
	"content-language",//12
	"content-length",//13
	"content-location",//14
	"content-md5",//15
	"content-range",//16
	"content-type",//17
	"cookie",//1.30x42
	"date",//18
	"eip",
	"encoding-version",//1.30x43
	"etag",//1.10x13
	"expect",//1.30x38
	"expires",//1.10x14
	"from",//1.10x15
	"gtype",
	"host",//1.10x16
    "idfv",
	"if-match",//1.10x18
	"if-modified-since",//1.10x17
	"if-none-match",//1.10x19
	"if-range",//1.10x1A
	"if-unmodified-since",//1.10x1B
	"last-modified",//1.10x1D
	"location",//1.10x1C
	"max-forwards",//1.10x1E
	"mobile_type",
	"newapp",
	"onsale",
	"outlogin",

	"pragma",//1.10x1F
	"profile",//1.20x35
	"profile-diff",//1.20x36
	"profile-warning",//1.20x37
	"proxy-authenticate",//1.10x20
	"proxy-authorization",//1.10x21
	"public",//1.10x22
	"push-flag",//1.20x34
	"range",//1.10x23
	"referer",//1.10x24
	"retry-after",//1.10x25
	"server",//1.10x26
	"set-cookie",//1.30x41
    "showad",
	"showbar",
	"stype",
    "sysvers",
	"te",//1.30x39
	"trailer",//1.30x3A
	"transfer-encoding",//1.10x27
	"upgrade",//1.10x28
	"user-agent",//1.10x29
	"vary",//1.10x2A
	"via",//1.10x2B
	"warning",//1.10x2C
	"www-authenticate",//1.10x2D
	"x-filter-url",
	"x-powered-by",
	"x-wap-application-id",//1.20x2F
	"x-wap-content-uri",//1.20x30
	"x-wap-initiator-uri",//1.20x31
	"x-wap-tod",//1.30x3F
	"xtoken",
	//"XID",
};

#define HDCOUNT sizeof(XEnumHeader::_strHeaders)/sizeof(XPCTSTR)

XEnumHeader::XEnumHeader():XSortConstStr(XEnumHeader::_strHeaders,HDCOUNT)
{
}

XPCTSTR XEnumLan::_strLanList[]=
{
	"abkhazian;ab;",//0x02
	"afar;aa;",//0x01
	"afrikaans;af;",//0x03
	"albanian;sq;",//0x6B
	"amharic;am;",//0x04
	"arabic;ar;",//0x05
	"armenian;hy;",//0x2ETajiktg0x74
	"assamese;as;",//0x06
	"aymara;ay;",//0x07
	"azerbaijani;az;",//0x08
	"bashkir;ba;",//0x09
	"basque;eu;",//0x1D
	"bengali;bangla;bn",//0x0E
	"bhutani;dz;",//0x17
	"bihari;bh;",//0x0C
	"bislama;bi;",//0x0D
	"breton;br;",//0x10
	"bulgarian;bg;",//0x0B
	"burmese;my;",//0x4F
	"byelorussian;be;",//0x0A
	"cambodian;km;",//0x3BUrduur0x21
	"catalan;ca;",//0x11
	"chinese;ch;"
	"chinese;zh;",//0x41
	"corsican;co;",//0x12
	"croatian;hr;",//0x2CTamilta0x72
	"czech;cs;",//0x13
	"danish;da;",//0x15
	"dutch;nl;",//0x52
	"english;en;",//0x19
	"esperanto;eo;",//0x1A
	"estonian;et;",//0x1C
	"faeroese;fo;",//0x82Sloveniansl0x67
	"fiji;fj;",//0x20
	"finnish;fi;",//0x1F
	"french;fr;",//0x22
	"frisian;fy;",//0x83Shonasn0x69
	"galician;gl;",//0x26
	"georgian;ka;",//0x38Twitw0x7E
	"german;de;",//0x16
	"greek;el;",//0x18
	"greenlandic;kl;",//0x8AUkrainianuk0x50
	"guarani;gn;",//0x27
	"gujarati;gu;",//0x28
	"hausa;ha;",//0x29
	"hebrew;he;",//0x2A
	"hindi;hi;",//0x2BSwahilisw0x71
	"hungarian;hu;",//0x2DTelugute0x73
	"icelandic;is;",//0x33Setswanatn0x79
	"indonesian;id;",//0x30Tigrinyati0x76
	"interlingua;ia;",//0x84Thaith0x75
	"interlingue;ie;",//0x86Turkmentk0x77
	"inuktitut;iu;",//0x89Turkishtr0x7B
	"inupiak;ik;",//0x87Tagalogtl0x78
	"irish;ga;",//0x24
	"italian;it;",//0x34Tongato0x7A
	"japanese;ja;",//0x36Tsongats0x7C
	"javanese;jw;",//0x37Tatartt0x7D
	"kannada;kn;",//0x3CUzbekuz0x23
	"kashmiri;ks;",//0x3EVolapukvo0x85
	"kazakh;kk;",//0x39Uighurug0x7F
	"kinyarwanda;rw;",//0x60
	"kirghiz;ky;",//0x40Xhosaxh0x32
	"kirundi;rn;",//0x5D
	"kurdish;ku;",//0x3FWolofwo0x31
	"laothian;lo;",//0x43Zhuangza0x3A
	"latin;la;",//0x8BYiddish(formerlyji)yi0x88
	"latvian;lettish;lv;",//0x45Zuluzu0x5C
	"lingala;ln;",//0x42Yorubayo0x35
	"lithuanian;lt;",//0x44Chinesezh0x41
	"macedonian;mk;",//0x48
	"malagasy;mg;",//0x46
	"malay;ms;",//0x4D
	"malayalam;ml;",//0x49
	"maltese;mt;",//0x4E
	"maori;mi;",//0x47
	"marathi;mr;",//0x4C
	"moldavian;mo;",//0x4B
	"mongolian;mn;",//0x4A
	"nauru;na;",//0x81
	"nepali;ne;",//0x51
	"norwegian;no;",//0x53
	"occitan;oc;",//0x54
	"oriya;or;",//0x56
	"oromo;om;",//0x55
	"persian;fa;",//0x1E
	"polish;po;",//0x58
	"portuguese;pt",//0x5A
	"punjabi;pa;",//0x57
	"pushto;ps;",//0x59
	"quechua;qu;",//0x5B
	"rhaeto-romance;rm;",//0x8C
	"romanian;ro;",//0x5E
	"russian;ru;",//0x5F
	"samoan;sm;",//0x68
	"sangho;sg;",//0x63
	"sanskrit;sa;",//0x61
	"scots;gaelic;gd",//0x25
	"serbian;sr;",//0x6C
	"serbo-croatian;sh;",//0x64
	"sesotho;st;",//0x6E
	"setswana;tn;",//0x79
	"shona;sn;",//0x69
	"sindhi;sd;",//0x62
	"sinhalese;si;",//0x65
	"siswati;ss;",//0x6D
	"slovak;sk;",//0x66
	"slovenian;sl;",//0x67
	"somali;so;",//0x6A
	"spanish;es;",//0x1B
	"sundanese;su;",//0x6F
	"swahili;sw;",//0x71
	"swedish;sv;",//0x70
	"tagalog;tl;",//0x78
	"tajik;tg;",//0x74
	"tamil;ta;",//0x72
	"tatar;tt;",//0x7D
	"telugu;te;",//0x73
	"thai;th;",//0x75
	"tibetan;bo;",//0x0F
	"tigrinya;ti;",//0x76
	"tonga;to;",//0x7A
	"tsonga;ts;",//0x7C
	"turkish;tr;",//0x7B
	"turkmen;tk;",//0x77
	"twi;tw;",//0x7E
	"uighur;ug;",//0x7F
	"ukrainian;uk;",//0x50
	"undef;",//0x80
	"urdu;ur;",//0x21
	"uzbek;uz;",//0x23
	"vietnamese;vi;",//0x2F
	"volapuk;vo;",//0x85
	"welsh;cy;",//0x14
	"wolof;wo;",//0x31
	"xhosa;xh;",//0x32
	"yiddish;yi;",//0x88
	"yoruba;yo;",//0x35
	"zhuang;za;",//0x3A
	"zulu;zu;",//0x5C
};

#define LANCOUNT sizeof(XEnumLan::_strLanList)/sizeof(XPCTSTR)
XEnumLan::XEnumLan():XSortConstStr(XEnumLan::_strLanList,LANCOUNT)
{
}


XEnumCharset::ALIAS XEnumCharset::_aliCharset[]=
{
	{"big5",0x07ea},
	{"iso-10646-ucs-2",0x03e8}, // 1000
	{"iso-8859-1",0x04},
	{"iso-8859-2",0x05},// 5
	{"iso-8859-3",0x06},// 6
	{"iso-8859-4",0x07},//7
	{"iso-8859-5",0x08},//8
	{"iso-8859-6",0x09},//9
	{"iso-8859-7",0x0A},// 10
	{"iso-8859-8",0x0B},// 11
	{"iso-8859-9",0x0C},// 12
	{"shift_jis",0x11}, // 17
	{"us-ascii",0x03}, // 3
	{"utf-8",0x6A}, // 106
	{"gb2312",0x02}, //2
	{"gbk",0x02}, //2
	{"gb2314",0x02} //2
};
#define CHCOUNT sizeof(XEnumCharset::_aliCharset)/sizeof(XEnumCharset::ALIAS)

XINT XEnumCharset::Index(XString8&src)
{
	for(int i=0;i<CHCOUNT;i++)
	{
		if(src.Compare(_aliCharset[i].strName,XTRUE)==0) return i;
	}
	return -1;
}

XINT XEnumCharset::Index(XINT ID)
{
	for(int i=0;i<CHCOUNT;i++)
	{
		if(ID==_aliCharset[i].ID) return i;
	}
	return -1;
}

XPCTSTR XEnumCharset::Key(XINT nIndex)
{
	if(nIndex<0||nIndex>=CHCOUNT) return XNULL;
	return _aliCharset[nIndex].strName;
}

XPCTSTR XEnumAlign::_strAlign[]=
{
	"bottom",
	"center",
	"left",
	"middle",
	"no-repeat",
	"nowrap",
	"repeat",
	"repeat-x",
	"repeat-y",
	"right",
	"top",
	"wrap",
};

#define ALICOUNT sizeof(XEnumAlign::_strAlign)/sizeof(XPCTSTR)
XEnumAlign::XEnumAlign():XSortConstStr(_strAlign,ALICOUNT)
{

}

XPCTSTR XEnumMouseAct::_strAct[]=
{
	"a","b","down","left","left-key","posx","posy","right","right-key","up",
	"xkey_0","xkey_1","xkey_2","xkey_3","xkey_4","xkey_5","xkey_6","xkey_7","xkey_8","xkey_9",
	"xkey_a","xkey_b","xkey_c","xkey_d","xkey_down","xkey_e","xkey_enter","xkey_f","xkey_g",
	"xkey_h","xkey_i","xkey_j","xkey_k","xkey_l","xkey_left","xkey_m","xkey_n",
	"xkey_o","xkey_p","xkey_q","xkey_r","xkey_right","xkey_s","xkey_space","xkey_t","xkey_u","xkey_up",
	"xkey_v","xkey_w","xkey_x","xkey_y","xkey_z"
};

#define ACTCOUNT sizeof(XEnumMouseAct::_strAct)/sizeof(XPCTSTR)
XEnumMouseAct::XEnumMouseAct():XSortConstStr(_strAct,ACTCOUNT)
{

}

XPCTSTR XEnumArg::_strArgList[]=
{
	"charset",//1.10x01Well-known-charset
	"comment",//1.30x0CText-string
	"differences",//1.10x07Field-name
	"domain",//1.30x0DText-string
	"filename",//1.10x06Text-string
	"level",//1.10x02Version-value
	"max-age",//1.30x0EDelta-seconds-value
	"name",//1.10x05Text-string
	"padding",//1.10x08Short-integer
	"path",//1.30x0FText-string
	"q",//1.10x00Q-value
	"reserve",
	"secure",//1.30x10No-value
	"start",//(withmultipart/related)1.20x0AText-string
	"start-info",//(withmultipart/related)1.20x0BText-string
	"type",//(whenusedasparameterofContent-Type:multipart/related)1.20x09Constrained-encoding
};

#define ARGCOUNT sizeof(XEnumArg::_strArgList)/sizeof(XPCTSTR)
XEnumArg::XEnumArg():XSortConstStr(XEnumArg::_strArgList,ARGCOUNT)
{
}


XPCTSTR XEnumType::_strTypes[]=
{
	"accept",
	"delete",
	"help",
	"options",
	"prev",
	"reset",
	"unknown",
	"vnd.*"
	"x-*",
};

#define TPCOUNT sizeof(XEnumType::_strTypes)/sizeof(XPCTSTR)
XEnumType::XEnumType():XSortConstStr(XEnumType::_strTypes,TPCOUNT)
{
}

XPCTSTR XEnumMethod::_strMethod[]={"GET","POST"};

XINT XEnumMethod::Index(XString8&strKey)
{
	if(strKey.Compare(_strMethod[POST],XTRUE)==0) return POST;
	return GET;
}

XPCTSTR XEnumMethod::Key(XINT id)
{
	if(id<0||id>=2) return XNULL;
	return _strMethod[id];
}

XEnumAlias::ALIAS XEnumAlias::_aliaChars[]=
{
	//{"nbsp",160},
	{"Alpha",'A'},//913},
	{"Beta",914},
	{"Chi",935},
	{"Delta",916},
	{"Epsilon",917},
	{"Eta",919},
	{"Gamma",915},
	{"Iota",921},
	{"Kappa",922},
	{"Lambda",923},
	{"Mu",924},
	{"Nu",925},
	{"Omega",937},
	{"Omicron",927},
	{"Phi",934},
	{"Pi",928},
	{"Prime",8243},
	{"Psi",936},
	{"Rho",929},
	{"Sigma",931},
	{"Tau",932},
	{"Theta",920},
	{"Upsilon",933},
	{"Xi",926},
	{"Zeta",918},
	{"acute",180},
	{"alefsym",8501},
	{"alpha",945},
	{"amp",38},
	{"and",8869},
	{"ang",8736},
	{"apos",39},
	{"asymp",8773},
	{"beta",946},
	{"brvbar",166},
	{"bull",8226},
	{"cap",8745},
	{"cent",162},
	{"chi",967},
	{"clubs",9827},
	{"cong",8773},
	{"copy",169},
	{"crarr",8629},
	{"cup",8746},
	{"curren",164},
	{"dArr",8659},
	{"darr",8595},
	{"deg",176},
	{"delta",948},
	{"diams",9830},
	{"empty",8709},
	{"epsilon",949},
	{"equiv",8801},
	{"eta",951},
	{"exist",8707},
	{"forall",8704},
	{"frasl",8260},
	{"gamma",947},
	{"ge",8805},
	{"gt",62},
	{"hArr",8660},
	{"harr",8596},
	{"hearts",9829},
	{"hellip",8230},
	{"iexcl",161},
	{"image",8465},
	{"infin",8734},
	{"int",8747},
	{"iota",953},
	{"isin",8712},
	{"kappa",954},
	{"lArr",8656},
	{"lambda",955},
	{"laquo",171},
	{"larr",8592},
	{"lceil",8968},
	{"le",8804},
	{"lfloor",8970},
	{"lowast",8727},
	{"loz",9674},
	{"lt",60},
	{"macr",175},
	{"micro",181},
	{"middot",183},
	{"minus",8722},
	{"mu",956},
	{"nabla",8711},
	{"nbsp",32},
	{"ne",8800},
	{"ni",8715},
	{"not",172},
	{"notin",8713},
	{"nsub",8836},
	{"nu",957},
	{"oline",8254},
	{"omega",969},
	{"omicron",959},
	{"oplus",8853},
	{"or",8870},
	{"ordf",170},
	{"otimes",8855},
	{"part",8706},
	{"perp",8869},
	{"phi",966},
	{"pi",960},
	{"piv",982},
	{"plusmn",177},
	{"pound",163},
	{"prime",8242},
	{"prod",8719},
	{"prop",8733},
	{"psi",968},
	{"quot",34},
	{"rArr",8658},
	{"raquo",8594},
	{"rarr",8594},
	{"rceil",8969},
	{"real",8476},
	{"reg",174},
	{"rfloor",8971},
	{"rho",961},
	{"sdot",8901},
	{"sect",167},
	{"shy",173},
	{"shy",173},
	{"sigma",963},
	{"sigmaf",962},
	{"sim",8764},
	{"spades",9824},
	{"sub",8834},
	{"sube",8838},
	{"sum",8722},
	{"sup",8835},
	{"sup2",178},
	{"sup3",179},
	{"supe",8839},
	{"tau",964},
	{"there4",8756},
	{"theta",952},
	{"thetasym",977},
	{"trade",8482},
	{"uArr",8657},
	{"uarr",8593},
	{"uml",168},
	{"upsih",978},
	{"upsilon",965},
	{"vradic",8730},
	{"weierp",8472},
	{"xi",958},
	{"yen",165},
	{"zeta",950},

};

#define ALIACOUNT sizeof(_aliaChars)/sizeof(ALIAS)

XU32 XEnumAlias::GetCount(){return ALIACOUNT;}
void* XEnumAlias::GetData(XU32 id)
{
	if(id>=ALIACOUNT) return XNULL;
	return (void*)_aliaChars[id].strName;
}

XWCHAR XEnumAlias::Char(XPCTSTR src)
{
	int id=Index((void*)src,XSS_NORMAL);
	if(id<0) return -1;
	return _aliaChars[id].ID;
	/*for(int i=0;i<sizeof(_aliaChars)/sizeof(ALIAS);i++)
	{
	if(src==_aliaChars[i].strName) return
	_aliaChars[i].ID;
	}
	return -1;*/

}


/*XPCTSTR XEnumAlias::Name(XWCHAR c)
{
int id=Index(c);
if(id<0) return XNULL;
return _aliaChars[id].strName;

}*/

/*XPCTSTR XEnumAlias::operator [](XU32 nIndex)
{
if(nIndex>=sizeof(_aliaChars)/sizeof(ALIAS)) return XNULL;
return _aliaChars[nIndex].strName;
} */

XPCTSTR XEnumInputType::_strTextType[]=
{
	"button",
	"checkbox",
	"file",
	"hidden",
	"image",
	"password",
	"radio",
	"reset",
	"submit",
	"text",
	"text/baiwanscript",
	"text/css",
	"text/javascript",
	"url"
};

#define TTCOUNT sizeof(_strTextType)/sizeof(XPCTSTR)


XEnumInputType::XEnumInputType():XSortConstStr(_strTextType,TTCOUNT)
{
}


XEnumHttpStatus::_HTTPSTATUS XEnumHttpStatus::_httpStatus[]=
{
	{XERR_OK,"Success",0},//"�ɹ�",0},
	{XERR_CANT_CONNECT,"Can't Connect Server",1},//"�޷����ӷ�����",1},
	{XERR_NO_RESPONSE,"No Response",2},//������û�з�Ӧ", 2},
	{XERR_USER_CANCELED,"User Canceled",3},//�û�ȡ��",3},
	{XERR_DOMAN,"Doman Error",4},//���ܻ�ȡ����",4},
	{XERR_SEND_ERROR,"Send Data Error",5},//"������ݴ���",5},
	{XERR_REV_ERROR,"Recive Data Error",6},//������ݴ���",6},
	{XERR_BAD_PROTO,"Protocols Error",7},//��������Э������",7},
	{XERR_UNKNOW,"Unknow",8},//δ֪����",8},
	{XERR_NOT_EXIST,"File Not Exist",9},//�ļ���Ŀ¼������",9},
	{XHTTP_STATUS_CONTINUE,"Continue",0x10},
	{XHTTP_STATUS_SWITCH_PROTOCOLS,"Switching Protocols",0x11},
	{XHTTP_STATUS_OK,"OK, Success",0x20},
	{XHTTP_STATUS_CREATED,"Created",0x21},
	{XHTTP_STATUS_ACCEPTED,"Accepted",0x22},
	{XHTTP_STATUS_PARTIAL,"Non-Authoritative Information",0x23},
	{XHTTP_STATUS_NO_CONTENT,"No Content",0x24},
	{XHTTP_STATUS_RESET_CONTENT,"Reset Content",0x25},
	{XHTTP_STATUS_PARTIAL_CONTENT,"Partial Content",0x26},
	{XHTTP_STATUS_AMBIGUOUS,"Multiple Choices",0x30},
	{XHTTP_STATUS_MOVED,"Moved Permanently",0x31},
	{XHTTP_STATUS_REDIRECT,"Moved temporarily",0x32},
	{XHTTP_STATUS_REDIRECT_METHOD,"See Other",0x33},
	{XHTTP_STATUS_NOT_MODIFIED,"Not modified",0x34},
	{XHTTP_STATUS_USE_PROXY,"Use Proxy",0x35},
	{XHTTP_STATUS_306,"(reserved)",0x36},
	{XHTTP_STATUS_REDIRECT_KEEP_VERB,"Temporary Redirect",0x37},
	{XHTTP_STATUS_BAD_REQUEST,"Bad Request",0x40},
	{XHTTP_STATUS_DENIED,"Unauthorized",0x41},
	{XHTTP_STATUS_PAYMENT_REQ,"Payment required",0x42},
	{XHTTP_STATUS_FORBIDDEN,"Forbidden",0x43},
	{XHTTP_STATUS_NOT_FOUND,"Not Found",0x44},
	{XHTTP_STATUS_BAD_METHOD,"Method not allowed",0x45},
	{XHTTP_STATUS_NONE_ACCEPTABLE,"Not Acceptable",0x46},
	{XHTTP_STATUS_PROXY_AUTH_REQ,"Proxy Authentication required",0x47},
	{XHTTP_STATUS_REQUEST_TIMEOUT,"Request Timeout",0x48},
	{XHTTP_STATUS_CONFLICT,"Conflict",0x49},
	{XHTTP_STATUS_GONE,"Gone",0x4A},
	{XHTTP_STATUS_LENGTH_REQUIRED,"Length Required",0x4B},
	{XHTTP_STATUS_PRECOND_FAILED,"Precondition failed",0x4C},
	{XHTTP_STATUS_REQUEST_TOO_LARGE,"Request entity too large",0x4D},
	{XHTTP_STATUS_URI_TOO_LONG,"Request-URI too large",0x4E},
	{XHTTP_STATUS_UNSUPPORTED_MEDIA,"Unsupported media type",0x4F},
	{XHTTP_STATUS_OUT_RANGE,"Requested Range Not Satisfiable",0x50},
	{XHTTP_STATUS_EXPECTATION,"Expectation Failed",0x51},
	{XHTTP_STATUS_SERVER_ERROR,"Internal Server Error",0x60},
	{XHTTP_STATUS_NOT_SUPPORTED,"Not Implemented",0x61},
	{XHTTP_STATUS_BAD_GATEWAY,"Bad Gateway",0x62},
	{XHTTP_STATUS_SERVICE_UNAVAIL,"Service Unavailable",0x63},
	{XHTTP_STATUS_GATEWAY_TIMEOUT,"Gateway Timeout",0x64},
	{XHTTP_STATUS_VERSION_NOT_SUP,"HTTP version not supported",0x65}
};
/////////////////////////////////////////////////////////////////
#define HTSCOUNT sizeof(XEnumHttpStatus::_httpStatus)/sizeof(XEnumHttpStatus::_HTTPSTATUS)
class XEnumHttpStatusW:public XSort
{
public:
	XU32 GetCount(){return HTSCOUNT;}
	void* GetData(XU32 id)
	{
		return (void*)XEnumHttpStatus::_httpStatus[id].nWapCode;
	}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareInt((XINT)iSrt,(XINT)iDst);
	}
};

class XEnumHttpStatusE:public XSort
{
public:
	XU32 GetCount(){return HTSCOUNT;}
	void* GetData(XU32 id)
	{
		return (void*)XEnumHttpStatus::_httpStatus[id].nCode;
	}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareInt((XINT)iSrt,(XINT)iDst);
	}
};

XU16 XEnumHttpStatus::Match(XU8 nWapCode)
{
	/*for(int i=0;i<sizeof(_httpStatus)/sizeof(_HTTPSTATUS);i++)
	{
	if(_httpStatus[i].nWapCode==nWapCode)
	return _httpStatus[i].nCode;
	} */
	int id=XEnumHttpStatusW().Index((void*)nWapCode);
	if(id>=0)
		return _httpStatus[id].nCode;
	return XERR_UNKNOW;
}

XPCTSTR XEnumHttpStatus::GetErrorString(XU16 id)
{
	int i=XEnumHttpStatusE().Index((void*)id);
	if(i>=0) return _httpStatus[i].strError;
	return _httpStatus[XERR_UNKNOW].strError;
}
//////////////////////////////////////////////////////////////////////////////
XPCTSTR XEnumConfig::_strConfig[]={"bookmark","config","hot","nearby","today","update"};

#define CFGCOUNT sizeof(XEnumConfig::_strConfig)/sizeof(XPCTSTR)

XINT XEnumConfig::Index(XString8&strKey)
{
	return strKey.IndexOf(_strConfig,CFGCOUNT,-1,XString8::XLS_NOCASE);
}

XPCTSTR XEnumConfig::Key(XINT id)
{
	if(id<0||id>=CFGCOUNT) return XNULL;
	return _strConfig[id];
}


//---------------------------------------------------------------------------
//��ɫ���б�
//��ɫ�� ʮ�������ɫֵ ��ɫ
XEnumColor::COLORALIA XEnumColor::m_colors[]={
	{"AliceBlue",0xFFF0F8FF},
	{"AntiqueWhite",0xFFFAEBD7},
	{"Aqua",0xFF00FFFF},
	{"Aquamarine",0xFF7FFFD4},
	{"Azure",0xFFF0FFFF},
	{"Beige",0xFFF5F5DC},
	{"Bisque",0xFFFFE4C4},
	{"Black",0xFF000000},
	{"BlanchedAlmond",0xFFFFEBCD},
	{"Blue",0xFF0000FF},
	{"BlueViolet",0xFF8A2BE2},
	{"Brown",0xFFA52A2A},
	{"BurlyWood",0xFFDEB887},
	{"CadetBlue",0xFF5F9EA0},
	{"Chartreuse",0xFF7FFF00},
	{"Chocolate",0xFFD2691E},
	{"Coral",0xFFFF7F50},
	{"CornflowerBlue",0xFF6495ED},
	{"Cornsilk",0xFFFFF8DC},
	{"Crimson",0xFFDC143C},
	{"Cyan",0xFF00FFFF},
	{"DarkBlue",0xFF00008B},
	{"DarkCyan",0xFF008B8B},
	{"DarkGoldenRod",0xFFB8860B},
	{"DarkGray",0xFFA9A9A9},
	{"DarkGreen",0xFF006400},
	{"DarkKhaki",0xFFBDB76B},
	{"DarkMagenta",0xFF8B008B},
	{"DarkOliveGreen",0xFF556B2F},
	{"DarkOrchid",0xFF9932CC},
	{"DarkRed",0xFF8B0000},
	{"DarkSalmon",0xFFE9967A},
	{"DarkSeaGreen",0xFF8FBC8F},
	{"DarkSlateBlue",0xFF483D8B},
	{"DarkSlateGray",0xFF2F4F4F},
	{"DarkTurquoise",0xFF00CED1},
	{"DarkViolet",0xFF9400D3},
	{"Darkorange",0xFFFF8C00},
	{"DeepPink",0xFFFF1493},
	{"DeepSkyBlue",0xFF00BFFF},
	{"DimGray",0xFF696969},
	{"DodgerBlue",0xFF1E90FF},
	{"Feldspar",0xFFD19275},
	{"FireBrick",0xFFB22222},
	{"FloralWhite",0xFFFFFAF0},
	{"ForestGreen",0xFF228B22},
	{"Fuchsia",0xFFFF00FF},
	{"Gainsboro",0xFFDCDCDC},
	{"GhostWhite",0xFFF8F8FF},
	{"Gold",0xFFFFD700},
	{"GoldenRod",0xFFDAA520},
	{"Gray",0xFF808080},
	{"Green",0xFF008000},
	{"GreenYellow",0xFFADFF2F},
	{"HoneyDew",0xFFF0FFF0},
	{"HotPink",0xFFFF69B4},
	{"IndianRed",0xFFCD5C5C},
	{"Indigo",0xFF4B0082},
	{"Ivory",0xFFFFFFF0},
	{"Khaki",0xFFF0E68C},
	{"Lavender",0xFFE6E6FA},
	{"LavenderBlush",0xFFFFF0F5},
	{"LawnGreen",0xFF7CFC00},
	{"LemonChiffon",0xFFFFFACD},
	{"LightBlue",0xFFADD8E6},
	{"LightCoral",0xFFF08080},
	{"LightCyan",0xFFE0FFFF},
	{"LightGoldenRodYellow",0xFFFAFAD2},
	{"LightGreen",0xFF90EE90},
	{"LightGrey",0xFFD3D3D3},
	{"LightPink",0xFFFFB6C1},
	{"LightSalmon",0xFFFFA07A},
	{"LightSeaGreen",0xFF20B2AA},
	{"LightSkyBlue",0xFF87CEFA},
	{"LightSlateBlue",0xFF8470FF},
	{"LightSlateGray",0xFF778899},
	{"LightSteelBlue",0xFFB0C4DE},
	{"LightYellow",0xFFFFFFE0},
	{"Lime",0xFF00FF00},
	{"LimeGreen",0xFF32CD32},
	{"Linen",0xFFFAF0E6},
	{"Magenta",0xFFFF00FF},
	{"Maroon",0xFF800000},
	{"MediumAquaMarine",0xFF66CDAA},
	{"MediumBlue",0xFF0000CD},
	{"MediumOrchid",0xFFBA55D3},
	{"MediumPurple",0xFF9370D8},
	{"MediumSeaGreen",0xFF3CB371},
	{"MediumSlateBlue",0xFF7B68EE},
	{"MediumSpringGreen",0xFF00FA9A},
	{"MediumTurquoise",0xFF48D1CC},
	{"MediumVioletRed",0xFFC71585},
	{"MidnightBlue",0xFF191970},
	{"MintCream",0xFFF5FFFA},
	{"MistyRose",0xFFFFE4E1},
	{"Moccasin",0xFFFFE4B5},
	{"NavajoWhite",0xFFFFDEAD},
	{"Navy",0xFF000080},
	{"Null",0x00FFFFFF},
	{"OldLace",0xFFFDF5E6},
	{"Olive",0xFF808000},
	{"OliveDrab",0xFF6B8E23},
	{"Orange",0xFFFFA500},
	{"OrangeRed",0xFFFF4500},
	{"Orchid",0xFFDA70D6},
	{"PaleGoldenRod",0xFFEEE8AA},
	{"PaleGreen",0xFF98FB98},
	{"PaleTurquoise",0xFFAFEEEE},
	{"PaleVioletRed",0xFFD87093},
	{"PapayaWhip",0xFFFFEFD5},
	{"PeachPuff",0xFFFFDAB9},
	{"Peru",0xFFCD853F},
	{"Pink",0xFFFFC0CB},
	{"Plum",0xFFDDA0DD},
	{"PowderBlue",0xFFB0E0E6},
	{"Purple",0xFF800080},
	{"Red",0xFFFF0000},
	{"RosyBrown",0xFFBC8F8F},
	{"RoyalBlue",0xFF4169E1},
	{"SaddleBrown",0xFF8B4513},
	{"Salmon",0xFFFA8072},
	{"SandyBrown",0xFFF4A460},
	{"SeaGreen",0xFF2E8B57},
	{"SeaShell",0xFFFFF5EE},
	{"Sienna",0xFFA0522D},
	{"Silver",0xFFC0C0C0},
	{"SkyBlue",0xFF87CEEB},
	{"SlateBlue",0xFF6A5ACD},
	{"SlateGray",0xFF708090},
	{"Snow",0xFFFFFAFA},
	{"SpringGreen",0xFF00FF7F},
	{"SteelBlue",0xFF4682B4},
	{"Tan",0xFFD2B48C},
	{"Teal",0xFF008080},
	{"Thistle",0xFFD8BFD8},
	{"Tomato",0xFFFF6347},
	{"Turquoise",0xFF40E0D0},
	{"Violet",0xFFEE82EE},
	{"VioletRed",0xFFD02090},
	{"Wheat",0xFFF5DEB3},
	{"White",0xFFFFFFFF},
	{"WhiteSmoke",0xFFF5F5F5},
	{"Yellow",0xFFFFFF00},
	{"YellowGreen",0xFF9ACD32},
};

#define COLORCOUNT sizeof(m_colors)/sizeof(COLORALIA)

XU32 XEnumColor::GetCount()
{
	return COLORCOUNT;
}

void* XEnumColor::GetData(XU32 id)
{
	if(id>=COLORCOUNT) return XNULL;
	return (void*)m_colors[id].strColorName;
}

XU32 XEnumColor::Color(XString8& strColor,XU8 bCss)
{

	if(strColor.GetLength()<3) return 0;
	if(strColor[0]=='#')
	{
//#if (__CORE_VERSION__>=0x02077000)
		XU8 cc=strColor[1];
		if(cc=='N'||cc=='n')
			return 0;
//#endif
		XU32 c=strColor.ToInt(16)|0xff000000;
		if(bCss&&strColor.GetLength()<=4)
		{
			XU8 r=(XU8)(c&0xf);
			//r=r|(r<<4);
			XU8 g=(XU8)((c>>4)&0xf);
			//g=g|(g<<4);
			XU8 b=(XU8)((c>>8)&0xf);

			c=0xff000000|
				r|(r<<4)|(g<<8)|(g<<12)|(b<<16)|(b<<20);
		}
		return c;
	}
	//else if(strColor.Compare("tran",XTRUE,4)==0) return 1;
	else if(strColor.Compare("rgb(",XTRUE,4)==0)
	{
		XString8Array list;
		list.SplitString(strColor,',',XTRUE,4);
		XU32 nColor=0xff000000;
		if(list.GetSize()>=3)
		{
			int c=list[0].ToInt();
			if(c<0) c=-c*255/100;
			nColor|=((c&0xff)<<16);
			c=list[1].ToInt();
			if(c<0) c=-c*255/100;
			nColor|=((c&0xff)<<8);
			c=list[2].ToInt();
			if(c<0) c=-c*255/100;
			nColor|=((c&0xff));
		}
		return nColor;
	}
	else// return XEnumColor().Color(strColor);
	{
		int mi;
		int id=Index(strColor,mi,XSS_NOCASE);
		if(id<0)
		{
			if(strColor[0]>='0'&&strColor[1]<='9')
			{
				int nColor=(strColor.ToInt());
				if(nColor) nColor|=0xff000000;
				return nColor;
			}
			return 0;
		}
		else
			return m_colors[id].nColor;
	}
}

////////////////////////////////////////////////////////////////////
XPCTSTR XEnumFrame::_strFrames[]=
{
	"above",
	"all",
	"below",
	"border",
	"box",
	"cols",
	"groups",
	"hsides",
	"lhs",
	"none",
	"rhs",
	"rows",
	"void",
	"vsides",
};

#define FRMCOUNT sizeof(_strFrames)/sizeof(XPCTSTR)

XEnumFrame::XEnumFrame():XSortConstStr(_strFrames,FRMCOUNT)
{

}

static XPCTSTR _strFontSize[]=
{
	"xx-small",
	"x-small",
	"small",
	"medium ",
	"large",
	"x-large",
	"xx-large",
	"smaller",
	"larger"
};

XU8 XEnumListType::GetFontSize(XPCTSTR strSize)
{
	if(strSize==XNULL||strSize[0]==0)
		return 1;
	XString8 str(strSize,STRING_ATTACH);
	for(int i=0;i<sizeof(_strFontSize)/sizeof(XPCTSTR);i++)
	{
		if(str.Compare(_strFontSize[i],XTRUE)==0)
		{
			str.EmptyNoFree();
			//if(i<2) return 0;
			return i;
		}
	}
	int v=str.ToInt(10,XTRUE);
	if(v<0)
	{
		v=-(v+50)*6/100;
		//if(v>10) v=10;
	}
	else v=(v-12);
	v=XMAX(0,XMIN(v,10));
	str.EmptyNoFree();
	return v;
}

static XPCTSTR _strLineType[]=
{
	"dashed",
	"dotted",
	"double",
	"groove",
	"hidden",
	"inset",
	"medium",
	"none",
	"outset",
	"ridge",
	"solid",
	"thin",
	"think"
};


XU8 XEnumListType::GetBorderType(XPCTSTR strType)
{
	if(strType==XNULL) 
		return LS_EMPTY;
	XSortConstStr sort(_strLineType,sizeof(_strLineType)/sizeof(XPCTSTR));
	return sort.Index((void*)strType)+1;
}

static XPCTSTR _strDisplayMode[]=
{
	"none","inline","block",
	"list-item","run-in","compact",
	"marker","table","inline-table"
};

XU8 XEnumListType::GetDisplayMode(XPCTSTR strMode)
{
	if(strMode==XNULL) return DISPLAY_EMPTY;
	XString8 str(strMode,STRING_ATTACH);
	for(int i=0;i<sizeof(_strDisplayMode)/sizeof(XPCTSTR);i++)
	{
		if(str.Compare(_strDisplayMode[i],XTRUE)==0)
		{
			str.EmptyNoFree();
			return i+1;
		}
	}
	str.EmptyNoFree();
	return DISPLAY_EMPTY;
}
