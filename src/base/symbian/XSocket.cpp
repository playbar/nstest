// XSocket.cpp: implementation of the XSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XSocket.h"
#include "XMutex.h"
#include "XString.h"
#include "ZCConvert.h"
#include "ZConnection.h"
#include <wchar.h>
#include "symbiandebug.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XSocket::XSocket() //: CActive(EPriorityHigh)
	{
	// TODO Auto-generated constructor stub
//	m_hReader=NULL;
//	m_hWriter=NULL;
//	m_hWait=NULL;
//	m_hConnectWait=NULL;
	m_nState=0;
	m_nRequest=0;
	m_bConnected=XFALSE;
	m_bIsStart = false;
	m_nRecvLen = 0;
//	CActiveScheduler::Add(this);
	}

XSocket::~XSocket()
	{
	// TODO Auto-generated destructor stub
	Close();
	}

int XSocket::CheckSocket()
	{
//	if(m_hReader)
	if(m_bConnected)
		{
		if(WaitForReply(0)>=XWAIT_OK)
			{
			unsigned long data;
			if(Receive((char*)&data,sizeof(unsigned long))==0)
				{
				Close();
				return false;
				}
			return true;
			}
		}
	return false;
	}

int XSocket::Receive(void*pData,int nSize)
	{
//	Cancel();
//	return m_hReader->Read((char*)pData,nSize);
	TPtr8 ptr((unsigned char*)pData,nSize);
//	ptr.Set((unsigned char*)pData,0,nSize);
	int templen = m_nRecvLen();
	m_nRecvLen = 0;
	if(templen >= 0)
		{
		ptr.Append(m_iBuf);
		ptr.Set((unsigned char *)pData+templen,0,nSize-templen);
		}
	m_iSocket.RecvOneOrMore(ptr,0,m_Status,m_nRecvLen);
	User::WaitForRequest(m_Status);
	if(m_Status == KErrNone)
		{
	int len = m_nRecvLen()+templen;
	m_nRecvLen = 0;
	m_bIsStart = false;

	FILE *fl;
	fl=fopen("C:/Data/respond.txt","wb");
	if(fl){ fwrite(pData,1,len,fl);
			fclose(fl);}

	return len;
		}
	else
		{
		return -1;
		}
	}

int XSocket::Send(void*pData,int nSize)
	{

	{FILE *fl;
	fl=fopen("C:/Data/request.txt","wb");
	if(fl){ fwrite(pData,1,nSize,fl);
			fclose(fl);}}

//	m_hWriter->Cancel();
//	return m_hWriter->Write((char*)pData,nSize);
	TSockXfrLength len;
	len=-1;
	TPtr8 ptr((unsigned char*)pData,nSize,nSize);
	m_iSocket.CancelSend();
	m_Status=100;
	m_iSocket.Send(ptr,0,m_Status,len);
	m_Timer.Cancel();
	m_Timer.After(m_timeStatus,2000000);
	User::WaitForAnyRequest();
	if(m_Status==KErrNone)
		m_Timer.Cancel();
	else
		m_iSocket.CancelSend();
	return len();
	}

int XSocket::WaitForReply(unsigned char nTime)
	{
	//if(!(m_nState&))
#ifdef _NetLog
	{FILE *fl;
	fl=fopen(DefaultLog,"a");
	if(fl){ fprintf(fl,"int XSocket::WaitForReply(%d)--------is start\r\n",nTime);
			fclose(fl);}}
#endif
	if(!m_bConnected)
	{
		if(!(m_nState&KSOCKSERVISCONNECT))
			return NOTCONNECTED;
		if(!(m_nState&KSOCKETISCONNECT))
		{
			m_timeStatus=100;
			m_Timer.Cancel();
			m_Timer.After(m_timeStatus,nTime?nTime*1000000:1000000);
			User::WaitForAnyRequest();
			#ifdef _NetLog
				static int tempa=1;
				{FILE *fl;
				fl=fopen(DefaultLog,"a");
				if(fl){ fprintf(fl,"%d\r\n",tempa);
						fclose(fl);}}
				tempa++;
			#endif
			if(m_Status.Int()==0)
			{
				m_Timer.Cancel();
				m_nState |= KSOCKETISCONNECT;
				m_bConnected=XTRUE;
				return CONNECTOK;
			}	
			return CONNECTTIMEOUT;
		}
		else
			{
				m_bConnected=XTRUE;
				return CONNECTOK;
			}
	}
	if(m_nState ^ (KSOCKSERVISCONNECT | KSOCKETISCONNECT |KREADERISCONNECT |KWRITERISCONNECT))
		return NOTCONNECTED;
//	int t=nTime/100;
//	if(t<=0) t=1;

//	if(!m_bIsStart)
//		_Start();
//	while(1)
//		{
//		if(m_nRecvLen() != 0) return REVOK;
//		t--;
//		if(t<=0) break;
//		User::After(100000);
//		}
	if(_Start(nTime*1000)) return REVOK;
	return REVTIMEOUT;
	}

int XSocket::Close()
	{
//	Cancel();
//	if(m_hReader)
//		delete m_hReader;
//	if(m_hWriter)
//		delete m_hWriter;
//	if(m_hConnectWait)
//		delete m_hConnectWait;
//	if(m_hWait)
//		delete m_hWait;
//	m_hReader=NULL;
//	m_hWriter=NULL;
//	m_hConnectWait=NULL;
//	m_hWait=NULL;
	m_Timer.Close();
	m_iSocket.Close();
//	m_iSocketServ.Close();
	m_nState=0;
	m_bConnected = false;
	m_bIsStart = false;
	m_nRecvLen = 0;
	
	return true;
	}

int XSocket::ConnectTCP(const char* strHost,unsigned short uPort)
	{
#ifdef _NetLog
	{FILE *fl;
	fl=fopen("C:/Data/connect.txt","wb");
	if(fl){ fprintf(fl,"%s      %d",strHost,uPort);
			fclose(fl);}}
#endif
	if(m_bConnected) return XC_OK;
	if(!(m_nState ^ (KSOCKSERVISCONNECT | KSOCKETISCONNECT |KREADERISCONNECT |KWRITERISCONNECT)))
		return XC_OK;
#ifdef _NetLog
SYMBIANDEB2(DefaultLog,"it is start to connect %s:%d\r\n",strHost,uPort);
#endif
		m_nState |= KREADERISCONNECT;

		if(!(m_nState & KWRITERISCONNECT))
			{
				m_Timer.CreateLocal();
				m_nState |= KWRITERISCONNECT;
			}

	

	//连接socket服务会话
	if(m_nState & KSOCKSERVISCONNECT);
	else
		{
			//增加RConnection，主动连接物理网络
			/* RConnection m_Connection;
			 * TCommDbCommPref pref;
			 * pref.SetIapId(4);    //设置接入点信息在数据库中的Id，从配置文件中取得上次的接入点信息
			 * pref.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);		//设置不弹出选择窗口
			 * pref.SetDirection(ECommDbConnectionDirectionOutgoing);       //设置接入点的连接方向
			 * pref.SetBearerSet(ECommDbBearerGPRS);						//设置物理连接，用于承载接入点
			 * m_Connection.Open(m_iSocketServ);							//打开连接管理服务
			 * m_Connection.Start(pref,iStatus);							//异步
			 * */
			if(m_iSocket.Open(CZConnection::GetInstance()->m_server,KAfInet,KSockStream,KProtocolInetTcp)!=KErrNone)
				{
					{FILE *fl;
					fl=fopen(DefaultLog,"a");
					if(fl){ fprintf(fl,"it have error when open the socket\r\n");
							fclose(fl);}}
				return XC_ERROR;
				}
			m_nState|=KSOCKSERVISCONNECT;
		}
#ifdef _NetLog
SYMBIANDEB(DefaultLog,"socket is opened\r\n");
#endif
	//解析主机名，得到IP地址
//	XString8 str;
//	str.ConvertFrom(strHost,XCHARSET_GB2312);
//	XString16 str16(str);
//	TText16         *tempName=str16.GetData();
//测试用
//	char* netname = NULL;
//	CZConnection::GetInstance()->GetConnectionName(netname);
//	SYMBIANDEB1(DefaultLog,"connection name is %s\r\n",netname);
//	delete netname;
//测试结束
	CZConnection* connection = CZConnection::GetInstance();
	if(!connection->IsProxy())
		{
		TText16         *tempName=ZCConvert::ConvertToUnicode((unsigned char*)strHost);
		#ifdef _NetLog
		int l = wcslen((wchar_t*)tempName);
		SYMBIANDEB1(DefaultLog,"whostname's length is %d\r\n",l);
		#endif
		TPtr            iPtr(tempName,wcslen((wchar_t*)tempName),256);
		if(m_destAddr.Input(iPtr)!=KErrNone)
			{
			RHostResolver   iHostResolver;
			TNameEntry      iResult;
	//		RConnection		iConnection;
	//		iConnection.Open(m_iSocketServ);
			if(KErrNone==iHostResolver.Open(connection->m_server,KAfInet,KProtocolInetUdp,connection->m_globalConnection))
				{
				#ifdef _NetLog
				SYMBIANDEB(DefaultLog,"iHostResolver is opened\r\n");
				#endif
	//			m_Status = 100;
	//			iHostResolver.GetByName(iPtr,iResult,m_Status);
	//			User::WaitForRequest(m_Status);
				if(KErrNone == iHostResolver.GetByName(iPtr,iResult))
					{
					iHostResolver.Close();
					m_destAddr=iResult().iAddr;
					}
				else
					{
					delete []tempName;
					iHostResolver.Close();
						{FILE *fl;
						fl=fopen(DefaultLog,"a");
						if(fl){ fprintf(fl,"host name is not find\r\n");
								fclose(fl);}}
					return XC_ERROR;
					}
				}
			else
				{
					{FILE *fl;
					fl=fopen(DefaultLog,"a");
					if(fl){ fprintf(fl,"RHostResolver dns is not open\r\n");
							fclose(fl);}}
				delete []tempName;
				return XC_ERROR;
				}
			}
		delete []tempName;
		m_destAddr.SetPort(uPort);
		}
	else
		{
		m_destAddr.SetAddress(connection->m_address.Address());
		m_destAddr.SetPort(connection->m_address.Port());
		}
#ifdef _NetLog
SYMBIANDEB(DefaultLog,"address is ready\r\n");
#endif
	
	//连接socket
	if(!(m_nState & KSOCKETISCONNECT))
		{
		m_nRequest=EConnect;
		m_Status=100;
//		m_timeStatus=100;
//		m_Timer.Cancel();
		m_iSocket.Connect(m_destAddr,m_Status);
//		m_Timer.After(m_timeStatus,2000);
//		User::WaitForAnyRequest();
		#ifdef _NetLog
			SYMBIANDEB(DefaultLog,"ConnectTCP is over, the value is XC_WAIT for return\r\n");
		#endif
		return XC_WAIT;
		}
	else
		{
		#ifdef _NetLog
			SYMBIANDEB(DefaultLog,"ConnectTCP is over, the value is XC_OK for return\r\n");
		#endif
		return XC_OK;
		}
	}

void XSocket::RunL()
	{
//	int err=iStatus.Int();
//	//KErrNone;
//	switch(m_nRequest)
//		{
//		case EConnect:
//			{
//			if(KErrNone==err)
//				{
//				m_nState |= KSOCKETISCONNECT;
////				m_hReader->StartL();
//				}
//			m_nRequest=0;
//			}
//			break;
//		case EWait:
//			{
//			m_nRequest=0;
//			}
//			break;
//		}
	}
void XSocket::DoCancel()
	{
//	m_hWriter->Cancel();
//	m_iSocket.CancelRecv();
//	if(IsActive())
//		{
//		if(m_nRequest==EConnect)
//			m_iSocket.CancelConnect();
//		}
	}

void XSocket::OnSend(const unsigned char *msg,int err)           //come from MWriterObserver
	{
	
	}

void XSocket::OnReceive(const unsigned char *msg,int err)           //come from MReaderObserver
	{
	
	}

void XSocket::OnTimer(const unsigned char *msg,int err)          //come from MTimerObserver
	{
//	if(m_hWait->IsStarted())
//		m_hWait->AsyncStop();
//	m_iSocket.CancelConnect();
	}

bool XSocket::_Start(int time)
	{
	if(!m_bIsStart)
		{
		m_bIsStart = true;
		m_iSocket.CancelRecv();
		m_Status=100;
		m_timeStatus=100;
		m_iSocket.RecvOneOrMore(m_iBuf,0,m_Status,m_nRecvLen);
		m_Timer.Cancel();
		m_Timer.After(m_timeStatus,time?time*1000000:1000000);
		User::WaitForAnyRequest();
		if(m_Status==KErrNone)
			{
			m_Timer.Cancel();
			return true;			//可以收数据
			}
		else
			{
			m_bIsStart=false;
			m_iSocket.CancelRecv();
			return false;			//没有收到数据
			}
		}
	return true;
	}
