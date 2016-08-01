#ifndef __XENUMDATA_H__
#define __XENUMDATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define XEAB XEnumAttribs
class XVar;
class XCSS;

enum {STA_NORMAL,STA_LINK,STA_ACTIVE,STA_SELECTED,STA_SET};

class _XEXT_CLASS XEnumAttribs : public XSort
{
public:
	enum {TP_FLOAT=0,TP_FIXED=1};
	struct _ATTR
	{
		XPCTSTR strName;
		XU8		nType;
	};
	enum {TVERT,THORZ,TAUTO};
	enum {TFIT,TORIGINAL};
	enum {TSPEAKER,TPHONE,TEARSET};
	enum 
	{
		TINT,TPECENT,TSTRING,
		TCOLOR,TALIGN,TURL,
		TINPUT,TLAN,TCHARSET,TCHAR,TMARK,
		TLOGIC,TFORMAT,TMETHOD,TFRAME,TID,TCLASS,TSHAPE,TMAPID,
		TPTYPE,TMOUSEACT,TSOUNDDEVICE,TZOOM,TSCDIR,TSCORETYPE,TKEY,TQUALITY,TACCDIR,TROTATE,
		TSWFTYPE
	};
	enum 
	{
		ABBR=0,
		ACCEPT_CHARSET,
		ACCESSKEY,
		ACCSPEED,
		ACTION,
		ALIGN,
		ALINK,
		ALLOWSCRIPTACCESS,
		ALT,
		AUTOTAG,
		AXIS,
		BACKGROUND,
		BGCOLOR,
		BLOCKINDENT,
		BORDER,
		BUTTON_HOR_POS,
		BUTTON_LONG_PRESS,
		BUTTON_LONG_PRESS_LOCK_TIME,

		BUTTON_MAP_TO,
		BUTTON_NAME,

		BUTTON_VER_POS,
		CACHE_CONTROL,
		CELLPADDING,
		CELLSPACING,
		CHAR,
		CHAROFF,
		CHARSET,
		CHECKED,
		CHUNKED,
		CLASS,
		COLOR,
		COLS,
		COLSPAN,
		COLUMNS,
		CONTENT,
		CONTENT_ID,
		CONTENT_LEVEL,
		COORDS,
		DATA_ATR,
		DEFAULT_BUTTON_FLOAT_ALIGN,
		DEFAULT_BUTTON_FLOAT_SIDE,
		DEFAULT_BUTTON_POSITION,
		DEFAULT_LONG_PRESS_LOCK_TIME,
		DEFAULT_MOUSE_MOVE_SPACE,
		DEFAULT_OPERATE_MODE,
		DIRECTION,
		DISABLED,
		DOMAIN1,
		EMPTYOK,
		ENCODING,
		ENCTYPE,
		XEVENT,
		FACE,
		FLASHVARS,
		FORMAT,
		FORUA,
		FRAME,
		HEADERS,//headers
		HEIGHT,
		HOTKEY_FOR_ZOOM,
		HREF,
		HSPACE,
		HTTP_EQUIV,
		ID,
		INAME,
		INDENT,
		XINTERFACE,
		IVALUE,
		KEY_MAP_TO,
		KEY_PRESSED,
		LABEL,
		LEADING,
		LEFTMARGIN,
		LETTERSPACING,
		LINK,
		LOCALSRC,
		LOOP,
		LONG_PRESS,
		LONG_PRESS_LOCK_TIME,
		MAXLENGTH,
		MD5ENC,
		METHOD,
		MODE,
		MOUSE_ACT,
		MULTIPLE,
		MUTE,
		NAME,
		NEWCONTEXT,
		NOSHADE,
		NOWRAP,
		ONCLICK,
		ONENTERBACKWARD,
		ONENTERFORWARD,
		ONKEYDOWN,
		ONKEYPRESS,
		ONKEYUP,
		ONLOAD,
		ONLYONE,
		ONMOUSEDOWN,
		ONMOUSEOUT,
		ONMOUSEOVER,
		ONMOUSEUP,
		ONPICK,
		ONTIMER,
		ONUNLOAD,
		OPTIONAL_X,
		ORDERED,
		ORG,
		PATH,
		PLAY,
		PLUGINSPAGE,
		QUALITY,
		READONLY,
		RIGHTMARGIN,
		ROWS,
		ROWSPAN,
		RULES,
		SCHEME,
		SCORE_TYPE,
		SCORE_VAR_NAME,
		SCREEN_DIRECTION,
		SENDREFERER,
		SHAPE,
		SIZE,
		SOUND_DEVICE,
		SRC,
		STEP,
		STYLE,
		SUBMIT_URL,
		TABINDEX,
		TABSTOPS,
		TARGET,
		TEXT,
		TITLE,
		TOPSCORE_URL,
		TOUCHSIZE,
		TYPE,
		USEMAP,
		VALIGN,
		VALUE,
		VERSION,
		VIRTUAL_KEY,
		VLINK,
		VOLUME,
		VSPACE,
		WIDTH,
		WMODE,
		XML_LANG,
		XML_SPACE,
		ZACC,
		ZADPOS,
		ZADURL,
		ZALIGN,
		ZBUTTON,
		ZCONTENT_TYPE,
		ZMOUSE_POS,
		ZOFFX,
		ZOFFY,
		ZOOM,
		ZOOM_SIZE,
		ZTAGS,
		ZWAITFOR,

		POSX=0x1000,
		LETTER_SPACING,
		TEXT_ALIGN,
		TEXT_INDENT,
		TEXT_DECORATION,
		//LINK_DECORATION,
		FONT_FAMILY,
		FONT_SIZE,
		FONT_STYLE,
		FONT_WEIGHT,
		FONT_HEIGHT,
		BORDER_COLLAPSE,
		BORDER_TOPWIDTH,
		BORDER_RIGHTWIDTH,
		BORDER_BOTTOMWIDTH,
		BORDER_LEFTWIDTH,
		BORDER_TOPSTYLE,
		BORDER_RIGHTSTYLE,
		BORDER_BOTTOMSTYLE,
		BORDER_LEFTSTYLE,
		BORDER_TOPCOLOR,
		BORDER_RIGHTCOLOR,
		BORDER_BOTTOMCOLOR,
		BORDER_LEFTCOLOR,
		MARGIN_TOP,
		MARGIN_RIGHT,
		MARGIN_BOTTOM,
		MARGIN_LEFT,
		PADDING_TOP,
		PADDING_RIGHT,
		PADDING_BOTTOM,
		PADDING_LEFT,
		POSITION_MODE,
		LIST_IMAGE,
		LIST_STYLE_TYPE,
		DISPLAY_MODE,
		HIDDENITEM,
		CSS_WIDTH,
		CSS_HEIGHT,
		POSY,
		CLEAR,
		ATTACH,
		POSITIONX,
		POSITIONY,
		BACKIMAGE,
		FLOAT,
		REPEAT,
		WORD_SPACING,
		TEXT_TRANSFORM,
		OVERFLOW1,
		POS_TOP,
		POS_LEFT,
		POS_RIGHT,
		POS_BOTTOM,
		LINE_HEIGHT,
		//KERNING,
	};
public:
	XPCTSTR operator [](int id){return _attribs[id].strName;}
	static _ATTR _attribs[];
	void* GetData(XU32 id);
	//XINT	GetIData(XU32 id);
	XU32	GetCount();
	XINT	Compare(void*iSrt,void*iDst,XU8 mode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,mode);
	}
	//XINT IndexOf(XPCTSTR strName);
	//XINT IndexOf(XU32 ID);
	//static _ATTR _strAttribs[];
	//static XPCTSTR GetName(int index);
	//static XINT GetID(int index);
	static XU8	GetType(int index);
	static const char* GetName(int index);
	static void FormatVar(XVar&var,XCSS*pCss);
	XEnumAttribs(){}
};

#define XEH XEnumHeader
class _XEXT_CLASS XEnumHeader:public XSortConstStr
{
public:
	enum 
	{
		ACCEPT=0,
		ACCEPT_APPLICATION,
		ACCEPT_CHARSET,
		ACCEPT_ENCODING,
		ACCEPT_LANGUAGE,
		ACCEPT_RANGES,
		AGE,
		ALLOW,
		APPDESC,
		APPSTATUS,
		AUTHOR,
		BEARER_INDICATION,
		CACHE_CONTROL,
		CONFIG_URL,
		CHANNEL_ID,
		CLIENT_ID,
		CONNECTION,
		CONTENT_BASE,
		CONTENT_DISPOSITION,
		CONTENT_ENCODING,
		CONTENT_ID,
		CONTENT_LANGUAGE,
		CONTENT_LENGTH,
		CONTENT_LOCATION,
		CONTENT_MD5,
		CONTENT_RANGE,
		CONTENT_TYPE,
		COOKIE,
		DATE,
		EIP,
		ENCODING_VERSION,
		ETAG,
		EXPECT,
		EXPIRES,
		FROM,
		GTYPE,
		HOST,
        IDFV,
		IF_MATCH,
		IF_MODIFIED_SINCE,
		IF_NONE_MATCH,
		IF_RANGE,
		IF_UNMODIFIED_SINCE,
		LAST_MODIFIED,
		LOCATION,
		MAX_FORWARDS,
		MOBILE_TYPE,
		NEWAPP,
		ONSALE,
		OUTLOGIN,
		PRAGMA,
		PROFILE,
		PROFILE_DIFF,
		PROFILE_WARNING,
		PROXY_AUTHENT,
		PROXY_AUTHOR,
		PUBLIC,
		PUSH_FLAG,
		RANGE,
		REFERER,
		RETRY_AFTER,
		SERVER,
		SET_COOKIE,
        SHOWAD,
		SHOWBAR,
		STYPE,
        SYSVERS,
		TE,
		TRAILER,
		TRANSFER_ENCODING,
		UPGRADE,
		USER_AGENT,
		VARY,
		VIA,
		WARNING,
		WWW_AUTHENT,
		X_FILTER_URL,
		X_POWERED_BY,
		X_WAP_APPLICATION_ID,
		X_WAP_CONTENT_URI,
		X_WAP_INITIATOR_URI,
		X_WAP_TOD,
		XTOKENID,
		//	XCLIENTID,
	};
public:
	XPCTSTR operator [](int id)
	{
		return _strHeaders[id];
	}
	static XPCTSTR _strHeaders[];
	XEnumHeader();
};



class _XEXT_CLASS XEnumLan:public XSortConstStr
{
public:
	enum 
	{
		ABKHAZIAN=0,
		AFAR,
		AFRIKAANS,
		ALBANIAN,
		AMHARIC,
		ARABIC,
		ARMENIAN,
		ASSAMESE,
		AYMARA,
		AZERBAIJANI,
		BASHKIR,
		BASQUE,
		BENGALI,
		BHUTANI,
		BIHARI,
		BISLAMA,
		BRETON,
		BULGARIAN,
		BURMESE,
		BYELORUSSIAN,
		CAMBODIAN,
		CATALAN,
		CHINESE,
		CORSIAN,
		CROATIAN,
		CZECH,
		DANISH,
		DUTCH,
		ENGLISH,
		ESPERANTO,
		ESTONIAN,
		FAEROESE,
		FIJI,
		FINNISH,
		FRENCH,
		FRISIAN,
		GALICIAN,
		GEORGIAN,
		GERMAN,
		GREEK,
		GREENLANDIC,
		GUARANI,
		GUJARATI,
		HAUSA,
		HEBREW,
		HINDI,
		HUNGARIAN,
		ICELANDIC,
		INDONESIAN,
		INTERLINGUA,
		INTERLINGUE,
		INUKTITUT,
		INUPIAK,
		IRISH,
		ITALIAN,
		JAPANESE,
		JAVANESE,
		KANNADA,
		KASHMIRI,
		KAZAKH,
		KINYARWANDA,
		KIRGHIZ,
		KIRUNDI,
		KURDISH,
		LAOTHIAN,
		LATIN,
		LATVIAN,
		LINGALA,
		LITHUANIAN,
		MACEDONIAN,
		MALAGASY,
		MALAY,
		MALAYALAM,
		MALTESE,
		MAORI,
		MARATHI,
		MOLDAVIAN,
		MONGOLIAN,
		NAURU,
		NEPALI,
		NORWEGIAN,
		OCCITAN,
		ORIYA,
		OROMO,
		PERSIAN,
		POLISH,
		PORTUGUESE,
		PUNJABI,
		PUSHTO,
		QUECHUA,
		RHAETO_ROMANCE,
		ROMANIAN,
		RUSSIAN,
		SAMOAN,
		SANGHO,
		SANSKRIT,
		SCOTS,
		SERBIAN,
		SERBO_CROATIAN,
		SESOTHO,
		SETSWANA,
		SHONA,
		SINDHI,
		SINHALESE,
		SISWATI,
		SLOVAK,
		SLOVENIAN,
		SOMALI,
		SPANISH,
		SUNDANESE,
		SWAHILI,
		SWEDISH,
		TAGALOG,
		TAJIK,
		TAMIL,
		TATAR,
		TELUGU,
		THAI,
		TIBETAN,
		TIGRINYA,
		TONGA,
		TSONGA,
		TURKISH,
		TURKMEN,
		TWI,
		UIGHUR,
		UKRAINIAN,
		UNDEF,
		URDU,
		UZBEK,
		VIETNAMESE,
		VOLAPUK,
		WELSH,
		WOLOF,
		XHOSA,
		YIDDISH,
		YORUBA,
		ZHUANG,
		ZULU,
	};
	static XPCTSTR _strLanList[];
public:
	XPCTSTR operator [](int id){return _strLanList[id];}
	XEnumLan();
};

#define XEC XEnumCharset

class XEnumCharset
{
public:
	struct ALIAS
	{
		XPCTSTR strName;
		XU16	ID;
	};
public:
	static XINT Index(XString8&strKey);
	static XINT Index(XINT nID);
	static XPCTSTR Key(XINT nID);
	static ALIAS _aliCharset[];
	XEnumCharset(){}
};

#define XEA XEnumAlign

class XEnumAlign:public XSortConstStr
{
public:
	enum {BOTTOM,CENTER,LEFT,MIDDLE,NOREPEAT,NOWRAP,REPEAT,REPEAT_X,
		REPEAT_Y,RIGHT,TOP,WRAP};
public:
	XPCTSTR operator[](int id){return _strAlign[id];}
	static XPCTSTR _strAlign[];
	XEnumAlign();

};

class XEnumMouseAct:public XSortConstStr
{
public:
	XPCTSTR operator[](int id){return _strAct[id];}
	static XPCTSTR _strAct[];
	XEnumMouseAct();

};

#define XET XEnumType

class XEnumType:public XSortConstStr
{
public:
	enum 
	{
		ACCEPT,
		DELETE_X,
		HELP,
		OPTIONS,
		RESET,
		PREV,
		UNKNOWN,
		VNDANY,
		XANY,
	};
	static XPCTSTR _strTypes[];
public:
	XEnumType();
};

class XEnumArg:public XSortConstStr
{
public:
	enum {
		CHARSET,
		COMMENT,
		DIFFERENCES,
		DOMAIN1,
		EXPIRES,
		FILENAME,
		LEVEL,
		MAX_AGE,
		NAME,
		PADDING,
		PATH,
		Q,
		SECURE,
		START,
		STARTINFO,
		TYPE,
	};
	static XPCTSTR _strArgList[];
public:
	XEnumArg();
};


#define XEM XEnumMethod

class _XEXT_CLASS XEnumMethod
{
public:
	enum {GET,POST};//CONNECT,DELETE,GET,HEAD,POST,PUT,TRACE};
	static XPCTSTR _strMethod[];
	static XINT Index(XString8&strKey);
	static XPCTSTR Key(XINT id);
protected:
	XEnumMethod(){}
};

class _XEXT_CLASS XEnumAlias:public XSort
{
public:
	struct ALIAS
	{
		XPCTSTR strName;
		XWCHAR	ID;
	};
	//static XPCTSTR operator[](XU32 nIndex);
	XU32 GetCount();
	void* GetData(XU32 id);
	//XINT	GetIData(XU32 id);
	XWCHAR Char(XPCTSTR src);
	//XPCTSTR Name(XWCHAR ID);
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	//static XPCTSTR Key(XINT id);
	static ALIAS _aliaChars[];
	XEnumAlias(){}
};

#define XEIT XEnumInputType

#define XELT XEnumListType

class _XEXT_CLASS XEnumListType
{
public:
	enum {DISPLAY_EMPTY,DISPLAY_NONE,
		DISPLAY_INLINE,DISPLAY_BLOCK,
		DISPLAY_LIST,DISPLAY_RUNIN,
		DISPLAY_COMPACT,DISPLAY_MARKER,
		DISPLAY_TABLE,DISPLAY_INTABLE};

	enum {TYPE_A=40,TYPE_LA,TYPE_I,TYPE_LI,
		TYPE_NUM,TYPE_DISC,TYPE_SQURE,
		TYPE_CIRCLE,TYPE_NONE};
	enum {SHA_RECT,SHA_CIRCLE,SHA_POLY};
	enum {FLOAT_NONE,FLOAT_LEFT,FLOAT_RIGHT,FLOAT_BOTH};
	//enum {CLEAR_NONE,FLOAT_LEFT,FLOAT_RIGHT,FLOAT_BOTH};
	enum {TRANS_NONE,TRANS_UPCASE,TRANS_LOCASE,TRANS_CAP};
	enum {DEC_EMPTY=0,DEC_NONE,DEC_UNDERLINE,DEC_MIDLINE,DEC_UPLINE,DEC_BLINK};
	enum {FS_EMPTY,FS_NORMAL,FS_ITALIC};
	enum {FWT_EMPTY,FWT_NORMAL,FWT_BOLD};
	enum {LS_EMPTY,LS_DASHED,LS_DOTTED,LS_DOUBLE,
		LS_GROOVE,LS_HIDDEN,LS_INSET,LS_MEDIUM,LS_NONE,
		LS_OUTSET,LS_RIDGE,LS_SOLID,LS_THIN,LS_THICK};
	enum {POS_STATIC,POS_RELATIVE,POS_ABSOLUTE,
		POS_FIXED};
public:
	static XU8 GetPositionType(XPCTSTR strMode)
	{
		if(strMode==XNULL) return POS_STATIC;
		switch(strMode[0])
		{
		default:
		case 's':
		case 'S':return POS_STATIC;
		case 'R':
		case 'r':return POS_RELATIVE;
		case 'a':
		case 'A':return POS_ABSOLUTE;
		case 'f':
		case 'F':return POS_FIXED;
		}
	}
	static XU8 GetDisplayMode(XPCTSTR strMode);
	static XU8 GetBorderType(XPCTSTR strType);
	static XU8 GetBorderWidth(XPCTSTR strWidth)
	{
		int v=GetBorderType(strWidth);
		switch(v)
		{
		case LS_THIN:v=2;break;
		case LS_MEDIUM:v=3;break;
		case LS_THICK:v=5;break;
		default:
			{
				if(strWidth[0]<'0'||
					strWidth[0]>'9') return -1;
				XString8 str(strWidth,STRING_ATTACH);
				v=str.ToInt();
				str.EmptyNoFree();
				if(v<0) v=0;
				else if(v>32) v=32;
			}
		}
		return (XU8)v;
	}
	static XU8 GetFontWeight(XPCTSTR strFont)
	{
		if(strFont==XNULL) return FWT_EMPTY;
		switch(strFont[0])
		{
		case 'B':
		case 'b':return FWT_BOLD;
		case 'n':
		case 'N':
		case 'L':
		case 'l':return FWT_NORMAL;
		}
		XString8 str(strFont,STRING_ATTACH);
		int v=str.ToInt();
		str.EmptyNoFree();
		if(v>=400) return FWT_BOLD;
		return FWT_NORMAL;
	}
	static XU8 GetFontStyle(XPCTSTR strFont)
	{
		if(strFont==XNULL||strFont[0]==0) return FS_EMPTY;
		switch(strFont[0])
		{
		case 'N':
		case 'n':
			return FS_NORMAL;
		case 'o':
		case 'O':
		case 'i':
		case 'I':return FS_ITALIC;
		}
		return FS_EMPTY;
	}
	static XU8 GetFontSize(XPCTSTR strSize);
	static XU8 GetDecoration(XPCTSTR strType)
	{
		if(strType==XNULL) return DEC_NONE;
		switch(strType[0])
		{
		default:return DEC_EMPTY;
		case 'n':
		case 'N':return DEC_NONE;
		case 'U':
		case 'u':return DEC_UNDERLINE;
		case 'o':
		case 'O':return DEC_UPLINE;
		case 'l':
		case 'L':return DEC_MIDLINE;
		case 'b':
		case 'B':return DEC_BLINK;
		}
	}
	static XU8 GetTransType(XPCTSTR strType)
	{
		if(strType==XNULL) return TRANS_NONE;
		switch(strType[0])
		{
		default:
		case 'n':
		case 'N':return TRANS_NONE;
		case 'u':
		case 'U':return TRANS_UPCASE;
		case 'l':
		case 'L':return TRANS_LOCASE;
		case 'C':
		case 'c':return TRANS_CAP;
		}
	}
	static XU8 GetFloatType(XPCTSTR strType)
	{
		if(strType==XNULL) return FLOAT_NONE;
		switch(strType[0])
		{
		case 'b':
		case 'B':return FLOAT_BOTH;break;
		case 'L':
		case 'l':return FLOAT_LEFT;break;
		case 'R':
		case 'r':return FLOAT_RIGHT;break;
		default:return FLOAT_NONE;
		}
	}
	static XU8 GetShapeType(XPCTSTR strType)
	{
		if(strType==XNULL) return SHA_RECT;
		switch(strType[0])
		{
		default:
		case 'r':
		case 'R':return SHA_RECT;
		case 'c':
		case 'C':return SHA_CIRCLE;
		case 'P':
		case 'p':return SHA_POLY;
		}
	}
	static XU8 GetType(XPCTSTR strType)
	{
		if(strType==XNULL) return TYPE_DISC;
		switch(strType[0])
		{
		default:
		case 'N':
		case 'n':return TYPE_NONE;
		case 'A':return TYPE_A;
		case 'a':return TYPE_LA;
		case 'I':return TYPE_I;
		case 'i':return TYPE_LI;
		case 'D':
		case 'd':return TYPE_DISC;
		case 's':
		case 'S':return TYPE_SQURE;
		case 'C':
		case 'c':return TYPE_CIRCLE;
		}
		return TYPE_DISC;
	}
protected:

};

class _XEXT_CLASS XEnumInputType:public XSortConstStr
{
public:
	enum {//

		BUTTON,CHECKBOX,FILE,HIDDEN,IMAGE,PASSWORD,RADIO,RESET,SUBMIT,TEXT,
		TEXT_BAIWAN,TEXT_CSS,TEXT_JAVASCRIPT,URL};
		XEnumInputType();
		static XPCTSTR _strTextType[];
};

#define XHTTP_STATUS_CONTINUE            100 // OK to continue with request
#define XHTTP_STATUS_SWITCH_PROTOCOLS    101 // server has switched protocols in upgrade header
#define XHTTP_STATUS_OK                  200 // request completed
#define XHTTP_STATUS_CREATED             201 // object created, reason = new URI
#define XHTTP_STATUS_ACCEPTED            202 // async completion (TBS)
#define XHTTP_STATUS_PARTIAL             203 // partial completion
#define XHTTP_STATUS_NO_CONTENT          204 // no info to return
#define XHTTP_STATUS_RESET_CONTENT       205 // request completed, but clear form
#define XHTTP_STATUS_PARTIAL_CONTENT     206 // partial GET furfilled
#define XHTTP_STATUS_AMBIGUOUS           300 // server couldn't decide what to return
#define XHTTP_STATUS_MOVED               301 // object permanently moved
#define XHTTP_STATUS_REDIRECT            302 // object temporarily moved
#define XHTTP_STATUS_REDIRECT_METHOD     303 // redirection w/ new access method
#define XHTTP_STATUS_NOT_MODIFIED        304 // if-modified-since was not modified
#define XHTTP_STATUS_USE_PROXY           305 // redirection to proxy, location header specifies proxy to use
#define XHTTP_STATUS_306				 306
#define XHTTP_STATUS_REDIRECT_KEEP_VERB  307 // HTTP/1.1: keep same verb
#define XHTTP_STATUS_BAD_REQUEST         400 // invalid syntax
#define XHTTP_STATUS_DENIED              401 // access denied
#define XHTTP_STATUS_PAYMENT_REQ         402 // payment required
#define XHTTP_STATUS_FORBIDDEN           403 // request forbidden
#define XHTTP_STATUS_NOT_FOUND           404 // object not found
#define XHTTP_STATUS_BAD_METHOD          405 // method is not allowed
#define XHTTP_STATUS_NONE_ACCEPTABLE     406 // no response acceptable to client found
#define XHTTP_STATUS_PROXY_AUTH_REQ      407 // proxy authentication required
#define XHTTP_STATUS_REQUEST_TIMEOUT     408 // server timed out waiting for request
#define XHTTP_STATUS_CONFLICT            409 // user should resubmit with more info
#define XHTTP_STATUS_GONE                410 // the resource is no longer available
#define XHTTP_STATUS_LENGTH_REQUIRED     411 // the server refused to accept request w/o a length
#define XHTTP_STATUS_PRECOND_FAILED      412 // precondition given in request failed
#define XHTTP_STATUS_REQUEST_TOO_LARGE   413 // request entity was too large
#define XHTTP_STATUS_URI_TOO_LONG        414 // request URI too long
#define XHTTP_STATUS_UNSUPPORTED_MEDIA   415 // unsupported media type
#define XHTTP_STATUS_OUT_RANGE			 416
#define XHTTP_STATUS_EXPECTATION		 417
#define XHTTP_STATUS_SERVER_ERROR        500 // internal server error
#define XHTTP_STATUS_NOT_SUPPORTED       501 // required not supported
#define XHTTP_STATUS_BAD_GATEWAY         502 // error response received from gateway
#define XHTTP_STATUS_SERVICE_UNAVAIL     503 // temporarily overloaded
#define XHTTP_STATUS_GATEWAY_TIMEOUT     504 // timed out waiting for gateway
#define XHTTP_STATUS_VERSION_NOT_SUP     505 // HTTP version not supported

class XEnumHttpStatus
{
public:

#pragma pack(1)
	struct _HTTPSTATUS
	{
		XU16 nCode;
		XPCTSTR strError;
		XU8	nWapCode;
	};
#pragma pack()

public:
	static XPCTSTR GetErrorString(XU16 id);
	static XU16 Match(XU8 nWapCode);
	static _HTTPSTATUS _httpStatus[];
};

enum 
{
	XERR_OK,XERR_CANT_CONNECT=1,XERR_NO_RESPONSE=2,XERR_USER_CANCELED=3,
	XERR_DOMAN=4,XERR_SEND_ERROR=5,XERR_REV_ERROR=6,XERR_BAD_PROTO=7,XERR_UNKNOW=8,
	XERR_NOT_EXIST=9
};

#define XECG XEnumConfig

class XEnumConfig//:public XSortStr
{
public:
	XPCTSTR operator[](XINT id){return Key(id);}
	enum {BOOKMARK,CONFIG,HOT,NEARBY,TODAY,UPDATE};
	static XPCTSTR _strConfig[];
	static XINT Index(XString8&strKey);
	static XPCTSTR Key(XINT id);
};

class XEnumColor:public XSort
{
public:
	struct COLORALIA
	{
		XPCTSTR strColorName;
		XU32	nColor;
	};
	XU32 GetCount();
	void* GetData(XU32 id);
public:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	XU32 ColorOf(XPCTSTR strColor,XU8 bCss=XFALSE)
	{
		XString8 str(strColor,STRING_ATTACH);
		XU32 c=Color(str,bCss);
		str.EmptyNoFree();
		return c;
	}
	XU32 Color(XString8&strColor,XU8 bCss=XFALSE );
	static COLORALIA m_colors[];
};

#define XEF XEnumFrame
class XEnumFrame:public XSortConstStr
{
public:
	enum
	{
		ABOVE,//"above",
		ALL,//"all",
		BELOW,//"bellow",
		BORDER,//"border",
		BOX,//"box",
		COLS,//"cols",
		GROUPS,//"groups",
		HSIDES,//"hsides",
		LHS,//"lhs",
		RHS,//"none",
		ROWS,//"rhs",
		VOID_X,//"rows",
		VSIDES//"void",
	};
	//"vsides",
public:
	XEnumFrame();
	static XPCTSTR _strFrames[];
};


#endif // __XENUMDATA_H__
