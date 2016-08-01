// sslapi.cpp

#ifdef _WIN32

#	include <winsock2.h>
#	include <Ws2tcpip.h>

#   define CLOSESOCKET closesocket

#else // for all unix/linux-based system

#	include <fcntl.h>
#   include <unistd.h>
#   include <netdb.h>
#   include <sys/socket.h>
#	include <netinet/in.h>

#   define CLOSESOCKET close
#   define SOCKET int
#	define INVALID_SOCKET -1

#   ifdef _POSIX_THREADS
#       include <pthread.h>
#   endif
#endif

#define SOCKET_IS_VALID(s) ((int)(s) >= 0)

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>

#include "SSLAPI.h"

#if defined(_WIN32)
#else // for all unix/linux-based system

#   define stricmp strcasecmp
#   define strnicmp strncasecmp

#endif

typedef int  (*REAL_SSL_VERIFY_CALLBACK) (int, X509_STORE_CTX *);
typedef void (*REAL_SSL_INFO_CALLBACK)   (const SSL *, int, int);
typedef void (*REAL_SSL_MSG_CALLBACK)    (int write_p, int version, int content_type, const void *buf, size_t len, SSL * ssl, void *arg);
typedef void (*REAL_SSL_TLSEXT_CALLBACK) (SSL * ssl, int client_server, int type, unsigned char *data, int len, void *arg);


// Variables available only in this module

static char user_pass[256] = {0}; // NOTES: Shared by all ssl contexts.

static SSL_LOG_CALLBACK log_func = NULL;

// Temp verify location
#if defined(ENABLE_BUILTIN_CA_CERTS)
static char ca_certs_filename[256] = {0};
#endif

// Store user data in ssl object
static int ssl_user_data_index = -1;

/////////////////////////////////////////////////////////////////////////////
//
// Thread support
//

#if defined(ENABLE_SSL_THREAD)

#if defined(_WIN32)
    static HANDLE *lock_cs;
#elif defined(_POSIX_THREADS)
    static pthread_mutex_t *lock_cs;
#else
#	error TODO
#endif

#if defined(ENABLE_SSL_SESSION_CACHE)
	static int sess_cache_lock_type = -1;
#endif

//
// Read Openssl CRYPTO_lock manpage for more information.
//
#if defined(_WIN32)

void SSLAPI_LockCallback(int mode, int type, const char * file, int line)
{
	if (mode & CRYPTO_LOCK)
		WaitForSingleObject(lock_cs[type], INFINITE);
	else
		ReleaseMutex(lock_cs[type]);
}


// RETURN:
//	0	Success
//	<0	SAERR_...
//
int SSLAPI_ThreadSetup()
{
	int numLocks = CRYPTO_num_locks();

#if defined(ENABLE_SSL_SESSION_CACHE)
	// The last lock is used for our session cache control
	numLocks += 1;
#endif

	lock_cs = (HANDLE *)OPENSSL_malloc(numLocks * sizeof(HANDLE));
	if (!lock_cs)
        return SAERR_ERROR;

	for (int i=0; i<numLocks; ++i)
	{
		if (!(lock_cs[i] = CreateMutex(NULL, FALSE, NULL)))
		{
			for (--i; i>=0; --i)
				CloseHandle(lock_cs[i]);

            return SAERR_ERROR;
		}
	}

	CRYPTO_set_locking_callback(SSLAPI_LockCallback);

#if defined(ENABLE_SSL_SESSION_CACHE)
	sess_cache_lock_type = numLocks - 1;
#endif
    
	return SAERR_OK;
}


void SSLAPI_ThreadCleanup(void)
{
	int numLocks = CRYPTO_num_locks();

#if defined(ENABLE_SSL_SESSION_CACHE)
	numLocks += 1;
#endif

	CRYPTO_set_locking_callback(NULL);

	for (int i=0; i<numLocks; ++i)
		CloseHandle(lock_cs[i]);

#if defined(ENABLE_SSL_SESSION_CACHE)
	sess_cache_lock_type = -1;
#endif

	OPENSSL_free(lock_cs);
}

#elif defined(_POSIX_THREADS)

void SSLAPI_LockCallback(int mode, int type, const char * file, int line)
{
	if (mode & CRYPTO_LOCK)
		pthread_mutex_lock(&(lock_cs[type]));
	else
		pthread_mutex_unlock(&(lock_cs[type]));
}


// RETURN:
//	0	Success
//	<0	SAERR_...
//
int SSLAPI_ThreadSetup()
{
	int numLocks = CRYPTO_num_locks();

#if defined(ENABLE_SSL_SESSION_CACHE)
	numLocks += 1;
#endif
	
	lock_cs=(pthread_mutex_t *)OPENSSL_malloc(numLocks * sizeof(pthread_mutex_t));
	if (!lock_cs)
        return SAERR_ERROR;
	
	for (int i=0; i<numLocks; ++i)
	{
		if (0 != pthread_mutex_init(&(lock_cs[i]), NULL))
		{
			for (--i; i>=0; --i)
				pthread_mutex_destroy(&(lock_cs[i]));

			return SAERR_ERROR;
		}
	}
	
	CRYPTO_set_locking_callback(SSLAPI_LockCallback);

#if defined(ENABLE_SSL_SESSION_CACHE)
	sess_cache_lock_type = numLocks - 1;
#endif
    
    return SAERR_OK;
}


void SSLAPI_ThreadCleanup(void)
{
	int numLocks = CRYPTO_num_locks();
	
#if defined(ENABLE_SSL_SESSION_CACHE)
	numLocks += 1;
#endif

	CRYPTO_set_locking_callback(NULL);
	
	for (int i=0; i<numLocks; ++i)
		pthread_mutex_destroy(&(lock_cs[i]));

#if defined(ENABLE_SSL_SESSION_CACHE)
	sess_cache_lock_type = -1;
#endif

	OPENSSL_free(lock_cs);
}

#else
#error TODO
#endif // _WIN32

#endif // ENABLE_SSL_THREAD


/////////////////////////////////////////////////////////////////////////////
//
// SSL Helper and Wrapper APIs
//

int SSLAPI_Log(const char * szFmt, ...)
{
	if (!log_func)
		return 0;

	va_list marker;
	va_start(marker, szFmt);
	char buffer[512];
	int n = sprintf(buffer, "[thread %lx] ", SSLAPI_GetCurThrNum());
	int m = vsprintf(&buffer[n], szFmt, marker);
	log_func(buffer);

	return m+n;
}


const char * SSLAPI_SSLGetErrorName(int nError)
{
    switch (nError)
    {
        case SSL_ERROR_NONE:
            return "SSL_ERROR_NONE";
        case SSL_ERROR_SSL:
            return "SSL_ERROR_SSL";
        case SSL_ERROR_WANT_READ:
            return "SSL_ERROR_WANT_READ";
        case SSL_ERROR_WANT_WRITE:
            return "SSL_ERROR_WANT_WRITE";
        case SSL_ERROR_WANT_X509_LOOKUP:
            return "SSL_ERROR_WANT_X509_LOOKUP";
        case SSL_ERROR_SYSCALL:
            return "SSL_ERROR_SYSCALL";
        case SSL_ERROR_ZERO_RETURN:
            return "SSL_ERROR_ZERO_RETURN";
        case SSL_ERROR_WANT_CONNECT:
            return "SSL_ERROR_WANT_CONNECT";
        case SSL_ERROR_WANT_ACCEPT:
            return "SSL_ERROR_WANT_ACCEPT";
        default:
            return "SSL_ERROR_UNKNOWN";
    }
}


// NOTES:
//	Returns the unique number (not thread id) of the current thread.
//
unsigned long SSLAPI_GetCurThrNum()
{
	CRYPTO_THREADID id;
	CRYPTO_THREADID_current(&id);
	return id.val;
}


void SSLAPI_msleep(int ms)
{
#ifdef _WIN32
	Sleep(ms);
#else // unix
	usleep(ms*1000);
#endif
}


void SSLAPI_Init(SSL_LOG_CALLBACK logFunc)
{
	char idx_name[100];
	strcpy(idx_name, "ssl-user-data");

	log_func = logFunc;

	CRYPTO_malloc_init();
	SSL_load_error_strings();
	SSL_library_init();
	ssl_user_data_index = SSL_get_ex_new_index(0, idx_name, NULL, NULL, NULL);

#ifdef ENABLE_BUILTIN_CA_CERTS
#endif

#ifdef _WIN32
	RAND_screen();
#endif
}


SSLMETHODPTR SSLAPI_GetMethod(int nProto)
{
	switch (nProto)
	{
        case SSL_PROTO_TLSV1:
            return (SSLMETHODPTR)TLSv1_method();
            
        case SSL_PROTO_SSLV3:
            return (SSLMETHODPTR)SSLv3_method();
            
        default:
            return NULL;
	}
}




// RETURN:
//  >=0	Password length
//
int SSLAPI_PassCallback(char *buf, int num, int rwflag, void * p)
{
	char * pUserPass = (char *)p;
    
	if (strlen(pUserPass) < num)
	{
		strcpy(buf, pUserPass);
		return strlen(pUserPass);
	}
	else
	{
		buf[0] = 0;
		return 0;
	}
}

#if defined(_DEBUG)

// NOTES:
//	Copied from bio_cb.c.
//
long SSLAPI_BioCallback(BIOPTR pBIO, int cmd, const char *argp, int argi, long argl, long ret)
{
	BIO * bio = (BIO *)pBIO;
	long r=1;

	SSLLOG(("SSLAPI_BioCallback: entry\n"));

	if (BIO_CB_RETURN & cmd)
		r=ret;

	SSLLOG(("SSLAPI_BioCallback: BIO[%08lX]: ",(unsigned long)bio));

	switch (cmd)
	{
	case BIO_CB_FREE:
		SSLLOG(("Free - %s\n",bio->method->name));
		break;
	case BIO_CB_READ:
		if (bio->method->type & BIO_TYPE_DESCRIPTOR)
			SSLLOG(("read(%d,%lu) - %s fd=%d\n",
				 bio->num,(unsigned long)argi,
				 bio->method->name,bio->num));
		else
			SSLLOG(("read(%d,%lu) - %s\n",
				 bio->num,(unsigned long)argi,
				 bio->method->name));
		break;
	case BIO_CB_WRITE:
		if (bio->method->type & BIO_TYPE_DESCRIPTOR)
			SSLLOG(("write(%d,%lu) - %s fd=%d\n",
				 bio->num,(unsigned long)argi,
				 bio->method->name,bio->num));
		else
			SSLLOG(("write(%d,%lu) - %s\n",
				 bio->num,(unsigned long)argi,
				 bio->method->name));
		break;
	case BIO_CB_PUTS:
		SSLLOG(("puts() - %s\n",bio->method->name));
		break;
	case BIO_CB_GETS:
		SSLLOG(("gets(%lu) - %s\n",(unsigned long)argi,bio->method->name));
		break;
	case BIO_CB_CTRL:
		SSLLOG(("ctrl(%lu) - %s\n",(unsigned long)argi,bio->method->name));
		break;
	case BIO_CB_RETURN|BIO_CB_READ:
		SSLLOG(("read return %ld\n",ret));
		break;
	case BIO_CB_RETURN|BIO_CB_WRITE:
		SSLLOG(("write return %ld\n",ret));
		break;
	case BIO_CB_RETURN|BIO_CB_GETS:
		SSLLOG(("gets return %ld\n",ret));
		break;
	case BIO_CB_RETURN|BIO_CB_PUTS:
		SSLLOG(("puts return %ld\n",ret));
		break;
	case BIO_CB_RETURN|BIO_CB_CTRL:
		SSLLOG(("ctrl return %ld\n",ret));
		break;
	default:
		SSLLOG(("bio callback - unknown type (%d)\n",cmd));
		break;
	}

	SSLLOG(("SSLAPI_BioCallback: return %d\n", r));
	return(r);
}
#endif


// NOTES:
//  See SSL_CTX_set_cert_verify_callback manpage for more information.
//
int SSLAPI_CertVerifyCallback(X509STORECTXPTR pSTR, void * arg)
{
	X509_STORE_CTX * store = (X509_STORE_CTX *)pSTR;

	SSLLOG(("SSLAPI_CertVerifyCallback: store=%p, arg=%p\n", store, arg));
	
	// See ssl_cert.c(526)
	int r = X509_verify_cert(store);

	return r;
}

void SSLAPI_MessageCallback(int write_p, int version, int content_type, const void *buf, size_t len, SSL *ssl, void *arg)
{
#if defined(_DEBUG)

	const char *str_write_p, *str_version, *str_content_type = "", *str_details1 = "", *str_details2= "";
	
	str_write_p = write_p ? ">>>" : "<<<";

	switch (version)
		{
	case SSL2_VERSION:
		str_version = "SSL 2.0";
		break;
	case SSL3_VERSION:
		str_version = "SSL 3.0 ";
		break;
	case TLS1_VERSION:
		str_version = "TLS 1.0 ";
		break;
	case DTLS1_VERSION:
		str_version = "DTLS 1.0 ";
		break;
	case DTLS1_BAD_VER:
		str_version = "DTLS 1.0 (bad) ";
		break;
	default:
		str_version = "???";
		}

	if (version == SSL2_VERSION)
		{
		str_details1 = "???";

		if (len > 0)
			{
			switch (((const unsigned char*)buf)[0])
				{
				case 0:
					str_details1 = ", ERROR:";
					str_details2 = " ???";
					if (len >= 3)
						{
						unsigned err = (((const unsigned char*)buf)[1]<<8) + ((const unsigned char*)buf)[2];
						
						switch (err)
							{
						case 0x0001:
							str_details2 = " NO-CIPHER-ERROR";
							break;
						case 0x0002:
							str_details2 = " NO-CERTIFICATE-ERROR";
							break;
						case 0x0004:
							str_details2 = " BAD-CERTIFICATE-ERROR";
							break;
						case 0x0006:
							str_details2 = " UNSUPPORTED-CERTIFICATE-TYPE-ERROR";
							break;
							}
						}

					break;
				case 1:
					str_details1 = ", CLIENT-HELLO";
					break;
				case 2:
					str_details1 = ", CLIENT-MASTER-KEY";
					break;
				case 3:
					str_details1 = ", CLIENT-FINISHED";
					break;
				case 4:
					str_details1 = ", SERVER-HELLO";
					break;
				case 5:
					str_details1 = ", SERVER-VERIFY";
					break;
				case 6:
					str_details1 = ", SERVER-FINISHED";
					break;
				case 7:
					str_details1 = ", REQUEST-CERTIFICATE";
					break;
				case 8:
					str_details1 = ", CLIENT-CERTIFICATE";
					break;
				}
			}
		}

	if (version == SSL3_VERSION ||
	    version == TLS1_VERSION ||
	    version == DTLS1_VERSION ||
	    version == DTLS1_BAD_VER)
		{
		switch (content_type)
			{
		case 20:
			str_content_type = "ChangeCipherSpec";
			break;
		case 21:
			str_content_type = "Alert";
			break;
		case 22:
			str_content_type = "Handshake";
			break;
			}

		if (content_type == 21) /* Alert */
			{
			str_details1 = ", ???";
			
			if (len == 2)
				{
				switch (((const unsigned char*)buf)[0])
					{
				case 1:
					str_details1 = ", warning";
					break;
				case 2:
					str_details1 = ", fatal";
					break;
					}

				str_details2 = " ???";
				switch (((const unsigned char*)buf)[1])
					{
				case 0:
					str_details2 = " close_notify";
					break;
				case 10:
					str_details2 = " unexpected_message";
					break;
				case 20:
					str_details2 = " bad_record_mac";
					break;
				case 21:
					str_details2 = " decryption_failed";
					break;
				case 22:
					str_details2 = " record_overflow";
					break;
				case 30:
					str_details2 = " decompression_failure";
					break;
				case 40:
					str_details2 = " handshake_failure";
					break;
				case 42:
					str_details2 = " bad_certificate";
					break;
				case 43:
					str_details2 = " unsupported_certificate";
					break;
				case 44:
					str_details2 = " certificate_revoked";
					break;
				case 45:
					str_details2 = " certificate_expired";
					break;
				case 46:
					str_details2 = " certificate_unknown";
					break;
				case 47:
					str_details2 = " illegal_parameter";
					break;
				case 48:
					str_details2 = " unknown_ca";
					break;
				case 49:
					str_details2 = " access_denied";
					break;
				case 50:
					str_details2 = " decode_error";
					break;
				case 51:
					str_details2 = " decrypt_error";
					break;
				case 60:
					str_details2 = " export_restriction";
					break;
				case 70:
					str_details2 = " protocol_version";
					break;
				case 71:
					str_details2 = " insufficient_security";
					break;
				case 80:
					str_details2 = " internal_error";
					break;
				case 90:
					str_details2 = " user_canceled";
					break;
				case 100:
					str_details2 = " no_renegotiation";
					break;
				case 110:
					str_details2 = " unsupported_extension";
					break;
				case 111:
					str_details2 = " certificate_unobtainable";
					break;
				case 112:
					str_details2 = " unrecognized_name";
					break;
				case 113:
					str_details2 = " bad_certificate_status_response";
					break;
				case 114:
					str_details2 = " bad_certificate_hash_value";
					break;
					}
				}
			}
		
		if (content_type == 22) /* Handshake */
			{
			str_details1 = "???";

			if (len > 0)
				{
				switch (((const unsigned char*)buf)[0])
					{
				case 0:
					str_details1 = ", HelloRequest";
					break;
				case 1:
					str_details1 = ", ClientHello";
					break;
				case 2:
					str_details1 = ", ServerHello";
					break;
				case 3:
					str_details1 = ", HelloVerifyRequest";
					break;
				case 11:
					str_details1 = ", Certificate";
					break;
				case 12:
					str_details1 = ", ServerKeyExchange";
					break;
				case 13:
					str_details1 = ", CertificateRequest";
					break;
				case 14:
					str_details1 = ", ServerHelloDone";
					break;
				case 15:
					str_details1 = ", CertificateVerify";
					break;
				case 16:
					str_details1 = ", ClientKeyExchange";
					break;
				case 20:
					str_details1 = ", Finished";
					break;
					}
				}
			}
		}

	SSLLOG(("%s %s%s [length %04lx]%s%s\n", str_write_p, str_version, str_content_type, (unsigned long)len, str_details1, str_details2));

#if 0
	if (len > 0)
		{
		size_t num, i;
		
		SSLLOG(("   "));

		num = len;
#if 0
		if (num > 16)
			num = 16;
#endif
		for (i = 0; i < num; i++)
			{
			if (i % 16 == 0 && i > 0)
				SSLLOG(("\n   "));
			SSLLOG((" %02x", ((const unsigned char*)buf)[i]));
			}
		if (i < len)
			SSLLOG((" ..."));
		SSLLOG(("\n"));
		}
#endif

#endif // _DEBUG
}

void SSLAPI_TlsextCallback(SSL *s, int client_server, int type, unsigned char *data, int len, void *arg)
{
#if defined(_DEBUG)
	const char *extname;

	switch(type)
		{
		case TLSEXT_TYPE_server_name:
		extname = "server name";
		break;

		case TLSEXT_TYPE_max_fragment_length:
		extname = "max fragment length";
		break;

		case TLSEXT_TYPE_client_certificate_url:
		extname = "client certificate URL";
		break;

		case TLSEXT_TYPE_trusted_ca_keys:
		extname = "trusted CA keys";
		break;

		case TLSEXT_TYPE_truncated_hmac:
		extname = "truncated HMAC";
		break;

		case TLSEXT_TYPE_status_request:
		extname = "status request";
		break;

		case TLSEXT_TYPE_elliptic_curves:
		extname = "elliptic curves";
		break;

		case TLSEXT_TYPE_ec_point_formats:
		extname = "EC point formats";
		break;

		case TLSEXT_TYPE_session_ticket:
		extname = "server ticket";
		break;

		case TLSEXT_TYPE_renegotiate:
		extname = "renegotiate";
		break;

#ifdef TLSEXT_TYPE_opaque_prf_input
		case TLSEXT_TYPE_opaque_prf_input:
		extname = "opaque PRF input";
		break;
#endif

		default:
		extname = "unknown";
		break;

		}
	
	SSLLOG(("TLS %s extension \"%s\" (id=%d), len=%d\n", client_server ? "server": "client", extname, type, len));
	// BIO_dump(bio, (char *)data, len);
	// (void)BIO_flush(bio);

#endif // _DEBUG
}

// NOTES:
//	See SSL_CTX_set_info_callback manpage for more information.
//
void SSLAPI_InfoCallback(const SSLPTR pSSL, int where, int ret)
{
#if defined(_DEBUG)

#define INFO_LEVEL 2 // 0, 1, or 2

#if INFO_LEVEL>0

	SSL * ssl = (SSL *)pSSL;
	const char *str;
	int w;

	w = where & ~SSL_ST_MASK;

	if (w & SSL_ST_CONNECT)
		str = "SSL_connect";
	else if (w & SSL_ST_ACCEPT)
		str = "SSL_accept";
	else
		str = "undefined";

#if INFO_LEVEL>1
	if (where & SSL_CB_LOOP)
	{
		SSLLOG(("SSLAPI_InfoCallback: %ssl:%ssl\n", str, SSL_state_string_long(ssl)));
	}
	else if (where & SSL_CB_ALERT)
	{
		str = (where & SSL_CB_READ) ? "read" : "write";
		SSLLOG(("SSLAPI_InfoCallback: SSL3 alert %ssl:%ssl:%ssl\n", str, SSL_alert_type_string_long(ret), SSL_alert_desc_string_long(ret)));
	}
	else if (where & SSL_CB_EXIT)
	{
		if (ret == 0)
			SSLLOG(("SSLAPI_InfoCallback: %ssl:failed in %ssl\n", str, SSL_state_string_long(ssl)));
        else if (ret < 0)
			SSLLOG(("SSLAPI_InfoCallback: %ssl:error in %ssl\n", str, SSL_state_string_long(ssl)));
	}
	else
#endif
#if INFO_LEVEL>0
	if (where & SSL_CB_HANDSHAKE_DONE)
	{
		SSLLOG(("SSLAPI_InfoCallback: ssl handshake done, session %s\n\n", SSL_session_reused(ssl) ? "REUSED" : "NEGOTIATED"));
	}
#endif
#endif
#endif // _DEBUG
}


// NOTES:
//	See SSL[_CTX]_set_verify manpage for more information.
//
int SSLAPI_VerifyCallback(int preverify_ok, X509STORECTXPTR pSTR)
{
	X509_STORE_CTX * store = (X509_STORE_CTX *)pSTR;
    int err = X509_STORE_CTX_get_error(store);
    SSL * ssl = (SSL *)X509_STORE_CTX_get_ex_data(store, SSL_get_ex_data_X509_STORE_CTX_idx());
	X509 * x509 = X509_STORE_CTX_get_current_cert(store);
    int depth = X509_STORE_CTX_get_error_depth(store);
	char * pszHostname = (char *)SSL_get_ex_data(ssl, ssl_user_data_index);

	SSLLOG(("SSLAPI_VerifyCallback: pv_ok=%d, store=%x, err=%d, depth=%d, x509=%x\n", preverify_ok, store, err, depth, x509));

	int verify_ok = preverify_ok;

	if (ssl && x509 && pszHostname)
	{
		if (depth == 0)
		{
			// continue to ensure that the hostname to which we connected matches the common name in the current site certificate

			char szSubjectName[256] = {0};
			char szCommonName[256] = {0};
			bool fHostnameMatched = false;

			X509_NAME_oneline(X509_get_subject_name(x509), szSubjectName, sizeof(szSubjectName));
			X509_NAME_get_text_by_NID(X509_get_subject_name(x509), NID_commonName, szCommonName, sizeof(szCommonName));

			fHostnameMatched = (stricmp(szCommonName, pszHostname) == 0);

			// some common names contain wildcards, such as "*.cmpay.com", "*.eff.org", etc...
			if (!fHostnameMatched)
			{
				if (strncmp(szCommonName, "*.", 2) == 0)
				{
					char * pszCommonNameSuffix = &szCommonName[2];
					int lenHostname = strlen(pszHostname);
					int lenCommonNameSuffix = strlen(pszCommonNameSuffix);
					if (lenHostname >= lenCommonNameSuffix)
					{
						if (stricmp(pszCommonNameSuffix, &pszHostname[lenHostname-lenCommonNameSuffix]) == 0)
							fHostnameMatched = true;
					}
				}
			}
			
			verify_ok = (verify_ok && fHostnameMatched) ? 1 : 0;

			SSLLOG(("... subjectName=%s\n", szSubjectName));
			SSLLOG(("... commonName=%s\n", szCommonName));
			SSLLOG(("... current-hostname=%s, %s commonName\n", pszHostname, (fHostnameMatched ? "==" : "!=")));
		}
	}
	else
	{
		verify_ok = 0;
	}

	SSLLOG(("... return %d\n", verify_ok));
	return verify_ok;
}


// RETURN:
//  One of the following values:
//
//      SAERR_OK
//      SAERR_TIMEOUT
//      SAERR_ERROR
//
// NOTES:
//	tmout is the number of milliseconds to wait
//
int SSLAPI_WaitForIOReady(int fd, int forWrite, int tmout)
{
	const char * opr = (forWrite ? "writing" : "reading");

	SSLLOG(("SSLAPI_WaitForIOReady: Waiting %d ms for %s\n", tmout, opr));

	fd_set fds;
	struct timeval tv, *ptv = NULL;
    
	if (tmout >= 0) // wait for specified millisecs
	{
		tv.tv_sec = (long)(tmout/1000);
		tv.tv_usec = (long)(tmout%1000*1000);
		ptv = &tv;
	}
    
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
    
	int nErr = select(fd+1, forWrite ? NULL : &fds, forWrite ? &fds : NULL, NULL, ptv);
    
	const char * res = (nErr < 0 ? "Failed" : (nErr == 0 ? "Timeout" : "Ready"));

	SSLLOG(("SSLAPI_WaitForIOReady: %s\n", res)); 
    
    return nErr < 0 ? SAERR_ERROR : (nErr == 0 ? SAERR_TIMEOUT : SAERR_OK);
}


// Return cpu time in milliseconds
double SSLAPI_GetProcTime()
{
	return 1000.0 * clock() / CLOCKS_PER_SEC;
}


// Adjust timeout value
//
int SSLAPI_AdjustTimeout(double base, int tmout)
{
	double exp = base + (double)tmout;
	double cur = SSLAPI_GetProcTime();

	return exp <= cur ? 0 : (int)(exp - cur);
}

// RETURN:
//	SSL_CTX *	Success
//	NULL		Failure
//
SSLCTXPTR SSLAPI_SSLCTXNew(
	const SSLMETHODPTR pMETHOD,			// Such as TLSv1_method(void), ...
	const char * szCiphers,				// Use "DEFAULT" if NULL
	int verifyMode,						// one or more of SSL_VERIFY_NONE/_PEER/_FAIL_IF_NO_PEER_CERT/_CLIENT_ONCE
	SSL_VERIFY_CALLBACK verifyCallback,	// Verify callback, ignored if NULL
	SSL_INFO_CALLBACK infoCallback,		// Information Callback, ignored if NULL
	const char * szUserX509File,		// User certificate in PEM format, ignored if NULL.
	const char * szUserKeyFile,			// User key in PEM format, ignored if szUserX509File is NULL.
	const char * szUserKeyPass,			// User key passphrase, ignored if NULL.
	const char * szCaCertFile,			// File containing one or more CA certificates in PEM format.
	const char * szCaCertPath			// Directory storing CA certificate files.
)
{
	SSL_METHOD * method = (SSL_METHOD *)pMETHOD;
	SSL_CTX * ctx = NULL;

	do 
	{
		if (!method)
			break;

		ctx = SSL_CTX_new(method);

		if (!ctx)
			break;

		if (!szCiphers)
			szCiphers = "DEFAULT";

		if (!SSL_CTX_set_cipher_list(ctx, szCiphers))
        {
            SSLLOG(("SSLAPI_SSLCTXNew: bad ciphers '%s'\n", szCiphers));
            break;
        }

		SSL_CTX_set_verify(ctx, verifyMode, (REAL_SSL_VERIFY_CALLBACK)verifyCallback);

		if (infoCallback)
			SSL_CTX_set_info_callback(ctx, (REAL_SSL_INFO_CALLBACK)infoCallback);

		if (szUserX509File)
		{
			// SSL_CTX_use_certificate_chain_file() loads a certificate chain from file into ctx.
            // The certificates must be in PEM format and must be sorted starting with the subject's
            // certificate (actual client or server certificate), followed by intermediate
			// CA certificates if applicable, and ending at the highest level (root) CA.
			if (1 != SSL_CTX_use_certificate_chain_file(ctx, szUserX509File))
            {
                SSLLOG(("SSLAPI_SSLCTXNew: bad x509 file '%s'\n", szUserX509File));
                break;
            }
            
			if (!szUserKeyFile)
            {
                SSLLOG(("SSLAPI_SSLCTXNew: key file missing '%s'\n", szUserKeyFile));
                break;
            }

			// SSL_CTX_use_PrivateKey_file() adds the first private key found in file to ctx. The formatting
            // type of the certificate must be specified from the known types SSL_FILETYPE_PEM, SSL_FILETYPE_ASN1.
			if (1 != SSL_CTX_use_PrivateKey_file(ctx, szUserKeyFile, SSL_FILETYPE_PEM))
            {
                SSLLOG(("SSLAPI_SSLCTXNew: bad key file '%s'\n", szUserKeyFile));
                break;
            }

			int lenPass = szUserKeyPass ? strlen(szUserKeyPass) : 0;
			if (lenPass >= sizeof(user_pass))
            {
                SSLLOG(("SSLAPI_SSLCTXNew: key pass too long, limit is %d\n", sizeof(user_pass)-1));
                break;
            }
            
			// Both
			//		SSL_CTX_set_default_passwd_cb, and
			//		SSL_CTX_set_default_passwd_cb_userdata
			// always succeed
			SSL_CTX_set_default_passwd_cb(ctx, SSLAPI_PassCallback);
			SSL_CTX_set_default_passwd_cb_userdata(ctx, user_pass);
		}
		else
		{
			SSLLOG(("SSLAPI_SSLCTXNew: no user certs\n"));
		}

		// Load TRUSTED certificates
		if (szCaCertFile || szCaCertPath)
		{
			// NOTES: SSL_CTX_load_verify_location return 1 on success, otherwise 0 on failure
			if (1 != SSL_CTX_load_verify_locations(ctx, szCaCertFile, szCaCertPath))
            {
                SSLLOG(("SSLAPI_SSLCTXNew: bad verify localtion path '%s' or file '%s'\n", szCaCertFile, szCaCertPath));
                break;
            }
		}
        else
        {
			SSLLOG(("Warning: must specified verify location for higher security!\n"));
        }

#if defined(ENABLE_BUILTIN_CA_CERTS)
		SSLAPI_SSLCTXAddBuiltinChainCerts(ctx);
#endif

#if 1
		SSL_CTX_set_msg_callback(ctx, SSLAPI_MessageCallback);
		// SSL_CTX_set_tlsext_debug_callback(ctx, SSLAPI_TlsextCallback);
#endif
		SSLLOG(("SSLAPI_SSLCTXNew: succeeded\n"));
		return (SSLCTXPTR)ctx;
	}
	while (0);

	if (ctx)
		SSL_CTX_free(ctx);

	SSLLOG(("SSLAPI_SSLCTXNew: FAILED\n"));

	return NULL;
}


void SSLAPI_SSLCTXFree(SSLCTXPTR pCTX)
{
    SSL_CTX * ctx = (SSL_CTX *)pCTX;
    
	SSL_CTX_free(ctx);
    
    SSLLOG(("SSLAPI_SSLCTXFree: succeeded\n"));
}


void SSLAPI_SetConnectState(SSLPTR pSSL)
{
	SSL * ssl = (SSL *)pSSL;

	SSL_set_connect_state(ssl);
}

void SSLAPI_SetUserData(SSLPTR pSSL, void * pData)
{
	SSL * ssl = (SSL *)pSSL;

	SSL_set_ex_data(ssl, ssl_user_data_index, pData);
}


void * SSLAPI_GetUserData(SSLPTR pSSL)
{
	SSL * ssl = (SSL *)pSSL;

	return SSL_get_ex_data(ssl, ssl_user_data_index);
}


// RETURN:
//	SSL *	Success
//	NULL	Failure
//
SSLPTR SSLAPI_SSLNew(SSLCTXPTR pCTX)
{
	SSL_CTX * ctx = (SSL_CTX *)pCTX;
	SSL * ssl = NULL;

	do
	{
		ssl = SSL_new(ctx);
		if (!ssl)
			break;

		SSLLOG(("SSLAPI_SSLNew: succeeded\n"));

		return (SSLPTR)ssl;
	}
	while (0);

	if (ssl)
		SSL_free(ssl);

	SSLLOG(("SSLAPI_SSLNew: FAILED\n"));
	return NULL;
}


void SSLAPI_SSLFree(SSLPTR pSSL)
{
    SSL * ssl = (SSL *)pSSL;
    
    SSL_free(ssl);

    SSLLOG(("SSLAPI_SSLFree: Freed\n"));
}


// Clear SSL object for later reuse
//
void SSLAPI_SSLClear(SSLPTR pSSL)
{
    SSL * ssl = (SSL *)pSSL;
    
    SOCKET s = SSL_get_fd(ssl);
    
    SSL_set_fd(ssl, -1);
    
	// Warning: SOCKET is unsigned in WIN32 platform.
	if (SOCKET_IS_VALID(s))
        CLOSESOCKET(s);
    
    SSLLOG(("SSLAPI_SSLClear: Cleared\n"));
}


// Do SSL negotiating in non-blocking mode.
//
// Return:
//	0   Success
//	<0  SAERR_...
//
int SSLAPI_SSLConnectNB(SSLPTR pSSL, int tmout)
{
    int saErr = SAERR_ERROR;
    
	SSL * ssl = (SSL *)pSSL;
    
	double t0 = SSLAPI_GetProcTime();
    
    SOCKET s = SSL_get_fd(ssl);
    
    if (SOCKET_IS_VALID(s)) for (int i=0; ; ++i)
	{
		SSLLOG(("SSLAPI_SSLConnectNB: tmout=%d, loop=%d\n", tmout, i));
        
		int nErr = SSL_connect(ssl);
        
        if (nErr == 1)
        {
            // connected
            SSLLOG(("SSLAPI_SSLConnectNB: succeeded\n"));
            
            return SAERR_OK;
        }
		else if (nErr == 0)
        {
            // error
            SSLLOG(("SSLAPI_SSLConnectNB: shutdown controlled\n"));
			saErr = SAERR_EOF;
            break;
        }
        else // nErr < 0
        {
            nErr = SSL_get_error(ssl, nErr);
            
            SSLLOG(("SSLAPI_SSLConnectNB: %s detected\n", SSLAPI_SSLGetErrorName(nErr)));
            
            if (nErr != SSL_ERROR_WANT_READ && nErr != SSL_ERROR_WANT_WRITE)
			{
				saErr = SAERR_ERROR;
                break;
			}
            
            int saErr2 = (nErr == SSL_ERROR_WANT_READ) ? SSLAPI_WaitForReading(s, tmout) : SSLAPI_WaitForWriting(s, tmout);
            if (saErr2 < 0) // error or timeout
			{
				saErr = saErr2;
				break;
			}

            // tmout = SSLAPI_AdjustTimeout(t0, tmout);
        }
    }
    
    SSLLOG(("SSLAPI_SSLConnectNB: FAILED\n"));
    
	return saErr;
}


// Send SSL close notification to the server, and close the underlying socket.
//
// Return:
//	0		Success
//	<0		SAERR_...
//
int SSLAPI_SSLShutdownNB(SSLPTR pSSL, int tmout)
{
    int saErr = SAERR_ERROR;
    
	SSL * ssl = (SSL *)pSSL;
    
	double t0 = SSLAPI_GetProcTime();
    
    SOCKET s = SSL_get_fd(ssl);
    
    if (SOCKET_IS_VALID(s)) for (int i=0; ; ++i)
	{
		SSLLOG(("SSLAPI_SSLShutdownNB: timeout=%d, loop=%d\n", tmout, i));
        
		int nErr = SSL_shutdown(ssl);
        
		if (nErr == 0 /* unidirectional close done*/ || nErr == 1 /* bidirectional close done */)
		{
            SSLLOG(("SSLAPI_SSLShutdownNB: succeeded\n"));
			return SAERR_OK;
		}
        else // nErr < 0
        {
            nErr = SSL_get_error(ssl, nErr);
            
            SSLLOG(("SSLAPI_SSLShutdownNB: %s detected\n", SSLAPI_SSLGetErrorName(nErr)));
            
            if (nErr != SSL_ERROR_WANT_READ && nErr != SSL_ERROR_WANT_WRITE)
			{
				saErr = SAERR_ERROR;
                break;
			}

			int saErr2 = (nErr == SSL_ERROR_WANT_READ) ? SSLAPI_WaitForReading(s, tmout) : SSLAPI_WaitForWriting(s, tmout);
			if (saErr2 < 0) // error or timeout
			{
				saErr = saErr2;
				break;
			}

            // tmout = SSLAPI_AdjustTimeout(t0, tmout);
        }
  	}
    
	SSLLOG(("SSLAPI_SSLShutdownNB: FAILED\n"));
    
	return saErr;
}


// Shutdown the SSL connection without notifying the server.
//
void SSLAPI_SSLShutdownQuietly(SSLPTR pSSL)
{
	SSL * ssl = (SSL *)pSSL;
    
	SSL_set_shutdown(ssl, SSL_SENT_SHUTDOWN|SSL_RECEIVED_SHUTDOWN);
    
    SSLAPI_SSLClear((SSLPTR)ssl);
    
    SSLLOG(("SSLAPI_SSLShutdownQuietly: succeeded\n"));
}


// Check if the ssl session is reused. 
//
int SSLAPI_SSLSessionReused(SSLPTR pSSL)
{
    SSL * ssl = (SSL *)pSSL;
    
	return SSL_session_reused(ssl);
}

// RETURN:
//	0	connected
//	<0	SAERR_...
//
// NOTES:
//	SSL object pointed by pSSL must contain a valid socket that may or may not be fully connected.
//
int SSLAPI_CheckConnectionNB(SSLPTR pSSL, int tmout)
{
	SSLLOG(("SSLAPI_CheckConnectionNB: waiting for connection in %d ms\n", tmout));

	SSL * ssl = (SSL *)pSSL;

	SOCKET s = SSL_get_fd(ssl);

	if (SOCKET_IS_VALID(s))
		return SSLAPI_WaitForWriting(s, tmout);
	else
		return SAERR_ERROR;
}


// Prepare a socket connection for the specified ssl object.
//
// RETURN:
//  0       Success
//  <0      SAERR_...
//
// NOTES:
//	If this function succeeds, the socket fd in the SSL object may or may not be fully-connected. 
//	The caller should later call SSLAPI_CheckSocketConnectionNB() to see if the connection is completed.
//
int SSLAPI_PrepareConnectionToHost(SSLPTR pSSL, const char * szHost, unsigned short nPort)
{
    SSLLOG(("SSLAPI_PrepareConnectionToHostNB: connecting to %s:%u ...\n", szHost, nPort));
    
	SSL * ssl = (SSL *)pSSL;
    
    SOCKET s = SSLAPI_TcpConnectNB(szHost, nPort, 0);
	if (SOCKET_IS_VALID(s))
    {
        if (SSL_set_fd(ssl, s) == 1)
        {
			SSLLOG(("SSLAPI_PrepareConnectionToHostNB: succeeded, but the underlying connection may be pending\n"));
			return SAERR_OK;
		}
		else
		{
            SSLLOG(("SSLAPI_PrepareConnectionToHostNB: SSL_set_fd error\n"));
			CLOSESOCKET(s);
		}
	}
	else
	{
		SSLLOG(("SSLAPI_PrepareConnectionToHostNB: SSLAPI_TcpConnectNB return error %d\n", s));
	}

	SSLLOG(("SSLAPI_PrepareConnectionToHostNB: FAILED\n"));
    
	return SAERR_ERROR;
}


// RETURN:
//  0	Success
//  <0	SAERR_...
//
// NOTES:
//	If szHostName is NULL, SSLCheckServerCert does not check the common name of the peer's certificate
//

// Deprecated! Cert checking will be done in SSLAPI_VerifyCallback()
#if 0
int SSLAPI_CheckCert(SSLPTR pSSL, const char * szHostName)
{
	SSL * ssl = (SSL *)pSSL;
	X509 * x509Peer = NULL;
	char buf[256] = {0};

	do
	{
		// If the verify mode is SSL_VERIFY_NONE, this verify result is not X509_V_OK always
		// if (SSL_get_verify_result(ssl) != X509_V_OK)
		//	break;

		if (!(x509Peer = SSL_get_peer_certificate(ssl)))
			break;

		X509_NAME_get_text_by_NID(X509_get_subject_name(x509Peer), NID_commonName, buf, sizeof(buf));
		X509_free(x509Peer);

		if (szHostName && strcmp(buf, szHostName))
			break;

		SSLLOG(("SSLAPI_CheckCert: X509 name=%s, host=%s, check passed\n", buf, szHostName));
		return SAERR_OK;
	}
	while (0);

	SSLLOG(("SSLAPI_CheckCert: NOT passed\n"));
	return SAERR_ERROR;
}
#endif

// Trying to read at most len bytes.
//
// RETURN:
//	>0		The number, which may be less than len, of the bytes successfully read
//	0		Connection closed
//	<0		SAERR_...
//
// NOTES:
//	tmout:
//			-1	wait infinitely
//			0	don't wait
//			>0	wait for tmout millisecs
//
int SSLAPI_SSLReadNB(SSLPTR pSSL, char * buffer, int len, int tmout)
{
	int saErr = SAERR_ERROR;

	SSL * ssl = (SSL *)pSSL;
	int off = 0;

	double t0 = SSLAPI_GetProcTime();
    
    SOCKET s = SSL_get_fd(ssl);
    
    if (SOCKET_IS_VALID(s)) while (off < len)
	{
        SSLLOG(("SSLAPI_SSLReadNB: trying to read %d bytes in %d millisecs\n", len-off, tmout));
        
		int lenRead = SSL_read(ssl, buffer + off, len - off);
        
		if (lenRead > 0) // some bytes read
		{
			off += lenRead;
		}
		else if (lenRead == 0) // shutdown-ed
		{
			saErr = SAERR_EOF;
			break;
		}
		else // lenRead < 0
		{
			int nErr = SSL_get_error(ssl, lenRead);
            
            SSLLOG(("SSLAPI_SSLReadNB: SSL_read returned %s\n", SSLAPI_SSLGetErrorName(nErr)));
            
			if (nErr == SSL_ERROR_ZERO_RETURN) // shutdown-ed
			{
				saErr = SAERR_EOF;
				break;
			}
			else if (nErr != SSL_ERROR_WANT_READ && nErr != SSL_ERROR_WANT_WRITE) // other errors
			{
				saErr = SAERR_ERROR;
				break;
			}
            
            int saErr2 = (nErr == SSL_ERROR_WANT_READ) ? SSLAPI_WaitForReading(s, tmout) : SSLAPI_WaitForWriting(s, tmout);
            if (saErr2 < 0) // error or timeout
			{
				saErr = saErr2;
                break;
			}
            
            // tmout = SSLAPI_AdjustTimeout(t0, tmout);
		}
	}
    
	if (off > 0) // off has priority
	{
		SSLLOG(("SSLAPI_SSLReadNB: %d bytes read and returned\n", off));
		return off;
	}
	else if (saErr == SAERR_EOF || saErr == SAERR_OK) // off == 0
	{
		SSLLOG(("SSLAPI_SSLReadNB: EOF\n"));
		return 0;
	}
    else // other error conditions
    {
		SSLLOG(("SSLAPI_SSLReadNB: FAILED\n"));
		return saErr;
	}
}

// Trying to write at most len bytes.
//
// RETURN:
//	>0		The number, which may be less than len, of the bytes successfully written
//	0		Connection closed
//	<0		SAERR_...
//
int SSLAPI_SSLWriteNB(SSLPTR pSSL, char * buffer, int len, int tmout)
{
	int saErr = SAERR_ERROR;

	SSL * ssl = (SSL *)pSSL;
	int off = 0;
    
	double t0 = SSLAPI_GetProcTime();
    
    SOCKET s = SSL_get_fd(ssl);
    
    if (SOCKET_IS_VALID(s)) while (off < len)
	{
        SSLLOG(("SSLAPI_SSLWriteNB: trying to write %d bytes in %d millisecs\n", len-off, tmout));
        
		int lenWriten = SSL_write(ssl, buffer + off, len - off);
        
		if (lenWriten > 0) // some bytes writen
		{
			off += lenWriten;
		}
		else if (lenWriten == 0) // eof
		{
			saErr = SAERR_EOF;
			break;
		}
		else // lenWriten < 0, error
		{
			int nErr = SSL_get_error(ssl, lenWriten);
            
            SSLLOG(("SSLAPI_SSLWriteNB: SSL_write returned %s\n", SSLAPI_SSLGetErrorName(nErr)));
            
			if (nErr == SSL_ERROR_ZERO_RETURN) // shutdown-ed
			{
				saErr = SAERR_EOF;
				break;
			}
			else if (nErr != SSL_ERROR_WANT_READ && nErr != SSL_ERROR_WANT_WRITE) // other errors
			{
				saErr = SAERR_ERROR;
				break;
			}
            
            int saErr2 = (nErr == SSL_ERROR_WANT_READ) ? SSLAPI_WaitForReading(s, tmout) : SSLAPI_WaitForWriting(s, tmout);
            if (saErr2 < 0) // error or timeout
			{
				saErr = saErr2;
                break;
			}
			// tmout = SSLAPI_AdjustTimeout(t0, tmout);
		}
	}
    
	if (off > 0) // off has priority
	{
		SSLLOG(("SSLAPI_SSLWriteNB: %d bytes writen\n", off));
		return off;
	}
	else if (saErr == SAERR_EOF || saErr == SAERR_OK) // off == 0
	{
		SSLLOG(("SSLAPI_SSLWriteNB: EOF\n"));
		return 0;
	}
    else // other error conditions
    {
		SSLLOG(("SSLAPI_SSLWriteNB: FAILED\n"));
		return saErr;
	}
}


// Read a nul-terminated line from the SSL connection.
//
// RETURN:
//	>0	Length of the received line that may not have a trailing LF due to insufficient buffer or other reasons.
//      Use SSLAPI_HasTrailingLF to check if this line is complete.
//	0	Connection closed
//	<0	SAERR_...
//
int SSLAPI_SSLReadLine(SSLPTR pSSL, char * buffer, int sizeBuffer, int tmout)
{
    int saErr = SAERR_ERROR;

	SSL * ssl = (SSL *)pSSL;
	int off = 0;
    
	sizeBuffer--; // Preserved 1 byte for the terminating NUL
    
	while (off < sizeBuffer)
	{
		int lenRead = SSLAPI_SSLReadNB((SSLPTR)ssl, buffer + off, 1, tmout);
        
		if (lenRead == 0) // eof
		{
			saErr = SAERR_EOF;
			break;
		}
		else if (lenRead == 1)
		{
			if (buffer[off++] == '\n')
            {
				saErr = SAERR_OK;
				break;
            }
		}
		else // lenRead < 0 
		{
            saErr = lenRead;
            break;
		}
	}
    
    buffer[off] = 0;
    
	if (off > 0) // off has priority
	{
		SSLLOG(("SSLAPI_SSLReadLine: %d bytes read and returned\n", off));
		return off;
	}
	else if (saErr == SAERR_EOF || saErr == SAERR_OK) // off == 0
	{
		SSLLOG(("SSLAPI_SSLReadLine: EOF\n"));
		return 0;
	}
    else // other error conditions
    {
		SSLLOG(("SSLAPI_SSLReadLine: FAILED\n"));
		return saErr;
	}
}


// Read the entire nul-terminated http message header, of which the last line is empty, from the SSL connection.
//
// RETURN:
//	>0	Length of the received http message header. This header may be partial due to insufficient buffer or other reasons.
//      The caller may traverse the returned buffer to see if it contains a complete http message header.
//	0	EOF
//	<0	SAERR_...
//
int SSLAPI_SSLReadHttpMsgNB(SSLPTR pSSL, char * buffer, int sizeBuffer, int tmout)
{
    int saErr = SAERR_ERROR;

	SSL * ssl = (SSL *)pSSL;
    int off = 0;
    
    while (off < sizeBuffer)
	{
		char * p = buffer + off;
        
		int lenRead = SSLAPI_SSLReadLine((SSLPTR)ssl, p, sizeBuffer - off, tmout);
        
		if (lenRead < 0) // error
        {
            saErr = lenRead;
            break;
		}
        else if (lenRead == 0) // eof
		{
			saErr = SAERR_EOF;
            break;
		}
        else if (!SSLAPI_HasTrailingLF(p)) // lenRead > 0
        {
			// incomplete line
            saErr = SAERR_BADDATA;
            break;
        }
        else
        {
			// complete line
            off += lenRead;
            if (SSLAPI_IsEmptyLine(p))
			{
				saErr = SAERR_OK;
                break;
			}
        }
	}
    
    buffer[off] = 0;
    
	if (off > 0) // off has priority
	{
		SSLLOG(("SSLAPI_SSLReadHttpMsgNB: %d bytes read and returned\n", off));
		return off;
	}
	else if (saErr == SAERR_EOF || saErr == SAERR_OK) // off == 0
	{
		SSLLOG(("SSLAPI_SSLReadHttpMsgNB: EOF\n"));
		return 0;
	}
    else // other error conditions
    {
		SSLLOG(("SSLAPI_SSLReadHttpMsgNB: FAILED\n"));
		return saErr;
	}
}

// RETURN:
//  0	Success
//	<0	Failure
//
int SSLAPI_SetBlockingMode(SOCKET s, int non_blocking)
{
#if defined(_WIN32)
	u_long iMode = non_blocking;
	if (ioctlsocket(s, FIONBIO, &iMode) == 0)
		return 0;
#else
    int flag = fcntl(s, F_GETFL, 0);
    if (flag != -1)
    {
		if (non_blocking)
			flag = flag | O_NONBLOCK;
		else
			flag = flag & ~O_NONBLOCK;

        if (fcntl(s, F_SETFL, flag) != -1)
			return 0;
	}
#endif
	return SAERR_ERROR;
}

// Create a TCP connection to the specified host, in non-blocking mode.
//
// RETURN:
//  >=0 the client-side socket.
//  <0  SAERR_...
//
// NOTES:
//	The behavior of this function vary according the value of timeout:
//
//		If tmout != 0, this function returns a connection only when its fully-connected:
//				tmout <0	this function will wait infinitely until the connection is completed or some error occurs.
//				tmout >0	this function will wait tmout milliseconds for the connection.
//
//		If tmout == 0, this function will not wait. It returns a connection even it's pre-mature.
//
int SSLAPI_TcpConnectNB(const char * szHost, unsigned short nPort, int tmout)
{
    struct addrinfo hints, *res, *res0;
    SOCKET s = -1;
	int saErr = SAERR_ERROR;
    
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;
    
    if (getaddrinfo(szHost, NULL, &hints, &res0))
    {
		return SAERR_ERROR;
    }
    
	// Traverse all the available addresses
    for (res = res0; res; res = res->ai_next)
    {
        s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if (SOCKET_IS_VALID(s))
		{
			if (SSLAPI_SetBlockingMode(s, 1) == 0)
			{
				struct sockaddr_in sin = *(sockaddr_in *)(res->ai_addr);
				sin.sin_port = htons(nPort);
				if (connect(s, (sockaddr *)&sin, sizeof(sin)) == 0)
					break; // connected

#if defined(_WIN32)
				if (WSAGetLastError() == WSAEWOULDBLOCK)
#else // unix/linux-based system
				if (errno == EINPROGRESS)
#endif
				{
					// connection pending
					if (tmout == 0)
						break; // dont wait

					saErr = SSLAPI_WaitForWriting(s, tmout);
					if (saErr == 0)
						break; // connected
				}
			}

			CLOSESOCKET(s);
			s = -1;
		}
	} // for

    if (res0)
        freeaddrinfo(res0);
    
	if (SOCKET_IS_VALID(s))
		return s;
	else if (saErr < 0)
		return saErr;
	else
		return SAERR_ERROR;
}


// RETURN:
//	>=0	Length of the nul-teriminated value
//	<0	SAERR_...
//
int SSLAPI_ExtractHttpHeader(const char * szHttpMsg, int lenHttpMsg, const char * szName, char * buffer, int sizeBuffer)
{
	int lenName = strlen(szName);

	if (lenHttpMsg < 0)
		lenHttpMsg = strlen(szHttpMsg);

	// Bypass the first line
	const char * pLineBegin = strchr(szHttpMsg, '\n');

	while (pLineBegin)
	{
		++pLineBegin;
		const char * pLineEnd = strchr(pLineBegin, '\n');
		if (!pLineEnd) // LF not found in this line
			break;

		const char * pColon = strchr(pLineBegin, ':');
		if (!pColon || pColon > pLineEnd) // Colon not found within this line 
			break;

		if ((lenName == (int)(pColon - pLineBegin)) && strnicmp(szName, pLineBegin, lenName) == 0)
		{
			// Name match
			const char * pValue = pColon + 1;
			int lenValue = (int)(pLineEnd - pValue);
			if (lenValue >= sizeBuffer) // Buffer low
				break;

			strncpy(buffer, pValue, lenValue);
			buffer[lenValue] = 0;
			SSLAPI_Trim(buffer);

			return strlen(buffer);
		}

		// go to the next line
		pLineBegin = pLineEnd;
	}

	return SAERR_NOTFOUND;
}


// RETURN:
//	>=0	Content length
//	<0	SAERR_...
//
int SSLAPI_ExtractHttpContentLength(const char * szHttpMsg, int lenHttpMsg)
{
	char buffer[32];
	
	int saErr = SSLAPI_ExtractHttpHeader(szHttpMsg, lenHttpMsg, "Content-Length", buffer, sizeof(buffer));
	if (saErr < 0)
		return saErr;

	SSLAPI_Trim(buffer);
	return atoi(buffer);
}


// RETURN:
//	1	Connection: Keep-Alive
//	0	Connection: Close
//  <0  SAERR_...
//
int SSLAPI_ExtractHttpConnection(const char * szHttpMsg, int lenHttpMsg)
{
	char buffer[32];

	int saErr = SSLAPI_ExtractHttpHeader(szHttpMsg, lenHttpMsg, "Connection", buffer, sizeof(buffer));
	if (saErr < 0)
		return saErr;

	return stricmp(buffer, "Keep-Alive") == 0 ? 1 : 0;
}


// Read an HTTP request from stdin
//
// Return:
//	>0	Length
//  <0  SAERR_...
//
int SSLAPI_GetHttpRequest(char * buffer, int sizeBuffer)
{
	--sizeBuffer;

	int off = 0;

	while (off < sizeBuffer)
	{
		char * p = buffer + off;
		int sizeFree = sizeBuffer - off;

		if (!fgets(p, sizeFree, stdin))
			break;

		int lenRead = strlen(p);
		if (!lenRead)
			break;

		if (p[lenRead-1] != '\n')
			break;

		off += lenRead;

		if (SSLAPI_IsEmptyLine(p)) // Done
			return off;
	}

	return SAERR_ERROR;
}


// RETURN:
//	Always sz
//
char * SSLAPI_TrimLeft(char * sz)
{
	int len = strlen(sz);
	int off;

	for (off = 0; off < len && isspace(sz[off]); ++off);

	if (off)
		strcpy(sz, sz+off);

	return sz;
}

// RETURN:
//	Always sz
//
char * SSLAPI_TrimRight(char * sz)
{
	int len = strlen(sz);
	int off;

	for (off = len; off >=0 && isspace(sz[off]); --off);

	sz[off+1] = 0;
	
	return sz;
}

// RETURN:
//	Always sz
//
char * SSLAPI_Trim(char * sz)
{
	return SSLAPI_TrimLeft(SSLAPI_TrimRight(sz));
}

// RETURN:
//	1	sz ends with an LF
//	0	No
//
int SSLAPI_HasTrailingLF(const char * sz)
{
	int len = strlen(sz);

	return len > 0 && sz[len-1] == '\n' ? 1 : 0;
}

// RETURN:
//	1	sz is a blank string
//	0	No
//
int SSLAPI_IsBlankString(const char * sz)
{
	for (int off = strlen(sz)-1; off >= 0; --off)
		if (!isspace(sz[off]))
			return 0;

	return 1;
}

// RETURN:
//	1	sz is an empty line
//	0	No		
// 
int SSLAPI_IsEmptyLine(const char * sz)
{
	return strcmp(sz, "\n") == 0 || strcmp(sz, "\r\n") == 0 ? 1 : 0;
}


/////////////////////////////////////////////////////////////////////////////
//
// SSL session cache management apis.
//
#if defined(ENABLE_SSL_SESSION_CACHE)

#if defined(ENABLE_SSL_THREAD)

#define LOCK_SESS_CACHE() SSLAPI_LockCallback(CRYPTO_LOCK, sess_cache_lock_type, __FILE__, __LINE__)
#define UNLOCK_SESS_CACHE() SSLAPI_LockCallback(CRYPTO_UNLOCK, sess_cache_lock_type, __FILE__, __LINE__)

#else

#define LOCK_SESS_CACHE() do { } while (0)
#define LOCK_SESS_CACHE() do { } while (0)

#endif

// Session Cache Entry
typedef struct
{
	char host[SSL_HOST_SIZE];
	int port; // always 0, not used in this impl.
	SSL_SESSION * sess;

} SSCACHE_ENTRY;

#define SSCACHE_SIZE 10

static SSCACHE_ENTRY ssl_sess_cache[SSCACHE_SIZE];

#define SSCACHE_ENTRY_PTR(i) (&ssl_sess_cache[i])

#define SSCACHE_ZERO_ENTRY(entry_ptr) do { \
	(entry_ptr)->host[0] = 0; \
	(entry_ptr)->port = 0; \
	(entry_ptr)->sess = 0; \
	} while (0)

#define SSCACHE_RELEASE_ENTRY(entry_ptr) do { \
	if ((entry_ptr)->sess) \
		SSL_SESSION_free((entry_ptr)->sess); \
	SSCACHE_ZERO_ENTRY(entry_ptr); \
	} while (0)


//
// Flush the session cache. All expired entries will be removed from the cache and freed
//
void SSLAPI_FlushSessionCache()
{
	for (int i=0; i<SSCACHE_SIZE; ++i)
	{
		SSCACHE_ENTRY * p = SSCACHE_ENTRY_PTR(i);

		if (p->sess)
		{
			long nSessTimeout = SSL_SESSION_get_timeout(p->sess);
			long nSessTime = SSL_SESSION_get_time(p->sess);
			if (nSessTime + nSessTimeout < (long)time(NULL))
				SSCACHE_RELEASE_ENTRY(p);
		}
	}
}


//
// RETURN:
//	>=0	The id of the first empty entry
//	<0	Empty entry not found
//
int SSLAPI_FindEmptySessionCacheEntry()
{
	for (int i=0; i<SSCACHE_SIZE; ++i)
	{
		if (!SSCACHE_ENTRY_PTR(i)->sess)
			return i;
	}

	return -1;
}

//
// RETURN:
//	>=0	The id of the entry of the oldest session in the cache
//	<0	Oldest entry not found
//
int SSLAPI_FindOldestSessionCacheEntry()
{
	long nOldest = (long)time(NULL) + 1;
	int id = -1;

	for (int i=0; i<SSCACHE_SIZE; ++i)
	{
		if (SSCACHE_ENTRY_PTR(i)->sess)
		{
			long nSessTime = SSL_SESSION_get_time(SSCACHE_ENTRY_PTR(i)->sess);
			if (nSessTime < nOldest)
			{
				nOldest = nSessTime;
				id = i;
			}
		}
	}

	return id;
}


//
// RETURN:
//	>=0	The id of the session entry associated with the specified host
//	<0	host not found
//
int SSLAPI_FindSessionCacheEntryByHost(const char * host)
{
	for (int i=0; i<SSCACHE_SIZE; ++i)
	{
		SSCACHE_ENTRY * p = SSCACHE_ENTRY_PTR(i);

		if (p->sess && stricmp(host, ssl_sess_cache[i].host) == 0)
			return i;
	}
	return -1;
}


void SSLAPI_SessionCacheSetup()
{
	for (int i=0; i<SSCACHE_SIZE; ++i)

		SSCACHE_ZERO_ENTRY(SSCACHE_ENTRY_PTR(i));
}


void SSLAPI_SessionCacheCleanup()
{
	for (int i=0; i<SSCACHE_SIZE; ++i)

		if (ssl_sess_cache[i].sess)
			SSCACHE_RELEASE_ENTRY(SSCACHE_ENTRY_PTR(i));
}


//
// RETURN:
//	SSL_SESSION *	Success, the session is removed from the cache
//	NULL			Session not found, or other error occurred
//
// NOTES:
//	If this function returns a session, the reference count of that session is not increased.
//
SSLSESSPTR SSLAPI_GetSessionFromCache(const char * host)
{
	SSL_SESSION * sess = NULL;

	LOCK_SESS_CACHE();

	SSLAPI_FlushSessionCache();

	int id = SSLAPI_FindSessionCacheEntryByHost(host);
	if (id >= 0)
		sess = ssl_sess_cache[id].sess;

	UNLOCK_SESS_CACHE();

	return (SSLSESSPTR)sess;
}

//
// Set a session into the cache.
//
// RETURN:
//	0	Success
//	<0	SAERR_...
//
// NOTES:
//	The reference count of the session set into this cache is not increased.
//
int SSLAPI_SetSessionToCache(const char * host, SSLSESSPTR pSESS)
{
	if (!pSESS || !host || strlen(host) >= sizeof(ssl_sess_cache[0].host))
		return SAERR_ERROR;

	int id;

	LOCK_SESS_CACHE();

	SSLAPI_FlushSessionCache();

	id = SSLAPI_FindSessionCacheEntryByHost(host);
	if (id >=0)
	{
		SSCACHE_RELEASE_ENTRY(&ssl_sess_cache[id]);
	}
	else // id < 0
	{
		id = SSLAPI_FindEmptySessionCacheEntry();
		if (id < 0)
		{
			id = SSLAPI_FindOldestSessionCacheEntry();
			if (id >= 0)
			{
				SSCACHE_RELEASE_ENTRY(&ssl_sess_cache[id]);
			}
		}
	}

	if (id >= 0)
	{
		SSCACHE_ENTRY * p = SSCACHE_ENTRY_PTR(id);
		p->sess = (SSL_SESSION *)pSESS;
		p->port = 0;
		strcpy(p->host, host);
	}

	UNLOCK_SESS_CACHE();

	return id>=0 ? 0 : SAERR_ERROR;
}

//
// RETURN:
//	SSLSESSPTR	Success
//	NULL		Failure
//
// NOTES:
//	The reference count of the returned session is increased.
//
SSLSESSPTR SSLAPI_Get1SessionFromSSL(SSLPTR pSSL)
{
	SSL * ssl = (SSL *)pSSL;

	return SSL_get1_session(ssl);
}


//
// RETURN:
//	0	Success
//	<0	SAERR_...
//
// NOTES:
//	The reference count of the session is not increased.
//
int SSLAPI_SetSessionToSSL(SSLPTR pSSL, SSLSESSPTR pSESS)
{
	SSL * ssl = (SSL *)pSSL;
	SSL_SESSION * sess = (SSL_SESSION *)pSESS;

	return SSL_set_session(ssl, sess) == 1 ? 0 : SAERR_ERROR;
}


//
// RETURN:
//	none
//
void SSLAPI_SSLSessionFree(SSLSESSPTR pSESS)
{
	SSL_SESSION_free((SSL_SESSION *)pSESS);
}


#endif // ENABLE_SSL_SESSION_CACHE


/////////////////////////////////////////////////////////////////////////////
//
// Builtin ca certs for testing purpose
//

const char * SSLAPI_GetTestCaCertFileName()
{
#if defined(_WIN32)
	return "c:\\testcacerts.pem";
#elif defined(__APPLE__)
	return "/tmp/testcacerts.pem";
#elif defined(__ANDROID__)
	return "/tmp/testcacerts.pem";
#else
#error TODO
#endif
}


// RETURN:
//	0	Success
//	<0	SAERR_...
//
int SSLAPI_CreateTestCaCertFile()
{
	// NOTES:
	//  These certificates are exporeted from IE8

	const char *builtinCerts[] = {

		// Make sure all of these certs are not expired!!!
		
        // ./interm/DigiCert High Assurance CA-3.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIGWDCCBUCgAwIBAgIQCl8RTQNbF5EX0u/UA4w/OzANBgkqhkiG9w0BAQUFADBs\n"
        "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
        "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
        "ZSBFViBSb290IENBMB4XDTA4MDQwMjEyMDAwMFoXDTIyMDQwMzAwMDAwMFowZjEL\n"
        "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
        "LmRpZ2ljZXJ0LmNvbTElMCMGA1UEAxMcRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n"
        "Q0EtMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL9hCikQH17+NDdR\n"
        "CPge+yLtYb4LDXBMUGMmdRW5QYiXtvCgFbsIYOBC6AUpEIc2iihlqO8xB3RtNpcv\n"
        "KEZmBMcqeSZ6mdWOw21PoF6tvD2Rwll7XjZswFPPAAgyPhBkWBATaccM7pxCUQD5\n"
        "BUTuJM56H+2MEb0SqPMV9Bx6MWkBG6fmXcCabH4JnudSREoQOiPkm7YDr6ictFuf\n"
        "1EutkozOtREqqjcYjbTCuNhcBoz4/yO9NV7UfD5+gw6RlgWYw7If48hl66l7XaAs\n"
        "zPw82W3tzPpLQ4zJ1LilYRyyQLYoEt+5+F/+07LJ7z20Hkt8HEyZNp496+ynaF4d\n"
        "32duXvsCAwEAAaOCAvowggL2MA4GA1UdDwEB/wQEAwIBhjCCAcYGA1UdIASCAb0w\n"
        "ggG5MIIBtQYLYIZIAYb9bAEDAAIwggGkMDoGCCsGAQUFBwIBFi5odHRwOi8vd3d3\n"
        "LmRpZ2ljZXJ0LmNvbS9zc2wtY3BzLXJlcG9zaXRvcnkuaHRtMIIBZAYIKwYBBQUH\n"
        "AgIwggFWHoIBUgBBAG4AeQAgAHUAcwBlACAAbwBmACAAdABoAGkAcwAgAEMAZQBy\n"
        "AHQAaQBmAGkAYwBhAHQAZQAgAGMAbwBuAHMAdABpAHQAdQB0AGUAcwAgAGEAYwBj\n"
        "AGUAcAB0AGEAbgBjAGUAIABvAGYAIAB0AGgAZQAgAEQAaQBnAGkAQwBlAHIAdAAg\n"
        "AEMAUAAvAEMAUABTACAAYQBuAGQAIAB0AGgAZQAgAFIAZQBsAHkAaQBuAGcAIABQ\n"
        "AGEAcgB0AHkAIABBAGcAcgBlAGUAbQBlAG4AdAAgAHcAaABpAGMAaAAgAGwAaQBt\n"
        "AGkAdAAgAGwAaQBhAGIAaQBsAGkAdAB5ACAAYQBuAGQAIABhAHIAZQAgAGkAbgBj\n"
        "AG8AcgBwAG8AcgBhAHQAZQBkACAAaABlAHIAZQBpAG4AIABiAHkAIAByAGUAZgBl\n"
        "AHIAZQBuAGMAZQAuMBIGA1UdEwEB/wQIMAYBAf8CAQAwNAYIKwYBBQUHAQEEKDAm\n"
        "MCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5jb20wgY8GA1UdHwSB\n"
        "hzCBhDBAoD6gPIY6aHR0cDovL2NybDMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0SGln\n"
        "aEFzc3VyYW5jZUVWUm9vdENBLmNybDBAoD6gPIY6aHR0cDovL2NybDQuZGlnaWNl\n"
        "cnQuY29tL0RpZ2lDZXJ0SGlnaEFzc3VyYW5jZUVWUm9vdENBLmNybDAfBgNVHSME\n"
        "GDAWgBSxPsNpA/i/RwHUmCYaCALvY2QrwzAdBgNVHQ4EFgQUUOpzidsp+xCPnuUB\n"
        "INTeeZlIg/cwDQYJKoZIhvcNAQEFBQADggEBAB7ipUiebNtTOA/vphoqrOIDQ+2a\n"
        "vD6OdRvw/S4iWawTwGHi5/rpmc2HCXVUKL9GYNy+USyS8xuRfDEIcOI3ucFbqL2j\n"
        "CwD7GhX9A61YasXHJJlIR0YxHpLvtF9ONMeQvzHB+LGEhtCcAarfilYGzjrpDq6X\n"
        "dF3XcZpCdF/ejUN83ulV7WkAywXgemFhM9EZTfkI7qA5xSU1tyvED7Ld8aW3DiTE\n"
        "JiiNeXf1L/BXunwH1OH8zVowV36GEEfdMR/X/KLCvzB8XSSq6PmuX2p0ws5rs0bY\n"
        "Ib4p1I5eFdZCSucyb6Sxa1GDWL4/bcf72gMhy2oWGU4K8K2Eyl2Us1p292E=\n"
        "-----END CERTIFICATE-----\n",

        // ./interm/Microsoft Certificate Trust List PCA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIF6jCCA9KgAwIBAgIKYRGdxAAAAAAAKjANBgkqhkiG9w0BAQUFADBfMRMwEQYK\n"
        "CZImiZPyLGQBGRYDY29tMRkwFwYKCZImiZPyLGQBGRYJbWljcm9zb2Z0MS0wKwYD\n"
        "VQQDEyRNaWNyb3NvZnQgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkwHhcNMDkw\n"
        "NDAyMTczNzEwWhcNMTkwNDAyMTc0NzEwWjCBgzELMAkGA1UEBhMCVVMxEzARBgNV\n"
        "BAgTCldhc2hpbmd0b24xEDAOBgNVBAcTB1JlZG1vbmQxHjAcBgNVBAoTFU1pY3Jv\n"
        "c29mdCBDb3Jwb3JhdGlvbjEtMCsGA1UEAxMkTWljcm9zb2Z0IENlcnRpZmljYXRl\n"
        "IFRydXN0IExpc3QgUENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA\n"
        "pgKXmmA02BV0cs2coHuhAeGj3YyNZSUJ5TlsyEk2rtGoTiSf6PueSVFE5vHLf2BV\n"
        "HjFRU9elTkp/EpIDHL7qjzuqpBVcaAwJ0hJaJTGcogMEht65SJPQ8c9rxLqbfkfI\n"
        "q15h/a8KkGofnWyrNDftQhT7hwgNp1/iJmDmuTRcd0Y8XgyT3kvleGv5oU4v3FH5\n"
        "e7j5akncU7Wzy3DYI3Pucld07XkihJoWJtUQDsoYEOA6IIUFW5TOkdzqjGGqaoWv\n"
        "SUgr4YGFx9vo2LhFw1Mh4FCUCVWzhlYUA9urZ53YLlL4cvMUQs/1FJnIAt1HjqS9\n"
        "HW95HYeZvn0Zzx9VOI1c/wIDAQABo4IBgTCCAX0wDwYDVR0TAQH/BAUwAwEB/zAd\n"
        "BgNVHQ4EFgQU4ZKXqF9UTRnNK+oho9ke/KEx38MwCwYDVR0PBAQDAgGGMBIGCSsG\n"
        "AQQBgjcVAQQFAgMBAAEwIwYJKwYBBAGCNxUCBBYEFOh+eATTdJZ02t3w20MR4CK3\n"
        "DfvCMBkGCSsGAQQBgjcUAgQMHgoAUwB1AGIAQwBBMB8GA1UdIwQYMBaAFA6sgmBA\n"
        "VieX5SUT/CrhClOVWeSkMFAGA1UdHwRJMEcwRaBDoEGGP2h0dHA6Ly9jcmwubWlj\n"
        "cm9zb2Z0LmNvbS9wa2kvY3JsL3Byb2R1Y3RzL21pY3Jvc29mdHJvb3RjZXJ0LmNy\n"
        "bDBUBggrBgEFBQcBAQRIMEYwRAYIKwYBBQUHMAKGOGh0dHA6Ly93d3cubWljcm9z\n"
        "b2Z0LmNvbS9wa2kvY2VydHMvTWljcm9zb2Z0Um9vdENlcnQuY3J0MCEGA1UdJQQa\n"
        "MBgGCisGAQQBgjcKAwEGCisGAQQBgjcKAwkwDQYJKoZIhvcNAQEFBQADggIBANHP\n"
        "GBcN8iHA6MgxAf574jEUvDpDVFhD1rKHRJuUj9Ivbrm14z/hXUEtgIJdlyDl0Jod\n"
        "dKa2pdbHEtK4/ECiKf/QxOvfEi7jd0ajuEcXtEe4UPG2BGOAmxYHJ2wbubySFUtW\n"
        "d4iLNw1QDC695zY00S0Bqt8E19tAxdTrTTHJ0SXSxed4hqKVFqzG9rkLUaY14LVQ\n"
        "hvu5aAsDk53dsyqWnd5XrYuEx0rMWiJU3r3QCaLYvCH7h5ZdGhe0HbvgjDKBO6xH\n"
        "fGAicJXv/Vm9hDoc8XodSn0QnY12/+4JIsPOU4hqi+xnin95sA19kzKdLYcjkLfi\n"
        "GpcxphFIBbufxv0WCxcCpwG+qKW+q2ddLt/JF/y3cXHVwOe5b1H8Vxet/RO+BDJn\n"
        "jqqtCMyElCCkhPwktkoDVrzDiS4AiTQrKwgIhF7gKeskh9IcsRq1Gs0UNWyiJW/h\n"
        "L8sa1+SP2N13bLe4le8Am/cmoy+x+PFEjntGn4OLRp7u1o52Hc/1uSuueo0ow64O\n"
        "aqoJ/R/kSvLNjw2MG6QAdp/snXXiNL4cHN4y4Aazl+DGCCdTTwgyM9cOf3xxJZqj\n"
        "OwZtMgLmbjonLUFPFqOWsGuB2zNrbICxF5FoLtPMbg2N+eOrFdRFHYegxT3XF7Zj\n"
        "I7St7aeyh/0ieq5bJg/LdYVfNxKS5zExa/xb2GA6\n"
        "-----END CERTIFICATE-----\n",

        // ./interm/Microsoft Internet Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIFEjCCBHugAwIBAgIEBydiAjANBgkqhkiG9w0BAQUFADB1MQswCQYDVQQGEwJV\n"
        "UzEYMBYGA1UEChMPR1RFIENvcnBvcmF0aW9uMScwJQYDVQQLEx5HVEUgQ3liZXJU\n"
        "cnVzdCBTb2x1dGlvbnMsIEluYy4xIzAhBgNVBAMTGkdURSBDeWJlclRydXN0IEds\n"
        "b2JhbCBSb290MB4XDTEwMDQxNDE4MTIyNloXDTE4MDQxNDE4MTIxNFowJzElMCMG\n"
        "A1UEAxMcTWljcm9zb2Z0IEludGVybmV0IEF1dGhvcml0eTCCAiIwDQYJKoZIhvcN\n"
        "AQEBBQADggIPADCCAgoCggIBAL30zSelStiQGduyCntgpE6PDpijnHxQduuzSoyf\n"
        "GLDnmsUrgoYJKKwREjIm9Rnq8LhnaMUG/fQZrtkToNCBJwimeZwE9UgyLjYfq2sm\n"
        "7KFDtJ2AsEkD6oJJXwUTxaCDX+Eq9AQZS37I2oi8tV0DvHhWoel/xWrvtv8dAVm3\n"
        "H1NaX8b4kW3FfUOTGHRF7RW6snzIOjQUHqpj9+XUS8gjK4dplROZCRTvegEgTrfG\n"
        "SEGuyYcBKdnChzh/tkKk8LLOLf20TFfwqNbLTvpfXf25+wncFoVk5XGc1fEzlzhn\n"
        "Lpu8FzYFfhA2f37rmFpbHK2l5wkQfflKL7OPNxXWb7lbN9y3n3+OZn8jXO0Sf4wH\n"
        "8P4Z+bg0Q3uy6oX7jKmq3/2RDSz1+6+XifEGiq9J9jwuI/ZEFiWREeIjw8qFVUkq\n"
        "yCGvfREmhrAoRbqH7jYTgdVLRxqO2wnx0ZcpUBQymQnj8sDnU49r9PoTXDyN7lSZ\n"
        "DydHTjwS848SF0bwiWpFs7U8DHdFBC+9vrWemDwFO7tBOYQgvHkE1kLNPonp53o3\n"
        "SRC0zJ8kXCOmSG7749TuISmT5P2AGhs6bMH369nUTb7xEfaijkIkoU9ptdJoFInZ\n"
        "n5DYH54b5m1kJSm2NEOkW/UN63QGfp/xY9xFp3w6mlxrc9jDWASOiG8T0ObQ383E\n"
        "Cg4HAgMBAAGjggF3MIIBczASBgNVHRMBAf8ECDAGAQH/AgEBMFsGA1UdIARUMFIw\n"
        "SAYJKwYBBAGxPgEAMDswOQYIKwYBBQUHAgEWLWh0dHA6Ly9jeWJlcnRydXN0Lm9t\n"
        "bmlyb290LmNvbS9yZXBvc2l0b3J5LmNmbTAGBgRVHSAAMA4GA1UdDwEB/wQEAwIB\n"
        "hjCBiQYDVR0jBIGBMH+heaR3MHUxCzAJBgNVBAYTAlVTMRgwFgYDVQQKEw9HVEUg\n"
        "Q29ycG9yYXRpb24xJzAlBgNVBAsTHkdURSBDeWJlclRydXN0IFNvbHV0aW9ucywg\n"
        "SW5jLjEjMCEGA1UEAxMaR1RFIEN5YmVyVHJ1c3QgR2xvYmFsIFJvb3SCAgGlMEUG\n"
        "A1UdHwQ+MDwwOqA4oDaGNGh0dHA6Ly93d3cucHVibGljLXRydXN0LmNvbS9jZ2kt\n"
        "YmluL0NSTC8yMDE4L2NkcC5jcmwwHQYDVR0OBBYEFDMh8Mv+oqBEkt72OzPYXwFL\n"
        "l3hdMA0GCSqGSIb3DQEBBQUAA4GBACtI85T7RMWTatZN/rQTThImF8qyWqsJuVak\n"
        "b39XnmSy9eTTNe9jZcvlLBWc7874KsWSZCtJPjw2bL0Ym2Rnlz/taNAWwRM88lGg\n"
        "V94kzjWraZBOKww6+bTxgPptAHmmOpaZTjpuVNCjWW6LHZVJu5XYdbjhEjOsXCe7\n"
        "y1Vx1frt\n"
        "-----END CERTIFICATE-----\n",

        // ./interm/RapidSSL CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIID1TCCAr2gAwIBAgIDAjbRMA0GCSqGSIb3DQEBBQUAMEIxCzAJBgNVBAYTAlVT\n"
        "MRYwFAYDVQQKEw1HZW9UcnVzdCBJbmMuMRswGQYDVQQDExJHZW9UcnVzdCBHbG9i\n"
        "YWwgQ0EwHhcNMTAwMjE5MjI0NTA1WhcNMjAwMjE4MjI0NTA1WjA8MQswCQYDVQQG\n"
        "EwJVUzEXMBUGA1UEChMOR2VvVHJ1c3QsIEluYy4xFDASBgNVBAMTC1JhcGlkU1NM\n"
        "IENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAx3H4Vsce2cy1rfa0\n"
        "l6P7oeYLUF9QqjraD/w9KSRDxhApwfxVQHLuverfn7ZB9EhLyG7+T1cSi1v6kt1e\n"
        "6K3z8Buxe037z/3R5fjj3Of1c3/fAUnPjFbBvTfjW761T4uL8NpPx+PdVUdp3/Jb\n"
        "ewdPPeWsIcHIHXro5/YPoar1b96oZU8QiZwD84l6pV4BcjPtqelaHnnzh8jfyMX8\n"
        "N8iamte4dsywPuf95lTq319SQXhZV63xEtZ/vNWfcNMFbPqjfWdY3SZiHTGSDHl5\n"
        "HI7PynvBZq+odEj7joLCniyZXHstXZu8W1eefDp6E63yoxhbK1kPzVw662gzxigd\n"
        "gtFQiwIDAQABo4HZMIHWMA4GA1UdDwEB/wQEAwIBBjAdBgNVHQ4EFgQUa2k9ahhC\n"
        "St2PAmU5/TUkhniRFjAwHwYDVR0jBBgwFoAUwHqYaI2J+6sFZAwRfap9ZbjKzE4w\n"
        "EgYDVR0TAQH/BAgwBgEB/wIBADA6BgNVHR8EMzAxMC+gLaArhilodHRwOi8vY3Js\n"
        "Lmdlb3RydXN0LmNvbS9jcmxzL2d0Z2xvYmFsLmNybDA0BggrBgEFBQcBAQQoMCYw\n"
        "JAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmdlb3RydXN0LmNvbTANBgkqhkiG9w0B\n"
        "AQUFAAOCAQEAq7y8Cl0YlOPBscOoTFXWvrSY8e48HM3P8yQkXJYDJ1j8Nq6iL4/x\n"
        "/torAsMzvcjdSCIrYA+lAxD9d/jQ7ZZnT/3qRyBwVNypDFV+4ZYlitm12ldKvo2O\n"
        "SUNjpWxOJ4cl61tt/qJ/OCjgNqutOaWlYsS3XFgsql0BYKZiZ6PAx2Ij9OdsRu61\n"
        "04BqIhPSLT90T+qvjF+0OJzbrs6vhB6m9jRRWXnT43XcvNfzc9+S7NIgWW+c+5X4\n"
        "knYYCnwPLKbK3opie9jzzl9ovY8+wXS7FXI6FoOpC+ZNmZzYV+yoAVHHb1c0XqtK\n"
        "LEL2TxyJeN4mTvVvk0wVaydWTQBUbHq3tw==\n"
        "-----END CERTIFICATE-----\n",

        // ./interm/Root Agency.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIByjCCAXSgAwIBAgIQBjdsAKoAZIoRz7jUqlw19DANBgkqhkiG9w0BAQQFADAW\n"
        "MRQwEgYDVQQDEwtSb290IEFnZW5jeTAeFw05NjA1MjgyMjAyNTlaFw0zOTEyMzEy\n"
        "MzU5NTlaMBYxFDASBgNVBAMTC1Jvb3QgQWdlbmN5MFswDQYJKoZIhvcNAQEBBQAD\n"
        "SgAwRwJAgVUiuYqkb+3W59lmD1W8183VvE5AAiGisfeHMIVe0vJEudybdbb7Rl9C\n"
        "tp0jNgveVA/NvR+ZKhBYEctAy7WnQQIDAQABo4GeMIGbMFAGA1UEAwRJE0dGb3Ig\n"
        "VGVzdGluZyBQdXJwb3NlcyBPbmx5IFNhbXBsZSBTb2Z0d2FyZSBQdWJsaXNoaW5n\n"
        "IENyZWRlbnRpYWxzIEFnZW5jeTBHBgNVHQEEQDA+gBAS5AktBh0dTwCNYSHcFmRj\n"
        "oRgwFjEUMBIGA1UEAxMLUm9vdCBBZ2VuY3mCEAY3bACqAGSKEc+41KpcNfQwDQYJ\n"
        "KoZIhvcNAQEEBQADQQAtLj57iUKJP6ghF/rw9cOV22JpW8ncwbP68MRvb2Savecb\n"
        "JWhyg2e9VrCNAb0q98xLvYeluocgTEIRQa0QFzuM\n"
        "-----END CERTIFICATE-----\n",

        // ./interm/UTN - DATACorp SGC.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDuDCCAqCgAwIBAgIQCHyOH62bVqUR01aaLEpooDANBgkqhkiG9w0BAQQFADAd\n"
        "MRswGQYDVQQDExJSb290IFNHQyBBdXRob3JpdHkwHhcNOTkwODIwMDA1NzEwWhcN\n"
        "MTkwNjI0MDcwMDAwWjCBkzELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAlVUMRcwFQYD\n"
        "VQQHEw5TYWx0IExha2UgQ2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3\n"
        "b3JrMSEwHwYDVQQLExhodHRwOi8vd3d3LnVzZXJ0cnVzdC5jb20xGzAZBgNVBAMT\n"
        "ElVUTiAtIERBVEFDb3JwIFNHQzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\n"
        "ggEBAN/uWBCiK25VxI6/LkYJ5+AIDy4rehOUG732toCOZQWTAB68r+IPjhkNEkfs\n"
        "rK2j+i5w+N5u+1ZCFZ4uXO8j3iG5BXYnGQ9P1sOctL6UGWPyphEK61NInL7yKTsW\n"
        "6BqgTKbJ9BhZaMBw8lMAwF5QgqVWbzb5SuBEhqBNTtZHbklKy2fXpsQFuY4e9Pz/\n"
        "zec24JwFbLIzIhXQtODMF8CywPT+Mj8pKpV72PKnTg9UfKENgLMJA8H/XN1emj68\n"
        "rrxHimquccofsSq4X0IFC+xGMNFyC8rpVm317994vmG6sqWuBEy8qKxpFZe97+u0\n"
        "jL81+NTD0SgOXDqfcBgzIHfEoq8CAwEAAaN9MHswDQYDVR0KBAYwBAMCB4AwIAYD\n"
        "VR0lBBkwFwYKKwYBBAGCNwoDAwYJYIZIAYb4QgQBMEgGA1UdAQRBMD+AEA0nKeQF\n"
        "Kpe0d1g1R5MtBrihHzAdMRswGQYDVQQDExJSb290IFNHQyBBdXRob3JpdHmCCiCd\n"
        "EdEOf3uFdIAwDQYJKoZIhvcNAQEEBQADggEBAOL46F2ulXRXY9XRON+hgsGSFEyI\n"
        "S0qGyYFcNz+GrXsphzTZ4D9kQuRNHu8l3o8Zftgky9G5zLt8ESNlrZkY/m4wf4Rb\n"
        "6sxb1snrex9sUYlw0uPYTQpeMA558Cgmb98WPfxqRRpFekId1b9Q0cOf+5iGvdr5\n"
        "+5ex64V8RhD1CelH0ixeoFLrSCo8YKX73mamkOHaOiMCD6+del0DMzumpSYNRlTf\n"
        "U0rMtVZAMoczfbNI2OgFy71/SVvYP4Np65UbzFOUuBgZEMECwsPDF/pf3J+XziII\n"
        "4ldv1i2U5+P20h0PQ60OF8InzEAX8UVgS9/B6Imcbg5UV6wH21OwmCYSZss=\n"
        "-----END CERTIFICATE-----\n",

        // ./interm/VeriSign Class 3 International Server CA - G3.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIGKTCCBRGgAwIBAgIQZBvoIM4CCBPzLU0tldZ+ZzANBgkqhkiG9w0BAQUFADCB\n"
        "yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\n"
        "ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\n"
        "U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\n"
        "ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\n"
        "aG9yaXR5IC0gRzUwHhcNMTAwMjA4MDAwMDAwWhcNMjAwMjA3MjM1OTU5WjCBvDEL\n"
        "MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\n"
        "ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTswOQYDVQQLEzJUZXJtcyBvZiB1c2UgYXQg\n"
        "aHR0cHM6Ly93d3cudmVyaXNpZ24uY29tL3JwYSAoYykxMDE2MDQGA1UEAxMtVmVy\n"
        "aVNpZ24gQ2xhc3MgMyBJbnRlcm5hdGlvbmFsIFNlcnZlciBDQSAtIEczMIIBIjAN\n"
        "BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmdacYvAV9IGaQQhZjxOdF8mfUdza\n"
        "sVLv/+NB3eDfxCjG4615HycQmLi7IJfBKERBD+qpqFLPTU4bi7u1xHbZzFYG7rNV\n"
        "ICreFY1xy1TIbxfNiQDk3P/hwB9ocenHKS5+vDv85burJlSLZpDN9pK5MSSAvJ5s\n"
        "1fx+0uFLjNxC+kRLX/gYtS4w9D0SmNNiBXNUppyiHb5SgzoHRsQ7AlYhv/JRT9Cm\n"
        "mTnprqU/iZucff5NYAclIPe712mDK4KTQzfZg0EbawurSmaET0qO3n40mY5o1so5\n"
        "BptMs5pITRNGtFghBMT7oE2sLktiEuP7TfbJUQABH/weaoEqOOC5T9YtRQIDAQAB\n"
        "o4ICFTCCAhEwEgYDVR0TAQH/BAgwBgEB/wIBADBwBgNVHSAEaTBnMGUGC2CGSAGG\n"
        "+EUBBxcDMFYwKAYIKwYBBQUHAgEWHGh0dHBzOi8vd3d3LnZlcmlzaWduLmNvbS9j\n"
        "cHMwKgYIKwYBBQUHAgIwHhocaHR0cHM6Ly93d3cudmVyaXNpZ24uY29tL3JwYTAO\n"
        "BgNVHQ8BAf8EBAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2Uv\n"
        "Z2lmMCEwHzAHBgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDov\n"
        "L2xvZ28udmVyaXNpZ24uY29tL3ZzbG9nby5naWYwNAYDVR0lBC0wKwYIKwYBBQUH\n"
        "AwEGCCsGAQUFBwMCBglghkgBhvhCBAEGCmCGSAGG+EUBCAEwNAYIKwYBBQUHAQEE\n"
        "KDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC52ZXJpc2lnbi5jb20wNAYDVR0f\n"
        "BC0wKzApoCegJYYjaHR0cDovL2NybC52ZXJpc2lnbi5jb20vcGNhMy1nNS5jcmww\n"
        "KAYDVR0RBCEwH6QdMBsxGTAXBgNVBAMTEFZlcmlTaWduTVBLSS0yLTcwHQYDVR0O\n"
        "BBYEFNebfNgioBX33a1fzimbWMO8RgC1MB8GA1UdIwQYMBaAFH/TZafC3ey78DAJ\n"
        "80M5+gKvMzEzMA0GCSqGSIb3DQEBBQUAA4IBAQBxtX1zUkrd1000Ky6vlEalSVAC\n"
        "T/gvF3DyE9wfIYaqwk98NzzURniuXXhv0bpavBCrWDbFjGIVRWAXIeLVQqh3oVXY\n"
        "QwRR9m66SOZdTLdE0z6k1dYzmp8N5tdOlkSVWmzWoxZTDphDzqS4w2Z6BVxiEOgb\n"
        "Ett9LnZQ/9/XaxvMisxx+rNAVnwzeneUW/ULU/sOX7xo+68q7jA3eRaTJX9NEP9X\n"
        "+79uOzMh3nnchhdZLUNkt6Zmh+q8lkYZGoaLb9e3SQBb26O/KZru99MzrqP0nkzK\n"
        "XmnUG623kHdq2FlveasB+lXwiiFm5WVu/XzT3x7rfj8GkPsZC9MGAht4Q5mo\n"
        "-----END CERTIFICATE-----\n",

        // ./interm/VeriSign Class 3 Secure Server CA - G2.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIGLDCCBZWgAwIBAgIQbk/6s8XmacTRZ8mSq+hYxDANBgkqhkiG9w0BAQUFADCB\n"
        "wTELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMTwwOgYDVQQL\n"
        "EzNDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9yaXR5\n"
        "IC0gRzIxOjA4BgNVBAsTMShjKSAxOTk4IFZlcmlTaWduLCBJbmMuIC0gRm9yIGF1\n"
        "dGhvcml6ZWQgdXNlIG9ubHkxHzAdBgNVBAsTFlZlcmlTaWduIFRydXN0IE5ldHdv\n"
        "cmswHhcNMDkwMzI1MDAwMDAwWhcNMTkwMzI0MjM1OTU5WjCBtTELMAkGA1UEBhMC\n"
        "VVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZWZXJpU2lnbiBU\n"
        "cnVzdCBOZXR3b3JrMTswOQYDVQQLEzJUZXJtcyBvZiB1c2UgYXQgaHR0cHM6Ly93\n"
        "d3cudmVyaXNpZ24uY29tL3JwYSAoYykwOTEvMC0GA1UEAxMmVmVyaVNpZ24gQ2xh\n"
        "c3MgMyBTZWN1cmUgU2VydmVyIENBIC0gRzIwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n"
        "DwAwggEKAoIBAQDUVo9XOzcopkBj0pXVBXTatRlqltZxVy/iwDSMoJWzjOE3JPMu\n"
        "7UNFBY6J1/raSrX4Po1Ox/lJUEU3QJ90qqBRVWHxYISJpZ6AjS+wIapFgsTPtBR/\n"
        "RxUgKIKwaBLArlwH1/ZZzMtiVlxNSf8miKtUUTovStoOmOKJcrn892g8xB85essX\n"
        "gfMMrQ/cYWIbEAsEHikYcV5iy0PevjG6cQIZTiapUdqMZGkD3pz9ff17Ybz8hHyI\n"
        "XLTDe+1fK0YS8f0AAZqLW+mjBS6PLlve8xt4+GaRCMBeztWwNsrUqHugffkwer/4\n"
        "3RlRKyC6/qfPoU6wZ/WAqiuDLtKOVImOHikLAgMBAAGjggKpMIICpTA0BggrBgEF\n"
        "BQcBAQQoMCYwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLnZlcmlzaWduLmNvbTAS\n"
        "BgNVHRMBAf8ECDAGAQH/AgEAMHAGA1UdIARpMGcwZQYLYIZIAYb4RQEHFwMwVjAo\n"
        "BggrBgEFBQcCARYcaHR0cHM6Ly93d3cudmVyaXNpZ24uY29tL2NwczAqBggrBgEF\n"
        "BQcCAjAeGhxodHRwczovL3d3dy52ZXJpc2lnbi5jb20vcnBhMDQGA1UdHwQtMCsw\n"
        "KaAnoCWGI2h0dHA6Ly9jcmwudmVyaXNpZ24uY29tL3BjYTMtZzIuY3JsMA4GA1Ud\n"
        "DwEB/wQEAwIBBjBtBggrBgEFBQcBDARhMF+hXaBbMFkwVzBVFglpbWFnZS9naWYw\n"
        "ITAfMAcGBSsOAwIaBBSP5dMahqyNjmvDz4Bq1EgYLHsZLjAlFiNodHRwOi8vbG9n\n"
        "by52ZXJpc2lnbi5jb20vdnNsb2dvLmdpZjApBgNVHREEIjAgpB4wHDEaMBgGA1UE\n"
        "AxMRQ2xhc3MzQ0EyMDQ4LTEtNTIwHQYDVR0OBBYEFKXvCxHOwEEDo0plkEiyHOBX\n"
        "LX1HMIHnBgNVHSMEgd8wgdyhgcekgcQwgcExCzAJBgNVBAYTAlVTMRcwFQYDVQQK\n"
        "Ew5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xhc3MgMyBQdWJsaWMgUHJpbWFy\n"
        "eSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcyMTowOAYDVQQLEzEoYykgMTk5\n"
        "OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MR8wHQYD\n"
        "VQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrghB92f4Hz6getxB5Z/uniTTGMA0G\n"
        "CSqGSIb3DQEBBQUAA4GBAGN0Lz1Tqi+X7CYRZhr+8d5BJxnSf9jBHPniOFY6H5Cu\n"
        "OcUgdav4bC1nHynCIdcUiGNLsJsnY5H48KMBJLb7j+M9AgtvVP7UzNvWhb98lR5e\n"
        "YhHB2QmcQrmy1KotmDojYMyimvFu6M+O0Ro8XhnF15s1sAIjJOUFuNWI4+D6ufRf\n"
        "-----END CERTIFICATE-----\n",

        // ./interm/VeriSign Class 3 Secure Server CA - G3.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIF7DCCBNSgAwIBAgIQbsx6pacDIAm4zrz06VLUkTANBgkqhkiG9w0BAQUFADCB\n"
        "yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\n"
        "ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\n"
        "U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\n"
        "ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\n"
        "aG9yaXR5IC0gRzUwHhcNMTAwMjA4MDAwMDAwWhcNMjAwMjA3MjM1OTU5WjCBtTEL\n"
        "MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\n"
        "ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTswOQYDVQQLEzJUZXJtcyBvZiB1c2UgYXQg\n"
        "aHR0cHM6Ly93d3cudmVyaXNpZ24uY29tL3JwYSAoYykxMDEvMC0GA1UEAxMmVmVy\n"
        "aVNpZ24gQ2xhc3MgMyBTZWN1cmUgU2VydmVyIENBIC0gRzMwggEiMA0GCSqGSIb3\n"
        "DQEBAQUAA4IBDwAwggEKAoIBAQCxh4QfwgxF9byrJZenraI+nLr2wTm4i8rCrFbG\n"
        "5btljkRPTc5v7QlK1K9OEJxoiy6Ve4mbE8riNDTB81vzSXtig0iBdNGIeGwCU/m8\n"
        "f0MmV1gzgzszChew0E6RJK2GfWQS3HRKNKEdCuqWHQsV/KNLO85jiND4LQyUhhDK\n"
        "tpo9yus3nABINYYpUHjoRWPNGUFP9ZXse5jUxHGzUL4os4+guVOc9cosI6n9FAbo\n"
        "GLSa6Dxugf3kzTU2s1HTaewSulZub5tXxYsU5w7HnO1KVGrJTcW/EbGuHGeBy0RV\n"
        "M5l/JJs/U0V/hhrzPPptf4H1uErT9YU3HLWm0AnkGHs4TvoPAgMBAAGjggHfMIIB\n"
        "2zA0BggrBgEFBQcBAQQoMCYwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLnZlcmlz\n"
        "aWduLmNvbTASBgNVHRMBAf8ECDAGAQH/AgEAMHAGA1UdIARpMGcwZQYLYIZIAYb4\n"
        "RQEHFwMwVjAoBggrBgEFBQcCARYcaHR0cHM6Ly93d3cudmVyaXNpZ24uY29tL2Nw\n"
        "czAqBggrBgEFBQcCAjAeGhxodHRwczovL3d3dy52ZXJpc2lnbi5jb20vcnBhMDQG\n"
        "A1UdHwQtMCswKaAnoCWGI2h0dHA6Ly9jcmwudmVyaXNpZ24uY29tL3BjYTMtZzUu\n"
        "Y3JsMA4GA1UdDwEB/wQEAwIBBjBtBggrBgEFBQcBDARhMF+hXaBbMFkwVzBVFglp\n"
        "bWFnZS9naWYwITAfMAcGBSsOAwIaBBSP5dMahqyNjmvDz4Bq1EgYLHsZLjAlFiNo\n"
        "dHRwOi8vbG9nby52ZXJpc2lnbi5jb20vdnNsb2dvLmdpZjAoBgNVHREEITAfpB0w\n"
        "GzEZMBcGA1UEAxMQVmVyaVNpZ25NUEtJLTItNjAdBgNVHQ4EFgQUDURcFlNEwYJ+\n"
        "HSCrJfQBY9i+eaUwHwYDVR0jBBgwFoAUf9Nlp8Ld7LvwMAnzQzn6Aq8zMTMwDQYJ\n"
        "KoZIhvcNAQEFBQADggEBAAyDJO/dwwzZWJz+NrbrioBL0aP3nfPMU++CnqOh5pfB\n"
        "WJ11bOAdG0z60cEtBcDqbrIicFXZIDNAMwfCZYP6j0M3m+oOmmxw7vacgDvZN/R6\n"
        "bezQGH1JSsqZxxkoor7YdyT3hSaGbYcFQEFn0Sc67dxIHSLNCwuLvPSxe/20majp\n"
        "dirhGi2HbnTTiN0eIsbfFrYrghQKlFzyUOyvzv9iNw2tZdMGQVPtAhTItVgooazg\n"
        "W+yzf5VK+wPIrSbb5mZ4EkrZn0L74ZjmQoObj49nJOhhGbXdzbULJgWOw27EyHW4\n"
        "Rs/iGAZeqa6ogZpHFt4MKGwlJ7net4RYxh84HqTEy2Y=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Certiposte Editeur.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDuzCCAqOgAwIBAgIEAQAAADANBgkqhkiG9w0BAQQFADBJMQswCQYDVQQGEwJG\n"
        "UjETMBEGA1UEChMKQ2VydGlwb3N0ZTElMCMGA1UEAxMcQ2VydGlwb3N0ZSBDbGFz\n"
        "c2UgQSBQZXJzb25uZTAeFw05ODA2MjQwODAwMDBaFw0xODA2MjQwODAwMDBaMEkx\n"
        "CzAJBgNVBAYTAkZSMRMwEQYDVQQKEwpDZXJ0aXBvc3RlMSUwIwYDVQQDExxDZXJ0\n"
        "aXBvc3RlIENsYXNzZSBBIFBlcnNvbm5lMIIBITANBgkqhkiG9w0BAQEFAAOCAQ4A\n"
        "MIIBCQKCAQAox3xaJSN48rMAR0Biy2+MQlCfnl7UXA5lC1hWlSvjRtBhNuAtRpuC\n"
        "y5Hu0pV8mpKvBAp+pp/g17HDRfmYQRs5redW19m2f867OS4sO8+2cwODzhNdMmpj\n"
        "ottb+Esz6FBsy6gX7J6TuWwGSyYLdx6e+eWMiTfS0bv9qYwrLJwQMdhLjM23cX44\n"
        "LCnjF7JP6FK245I80v3hAtphEHTSGvPI0dFmB1/EhGNpva5s3GUjHLf798YTLoN+\n"
        "P6nlCyBtAQo34lzait4icOkN4HQ9xOtxm2Eq4g0Ui0xGN0wm0mjWVsNXqqJgN69f\n"
        "naCzgILmQypMgAAJUNmoanNtA/5ec5LlAgMBAAGjgaswgagwDgYDVR0PAQH/BAQD\n"
        "AgEGMBIGA1UdEwEB/wQIMAYBAf8CAQMwPwYDVR0OBDgENkM9IEZSLCBPPSBDZXJ0\n"
        "aXBvc3RlLCBDTj0gQ2VydGlwb3N0ZSBDbGFzc2UgQSBQZXJzb25uZTBBBgNVHSME\n"
        "OjA4gDZDPSBGUiwgTz0gQ2VydGlwb3N0ZSwgQ049IENlcnRpcG9zdGUgQ2xhc3Nl\n"
        "IEEgUGVyc29ubmUwDQYJKoZIhvcNAQEEBQADggEBAB6Ii98OcWRdSMSQRBLLJ12P\n"
        "k+NzhQvMQFtX86A3bxlIo5S7WiZVDySOeOeeBILseNsJyuGFg+9xWWG6Jqq6bn0d\n"
        "84gBsrKKiTx2s2KmXsLsZFuCvf+wlDp2LPl+KYZKWXpeKLFE2L1GGKmnbDC5Ckfn\n"
        "qRyQKKfySVQ2sjPHBrFObIc5QHodnI9tozSj3dq7Dw5YhDc2yeMxBUhdY0lMMnB9\n"
        "xdgqqowKMGDdNASa4ULYyaphuZe820rESDThsBNYk4jA/6jUKmrIiu0nl/ehe6Ts\n"
        "eZhXYVPsg2C/d3po0N7adbPuY+b7sEiRagvprQqTrDyJM6qkRhdU1Z2FTXvKOvY=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Certiposte Serveur.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDkzCCAnugAwIBAgIEAgAAADANBgkqhkiG9w0BAQQFADA/MQswCQYDVQQGEwJG\n"
        "UjETMBEGA1UEChMKQ2VydGlwb3N0ZTEbMBkGA1UEAxMSQ2VydGlwb3N0ZSBTZXJ2\n"
        "ZXVyMB4XDTk4MDYyNDA4MDAwMFoXDTE4MDYyNDA4MDAwMFowPzELMAkGA1UEBhMC\n"
        "RlIxEzARBgNVBAoTCkNlcnRpcG9zdGUxGzAZBgNVBAMTEkNlcnRpcG9zdGUgU2Vy\n"
        "dmV1cjCCASEwDQYJKoZIhvcNAQEBBQADggEOADCCAQkCggEAPqd4MziR4spWldGR\n"
        "RrIj3KVWfw/Hl7jinpEuKBYujvNHtlyDxPDm/mcak2R9QAnYHWgA7NQR7thjiQMj\n"
        "6YRfx1UOSuJyuynBVWR2qOOSzff2bkJWGFIzifmAFKgZUX7Hd796qDHAfRduQ07V\n"
        "ZskFPkvi5eJVG3SqUR2qzJgRd6Tr2yoOJ8fVzUjs3H5OSedBzZkL0UteGBsNOgqb\n"
        "LZs5x0AM0HYHfHyPflxyjofVvvKmud6/3Z8SeUd8xcmXQEYAktMTWpNPnX7YX0zB\n"
        "XUhJ2S2YPPzMp4ch8D6Ocn1ZODYWGgxIvqHy1T6oFgwgWc/ODzODPa2r9f/d1/1p\n"
        "CIgA7wIDAQABo4GXMIGUMA4GA1UdDwEB/wQEAwIBBjASBgNVHRMBAf8ECDAGAQH/\n"
        "AgEDMDUGA1UdDgQuBCxDPSBGUiwgTz0gQ2VydGlwb3N0ZSwgQ049IENlcnRpcG9z\n"
        "dGUgU2VydmV1cjA3BgNVHSMEMDAugCxDPSBGUiwgTz0gQ2VydGlwb3N0ZSwgQ049\n"
        "IENlcnRpcG9zdGUgU2VydmV1cjANBgkqhkiG9w0BAQQFAAOCAQEACArMs7ciiZrv\n"
        "VKv/PBKQ9O5+gcQAL27NobHE2ryy3rJBQPwsB57vpKMnv2ZUtka+hG+ri2JaNBQL\n"
        "qDmijh/dGYmvUYPTX+n76X1IhlkqcNHtN3Bwec3iOvtqjtVf9665TMoUTs/f123t\n"
        "x8bQrhZkonWnNm+Yhb7+4Q67ecf1janH7GcN/SXsfx7jJpreWULf7v7Cvpr4R7qI\n"
        "JcmdHIntmf7JPM5n6cDBv17uSW63er7NkVnMFHwK1QaGFLMybFkzaeGrvFm4k3lR\n"
        "efiXDmuOe+FhJgHYezYHf44LvPRPwhSrzi9+Aq3o8pWDguJuZDIUP1F1jMa+LPwv\n"
        "REXfFcUW+w==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Certisign Autoridade Certificadora AC1S.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICZDCCAc2gAwIBAgIBBTANBgkqhkiG9w0BAQQFADBvMQswCQYDVQQGEwJCUjEu\n"
        "MCwGA1UEChMlQ2VydGlzaWduIENlcnRpZmljYWRvcmEgRGlnaXRhbCBMdGRhLjEw\n"
        "MC4GA1UECxMnQ2VydGlzaWduIEF1dG9yaWRhZGUgQ2VydGlmaWNhZG9yYSBBQzFT\n"
        "MB4XDTk5MDYyNzAwMDAwMFoXDTE4MDYyNzAwMDAwMFowbzELMAkGA1UEBhMCQlIx\n"
        "LjAsBgNVBAoTJUNlcnRpc2lnbiBDZXJ0aWZpY2Fkb3JhIERpZ2l0YWwgTHRkYS4x\n"
        "MDAuBgNVBAsTJ0NlcnRpc2lnbiBBdXRvcmlkYWRlIENlcnRpZmljYWRvcmEgQUMx\n"
        "UzCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEAsMCV66PFQfibb3FjjpERV/kD\n"
        "x2iaGpxh8MMPDJdiiH4mATV9/fFvTxeVqn7yu7zv4nZzYZU2UYM+hOg/ulc2N5QT\n"
        "bJJkH5QN61QvUVrlKF7wkobn5G0hel83C+dbvS0CqIH5P8sUyLykxePRyzfcq+WH\n"
        "qvILmC56z8wewn/o/QECAwEAAaMQMA4wDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0B\n"
        "AQQFAAOBgQAbrqnYdrx6nviLGuf7RdNeMA6TvfipXQNCm/Bkxnk9Q2k8QKE2RkiC\n"
        "VuiI8DqV5r7oobZAvf/WLK2hMwZzeAJl6e17e0mEbWTT852bbJ/hQwOYYMC10+Xr\n"
        "SKlJYuK5zswkCtgiwT03MAScdf6IugSleqIA3u0BHaY5E6Rs2LWaog==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Certisign Autoridade Certificadora AC2.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICajCCAdOgAwIBAgIBADANBgkqhkiG9w0BAQQFADByMQswCQYDVQQGEwJCUjEu\n"
        "MCwGA1UEChMlQ2VydGlzaWduIENlcnRpZmljYWRvcmEgRGlnaXRhbCBMdGRhLjEz\n"
        "MDEGA1UECxMqQ2VydGlzaWduIC0gQXV0b3JpZGFkZSBDZXJ0aWZpY2Fkb3JhIC0g\n"
        "QUMyMB4XDTk5MDYyNzAwMDAwMFoXDTE4MDYyNzAwMDAwMFowcjELMAkGA1UEBhMC\n"
        "QlIxLjAsBgNVBAoTJUNlcnRpc2lnbiBDZXJ0aWZpY2Fkb3JhIERpZ2l0YWwgTHRk\n"
        "YS4xMzAxBgNVBAsTKkNlcnRpc2lnbiAtIEF1dG9yaWRhZGUgQ2VydGlmaWNhZG9y\n"
        "YSAtIEFDMjCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEAuTDMpeuQ1laUZzOJ\n"
        "g21GvnCXkxdOCidwzsnF8sXSLdERnHcybHTTkm/66dW4YZqqTBM+NTrw7gk22dGX\n"
        "McderEFJv0htTiBTyXtqo5Yxa7JuqM8vckwGrTLPWgjzfRCcqru6c/nz8Lgkb6hl\n"
        "tiL+BVsQVUTCAx2t0SLMCG37jMsCAwEAAaMQMA4wDAYDVR0TBAUwAwEB/zANBgkq\n"
        "hkiG9w0BAQQFAAOBgQBIa969g3LQgWoJEgXcYwOVaoZILqriJUtiZDyHhKQwpL2l\n"
        "HotWEldDZE/H6Y+9qvEadXmKU5H2Q10Cmg/XQhQ6go30aR6wrFHOydXbYE4ocChR\n"
        "SerdgyZhnAv1jqoVp7UKvg+4jQLU5RFAPG7HDGN8UWPa4lIQcj1NsyPRTQVBxA==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Certisign Autoridade Certificadora AC3S.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICszCCAhwCAQAwDQYJKoZIhvcNAQEEBQAwgaExCzAJBgNVBAYTAkJSMRcwFQYD\n"
        "VQQIEw5SaW8gZGUgSmFuZWlybzEXMBUGA1UEBxMOUmlvIGRlIEphbmVpcm8xLjAs\n"
        "BgNVBAoTJUNlcnRpc2lnbiBDZXJ0aWZpY2Fkb3JhIERpZ2l0YWwgTHRkYS4xMDAu\n"
        "BgNVBAsTJ0NlcnRpc2lnbiBBdXRvcmlkYWRlIENlcnRpZmljYWRvcmEgQUMzUzAe\n"
        "Fw05OTA3MDkyMDU2MzJaFw0xODA3MDkyMDU2MzJaMIGhMQswCQYDVQQGEwJCUjEX\n"
        "MBUGA1UECBMOUmlvIGRlIEphbmVpcm8xFzAVBgNVBAcTDlJpbyBkZSBKYW5laXJv\n"
        "MS4wLAYDVQQKEyVDZXJ0aXNpZ24gQ2VydGlmaWNhZG9yYSBEaWdpdGFsIEx0ZGEu\n"
        "MTAwLgYDVQQLEydDZXJ0aXNpZ24gQXV0b3JpZGFkZSBDZXJ0aWZpY2Fkb3JhIEFD\n"
        "M1MwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAM5kTtbPrnyYJ5jvKO+6NMM1\n"
        "Gp3SZh5x8eSWLScXOZBFjy3v5tVHqeGDgXw+KHH6gLtd+ENrGRDBmG8Vni2uSWRE\n"
        "7IQaaVkWV7I9kthmS7+ZrUMYxzjJi44sBC6zS2uT9p0fhLLN5gDCvjalvFm1n4sZ\n"
        "IP/lqCikVbaKtrLF10TjAgMBAAEwDQYJKoZIhvcNAQEEBQADgYEAy3XX5MH/8qBD\n"
        "jJ0upEOfrpFIuGq3/8gzjBPG0uQmcHq4re2sm8WlK6pXiwTXgc2qDTlv5ESpkGUC\n"
        "9/p41tRVd8VrQCR4VMvERrHl0oZAhZAI/NsVq84qPmi+F1/EstC8si3NTLcMxC34\n"
        "1gtBg8rkdGkL4VWxkRzJSjw+8FIUHfQ=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Certisign Autoridade Certificadora AC4.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICajCCAdOgAwIBAgIBADANBgkqhkiG9w0BAQQFADByMQswCQYDVQQGEwJCUjEu\n"
        "MCwGA1UEChMlQ2VydGlzaWduIENlcnRpZmljYWRvcmEgRGlnaXRhbCBMdGRhLjEz\n"
        "MDEGA1UECxMqQ2VydGlzaWduIC0gQXV0b3JpZGFkZSBDZXJ0aWZpY2Fkb3JhIC0g\n"
        "QUM0MB4XDTk5MDYyNzAwMDAwMFoXDTE4MDYyNzAwMDAwMFowcjELMAkGA1UEBhMC\n"
        "QlIxLjAsBgNVBAoTJUNlcnRpc2lnbiBDZXJ0aWZpY2Fkb3JhIERpZ2l0YWwgTHRk\n"
        "YS4xMzAxBgNVBAsTKkNlcnRpc2lnbiAtIEF1dG9yaWRhZGUgQ2VydGlmaWNhZG9y\n"
        "YSAtIEFDNDCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEA7IPUzIOQP1/svsHp\n"
        "0MdYRlkf8ZNKshcTC88pmasERTPj0V6+vI4ld2Ldm5KbMQpARXnV8rPdbLlA/Zh/\n"
        "QKIt6bKTvUvVwd7p0fpx9z9BvPQUr81ryDlKnWBltITQakHpY66baeq5RhzbGAg7\n"
        "YkzeQdeehn8TchHU3OreYKW73RsCAwEAAaMQMA4wDAYDVR0TBAUwAwEB/zANBgkq\n"
        "hkiG9w0BAQQFAAOBgQApXY3251O97KOBFkGxHvgkPaW3Dj7zf/uOyJ6jVucUQd8u\n"
        "Jd1O7RJA39lCGcQwmJSZUoojwGm1moJ5Q9yJBB8yv2uNvihzKUyj+1TNY/tfVGtS\n"
        "FRig6pXHDwmp74++q2q81qgGpOLSfePyThC5wDwuJaHyk7qhjbavh8aZ0z6hHw==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/CertPlus Class 1 Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDkjCCAnqgAwIBAgIRAIb+HV/DgfhH1zMsc5R1ezcwDQYJKoZIhvcNAQEFBQAw\n"
        "PTELMAkGA1UEBhMCRlIxETAPBgNVBAoTCENlcnRwbHVzMRswGQYDVQQDExJDbGFz\n"
        "cyAxIFByaW1hcnkgQ0EwHhcNOTkwNzA3MTcwMTAwWhcNMjAwNzA2MjM1OTU5WjA9\n"
        "MQswCQYDVQQGEwJGUjERMA8GA1UEChMIQ2VydHBsdXMxGzAZBgNVBAMTEkNsYXNz\n"
        "IDEgUHJpbWFyeSBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMNr\n"
        "Kcgux658UgXpk4lWClp5yczcJe+Hii0F2IEXBBs6Rfh5Dg1J1IFByKtF+3XIFfLi\n"
        "0LzZYWyEpBMZp9n58kncTA/GfVe0E9weCXEpNp2AWPQb2IkU69PSk0sq8XfnamW9\n"
        "GeWq/QxKY+HCmbQPBLZaGzY8Nzy/xeVe8V9XDh/H/Szn5G9qm2KWORGbZvAstlLc\n"
        "4Sf/223+66tjFB1jiGPiFgz5IFQPm90VanaDT70ni1OmN9KH8kdQUjtm6dF4Ghuy\n"
        "O2nwWTw4W9EqTDNeDspWpe0tW4KDTdY4TopWtmovJGsSMKYq3q8Mu6pEEhwTxWUP\n"
        "4/3YGjFL5D8MfEXoFjMCAwEAAaOBjDCBiTAPBgNVHRMECDAGAQH/AgEKMAsGA1Ud\n"
        "DwQEAwIBBjAdBgNVHQ4EFgQUIy7f6YG00IT9jrup3fkMo+PzSVMwEQYJYIZIAYb4\n"
        "QgEBBAQDAgEGMDcGA1UdHwQwMC4wLKAqoCiGJmh0dHA6Ly93d3cuY2VydHBsdXMu\n"
        "Y29tL0NSTC9jbGFzczEuY3JsMA0GCSqGSIb3DQEBBQUAA4IBAQC+JlPWXp+QPwGU\n"
        "AuwIwYia0Kf6UA3a/JQdqNRQI9CfHIthsbKqRA+s2BMN1dah6DF40O8eg0Ca5f1B\n"
        "6ZL97qOVkPPAuG9+9zSlod88895WssffhyZEx+KDuRNvMEVxKSIVq8TbP0/zd/mZ\n"
        "GDT1c5J9N2ZnSW4zmk0plIu9UDljZQ8Q53sQatFH+Z4lDCcpwtsIiOeqiTpFqp9J\n"
        "sn3qaIpA/pHBwkn/IdFnDhoSHUouHPuU+W07eTL5HimRCkKWRfHvylgPcp0MqYO8\n"
        "Mt2YBNFZ4WQXvcyBowNelmDlfxY4CmvR+rOEnSrKPvtC+E/op1soGRit4KZmHrAp\n"
        "IfK4mY//\n"
        "-----END CERTIFICATE-----\n",

        // ./root/CertPlus Class 2 Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDkjCCAnqgAwIBAgIRAIW9S/PY2uNp9pTXX8OlRCMwDQYJKoZIhvcNAQEFBQAw\n"
        "PTELMAkGA1UEBhMCRlIxETAPBgNVBAoTCENlcnRwbHVzMRswGQYDVQQDExJDbGFz\n"
        "cyAyIFByaW1hcnkgQ0EwHhcNOTkwNzA3MTcwNTAwWhcNMTkwNzA2MjM1OTU5WjA9\n"
        "MQswCQYDVQQGEwJGUjERMA8GA1UEChMIQ2VydHBsdXMxGzAZBgNVBAMTEkNsYXNz\n"
        "IDIgUHJpbWFyeSBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANxQ\n"
        "ltAS+DXSCHh6tlJw/W/uz7kRy1134ezpfgSN1sxvc0NXYKwzCkTsA18cgCSR5aiR\n"
        "VhKC9+Ar9NuuYS6JEI1rbLqzAr3VNsVINyPi8Fo3UjMXEuLRYE2+L0ER4/YXJQyL\n"
        "kcAbmXuZVg2v7tK8R1fjeUl7NIknJITesezpWE7+Tt9avkGtrAjFGA7v0lPubNCd\n"
        "EgETjdyAYveVqUSISnFOYFWe2yMZeVYHDD9jC1yw4r5+FfyUM1hBOHTE4Y+L3yas\n"
        "H7WLO7dDWWuwJKZtkIvEcupdM5i3y95ee++U8Rs+yskhwcWYAqqi9lt3m/V+llU0\n"
        "HGdpwPFC40es/CgcZlUCAwEAAaOBjDCBiTAPBgNVHRMECDAGAQH/AgEKMAsGA1Ud\n"
        "DwQEAwIBBjAdBgNVHQ4EFgQU43Mt38sOKAze3bOkynm4jrvoMIkwEQYJYIZIAYb4\n"
        "QgEBBAQDAgEGMDcGA1UdHwQwMC4wLKAqoCiGJmh0dHA6Ly93d3cuY2VydHBsdXMu\n"
        "Y29tL0NSTC9jbGFzczIuY3JsMA0GCSqGSIb3DQEBBQUAA4IBAQCnVM+IRBnL39R/\n"
        "AN9WM2K191EBkOvDP9GIROkkXe/nFL0gt5o8AP5tn9uQ3Nf0YtaLcF3n5QRIqWh8\n"
        "yfFC82x/xXp8HVGIutIKPidd3i1RTtMTZGnkLuPT55sJmabglZvOGtd/vjzOUrMR\n"
        "FcEPF80Du5wlFbqidon8BvEY0JNLDnyCt6X09l/+7UCmnYR0ObncHoUW2ikbhiMA\n"
        "ybuJfm6AiB4vFLQDJKgybwOaRywwvlbGp0ICcBvqQNi6BQNwB6SW//1IMwrh3KWB\n"
        "kJtN3X3n57LNXMhqlfil9o3EXXgIvnsG1knPGTZQIy4I5p4FTUcY1Rbpsda2ENW7\n"
        "l7+ijrRU\n"
        "-----END CERTIFICATE-----\n",

        // ./root/CertPlus Class 3 Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDkTCCAnmgAwIBAgIQWd+sfopmFRHliCTO76m8qTANBgkqhkiG9w0BAQUFADA9\n"
        "MQswCQYDVQQGEwJGUjERMA8GA1UEChMIQ2VydHBsdXMxGzAZBgNVBAMTEkNsYXNz\n"
        "IDMgUHJpbWFyeSBDQTAeFw05OTA3MDcxNzA4MDBaFw0xOTA3MDYyMzU5NTlaMD0x\n"
        "CzAJBgNVBAYTAkZSMREwDwYDVQQKEwhDZXJ0cGx1czEbMBkGA1UEAxMSQ2xhc3Mg\n"
        "MyBQcmltYXJ5IENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAt5Qw\n"
        "btBM2X5eYOVvuybKUm9rbIWZpQvvABpvG01YtRgH+t17+MssUt38nLiNUum5sgt8\n"
        "9Y9bmUgJWN7NSJWGJYkFNCcwC1e2DHdjKctsqj65mVESDZhwdkdM+UmWIgj5a+qq\n"
        "ADajFaccHp+Mylp5eyHaiR9jfnmSUAkEKK3O420ESUqZsT9FCXhYIO+N/oDIBO0p\n"
        "LKBYjYQCJZc/oBPXe4sj45+4x7hCQDgbkkq9SpRVx1YVDYF3zJ+iN4krW4UNi3f4\n"
        "xIv7EMuUx+kaVhKXZhTEu9d9bQIbv3FiJhjpSYr6o97hhK2AykriIoxqCGGDsiLH\n"
        "CYg4Vl3RMavwCZ8TWQIDAQABo4GMMIGJMA8GA1UdEwQIMAYBAf8CAQowCwYDVR0P\n"
        "BAQDAgEGMB0GA1UdDgQWBBRqZDZ2UJxO9PdPFfUN7TMlYOzLIDARBglghkgBhvhC\n"
        "AQEEBAMCAQYwNwYDVR0fBDAwLjAsoCqgKIYmaHR0cDovL3d3dy5jZXJ0cGx1cy5j\n"
        "b20vQ1JML2NsYXNzMy5jcmwwDQYJKoZIhvcNAQEFBQADggEBAJqhPjiu4VLSRQ3R\n"
        "6YNjrYyA9JPDuMI6IpcnslGGPtLyl958u9eWQFPKUqqKbGPMunGK4vDwEIoXsE+P\n"
        "xu8OMTBaja0Ex6uMXxoIz5oXmUa+jmtKan8BR2klGPhOPEoEKwbyEAA9/z6l6A7P\n"
        "cDiGu8WODNbiS0tm83NfgLU78RIp6nO8aJ4cYPfzqkLYFsho3nMLP0DBd3dUHZRX\n"
        "KQGTW5jDawT6fCmNrEfw/lUVS69xI/XDzgFnkyaArPSCisQbQErcxjkKbmO2ZE16\n"
        "zpCqmb02/Vn8L/xFvt6OM1n6igsPMDZN41u11pZQ17/+hdzatIIrzBsIP/GaTrXh\n"
        "jwhhJBY=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/CertPlus Class 3P Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDlTCCAn2gAwIBAgIRAL9c27byHG7ATet6Ajs26HkwDQYJKoZIhvcNAQEFBQAw\n"
        "PjELMAkGA1UEBhMCRlIxETAPBgNVBAoTCENlcnRwbHVzMRwwGgYDVQQDExNDbGFz\n"
        "cyAzUCBQcmltYXJ5IENBMB4XDTk5MDcwNzE3MTAwMFoXDTE5MDcwNjIzNTk1OVow\n"
        "PjELMAkGA1UEBhMCRlIxETAPBgNVBAoTCENlcnRwbHVzMRwwGgYDVQQDExNDbGFz\n"
        "cyAzUCBQcmltYXJ5IENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA\n"
        "qzf/62CbQXhp9UlYsN4fcWmmK+OuUMapvJPpIL7kxBOCVu/wQzIJypt1A498T+Hg\n"
        "T3aeC61kehQ6mp2/LxYLZRyp7py84xply0+F6pJWdWbWVUDv+8zWOD+rHO9CjRmJ\n"
        "9reVhsKnHen3KfEq2WV5/Cv1jsoad36e6Kz5Zr9F++gTnV+2c+V9e477EnRdHwZe\n"
        "hRumXhhEALq8027RUg4GrevutbTBu7zrOA9IIpHHb9K4cju6f8CNbLe8R3MhKoX/\n"
        "rNYoohnVl2o6uaxtRezmTcPbqF3FXYKYrEpaquYrCAwQdLxi9jpJBGbYURwmpth1\n"
        "n5y/rmBRPVy8ok97iWfNUwIDAQABo4GNMIGKMA8GA1UdEwQIMAYBAf8CAQowCwYD\n"
        "VR0PBAQDAgEGMB0GA1UdDgQWBBSG4eGBcb9qEvEK8gHkyPtAzmiAiTARBglghkgB\n"
        "hvhCAQEEBAMCAAEwOAYDVR0fBDEwLzAtoCugKYYnaHR0cDovL3d3dy5jZXJ0cGx1\n"
        "cy5jb20vQ1JML2NsYXNzM1AuY3JsMA0GCSqGSIb3DQEBBQUAA4IBAQAlquEiQMKk\n"
        "gDy3ol2ZjR96QjU4ZhcR372v/BURmBkz5gVChFSoS+uwnds32hZSQBF0aL/pybIQ\n"
        "hLcdRAB5Jxz1WAYXGDI1tjCXY8amORvI7kYXYsUu5wqjmoowY3OqFKVNCqhyk/BJ\n"
        "ERCQfBh9qCAFxMJ6NbocWgrgLnjIiLHPVwHsPeIGEzTAqNz6gIAF7gV2vZ0ryJ1Q\n"
        "b2vFQFCE/V0d5pCcENOkxrkoGt61+Apwqs7eUD0DgNvYiMVIBuQDc90WzjbW5Zvq\n"
        "d9qylrVlpwRdI673k7JeilFkX9rPjD1BW975o+kqfEcQH/YyPH5w6d+h1S4NsRpF\n"
        "tLwS7SgX6R4C\n"
        "-----END CERTIFICATE-----\n",

        // ./root/CertPlus Class 3TS Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDgzCCAmugAwIBAgIRAK8/ZGtWAoZmxthD+Ut9ls8wDQYJKoZIhvcNAQEFBQAw\n"
        "PzELMAkGA1UEBhMCRlIxETAPBgNVBAoTCENlcnRwbHVzMR0wGwYDVQQDExRDbGFz\n"
        "cyAzVFMgUHJpbWFyeSBDQTAeFw05OTA3MDcxNzE0MDBaFw0xOTA3MDYyMzU5NTla\n"
        "MD8xCzAJBgNVBAYTAkZSMREwDwYDVQQKEwhDZXJ0cGx1czEdMBsGA1UEAxMUQ2xh\n"
        "c3MgM1RTIFByaW1hcnkgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\n"
        "AQC9ZZojQwA8+SUBQ5hbRjQ3Zzf0W1eOo97NjC9BsxU95ZiL8sxcp8evNcezp0/H\n"
        "EClX6QOJg3IGdlLBcAI3QO3wzouxsYmsAqhb3dzVQ0UG3Ci26VEbau7ze0jFnRGu\n"
        "jaZyk8AlQpGoPkbiaEyBQLBC03q+e0fSR1AMpBKmyuQS0MB8OjwBqcoClt6CWsX7\n"
        "Gbo8nNgPdB+YGK6UUDIHUhARSkvsWFUs1J5LlHsF0nLfBvCNywie5O60RK2C8Rth\n"
        "avGz+BYxNkIkupC/tX5hbsJZDFR5tEyIKB7MG9+EiyPIvq8H8pGy2o3KjF98KDx8\n"
        "iWwGWb0+zsWC02rJrGhV7sWvAgMBAAGjejB4MA8GA1UdEwQIMAYBAf8CAQowCwYD\n"
        "VR0PBAQDAgEGMB0GA1UdDgQWBBSidg45eGuPui1xSgj3ontuAKJkJTA5BgNVHR8E\n"
        "MjAwMC6gLKAqhihodHRwOi8vd3d3LmNlcnRwbHVzLmNvbS9DUkwvY2xhc3MzVFMu\n"
        "Y3JsMA0GCSqGSIb3DQEBBQUAA4IBAQC1n2icxJLwd3wTwhW8HNQObnQoofqG0ggb\n"
        "Jfzke9l90wJYZ28EblDue0oBAmO/QjbWvgz+IgyZPY7zrh2U1qrqpj3jQ55HuwnJ\n"
        "y+7OoSwPqnzffHOS5Uu5BMQ72yD+YPkMbngaPUjXJ0iYtZGspNwis+edX+zlQGIm\n"
        "j3oXxur7CtZDW5DbNhbtmC6eX6wUB4IqUL+mv2+mBzRytmFo7FGYGVN36Mi/9iF7\n"
        "vLDef3Q6+RU0OJr7iTPwEg2f6PGWbfYdSH4+Eu/6BmdZ3rOy1pcLO0BXQrLBe8Zt\n"
        "2CZSNCXShbahCgrdqpSqTnsgk92kgENWz7bE4FjpbJWPYm5YBq94\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Certum.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDDDCCAfSgAwIBAgIDAQAgMA0GCSqGSIb3DQEBBQUAMD4xCzAJBgNVBAYTAlBM\n"
        "MRswGQYDVQQKExJVbml6ZXRvIFNwLiB6IG8uby4xEjAQBgNVBAMTCUNlcnR1bSBD\n"
        "QTAeFw0wMjA2MTExMDQ2MzlaFw0yNzA2MTExMDQ2MzlaMD4xCzAJBgNVBAYTAlBM\n"
        "MRswGQYDVQQKExJVbml6ZXRvIFNwLiB6IG8uby4xEjAQBgNVBAMTCUNlcnR1bSBD\n"
        "QTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAM6xwS7TT3zNJc4YPk/E\n"
        "jG+AanPIW1H4m9LcuwBcsaD8dQPugfCI7iNS6eYVM42sLQnFdvkrOYCJ5JdLkKWo\n"
        "ePhzQ3ukYbDYWMzhbGZ+nPMJXlVjhNWo7/OxLjBos8Q82KxujZlakE403Daaj4GI\n"
        "ULdtlkIJ89eVgw1BS7Bqa/j8D35in2fE7SZfECYPCE/wpFcozo+47UX2bu4lXapu\n"
        "Ob7kky/ZR6By6/qmW6/KUz/iDsaWVhFu9+lmqSbYf5VT7QqFiLpPKaVCjF62/IUg\n"
        "AKpoC6EahQGcxEZjgoi2IrHu/qpGWX7PNSzVttpd90gzFFS269lvzs2I1qsb2pY7\n"
        "HVkCAwEAAaMTMBEwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQUFAAOCAQEA\n"
        "uI3O7+cUus/usESSbLQ5PqKEbq24IXfS1HeCh+YgQYHu4vgRt2PRFze+GXYkHAQa\n"
        "TOs9qmdvLdTN/mUxcMUbpgIKumB7bVjCmkn+YzILa+M6wKyrO7Do0wlRjBCDxjTg\n"
        "xSvgGrZgFCdsMneMvLJymM/NzD+5yCRCFNZX/OYmQ6kd5YCQzgNUKD73P9P4Te1q\n"
        "CjqTE5s7FCMTY5w/0YcneeVMUeMBrYVdGjux1XMQpNPyvG5k9VpWkKjHDkx0Dy5x\n"
        "O/fIR/RpbxXyEV6DHpx8Uq79AtoSqFlnGNu8cN2bsWntgM6JQEhqDjXKKWYVIZQs\n"
        "6GAqm4VKQPNriiTsBhYscw==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/CNNIC Root.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDVTCCAj2gAwIBAgIESTMAATANBgkqhkiG9w0BAQUFADAyMQswCQYDVQQGEwJD\n"
        "TjEOMAwGA1UEChMFQ05OSUMxEzARBgNVBAMTCkNOTklDIFJPT1QwHhcNMDcwNDE2\n"
        "MDcwOTE0WhcNMjcwNDE2MDcwOTE0WjAyMQswCQYDVQQGEwJDTjEOMAwGA1UEChMF\n"
        "Q05OSUMxEzARBgNVBAMTCkNOTklDIFJPT1QwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n"
        "DwAwggEKAoIBAQDTNfc/c3et6FtzF8LRb+1VvG7q6KR5smzDo+/hn7E7SIX1mlwh\n"
        "IhAsxYLO2uOabjfhhyzcuQxauohV3/2q2x8x6gHx3zkBwRP9SFIhxFXf2tizVHa6\n"
        "dLG3fdfA6PZZxU3Iva0fFNrfWEQlMhkqx35+jq44sDB7R3IJMfAw28Mbdim7aXZO\n"
        "V/kbZKKTVrdvmW7bCgScEeOAH8tjlBAKqeFkgjH5jCftppkA9nCTGPihNIaj3XrC\n"
        "GHn2emU1z5DrvTOTn1OrczvmmzQgLx3vqR1jGqCA2wMv+SYahtKNu6m+UjqHZ0gN\n"
        "v7Sg2Ca+I19zN38m5pIEo3/PIKe38zrKy5nLAgMBAAGjczBxMBEGCWCGSAGG+EIB\n"
        "AQQEAwIABzAfBgNVHSMEGDAWgBRl8jGtKvf33VKWCscCwQ7vptU7ETAPBgNVHRMB\n"
        "Af8EBTADAQH/MAsGA1UdDwQEAwIB/jAdBgNVHQ4EFgQUZfIxrSr3991SlgrHAsEO\n"
        "76bVOxEwDQYJKoZIhvcNAQEFBQADggEBAEs17szkrr/Dbq2flTtLP1se31cpolnK\n"
        "OOK5Gv+e5m4y3R6u6jW39ZORTtpC4cMXYFDy0VwmuYK36m3knITnA3kXr5g9lNvH\n"
        "ugDnuL8BV8F3RTIMO/G0HAiw/VGgod2aHRM2mm23xzy54cXZF/qD1T0VoDy7Hgvi\n"
        "yJA/qIYM/PmLXoXLT1tLYhFHxUV8BS9BsZ4QaRuZluBVeftOhpm4lNqGOGqTo+fL\n"
        "buXf6iFViZx9fX+Y9QCJ7uOEwFyWtcVG6kbghVW2G8kS1sHNzYDzAgE8yGnLRUhj\n"
        "2JTQ7IUOO04RZfSCjKY9ri4ilAnIXOo8gV0WKgOXFlUJ24pBgp5mmxE=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Deutsche Telekom Root CA 1.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICmjCCAgOgAwIBAgIBJDANBgkqhkiG9w0BAQQFADBxMQswCQYDVQQGEwJERTEc\n"
        "MBoGA1UEChMTRGV1dHNjaGUgVGVsZWtvbSBBRzEfMB0GA1UECxMWVC1UZWxlU2Vj\n"
        "IFRydXN0IENlbnRlcjEjMCEGA1UEAxMaRGV1dHNjaGUgVGVsZWtvbSBSb290IENB\n"
        "IDEwHhcNOTkwNzA5MTEzNDAwWhcNMTkwNzA5MjM1OTAwWjBxMQswCQYDVQQGEwJE\n"
        "RTEcMBoGA1UEChMTRGV1dHNjaGUgVGVsZWtvbSBBRzEfMB0GA1UECxMWVC1UZWxl\n"
        "U2VjIFRydXN0IENlbnRlcjEjMCEGA1UEAxMaRGV1dHNjaGUgVGVsZWtvbSBSb290\n"
        "IENBIDEwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBANDdmwygF0RED68hQHNn\n"
        "VvA+aWgRutk38oGuwySsaaHN/KYYVVb/i58ywdvneCw522CBQaXv082AjRg84lIM\n"
        "C5/3ZJ7loPC4YWL0v+Cj2lgr/RUEa706nnyd8j3Y5JXD7E7C8WWrDEvsR4Jb4uFQ\n"
        "ddj2YbQYXO0zoEseg/tPhLx5AgMBAAGjQjBAMB0GA1UdDgQWBBQUMeJ/nMoSlfvx\n"
        "cCDbTSgTcUJhxjAPBgNVHRMECDAGAQH/AgEFMA4GA1UdDwEB/wQEAwIBBjANBgkq\n"
        "hkiG9w0BAQQFAAOBgQCdHRD6k8gdZCBFXpxvBW2jUn+nqIdls2eaNoZ4chYV/9dF\n"
        "cygBhoidkere1ikLDD6jmXRADMzsEOBk/HC2ujkSJ/JeAFC1C9ZK36lv9LMJKID8\n"
        "0h7E7HBGhUzeRbABlTisFp9GSu4ty73LZbE65aZLBEsOM/8ce8iEhEfhWmRGSg==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Deutsche Telekom Root CA 2.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDnzCCAoegAwIBAgIBJjANBgkqhkiG9w0BAQUFADBxMQswCQYDVQQGEwJERTEc\n"
        "MBoGA1UEChMTRGV1dHNjaGUgVGVsZWtvbSBBRzEfMB0GA1UECxMWVC1UZWxlU2Vj\n"
        "IFRydXN0IENlbnRlcjEjMCEGA1UEAxMaRGV1dHNjaGUgVGVsZWtvbSBSb290IENB\n"
        "IDIwHhcNOTkwNzA5MTIxMTAwWhcNMTkwNzA5MjM1OTAwWjBxMQswCQYDVQQGEwJE\n"
        "RTEcMBoGA1UEChMTRGV1dHNjaGUgVGVsZWtvbSBBRzEfMB0GA1UECxMWVC1UZWxl\n"
        "U2VjIFRydXN0IENlbnRlcjEjMCEGA1UEAxMaRGV1dHNjaGUgVGVsZWtvbSBSb290\n"
        "IENBIDIwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCrC6M14IspFLEU\n"
        "ha88EOQ5bzVdSq7d6mGNlUn0b2SjGmBmpKlAIoTZ1KXleJMOaAGtuU1cOs7TuKhC\n"
        "QN/Po7qCWWqSG6wcmtoIKyUn+WkjR/Hg6yx6m/UTAtB+NHzCnjwAWav12gz1Mjwr\n"
        "rFDa1sPeg5TKqAyZMg4ISFZbavva4VhYAUlfckE8FQYBjl2tqriTtM2e66foai1S\n"
        "NNs671x1Udrb8zH57nGYMsRUFUQM+ZtV7a3fGAigo4aKSe5TBY8ZTNXeWHmb0moc\n"
        "QqvF1afPaA+W5OFhmHZhyJF81j4A4pFQh+GdCuatl9Idxjp9y7zaAzTVjlsB9WoH\n"
        "txa2bkp/AgMBAAGjQjBAMB0GA1UdDgQWBBQxw3kbuvVT1xfgiXotF2wKsyudMzAP\n"
        "BgNVHRMECDAGAQH/AgEFMA4GA1UdDwEB/wQEAwIBBjANBgkqhkiG9w0BAQUFAAOC\n"
        "AQEAlGRZrTlk5ynrE/5aw4sTV8gEJPB0d8Bg42f76Ymmg7+Wgnxu1MM9756Abrsp\n"
        "tJh6sTtU6zkXR34ajgv8HzFZMQSyzhfzLMdiNlXiItiJVbSYSKpk+tYcNthEeFpa\n"
        "IzpXl/V6ME+un2pMSyuOoAPjPuCp1NJ70rOo4nI8rZ7/gFnkm0W09juwzTkZmDLl\n"
        "6iFhkOQxIY40sfcvNUqFENrnijchvllj4PKFiDFT1FQUhXB59C4Gdyd1Lx+4ivn+\n"
        "xbrYNuSD7Odlt79jWvNGr4GUN9RBjNYj1h7P9WgbRGOiWrqnNVmh5XAFmw4jV5mU\n"
        "Cm26OWMohpLzGITY+9HPBVZkVw==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/DigiCert (6).cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\n"
        "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
        "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\n"
        "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\n"
        "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\n"
        "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\n"
        "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\n"
        "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\n"
        "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\n"
        "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\n"
        "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\n"
        "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\n"
        "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\n"
        "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\n"
        "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\n"
        "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\n"
        "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\n"
        "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\n"
        "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\n"
        "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\n"
        "+OkuE6N36B9K\n"
        "-----END CERTIFICATE-----\n",

        // ./root/DigiCert.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
        "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
        "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
        "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
        "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
        "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
        "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
        "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
        "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
        "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
        "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
        "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
        "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
        "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
        "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
        "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
        "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
        "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
        "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
        "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/DST (ANX Network) CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDTTCCAragAwIBAgIENm6ibzANBgkqhkiG9w0BAQUFADBSMQswCQYDVQQGEwJV\n"
        "UzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMR0wGwYDVQQL\n"
        "ExREU1QgKEFOWCBOZXR3b3JrKSBDQTAeFw05ODEyMDkxNTQ2NDhaFw0xODEyMDkx\n"
        "NjE2NDhaMFIxCzAJBgNVBAYTAlVTMSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVy\n"
        "ZSBUcnVzdCBDby4xHTAbBgNVBAsTFERTVCAoQU5YIE5ldHdvcmspIENBMIGdMA0G\n"
        "CSqGSIb3DQEBAQUAA4GLADCBhwKBgQC0SBGAWKDVpZkP9jcsRLZu0XzzKmueEbaI\n"
        "IwRccSWeahJ3EW6/aDllqPay9qIYsokVoGe3eowiSGv2hDQftsr3G3LL8ltI04ce\n"
        "InYTBLSsbJZ/5w4IyTJRMC3VgOghZ7rzXggkLAdZnZAa7kbJtaQelrRBkdR/0o04\n"
        "JrBvQ24JfQIBA6OCATAwggEsMBEGCWCGSAGG+EIBAQQEAwIABzB0BgNVHR8EbTBr\n"
        "MGmgZ6BlpGMwYTELMAkGA1UEBhMCVVMxJDAiBgNVBAoTG0RpZ2l0YWwgU2lnbmF0\n"
        "dXJlIFRydXN0IENvLjEdMBsGA1UECxMURFNUIChBTlggTmV0d29yaykgQ0ExDTAL\n"
        "BgNVBAMTBENSTDEwKwYDVR0QBCQwIoAPMTk5ODEyMDkxNTQ2NDhagQ8yMDE4MTIw\n"
        "OTE1NDY0OFowCwYDVR0PBAQDAgEGMB8GA1UdIwQYMBaAFIwWVXDMFgpTZMKlhKqz\n"
        "ZBdDP4I2MB0GA1UdDgQWBBSMFlVwzBYKU2TCpYSqs2QXQz+CNjAMBgNVHRMEBTAD\n"
        "AQH/MBkGCSqGSIb2fQdBAAQMMAobBFY0LjADAgSQMA0GCSqGSIb3DQEBBQUAA4GB\n"
        "AEklyWCxDF+pORDTxTRVfc95wynr3vnCQPnoVsXwL+z02exIUbhjOF6TbhiWhbnK\n"
        "UJykuOpmJmiThW9vTHHQvnoLPDG5975pnhDX0UDorBZxq66rOOFwscqSFuBdhaYY\n"
        "gAYAnOGmGEJRp2hoWe8mlF+tMQz+KR4XAYQ3W+gSMqNd\n"
        "-----END CERTIFICATE-----\n",

        // ./root/DST-Entrust GTI CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDRzCCArCgAwIBAgIENm3FGDANBgkqhkiG9w0BAQUFADBQMQswCQYDVQQGEwJV\n"
        "UzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRswGQYDVQQL\n"
        "ExJEU1QtRW50cnVzdCBHVEkgQ0EwHhcNOTgxMjA5MDAwMjI0WhcNMTgxMjA5MDAz\n"
        "MjI0WjBQMQswCQYDVQQGEwJVUzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUg\n"
        "VHJ1c3QgQ28uMRswGQYDVQQLExJEU1QtRW50cnVzdCBHVEkgQ0EwgZ0wDQYJKoZI\n"
        "hvcNAQEBBQADgYsAMIGHAoGBALYd90uNDxPjEvUJ/gYyDq9MQfV91Ec9KgrfgwXe\n"
        "3n3mAxb2UTrLRxpKrX7E/R20vnSKeN0Lg460hBPE+/htKa6h4Q8PQ+O1XmBp+oOU\n"
        "/Hnm3Hbt0UQrjv0Su/4XdxcMie2n71F9xO04wzujevviTaBgtfL9E2XTxuw/vjWc\n"
        "PSLvAgEDo4IBLjCCASowEQYJYIZIAYb4QgEBBAQDAgAHMHIGA1UdHwRrMGkwZ6Bl\n"
        "oGOkYTBfMQswCQYDVQQGEwJVUzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUg\n"
        "VHJ1c3QgQ28uMRswGQYDVQQLExJEU1QtRW50cnVzdCBHVEkgQ0ExDTALBgNVBAMT\n"
        "BENSTDEwKwYDVR0QBCQwIoAPMTk5ODEyMDkwMDAyMjRagQ8yMDE4MTIwOTAwMDIy\n"
        "NFowCwYDVR0PBAQDAgEGMB8GA1UdIwQYMBaAFJOaRMrQeFOAKUkE38evMz+ZdV+u\n"
        "MB0GA1UdDgQWBBSTmkTK0HhTgClJBN/HrzM/mXVfrjAMBgNVHRMEBTADAQH/MBkG\n"
        "CSqGSIb2fQdBAAQMMAobBFY0LjADAgSQMA0GCSqGSIb3DQEBBQUAA4GBAGSJzAOn\n"
        "3AryWCDn/RegKHLNh7DNmLUkR2MzMRAQsu+KV3KuTAPgZ5+sYEOEIsGpo+Wxp94J\n"
        "1M8NeEYjW49Je/4TIpeU6nJI4SwgeJbpZkUZywllY2E/0UmYsXYQVdVjSmZLpAdr\n"
        "3nt/ueaTWxoCW4AO3Y0Y1Iqjwmjxo+AY0U5M\n"
        "-----END CERTIFICATE-----\n",

        // ./root/DSTCA E1.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDKTCCApKgAwIBAgIENnAVljANBgkqhkiG9w0BAQUFADBGMQswCQYDVQQGEwJV\n"
        "UzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMREwDwYDVQQL\n"
        "EwhEU1RDQSBFMTAeFw05ODEyMTAxODEwMjNaFw0xODEyMTAxODQwMjNaMEYxCzAJ\n"
        "BgNVBAYTAlVTMSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4x\n"
        "ETAPBgNVBAsTCERTVENBIEUxMIGdMA0GCSqGSIb3DQEBAQUAA4GLADCBhwKBgQCg\n"
        "bIGpzzQeJN3+hijM3oMv+V7UQtLodGBmE5gGHKlREmlvMVW5SXIACH7TpWJENySZ\n"
        "j9mDSI+ZbZUTu0M7LklOiDfBu1h//uG9+LthzfNHwJmm8fOR6Hh8AMthyUQncWlV\n"
        "Sn5JTe2io74CTADKAqjuAQIxZA9SLRN0dja1erQtcQIBA6OCASQwggEgMBEGCWCG\n"
        "SAGG+EIBAQQEAwIABzBoBgNVHR8EYTBfMF2gW6BZpFcwVTELMAkGA1UEBhMCVVMx\n"
        "JDAiBgNVBAoTG0RpZ2l0YWwgU2lnbmF0dXJlIFRydXN0IENvLjERMA8GA1UECxMI\n"
        "RFNUQ0EgRTExDTALBgNVBAMTBENSTDEwKwYDVR0QBCQwIoAPMTk5ODEyMTAxODEw\n"
        "MjNagQ8yMDE4MTIxMDE4MTAyM1owCwYDVR0PBAQDAgEGMB8GA1UdIwQYMBaAFGp5\n"
        "fpFpRhgTCgJ3pVlbYJglDqL4MB0GA1UdDgQWBBRqeX6RaUYYEwoCd6VZW2CYJQ6i\n"
        "+DAMBgNVHRMEBTADAQH/MBkGCSqGSIb2fQdBAAQMMAobBFY0LjADAgSQMA0GCSqG\n"
        "SIb3DQEBBQUAA4GBACIS2Hod3IEGtgllsofIH160L+nEHvI8wbsEkBFKg05+k7lN\n"
        "QseSJqBcNJo4cvj9axY+IO6CizEqkzaFI4iKPANo08kJD038bKTaKHKTDomAsH3+\n"
        "gG9lbRgzl4vCa4nuYD3Im+9/KzJic5PLPON74nZ4RbyhkwS7hp86W0N6w4pl\n"
        "-----END CERTIFICATE-----\n",

        // ./root/DSTCA E2.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDKTCCApKgAwIBAgIENm7TzjANBgkqhkiG9w0BAQUFADBGMQswCQYDVQQGEwJV\n"
        "UzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMREwDwYDVQQL\n"
        "EwhEU1RDQSBFMjAeFw05ODEyMDkxOTE3MjZaFw0xODEyMDkxOTQ3MjZaMEYxCzAJ\n"
        "BgNVBAYTAlVTMSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4x\n"
        "ETAPBgNVBAsTCERTVENBIEUyMIGdMA0GCSqGSIb3DQEBAQUAA4GLADCBhwKBgQC/\n"
        "k48Xku8zExjrEH9OFr//Bo8qhbxe+SSmJIi2A7fBw18DW9Fvrn5C6mYjuGODVvso\n"
        "LeE4i7TuqAHhzhy2iCoiRoX7n6dwqUcUP87eZfCocfdPJmyMvMa1795JJ/9IKn3o\n"
        "TQPMx7JSxhcxEzu1TdvIxPbDDyQq2gyd55FbgM2UnQIBA6OCASQwggEgMBEGCWCG\n"
        "SAGG+EIBAQQEAwIABzBoBgNVHR8EYTBfMF2gW6BZpFcwVTELMAkGA1UEBhMCVVMx\n"
        "JDAiBgNVBAoTG0RpZ2l0YWwgU2lnbmF0dXJlIFRydXN0IENvLjERMA8GA1UECxMI\n"
        "RFNUQ0EgRTIxDTALBgNVBAMTBENSTDEwKwYDVR0QBCQwIoAPMTk5ODEyMDkxOTE3\n"
        "MjZagQ8yMDE4MTIwOTE5MTcyNlowCwYDVR0PBAQDAgEGMB8GA1UdIwQYMBaAFB6C\n"
        "TShlgDzJQW6sNS5ay97u+DlbMB0GA1UdDgQWBBQegk0oZYA8yUFurDUuWsve7vg5\n"
        "WzAMBgNVHRMEBTADAQH/MBkGCSqGSIb2fQdBAAQMMAobBFY0LjADAgSQMA0GCSqG\n"
        "SIb3DQEBBQUAA4GBAEeNg61i8tuwnkUiBbmi1gMOOHLnnvx75pO2mqWilMg0HZHR\n"
        "xdf0CiUPPXiBng+xZ8SQTGPdXqfiup/1902lMXucKS1M/mQ+7LZT/uqb7YLbdHVL\n"
        "B3luHtgZg3Pe9T7Qtd7nS2h9Qy4qIOF+oHhEngj1mPnHfxsb1gYgAlihw6ID\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Entrust (2048).cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEKjCCAxKgAwIBAgIEOGPe+DANBgkqhkiG9w0BAQUFADCBtDEUMBIGA1UEChML\n"
        "RW50cnVzdC5uZXQxQDA+BgNVBAsUN3d3dy5lbnRydXN0Lm5ldC9DUFNfMjA0OCBp\n"
        "bmNvcnAuIGJ5IHJlZi4gKGxpbWl0cyBsaWFiLikxJTAjBgNVBAsTHChjKSAxOTk5\n"
        "IEVudHJ1c3QubmV0IExpbWl0ZWQxMzAxBgNVBAMTKkVudHJ1c3QubmV0IENlcnRp\n"
        "ZmljYXRpb24gQXV0aG9yaXR5ICgyMDQ4KTAeFw05OTEyMjQxNzUwNTFaFw0yOTA3\n"
        "MjQxNDE1MTJaMIG0MRQwEgYDVQQKEwtFbnRydXN0Lm5ldDFAMD4GA1UECxQ3d3d3\n"
        "LmVudHJ1c3QubmV0L0NQU18yMDQ4IGluY29ycC4gYnkgcmVmLiAobGltaXRzIGxp\n"
        "YWIuKTElMCMGA1UECxMcKGMpIDE5OTkgRW50cnVzdC5uZXQgTGltaXRlZDEzMDEG\n"
        "A1UEAxMqRW50cnVzdC5uZXQgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkgKDIwNDgp\n"
        "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArU1LqRKGsuqjIAcVFmQq\n"
        "K0vRvwtKTY7tgHalZ7d4QMBzQshowNtTK91euHaYNZOLGp18EzoOH1u3Hs/lJBQe\n"
        "sYGpjX24zGtLA/ECDNyrpUAkAH90lKGdCCmziAv1h3edVc3kw37XamSrhRSGlVuX\n"
        "MlBvPci6Zgzj/L24ScF2iUkZ/cCovYmjZy/Gn7xxGWC4LeksyZB2ZnuU4q941mVT\n"
        "XTzWnLLPKQP5L6RQstRIzgUyVYr9smRMDuSYB3Xbf9+5CFVghTAp+XtIpGmG4zU/\n"
        "HoZdenoVve8AjhUiVBcAkCaTvA5JaJG/+EfTnZVCwQ5N328mz8MYIWJmQ3DW1cAH\n"
        "4QIDAQABo0IwQDAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNV\n"
        "HQ4EFgQUVeSB0RGAvtiJuQijMfmhJAkWuXAwDQYJKoZIhvcNAQEFBQADggEBADub\n"
        "j1abMOdTmXx6eadNl9cZlZD7Bh/KM3xGY4+WZiT6QBshJ8rmcnPyT/4xmf3IDExo\n"
        "U8aAghOY+rat2l098c5u9hURlIIM7j+VrxGrD9cv3h8Dj1csHsm7mhpElesYT6Yf\n"
        "zX1XEC+bBAlahLVu2B064dae0Wx5XnkcFMXj0EyTO2U87d89vqbllRrDtRnDvV5b\n"
        "u/8j72gZyxKTJ1wDLW8w0B62GqzeWvfRqqgnpv55gcR5mTNXuhKwqeBCbJPKVt7+\n"
        "bYQLCIt+jerXmCHG8+c8eS9enNFMFY3h7CI3zJpDC5fcgJCNs2ebb0gIFVbPv/Er\n"
        "fF6adulZkMV8gzURZVE=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Entrust.net Secure Server Certification Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIE2DCCBEGgAwIBAgIEN0rSQzANBgkqhkiG9w0BAQUFADCBwzELMAkGA1UEBhMC\n"
        "VVMxFDASBgNVBAoTC0VudHJ1c3QubmV0MTswOQYDVQQLEzJ3d3cuZW50cnVzdC5u\n"
        "ZXQvQ1BTIGluY29ycC4gYnkgcmVmLiAobGltaXRzIGxpYWIuKTElMCMGA1UECxMc\n"
        "KGMpIDE5OTkgRW50cnVzdC5uZXQgTGltaXRlZDE6MDgGA1UEAxMxRW50cnVzdC5u\n"
        "ZXQgU2VjdXJlIFNlcnZlciBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTAeFw05OTA1\n"
        "MjUxNjA5NDBaFw0xOTA1MjUxNjM5NDBaMIHDMQswCQYDVQQGEwJVUzEUMBIGA1UE\n"
        "ChMLRW50cnVzdC5uZXQxOzA5BgNVBAsTMnd3dy5lbnRydXN0Lm5ldC9DUFMgaW5j\n"
        "b3JwLiBieSByZWYuIChsaW1pdHMgbGlhYi4pMSUwIwYDVQQLExwoYykgMTk5OSBF\n"
        "bnRydXN0Lm5ldCBMaW1pdGVkMTowOAYDVQQDEzFFbnRydXN0Lm5ldCBTZWN1cmUg\n"
        "U2VydmVyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIGdMA0GCSqGSIb3DQEBAQUA\n"
        "A4GLADCBhwKBgQDNKIM0VBuJ8w+vN5Ex/68xYMmo6LIQaO2f55M28Qpku0f1BBc/\n"
        "I0dNxScZgSYMVHINiC3ZH5oSn7yzcdOAGT9HZnuMNSjSuQrfJNqc1lB5gXpa0zf3\n"
        "wkrYKZImZNHkmGw6AIr1NJtl+O3jEP/9uElY3KDegjlrgbEWGWG5VLbmQwIBA6OC\n"
        "AdcwggHTMBEGCWCGSAGG+EIBAQQEAwIABzCCARkGA1UdHwSCARAwggEMMIHeoIHb\n"
        "oIHYpIHVMIHSMQswCQYDVQQGEwJVUzEUMBIGA1UEChMLRW50cnVzdC5uZXQxOzA5\n"
        "BgNVBAsTMnd3dy5lbnRydXN0Lm5ldC9DUFMgaW5jb3JwLiBieSByZWYuIChsaW1p\n"
        "dHMgbGlhYi4pMSUwIwYDVQQLExwoYykgMTk5OSBFbnRydXN0Lm5ldCBMaW1pdGVk\n"
        "MTowOAYDVQQDEzFFbnRydXN0Lm5ldCBTZWN1cmUgU2VydmVyIENlcnRpZmljYXRp\n"
        "b24gQXV0aG9yaXR5MQ0wCwYDVQQDEwRDUkwxMCmgJ6AlhiNodHRwOi8vd3d3LmVu\n"
        "dHJ1c3QubmV0L0NSTC9uZXQxLmNybDArBgNVHRAEJDAigA8xOTk5MDUyNTE2MDk0\n"
        "MFqBDzIwMTkwNTI1MTYwOTQwWjALBgNVHQ8EBAMCAQYwHwYDVR0jBBgwFoAU8Bdi\n"
        "E1U9s/8KAGv7UISX8+1i0BowHQYDVR0OBBYEFPAXYhNVPbP/CgBr+1CEl/PtYtAa\n"
        "MAwGA1UdEwQFMAMBAf8wGQYJKoZIhvZ9B0EABAwwChsEVjQuMAMCBJAwDQYJKoZI\n"
        "hvcNAQEFBQADgYEAkNwwAvpkdMKnCqV8IY00F6j7Rw7/JXyNEwr75Ji174z4xRAN\n"
        "95K+8cPV1ZVqBLssziY2ZcgxxufuP+NXdYR6Ee9GTxj005i7qIcyunL2POI9n9cd\n"
        "2cNgQ4xYDiKWL2KjLB+6rQXvqzJ4h6BUcxm1XAX5Uj5tLUUL9wqT6u0G+bI=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Equifax Secure Certificate Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDIDCCAomgAwIBAgIENd70zzANBgkqhkiG9w0BAQUFADBOMQswCQYDVQQGEwJV\n"
        "UzEQMA4GA1UEChMHRXF1aWZheDEtMCsGA1UECxMkRXF1aWZheCBTZWN1cmUgQ2Vy\n"
        "dGlmaWNhdGUgQXV0aG9yaXR5MB4XDTk4MDgyMjE2NDE1MVoXDTE4MDgyMjE2NDE1\n"
        "MVowTjELMAkGA1UEBhMCVVMxEDAOBgNVBAoTB0VxdWlmYXgxLTArBgNVBAsTJEVx\n"
        "dWlmYXggU2VjdXJlIENlcnRpZmljYXRlIEF1dGhvcml0eTCBnzANBgkqhkiG9w0B\n"
        "AQEFAAOBjQAwgYkCgYEAwV2xWGcIYu6gmi0fCG2RFGiYCh7+2gRvE4RiIcPRfM6f\n"
        "BeC4AfBONOziipUEZKzxa1NfBbPLZ4C/QgKO/t0BCezhABRP/PvwDN1Dulsr4R+A\n"
        "cJkVV5MW8Q+XarfCaCMczE1ZMKxRHjuvK9buY0V7xdlfUNLjUA86iOe/FP3gx7kC\n"
        "AwEAAaOCAQkwggEFMHAGA1UdHwRpMGcwZaBjoGGkXzBdMQswCQYDVQQGEwJVUzEQ\n"
        "MA4GA1UEChMHRXF1aWZheDEtMCsGA1UECxMkRXF1aWZheCBTZWN1cmUgQ2VydGlm\n"
        "aWNhdGUgQXV0aG9yaXR5MQ0wCwYDVQQDEwRDUkwxMBoGA1UdEAQTMBGBDzIwMTgw\n"
        "ODIyMTY0MTUxWjALBgNVHQ8EBAMCAQYwHwYDVR0jBBgwFoAUSOZo+SvSspXXR9gj\n"
        "IBBPM5iQn9QwHQYDVR0OBBYEFEjmaPkr0rKV10fYIyAQTzOYkJ/UMAwGA1UdEwQF\n"
        "MAMBAf8wGgYJKoZIhvZ9B0EABA0wCxsFVjMuMGMDAgbAMA0GCSqGSIb3DQEBBQUA\n"
        "A4GBAFjOKer89961zgK5F7WF0bnj4JXMJTENAKaSbn+2kmOeUJXRmm/kEd5jhW6Y\n"
        "7qj/WsjTVbJmcVfewCHrPSqnI0kBBIZCe/zuf6IWUrVnZ9NA2zsmWLIodz2uFHdh\n"
        "1voqZiegDfqnc1zqcPGUIWVEX/r87yloqaKHee9570+sB3c4\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Equifax Secure eBusiness CA-1.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICgjCCAeugAwIBAgIBBDANBgkqhkiG9w0BAQQFADBTMQswCQYDVQQGEwJVUzEc\n"
        "MBoGA1UEChMTRXF1aWZheCBTZWN1cmUgSW5jLjEmMCQGA1UEAxMdRXF1aWZheCBT\n"
        "ZWN1cmUgZUJ1c2luZXNzIENBLTEwHhcNOTkwNjIxMDQwMDAwWhcNMjAwNjIxMDQw\n"
        "MDAwWjBTMQswCQYDVQQGEwJVUzEcMBoGA1UEChMTRXF1aWZheCBTZWN1cmUgSW5j\n"
        "LjEmMCQGA1UEAxMdRXF1aWZheCBTZWN1cmUgZUJ1c2luZXNzIENBLTEwgZ8wDQYJ\n"
        "KoZIhvcNAQEBBQADgY0AMIGJAoGBAM4vGbwXt3fek6lfWg0XTzQaDJj0ItlZ1MRo\n"
        "RvC0NcWFAyDGr0WlIVFFQesWWDYyb+JQYmT5/VGcqiTZ9J2DKocKIdMSODRsjQBu\n"
        "WqDZQu4aIZX5UkxVWsUPOE9G+m34LjXWHXzr4vCwdYDIqROsvojvOm6rXyo4YgKw\n"
        "Env+j6YDAgMBAAGjZjBkMBEGCWCGSAGG+EIBAQQEAwIABzAPBgNVHRMBAf8EBTAD\n"
        "AQH/MB8GA1UdIwQYMBaAFEp4MlIR21kWNl7fwRQ2QGpHfEyhMB0GA1UdDgQWBBRK\n"
        "eDJSEdtZFjZe38EUNkBqR3xMoTANBgkqhkiG9w0BAQQFAAOBgQB1W6ibAxHm6VZM\n"
        "zfmpTMANmvPMZWnmJXbMWbfWVMMdzZmsGd20hdXgPfxiIKeES1hl8eL5lSE/9dR+\n"
        "WB5Hh1Q+WKG1tfgq73HnvMP2sUlG4tega+VWeponmHxGYhTnyfxuAxJ5gDgdSIKN\n"
        "/Bf+KpYrtWKmpj29f5JZzVoqgrI3eQ==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Equifax Secure eBusiness CA-2.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDIDCCAomgAwIBAgIEN3DPtTANBgkqhkiG9w0BAQUFADBOMQswCQYDVQQGEwJV\n"
        "UzEXMBUGA1UEChMORXF1aWZheCBTZWN1cmUxJjAkBgNVBAsTHUVxdWlmYXggU2Vj\n"
        "dXJlIGVCdXNpbmVzcyBDQS0yMB4XDTk5MDYyMzEyMTQ0NVoXDTE5MDYyMzEyMTQ0\n"
        "NVowTjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDkVxdWlmYXggU2VjdXJlMSYwJAYD\n"
        "VQQLEx1FcXVpZmF4IFNlY3VyZSBlQnVzaW5lc3MgQ0EtMjCBnzANBgkqhkiG9w0B\n"
        "AQEFAAOBjQAwgYkCgYEA5Dk5kx5SBhsoNviyoynF7Y6yEb3+6+e0dMKP/wXn2Z0G\n"
        "vxLIPw7y1tEkshHe0XMJitSxLJgJDR5QRrKDpkWNYmi7hRsgcDKqQM2mll/EcTc/\n"
        "BPO3QSQ5BxoeLmFYoBIL5aXfxavqN3HMHMg3OrmXUqesxWoklE6ce8/AatbfIb0C\n"
        "AwEAAaOCAQkwggEFMHAGA1UdHwRpMGcwZaBjoGGkXzBdMQswCQYDVQQGEwJVUzEX\n"
        "MBUGA1UEChMORXF1aWZheCBTZWN1cmUxJjAkBgNVBAsTHUVxdWlmYXggU2VjdXJl\n"
        "IGVCdXNpbmVzcyBDQS0yMQ0wCwYDVQQDEwRDUkwxMBoGA1UdEAQTMBGBDzIwMTkw\n"
        "NjIzMTIxNDQ1WjALBgNVHQ8EBAMCAQYwHwYDVR0jBBgwFoAUUJ4L6q9euSBIplBq\n"
        "y/3YIHqngnYwHQYDVR0OBBYEFFCeC+qvXrkgSKZQasv92CB6p4J2MAwGA1UdEwQF\n"
        "MAMBAf8wGgYJKoZIhvZ9B0EABA0wCxsFVjMuMGMDAgbAMA0GCSqGSIb3DQEBBQUA\n"
        "A4GBAAyGgq3oThr1jokn4jVYPSm0B482UJW/bsGe68SQsoWou7dC4A8HOd/7npCy\n"
        "0cE+U58DRLB+S/Rv5Hwf5+Kx5Lia78O9zt4LMjTZ3ijtM2vE1Nc9ElirfQkty3D1\n"
        "E4qUoSek1nDFbZS1yX2doNLGCEnZZpum0/QL3MUmV+GRMOrN\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Equifax Secure Global eBusiness CA-1.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICkDCCAfmgAwIBAgIBATANBgkqhkiG9w0BAQQFADBaMQswCQYDVQQGEwJVUzEc\n"
        "MBoGA1UEChMTRXF1aWZheCBTZWN1cmUgSW5jLjEtMCsGA1UEAxMkRXF1aWZheCBT\n"
        "ZWN1cmUgR2xvYmFsIGVCdXNpbmVzcyBDQS0xMB4XDTk5MDYyMTA0MDAwMFoXDTIw\n"
        "MDYyMTA0MDAwMFowWjELMAkGA1UEBhMCVVMxHDAaBgNVBAoTE0VxdWlmYXggU2Vj\n"
        "dXJlIEluYy4xLTArBgNVBAMTJEVxdWlmYXggU2VjdXJlIEdsb2JhbCBlQnVzaW5l\n"
        "c3MgQ0EtMTCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEAuucXkAJlsTRVPEnC\n"
        "UdXfp9E3j9HngXNBUmCbnaEXJnitx7HoJpQytd4zjTov2/KaelpzmKNc6fuKcxtc\n"
        "58O/gGzNqfTWK8D3+ZmqY6KxRwIP1ORROhI8bIpaVIRw28HFkM9yRcuoWcDNM50/\n"
        "o5brhTMhHD4ePmBudpxnhcXIw2ECAwEAAaNmMGQwEQYJYIZIAYb4QgEBBAQDAgAH\n"
        "MA8GA1UdEwEB/wQFMAMBAf8wHwYDVR0jBBgwFoAUvqigdHJQa0S3ySPY+6j/s1dr\n"
        "aGwwHQYDVR0OBBYEFL6ooHRyUGtEt8kj2Puo/7NXa2hsMA0GCSqGSIb3DQEBBAUA\n"
        "A4GBADDiAVGqx+pf2rnQZQ8w1j7aDRRJbpGTJxQx78T3LUX47Me/okENI7SS+RkA\n"
        "Z70Br83gcfxaz2TE4JaY0KNA4gGK7ycH8WUBikQtBmV1UsCGECAhX2xrD2yuCRyv\n"
        "8qIYNMR1pHMc8Y3c7635s3a0kr/clRAevsvIO1qEYBlWlKlV\n"
        "-----END CERTIFICATE-----\n",

        // ./root/EUnet International Root CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICqjCCAhOgAwIBAgIBADANBgkqhkiG9w0BAQUFADBEMRwwGgYDVQQKExNFVW5l\n"
        "dCBJbnRlcm5hdGlvbmFsMSQwIgYDVQQDExtFVW5ldCBJbnRlcm5hdGlvbmFsIFJv\n"
        "b3QgQ0EwHhcNOTkwMTE1MTUwMDU0WhcNMTgxMDAyMTUwMDU0WjBEMRwwGgYDVQQK\n"
        "ExNFVW5ldCBJbnRlcm5hdGlvbmFsMSQwIgYDVQQDExtFVW5ldCBJbnRlcm5hdGlv\n"
        "bmFsIFJvb3QgQ0EwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAJ5BO9lBSYso\n"
        "nFkU91BoKHtNCFxn/4UeC6bPOjICsHx8Sp+2iQIhDEqZi8tkUX54WrZeqVO37+/H\n"
        "36UhWZdO4gflLXwZ1SrKyDPTlhAu3aAAn/3K6GGpU8q2HRg+nBHgY+cqZ1vIRJXw\n"
        "E1VIu75VuGc3zcsAEw38T2kUalGU7LwnAgMBAAGjgaswgagwHQYDVR0OBBYEFPfn\n"
        "3cvO2zj5trJnDlTQWQwyP929MGwGA1UdIwRlMGOAFPfn3cvO2zj5trJnDlTQWQwy\n"
        "P929oUikRjBEMRwwGgYDVQQKExNFVW5ldCBJbnRlcm5hdGlvbmFsMSQwIgYDVQQD\n"
        "ExtFVW5ldCBJbnRlcm5hdGlvbmFsIFJvb3QgQ0GCAQAwDAYDVR0TBAUwAwEB/zAL\n"
        "BgNVHQ8EBAMCAQYwDQYJKoZIhvcNAQEFBQADgYEAXpHCGSGnCQkOM8Gomr2Y+bLy\n"
        "sWDElw2lMJtfOwnfTXSsMhZ2C9PcIZ8iDwl2B8ulk0L4tG5cEo8Ur46QG2oD/ykx\n"
        "HjNKFxz3N40fgMCDZsQAplYV+qAyVROqS/5J8Nqa09a+pahKzWjs/3O2qwuj22FE\n"
        "mmRjCbEnB7VAjxcALlA=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Fabrica Nacional de Moneda y Timbre.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIC+TCCAmKgAwIBAgIENvEbGTANBgkqhkiG9w0BAQUFADA2MQswCQYDVQQGEwJF\n"
        "UzENMAsGA1UEChMERk5NVDEYMBYGA1UECxMPRk5NVCBDbGFzZSAyIENBMB4XDTk5\n"
        "MDMxODE0NTYxOVoXDTE5MDMxODE1MjYxOVowNjELMAkGA1UEBhMCRVMxDTALBgNV\n"
        "BAoTBEZOTVQxGDAWBgNVBAsTD0ZOTVQgQ2xhc2UgMiBDQTCBnTANBgkqhkiG9w0B\n"
        "AQEFAAOBiwAwgYcCgYEAmD+tGTaTPT7+dkIU/TVv8fqtInpY40bQXcZa+WItjzFe\n"
        "/rQw/lB0rNadHeBixkndFBJ9cQusBsE/1waH4JCJ1uXjA7LyJ7GfM8iqazZKo8Q/\n"
        "eUGdiUYvKz5j1DhWkaodsQ1CdU3zh07jD03MtGy/YhOH6tCbjrbi/xn0lAnVlmEC\n"
        "AQOjggEUMIIBEDARBglghkgBhvhCAQEEBAMCAAcwWAYDVR0fBFEwTzBNoEugSaRH\n"
        "MEUxCzAJBgNVBAYTAkVTMQ0wCwYDVQQKEwRGTk1UMRgwFgYDVQQLEw9GTk1UIENs\n"
        "YXNlIDIgQ0ExDTALBgNVBAMTBENSTDEwKwYDVR0QBCQwIoAPMTk5OTAzMTgxNDU2\n"
        "MTlagQ8yMDE5MDMxODE0NTYxOVowCwYDVR0PBAQDAgEGMB8GA1UdIwQYMBaAFECa\n"
        "dkSXdAfErBTLHo1POkV8MNdhMB0GA1UdDgQWBBRAmnZEl3QHxKwUyx6NTzpFfDDX\n"
        "YTAMBgNVHRMEBTADAQH/MBkGCSqGSIb2fQdBAAQMMAobBFY0LjADAgSQMA0GCSqG\n"
        "SIb3DQEBBQUAA4GBAGFMoHxZY1tm+O5lE85DgEe5sjXJyITHa3NgReSdN531jiW5\n"
        "+aqqyuP4Q5wvoIkFsUUylCoeA41dpt7PV5Xa3yZgX8vflR64zgjY+IrJT6lodZPj\n"
        "LwVMZGACokIeb4ZoZVUO2ENv8pExPqNHPCgFr0W2nSJMJntLfVsV+RlG3whd\n"
        "-----END CERTIFICATE-----\n",

        // ./root/FESTE, Public Notary Certs.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICmTCCAgICAQAwDQYJKoZIhvcNAQEEBQAwgZQxCzAJBgNVBAYTAkVTMRIwEAYD\n"
        "VQQIEwlCYXJjZWxvbmExEjAQBgNVBAcTCUJhcmNlbG9uYTEYMBYGA1UEChMPRnVu\n"
        "ZGFjaW9uIEZFU1RFMSMwIQYDVQQDExpGRVNURSwgUHVibGljIE5vdGFyeSBDZXJ0\n"
        "czEeMBwGCSqGSIb3DQEJARYPZmVzdGVAZmVzdGUub3JnMB4XDTk5MDUxMzE5MjEy\n"
        "OFoXDTIwMDEwMTE5MjEyOFowgZQxCzAJBgNVBAYTAkVTMRIwEAYDVQQIEwlCYXJj\n"
        "ZWxvbmExEjAQBgNVBAcTCUJhcmNlbG9uYTEYMBYGA1UEChMPRnVuZGFjaW9uIEZF\n"
        "U1RFMSMwIQYDVQQDExpGRVNURSwgUHVibGljIE5vdGFyeSBDZXJ0czEeMBwGCSqG\n"
        "SIb3DQEJARYPZmVzdGVAZmVzdGUub3JnMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCB\n"
        "iQKBgQDhg/ObLsnn4cf0VAXNdkD+tLMTvucVXoYm6EB3GlU/0QMmjPqHX6TF+f61\n"
        "MonGf0GR2BVATnBS8PHa+GI1mV4clFNhzD5iwINdWNH4SBFxbPewd+EYl7QHKDCR\n"
        "McdPVPOEnsxZiUVtfrTJ245ClWbU3x4YTfylD9YahDnEyvK98wIDAQABMA0GCSqG\n"
        "SIb3DQEBBAUAA4GBAKY/Lh5JIBFgcKST3mTjaHSKt7c8eyF6ouErXa4o68k3RdI1\n"
        "e1gX2hD2ruFWYrrtIXyum2F8pP7euO0Y7iKIaDAiAWhZYnq7c5mLywn1nfGBvvbU\n"
        "lYjtVQzFnCYVjPFDpQNhffVm6rWhMjuED+ykatF75/Mv3wRihCluNha9WID/\n"
        "-----END CERTIFICATE-----\n",

        // ./root/FESTE, Verified Certs.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICjzCCAfgCAQAwDQYJKoZIhvcNAQEEBQAwgY8xCzAJBgNVBAYTAkVTMRIwEAYD\n"
        "VQQIEwlCYXJjZWxvbmExEjAQBgNVBAcTCUJhcmNlbG9uYTEYMBYGA1UEChMPRnVu\n"
        "ZGFjaW9uIEZFU1RFMR4wHAYDVQQDExVGRVNURSwgVmVyaWZpZWQgQ2VydHMxHjAc\n"
        "BgkqhkiG9w0BCQEWD2Zlc3RlQGZlc3RlLm9yZzAeFw05OTA1MTMxOTIzNTNaFw0y\n"
        "MDAxMDExOTIzNTNaMIGPMQswCQYDVQQGEwJFUzESMBAGA1UECBMJQmFyY2Vsb25h\n"
        "MRIwEAYDVQQHEwlCYXJjZWxvbmExGDAWBgNVBAoTD0Z1bmRhY2lvbiBGRVNURTEe\n"
        "MBwGA1UEAxMVRkVTVEUsIFZlcmlmaWVkIENlcnRzMR4wHAYJKoZIhvcNAQkBFg9m\n"
        "ZXN0ZUBmZXN0ZS5vcmcwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAOpjnx84\n"
        "GoQEhLNLkxmEontiw6erLOxgTlUR4gQyJyA1+Rp4Hydnp+CCsAXoJqmbikRWv0uE\n"
        "U6Mipg4WU7l/rxDIMxhU2cZMfNFbVlJThqZbPxPywwHSMaEzXLxd5NxiPMojl09w\n"
        "+4GOsNmer5PqtrEkjQQtUUM8OOwnQ3GH6DzHAgMBAAEwDQYJKoZIhvcNAQEEBQAD\n"
        "gYEAfvj3P7Lu3vohb4EUd4ae71xPjpbPfHXP1F1/cUbURQpgaiqW8dSIpI6j9xcw\n"
        "LsFK8Butk7JgKQzhxHWSZcOGzXLGMh+gVb485XfbHWQTmIslbgB/7KPhWgTYLKgS\n"
        "ozCGRKBhN/4QMHk0zeX6AnU192Ft9R9kmwOQ0EyhTG4bO9I=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/First Data Digital Certificates Inc. Certification Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEJzCCA5CgAwIBAgIEN35hxjANBgkqhkiG9w0BAQQFADCBgzELMAkGA1UEBhMC\n"
        "VVMxLTArBgNVBAoTJEZpcnN0IERhdGEgRGlnaXRhbCBDZXJ0aWZpY2F0ZXMgSW5j\n"
        "LjFFMEMGA1UEAxM8Rmlyc3QgRGF0YSBEaWdpdGFsIENlcnRpZmljYXRlcyBJbmMu\n"
        "IENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTk5MDcwMzE4NDczNFoXDTE5MDcw\n"
        "MzE5MTczNFowgYMxCzAJBgNVBAYTAlVTMS0wKwYDVQQKEyRGaXJzdCBEYXRhIERp\n"
        "Z2l0YWwgQ2VydGlmaWNhdGVzIEluYy4xRTBDBgNVBAMTPEZpcnN0IERhdGEgRGln\n"
        "aXRhbCBDZXJ0aWZpY2F0ZXMgSW5jLiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTCB\n"
        "nTANBgkqhkiG9w0BAQEFAAOBiwAwgYcCgYEA3xwUHgm5v6RAciCZebaEIvTXhZLF\n"
        "BCToBy4C5BeVBTeVdj38seUPhw5iuSwwlybhCxVnAKYV3uiNy5XsAlhSwEdlM0xW\n"
        "nwofBMA3UIFXut/68mtn68vQgA/ZV5UQZXsGRVjrrrRe45MVK5m8tikv+0KfRysu\n"
        "TOs0KDKZDu//b6ECAQOjggGmMIIBojARBglghkgBhvhCAQEEBAMCAAcwgawGA1Ud\n"
        "HwSBpDCBoTCBnqCBm6CBmKSBlTCBkjELMAkGA1UEBhMCVVMxLTArBgNVBAoTJEZp\n"
        "cnN0IERhdGEgRGlnaXRhbCBDZXJ0aWZpY2F0ZXMgSW5jLjFFMEMGA1UEAxM8Rmly\n"
        "c3QgRGF0YSBEaWdpdGFsIENlcnRpZmljYXRlcyBJbmMuIENlcnRpZmljYXRpb24g\n"
        "QXV0aG9yaXR5MQ0wCwYDVQQDEwRDUkwxMCsGA1UdEAQkMCKADzE5OTkwNzAzMTg0\n"
        "NzM0WoEPMjAxOTA3MDMxODQ3MzRaMAsGA1UdDwQEAwIBBjAfBgNVHSMEGDAWgBSm\n"
        "uCDJFkuPT1wMw8PumA0+fu5WVTAdBgNVHQ4EFgQUprggyRZLj09cDMPD7pgNPn7u\n"
        "VlUwDAYDVR0TBAUwAwEB/zA7BgNVHSUENDAyBggrBgEFBQcDAQYIKwYBBQUHAwIG\n"
        "CCsGAQUFBwMDBggrBgEFBQcDBAYIKwYBBQUHAwgwGQYJKoZIhvZ9B0EABAwwChsE\n"
        "VjQuMAMCBJAwDQYJKoZIhvcNAQEEBQADgYEAEObEaCOpbLeXSbFzNp3+v3KiDhLC\n"
        "KlEGH2mTlDARNYVOqHkG43FVPBxWYx5Ee2qBwjB1bN7z8gzDTsp/ycbAX1/vxAZi\n"
        "qk/6EN4yzOAu/2rixcdFKXU5+YxZC8ZrmQSYWsy6v7F4ApGqtoeAO1cUWzz8zAPK\n"
        "hqGZqDpta2V+Ubg=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/GeoTrust Global CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDVDCCAjygAwIBAgIDAjRWMA0GCSqGSIb3DQEBBQUAMEIxCzAJBgNVBAYTAlVT\n"
        "MRYwFAYDVQQKEw1HZW9UcnVzdCBJbmMuMRswGQYDVQQDExJHZW9UcnVzdCBHbG9i\n"
        "YWwgQ0EwHhcNMDIwNTIxMDQwMDAwWhcNMjIwNTIxMDQwMDAwWjBCMQswCQYDVQQG\n"
        "EwJVUzEWMBQGA1UEChMNR2VvVHJ1c3QgSW5jLjEbMBkGA1UEAxMSR2VvVHJ1c3Qg\n"
        "R2xvYmFsIENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2swYYzD9\n"
        "9BcjGlZ+W988bDjkcbd4kdS8odhM+KhDtgPpTSEHCIjaWC9mOSm9BXiLnTjoBbdq\n"
        "fnGk5sRgprDvgOSJKA+eJdbtg/OtppHHmMlCGDUUna2YRpIuT8rxh0PBFpVXLVDv\n"
        "iS2Aelet8u5fa9IAjbkU+BQVNdnARqN7csiRv8lVK83Qlz6cJmTM386DGXHKTubU\n"
        "1XupGc1V3sjs0l44U+VcT4wt/lAjNvxm5suOpDkZALeVAjmRCw7+OC7RHQWa9k0+\n"
        "bw8HHa8sHo9gOeL6NlMTOdReJivbPagUvTLrGAMoUgRx5aszPeE4uwc2hGKceeoW\n"
        "MPRfwCvocWvk+QIDAQABo1MwUTAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBTA\n"
        "ephojYn7qwVkDBF9qn1luMrMTjAfBgNVHSMEGDAWgBTAephojYn7qwVkDBF9qn1l\n"
        "uMrMTjANBgkqhkiG9w0BAQUFAAOCAQEANeMpauUvXVSOKVCUn5kaFOSPeCpilKIn\n"
        "Z57QzxpeR+nBsqTP3UEaBU6bS+5Kb1VSsyShNwrrZHYqLizz/Tt1kL/6cdjHPTfS\n"
        "tQWVYrmm3ok9Nns4d0iXrKYgjy6myQzCsplFAMfOEVEiIuCl6rYVSAlk6l5PdPcF\n"
        "PseKUgzbFbS9bZvlxrFUaKnjaZC2mqUPuLk/IH2uSrW4nOQdtqvmlKXBx4Ot2/Un\n"
        "hw4EbNX/3aBd7YdStysVAq45pmp06drE57xNNB6pXE0zX5IJL4hmXXeXxx12E6nV\n"
        "5fEWCRE11azbJHFwLJhWC9kXtNHjUStedejV0NxPNO3CBWaAocvmMw==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/GlobalSign.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\n"
        "A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\n"
        "b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\n"
        "MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\n"
        "YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\n"
        "aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\n"
        "jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\n"
        "xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\n"
        "1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\n"
        "snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\n"
        "U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\n"
        "9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\n"
        "BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\n"
        "AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\n"
        "yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\n"
        "38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\n"
        "AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\n"
        "DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\n"
        "HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Go Daddy Class 2 Certification Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEADCCAuigAwIBAgIBADANBgkqhkiG9w0BAQUFADBjMQswCQYDVQQGEwJVUzEh\n"
        "MB8GA1UEChMYVGhlIEdvIERhZGR5IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBE\n"
        "YWRkeSBDbGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTA0MDYyOTE3\n"
        "MDYyMFoXDTM0MDYyOTE3MDYyMFowYzELMAkGA1UEBhMCVVMxITAfBgNVBAoTGFRo\n"
        "ZSBHbyBEYWRkeSBHcm91cCwgSW5jLjExMC8GA1UECxMoR28gRGFkZHkgQ2xhc3Mg\n"
        "MiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTCCASAwDQYJKoZIhvcNAQEBBQADggEN\n"
        "ADCCAQgCggEBAN6d1+pXGEmhW+vXX0iG6r7d/+TvZxz0ZWizV3GgXne77ZtJ6XCA\n"
        "PVYYYwhv2vLM0D9/AlQiVBDYsoHUwHU9S3/Hd8M+eKsaA7Ugay9qK7HFiH7Eux6w\n"
        "wdhFJ2+qN1j3hybX2C32qRe3H3I2TqYXP2WYktsqbl2i/ojgC95/5Y0V4evLOtXi\n"
        "EqITLdiOr18SPaAIBQi2XKVlOARFmR6jYGB0xUGlcmIbYsUfb18aQr4CUWWoriMY\n"
        "avx4A6lNf4DD+qta/KFApMoZFv6yyO9ecw3ud72a9nmYvLEHZ6IVDd2gWMZEewo+\n"
        "YihfukEHU1jPEX44dMX4/7VpkI+EdOqXG68CAQOjgcAwgb0wHQYDVR0OBBYEFNLE\n"
        "sNKR1EwRcbNhyz2h/t2oatTjMIGNBgNVHSMEgYUwgYKAFNLEsNKR1EwRcbNhyz2h\n"
        "/t2oatTjoWekZTBjMQswCQYDVQQGEwJVUzEhMB8GA1UEChMYVGhlIEdvIERhZGR5\n"
        "IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBEYWRkeSBDbGFzcyAyIENlcnRpZmlj\n"
        "YXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEFBQAD\n"
        "ggEBADJL87LKPpH8EsahB4yOd6AzBhRckB4Y9wimPQoZ+YeAEW5p5JYXMP80kWNy\n"
        "OO7MHAGjHZQopDH2esRU1/blMVgDoszOYtuURXO1v0XJJLXVggKtI3lpjbi2Tc7P\n"
        "TMozI+gciKqdi0FuFskg5YmezTvacPd+mSYgFFQlq25zheabIZ0KbIIOqPjCDPoQ\n"
        "HmyW74cNxA9hi63ugyuV+I6ShHI56yDqg+2DzZduCLzrTia2cyvk0/ZM/iZx4mER\n"
        "dEr/VxqHD3VILs9RaRegAhJhldXRQLIQTO7ErBBDpqWeCtWVYpoNz4iCxTIM5Cuf\n"
        "ReYNnyicsbkqWletNw+vHX/bvZ8=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/GTE CyberTrust Global Root.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICWjCCAcMCAgGlMA0GCSqGSIb3DQEBBAUAMHUxCzAJBgNVBAYTAlVTMRgwFgYD\n"
        "VQQKEw9HVEUgQ29ycG9yYXRpb24xJzAlBgNVBAsTHkdURSBDeWJlclRydXN0IFNv\n"
        "bHV0aW9ucywgSW5jLjEjMCEGA1UEAxMaR1RFIEN5YmVyVHJ1c3QgR2xvYmFsIFJv\n"
        "b3QwHhcNOTgwODEzMDAyOTAwWhcNMTgwODEzMjM1OTAwWjB1MQswCQYDVQQGEwJV\n"
        "UzEYMBYGA1UEChMPR1RFIENvcnBvcmF0aW9uMScwJQYDVQQLEx5HVEUgQ3liZXJU\n"
        "cnVzdCBTb2x1dGlvbnMsIEluYy4xIzAhBgNVBAMTGkdURSBDeWJlclRydXN0IEds\n"
        "b2JhbCBSb290MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCVD6C28FCc6HrH\n"
        "iM3dFw4usJTQGz0O9pTAipTHBsiQl8i4ZBp6fmw8U+E3KHNgf7KXUwefU/ltWJTS\n"
        "r41tiGeA5u2ylc9yMcqlHHK6XALnZELn+aks1joNrI1CqiQBOeacPwGFVw1Yh0X4\n"
        "04Wqk2kmhXBIgD8SFcd5tB8FLztimQIDAQABMA0GCSqGSIb3DQEBBAUAA4GBAG3r\n"
        "GwnpXtlR22ciYaQqPEh346B8pt5zohQDhT37qw4wxYMWM4ETCJ57NE7fQMh017l9\n"
        "3PR2VX2bY1QY6fDq81yx2YtCHrnAlU66+tXifPVoYb+O7AWXX1uw16OFNMQkpw0P\n"
        "lZPvy5TYnh+dXIVtx6quTx8itc2VrbqnzPmrC3p/\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Japan Certification Services, Inc. SecureSign RootCA1.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDKTCCAhECCF9gWF8AAAAAMA0GCSqGSIb3DQEBBQUAMFcxCzAJBgNVBAYTAkpQ\n"
        "MSswKQYDVQQKEyJKYXBhbiBDZXJ0aWZpY2F0aW9uIFNlcnZpY2VzLCBJbmMuMRsw\n"
        "GQYDVQQDExJTZWN1cmVTaWduIFJvb3RDQTEwHhcNOTkwOTE1MTUwMDAxWhcNMjAw\n"
        "OTE1MTQ1OTU5WjBXMQswCQYDVQQGEwJKUDErMCkGA1UEChMiSmFwYW4gQ2VydGlm\n"
        "aWNhdGlvbiBTZXJ2aWNlcywgSW5jLjEbMBkGA1UEAxMSU2VjdXJlU2lnbiBSb290\n"
        "Q0ExMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAlJAMS3EpHNr2aHl6\n"
        "pLrn0syNr+hHkJkfxirql2PoH84XV8Yas6jHfIftNTWAurpubb4X/swtG2zvigBJ\n"
        "FuHuBl5KB12rPdFQuJFG1NTaFdiUXA7K19q/oPdJPMi7zuomgQoULZwNN0VrQcpX\n"
        "izjwJh8x/M80jo93wT/jq1Q8J7TOMkxVE2L8/joWJc8ba6Ijt+DqAmm79yJxbXwL\n"
        "GZOhl5zjkWkfaOQvfRBtj2euwRCisF5jSpf35niprSa7VMnftO7FntMl3RNoU/mP\n"
        "6Ozl3oHWeD7uUEC0ATysFcGCOy5/8VIni3Lg59v5iynDw0orM4mrXCoH/HwjHitP\n"
        "CCL+wQIDAQABMA0GCSqGSIb3DQEBBQUAA4IBAQBV7W97k+VFMU5o1VWBoggfbN0J\n"
        "xXgacFfI3wiBrmZ3xnUP5O9JiwNcbP8ckKRystMWErIG+EaGrr+nFduFTfrCLU2z\n"
        "tbBD73x+B9tfs1dGUXYHhkT9B+rxy0tFTWanMybE+UOqjRKz1I1otvcCebQtWtcD\n"
        "mAQsaZmv9GY7ZKyywCvIaVSeTE5IGI3OV7U7UeUb1/o5YNtWRRO+52bVI/Z8SACw\n"
        "TO80jSKssi7RTDjN+lgDBu46c4cKBTrK5K/Uwe4chX8lFs8nAR+EincI0NNG6CDs\n"
        "n6SM8bzNxBI2gB7HCSiv6Ai+wNOyPtcuZz2jzrs0+uKFzazOVR1FW3iF04V6\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Japan Certification Services, Inc. SecureSign RootCA2.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDKTCCAhECCF9gWF8AAAAAMA0GCSqGSIb3DQEBBQUAMFcxCzAJBgNVBAYTAkpQ\n"
        "MSswKQYDVQQKEyJKYXBhbiBDZXJ0aWZpY2F0aW9uIFNlcnZpY2VzLCBJbmMuMRsw\n"
        "GQYDVQQDExJTZWN1cmVTaWduIFJvb3RDQTIwHhcNOTkwOTE1MTUwMDAxWhcNMjAw\n"
        "OTE1MTQ1OTU5WjBXMQswCQYDVQQGEwJKUDErMCkGA1UEChMiSmFwYW4gQ2VydGlm\n"
        "aWNhdGlvbiBTZXJ2aWNlcywgSW5jLjEbMBkGA1UEAxMSU2VjdXJlU2lnbiBSb290\n"
        "Q0EyMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAlnuSIz9g3wk8WIAI\n"
        "42MJl+jkC3Vh1M0Oo/LjHkO6g/+6gVwvyN6Qi0wOLyn5B9aOs6Yor4Iqe8K0Zkxx\n"
        "9Ax0GrjbGuhoN6n5oaJuHCjNbCY8jyoznp3LtHnE2WQ9lcYzqEf75QcJ3PZtuCVC\n"
        "TMP7Su1bLtQHqOWTECSTWG59wdAez+kp19C8X0zwFRbD2MLO41sXW5SLKGsUZyQ7\n"
        "9FLsDW58TrSZAtvJ8w+CqwH0jN4WcMa8Fwdh/xFAhOosG3o6sANhB6qWjdDauYOO\n"
        "5J1RaXVxZIG0iFXcEIPOLaX1MJZhLjsK/IdfnFyCdRMe05jR7cntchYcDAbcWSB+\n"
        "8F3v9wIDAQABMA0GCSqGSIb3DQEBBQUAA4IBAQBp4FZw/zNayOHI06wwIXNITiLi\n"
        "MijLvBc6jX440fm3PzCFMo1X/B9Afz96dOwSCWTB/dz8HDW88Qu06qFc3ispkIID\n"
        "FsqexLuwko2VVlKZMsCQm1VD5PNNdlGtbDxcncJBfY1M8KmsOr4gJwWXcEZotmT4\n"
        "JZnplcOOOunB4hrAIrFtrbDunyzsdMPBZCySb5ZoCwFX6IaZr+eIyFlgbrJWBKtU\n"
        "uF0YFnbN2h50kEubRhGXCYHzzvJhmQPz6W5ITD/bvIDbgSD+vVxG/07nr03+vESA\n"
        "4wluZ3ol+1fnPOZVpaIBhig/f1/r9OsahFUxnyTwyfZpTBRSE7P5JBoX2Rhw\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Japan Certification Services, Inc. SecureSign RootCA3.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDKTCCAhECCF9gWF8AAAAAMA0GCSqGSIb3DQEBBQUAMFcxCzAJBgNVBAYTAkpQ\n"
        "MSswKQYDVQQKEyJKYXBhbiBDZXJ0aWZpY2F0aW9uIFNlcnZpY2VzLCBJbmMuMRsw\n"
        "GQYDVQQDExJTZWN1cmVTaWduIFJvb3RDQTMwHhcNOTkwOTE1MTUwMDAxWhcNMjAw\n"
        "OTE1MTQ1OTU5WjBXMQswCQYDVQQGEwJKUDErMCkGA1UEChMiSmFwYW4gQ2VydGlm\n"
        "aWNhdGlvbiBTZXJ2aWNlcywgSW5jLjEbMBkGA1UEAxMSU2VjdXJlU2lnbiBSb290\n"
        "Q0EzMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmV4egJZmI2TOnIwA\n"
        "PgqvCOm4BOCEuG1TdU02qLXg14xOYFW2A5ebWhqn87o92ZqUMXZ0I8n37BJd2CDU\n"
        "Hekbojd2BA8+rBZpO+H/EC9WJeQzUBMJzE4Oq/Dkddtx1fxKze3bDzUFFdWwZntC\n"
        "eyblWeK1x8Cyx6FD/Q8vC4MlJVeBu7vRNTB0kZCyj59o1dJDt7JFqSPAVtiHEtNz\n"
        "/stZ6q/85x9eVEUcqm2Vk2JHQkFeT+s2Bw4oeFQKfMDDJBOGAwK5rHaSSlrdxdzs\n"
        "+LPbK7UbNud4gkyVfiBWsnUcfZfvf5Q4KaIA4tHqseM0NjFAWLiqt86BGgwXgQ39\n"
        "67jTvQIDAQABMA0GCSqGSIb3DQEBBQUAA4IBAQCOmITWG42BrNADeug5vE3lEmqv\n"
        "qnJgMX8aLHphfjxMnBxqXEzRPZZZCy+Rsa3z4ajSn8GLdtyPrDCpsCvaZdajjtxQ\n"
        "S1ZNQkVKEh1hlSuanYJkFV31+8AWszcPA6obgFU5AEbSlIguLLTNq1u16cub5l8w\n"
        "vtB0XpsQL4B1Pwn5LuFmH1ls7W8uPjunJJA0u+SQp0vIL3kpXzZoHh0aFWrg3yuB\n"
        "QcY7SkaRv8qTe+GRLC08B18CrHueTg2tPlzkHGgin8+BsXtvmM2hOPPUiSVDhE9O\n"
        "4m5eJCRdlMI5z6X5cWAY+krJ5AB8Xv2JUrdtgnd4b61FxnLRgE2lBUPty+Kz\n"
        "-----END CERTIFICATE-----\n",

        // ./root/KeyMail PTT Post Root CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDkzCCAnugAwIBAgIEN3uAZDANBgkqhkiG9w0BAQUFADBtMQswCQYDVQQGEwJO\n"
        "TDERMA8GA1UEChMIUFRUIFBvc3QxEDAOBgNVBAsTB0tleU1haWwxGTAXBgNVBAMT\n"
        "EFBUVCBQb3N0IFJvb3QgQ0ExHjAcBgoJkiaJk/IsZAEDFA5jYUBwdHQtcG9zdC5u\n"
        "bDAeFw05OTA3MDExNDUxNDhaFw0xOTA2MjYxMzUxNDhaMG0xCzAJBgNVBAYTAk5M\n"
        "MREwDwYDVQQKEwhQVFQgUG9zdDEQMA4GA1UECxMHS2V5TWFpbDEZMBcGA1UEAxMQ\n"
        "UFRUIFBvc3QgUm9vdCBDQTEeMBwGCgmSJomT8ixkAQMUDmNhQHB0dC1wb3N0Lm5s\n"
        "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsH7iOgHxSK1T1HHO276A\n"
        "4FCtmaKEeto6JyQ6EYE2Eg3mo5mOpMwmtQ5hxu4oq22G3y6XYfpAacmNjMQxe/pS\n"
        "XlZMIJ5gGl9sSnjJiTyflYasd2cOpg5C6CxiSTJLBD4yQ5AOCiLKyHQOhe+DgcVb\n"
        "8ttshQhvTialBqt245iiTl7EgODo+8zpMGzycmGuJ35T1BWUD9KPeYLZ9o+rxhPm\n"
        "HJh0SwBhDnlpVPKQsqMJAWX3BEdsTvopK/AOBheT3ILAEd6PsDBGWUhKZs42r8fP\n"
        "MdGSdBQj1aq64InbEtHs1GkjuAsWSTPOGvninF98aB13uwGqZ+Ixxv/WOmn9DBt8\n"
        "IwIDAQABozswOTAPBgNVHRMBAf8EBTADAQH/MBEGA1UdDgQKBAhKGyoEHidFnjAT\n"
        "BgNVHSMEDDAKgAhKGyoEHidFnjANBgkqhkiG9w0BAQUFAAOCAQEAPfgyHnWiN2Vo\n"
        "VxwCfWX8W4pvWb++Mis950uAF1HMVWtkvbcFOE4rqZ0F0hWHL0gZkzB1/UvmIIVA\n"
        "m8fsXUO3tQzH6ksard18mzGvI/HiBUDZ5b9Ef2PMvgUiMQvl8orwt692y+O0wbcM\n"
        "th0GEleMHsvDipGMsV9oF5ihMVWsp2Rq6TSrim7o1H8+WePowplh9K20Sw4L1ISo\n"
        "plzoYEO83X5I8sNpXOmuEoGmDELOEuRzb78zmjNSeJTz2M5BGYxQJd8SkikrU6m5\n"
        "ZcqPIleFWvxZ1siS0HASVP1kmbycBVxUMenEElQBxoE2Nqxbiw2Gidi5ClJNyJ68\n"
        "19t/gG3SrQ==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Microsoft Root Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEEjCCAvqgAwIBAgIPAMEAizw8iBHRPvZj7N9AMA0GCSqGSIb3DQEBBAUAMHAx\n"
        "KzApBgNVBAsTIkNvcHlyaWdodCAoYykgMTk5NyBNaWNyb3NvZnQgQ29ycC4xHjAc\n"
        "BgNVBAsTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEhMB8GA1UEAxMYTWljcm9zb2Z0\n"
        "IFJvb3QgQXV0aG9yaXR5MB4XDTk3MDExMDA3MDAwMFoXDTIwMTIzMTA3MDAwMFow\n"
        "cDErMCkGA1UECxMiQ29weXJpZ2h0IChjKSAxOTk3IE1pY3Jvc29mdCBDb3JwLjEe\n"
        "MBwGA1UECxMVTWljcm9zb2Z0IENvcnBvcmF0aW9uMSEwHwYDVQQDExhNaWNyb3Nv\n"
        "ZnQgUm9vdCBBdXRob3JpdHkwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\n"
        "AQCpAr3BcOY78k4bKJ+XeF4w6qKpjSVf+P6VTKO3/p2iID58UaKboo9gMmvRQmR5\n"
        "7qx2yVTa8uuchhyPn4Rms8VremIj1h083g8BkuiWxL8tZpqaaCaZ0Dosvwy1WCbB\n"
        "RucKPjiWLKkoOajsSYNC44QPu5psVWGsgnyhYC13TOmZtGQ7mlAcMQgkFJ+p55Er\n"
        "GOY9mGMUYFgFZZ8dN1KH96fvlALGG9O/VUWziYC/OuxUlE6u/ad6bXROrxjMlgko\n"
        "IQBXkGBpN7tLEgc8Vv9b+6RmCgim0oFWV++2O14WgXcE2va+roCV/rDNf9anGnJc\n"
        "PMq88AijIjCzBoXJsyB3E4XfAgMBAAGjgagwgaUwgaIGA1UdAQSBmjCBl4AQW9Bw\n"
        "72lyniNRfhSyTY7/y6FyMHAxKzApBgNVBAsTIkNvcHlyaWdodCAoYykgMTk5NyBN\n"
        "aWNyb3NvZnQgQ29ycC4xHjAcBgNVBAsTFU1pY3Jvc29mdCBDb3Jwb3JhdGlvbjEh\n"
        "MB8GA1UEAxMYTWljcm9zb2Z0IFJvb3QgQXV0aG9yaXR5gg8AwQCLPDyIEdE+9mPs\n"
        "30AwDQYJKoZIhvcNAQEEBQADggEBAJXoC8CN85cYNe24ASTYdxHzXGAyn54Lyz4F\n"
        "kYiPyTrmIfLwV5MstaBHyGLv/NfMOztaqTZUaf4kbT/JzKreBXzdMY09nxBwarv+\n"
        "Ek8YacD80EPjEVogT+pie6+qGcgrNyUtvmWhEoolD2Oj91Qc+SHJ1hXzUqxuQzIH\n"
        "/YIX+OVnbA1R9r3xUse958Qw/CAxCYgdlSkaTdUdAqXxgOADtFv0sd3IV+5lScdS\n"
        "VLa0AygS/5DW8AiPfriXxas3LOR65Kh343agANBqP8HSNorgQRKoNWobats14dQc\n"
        "BOSoRQTIWjM4bk0cDWK3CqKM09VUP0bNHFWmcNsSOoeTdZ+n0qA=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Microsoft Root Certificate Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIFmTCCA4GgAwIBAgIQea0WoUqgpa1Mc1j0BxMuZTANBgkqhkiG9w0BAQUFADBf\n"
        "MRMwEQYKCZImiZPyLGQBGRYDY29tMRkwFwYKCZImiZPyLGQBGRYJbWljcm9zb2Z0\n"
        "MS0wKwYDVQQDEyRNaWNyb3NvZnQgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkw\n"
        "HhcNMDEwNTA5MjMxOTIyWhcNMjEwNTA5MjMyODEzWjBfMRMwEQYKCZImiZPyLGQB\n"
        "GRYDY29tMRkwFwYKCZImiZPyLGQBGRYJbWljcm9zb2Z0MS0wKwYDVQQDEyRNaWNy\n"
        "b3NvZnQgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEB\n"
        "AQUAA4ICDwAwggIKAoICAQDzXfqAZ9Rap6kMLJAg0DUIPHWEzbcHiZyJ2t7Ow2D6\n"
        "kWhanpRxKRh2fMLgyCV2lA5Y+gQ0Nubfr/eAuulYCyuT5Z0F43cikfc0ZDwikR1e\n"
        "4QmQvBT+/HVYGeF5tweSo66IWQjYnwfKA1j8aCltMtfSqMtL/OELSDJP5uu4rU/k\n"
        "XG8TlJnbldV126gat5SRtHdb9UgMj2p5fRRwBH1tr5D12nDYR7e/my9s5wW34RFg\n"
        "rHmRFHzF1qbk4X7Vw37lktI8ALU2gt554W3ztW74nzPJy1J9c5g224uha6KVl5uj\n"
        "3sJNJv8GlmclBsjnrOTuEjOVMZnINQhONMp5U9W1vmMyWUA2wKVOBE0921sHM+RY\n"
        "v+8/U2TYQlk1V/0PRXwkBE2e1jh0EZcikM5oRHSSb9VLb7CG48c2QqDQ/MHAWvmj\n"
        "YbkwR3GWChawkcBCle8Qfyhq4yofseTNAz93cQTHIPxJDx1FiKTXy36IrY4t7EXb\n"
        "xFEEySr87IaemhGXW97OU4jm4rf9rJXCKEDb7wSQ34EzOdmyRaUjhwalVYkxuwYt\n"
        "YA5BGH0fLrWXyxHrFdUkpZTvFRSJ/Utz+jJb/NEzAPlZYnAHMuouq0Ate8rdIWcb\n"
        "MJmPFqojqEHRsG4RmzbE3kB0nOFYZcFgHnpbOMiPuwQmfNQWQOW2a2yqhv0Av87B\n"
        "NQIDAQABo1EwTzALBgNVHQ8EBAMCAcYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n"
        "FgQUDqyCYEBWJ5flJRP8KuEKU5VZ5KQwEAYJKwYBBAGCNxUBBAMCAQAwDQYJKoZI\n"
        "hvcNAQEFBQADggIBAMURTQM6YN1dUhF3j7K7NsiyBb+0t6jYIJ1cEwO2HCL6BhM1\n"
        "tshj1JpHbyZX0lXxBLEmX9apUGigvNK4bszD6azfGc14rFl0rGY0NsQbPmw4TDMO\n"
        "MBINoyb+UVMA/69aToQNDx/kbQUuToVLjWwzb1TSZKu/UK99ejmgN+1jAw/8EwbO\n"
        "FjbUVDuVG1FiOuVNF9QFOZKaJ6hbqr3su77jIIlgcWxWs6UT0G0OI36VA+1oPfLY\n"
        "Y7hrTbboMLXhypRL96KqXZkwsj2nwlFsKCABJCcrSwC3nRFrcL6yEIK8DJto0I07\n"
        "JIeqmShynTNfWZC99d6TnjpiWjQ54ohVHbkGsMGJay3XacMZEjaE0Mmg2v8vaXiy\n"
        "5Xra69cMwPe9Yxe4ORM4ojZbe/KFVmodZGLBOOKqv1FmopT1EpxmIhBr8rcwki3y\n"
        "KfA9OxRDaKLxnCk3y844ICVtfGfzfiQSJAMIgUfspZ6X9RjXz7vV73aW7/3O21ad\n"
        "laBC+ZdY4dcxItNfWeY+biIA6kOEtiXb2fMIVmjAZGsdfOy2k6JiV24u2OdYj8Qx\n"
        "SSbd3ik1h/UwcXBbFDxpvYkSfesuo/7Yf56CWlIKK8FDK9kwiJ/IEPuJjeahhXUz\n"
        "fmye23MTZGJppS99ypZtn/gETTCSPW4hFCHJPeDD/YprnUr90aGdmUN3P7Da\n"
        "-----END CERTIFICATE-----\n",

        // ./root/NetLock Expressz (Class C) Tanusitvanykiado.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIFTzCCBLigAwIBAgIBaDANBgkqhkiG9w0BAQQFADCBmzELMAkGA1UEBhMCSFUx\n"
        "ETAPBgNVBAcTCEJ1ZGFwZXN0MScwJQYDVQQKEx5OZXRMb2NrIEhhbG96YXRiaXp0\n"
        "b25zYWdpIEtmdC4xGjAYBgNVBAsTEVRhbnVzaXR2YW55a2lhZG9rMTQwMgYDVQQD\n"
        "EytOZXRMb2NrIEV4cHJlc3N6IChDbGFzcyBDKSBUYW51c2l0dmFueWtpYWRvMB4X\n"
        "DTk5MDIyNTE0MDgxMVoXDTE5MDIyMDE0MDgxMVowgZsxCzAJBgNVBAYTAkhVMREw\n"
        "DwYDVQQHEwhCdWRhcGVzdDEnMCUGA1UEChMeTmV0TG9jayBIYWxvemF0Yml6dG9u\n"
        "c2FnaSBLZnQuMRowGAYDVQQLExFUYW51c2l0dmFueWtpYWRvazE0MDIGA1UEAxMr\n"
        "TmV0TG9jayBFeHByZXNzeiAoQ2xhc3MgQykgVGFudXNpdHZhbnlraWFkbzCBnzAN\n"
        "BgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEA6+ywbGGKIyWvYCDj2Z/8kwvbXY2wobNA\n"
        "OoLO/XXgeDIDhlqGlZHtU/qdQPzm6N3ZW3oDvV3zOwzDUXmbrVWg6dADEK8KuhRC\n"
        "2VImESLH0iDMgqSaqf64gXadarfSNnU+sYYJ9m5tfk63euyucYT2BDMIJTLrdKwW\n"
        "RMbkQJMdf60CAwEAAaOCAp8wggKbMBIGA1UdEwEB/wQIMAYBAf8CAQQwDgYDVR0P\n"
        "AQH/BAQDAgAGMBEGCWCGSAGG+EIBAQQEAwIABzCCAmAGCWCGSAGG+EIBDQSCAlEW\n"
        "ggJNRklHWUVMRU0hIEV6ZW4gdGFudXNpdHZhbnkgYSBOZXRMb2NrIEtmdC4gQWx0\n"
        "YWxhbm9zIFN6b2xnYWx0YXRhc2kgRmVsdGV0ZWxlaWJlbiBsZWlydCBlbGphcmFz\n"
        "b2sgYWxhcGphbiBrZXN6dWx0LiBBIGhpdGVsZXNpdGVzIGZvbHlhbWF0YXQgYSBO\n"
        "ZXRMb2NrIEtmdC4gdGVybWVrZmVsZWxvc3NlZy1iaXp0b3NpdGFzYSB2ZWRpLiBB\n"
        "IGRpZ2l0YWxpcyBhbGFpcmFzIGVsZm9nYWRhc2FuYWsgZmVsdGV0ZWxlIGF6IGVs\n"
        "b2lydCBlbGxlbm9yemVzaSBlbGphcmFzIG1lZ3RldGVsZS4gQXogZWxqYXJhcyBs\n"
        "ZWlyYXNhIG1lZ3RhbGFsaGF0byBhIE5ldExvY2sgS2Z0LiBJbnRlcm5ldCBob25s\n"
        "YXBqYW4gYSBodHRwczovL3d3dy5uZXRsb2NrLm5ldC9kb2NzIGNpbWVuIHZhZ3kg\n"
        "a2VyaGV0byBheiBlbGxlbm9yemVzQG5ldGxvY2submV0IGUtbWFpbCBjaW1lbi4g\n"
        "SU1QT1JUQU5UISBUaGUgaXNzdWFuY2UgYW5kIHRoZSB1c2Ugb2YgdGhpcyBjZXJ0\n"
        "aWZpY2F0ZSBpcyBzdWJqZWN0IHRvIHRoZSBOZXRMb2NrIENQUyBhdmFpbGFibGUg\n"
        "YXQgaHR0cHM6Ly93d3cubmV0bG9jay5uZXQvZG9jcyBvciBieSBlLW1haWwgYXQg\n"
        "Y3BzQG5ldGxvY2submV0LjANBgkqhkiG9w0BAQQFAAOBgQAQrX/XDDKACtiG8XmY\n"
        "ta3UzbM2xJZIwVzNmtkFLp++UOv0JhQQLdRmF/iewSf98e3ke0ugbLWrmldwpu2g\n"
        "pO0u9f38vf5NNwgMvOOWgyL1SRt/Syu0VMGAfJlOHdCM7tCs5ZL6dVb+ZKATj7i4\n"
        "Fp1hBWeAyNDYpQcCNJgEjTME1A==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/NetLock Kozjegyzoi (Class A) Tanusitvanykiado.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIGfTCCBWWgAwIBAgICAQMwDQYJKoZIhvcNAQEEBQAwga8xCzAJBgNVBAYTAkhV\n"
        "MRAwDgYDVQQIEwdIdW5nYXJ5MREwDwYDVQQHEwhCdWRhcGVzdDEnMCUGA1UEChMe\n"
        "TmV0TG9jayBIYWxvemF0Yml6dG9uc2FnaSBLZnQuMRowGAYDVQQLExFUYW51c2l0\n"
        "dmFueWtpYWRvazE2MDQGA1UEAxMtTmV0TG9jayBLb3pqZWd5em9pIChDbGFzcyBB\n"
        "KSBUYW51c2l0dmFueWtpYWRvMB4XDTk5MDIyNDIzMTQ0N1oXDTE5MDIxOTIzMTQ0\n"
        "N1owga8xCzAJBgNVBAYTAkhVMRAwDgYDVQQIEwdIdW5nYXJ5MREwDwYDVQQHEwhC\n"
        "dWRhcGVzdDEnMCUGA1UEChMeTmV0TG9jayBIYWxvemF0Yml6dG9uc2FnaSBLZnQu\n"
        "MRowGAYDVQQLExFUYW51c2l0dmFueWtpYWRvazE2MDQGA1UEAxMtTmV0TG9jayBL\n"
        "b3pqZWd5em9pIChDbGFzcyBBKSBUYW51c2l0dmFueWtpYWRvMIIBIjANBgkqhkiG\n"
        "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvHSMD7tM9DceqQWC2ObhbHDqeLVu0ThEDaiD\n"
        "zl3S1tWBxdRL51uUcCbbO51qTGL3cfNk1mE7PetzozfZz+qMkjvN9wfcZnSX9EUi\n"
        "3fRc4L9t875lM+QVOr/bmJBVOMTtplVjC7B4BPTjbsE/jvxReB+SnoPC/tmwqcm8\n"
        "WgD/qaiYdPv2LD4VOQ22BFWoDpggQrOxJa1+mm9dU7GrDPzr4PN6s6iz/0b2Y6LY\n"
        "Oph7tqyF/7AlT3Rj5xMHpQqPBffAZG9+pyeAlt7ULoZgx2srXnN7F+eRP2QM2Esi\n"
        "NCubMvJIH5+hCoR64sKtlz2O1cH5VqNQ6ca0+pii7pXmKgOM3wIDAQABo4ICnzCC\n"
        "ApswDgYDVR0PAQH/BAQDAgAGMBIGA1UdEwEB/wQIMAYBAf8CAQQwEQYJYIZIAYb4\n"
        "QgEBBAQDAgAHMIICYAYJYIZIAYb4QgENBIICURaCAk1GSUdZRUxFTSEgRXplbiB0\n"
        "YW51c2l0dmFueSBhIE5ldExvY2sgS2Z0LiBBbHRhbGFub3MgU3pvbGdhbHRhdGFz\n"
        "aSBGZWx0ZXRlbGVpYmVuIGxlaXJ0IGVsamFyYXNvayBhbGFwamFuIGtlc3p1bHQu\n"
        "IEEgaGl0ZWxlc2l0ZXMgZm9seWFtYXRhdCBhIE5ldExvY2sgS2Z0LiB0ZXJtZWtm\n"
        "ZWxlbG9zc2VnLWJpenRvc2l0YXNhIHZlZGkuIEEgZGlnaXRhbGlzIGFsYWlyYXMg\n"
        "ZWxmb2dhZGFzYW5hayBmZWx0ZXRlbGUgYXogZWxvaXJ0IGVsbGVub3J6ZXNpIGVs\n"
        "amFyYXMgbWVndGV0ZWxlLiBBeiBlbGphcmFzIGxlaXJhc2EgbWVndGFsYWxoYXRv\n"
        "IGEgTmV0TG9jayBLZnQuIEludGVybmV0IGhvbmxhcGphbiBhIGh0dHBzOi8vd3d3\n"
        "Lm5ldGxvY2submV0L2RvY3MgY2ltZW4gdmFneSBrZXJoZXRvIGF6IGVsbGVub3J6\n"
        "ZXNAbmV0bG9jay5uZXQgZS1tYWlsIGNpbWVuLiBJTVBPUlRBTlQhIFRoZSBpc3N1\n"
        "YW5jZSBhbmQgdGhlIHVzZSBvZiB0aGlzIGNlcnRpZmljYXRlIGlzIHN1YmplY3Qg\n"
        "dG8gdGhlIE5ldExvY2sgQ1BTIGF2YWlsYWJsZSBhdCBodHRwczovL3d3dy5uZXRs\n"
        "b2NrLm5ldC9kb2NzIG9yIGJ5IGUtbWFpbCBhdCBjcHNAbmV0bG9jay5uZXQuMA0G\n"
        "CSqGSIb3DQEBBAUAA4IBAQBIJEb3ulZv+sgoA0BO5TE5ayZrU3/b39/zcT0mwBQO\n"
        "xmd7I6gMc90Bu8bKbjc5VdXHjFYgDigKDtIqpLBJUsY4B/6+CgmM0ZjPytoUMaFP\n"
        "0jn8DxEsQ8Pdq5PHVT5HfBgaANzze9jyf1JsIPQLX2lS9O74silg6+NJMSEN1rUQ\n"
        "QeJBCWziGppWS3cC9qCbmieH6FUpccKQn0V4GuEVZD3QDtigdp+uxdAu6tYPVuxk\n"
        "f1qbFFgBJ34TUMdrKuZoPL9coAob4Q566eKAw+np9v1sEZ7Q5SgnK1QyQhSCdeZK\n"
        "8CtmdWOMovsEPoMOmzbwGOQmIMOM8CgHrTwXZoi1/baI\n"
        "-----END CERTIFICATE-----\n",

        // ./root/NetLock Uzleti (Class B) Tanusitvanykiado.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIFSzCCBLSgAwIBAgIBaTANBgkqhkiG9w0BAQQFADCBmTELMAkGA1UEBhMCSFUx\n"
        "ETAPBgNVBAcTCEJ1ZGFwZXN0MScwJQYDVQQKEx5OZXRMb2NrIEhhbG96YXRiaXp0\n"
        "b25zYWdpIEtmdC4xGjAYBgNVBAsTEVRhbnVzaXR2YW55a2lhZG9rMTIwMAYDVQQD\n"
        "EylOZXRMb2NrIFV6bGV0aSAoQ2xhc3MgQikgVGFudXNpdHZhbnlraWFkbzAeFw05\n"
        "OTAyMjUxNDEwMjJaFw0xOTAyMjAxNDEwMjJaMIGZMQswCQYDVQQGEwJIVTERMA8G\n"
        "A1UEBxMIQnVkYXBlc3QxJzAlBgNVBAoTHk5ldExvY2sgSGFsb3phdGJpenRvbnNh\n"
        "Z2kgS2Z0LjEaMBgGA1UECxMRVGFudXNpdHZhbnlraWFkb2sxMjAwBgNVBAMTKU5l\n"
        "dExvY2sgVXpsZXRpIChDbGFzcyBCKSBUYW51c2l0dmFueWtpYWRvMIGfMA0GCSqG\n"
        "SIb3DQEBAQUAA4GNADCBiQKBgQCx6gTsIKAjwo84YM/HRrPVG/77uZmeBNwcf4xK\n"
        "gZjupNTKihe5In+DCnVMm8Bp2GQ5o+2So/1bXHQawEfKOml2mrriRBf8TKPV/riX\n"
        "iK+IA4kfpPIEPsgHC+b5sy96YhQJRhTKZPWLgLViqNhr1nGTLbO/CVRY7QbrqHvc\n"
        "Q7GhaQIDAQABo4ICnzCCApswEgYDVR0TAQH/BAgwBgEB/wIBBDAOBgNVHQ8BAf8E\n"
        "BAMCAAYwEQYJYIZIAYb4QgEBBAQDAgAHMIICYAYJYIZIAYb4QgENBIICURaCAk1G\n"
        "SUdZRUxFTSEgRXplbiB0YW51c2l0dmFueSBhIE5ldExvY2sgS2Z0LiBBbHRhbGFu\n"
        "b3MgU3pvbGdhbHRhdGFzaSBGZWx0ZXRlbGVpYmVuIGxlaXJ0IGVsamFyYXNvayBh\n"
        "bGFwamFuIGtlc3p1bHQuIEEgaGl0ZWxlc2l0ZXMgZm9seWFtYXRhdCBhIE5ldExv\n"
        "Y2sgS2Z0LiB0ZXJtZWtmZWxlbG9zc2VnLWJpenRvc2l0YXNhIHZlZGkuIEEgZGln\n"
        "aXRhbGlzIGFsYWlyYXMgZWxmb2dhZGFzYW5hayBmZWx0ZXRlbGUgYXogZWxvaXJ0\n"
        "IGVsbGVub3J6ZXNpIGVsamFyYXMgbWVndGV0ZWxlLiBBeiBlbGphcmFzIGxlaXJh\n"
        "c2EgbWVndGFsYWxoYXRvIGEgTmV0TG9jayBLZnQuIEludGVybmV0IGhvbmxhcGph\n"
        "biBhIGh0dHBzOi8vd3d3Lm5ldGxvY2submV0L2RvY3MgY2ltZW4gdmFneSBrZXJo\n"
        "ZXRvIGF6IGVsbGVub3J6ZXNAbmV0bG9jay5uZXQgZS1tYWlsIGNpbWVuLiBJTVBP\n"
        "UlRBTlQhIFRoZSBpc3N1YW5jZSBhbmQgdGhlIHVzZSBvZiB0aGlzIGNlcnRpZmlj\n"
        "YXRlIGlzIHN1YmplY3QgdG8gdGhlIE5ldExvY2sgQ1BTIGF2YWlsYWJsZSBhdCBo\n"
        "dHRwczovL3d3dy5uZXRsb2NrLm5ldC9kb2NzIG9yIGJ5IGUtbWFpbCBhdCBjcHNA\n"
        "bmV0bG9jay5uZXQuMA0GCSqGSIb3DQEBBAUAA4GBAATbrowXr/gOkDFOzT4JwG06\n"
        "sPgzTEdM43WIEJessDgVkcYplswhwG08pXTP2IKlOcNl40JwuyKQ433bNXbhoLXa\n"
        "n3BukxowOR0w2y7jfLKRstE3Kfq51hdcR0/jHTjrn9V7lagonhVK0dHQKwCXoOKS\n"
        "NitjrFgBazMpUIaD8QFI\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Saunalahden Serveri CA .cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEfzCCA2egAwIBAgIBADANBgkqhkiG9w0BAQQFADCBizELMAkGA1UEBhMCRkkx\n"
        "ETAPBgNVBAcTCEhlbHNpbmtpMR8wHQYDVQQKExZTYXVuYWxhaGRlbiBTZXJ2ZXJp\n"
        "IE95MR8wHQYDVQQDExZTYXVuYWxhaGRlbiBTZXJ2ZXJpIENBMScwJQYJKoZIhvcN\n"
        "AQkBFhhnb2xkLWNlcnRzQHNhdW5hbGFodGkuZmkwHhcNOTkwNzAxMDQ1NzA4WhcN\n"
        "MTkwNjI2MDQ1NzA4WjCBizELMAkGA1UEBhMCRkkxETAPBgNVBAcTCEhlbHNpbmtp\n"
        "MR8wHQYDVQQKExZTYXVuYWxhaGRlbiBTZXJ2ZXJpIE95MR8wHQYDVQQDExZTYXVu\n"
        "YWxhaGRlbiBTZXJ2ZXJpIENBMScwJQYJKoZIhvcNAQkBFhhnb2xkLWNlcnRzQHNh\n"
        "dW5hbGFodGkuZmkwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDnBCnc\n"
        "1uBS09ZPAK8cgYi/PFRy8555Y/3LYNoMfHIvyhup4hBpZ3o5n8KgJcyDg42DERy4\n"
        "/efbqYijDJIhV8Gq4uozfkpmA7xzjdIcKf3Hf0Ptj+tXe6FBv01zMUo3KLQYV1g8\n"
        "aHBQk7oQHXjNk8PCgu+43w8MWNUwV0M6FCUqOFkGtETN28leRvJLuBxDQYHSPkOy\n"
        "riGeG0qAfKbKs/QmqS4s+Lf3PBgPmzJvEQlzxM0v6NPsSTbzgY3Ywv/QGm3d2BS/\n"
        "MtDpww0+CK/t2kudas8Ti5CCafrXZyNhS1v9pYBkHXr3pdPadxx4u1H750aAkHFx\n"
        "thb4Q7QZvRir3nZ7AgMBAAGjgeswgegwHQYDVR0OBBYEFGTG3PcyaAuybOpKy15u\n"
        "U0N0S/6ZMIG4BgNVHSMEgbAwga2AFGTG3PcyaAuybOpKy15uU0N0S/6ZoYGRpIGO\n"
        "MIGLMQswCQYDVQQGEwJGSTERMA8GA1UEBxMISGVsc2lua2kxHzAdBgNVBAoTFlNh\n"
        "dW5hbGFoZGVuIFNlcnZlcmkgT3kxHzAdBgNVBAMTFlNhdW5hbGFoZGVuIFNlcnZl\n"
        "cmkgQ0ExJzAlBgkqhkiG9w0BCQEWGGdvbGQtY2VydHNAc2F1bmFsYWh0aS5maYIB\n"
        "ADAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBBAUAA4IBAQAX7dGjfePOY7y5Gpz2\n"
        "UaJg9h1huIfSeFrHyNJfGO+yXd1NnVjk6lrHIMe0+u7GzsMvv/qb3mgMuptHpOxZ\n"
        "hBHfsa7+OPvugYEfEnTeBUqrRjjxFjqyyS/4f5xsU4midBiS3jmNCBdT+9mELsPK\n"
        "fQ9CT6dG3wr0QlkIglEJMix2UEtOtQkfgl1oRXlbKKwl6Q0851USpcNC9nS7+uA8\n"
        "zeiiaOjNg7fROje/sIgMTKVGHuXkdDC4ELIN1+CjsfS2XAL56UZqdIkmpwT9kJ1+\n"
        "1NhaT27CuOYmWojl8yaFSLvwyW92aLns0LlG3F02T7BEH6NexNciIXbskjS/yNkf\n"
        "aHki\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Saunalahden Serveri CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEhTCCA22gAwIBAgIBADANBgkqhkiG9w0BAQQFADCBjTELMAkGA1UEBhMCRkkx\n"
        "ETAPBgNVBAcTCEhlbHNpbmtpMR8wHQYDVQQKExZTYXVuYWxhaGRlbiBTZXJ2ZXJp\n"
        "IE95MR8wHQYDVQQDExZTYXVuYWxhaGRlbiBTZXJ2ZXJpIENBMSkwJwYJKoZIhvcN\n"
        "AQkBFhpzaWx2ZXItY2VydHNAc2F1bmFsYWh0aS5maTAeFw05OTA3MDEwNDU1MDNa\n"
        "Fw0xOTA2MjYwNDU1MDNaMIGNMQswCQYDVQQGEwJGSTERMA8GA1UEBxMISGVsc2lu\n"
        "a2kxHzAdBgNVBAoTFlNhdW5hbGFoZGVuIFNlcnZlcmkgT3kxHzAdBgNVBAMTFlNh\n"
        "dW5hbGFoZGVuIFNlcnZlcmkgQ0ExKTAnBgkqhkiG9w0BCQEWGnNpbHZlci1jZXJ0\n"
        "c0BzYXVuYWxhaHRpLmZpMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA\n"
        "0neMvIdsNk5TqmhgRbgjd2fj7kmC5mx/XpJdtRxhUteoYEsW+ut5fp1MkulXe16G\n"
        "MKkoPH030SHidhoZw++q2u74AZ4aOSovk3UDZj9uKU2NhGOMpx8VlLQ0SbTk00Gr\n"
        "uvvEXLWecvUoyjKCY0zHRPi0HcSKCldVkK8wiVQOp2gm00AHIrPOPKP7mNckPN58\n"
        "gkm0NIx9JNtkbmSy6f+GyKx+q1Pk0kH0EYTuR0wIHUTmVk0AfNqJQjnveAjRhea+\n"
        "XJ4zuTX/HM70g7XyZMUxSKm0rMXYPIwabab/Qq3z+EvOrNrFirAPAyPB9fPHWX8w\n"
        "8d9mHVoxBaJGHTnkVbOtDwIDAQABo4HtMIHqMB0GA1UdDgQWBBTRsV9BeXN5IFjJ\n"
        "L6KoR8dExmOEKDCBugYDVR0jBIGyMIGvgBTRsV9BeXN5IFjJL6KoR8dExmOEKKGB\n"
        "k6SBkDCBjTELMAkGA1UEBhMCRkkxETAPBgNVBAcTCEhlbHNpbmtpMR8wHQYDVQQK\n"
        "ExZTYXVuYWxhaGRlbiBTZXJ2ZXJpIE95MR8wHQYDVQQDExZTYXVuYWxhaGRlbiBT\n"
        "ZXJ2ZXJpIENBMSkwJwYJKoZIhvcNAQkBFhpzaWx2ZXItY2VydHNAc2F1bmFsYWh0\n"
        "aS5maYIBADAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBBAUAA4IBAQBfV7bHhYLg\n"
        "oISRnUCzjbA5//QSGYud2gRiRN5kTY0R3nlSy20Y8Z26B62GCaQaaqrClA5Z7BeK\n"
        "L0zjcc5wVp9bM0EUUFr2sntBZBntCBbWxhQcfEJCn+3RWQnUr+tHzNuveRS6PHgF\n"
        "ELPX9w550KHNPEsyyM8PAYSeoEsw0MYAV0lXJvOvm7rXYKQYPrL18oh9/l0HYsmI\n"
        "qawim5hYs7s2wUMKOI+K+7zTjuALtdwDCfTMNdDFcs3dFlEjQg/c4lizQRs0hnqI\n"
        "NIRkkAuuykmNtTkII11BtIv3GNCyS4/v712ex853T8iCQTDS/OZRYPxvaPT/lNuL\n"
        "YNGN02KKB2WR\n"
        "-----END CERTIFICATE-----\n",

        // ./root/SECOM Trust Systems CO LTD.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDWjCCAkKgAwIBAgIBADANBgkqhkiG9w0BAQUFADBQMQswCQYDVQQGEwJKUDEY\n"
        "MBYGA1UEChMPU0VDT00gVHJ1c3QubmV0MScwJQYDVQQLEx5TZWN1cml0eSBDb21t\n"
        "dW5pY2F0aW9uIFJvb3RDQTEwHhcNMDMwOTMwMDQyMDQ5WhcNMjMwOTMwMDQyMDQ5\n"
        "WjBQMQswCQYDVQQGEwJKUDEYMBYGA1UEChMPU0VDT00gVHJ1c3QubmV0MScwJQYD\n"
        "VQQLEx5TZWN1cml0eSBDb21tdW5pY2F0aW9uIFJvb3RDQTEwggEiMA0GCSqGSIb3\n"
        "DQEBAQUAA4IBDwAwggEKAoIBAQCzs/5/022x7xZ8V6UMbXaKL0u/ZPtM7orw8yl8\n"
        "9f/uKuDp6bpbZCKamm8sOiZpUQWZJtzVHGpxxpp9Hp3dfGzGjGdnSj74cbAZJ6kJ\n"
        "DKaVv0uMDPpVmDvY6CKhS3E4eayXkmmziX7qIWgGmBSWh9JhNrxtJ1aeV+7AwFb9\n"
        "Ms+k2Y7CI9eNqPPYJayX5HA49LY6tJ07lyZDo6G8SVlyTCMwhwFY9k6+HGhWZq/N\n"
        "QV3Is00qVUarH9oe4kA92819uZKAnDfdDJZkndwi92SL32HeFZRSFaB9UslLqCHJ\n"
        "xrHty8OVYNEP8Ktw+N/LTX7s1vqr2b1/VPKl6Xn62dZ2JChzAgMBAAGjPzA9MB0G\n"
        "A1UdDgQWBBSgc0mZaNyFW2XjmygvV5+9M7wHSDALBgNVHQ8EBAMCAQYwDwYDVR0T\n"
        "AQH/BAUwAwEB/zANBgkqhkiG9w0BAQUFAAOCAQEAaECpqLvkT115swW1F7NgE+vG\n"
        "kl3g0dNq/vu+m22/xwVtWSDEHPC32oRYAmP6SBbvT6UL90qY8j+eG61Ha2POCEfr\n"
        "Uj94nK9NrvjVT8+amCoQQTlSxN3Zmw7vkwGusi7KaEIkQmywszo+zenaSMQVy+n5\n"
        "Bw+SUEmK3TGXX8npN6o7WWWXlDLJs58+OmJYxUmtYg5xpTKqL8aJdkNAExNnPaJU\n"
        "JRDL8Try2frbSVa7pv6nQTXD4IhhyYjH3zYQIphZ6rBK+1YWc26sTfcioU+tHXot\n"
        "RSflMMFe8toTyyVCUZVHA4xsIcx0Qu1T/zOLjw9XARYvz6buyXAiFL39vmwLAw==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Societa Interbancaria per l'Automazione SIA Secure Client CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICyDCCAjGgAwIBAgIBMDANBgkqhkiG9w0BAQUFADBSMQswCQYDVQQGEwJJVDET\n"
        "MBEGA1UEChMKU0lBIFMucC5BLjEPMA0GA1UEBxMGTWlsYW5vMR0wGwYDVQQDExRT\n"
        "SUEgU2VjdXJlIENsaWVudCBDQTAeFw05OTA3MDgxMjQ1MDBaFw0xOTA3MDgyMzU5\n"
        "MDBaMFIxCzAJBgNVBAYTAklUMRMwEQYDVQQKEwpTSUEgUy5wLkEuMQ8wDQYDVQQH\n"
        "EwZNaWxhbm8xHTAbBgNVBAMTFFNJQSBTZWN1cmUgQ2xpZW50IENBMIGfMA0GCSqG\n"
        "SIb3DQEBAQUAA4GNADCBiQKBgQDS/LBAYGpmY1Jm5mkJcY2BmB4dHfPgSQ3IK2/Q\n"
        "d1FFxZ1uo1xw3hV4Fh5f4MJi9H0yQ3cI19/S9X83glLGfpOd8U1naMIvwiWIHXHm\n"
        "2ArQeORRQjlVBvOAYv6WpW3FRsdB5QASm2bB4o2VPtXHDFj3yGCknHhxlYzeegm/\n"
        "HNX8owIDAQABo4GtMIGqMDsGA1UdHwQ0MDIwMKAuoCyGKmh0dHA6Ly9jYS5zaWEu\n"
        "aXQvc2VjY2xpL3JlcG9zaXRvcnkvQ1JMLmRlcjBKBgNVHSAEQzBBMD8GBiuBBwEB\n"
        "AzA1MDMGCCsGAQUFBwIBFidodHRwczovL2NhLnNpYS5pdC9zZWNjbGkvcmVwb3Np\n"
        "dG9yeS9DUFMwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwDQYJKoZI\n"
        "hvcNAQEFBQADgYEAgwYCx20JM9pE1pHpf5g617koCBnXohH9HxL+hEoLc6ez1NtD\n"
        "er4kFKPWAIo6LRy4rYv815q2GEHk2SVW74HIFKRJDF13cBYFZyjhnt8a962rLlbu\n"
        "1yId7eh3Yb/cZ3AhkoN2dLHpA3KwKIh8wGTMxqw3Ea5NbH8xQGGQt9VSyEM=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Societa Interbancaria per l'Automazione SIA Secure Server CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDzTCCArWgAwIBAgIBMTANBgkqhkiG9w0BAQUFADBSMQswCQYDVQQGEwJJVDET\n"
        "MBEGA1UEChMKU0lBIFMucC5BLjEPMA0GA1UEBxMGTWlsYW5vMR0wGwYDVQQDExRT\n"
        "SUEgU2VjdXJlIFNlcnZlciBDQTAeFw05OTA3MDgxMzEzMDBaFw0xOTA3MDgyMzU5\n"
        "MDBaMFIxCzAJBgNVBAYTAklUMRMwEQYDVQQKEwpTSUEgUy5wLkEuMQ8wDQYDVQQH\n"
        "EwZNaWxhbm8xHTAbBgNVBAMTFFNJQSBTZWN1cmUgU2VydmVyIENBMIIBIjANBgkq\n"
        "hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA28ELzCfTEiIuuWQWdKxZJ+IqkACSntWY\n"
        "XCtRbhsTb1RvShCihethC+ztnH7Of2WTbsxsQZzILarGs5v7THCcEXXzcom6iQCt\n"
        "xy5J53PagLIs/vKXmfQCGzQvOaqL5u8F/Ln1ulR/ob+OHkg2Mwl0Yac9x5skx8OJ\n"
        "zcpOKDEjBhxiFY7fTxtrLUri9LDczvOQ/XmBE8E+Lma8+SJNCy9iM42oK+rpb3On\n"
        "N5QEL+leTQ3p7XwyP3lK5jp2KSBQ84+CRHJsMDRIWKpdGz8B6yHs6n6oK4Rd9sEx\n"
        "lU8pe7U1t/60BlewFNfyVVmMupu5MT/lqqrvJXCVkjZB8VWfwQhEnQIDAQABo4Gt\n"
        "MIGqMDsGA1UdHwQ0MDIwMKAuoCyGKmh0dHA6Ly9jYS5zaWEuaXQvc2Vjc3J2L3Jl\n"
        "cG9zaXRvcnkvQ1JMLmRlcjBKBgNVHSAEQzBBMD8GBiuBBwEBAjA1MDMGCCsGAQUF\n"
        "BwIBFidodHRwczovL2NhLnNpYS5pdC9zZWNzcnYvcmVwb3NpdG9yeS9DUFMwDwYD\n"
        "VR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwDQYJKoZIhvcNAQEFBQADggEB\n"
        "AELhiyLoXBNRRduRw5CUv9Uh8RDgQjq3rmIlg+zh5caYbM7WbdbYCi74ug9utb7B\n"
        "jTJGkYiYcxXJqulbwPG4O4JUaTtRGPNtWUVkhqAiy7T+47VZGABuFoMy/KfT4tMz\n"
        "s6ufphZo3fH1rxfT4uwIMHudvCCqt2JmH8d6YA3vAB+NeOB4BJRgeF1aP6TagT1f\n"
        "ZBaCL6nwOJ/bNG9YgCIqXpNTlnRPvoM3MjmuNbC71M9IaB7KxOxtuNWhCOz7CfnD\n"
        "uTAxD0NOCZta1ArzGbHDGhCzTofJp3Pk2Qt76yc/WNJ0DinIpxRIV2wqlTX9iPbm\n"
        "E/02WooRmzqWJkOnuY45sfA=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/SRCA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICmjCCAgOgAwIBAgIIbyZr5/jKH6QwDQYJKoZIhvcNAQEFBQAwRzELMAkGA1UE\n"
        "BhMCQ04xKTAnBgNVBAoTIFNpbm9yYWlsIENlcnRpZmljYXRpb24gQXV0aG9yaXR5\n"
        "MQ0wCwYDVQQDEwRTUkNBMB4XDTA5MDUyNTA2NTYwMFoXDTI5MDUyMDA2NTYwMFow\n"
        "RzELMAkGA1UEBhMCQ04xKTAnBgNVBAoTIFNpbm9yYWlsIENlcnRpZmljYXRpb24g\n"
        "QXV0aG9yaXR5MQ0wCwYDVQQDEwRTUkNBMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCB\n"
        "iQKBgQDMpbNeb34p0GvLkZ6t72/OOba4mX2K/eZRWFfnuk8e5jKDH+9BgCb29bSo\n"
        "tqPqTbxXWPxIOz8EjyUO3bfR5pQ8ovNTOlks2rS5BdMhoi4sUjCKi5ELiqtyww/X\n"
        "gY5iFqv6D4Pw9QvOUcdRVSbPWo1DwMmH75It6pk/rARIFHEjWwIDAQABo4GOMIGL\n"
        "MB8GA1UdIwQYMBaAFHletne34lKDQ+3HUYhMY4UsAENYMAwGA1UdEwQFMAMBAf8w\n"
        "LgYDVR0fBCcwJTAjoCGgH4YdaHR0cDovLzE5Mi4xNjguOS4xNDkvY3JsMS5jcmww\n"
        "CwYDVR0PBAQDAgH+MB0GA1UdDgQWBBR5XrZ3t+JSg0Ptx1GITGOFLABDWDANBgkq\n"
        "hkiG9w0BAQUFAAOBgQDGrAm2U/of1LbOnG2bnnQtgcVaBXiVJF8LKPaV23XQ96HU\n"
        "8xfgSZMJS6U00WHAI7zp0q208RSUft9wDq9ee///VOhzR6Tebg9QfyPSohkBrhXQ\n"
        "envQog555S+C3eJAAVeNCTeMS3N/M5hzBRJAoffn3qoYdAO1Q8bTguOi+2849A==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/StartCom Certification Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIHyTCCBbGgAwIBAgIBATANBgkqhkiG9w0BAQUFADB9MQswCQYDVQQGEwJJTDEW\n"
        "MBQGA1UEChMNU3RhcnRDb20gTHRkLjErMCkGA1UECxMiU2VjdXJlIERpZ2l0YWwg\n"
        "Q2VydGlmaWNhdGUgU2lnbmluZzEpMCcGA1UEAxMgU3RhcnRDb20gQ2VydGlmaWNh\n"
        "dGlvbiBBdXRob3JpdHkwHhcNMDYwOTE3MTk0NjM2WhcNMzYwOTE3MTk0NjM2WjB9\n"
        "MQswCQYDVQQGEwJJTDEWMBQGA1UEChMNU3RhcnRDb20gTHRkLjErMCkGA1UECxMi\n"
        "U2VjdXJlIERpZ2l0YWwgQ2VydGlmaWNhdGUgU2lnbmluZzEpMCcGA1UEAxMgU3Rh\n"
        "cnRDb20gQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUA\n"
        "A4ICDwAwggIKAoICAQDBiNsJvGxGfHiflXu1M5DycmLWwTYgIiRezul38kMKogZk\n"
        "pMyONvg45iPwbm2xPN1yo4UcodM9tDMr0y+v/uqwQVlntsQGfQqedIXWeUyAN3rf\n"
        "OQVSWff0G0ZDpNKFhdLDcfN1YjS6LIp/Ho/u7TTQEceWzVI9ujPW3U3eCztKS5/C\n"
        "Ji/6tRYccjV3yjxd5srhJosaNnZcAdt0FCX+7bWgiA/deMotHweXMAEtcnn6RtYT\n"
        "Kqi5pquDSR3l8u/d5AGOGAqPY1MWhWKpDhk6zLVmpsJrdAfkK+F2PrRt2PZE4XNi\n"
        "HzvEvqBTViVsUQn3qqvKv3b9bZvzndu/PWa8DFaqr5hIlTpL36dYUNk4dalb6kMM\n"
        "Av+Z6+hsTXBbKWWc3apdzK8BMewM69KN6Oqce+Zu9ydmDBpI125C4z/eIT574Q1w\n"
        "+2OqqGwaVLRcJXrJosmLFqa7LH4XXgVNWG4SHQHuEhANxjJ/GP/89PrNbpHoNkm+\n"
        "Gkhpi8KWTRoSsmkXwQqQ1vp5Iki/untp+HDH+no32NgN0nZPV/+Qt+OR0t3vwmC3\n"
        "Zzrd/qqc8NSLf3Iizsafl7b4r4qgEKjZ+xjGtrVcUjyJthkqcwEKDwOzEmDyei+B\n"
        "26Nu/yYwl/WL3YlXtq09s68rxbd2AvCl1iuahhQqcvbjM4xdCUsT37uMdBNSSwID\n"
        "AQABo4ICUjCCAk4wDAYDVR0TBAUwAwEB/zALBgNVHQ8EBAMCAa4wHQYDVR0OBBYE\n"
        "FE4L7xqkQFulF2mHMMo0aEPQQa7yMGQGA1UdHwRdMFswLKAqoCiGJmh0dHA6Ly9j\n"
        "ZXJ0LnN0YXJ0Y29tLm9yZy9zZnNjYS1jcmwuY3JsMCugKaAnhiVodHRwOi8vY3Js\n"
        "LnN0YXJ0Y29tLm9yZy9zZnNjYS1jcmwuY3JsMIIBXQYDVR0gBIIBVDCCAVAwggFM\n"
        "BgsrBgEEAYG1NwEBATCCATswLwYIKwYBBQUHAgEWI2h0dHA6Ly9jZXJ0LnN0YXJ0\n"
        "Y29tLm9yZy9wb2xpY3kucGRmMDUGCCsGAQUFBwIBFilodHRwOi8vY2VydC5zdGFy\n"
        "dGNvbS5vcmcvaW50ZXJtZWRpYXRlLnBkZjCB0AYIKwYBBQUHAgIwgcMwJxYgU3Rh\n"
        "cnQgQ29tbWVyY2lhbCAoU3RhcnRDb20pIEx0ZC4wAwIBARqBl0xpbWl0ZWQgTGlh\n"
        "YmlsaXR5LCByZWFkIHRoZSBzZWN0aW9uICpMZWdhbCBMaW1pdGF0aW9ucyogb2Yg\n"
        "dGhlIFN0YXJ0Q29tIENlcnRpZmljYXRpb24gQXV0aG9yaXR5IFBvbGljeSBhdmFp\n"
        "bGFibGUgYXQgaHR0cDovL2NlcnQuc3RhcnRjb20ub3JnL3BvbGljeS5wZGYwEQYJ\n"
        "YIZIAYb4QgEBBAQDAgAHMDgGCWCGSAGG+EIBDQQrFilTdGFydENvbSBGcmVlIFNT\n"
        "TCBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTANBgkqhkiG9w0BAQUFAAOCAgEAFmyZ\n"
        "9GYMNPXQhV59CuzaEE44HF7fpiUFS5Eyweg78T3dRAlbB0mKKctmArexmvclmAk8\n"
        "jhvh3TaHK0u7aNM5Zj2gJsfyOZEdUauCe37Vzlrk4gNXcGmXCPleWKYK34wGmkUW\n"
        "FjgKXlf2Ysd6AgXmvB618p70qSmD+LIU424oh0TDkBreOKk8rENNZEXO3SipXPJz\n"
        "ewT4F+irsfMuXGRuczE6Eri8sxHkfY+BUZo7jYn0TZNmezwD7dOaHZrzZVD1oNB1\n"
        "ny+v8OqCQ5j4aZyJecRDjkZy42Q2Eq/3JR44iZB3fsNrarnDy0RLrHiQi+fHLB5L\n"
        "EUTINFInzQpdn4XBidUaePKVEFMy3YCEZnXZtWgo+2EuvoSoOMCZEoalHmdkrQYu\n"
        "L6lwhceWD3yJZfWOQ1QOq92lgDmUYMA0yZZwLKMS9R9Ie70cfmu3nZD0Ijuu+Pwq\n"
        "yvqCUqDvr0tVk+vBtfAii6w0TiYiBKGHLHVKt+V9E9e4DGTANtLJL4YSjCMJwRuC\n"
        "O3NJo2pXh5Tl1njFmUNj403gdy3hZZlyaQQaRwnmDwFWJPsfvw55qVguucQJAX6V\n"
        "um0ABj6y6koQOdjQK/W/7HW/lwLFCRsI3FU34oH7N4RDYiDK51ZLZer+bMEkkySh\n"
        "NOsF/5oirpt9P/FlUQqmMGqz9IgcgA38corog14=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Thawte Personal Basic CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDITCCAoqgAwIBAgIBADANBgkqhkiG9w0BAQQFADCByzELMAkGA1UEBhMCWkEx\n"
        "FTATBgNVBAgTDFdlc3Rlcm4gQ2FwZTESMBAGA1UEBxMJQ2FwZSBUb3duMRowGAYD\n"
        "VQQKExFUaGF3dGUgQ29uc3VsdGluZzEoMCYGA1UECxMfQ2VydGlmaWNhdGlvbiBT\n"
        "ZXJ2aWNlcyBEaXZpc2lvbjEhMB8GA1UEAxMYVGhhd3RlIFBlcnNvbmFsIEJhc2lj\n"
        "IENBMSgwJgYJKoZIhvcNAQkBFhlwZXJzb25hbC1iYXNpY0B0aGF3dGUuY29tMB4X\n"
        "DTk2MDEwMTAwMDAwMFoXDTIwMTIzMTIzNTk1OVowgcsxCzAJBgNVBAYTAlpBMRUw\n"
        "EwYDVQQIEwxXZXN0ZXJuIENhcGUxEjAQBgNVBAcTCUNhcGUgVG93bjEaMBgGA1UE\n"
        "ChMRVGhhd3RlIENvbnN1bHRpbmcxKDAmBgNVBAsTH0NlcnRpZmljYXRpb24gU2Vy\n"
        "dmljZXMgRGl2aXNpb24xITAfBgNVBAMTGFRoYXd0ZSBQZXJzb25hbCBCYXNpYyBD\n"
        "QTEoMCYGCSqGSIb3DQEJARYZcGVyc29uYWwtYmFzaWNAdGhhd3RlLmNvbTCBnzAN\n"
        "BgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEAvLyTU23AUE+CFeZIlDWmWr5vQvoPR+53\n"
        "dXLdjUmbllegeNTKP1GzaQuRdhciB5dqxFGTS+CN7zeVoQxN2jSQHReJl+A1OFdK\n"
        "wPQIcOk8RHtQfmGakOMj04gRRif1CwcOu93RfyAKiLlWCy4cgNrx454p7xS9CkT7\n"
        "G1sY0b8jkyECAwEAAaMTMBEwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQQF\n"
        "AAOBgQAt4plrsD16iddZopQBHyvdEktTwq1/qqcAXJFAVyVKOKqEcLnZgA+le1z7\n"
        "c8a914phXAPjLSeoF+CEhULcXpvGt7Jtu3Sv5D/Lp7ew4F2+eIMllNLbgQ95B21P\n"
        "9DkVWlIBe94y1k049hJcBlDfBVu9FEuh3ym6O0GN92NWod8isQ==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Thawte Personal Freemail CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDLTCCApagAwIBAgIBADANBgkqhkiG9w0BAQQFADCB0TELMAkGA1UEBhMCWkEx\n"
        "FTATBgNVBAgTDFdlc3Rlcm4gQ2FwZTESMBAGA1UEBxMJQ2FwZSBUb3duMRowGAYD\n"
        "VQQKExFUaGF3dGUgQ29uc3VsdGluZzEoMCYGA1UECxMfQ2VydGlmaWNhdGlvbiBT\n"
        "ZXJ2aWNlcyBEaXZpc2lvbjEkMCIGA1UEAxMbVGhhd3RlIFBlcnNvbmFsIEZyZWVt\n"
        "YWlsIENBMSswKQYJKoZIhvcNAQkBFhxwZXJzb25hbC1mcmVlbWFpbEB0aGF3dGUu\n"
        "Y29tMB4XDTk2MDEwMTAwMDAwMFoXDTIwMTIzMTIzNTk1OVowgdExCzAJBgNVBAYT\n"
        "AlpBMRUwEwYDVQQIEwxXZXN0ZXJuIENhcGUxEjAQBgNVBAcTCUNhcGUgVG93bjEa\n"
        "MBgGA1UEChMRVGhhd3RlIENvbnN1bHRpbmcxKDAmBgNVBAsTH0NlcnRpZmljYXRp\n"
        "b24gU2VydmljZXMgRGl2aXNpb24xJDAiBgNVBAMTG1RoYXd0ZSBQZXJzb25hbCBG\n"
        "cmVlbWFpbCBDQTErMCkGCSqGSIb3DQEJARYccGVyc29uYWwtZnJlZW1haWxAdGhh\n"
        "d3RlLmNvbTCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEA1GnX1LCUZFtx6UfY\n"
        "DFG26nKRsIRefS0Nj3sS34UldSh0OkIsYyeflXtL734Zhx2G6qPduc6WZBrCFG5E\n"
        "rHzmj+hND3EfQDimAKOHePb5lIZererAXnbr2RSjXW56fAylS1V/Bhkpf56aJtVq\n"
        "uzgkCGqYx7Hao5iR/Xnb5VrEHLkCAwEAAaMTMBEwDwYDVR0TAQH/BAUwAwEB/zAN\n"
        "BgkqhkiG9w0BAQQFAAOBgQDH7JJ+Tvj1lqVnYiqk8E0RYNBvjWBYYawmu1I1XAjP\n"
        "MPuoSpaKH2JCI4wXD/S6ZJwXrEcp352YXtJsYHFcoqzceePnbgBHH7UNKOgCneSa\n"
        "/RP0ptl8sfjcXyMmCZGAc9AUG95DqYMl8uacLxXK/qarigd1iwzdUYRr5PjRznei\n"
        "gQ==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Thawte Personal Premium CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDKTCCApKgAwIBAgIBADANBgkqhkiG9w0BAQQFADCBzzELMAkGA1UEBhMCWkEx\n"
        "FTATBgNVBAgTDFdlc3Rlcm4gQ2FwZTESMBAGA1UEBxMJQ2FwZSBUb3duMRowGAYD\n"
        "VQQKExFUaGF3dGUgQ29uc3VsdGluZzEoMCYGA1UECxMfQ2VydGlmaWNhdGlvbiBT\n"
        "ZXJ2aWNlcyBEaXZpc2lvbjEjMCEGA1UEAxMaVGhhd3RlIFBlcnNvbmFsIFByZW1p\n"
        "dW0gQ0ExKjAoBgkqhkiG9w0BCQEWG3BlcnNvbmFsLXByZW1pdW1AdGhhd3RlLmNv\n"
        "bTAeFw05NjAxMDEwMDAwMDBaFw0yMDEyMzEyMzU5NTlaMIHPMQswCQYDVQQGEwJa\n"
        "QTEVMBMGA1UECBMMV2VzdGVybiBDYXBlMRIwEAYDVQQHEwlDYXBlIFRvd24xGjAY\n"
        "BgNVBAoTEVRoYXd0ZSBDb25zdWx0aW5nMSgwJgYDVQQLEx9DZXJ0aWZpY2F0aW9u\n"
        "IFNlcnZpY2VzIERpdmlzaW9uMSMwIQYDVQQDExpUaGF3dGUgUGVyc29uYWwgUHJl\n"
        "bWl1bSBDQTEqMCgGCSqGSIb3DQEJARYbcGVyc29uYWwtcHJlbWl1bUB0aGF3dGUu\n"
        "Y29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDJZtn4B0TPuYwu8KHvE0Vs\n"
        "Bd/eJxZRNkERbGw77f4QfRKe5ZtCmv5gMcNmt3M6SK5O0DI3lIi1DbbZ8/JE2dWI\n"
        "Et12TfIa/G8jHnrx2JhFTgcQ7xZC0EN1bUre4qrJMf8fAHB8Zs8QJQi6+u4A6UYD\n"
        "ZicRFTuqW/KY3TZCstqIdQIDAQABoxMwETAPBgNVHRMBAf8EBTADAQH/MA0GCSqG\n"
        "SIb3DQEBBAUAA4GBAGk2ifc0KjNyL2071CKyuG+axTZmDhs8obF1Wub9NdP4qPIH\n"
        "b4Vnjt4rueIXsDqg8A6iAJrf8xQVbrvIhVqYgPn/vnQdPfP+MCXRNzRn+qVxeTBh\n"
        "KXLA4CxM+1bkOqhv5TJZUtt1KFBZDPgLGeSs2a+WjS9Q2wfD6h+rM+D1KzGJ\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Thawte Premium Server CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDJzCCApCgAwIBAgIBATANBgkqhkiG9w0BAQQFADCBzjELMAkGA1UEBhMCWkEx\n"
        "FTATBgNVBAgTDFdlc3Rlcm4gQ2FwZTESMBAGA1UEBxMJQ2FwZSBUb3duMR0wGwYD\n"
        "VQQKExRUaGF3dGUgQ29uc3VsdGluZyBjYzEoMCYGA1UECxMfQ2VydGlmaWNhdGlv\n"
        "biBTZXJ2aWNlcyBEaXZpc2lvbjEhMB8GA1UEAxMYVGhhd3RlIFByZW1pdW0gU2Vy\n"
        "dmVyIENBMSgwJgYJKoZIhvcNAQkBFhlwcmVtaXVtLXNlcnZlckB0aGF3dGUuY29t\n"
        "MB4XDTk2MDgwMTAwMDAwMFoXDTIwMTIzMTIzNTk1OVowgc4xCzAJBgNVBAYTAlpB\n"
        "MRUwEwYDVQQIEwxXZXN0ZXJuIENhcGUxEjAQBgNVBAcTCUNhcGUgVG93bjEdMBsG\n"
        "A1UEChMUVGhhd3RlIENvbnN1bHRpbmcgY2MxKDAmBgNVBAsTH0NlcnRpZmljYXRp\n"
        "b24gU2VydmljZXMgRGl2aXNpb24xITAfBgNVBAMTGFRoYXd0ZSBQcmVtaXVtIFNl\n"
        "cnZlciBDQTEoMCYGCSqGSIb3DQEJARYZcHJlbWl1bS1zZXJ2ZXJAdGhhd3RlLmNv\n"
        "bTCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEA0jY2aovXwlue2oFBYo847kkE\n"
        "VdbQ7xwblRZH7xhINTpS9CtqBo87L+pW46+GjZ4X9560ZXUCTe/LCaIhUdib0GfQ\n"
        "ug2SBhRz1JPLlyoAnFxODLz6FVL88kRu2hFKbgifLy3j+ao6hnO2RlNYyIkFvYMR\n"
        "uHM/qgeN9EJN50CdHDcCAwEAAaMTMBEwDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG\n"
        "9w0BAQQFAAOBgQAmSCwWwlj66BZ0DKqqX1Q/8tfJeGBeXm43YyJ3Nn6yF8Q0ufUI\n"
        "hfzJATj/Tb7yFkJD57taRvvBxhEf8UqwKEbJw8RCfbz6q1lu1bdRiBHjpIUZa4JM\n"
        "pAwSremkrj/xw0llmozFyD4lt5SZu5IycQfwhl7tUCemDaYj+bvLpgcUQg==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Thawte Server CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDEzCCAnygAwIBAgIBATANBgkqhkiG9w0BAQQFADCBxDELMAkGA1UEBhMCWkEx\n"
        "FTATBgNVBAgTDFdlc3Rlcm4gQ2FwZTESMBAGA1UEBxMJQ2FwZSBUb3duMR0wGwYD\n"
        "VQQKExRUaGF3dGUgQ29uc3VsdGluZyBjYzEoMCYGA1UECxMfQ2VydGlmaWNhdGlv\n"
        "biBTZXJ2aWNlcyBEaXZpc2lvbjEZMBcGA1UEAxMQVGhhd3RlIFNlcnZlciBDQTEm\n"
        "MCQGCSqGSIb3DQEJARYXc2VydmVyLWNlcnRzQHRoYXd0ZS5jb20wHhcNOTYwODAx\n"
        "MDAwMDAwWhcNMjAxMjMxMjM1OTU5WjCBxDELMAkGA1UEBhMCWkExFTATBgNVBAgT\n"
        "DFdlc3Rlcm4gQ2FwZTESMBAGA1UEBxMJQ2FwZSBUb3duMR0wGwYDVQQKExRUaGF3\n"
        "dGUgQ29uc3VsdGluZyBjYzEoMCYGA1UECxMfQ2VydGlmaWNhdGlvbiBTZXJ2aWNl\n"
        "cyBEaXZpc2lvbjEZMBcGA1UEAxMQVGhhd3RlIFNlcnZlciBDQTEmMCQGCSqGSIb3\n"
        "DQEJARYXc2VydmVyLWNlcnRzQHRoYXd0ZS5jb20wgZ8wDQYJKoZIhvcNAQEBBQAD\n"
        "gY0AMIGJAoGBANOkUG7I/1Zr5s9dtuoMaHVHoqrC2oQl/Kj0R1HahbUgdJSGHg91\n"
        "yekIYfUGbTBuFRkC6VLAYttNmZ7iagxEOM3+vuNkCXDF/rFrKbYvScg71CcEJRCX\n"
        "L+eQbcAoQpnXTEPew/UhbVSfXcNY4cDk2VuwuNy0e982OsK1ZiIS1ocNAgMBAAGj\n"
        "EzARMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEEBQADgYEAB/pMaVz7lcxG\n"
        "7oWDTSEwjsrZqG9JGubaUeNgcGyEYRGhGshIPllDfU+VPaGLtwtimHp1it2ITk6e\n"
        "QNuozDJ0uW8NxuOzRAvZim+aKZuZGCg70eNAKJpaPNW15yAbi8qkq43pUdniTCxZ\n"
        "qdq5snUb9kLy78fyGPmJvKP/iiMucEc=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/Thawte Timestamping CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICoTCCAgqgAwIBAgIBADANBgkqhkiG9w0BAQQFADCBizELMAkGA1UEBhMCWkEx\n"
        "FTATBgNVBAgTDFdlc3Rlcm4gQ2FwZTEUMBIGA1UEBxMLRHVyYmFudmlsbGUxDzAN\n"
        "BgNVBAoTBlRoYXd0ZTEdMBsGA1UECxMUVGhhd3RlIENlcnRpZmljYXRpb24xHzAd\n"
        "BgNVBAMTFlRoYXd0ZSBUaW1lc3RhbXBpbmcgQ0EwHhcNOTcwMTAxMDAwMDAwWhcN\n"
        "MjAxMjMxMjM1OTU5WjCBizELMAkGA1UEBhMCWkExFTATBgNVBAgTDFdlc3Rlcm4g\n"
        "Q2FwZTEUMBIGA1UEBxMLRHVyYmFudmlsbGUxDzANBgNVBAoTBlRoYXd0ZTEdMBsG\n"
        "A1UECxMUVGhhd3RlIENlcnRpZmljYXRpb24xHzAdBgNVBAMTFlRoYXd0ZSBUaW1l\n"
        "c3RhbXBpbmcgQ0EwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBANYrWHhhRYZT\n"
        "6jR7UZztsOYuGA7+4F+oJ9O0yeB8WU4WDnNUYMF/9p8u6TqFJBU820cEY8OexJQa\n"
        "Wt9MevPZQx08EHp5JduQ/vBR5zDWQQD9nyjfeb6Uu522FOMjhdepQeBMpHmwKxqL\n"
        "8vg7ij5FrHGSALSQQZj7X+36ty6K+Ig3AgMBAAGjEzARMA8GA1UdEwEB/wQFMAMB\n"
        "Af8wDQYJKoZIhvcNAQEEBQADgYEAZ9viwuaHPUCDhjc1fR/OmsMMZiCouqoEiYbC\n"
        "9RAIDb/LogWK0E02PvTX72nGXuSwlG9KuefeW4i2e9vjJ+V2w/A1wcu1J5szedyQ\n"
        "pgCed/r8zSeUQhac0xxo7L9c3eWpexAKMnRUEzGLhQOEkbdYATAUOK8oyvyxUBkZ\n"
        "CayJSdM=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/thawte.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEIDCCAwigAwIBAgIQNE7VVyDV7exJ9C/ON9srbTANBgkqhkiG9w0BAQUFADCB\n"
        "qTELMAkGA1UEBhMCVVMxFTATBgNVBAoTDHRoYXd0ZSwgSW5jLjEoMCYGA1UECxMf\n"
        "Q2VydGlmaWNhdGlvbiBTZXJ2aWNlcyBEaXZpc2lvbjE4MDYGA1UECxMvKGMpIDIw\n"
        "MDYgdGhhd3RlLCBJbmMuIC0gRm9yIGF1dGhvcml6ZWQgdXNlIG9ubHkxHzAdBgNV\n"
        "BAMTFnRoYXd0ZSBQcmltYXJ5IFJvb3QgQ0EwHhcNMDYxMTE3MDAwMDAwWhcNMzYw\n"
        "NzE2MjM1OTU5WjCBqTELMAkGA1UEBhMCVVMxFTATBgNVBAoTDHRoYXd0ZSwgSW5j\n"
        "LjEoMCYGA1UECxMfQ2VydGlmaWNhdGlvbiBTZXJ2aWNlcyBEaXZpc2lvbjE4MDYG\n"
        "A1UECxMvKGMpIDIwMDYgdGhhd3RlLCBJbmMuIC0gRm9yIGF1dGhvcml6ZWQgdXNl\n"
        "IG9ubHkxHzAdBgNVBAMTFnRoYXd0ZSBQcmltYXJ5IFJvb3QgQ0EwggEiMA0GCSqG\n"
        "SIb3DQEBAQUAA4IBDwAwggEKAoIBAQCsoPD7gFnUnMekz52hWXMJEEUMDSxuaPFs\n"
        "W0hoSVk3/AszGcJ3f8wQLZU0HObrTQmnHNK4yZc2AreJ1CRfBsDMRJSUjQJib+ta\n"
        "3RGNKJpchJAQeg29dGYvajig4tVUROsdB58Hum/u6f1OCyn1PoSgAfGcq/gcfomk\n"
        "6KHYcWUNo1F77rzSImANuVud37r8UVsLr5iy6S7pBOhih94ryNdOwUxkHt3Ph1i6\n"
        "Sk/KaAcdHJ1KxtUvkcx8cXIcxcBn6zL9yZJclNqFwJu/U30rCfSMnZEfl2pSy94J\n"
        "NqR32HuHUETVPm4pafs5SSYeCaWAe0At6+gnhcn+Yf1+5nyXHdWdAgMBAAGjQjBA\n"
        "MA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgEGMB0GA1UdDgQWBBR7W0XP\n"
        "r87Lev0xkhpqtvNG61dIUDANBgkqhkiG9w0BAQUFAAOCAQEAeRHAS7ORtvzw6WfU\n"
        "DW5FvlXok9LOAz/t2iWwHVfLHjp2oEzsUHboZHIMpKnxuIvW1oeEuzLlQRHAd9mz\n"
        "YJ3rG9XRbkREqaYB7FViHXe4XI5ISXycO1cRrK1zN44veFyQaEfZYGDm/Ac9IiAX\n"
        "xPcW6cTYcvnIc3zfFi8VqT79aie2oetaupgf1eNNZAqdE8hhuvU5HIe6uL17In/2\n"
        "/qxAeeWsEG89jxt5dovEN7MhGITlNgDrYyCZuen+MwS7QcjBAvlEYyCegc5C09Y/\n"
        "LHbTY5xZ3Y+m4Q6gLkH3LpVHz7z9M/P2C2F+fpErgUfCJzDupxBdN49cOSvkBPB7\n"
        "jVaMaA==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/USERTrust.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIENjCCAx6gAwIBAgIBATANBgkqhkiG9w0BAQUFADBvMQswCQYDVQQGEwJTRTEU\n"
        "MBIGA1UEChMLQWRkVHJ1c3QgQUIxJjAkBgNVBAsTHUFkZFRydXN0IEV4dGVybmFs\n"
        "IFRUUCBOZXR3b3JrMSIwIAYDVQQDExlBZGRUcnVzdCBFeHRlcm5hbCBDQSBSb290\n"
        "MB4XDTAwMDUzMDEwNDgzOFoXDTIwMDUzMDEwNDgzOFowbzELMAkGA1UEBhMCU0Ux\n"
        "FDASBgNVBAoTC0FkZFRydXN0IEFCMSYwJAYDVQQLEx1BZGRUcnVzdCBFeHRlcm5h\n"
        "bCBUVFAgTmV0d29yazEiMCAGA1UEAxMZQWRkVHJ1c3QgRXh0ZXJuYWwgQ0EgUm9v\n"
        "dDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALf3GjPm8gAELTngTlvt\n"
        "H7xsD821+iO2zt6bETOXpClMfZOfvUq8k+0DGuOPz+VtUFrWlymUWoCwSXrbLpX9\n"
        "uMq/NzgtHj6RQa1wVsfwTz/oMp50ysiQVOnGXw94nZpAPA6sYapeFI+eh6FqUNzX\n"
        "mk6vBbOmcZSccbNQYArHE504B4YCqOmoaSYYkKtMsE8jqzpPhNjfzp/haW+710LX\n"
        "a0Tkx63ubUFfclpxCDezeWWkWaCUN/cALw3CknLa0Dhy2xSoRcRdKn23tNbE7qzN\n"
        "E0S3ySvdQwAl+mG5aWpYIxG3pzOPVnVZ9c0p10a3CitlttNCbxWyuHv77+ldU9U0\n"
        "WicCAwEAAaOB3DCB2TAdBgNVHQ4EFgQUrb2YejS0Jvf6xCZU7wO94CTLVBowCwYD\n"
        "VR0PBAQDAgEGMA8GA1UdEwEB/wQFMAMBAf8wgZkGA1UdIwSBkTCBjoAUrb2YejS0\n"
        "Jvf6xCZU7wO94CTLVBqhc6RxMG8xCzAJBgNVBAYTAlNFMRQwEgYDVQQKEwtBZGRU\n"
        "cnVzdCBBQjEmMCQGA1UECxMdQWRkVHJ1c3QgRXh0ZXJuYWwgVFRQIE5ldHdvcmsx\n"
        "IjAgBgNVBAMTGUFkZFRydXN0IEV4dGVybmFsIENBIFJvb3SCAQEwDQYJKoZIhvcN\n"
        "AQEFBQADggEBALCb4IUlwtYj4g+WBpKdQZic2YR5gdkeWxQHIzZlj7DYd7usQWxH\n"
        "YINRsPkyPef89iYTx4AWpb9a/IfPeHmJIZriTAcKhjW88t5RxNKWt9x+Tu5w/Rw5\n"
        "6wwCURQtjr0W4MHfRnXnJK3s9EK0hZNwEGe6nQY1ShjTK3rMUUKhemPR5ruhxSvC\n"
        "Nr4TDea9Y355e6cJDUCrat2PisP29owaQgVR1EX1n6diIWgVIEM8med8vSTYqZEX\n"
        "c4g/VhsxOBi0cQ+azcgOno4uG+GMmIPLHzHxREzGBHNJdmAPx/i9F4BrLunMTA5a\n"
        "mnkPIAou1Z5jJh5VkpTYghdae9C8x49OhgQ=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/UTN - DATACorp SGC.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEXjCCA0agAwIBAgIQRL4Mi1AAIbQR0ypoBqmtaTANBgkqhkiG9w0BAQUFADCB\n"
        "kzELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAlVUMRcwFQYDVQQHEw5TYWx0IExha2Ug\n"
        "Q2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMSEwHwYDVQQLExho\n"
        "dHRwOi8vd3d3LnVzZXJ0cnVzdC5jb20xGzAZBgNVBAMTElVUTiAtIERBVEFDb3Jw\n"
        "IFNHQzAeFw05OTA2MjQxODU3MjFaFw0xOTA2MjQxOTA2MzBaMIGTMQswCQYDVQQG\n"
        "EwJVUzELMAkGA1UECBMCVVQxFzAVBgNVBAcTDlNhbHQgTGFrZSBDaXR5MR4wHAYD\n"
        "VQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxITAfBgNVBAsTGGh0dHA6Ly93d3cu\n"
        "dXNlcnRydXN0LmNvbTEbMBkGA1UEAxMSVVROIC0gREFUQUNvcnAgU0dDMIIBIjAN\n"
        "BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA3+5YEKIrblXEjr8uRgnn4AgPLit6\n"
        "E5Qbvfa2gI5lBZMAHryv4g+OGQ0SR+ysraP6LnD43m77VkIVni5c7yPeIbkFdicZ\n"
        "D0/Ww5y0vpQZY/KmEQrrU0icvvIpOxboGqBMpsn0GFlowHDyUwDAXlCCpVZvNvlK\n"
        "4ESGoE1O1kduSUrLZ9emxAW5jh70/P/N5zbgnAVssjMiFdC04MwXwLLA9P4yPykq\n"
        "lXvY8qdOD1R8oQ2AswkDwf9c3V6aPryuvEeKaq5xyh+xKrhfQgUL7EYw0XILyulW\n"
        "bfXv33i+Ybqypa4ETLyorGkVl73v67SMvzX41MPRKA5cOp9wGDMgd8SirwIDAQAB\n"
        "o4GrMIGoMAsGA1UdDwQEAwIBxjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBRT\n"
        "MtGzz3/64PGgXYVOktKeRR20TzA9BgNVHR8ENjA0MDKgMKAuhixodHRwOi8vY3Js\n"
        "LnVzZXJ0cnVzdC5jb20vVVROLURBVEFDb3JwU0dDLmNybDAqBgNVHSUEIzAhBggr\n"
        "BgEFBQcDAQYKKwYBBAGCNwoDAwYJYIZIAYb4QgQBMA0GCSqGSIb3DQEBBQUAA4IB\n"
        "AQAnNZcAiosovcYzMB4p/OL31ZjUQLtgyr+rFywJNn9Q+kHcrpY6CiM+iVnJowft\n"
        "Gzet/Hy+UUla3joKVAgWRcKZsYfNjGjgaQPpxE6YsjuMFrMOoAyYUJuTqXAJyCyj\n"
        "j98C5OBxOvG0I3KgqgHf35g+FFCgMSa9KOlaMCZ1+XtgHI3zzVAmbQQnmt/VDUVH\n"
        "KWss5nbZqSl9Mt3JNjy9rjXxEZ4du5A/EkdOjtd+D2JzHVImOBwYSf0wdJrE5SIv\n"
        "2MCN7ZF6TACPcn9d2t0bi0Vr591pl6jFVkwPDPafepE39peC4N1xaf92P2BNPM/3\n"
        "mfnGV/TJVTl4uix5yaaIK/QI\n"
        "-----END CERTIFICATE-----\n",

        // ./root/UTN - USERFirst-Client Authentication and Email.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEojCCA4qgAwIBAgIQRL4Mi1AAJLQR0zYlJWfJiTANBgkqhkiG9w0BAQUFADCB\n"
        "rjELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAlVUMRcwFQYDVQQHEw5TYWx0IExha2Ug\n"
        "Q2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMSEwHwYDVQQLExho\n"
        "dHRwOi8vd3d3LnVzZXJ0cnVzdC5jb20xNjA0BgNVBAMTLVVUTi1VU0VSRmlyc3Qt\n"
        "Q2xpZW50IEF1dGhlbnRpY2F0aW9uIGFuZCBFbWFpbDAeFw05OTA3MDkxNzI4NTBa\n"
        "Fw0xOTA3MDkxNzM2NThaMIGuMQswCQYDVQQGEwJVUzELMAkGA1UECBMCVVQxFzAV\n"
        "BgNVBAcTDlNhbHQgTGFrZSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5l\n"
        "dHdvcmsxITAfBgNVBAsTGGh0dHA6Ly93d3cudXNlcnRydXN0LmNvbTE2MDQGA1UE\n"
        "AxMtVVROLVVTRVJGaXJzdC1DbGllbnQgQXV0aGVudGljYXRpb24gYW5kIEVtYWls\n"
        "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAsjmFpPJ9q0E7YkY3rs3B\n"
        "YHW8OWX5ShpHornMSMxqmNVNNRm5pELlzkniii8efNIxB8dOtINknS4p1aJkxIW9\n"
        "hVE1eaROaJB7HHqkkqgX8pgV8pPMyaQylbsMTzC9mKALi+VuG6JG+ni8om+rWV6l\n"
        "L8/K2m2qL+usobNqqrcuZzWLeeEeaYji5kbNoKXqvgvOdjp6Dpvq/NonWz1zHyLm\n"
        "SGHGTPNpsaguG7bUMSAsvIKKjqQOpdeJQ/wWWq8dcdcRWdq6hw2v+vPhwvCkxWeM\n"
        "1tZUOt4KpLoDd7NlyP0e03RiqhjKaJMeoYV+9Udly/hNVyh00jT/MLbu9mIwFIws\n"
        "6wIDAQABo4G5MIG2MAsGA1UdDwQEAwIBxjAPBgNVHRMBAf8EBTADAQH/MB0GA1Ud\n"
        "DgQWBBSJgmd9xJ0mcABLtFBIfN49rgRufTBYBgNVHR8EUTBPME2gS6BJhkdodHRw\n"
        "Oi8vY3JsLnVzZXJ0cnVzdC5jb20vVVROLVVTRVJGaXJzdC1DbGllbnRBdXRoZW50\n"
        "aWNhdGlvbmFuZEVtYWlsLmNybDAdBgNVHSUEFjAUBggrBgEFBQcDAgYIKwYBBQUH\n"
        "AwQwDQYJKoZIhvcNAQEFBQADggEBALFtYV2mGn98q0rkMPxTbyUkxsrt4jFcKw7u\n"
        "7mFVbwQ+zznexRtJlOTrIEy05p5QLnLZjfWqo7NK2lYcYJeA3IKirUq9iiv/Cwm0\n"
        "xtcgBEXkzYABurorbs6q15L+5K/r9CYdFip/bDCVNy8zEqx/3cfREYxRmLLQo5HQ\n"
        "rfafnoOTHh1CuEava2bwm3/q4wMC5QJRwarVNZ1yQAOJujEdxRBoUp7fooXFXAim\n"
        "eOZTT7Hot9MUnpOmw2TjrH5xzbyf6QMbzPvprDHBr3wVdAKZw7JHpsIyYdfHb0gk\n"
        "USeh1YdV8nuPmD0Wnu51tvjQjvLzxq4oW6fw8zYX/MMF08oDSlQ=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/UTN - USERFirst-Hardware.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEdDCCA1ygAwIBAgIQRL4Mi1AAJLQR0zYq/mUK/TANBgkqhkiG9w0BAQUFADCB\n"
        "lzELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAlVUMRcwFQYDVQQHEw5TYWx0IExha2Ug\n"
        "Q2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMSEwHwYDVQQLExho\n"
        "dHRwOi8vd3d3LnVzZXJ0cnVzdC5jb20xHzAdBgNVBAMTFlVUTi1VU0VSRmlyc3Qt\n"
        "SGFyZHdhcmUwHhcNOTkwNzA5MTgxMDQyWhcNMTkwNzA5MTgxOTIyWjCBlzELMAkG\n"
        "A1UEBhMCVVMxCzAJBgNVBAgTAlVUMRcwFQYDVQQHEw5TYWx0IExha2UgQ2l0eTEe\n"
        "MBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMSEwHwYDVQQLExhodHRwOi8v\n"
        "d3d3LnVzZXJ0cnVzdC5jb20xHzAdBgNVBAMTFlVUTi1VU0VSRmlyc3QtSGFyZHdh\n"
        "cmUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCx98M4P7Sof885glFn\n"
        "0G2f0v9Y8+efK+wNiVSZuTiZFvfgIXlIwrthdBKWHTxqctU8EGc6Oe0rE81m65UJ\n"
        "M6Rsl7HoxuzBdXmcRl6Nq9Bq/bkqVRcQVLMZ8Jr28bFdtqdt++BxF2uiiPsA3/4a\n"
        "MXcMmgF6sTLjKwEHOG7DpV4jvEWbe1DByTCP2+UretNb+zNAHqDVmBe8i4fDidNd\n"
        "oI6yqqr2jmmIBsX6iSHzCJ1pLgkzmykNRg+MzEk0sGlRvfkGzWitZky8PqxhvQqI\n"
        "DsjfPe58BEydCl5rkdbux+0ojatNh4lz0G6k0B4WixThdkQDf2Os5M1JnMWS9Ksy\n"
        "oUhbAgMBAAGjgbkwgbYwCwYDVR0PBAQDAgHGMA8GA1UdEwEB/wQFMAMBAf8wHQYD\n"
        "VR0OBBYEFKFyXyYbKJhDlV0HN9WFlp1L0sNFMEQGA1UdHwQ9MDswOaA3oDWGM2h0\n"
        "dHA6Ly9jcmwudXNlcnRydXN0LmNvbS9VVE4tVVNFUkZpcnN0LUhhcmR3YXJlLmNy\n"
        "bDAxBgNVHSUEKjAoBggrBgEFBQcDAQYIKwYBBQUHAwUGCCsGAQUFBwMGBggrBgEF\n"
        "BQcDBzANBgkqhkiG9w0BAQUFAAOCAQEARxkP3nTGmZev/K0oXnWO6y1n7k57K9cM\n"
        "//bey1WiCuFMVGWTYGufEpytXoMs61quwOQt9ABjHbjAbPLPSbtNk28Gpgoiskli\n"
        "CE7/yMgUsogWXecB5BKV5UU0s4tpvc+0hY91UZ59Ojg6FEgSxvunOxqNDYJAB+gE\n"
        "CJChicsZUN/KHAG8HQQZexB2lzvukJDKxA4fFm517zP4029bHpbj4HR3dHuKom4t\n"
        "3XbWOTCC8KucUvIqx69JXn7HaOWCgchqJ/kniCrVWFCVH/A7HFe7fRQ5YiuayZSS\n"
        "KqMiDP+JJn1fIytH1xUdqWqeUQ0qUZ6B+dQ7XnASfxAynB67nfhmqA==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/UTN - USERFirst-Network Applications.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEZDCCA0ygAwIBAgIQRL4Mi1AAJLQR0zYwS8AzdzANBgkqhkiG9w0BAQUFADCB\n"
        "ozELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAlVUMRcwFQYDVQQHEw5TYWx0IExha2Ug\n"
        "Q2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMSEwHwYDVQQLExho\n"
        "dHRwOi8vd3d3LnVzZXJ0cnVzdC5jb20xKzApBgNVBAMTIlVUTi1VU0VSRmlyc3Qt\n"
        "TmV0d29yayBBcHBsaWNhdGlvbnMwHhcNOTkwNzA5MTg0ODM5WhcNMTkwNzA5MTg1\n"
        "NzQ5WjCBozELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAlVUMRcwFQYDVQQHEw5TYWx0\n"
        "IExha2UgQ2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMSEwHwYD\n"
        "VQQLExhodHRwOi8vd3d3LnVzZXJ0cnVzdC5jb20xKzApBgNVBAMTIlVUTi1VU0VS\n"
        "Rmlyc3QtTmV0d29yayBBcHBsaWNhdGlvbnMwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n"
        "DwAwggEKAoIBAQCz+5Gh5DZVhawGNFugmliy+LUPBXeDrjKxdpJo7CNKyXY/45y2\n"
        "N3kDuatpjQclthln5LAbGHNhSuh+zdMvZOOmfAz6F4CjDUeJT1FxL+78P/m4FoCH\n"
        "iZMlIJpDgmkkdihZNaEdwH+DBmQWICzTSaSFtMBhf1EI+GgVkYDLpdXuOzr0hARe\n"
        "YFmnjDRy7rh4xdE7EkpvfmUnuaRVxblvQ6TFHSyZwFKkeEwVs0CYCGtDxgGwenv1\n"
        "axwiP8vv/6jQOkt2FZ7S0cYu49tXGzKiuG/ohqY/cKvlcJKrRB5AUPuco2LkbG6g\n"
        "yN7igEL66S/ozjIEj3yNtxyjNTwV3Z7DrpelAgMBAAGjgZEwgY4wCwYDVR0PBAQD\n"
        "AgHGMA8GA1UdEwEB/wQFMAMBAf8wHQYDVR0OBBYEFPqGydvguul49Uuo1hXf8NPh\n"
        "ahQ8ME8GA1UdHwRIMEYwRKBCoECGPmh0dHA6Ly9jcmwudXNlcnRydXN0LmNvbS9V\n"
        "VE4tVVNFUkZpcnN0LU5ldHdvcmtBcHBsaWNhdGlvbnMuY3JsMA0GCSqGSIb3DQEB\n"
        "BQUAA4IBAQCk8yXM0dSRgyLQzDKrm5ZONJFUICU0YV8qAhXhi6r/fWRRzwr/vH3Y\n"
        "IWp4yy9Rb/hCHTO967V7lMPDqaAt39EpHx3+jz+7qEUqf9FuVSTiuwL7MT++6Lzs\n"
        "QCv4AdRWOOTKRIK1YSAhZ2X28AvnNPilwpyjXEAfhZOVBt5P1CeptqX8Fs1zMT+4\n"
        "ZSfP1FMa8Kxun08FDAOBp4QpxFq9ZFdyrTvPNximmMatBrTcCKME1SmklpoSZ0qM\n"
        "YEWd8SOasACcaLWYUNPvji6SZbFIPiG+FTAqDbUMo2s/rn9X9R+WfN9v3YIwLGUb\n"
        "QErNaLly7HF27FSOH4UMAWr6pjisH8SE\n"
        "-----END CERTIFICATE-----\n",

        // ./root/UTN - USERFirst-Object.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEZjCCA06gAwIBAgIQRL4Mi1AAJLQR0zYt4LNfGzANBgkqhkiG9w0BAQUFADCB\n"
        "lTELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAlVUMRcwFQYDVQQHEw5TYWx0IExha2Ug\n"
        "Q2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3b3JrMSEwHwYDVQQLExho\n"
        "dHRwOi8vd3d3LnVzZXJ0cnVzdC5jb20xHTAbBgNVBAMTFFVUTi1VU0VSRmlyc3Qt\n"
        "T2JqZWN0MB4XDTk5MDcwOTE4MzEyMFoXDTE5MDcwOTE4NDAzNlowgZUxCzAJBgNV\n"
        "BAYTAlVTMQswCQYDVQQIEwJVVDEXMBUGA1UEBxMOU2FsdCBMYWtlIENpdHkxHjAc\n"
        "BgNVBAoTFVRoZSBVU0VSVFJVU1QgTmV0d29yazEhMB8GA1UECxMYaHR0cDovL3d3\n"
        "dy51c2VydHJ1c3QuY29tMR0wGwYDVQQDExRVVE4tVVNFUkZpcnN0LU9iamVjdDCC\n"
        "ASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAM6qgT+jo2F4qjEAVZURnicP\n"
        "HxzfOpuCaDDASmEd8S8O+r5596Uj71VRloTN2+O5bj4x2AogZ8f02b+U60cEPgLO\n"
        "KqJdhwQJ9jCdGIqXsqoc/EHSoTbL+z2RuufZcDX65OeQw5ujm9M89RKZd7G3CeBo\n"
        "5hy485RjiGpq/gt2yb70IuRnuasaXnfBhQfdDWy/7gbHd2pBnqcP1/vulBe3/IW+\n"
        "pKvEHDHd17bR5PDv3xaPslKT16HUiaEHLr/hARJCHhrh2JU022R5KP+6LhHC5ehb\n"
        "kkj7RwvCbNqtMoNB86XlQXD9ZZBt+vpRxPm9lisZBCzTbafc8H9vg2XiaquHhnUC\n"
        "AwEAAaOBrzCBrDALBgNVHQ8EBAMCAcYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n"
        "FgQU2u1kdBScFDyr3ZmpvVsoTYs8ydgwQgYDVR0fBDswOTA3oDWgM4YxaHR0cDov\n"
        "L2NybC51c2VydHJ1c3QuY29tL1VUTi1VU0VSRmlyc3QtT2JqZWN0LmNybDApBgNV\n"
        "HSUEIjAgBggrBgEFBQcDAwYIKwYBBQUHAwgGCisGAQQBgjcKAwQwDQYJKoZIhvcN\n"
        "AQEFBQADggEBAAgfUrE3RHjb/c652pWWmKpVZIC1WkDdIaXFwfNfLEzIR1pp6ujw\n"
        "NTX00CXzyKakh0q9G7FzCL3Uw8q2NbtZhncxzaeAFK4T7/yxSPlrJSUtUbYsbUXB\n"
        "mMiKVl0+7kNOPmsnjtA6S4ULX9Ptaqd1y9Fahy85dRNacrACgZ++8A+EVCBibGnU\n"
        "4U3GDZlDAQ0Slox4nb9QorFEqmrPF3rPbw/U+CRVX/A0FklmPlBGyWNxODFiuGK5\n"
        "81OtbLUrohKqGU8J2l7nk8aOFAj+8DCAGKCGhU3IfdeLA/5u1fedFqySLKAj5ZyR\n"
        "Uh+U3xeUc8OzwcFxBSAAeL0TUh2oPs0AH8g=\n"
        "-----END CERTIFICATE-----\n",

        // ./root/ValiCert Class 1 Policy Validation Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIC5zCCAlACAQEwDQYJKoZIhvcNAQEFBQAwgbsxJDAiBgNVBAcTG1ZhbGlDZXJ0\n"
        "IFZhbGlkYXRpb24gTmV0d29yazEXMBUGA1UEChMOVmFsaUNlcnQsIEluYy4xNTAz\n"
        "BgNVBAsTLFZhbGlDZXJ0IENsYXNzIDEgUG9saWN5IFZhbGlkYXRpb24gQXV0aG9y\n"
        "aXR5MSEwHwYDVQQDExhodHRwOi8vd3d3LnZhbGljZXJ0LmNvbS8xIDAeBgkqhkiG\n"
        "9w0BCQEWEWluZm9AdmFsaWNlcnQuY29tMB4XDTk5MDYyNTIyMjM0OFoXDTE5MDYy\n"
        "NTIyMjM0OFowgbsxJDAiBgNVBAcTG1ZhbGlDZXJ0IFZhbGlkYXRpb24gTmV0d29y\n"
        "azEXMBUGA1UEChMOVmFsaUNlcnQsIEluYy4xNTAzBgNVBAsTLFZhbGlDZXJ0IENs\n"
        "YXNzIDEgUG9saWN5IFZhbGlkYXRpb24gQXV0aG9yaXR5MSEwHwYDVQQDExhodHRw\n"
        "Oi8vd3d3LnZhbGljZXJ0LmNvbS8xIDAeBgkqhkiG9w0BCQEWEWluZm9AdmFsaWNl\n"
        "cnQuY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDYWYJ6ibiWuqYvaG9Y\n"
        "LqdUHAZu9OqNSLwxlBfw8068srg1knaw0KWlAdcAAxIiGQj4/xEjm84H9b9pGib+\n"
        "TunRf50sQB1ZaG6m+FiwnRqP0z/x3BkGgagO4DrdyFNFCQbmD3DD+kCmDuJWBQ8Y\n"
        "TfwggtFzVXSNdnKgHZ0dwN0/cQIDAQABMA0GCSqGSIb3DQEBBQUAA4GBAFBoPUn0\n"
        "LBwGlN+VYH+Wexf+T3GtZMjdd9LvWVXoP+iOBSoh8gfStadS/pyxtuJbdxdA6nLW\n"
        "I8sogTLDAHkY7FkXicnGah5xyf23dKUlRWnFSKsZ4UWKJWsZ7uW7EvV/96aNUcPw\n"
        "nXS3qT6gpf+2SQMT2iLM7XGCK5nPOrf1LXLI\n"
        "-----END CERTIFICATE-----\n",

        // ./root/ValiCert Class 2 Policy Validation Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIC5zCCAlACAQEwDQYJKoZIhvcNAQEFBQAwgbsxJDAiBgNVBAcTG1ZhbGlDZXJ0\n"
        "IFZhbGlkYXRpb24gTmV0d29yazEXMBUGA1UEChMOVmFsaUNlcnQsIEluYy4xNTAz\n"
        "BgNVBAsTLFZhbGlDZXJ0IENsYXNzIDIgUG9saWN5IFZhbGlkYXRpb24gQXV0aG9y\n"
        "aXR5MSEwHwYDVQQDExhodHRwOi8vd3d3LnZhbGljZXJ0LmNvbS8xIDAeBgkqhkiG\n"
        "9w0BCQEWEWluZm9AdmFsaWNlcnQuY29tMB4XDTk5MDYyNjAwMTk1NFoXDTE5MDYy\n"
        "NjAwMTk1NFowgbsxJDAiBgNVBAcTG1ZhbGlDZXJ0IFZhbGlkYXRpb24gTmV0d29y\n"
        "azEXMBUGA1UEChMOVmFsaUNlcnQsIEluYy4xNTAzBgNVBAsTLFZhbGlDZXJ0IENs\n"
        "YXNzIDIgUG9saWN5IFZhbGlkYXRpb24gQXV0aG9yaXR5MSEwHwYDVQQDExhodHRw\n"
        "Oi8vd3d3LnZhbGljZXJ0LmNvbS8xIDAeBgkqhkiG9w0BCQEWEWluZm9AdmFsaWNl\n"
        "cnQuY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDOOnHK5avIWZJV16vY\n"
        "dA757tn2VUdZZUcOBVXc65g2PFxTXdMwzzjsvUGJ7SVCCSRrCl6zfN1SLUzm1NZ9\n"
        "WlmpZdRJEy0kTRxQb7XBhVQ7/nHk01xC+YDgkRoKWzk2Z/M/VXwbP7RfZHM047QS\n"
        "v4dk+NoS/zcnwbNDu+97bi5p9wIDAQABMA0GCSqGSIb3DQEBBQUAA4GBADt/UG9v\n"
        "UJSZSWI4OB9L+KXIPqeCgfYrx+jFzug6EILLGACOTb2oWH+heQC1u+mNr0HZDzTu\n"
        "IYEZoDJJKPTEjlbVUjP9UNV+mWwD5MlM/Mtsq2azSiGM5bUMMj4QssxsodyamEwC\n"
        "W/POuZ6lcg5Ktz885hZo+L7tdEy8W9ViH0Pd\n"
        "-----END CERTIFICATE-----\n",

        // ./root/ValiCert Class 3 Policy Validation Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIC5zCCAlACAQEwDQYJKoZIhvcNAQEFBQAwgbsxJDAiBgNVBAcTG1ZhbGlDZXJ0\n"
        "IFZhbGlkYXRpb24gTmV0d29yazEXMBUGA1UEChMOVmFsaUNlcnQsIEluYy4xNTAz\n"
        "BgNVBAsTLFZhbGlDZXJ0IENsYXNzIDMgUG9saWN5IFZhbGlkYXRpb24gQXV0aG9y\n"
        "aXR5MSEwHwYDVQQDExhodHRwOi8vd3d3LnZhbGljZXJ0LmNvbS8xIDAeBgkqhkiG\n"
        "9w0BCQEWEWluZm9AdmFsaWNlcnQuY29tMB4XDTk5MDYyNjAwMjIzM1oXDTE5MDYy\n"
        "NjAwMjIzM1owgbsxJDAiBgNVBAcTG1ZhbGlDZXJ0IFZhbGlkYXRpb24gTmV0d29y\n"
        "azEXMBUGA1UEChMOVmFsaUNlcnQsIEluYy4xNTAzBgNVBAsTLFZhbGlDZXJ0IENs\n"
        "YXNzIDMgUG9saWN5IFZhbGlkYXRpb24gQXV0aG9yaXR5MSEwHwYDVQQDExhodHRw\n"
        "Oi8vd3d3LnZhbGljZXJ0LmNvbS8xIDAeBgkqhkiG9w0BCQEWEWluZm9AdmFsaWNl\n"
        "cnQuY29tMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDjmFGWHOjVsQaBalfD\n"
        "cnWTq8+epvzzFlLWLU2fNUSoLgRNB0mKOCn1dzfnt6td3zZxFJmP3MKS8edgkpfs\n"
        "2Ejcv8ECIMYkpChMMFp2bbFc893enhBxoYjHW5tBbcqwuI4V7q0zK89HBFx1cQqY\n"
        "JJgpp0lZpd34t0NiYfPT4tBVPwIDAQABMA0GCSqGSIb3DQEBBQUAA4GBAFa7AliE\n"
        "Zwgs3x/be0kz9dNnnfS0ChCzycUs4pJqcXgn8nCDQtM+z6lU9PHYkhaM0QTLS6vJ\n"
        "n0WuPIqpsHEzXcjFV9+vqDWzf4mH6eglkrh/hXqu1rweN1gqZ8mRzyqBPu3GOd/A\n"
        "PhmcGcwTTYJBtYze4D1gCCAPRX5ron+jjBXu\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 1 Primary CA (1).cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICPDCCAaUCEDJQM89Q0VbzXIGtZVxPyCUwDQYJKoZIhvcNAQECBQAwXzELMAkG\n"
        "A1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMTcwNQYDVQQLEy5DbGFz\n"
        "cyAxIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTk2\n"
        "MDEyOTAwMDAwMFoXDTIwMDEwNzIzNTk1OVowXzELMAkGA1UEBhMCVVMxFzAVBgNV\n"
        "BAoTDlZlcmlTaWduLCBJbmMuMTcwNQYDVQQLEy5DbGFzcyAxIFB1YmxpYyBQcmlt\n"
        "YXJ5IENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIGfMA0GCSqGSIb3DQEBAQUAA4GN\n"
        "ADCBiQKBgQDlGb9to1ZhLZlIcfZn3rmN67eehoAKkQ76OCWvRoiC5XOooJskXQ0f\n"
        "zGVuDLDQVoQYh5oGmxChc9+0WDlrbsH2FdWoqD+qEgaNMax/sDTXjzRniAnNFBHi\n"
        "TkVWaR94AoDa3EeRKbs2yWNcxeDXLYd7obcysHswuiovMaruo2fa2wIDAQABMA0G\n"
        "CSqGSIb3DQEBAgUAA4GBAEtEZmBoZOSYG/OwcuaViXzde7OVwB0u2NgZ0C00PcZQ\n"
        "mhCGjKo/O6gE/DdSlcPZydvN8oYGxLEb8IKIMEKOF1AcZHq4PplJdJf8rAJD+5YM\n"
        "VgQlDHx8h50kp9jwMim1pN9dokzFFjKoQvZFprY2ueC/ZTaTwtLXa9zeWdaiNfhF\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 1 Primary CA (4).cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDAjCCAmsCEEzH6qqYPnHTkxD4PTqJkZIwDQYJKoZIhvcNAQEFBQAwgcExCzAJ\n"
        "BgNVBAYTAlVTMRcwFQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xh\n"
        "c3MgMSBQdWJsaWMgUHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcy\n"
        "MTowOAYDVQQLEzEoYykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3Jp\n"
        "emVkIHVzZSBvbmx5MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMB4X\n"
        "DTk4MDUxODAwMDAwMFoXDTI4MDgwMTIzNTk1OVowgcExCzAJBgNVBAYTAlVTMRcw\n"
        "FQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xhc3MgMSBQdWJsaWMg\n"
        "UHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcyMTowOAYDVQQLEzEo\n"
        "YykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5\n"
        "MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMIGfMA0GCSqGSIb3DQEB\n"
        "AQUAA4GNADCBiQKBgQCq0Lq+Fi24g9TK0g+8djHKlNgdk4xWArzZbxpvUjZudVYK\n"
        "VdPfQ4chEWWKfo+9Id5rMj8bhDSVBZ1BNeuS65bdqlk/AVNtmU/t5eIqWpDBucSm\n"
        "Fc/IReumXY6cPvBkJHalzasab7bYe1FhbqZ/h8jit+U03EGI6glAvnOSPWvndQID\n"
        "AQABMA0GCSqGSIb3DQEBBQUAA4GBAKlPww3HZ74sy9mozS11534Vnjty637rXC0J\n"
        "h9ZrbWB85a7FkCMMXErQr7Fd88e2CtvgFZMN3QO8x3aKtd1Pw5sTdbgBwObJW2ul\n"
        "uIncrKTdcu1OofdPvAbT6shkdHvClUGcZXNY8ZCaPGqxmMnEh7zPRW1F4m4iP/68\n"
        "DzFc6PLZ\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 1 Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDAjCCAmsCEDnKVIn+UCIy/jLZ2/sbhBkwDQYJKoZIhvcNAQEFBQAwgcExCzAJ\n"
        "BgNVBAYTAlVTMRcwFQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xh\n"
        "c3MgMSBQdWJsaWMgUHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcy\n"
        "MTowOAYDVQQLEzEoYykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3Jp\n"
        "emVkIHVzZSBvbmx5MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMB4X\n"
        "DTk4MDUxODAwMDAwMFoXDTE4MDUxODIzNTk1OVowgcExCzAJBgNVBAYTAlVTMRcw\n"
        "FQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xhc3MgMSBQdWJsaWMg\n"
        "UHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcyMTowOAYDVQQLEzEo\n"
        "YykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5\n"
        "MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMIGfMA0GCSqGSIb3DQEB\n"
        "AQUAA4GNADCBiQKBgQCq0Lq+Fi24g9TK0g+8djHKlNgdk4xWArzZbxpvUjZudVYK\n"
        "VdPfQ4chEWWKfo+9Id5rMj8bhDSVBZ1BNeuS65bdqlk/AVNtmU/t5eIqWpDBucSm\n"
        "Fc/IReumXY6cPvBkJHalzasab7bYe1FhbqZ/h8jit+U03EGI6glAvnOSPWvndQID\n"
        "AQABMA0GCSqGSIb3DQEBBQUAA4GBAIv3GhDOdlwHq4OZ3BeAbzQ5XZg+a3Is4cei\n"
        "e0ApuXiIukzFo2penm574/ICQQxmvq37rqIUzpLzojSLtLK2JPLl1eDI5WJthHvL\n"
        "vrsDi3xXyvA3qZCviu4Dvh0onNkmdqDNxJ1O8K4HFtW+r1cIatCgQkJCHvQgzKV4\n"
        "gpUmOIpH\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 1 Public Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICPTCCAaYCEQDNun9W8N/kvFT+IqyzcqpVMA0GCSqGSIb3DQEBAgUAMF8xCzAJ\n"
        "BgNVBAYTAlVTMRcwFQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE3MDUGA1UECxMuQ2xh\n"
        "c3MgMSBQdWJsaWMgUHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTAeFw05\n"
        "NjAxMjkwMDAwMDBaFw0yODA4MDEyMzU5NTlaMF8xCzAJBgNVBAYTAlVTMRcwFQYD\n"
        "VQQKEw5WZXJpU2lnbiwgSW5jLjE3MDUGA1UECxMuQ2xhc3MgMSBQdWJsaWMgUHJp\n"
        "bWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTCBnzANBgkqhkiG9w0BAQEFAAOB\n"
        "jQAwgYkCgYEA5Rm/baNWYS2ZSHH2Z965jeu3noaACpEO+jglr0aIguVzqKCbJF0N\n"
        "H8xlbgyw0FaEGIeaBpsQoXPftFg5a27B9hXVqKg/qhIGjTGsf7A01480Z4gJzRQR\n"
        "4k5FVmkfeAKA2txHkSm7NsljXMXg1y2He6G3MrB7MLoqLzGq7qNn2tsCAwEAATAN\n"
        "BgkqhkiG9w0BAQIFAAOBgQBMP7iLxmjf7kMzDl3ppssHhE16M/+SG/Q2rdiVIjZo\n"
        "EWx8QszznC7EBz8UsA9P/5CSdvnivErpj82ggAr3xSnxgiJduLHdgSOjeyUVRjB5\n"
        "FvjqBUuUfx3CHMjjt/QQQDwTw18fU+hI5Ia0e6E1sHslurjTjqs/OJ0ANACY89Fx\n"
        "lA==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 2 Primary CA (2).cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDAzCCAmwCEQC5L2DMiJ+hekYJuFtwbIqvMA0GCSqGSIb3DQEBBQUAMIHBMQsw\n"
        "CQYDVQQGEwJVUzEXMBUGA1UEChMOVmVyaVNpZ24sIEluYy4xPDA6BgNVBAsTM0Ns\n"
        "YXNzIDIgUHVibGljIFByaW1hcnkgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkgLSBH\n"
        "MjE6MDgGA1UECxMxKGMpIDE5OTggVmVyaVNpZ24sIEluYy4gLSBGb3IgYXV0aG9y\n"
        "aXplZCB1c2Ugb25seTEfMB0GA1UECxMWVmVyaVNpZ24gVHJ1c3QgTmV0d29yazAe\n"
        "Fw05ODA1MTgwMDAwMDBaFw0yODA4MDEyMzU5NTlaMIHBMQswCQYDVQQGEwJVUzEX\n"
        "MBUGA1UEChMOVmVyaVNpZ24sIEluYy4xPDA6BgNVBAsTM0NsYXNzIDIgUHVibGlj\n"
        "IFByaW1hcnkgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkgLSBHMjE6MDgGA1UECxMx\n"
        "KGMpIDE5OTggVmVyaVNpZ24sIEluYy4gLSBGb3IgYXV0aG9yaXplZCB1c2Ugb25s\n"
        "eTEfMB0GA1UECxMWVmVyaVNpZ24gVHJ1c3QgTmV0d29yazCBnzANBgkqhkiG9w0B\n"
        "AQEFAAOBjQAwgYkCgYEAp4gBIXQs5xoD8JjhlzwPIQjxnNuX6Zr8wgQGE75fUsjM\n"
        "HiwSViy4AWkszJkfrbCWrnkE8hM5wXuYuggs6MKEEyyqaekJ9MepAqRCwiNPStjw\n"
        "DqL7MWzJ5m+ZJwf15vRMeJ5t60aG+rmGyVTyssSv1EYcWskVMP8NbPUtDm3Of3cC\n"
        "AwEAATANBgkqhkiG9w0BAQUFAAOBgQByLvl/0fFx+8Se9sVeUYpAmLho+Jscg9ji\n"
        "nb3/7aHmZuovCfTK1+qlK5X2JGCGTUQug6XELaDTrnhpb3LabK4I8GOSN+a7xDAX\n"
        "rXfMSTWqz9iP0b63GJZHc2pUIjRkLbYWm1lbtFFZOrMLFPQS32eg9K0yZF6xRnIn\n"
        "jBJ7xUS0rg==\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 2 Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDAjCCAmsCEB9CKF88iA+OPImzhLOrHxwwDQYJKoZIhvcNAQEFBQAwgcExCzAJ\n"
        "BgNVBAYTAlVTMRcwFQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xh\n"
        "c3MgMiBQdWJsaWMgUHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcy\n"
        "MTowOAYDVQQLEzEoYykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3Jp\n"
        "emVkIHVzZSBvbmx5MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMB4X\n"
        "DTk4MDUxODAwMDAwMFoXDTE4MDUxODIzNTk1OVowgcExCzAJBgNVBAYTAlVTMRcw\n"
        "FQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xhc3MgMiBQdWJsaWMg\n"
        "UHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcyMTowOAYDVQQLEzEo\n"
        "YykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5\n"
        "MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMIGfMA0GCSqGSIb3DQEB\n"
        "AQUAA4GNADCBiQKBgQCniAEhdCznGgPwmOGXPA8hCPGc25fpmvzCBAYTvl9SyMwe\n"
        "LBJWLLgBaSzMmR+tsJaueQTyEznBe5i6CCzowoQTLKpp6Qn0x6kCpELCI09K2PAO\n"
        "ovsxbMnmb5knB/Xm9Ex4nm3rRob6uYbJVPKyxK/URhxayRUw/w1s9S0Obc5/dwID\n"
        "AQABMA0GCSqGSIb3DQEBBQUAA4GBABFFqKR/8eNzIMq97t/1hyORPY2sR0Ua3m3b\n"
        "VCHODoMO+NzlQ9XrLmGRI+JyADRV98TPETPdweQiI1xQUxn4ZOf3CQ9FUaBXK9+8\n"
        "Imb+MXB7JToPxYp+w7tyAczwvU1SgaQbWFhT1VM69Q5q2umvxOFY80JvVGJHrDGU\n"
        "0Q3O7x0x\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 2 Public Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICPDCCAaUCEC0b/EoXjaOR6+f/9YtFvgswDQYJKoZIhvcNAQECBQAwXzELMAkG\n"
        "A1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMTcwNQYDVQQLEy5DbGFz\n"
        "cyAyIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTk2\n"
        "MDEyOTAwMDAwMFoXDTI4MDgwMTIzNTk1OVowXzELMAkGA1UEBhMCVVMxFzAVBgNV\n"
        "BAoTDlZlcmlTaWduLCBJbmMuMTcwNQYDVQQLEy5DbGFzcyAyIFB1YmxpYyBQcmlt\n"
        "YXJ5IENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIGfMA0GCSqGSIb3DQEBAQUAA4GN\n"
        "ADCBiQKBgQC2WoujDWojg4BrzzmH9CETMwZMJaLtVRKXxaeAufqDwSCg+i8VDXyh\n"
        "YGt+eSz6Bg86rvYbb7HS/y8oUl+DfUvEerf4Zh+AVPy3wo5ZShRXRtGak75BkQO7\n"
        "FYCTXOvnzAhsPz6zSvz/S2wj1VCCJkQZjiPDceoZJEcEnnW/yKYAHwIDAQABMA0G\n"
        "CSqGSIb3DQEBAgUAA4GBAIobK/o5wXTXXtgZZKJYSi034DNHD6zt96rbHuSLBlxg\n"
        "J8pFUs4W7z8GZOeUaHxgMxURaa+dYo2jA1Rrpr7l7gUYYAS/QoD90KioHgE796Nc\n"
        "r6Pc5iaAIzy4RHT3Cq5Ji2F4zCS/iIqnDupzGUH9TQPwiNHleI2lKk/2lw0Xd8rY\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 3 Primary CA (3).cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDAjCCAmsCEH3Z/gfPqB63EHln+6eJNMYwDQYJKoZIhvcNAQEFBQAwgcExCzAJ\n"
        "BgNVBAYTAlVTMRcwFQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xh\n"
        "c3MgMyBQdWJsaWMgUHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcy\n"
        "MTowOAYDVQQLEzEoYykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3Jp\n"
        "emVkIHVzZSBvbmx5MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMB4X\n"
        "DTk4MDUxODAwMDAwMFoXDTI4MDgwMTIzNTk1OVowgcExCzAJBgNVBAYTAlVTMRcw\n"
        "FQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xhc3MgMyBQdWJsaWMg\n"
        "UHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcyMTowOAYDVQQLEzEo\n"
        "YykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5\n"
        "MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMIGfMA0GCSqGSIb3DQEB\n"
        "AQUAA4GNADCBiQKBgQDMXtERXVxp0KvTuWpMmR9ZmDCOFoUgRm1HP9SFIIThbbP4\n"
        "pO0M8RcPO/mn+SXXwc+EY/J8Y8+iR/LGWzOOZEAEaMGAuWQcRXfH2G71lSk8UOg0\n"
        "13gfqLptQ5GVj0VXXn7F+8qkBOvqlzdUMG+7AUcyM83cV5tkaWH4mx0ciU9cZwID\n"
        "AQABMA0GCSqGSIb3DQEBBQUAA4GBAFFNzb5cy5gZnBWyATl4Lk0PZ3BwmcYQWpSk\n"
        "U01UbSuvDV1Ai2TT1+7eVmGSX6bEHRBhNtMsJzzoKQm5EWR0zLVznxxIqbxhAe7i\n"
        "F6YM40AIOw7n60RzKprxaZLvcRTDOaxxp5EJb+RxBrO6WVcmeQD2+A2iMzAo1KpY\n"
        "oJ2daZH9\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 3 Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDAjCCAmsCEEakM712H2pJ5qjDp/WFQPUwDQYJKoZIhvcNAQEFBQAwgcExCzAJ\n"
        "BgNVBAYTAlVTMRcwFQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xh\n"
        "c3MgMyBQdWJsaWMgUHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcy\n"
        "MTowOAYDVQQLEzEoYykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3Jp\n"
        "emVkIHVzZSBvbmx5MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMB4X\n"
        "DTk4MDUxODAwMDAwMFoXDTE4MDUxODIzNTk1OVowgcExCzAJBgNVBAYTAlVTMRcw\n"
        "FQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xhc3MgMyBQdWJsaWMg\n"
        "UHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcyMTowOAYDVQQLEzEo\n"
        "YykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5\n"
        "MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMIGfMA0GCSqGSIb3DQEB\n"
        "AQUAA4GNADCBiQKBgQDMXtERXVxp0KvTuWpMmR9ZmDCOFoUgRm1HP9SFIIThbbP4\n"
        "pO0M8RcPO/mn+SXXwc+EY/J8Y8+iR/LGWzOOZEAEaMGAuWQcRXfH2G71lSk8UOg0\n"
        "13gfqLptQ5GVj0VXXn7F+8qkBOvqlzdUMG+7AUcyM83cV5tkaWH4mx0ciU9cZwID\n"
        "AQABMA0GCSqGSIb3DQEBBQUAA4GBABB79Ik/3D0LuwBM6zQoy/0HqUNphvJLAKTH\n"
        "1diwgngO7ZY8ZnsHB+E+c/Z+csjFQd0pSFxj6zb0dS7FBI2qu7a3FKWAZkY9AQzS\n"
        "wAC1SBtLHfQpR6g8QhdYLXh7IFACJ0ubJwvt8y9UJnNI8CWpifefyaqKYbfKDD3W\n"
        "hHcGFOgV\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 3 Public Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICPDCCAaUCEHC65B0Q2Sk0tjjKewPMur8wDQYJKoZIhvcNAQECBQAwXzELMAkG\n"
        "A1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMTcwNQYDVQQLEy5DbGFz\n"
        "cyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTk2\n"
        "MDEyOTAwMDAwMFoXDTI4MDgwMTIzNTk1OVowXzELMAkGA1UEBhMCVVMxFzAVBgNV\n"
        "BAoTDlZlcmlTaWduLCBJbmMuMTcwNQYDVQQLEy5DbGFzcyAzIFB1YmxpYyBQcmlt\n"
        "YXJ5IENlcnRpZmljYXRpb24gQXV0aG9yaXR5MIGfMA0GCSqGSIb3DQEBAQUAA4GN\n"
        "ADCBiQKBgQDJXFme8huKARS0EN8EQNvjV69qRUCPhAwL0TPZ2RHP7gJYHyX3KqhE\n"
        "BarsAx94f56TuZoAqiN91qyFomNFx3InzPRMxnVx0jnvT0Lwdd8KkMaOIG+YD/is\n"
        "I19wKTakyYbnsZogy1Olhec9vn2a/iRFM9x2Fe0PonFkTGUugWhFpwIDAQABMA0G\n"
        "CSqGSIb3DQEBAgUAA4GBALtMEivPLCYATxQT3ab7/AoRhIzzKBxnki98tsX63/Do\n"
        "lbwdj2wsqFHMc9ikwFPwTtYmwHYBV4GSXiHx0bH/59AhWM1pF+NEHJwZRDmJXNyc\n"
        "AA9WjQKZ7aKQRUzkuxCkPfAyAw7xzvjoyVGM5mKf5p/AfbdynMk2OmufTqj/ZA1k\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 4 Primary CA (5).cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDAjCCAmsCEDKIjprS9esTR/h/xCA3JfgwDQYJKoZIhvcNAQEFBQAwgcExCzAJ\n"
        "BgNVBAYTAlVTMRcwFQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xh\n"
        "c3MgNCBQdWJsaWMgUHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcy\n"
        "MTowOAYDVQQLEzEoYykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3Jp\n"
        "emVkIHVzZSBvbmx5MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMB4X\n"
        "DTk4MDUxODAwMDAwMFoXDTI4MDgwMTIzNTk1OVowgcExCzAJBgNVBAYTAlVTMRcw\n"
        "FQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xhc3MgNCBQdWJsaWMg\n"
        "UHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcyMTowOAYDVQQLEzEo\n"
        "YykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5\n"
        "MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMIGfMA0GCSqGSIb3DQEB\n"
        "AQUAA4GNADCBiQKBgQC68OTP+cSuhVS5B1f5j8V/aBH4xBewRNzjMHPVKmIquNDM\n"
        "HO0oW369atyzkSTKQWI8/AIBvxwWMZQFl3Zuoq29YRdsTjCG8FE3KlDHqGKB3FtK\n"
        "qsGgtG7rL+VXxbErQHDbWk2hjh+9Ax/YA9SPTJlxvOKCzFjomDqG04Y48wApHwID\n"
        "AQABMA0GCSqGSIb3DQEBBQUAA4GBAIWMEsGnuVAVess+rLhDityq3RS6iYF+ATwj\n"
        "cSGIL4LcY/oCRaxFWdcqWERbt5+BO5JoPeI3JPV7bI92NZYJqFmduc4jq3TWg/0y\n"
        "cyfYaT5DdPauxYma51N86Xv2S/PBZYPejYqcPIiNOVn8qj8ijaHBZlCBckztImRP\n"
        "T8qAkbYp\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign Class 4 Primary CA.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDAjCCAmsCEH62EasyyEHbXqwB45We//0wDQYJKoZIhvcNAQEFBQAwgcExCzAJ\n"
        "BgNVBAYTAlVTMRcwFQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xh\n"
        "c3MgNCBQdWJsaWMgUHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcy\n"
        "MTowOAYDVQQLEzEoYykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3Jp\n"
        "emVkIHVzZSBvbmx5MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMB4X\n"
        "DTk4MDUxODAwMDAwMFoXDTE4MDUxODIzNTk1OVowgcExCzAJBgNVBAYTAlVTMRcw\n"
        "FQYDVQQKEw5WZXJpU2lnbiwgSW5jLjE8MDoGA1UECxMzQ2xhc3MgNCBQdWJsaWMg\n"
        "UHJpbWFyeSBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eSAtIEcyMTowOAYDVQQLEzEo\n"
        "YykgMTk5OCBWZXJpU2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5\n"
        "MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMIGfMA0GCSqGSIb3DQEB\n"
        "AQUAA4GNADCBiQKBgQC68OTP+cSuhVS5B1f5j8V/aBH4xBewRNzjMHPVKmIquNDM\n"
        "HO0oW369atyzkSTKQWI8/AIBvxwWMZQFl3Zuoq29YRdsTjCG8FE3KlDHqGKB3FtK\n"
        "qsGgtG7rL+VXxbErQHDbWk2hjh+9Ax/YA9SPTJlxvOKCzFjomDqG04Y48wApHwID\n"
        "AQABMA0GCSqGSIb3DQEBBQUAA4GBAIpoYody54SQCQLbALEo/xjFtWNkmivB0vdf\n"
        "6B8Mr8nr9R2hBH4/46aOxx39KBB1fD/K+RcmJ28rwdvJw2t8psbc5VqN++9AyIfv\n"
        "GVA2V4fx8j/l6qOWmZetma87v4S31Z5siwqj7+di+6Z36C9Oht0xovPKVpjsh57Z\n"
        "gq+EUkHo\n"
        "-----END CERTIFICATE-----\n",

        // ./root/VeriSign.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIE0zCCA7ugAwIBAgIQGNrRniZ96LtKIVjNzGs7SjANBgkqhkiG9w0BAQUFADCB\n"
        "yjELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQL\n"
        "ExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJp\n"
        "U2lnbiwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxW\n"
        "ZXJpU2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0\n"
        "aG9yaXR5IC0gRzUwHhcNMDYxMTA4MDAwMDAwWhcNMzYwNzE2MjM1OTU5WjCByjEL\n"
        "MAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMuMR8wHQYDVQQLExZW\n"
        "ZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTowOAYDVQQLEzEoYykgMjAwNiBWZXJpU2ln\n"
        "biwgSW5jLiAtIEZvciBhdXRob3JpemVkIHVzZSBvbmx5MUUwQwYDVQQDEzxWZXJp\n"
        "U2lnbiBDbGFzcyAzIFB1YmxpYyBQcmltYXJ5IENlcnRpZmljYXRpb24gQXV0aG9y\n"
        "aXR5IC0gRzUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCvJAgIKXo1\n"
        "nmAMqudLO07cfLw8RRy7K+D+KQL5VwijZIUVJ/XxrcgxiV0i6CqqpkKzj/i5Vbex\n"
        "t0uz/o9+B1fs70PbZmIVYc9gDaTY3vjgw2IIPVQT60nKWVSFJuUrjxuf6/WhkcIz\n"
        "SdhDY2pSS9KP6HBRTdGJaXvHcPaz3BJ023tdS1bTlr8Vd6Gw9KIl8q8ckmcY5fQG\n"
        "BO+QueQA5N06tRn/Arr0PO7gi+s3i+z016zy9vA9r911kTMZHRxAy3QkGSGT2RT+\n"
        "rCpSx4/VBEnkjWNHiDxpg8v+R70rfk/Fla4OndTRQ8Bnc+MUCH7lP59zuDMKz10/\n"
        "NIeWiu5T6CUVAgMBAAGjgbIwga8wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8E\n"
        "BAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2gWzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAH\n"
        "BgUrDgMCGgQUj+XTGoasjY5rw8+AatRIGCx7GS4wJRYjaHR0cDovL2xvZ28udmVy\n"
        "aXNpZ24uY29tL3ZzbG9nby5naWYwHQYDVR0OBBYEFH/TZafC3ey78DAJ80M5+gKv\n"
        "MzEzMA0GCSqGSIb3DQEBBQUAA4IBAQCTJEowX2LP2BqYLz3q3JktvXf2pXkiOOzE\n"
        "p6B4Eq1iDkVwZMXnl2YtmAl+X6/WzChl8gGqCBpH3vn5fJJaCGkgDdk+bW48DW7Y\n"
        "5gaRQBi5+MHt39tBquCWIMnNZBU4gcmU7qKEKQsTb47bDN0lAtukixlE0kF6BWlK\n"
        "WE9gyn6CagsCqiUXObXbf+eEZSqVir2G3l6BFoMtEMze/aiCKm0oHw0LxOXnGiYZ\n"
        "4fQRbxC1lfznQgUy286dUV4otp6F01vvpX1FQHKOtw5rDgb7MzVIcbidJ4vEZV8N\n"
        "hnacRHr2lVz2XTIIM6RUthg/aFzyQkqFOFSDX9HoLPKsEdao7WNq\n"
        "-----END CERTIFICATE-----\n",

        // ./root/ViaCode Certification Authority.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDFzCCAoCgAwIBAgIENuetnDANBgkqhkiG9w0BAQUFADBAMQswCQYDVQQGEwJH\n"
        "QjEQMA4GA1UEChMHVmlhQ29kZTEQMA4GA1UECxMHQ0EgRGF0YTENMAsGA1UECxME\n"
        "Q0EgMTAeFw05OTAzMTExMTE4NDhaFw0xOTAzMTExMTQ4NDhaMEAxCzAJBgNVBAYT\n"
        "AkdCMRAwDgYDVQQKEwdWaWFDb2RlMRAwDgYDVQQLEwdDQSBEYXRhMQ0wCwYDVQQL\n"
        "EwRDQSAxMIGdMA0GCSqGSIb3DQEBAQUAA4GLADCBhwKBgQCws2enlrV2g+kWA9ka\n"
        "oHbVdOecBdJRP9tPGaGoOU7LJH1hxB5qIK4Pgd7quDn9Gx9rNkDtTSEl8qPZoVHY\n"
        "bMAblvjUQpTUp84bj9NUJqKE7zKFr0o/8TI2rz3mOifrA8IlfvRhK62KGkvmmzZo\n"
        "1C/l0oiU3Baq2sIVTGzD4RmRyQIBA6OCAR4wggEaMBEGCWCGSAGG+EIBAQQEAwIA\n"
        "BzBiBgNVHR8EWzBZMFegVaBTpFEwTzELMAkGA1UEBhMCR0IxEDAOBgNVBAoTB1Zp\n"
        "YUNvZGUxEDAOBgNVBAsTB0NBIERhdGExDTALBgNVBAsTBENBIDExDTALBgNVBAMT\n"
        "BENSTDEwKwYDVR0QBCQwIoAPMTk5OTAzMTExMTE4NDhagQ8yMDE5MDMxMTExMTg0\n"
        "OFowCwYDVR0PBAQDAgEGMB8GA1UdIwQYMBaAFCiHsaeIf9bdy2+hHA5a0C4VMZ3M\n"
        "MB0GA1UdDgQWBBQoh7GniH/W3ctvoRwOWtAuFTGdzDAMBgNVHRMEBTADAQH/MBkG\n"
        "CSqGSIb2fQdBAAQMMAobBFY0LjADAgSQMA0GCSqGSIb3DQEBBQUAA4GBAFPf0unw\n"
        "7uPaAGeP6hVHdh/gtaSgRwLCs9i0StT7rByBuSMgY2LYVG8JddY65jCrsXO8FKT+\n"
        "RC9SZNAWYy05tuk47mUzkNGBGIAFZyNvPB1CfNHx84ag+stWbL4b9TKcYZO73+3A\n"
        "T8HR5UXO40QViNfMw/yrGJZR9OCofMHLmLzB\n"
        "-----END CERTIFICATE-----\n",

        // ./root/www.example.com.cer
        "-----BEGIN CERTIFICATE-----\n"
        "MIICjDCCAfWgAwIBAgIJAP5JtVUwsFO0MA0GCSqGSIb3DQEBBQUAMF8xCzAJBgNV\n"
        "BAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBX\n"
        "aWRnaXRzIFB0eSBMdGQxGDAWBgNVBAMMD3d3dy5leGFtcGxlLmNvbTAeFw0xMjAy\n"
        "MTYwNjUyMjZaFw0yMjAyMTMwNjUyMjZaMF8xCzAJBgNVBAYTAkFVMRMwEQYDVQQI\n"
        "DApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQx\n"
        "GDAWBgNVBAMMD3d3dy5leGFtcGxlLmNvbTCBnzANBgkqhkiG9w0BAQEFAAOBjQAw\n"
        "gYkCgYEAowQ6D2Ea05A+Hix7MlC2pwSQHEOV6bDvtO0v8FKqWhw1KgUR7dShciSp\n"
        "ZuZDyI4lE3nqLXoMQx8Z+0LqO7PGql47JdyoYEA5DfN0GIRqGq8qjMKxQCQJTIkV\n"
        "vkdDDrlL/8OPeTm9+kRfRxB0H6CSNxPQ7y6Ych4aBmWt4Mq+c/sCAwEAAaNQME4w\n"
        "HQYDVR0OBBYEFNvo/CeYyrruUo1u2SfaOG+nbkmfMB8GA1UdIwQYMBaAFNvo/CeY\n"
        "yrruUo1u2SfaOG+nbkmfMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEFBQADgYEA\n"
        "fKyA2DotoiT45lDLoIyy/DPUx/RCqwyO7W6jfYFS2F+F50hm02MUS6ocwvs8peFW\n"
        "jd+kPRY8blsEnLFpm9c9kmAfpsh4FY6waHbvYyvcygCrk0zWQM2/PNcG9yBdiNdE\n"
        "rxEsU7tPYLGCKjKmpcdUnYrmipN/53A3Uxej4kmkyaA=\n"
        "-----END CERTIFICATE-----\n",

		// Another cert for testing only
		"-----BEGIN CERTIFICATE-----\n"
		"MIIB1jCCAUOgAwIBAgIQVLa5JFRmurhFtNzsYxYJIjAJBgUrDgMCHQUAMBkxFzAV\n"
		"BgNVBAMTDnRlc3QuanFiYXIuY29tMB4XDTEyMDQxMjAzMTUyMloXDTIyMDQxMDAz\n"
		"MTUyMlowGTEXMBUGA1UEAxMOdGVzdC5qcWJhci5jb20wgZ8wDQYJKoZIhvcNAQEB\n"
		"BQADgY0AMIGJAoGBANwqg4+cGE2V2GVqkDMT+aNE+a/nbydnAuQQ2HfCR4TNnOr0\n"
		"/dN4n/VM0h/AIVgmIKot1UoN2UEzLFHTwX+w+SCTtdClpfhvPHLYl7qC/Y75qK2h\n"
		"kG7TNAMn+Z4pxEsp/jjVwroq7YR7CkE+9ZzsjhYdKd6BKbMz/5Ho7rJtRZ+NAgMB\n"
		"AAGjJzAlMBMGA1UdJQQMMAoGCCsGAQUFBwMBMA4GA1UdDwQHAwUAsAAAADAJBgUr\n"
		"DgMCHQUAA4GBADo3+XSxi6PsRCH5CXo6LBHSmGeNrL7/EnQxM39uUL0FyWz6J7RS\n"
		"uk6ohJbJE1oYv7mz9c87uCYwkZpK4oVZQXdrFf7/dcX6iayatry4kDUh36M2aczd\n"
		"W3h8CSSZmkxW3RJSXgNYy15CdKluJslrFB9Uoz8tn8CuPGP1vzZTPInp\n"
		"-----END CERTIFICATE-----\n"
	};
	
	BIO * b = BIO_new_file(SSLAPI_GetTestCaCertFileName(), "w");
	if (b)
	{
		for (int i=0; i<sizeof(builtinCerts)/sizeof(builtinCerts[0]); ++i)
		{
			BIO_printf(b, "%s", builtinCerts[i]);
		}
		BIO_free(b);
		return SAERR_OK;
	}

	return SAERR_ERROR;
}


void SSLAPI_DeleteTestCaCertFile()
{
	unlink(SSLAPI_GetTestCaCertFileName());
}



