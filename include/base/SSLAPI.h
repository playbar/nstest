// sslapi.h
#ifndef _SSLAPI_H
#define _SSLAPI_H

#include <time.h>

#define ENABLE_SSL_THREAD
#define ENABLE_SSL_SESSION_CACHE

#define SSL_HOST_SIZE	64

#if defined(_DEBUG)
#define ENABLE_SSL_LOG
#else
#define ENABLE_SSL_LOG
#endif

#if defined(ENABLE_SSL_LOG)
#define SSLLOG(x) SSLAPI_Log x
#else
#define SSLLOG(x) do {} while (0)
#endif

//
// 如果使用pSSL.h，则会造成某些CPP文件，比如XDrawDib.cpp，在VS2008中无法编译。所以，我们使用如下折中方案。
//
#define USE_SSL_H 0

#if USE_SSL_H

#include <openssl/ssl.h>

#define SSLPTR SSL *
#define SSLCTXPTR SSL_CTX *
#define X509STORECTXPTR X509_STORE_CTX *
#define BIOPTR BIO *
#define SSLMETHODPTR SSL_METHOD *
#define SSLSESSPTR SSL_SESSION *

#else

#define SSLPTR void *
#define SSLCTXPTR void *
#define X509STORECTXPTR void *
#define BIOPTR void *
#define SSLMETHODPTR void *
#define SSLSESSPTR void *

// Define them according to ssl.h

#define SSL_VERIFY_NONE                 0x00
#define SSL_VERIFY_PEER                 0x01
#define SSL_VERIFY_FAIL_IF_NO_PEER_CERT	0x02
#define SSL_VERIFY_CLIENT_ONCE          0x04

#endif

#define SAERR_OK             0
#define SAERR_WOULDBLOCK	-1
#define SAERR_TIMEOUT		-2
#define SAERR_NOTFOUND		-3
#define SAERR_EOF	        -4
#define SAERR_BADDATA		-5
#define SAERR_LOWBUF		-6
#define SAERR_NOTIMPL		-7
#define SAERR_ERROR         -100

// Used in SSLAPI_GetMethod
#define SSL_PROTO_SSLV3     1
#define SSL_PROTO_TLSV1     2

#define WAIT_FOR_READING    0
#define WAIT_FOR_WRITING    1

#define SSLAPI_WaitForReading(fd, tmout) SSLAPI_WaitForIOReady(fd, WAIT_FOR_READING, tmout)
#define SSLAPI_WaitForWriting(fd, tmout) SSLAPI_WaitForIOReady(fd, WAIT_FOR_WRITING, tmout)

#ifdef __cplusplus
extern "C" {
#endif

typedef int  (*SSL_VERIFY_CALLBACK) (int, X509STORECTXPTR);
typedef void (*SSL_INFO_CALLBACK)   (const SSLPTR, int, int);
typedef void (*SSL_LOG_CALLBACK)    (const char * msg);
typedef void (*SSL_MSG_CALLBACK)    (int write_p, int version, int content_type, const void *buf, size_t len, SSLPTR *pSSL, void *arg);
typedef void (*SSL_TLSEXT_CALLBACK) (SSLPTR pSSL, int client_server, int type, unsigned char *data, int len, void *arg);

#if defined(ENABLE_SSL_THREAD)

void SSLAPI_LockCallback(int mode, int type, const char * file, int line);
int  SSLAPI_ThreadSetup();
void SSLAPI_ThreadCleanup(void);

#endif

int SSLAPI_Log(const char * szFmt, ...);
const char * SSLAPI_SSLGetErrorName(int nError);
unsigned long SSLAPI_GetCurThrNum();
void SSLAPI_msleep(int ms);
void SSLAPI_Init(SSL_LOG_CALLBACK logFunc);
SSLMETHODPTR SSLAPI_GetMethod(int nProto);
int  SSLAPI_PassCallback(char *buf, int num, int rwflag, void * p);
long SSLAPI_BioCallback(BIOPTR pBIO, int cmd, const char *argp, int argi, long argl, long ret);
void SSLAPI_TlsextCallback(SSLPTR pSSL, int client_server, int type, unsigned char *data, int len, void *arg);
void SSLAPI_MessageCallback(int write_p, int version, int content_type, const void *buf, size_t len, SSLPTR pSSL, void *arg);
int  SSLAPI_CertVerifyCallback(X509STORECTXPTR pSTR, void * arg);
void SSLAPI_InfoCallback(const SSLPTR pSSL, int where, int ret);
int  SSLAPI_VerifyCallback(int preverify_ok, X509STORECTXPTR pSTR);
int  SSLAPI_WaitForIOReady(int fd, int forWrite, int tmout);
double SSLAPI_GetProcTime();
int SSLAPI_AdjustTimeout(double baseTime, int tmout);
SSLCTXPTR SSLAPI_SSLCTXNew(
    const SSLMETHODPTR pMETHOD,
    const char * szCiphers,
    int verifyMode,
    SSL_VERIFY_CALLBACK verifyCallback,
    SSL_INFO_CALLBACK infoCallback,
    const char * szUserX509File,
    const char * szUserKeyFile,
    const char * szUserKeyPass,
    const char * szCaCertFile,
    const char * szCaCertPath
);
void SSLAPI_SSLCTXFree(SSLCTXPTR pCTX);
void SSLAPI_SetConnectState(SSLPTR pSSL);
void SSLAPI_SetUserData(SSLPTR pSSL, void * pData);
void * SSLAPI_GetUserData(SSLPTR pSSL);
SSLPTR SSLAPI_SSLNew(SSLCTXPTR pCTX);
void SSLAPI_SSLFree(SSLPTR pSSL);
void SSLAPI_SSLClear(SSLPTR pSSL);
int SSLAPI_SSLConnectNB(SSLPTR pSSL, int tmout);
int SSLAPI_SSLShutdownNB(SSLPTR pSSL, int tmout);
void SSLAPI_SSLShutdownQuietly(SSLPTR pSSL);
int SSLAPI_SSLSessionReused(SSLPTR pSSL);
int SSLAPI_CheckConnectionNB(SSLPTR pSSL, int tmout);
int SSLAPI_PrepareConnectionToHost(SSLPTR pSSL, const char * szHost, unsigned short nPort);
int SSLAPI_CheckCert(SSLPTR pSSL, const char * szHostName);
int SSLAPI_SSLReadNB(SSLPTR pSSL, char * buffer, int len, int tmout);
int SSLAPI_SSLWriteNB(SSLPTR pSSL, char * buffer, int len, int tmout);
int SSLAPI_SSLReadLine(SSLPTR pSSL, char * buffer, int sizeBuffer, int tmout);
int SSLAPI_SSLReadHttpMsgNB(SSLPTR pSSL, char * buffer, int sizeBuffer, int tmout);
int SSLAPI_TcpConnectNB(const char * szHost, unsigned short nPort, int tmout);
int SSLAPI_ExtractHttpHeader(const char * szHttpMsg, int lenHttpMsg, const char * szName, char * buffer, int sizeBuffer);
int SSLAPI_ExtractHttpContentLength(const char * szHttpMsg, int lenHttpMsg);
int SSLAPI_ExtractHttpConnection(const char * szHttpMsg, int lenHttpMsg);
int SSLAPI_GetHttpRequest(char * buffer, int sizeBuffer);
char * SSLAPI_TrimLeft(char * sz);
char * SSLAPI_TrimRight(char * sz);
char * SSLAPI_Trim(char * sz);
int SSLAPI_HasTrailingLF(const char * sz);
int SSLAPI_IsBlankString(const char * sz);
int SSLAPI_IsEmptyLine(const char * sz);
int SSLAPI_CreateTestCaCertFile();
void SSLAPI_DeleteTestCaCertFile();
const char * SSLAPI_GetTestCaCertFileName();

#if defined(ENABLE_SSL_SESSION_CACHE)

void SSLAPI_SessionCacheSetup();
void SSLAPI_SessionCacheCleanup();

SSLSESSPTR SSLAPI_GetSessionFromCache(const char * host);
int SSLAPI_SetSessionToCache(const char * host, SSLSESSPTR pSESS);

SSLSESSPTR SSLAPI_Get1SessionFromSSL(SSLPTR pSSL);
int SSLAPI_SetSessionToSSL(SSLPTR pSSL, SSLSESSPTR pSESS);
void SSLAPI_SSLSessionFree(SSLSESSPTR pSESS);

#endif

#ifdef __cplusplus
}
#endif
#endif // _SSLAPI_H


