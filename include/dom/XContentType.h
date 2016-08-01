// XContentType.h: interface for the XContentType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCONTENTTYPE_H__315B0A37_45A5_4673_B85D_EB9488052DFE__INCLUDED_)
#define AFX_XCONTENTTYPE_H__315B0A37_45A5_4673_B85D_EB9488052DFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define XCNT XContentType


class XContentType  
{
public:
	enum {CANY,CTEXT,CMULTIPART,CAPPLICATION,CIMAGE,CAUDIO,CVIDEO,CDATA};
enum {
		TANY,		  
		T3GP,
		TALTERNATIVE,		
		TAU,
		TBYTERANGES,
		TFORM_DATA,
		TGIF,
		THTML,
		TJAVA_VM,
		TJAVASCRIPT,
		TJPEG,
		TMIXED,
		TMPEG,
		TMPEG2,
		TMSWORD,
		TOCTET_STREAM,
		TPAYSDKPID,
		TPDF,
		TPLAIN,
		TPNG,
		TQUICKTIME,
		TRAR,
		TRTF,
		TSWF,
		TTIFF,
		TVDO,
		TVIVO,
		TVND_VIVO,
		TVND_WAP_CHANNEL,
		TVND_WAP_CHANNELC,
		TVND_WAP_CO,
		TVND_WAP_COC,
		TVND_WAP_CONNECTIVITY_WBXML,
		TVND_WAP_CONNECTIVITY_XML,
		TVND_WAP_MULTIPART_ALTERNATIVE,
		TVND_WAP_MULTIPART_ANY,
		TVND_WAP_MULTIPART_BYTERANGES,
		TVND_WAP_MULTIPART_FORM_DATA,
		TVND_WAP_MULTIPART_MIXED,
		TVND_WAP_MULTIPART_RELATED,
		TVND_WAP_SI,
		TVND_WAP_SIA,
		TVND_WAP_SIC,
		TVND_WAP_SL,
		TVND_WAP_SLC,
		TVND_WAP_UAPROF,
		TVND_WAP_WBMP,
		TVND_WAP_WBXML,
		TVND_WAP_WML,
		TVND_WAP_WMLC,
		TVND_WAP_WMLSCRIPT,
		TVND_WAP_WMLSCRIPTC,
		TVND_WAP_WTLS_CA_CERTIFICATE,
		TVND_WAP_WTLS_USER_CERTIFICATE,
		TVND_WAP_XHTML_XML,
		TWAV,
		TX_AIFF,
		TX_GTAR,
		TX_HDML,
		TX_HDMLC,
		TX_JAVASCRIPT,
		TX_MPEG,
		TX_MPEG2,
		TX_MPEG3,
		TX_MPEG4,
		TX_MSVIDEO,
		TX_SGI_MOVIE,
		TX_SHOCK_WAVE_FLASH,
		TX_TAR,
		TX_TTML,
		TX_VCALENDAR,
		TX_VCARD,
		//TX_WAV,
		TX_WWW_FORM_URLENCODED,
		TX_X509_CA_CERT,
		TX_X509_USER_CERT,
		TX_X968_CA_CERT,
		TX_X968_CROSS_CERT,
		TX_X968_USER_CERT,
		TXCONFIG,
		TXFILTER,
		TXHTML_XML,//xhtml+xml
		TXML,
		TZIP,
	};
	XContentType();
	virtual ~XContentType();
public:
	//XINT GetDomType();
	XContentType(XU8 nClass,XU16 nType);
	void SetType(XPCTSTR strType);
	void SetType(XU8 nClass,XU16 nType,XU8 nCharset=XCHARSET_GB2312)
	{Reset(nClass,nType,nCharset);}
	void Reset(XU8 nClass=TANY,XU16 nType=TANY,XU8 nCharset=XCHARSET_GB2312);
	XPCTSTR GetTypeString(){return m_strType;}
	
#ifdef _SYMBIAN	  // modify by hgl
#ifdef _DEBUG
	#pragma pack(1)
#endif
	XU8 m_nClass,m_nCharset;
	XU16 m_nType;
#ifdef _DEBUG
	#pragma pack()
#endif
#else
	#pragma pack(1)
	XU8 m_nClass,m_nCharset;
	XU16 m_nType;
	#pragma pack()
#endif

	XString8 m_strType;
	static XU8 _wmlEncode[][3];
	static XPCTSTR _strClassList[];
	static XPCTSTR _strTypeList[];
};

#endif // !defined(AFX_XCONTENTTYPE_H__315B0A37_45A5_4673_B85D_EB9488052DFE__INCLUDED_)
