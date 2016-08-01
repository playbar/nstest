// XContentType.cpp: implementation of the XContentType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XContentType.h"
#include "XEnumData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XPCTSTR XContentType::_strClassList[]=
	{"*",
	 "text",
	 "Multipart",
	 "application",
	 "image",
	 "audio",
	 "video"
	};
XPCTSTR XContentType::_strTypeList[]=
	{"*",
	"3gp",
	"alternative",//1.10x0F
	"au",
	"byteranges",//1.10x0E
	"form-data",//1.10x0D
	"gif",//1.10x1D
	"html",
	"java-vm",//1.10x11
	"javascript",
	"jpeg",//1.10x1E
	"mixed",//1.10x0C
	"mpeg",
	"mpeg-2",
	"msword",
	"octet-stream",
	"paysdk-pid",
	"pdf",
	"plain",
	"png",//1.10x20
	"quicktime",
	"rar",
	"rtf",
	"swf",
	"tiff",//1.10x1F
	"vdo",
	"vivo",
#ifdef _SYMBIAN
	"vnd.symbian.install",
#endif
	"vnd.vivo",
	"vnd.wap.channel",//1.10x0A
	"vnd.wap.channelc",//1.10x16
	"vnd.wap.co",//1.20x31
	"vnd.wap.coc",//1.20x32
	"vnd.wap.connectivity-wbxml",//1.30x36
	"vnd.wap.connectivity-xml",//1.30x35
	"vnd.wap.multipart.*",//1.10x22
	"vnd.wap.multipart.alternative",//1.10x26
	"vnd.wap.multipart.byteranges",//1.10x25
	"vnd.wap.multipart.form-data",//1.10x24
	"vnd.wap.multipart.mixed",//1.10x23
	"vnd.wap.multipart.related",//1.20x33
	"vnd.wap.si",//1.10x2D
	"vnd.wap.sia",//1.20x34
	"vnd.wap.sic",//1.20x2E
	"vnd.wap.sl",//1.20x2F
	"vnd.wap.slc",//1.20x30
	"vnd.wap.uaprof",//1.10x17
	"vnd.wap.wbmp",//1.10x21
	"vnd.wap.wbxml",//1.10x29
	"vnd.wap.wml",
	"vnd.wap.wmlc",//1.10x08
	"vnd.wap.wmlscript",//1.10x09
	"vnd.wap.wmlscriptc",//1.10x15
	"vnd.wap.wtls-ca-certificate",//1.10x18
	"vnd.wap.wtls-user-certificate",//1.10x19	
	"vnd.wap.xhtml+xml",
	"wav",
	"x-aiff",
	"x-gtar",
	"x-hdml",//1.10x04
	"x-hdmlc",//1.10x13
	"x-javascript",
	"x-mpeg",
	"x-mpeg-2",
	"x-mpeg-3",
	"x-mpeg-4",
	"x-msvideo",
	"x-sgi-movie",
	"x-shockwave-flash",
	"x-tar",
	"x-ttml",//1.10x05
	"x-vCalendar",//1.10x06
	"x-vCard",//1.10x07
	"x-www-form-urlencoded",//1.10x12
	"x-x509-ca-cert",//1.10x1A
	"x-x509-user-cert",//1.10x1B
	"x-x968-ca-cert",//1.10x2B
	"x-x968-cross-cert",//1.10x2A
	"x-x968-user-cert",//1.10x2C
	"xconfig",
	"xfilter",
	"xhtml+xml",
	"xml",//1.10x27
//#ifdef __ANDROID__ // zhengjl 2012.03
//	"yyh-pid",
//#endif
	"zip",
};

XU8 XContentType::_wmlEncode[][3]=
{
	{CANY,TANY,0x0},
	{CTEXT,TANY,0x1},
	{CTEXT,THTML,0x2},
	{CTEXT,TPLAIN,0x3},
	{CTEXT,TX_HDML,0x4},
	{CTEXT,TX_TTML,0x5},
	{CTEXT,TX_VCALENDAR,0x6},
	{CTEXT,TX_VCARD,0x7},
	{CTEXT,TVND_WAP_WMLC,0x8},
	{CTEXT,TVND_WAP_WMLSCRIPT,0x9},
	{CTEXT,TVND_WAP_CHANNEL,0xA},
	{CMULTIPART,TANY,0xB},
	{CMULTIPART,TMIXED,0xC},
	{CMULTIPART,TFORM_DATA,0xD},
	{CMULTIPART,TBYTERANGES,0xE},
	{CMULTIPART,TALTERNATIVE,0xF},
	{CAPPLICATION,TANY,0x10},
	{CAPPLICATION,TJAVA_VM,0x11},
	{CAPPLICATION,TX_WWW_FORM_URLENCODED,0x12},
	{CAPPLICATION,TX_HDMLC,0x13},
	{CAPPLICATION,TVND_WAP_WMLC,0x14},
	{CAPPLICATION,TVND_WAP_WMLSCRIPTC,0x15},
	{CAPPLICATION,TVND_WAP_CHANNELC,0x16},
	{CAPPLICATION,TVND_WAP_UAPROF,0x17},
	{CAPPLICATION,TVND_WAP_WTLS_CA_CERTIFICATE,0x18},
	{CAPPLICATION,TVND_WAP_WTLS_USER_CERTIFICATE,0x19},
	{CAPPLICATION,TX_X509_CA_CERT,0x1a},
	{CAPPLICATION,TX_X509_USER_CERT,0x1b},
	{CIMAGE,TANY,0x1c},
	{CIMAGE,TGIF,0x1D},
	{CIMAGE,TJPEG,0x1E},
	{CIMAGE,TTIFF,0x1F},
	{CIMAGE,TPNG,0x20},
	{CIMAGE,TVND_WAP_WBMP,0x21},
	{CAPPLICATION,TVND_WAP_MULTIPART_ANY,0x22},
	{CAPPLICATION,TVND_WAP_MULTIPART_MIXED,0x23},
	{CAPPLICATION,TVND_WAP_MULTIPART_FORM_DATA,0x24},
	{CAPPLICATION,TVND_WAP_MULTIPART_BYTERANGES,0x25},
	{CAPPLICATION,TVND_WAP_MULTIPART_ALTERNATIVE,0x26},
	{CAPPLICATION,TXML,0x27},
	{CTEXT,TXML,0x28},
	{CAPPLICATION,TVND_WAP_WBXML,0x29},
	{CAPPLICATION,TX_X968_CROSS_CERT,0x2A},
	{CAPPLICATION,TX_X968_CA_CERT,0x2B},
	{CAPPLICATION,TX_X968_USER_CERT,0x2C},
	{CTEXT,TVND_WAP_SI,0x2D},
	{CAPPLICATION,TVND_WAP_SIC,0x2E},
	{CTEXT,TVND_WAP_SL,0x2F},
	{CAPPLICATION,TVND_WAP_SLC,0x30},
	{CTEXT,TVND_WAP_CO,0x31},
	{CAPPLICATION,TVND_WAP_COC,0x32},
	{CAPPLICATION,TVND_WAP_MULTIPART_RELATED,0x33},
	{CAPPLICATION,TVND_WAP_SIA,0x34},
	{CTEXT,TVND_WAP_CONNECTIVITY_XML,0x35},
	{CAPPLICATION,TVND_WAP_CONNECTIVITY_WBXML,0x36},
	{CTEXT,TVND_WAP_WML,0x37}
};


XContentType::XContentType()
{
   Reset(0,0,XCHARSET_UTF8);
}

XContentType::~XContentType()
{

}

void XContentType::Reset(XU8 nClass,XU16 nType,XU8 nCharset)
{
	m_nClass=nClass;
	m_nType=nType;
	m_nCharset=nCharset;
	m_strType=_strClassList[m_nClass];
	m_strType+=XDIV;
	m_strType+=_strTypeList[m_nType];
}

void XContentType::SetType(XPCTSTR str)
{
	m_strType=str;
	XString8Array list;
	list.SplitString(m_strType,XSPLIT);
	if(list.GetSize()>1)
	{
		m_strType=list[0];
		for(XU32 i=1;i<list.GetSize();i++)
		{
		  XString8Array vs;
		  vs.SplitString(list[i],XEQU);
		  if(vs.GetSize()>1)
		  {
			  XEnumAttribs ea;
			  if(vs[0].Compare(ea[XEAB::CHARSET],XTRUE)==0)
				  m_nCharset=XEnumCharset().Index(vs[1]);
		  }
		}
	}
	int id=m_strType.Find(XDIV);
	m_nClass=CANY;
	m_nType=TANY;
	XString8 strType,strClass;
	if(id>0)
	{
		m_strType.Left(strClass,id);
		strClass.TrimLeft();
		strClass.TrimRight();
		m_strType.Mid(strType,id+1);
		//int id1=m_strType.Find('+');
		//if(id1>0)
		//	m_strType.Delete(id1,m_strType.GetLength()-id1);
		strType.TrimLeft();
		strType.TrimRight();
	}
	else strType=m_strType;

	int i=strClass.IndexOf(_strClassList,sizeof(_strClassList)/sizeof(XPCTSTR),XTRUE);
	if(i>=0) m_nClass=i;
	XSortConstStr sort(_strTypeList,sizeof(_strTypeList)/sizeof(XPCTSTR));
	i=sort.Index(strType);
	if(i>=0) m_nType=i;
	
}

XContentType::XContentType(XU8 nClass, XU16 nType)
{
	Reset(nClass,nType);
}

