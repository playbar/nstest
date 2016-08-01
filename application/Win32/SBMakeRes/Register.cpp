// CRegister.cpp: implementation of the CRegister class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Register.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*DeviceData CDevice::deviceData = {	1, 0, 
									0x5a,
									27,
									0,
									0,
									0x8000,
									// The above Initialize is ignore !!!1
									0, //FRAME
									0, //INTERNAL
									1, //FRAME SYNC HEAD
									1, //com
									0, //TV
									0, //NORMAL
									124,
									66,
									66,
									176,
									150,
									150,
									0,
									0,
									FALSE };
int CDevice::deviceWidth = 500;
int CDevice::deviceHeight = 400;
BOOL   CDevice::deviceOK = TRUE;
COLORREF* CDevice::videoMem = NULL;
COLORREF* CDevice::overlayMem = NULL;*/

CRegister::CRegister( LPCSTR keyName )
{
   m_keyName = keyName;
}

CRegister::~CRegister()
{

}

int CRegister::Read(LPCSTR name, int nDefault)
{
   return AfxGetApp( ) -> GetProfileInt( m_keyName, name, nDefault );
}


void CRegister::Write(LPCSTR name, int nVal)
{
  AfxGetApp( ) -> WriteProfileInt( m_keyName, name, nVal );
}

CString CRegister::Read(LPCSTR name, LPCSTR sDefault)
{  return AfxGetApp( ) -> GetProfileString( m_keyName, name, sDefault ); }

void CRegister::Write(LPCSTR name, LPCSTR str)
{
  AfxGetApp( ) -> WriteProfileString( m_keyName, name, str ); 
}



void CRegister::WriteInt(LPCSTR m_sec, LPCSTR name, int nVal)
{  AfxGetApp( ) -> WriteProfileInt( m_sec, name, nVal ); }




void CRegister::Write(LPCSTR name, LPVOID lpStruct, UINT nSize)
{	
	AfxGetApp()->WriteProfileBinary(m_keyName,name,(LPBYTE)lpStruct,nSize);
}

void CRegister::Read(LPCSTR name, LPVOID lpStruct, UINT nSize)
{
	void* p=NULL;
	UINT n1=nSize;
    if(!AfxGetApp()->GetProfileBinary(m_keyName,name,(LPBYTE*)&p,&n1))
		return;
	if(n1>0&&p!=NULL) memcpy(lpStruct,p,nSize);
	if(p!=NULL) delete p;
}

//DEL void CRegister::Write(VGLineBuffer &buf)
//DEL {
//DEL 	Write(_dataLenKey,buf.GetCount());
//DEL 	Write(_dataKey,buf.GetBuffer(),buf.GetCount());
//DEL }

//DEL BOOL CRegister::Read(VGLineBuffer &buf)
//DEL {
//DEL    int nLen=Read(_dataLenKey,0);
//DEL    if(nLen==0) return FALSE;
//DEL    buf.SetLength(nLen);
//DEL    Read(_dataKey,buf,nLen);
//DEL    return TRUE;
//DEL }

void CRegister::SetName(LPCSTR name)
{
	m_keyName=name;
}
