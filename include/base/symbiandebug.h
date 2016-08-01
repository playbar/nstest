#ifndef __SYMBIANDEBUG_H_
#define __SYMBIANDEBUG_H_

#define __STARTDEBUGLOG__

//��������
//#define _LostSound
//����ͼ��XImage
//#define _LostImage
//�ڴ�ص���XXVar
//#define _XXVarDebug
//�ű���������splay ScriptThread
//#define _ScriptThreadDebug
//�������
//#define _NetLog
//��������
//#define _SoundLog
//default thread
//#define _DefaultThread

//����log����
//#define _SYMBIANDEBUG
#define DefaultLog "c:/Data/GGClient.log"

#ifdef _SYMBIAN
	#ifdef __STARTDEBUGLOG__
	#include <stdio.h>
	#include <e32std.h>
	#include <e32cmn.h>
	
	#define SYMBIANDEB(filename,s1) {FILE *fl; \
										fl=fopen(filename,"a"); \
										if(fl){ fprintf(fl,"file:%s	line:%d function:%s\r\n",__FILE__,__LINE__,__FUNCTION__) ; \
										fprintf(fl,s1); fclose(fl);}}
	
	#define SYMBIANDEB1(filename,s1,s2) {FILE *fl; \
										fl=fopen(filename,"a"); \
										if(fl){ fprintf(fl,"file:%s	line:%d function:%s\r\n",__FILE__,__LINE__,__FUNCTION__) ; \
										fprintf(fl,s1,s2); fclose(fl);}}
	
	#define SYMBIANDEB2(filename,s1,s2,s3) {FILE *fl; \
										fl=fopen(filename,"a"); \
										if(fl){ fprintf(fl,"file:%s	line:%d function:%s\r\n",__FILE__,__LINE__,__FUNCTION__) ; \
										fprintf(fl,s1,s2,s3); fclose(fl);}}
	
	#define SYMBIANDEB3(filename,s1,s2,s3,s4) {FILE *fl; \
										fl=fopen(filename,"a"); \
										if(fl){ fprintf(fl,"file:%s	line:%d function:%s\r\n",__FILE__,__LINE__,__FUNCTION__) ; \
										fprintf(fl,s1,s2,s3,s4); fclose(fl);}}
	
	#define SYMBIANDEBS(filename,buf,size) {FILE *fl; \
											fl=fopen(filename,"ab"); \
											if(fl){ fwrite(buf,1,size,fl); \
													fclose(fl);}}
	#define SYMBIANDEBM(filename,msg) {FILE *fl; \
										fl=fopen(filename,"a"); \
										if(fl){ fprintf(fl,"file:%s	line:%d function:%s\r\n",__FILE__,__LINE__,__FUNCTION__) ; \
												RProcess aProcess; \
												TProcessMemoryInfo info; \
												aProcess.GetMemoryInfo(info); \
												TUint32 memsize = (info.iCodeSize + \
														info.iConstDataSize + \
														info.iInitialisedDataSize + \
														info.iUninitialisedDataSize); \
												fprintf(fl,"%s mem:%d\r\n",msg,memsize);\
												fclose(fl);} \
										}
	#else
	#define SYMBIANDEB(filename,s1)
	#define SYMBIANDEB1(filename,s1,s2)
	#define SYMBIANDEB2(filename,s1,s2,s3)
	#define SYMBIANDEB3(filename,s1,s2,s3,s4)
	#define SYMBIANDEBS(filename,buf,size)
	#endif
#elif defined(_WIN32)
	#ifdef _DEBUG
		#ifdef __STARTDEBUGLOG__
			#include <stdio.h>
			
			#define SYMBIANDEB(filename,s1) {FILE *fl; \
												fl=fopen(filename,"a"); \
												if(fl){ fprintf(fl,s1); fclose(fl);}}
			
			#define SYMBIANDEB1(filename,s1,s2) {FILE *fl; \
												fl=fopen(filename,"a"); \
												if(fl){ fprintf(fl,s1,s2); fclose(fl);}}
			
			#define SYMBIANDEB2(filename,s1,s2,s3) {FILE *fl; \
												fl=fopen(filename,"a"); \
												if(fl){ fprintf(fl,s1,s2,s3); fclose(fl);}}
			
			#define SYMBIANDEB3(filename,s1,s2,s3,s4) {FILE *fl; \
												fl=fopen(filename,"a"); \
												if(fl){ fprintf(fl,s1,s2,s3,s4); fclose(fl);}}
		
			#define SYMBIANDEBS(filename,buf,size) {FILE *fl; \
												fl=fopen(filename,"ab"); \
												if(fl){fwrite(buf,1,size,fl); \
														fclose(fl);}}
		#else
			#define SYMBIANDEB(filename,s1)
			#define SYMBIANDEB1(filename,s1,s2)
			#define SYMBIANDEB2(filename,s1,s2,s3)
			#define SYMBIANDEB3(filename,s1,s2,s3,s4)
			#define SYMBIANDEBS(filename,buf,size)
		#endif
	#endif
#else
#define SYMBIANDEB
#define SYMBIANDEB1
#define SYMBIANDEB2
#define SYMBIANDEB3
#define SYMBIANDEBS
#endif
#endif           //file end
