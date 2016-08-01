//
// XCom.cpp: 
//
#include <assert.h>
#include <stdio.h>
#include "XString.h"
#include "XWindow.h"
#include "XCom.h"

#define XCOM_DEF_TIMEOUT 100 // in milliseconds

XBOOL     XCom::m_bSSLLibReady    = XFALSE;
SSLCTXPTR XCom::m_pCTX            = XNULL;
XINT      XCom::m_nTimeout		  = XCOM_DEF_TIMEOUT;


extern "C" void SSLLogCallback(const char * msg)
{
#if defined(_WIN32)
    
	XString16 str16;
	str16.ConvertFrom(msg, XCHARSET_GBK);
	XWindow::Trace(str16);

#else // for unix/linux-based system
    
    fprintf(stdout, "%s", msg);

#endif
}

//
// Used to init the ssl apis before main().
//
class XCOMINIT
{
public:
	XCOMINIT()
    {
		if (XCom::SSLLibStartup())
        {
            XCom::SSLCTXSetup(NULL, // Generally a user cert is not required to create an SSL connection.
                              NULL, //
                              NULL, //
							  NULL, // TODO!!!
                              NULL, // TODO!!!
                              0);
        }
    }

	~XCOMINIT()
    {
	}
    
} xcominitobj;

XBOOL XCom::SSLLibStartup()
{
#if defined(ENABLE_SSL_LOG)
	SSLAPI_Init(SSLLogCallback);
#else
    SSLAPI_Init(NULL);
#endif
    
#if defined(ENABLE_SSL_THREAD)
	if (SSLAPI_ThreadSetup() < 0)
	{
		SSLCTXCleanup();
		return XFALSE;
	}
#endif

	return m_bSSLLibReady = XTRUE;
}

// Note:
//	Call this method before any SSL object is opened.
//
// RETURN:
//	XTRUE
//	XFALSE
//
XBOOL XCom::SSLCTXSetup(XPCTSTR szUserX509File,		// May be NULL
						XPCTSTR szUserKeyFile,		// can't be NULL if szUserX509File is not NULL
						XPCTSTR szUserKeyPass,		// may be NULL if the user key file is not password-protected
						XPCTSTR szCaCertFile,		// Must specify a verify location for real security!!!
						XPCTSTR szCaCertPath,		// Must specify a verify location for real security!!!
						XINT nTimeout)				// in milliseconds, or -1 to wait infinitely, or 0 to use the default value
{
	if (!m_bSSLLibReady)
		return XFALSE;

	if (m_pCTX)
		return XTRUE;

#define USE_TEST_CA_CERTS

#if defined(USE_TEST_CA_CERTS)
	if (!szCaCertFile && !szCaCertPath)
	{
		SSLAPI_CreateTestCaCertFile();
		szCaCertFile = SSLAPI_GetTestCaCertFileName();
	}
#endif

	m_pCTX = SSLAPI_SSLCTXNew(SSLAPI_GetMethod(SSL_PROTO_TLSV1),
                              "DEFAULT",
                              SSL_VERIFY_PEER,
                              SSLAPI_VerifyCallback,
                              SSLAPI_InfoCallback,
                              szUserX509File,
                              szUserKeyFile,
                              szUserKeyPass,
                              szCaCertFile,
                              szCaCertPath);

	if (m_pCTX)
	{
        if (nTimeout == 0)
            m_nTimeout = XCOM_DEF_TIMEOUT;
        else if (nTimeout < 0)
            m_nTimeout = -1;
        else // > 0
            m_nTimeout = nTimeout;
        
		return XTRUE;
	}
	else
	{
		return XFALSE;
	}
}

// Note:
//	Call this method after all SSL objects created from this context are closed.
//
void XCom::SSLCTXCleanup()
{
	if (m_pCTX)
	{
		SSLAPI_SSLCTXFree(m_pCTX);
		m_pCTX = XNULL;
	}

#if defined(USE_TEST_CA_CERTS)
	SSLAPI_DeleteTestCaCertFile();
#endif
}


XBOOL XCom::SSLIsReady()
{
	return m_pCTX ? XTRUE : XFALSE;
}


XCom::XCom(XBOOL bUseSSL):m_bSocketConnected(XFALSE),m_pSSL(XNULL),m_bUseSSL(bUseSSL),m_prefetchNum(0),m_xsocket()
{
	if (m_bUseSSL)
	{
#if defined(ENABLE_SSL_LOG)
		SSLLOG(("XCom(this=%x)\n", this));
#endif
	}
}

XCom::~XCom()
{
	if (m_bUseSSL)
	{
#if defined(ENABLE_SSL_LOG)
		SSLLOG(("~XCom(this=%x)\n", this));
#endif
	    Close();
	}
	else
	{
	}
}

XBOOL XCom::IsValid()
{
	if (m_bUseSSL)
	{
		// SSLLOG(("XCom::IsValid(this=%x)\n", this));

		return (SSLIsReady() && m_pSSL) ? XTRUE : XFALSE;
	}
	else
	{
		return m_xsocket.IsValid();
	}
}

XBOOL XCom::IsConnect()
{
	if (m_bUseSSL)
	{
		// SSLLOG(("XCom::IsConnect(this=%x)\n", this));

		return (IsValid() && m_bSocketConnected) ? XTRUE : XFALSE;
	}
	else
	{
		return m_xsocket.IsConnect();
	}
}

XBOOL XCom::Close()
{
	if (m_bUseSSL)
	{
#if defined(ENABLE_SSL_LOG)
		SSLLOG(("XCom::Close(this=%x)\n", this));
#endif

		if (IsValid())
		{
			if (IsConnect())
				if (SSLAPI_SSLShutdownNB(m_pSSL, m_nTimeout) < 0)
					SSLAPI_SSLShutdownQuietly(m_pSSL);

#if defined(ENABLE_SSL_SESSION_CACHE)
			// Save session for later reuse
			SSLSESSPTR pSESS = SSLAPI_Get1SessionFromSSL(m_pSSL);
			if (pSESS)
				if (SSLAPI_SetSessionToCache(m_host, pSESS) < 0)
					SSLAPI_SSLSessionFree(pSESS);
#endif
			SSLAPI_SSLFree(m_pSSL);

			m_host[0] = 0;
			m_pSSL = XNULL;
			m_bSocketConnected = XFALSE;

			return XTRUE;
		}
		else
		{
			return XFALSE;
		}
	}
	else
	{
		return m_xsocket.Close();
	}
}

XINT XCom::Send(void *pData, int nSize)
{
	if (m_bUseSSL)
	{
#if defined(ENABLE_SSL_LOG)
		SSLLOG(("XCom::Send(this=%x)\n", this));
#endif

		if (IsConnect())
		{
			if (nSize <= 0)
				return -1;

			int saErr = SSLAPI_SSLWriteNB(m_pSSL, (char *)pData, nSize, m_nTimeout);
			if (saErr >= 0)
			{
				return saErr;
			}
			else if (saErr == SAERR_TIMEOUT)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return m_xsocket.Send(pData, nSize);
	}
}

int XCom::Receive(void *pData, int nSize)
{
	if (m_bUseSSL)
	{
#if defined(ENABLE_SSL_LOG)
		SSLLOG(("XCom::Receive(this=%x) expected %d bytes\n", this, nSize));
#endif

		if (IsConnect())
		{
			if (nSize <= 0)
				return -1;

			char * p = (char *)pData;
			int lenRet = 0;

			if (m_prefetchNum > 0)
			{
				lenRet = (m_prefetchNum < nSize) ? m_prefetchNum : nSize;
				memcpy(p, m_prefetchBuffer, lenRet);
				memcpy(m_prefetchBuffer, &m_prefetchBuffer[lenRet], m_prefetchNum-lenRet);
				m_prefetchNum -= lenRet;
				nSize -= lenRet;
				p += lenRet;
			}

			if (nSize == 0)
				return lenRet;

			// try to read more data
			int saErr = SSLAPI_SSLReadNB(m_pSSL, p, nSize, m_nTimeout);
			if (saErr > 0)
			{
				// some bytes read
				lenRet += saErr;
				return lenRet;
			}
			else if (lenRet > 0) // saErr <= 0
			{
				return lenRet;
			}
			else if (saErr == SAERR_TIMEOUT) // lenRet == 0
			{
				return 0;
			}
			else // other SAERR_...
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return m_xsocket.Receive(pData, nSize);
	}
}

XBOOL XCom::CheckSocket()
{
	if (m_bUseSSL)
	{
#if defined(ENABLE_SSL_LOG)
		SSLLOG(("XCom::CheckSocket(this=%x)\n", this));
#endif

		if (IsConnect())
		{
			if (WaitForReply(0) >= XWAIT_OK)
			{
				XU32 data;
				if (Receive((char*)&data, sizeof(XU32)) == 0)
				{
					Close();
					return XFALSE;
				}
			}
		}

		return XTRUE;
	}
	else
	{
		return m_xsocket.CheckSocket();
	}
}

int XCom::ConnectTCP(XPCTSTR strHost, XU16 uPort)
{
	if (m_bUseSSL)
	{
#if defined(ENABLE_SSL_LOG)
		SSLLOG(("XCom::ConnectTCP(this=%x)\n", this));
#endif

		if (SSLIsReady())
		{
			if (IsValid())
				Close();

			if (strlen(strHost) >= SSL_HOST_SIZE)
				return XC_ERROR;

			m_pSSL = SSLAPI_SSLNew(m_pCTX);
			if (!m_pSSL)
			{
				return XC_ERROR;
			}

			SSLAPI_SetConnectState(m_pSSL);

			if (SSLAPI_PrepareConnectionToHost(m_pSSL, strHost, uPort) < 0)
			{
				SSLAPI_SSLFree(m_pSSL);
				m_pSSL = XNULL;
				return XC_ERROR;
			}
			
			strcpy(m_host, strHost);

			SSLAPI_SetUserData(m_pSSL, m_host);

#if defined(ENABLE_SSL_SESSION_CACHE)
			// Try to reuse session
			SSLSESSPTR pSESS = SSLAPI_GetSessionFromCache(m_host);
			if (pSESS)
				SSLAPI_SetSessionToSSL(m_pSSL, pSESS);
#endif

			if (SSLAPI_CheckConnectionNB(m_pSSL, m_nTimeout) == 0)
			{
				m_bSocketConnected = XTRUE;
				return XC_OK;
			}

			return XC_WAIT;
		}
		else
		{
			return XC_ERROR;
		}
	}
	else
	{
		return m_xsocket.ConnectTCP(strHost, uPort);
	}
}


XINT XCom::WaitForReply(XU8 nTime)
{
	if (m_bUseSSL)
	{
#if defined(ENABLE_SSL_LOG)
		SSLLOG(("XCom::WaitForReply(this=%x)\n", this));
#endif
		if (IsValid())
		{
			if (!IsConnect()) switch (SSLAPI_CheckConnectionNB(m_pSSL, nTime))
			{
				case 0:
					m_bSocketConnected = XTRUE;
					return CONNECTOK;
				case SAERR_TIMEOUT:
					return CONNECTTIMEOUT;
				default:
					return CONNECTERROR;
			}
			else
			{
				if (m_prefetchNum > 0)
				{
					return REVOK;
				}
				else switch (SSLAPI_SSLReadNB(m_pSSL, m_prefetchBuffer, 1, nTime)) // try to prefetch a byte
				{
				case 1:
					m_prefetchNum = 1;
					return REVOK;

				case SAERR_TIMEOUT:
					return REVTIMEOUT;

				default:
					return REVERROR;
				}
			}
		}
		else
		{
			return NOTCONNECTED;
		}
	}
	else
	{
		return m_xsocket.WaitForReply(nTime);
	}
}
