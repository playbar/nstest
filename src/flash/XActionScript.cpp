// XActionScript.cpp: implementation of the XActionScript class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XActionScript.h"
#include "ObjectClip.h"
#include "ObjectCreator.h"
//#include <afx.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------*/
/* Coefficients used in the pure random number generator. */

#define c3  15731L
#define c2  789221L
#define c1  1376312589L

/* The pure random number generator returns random numbers between zero
/  and this number.  This value is useful to know for scaling random
/  numbers to a desired range. */
#define kRandomPureMax  0x7FFFFFFFL

/* Data structure for state of fast random number generator. */
typedef struct _XRandomFast
{
    /* Random result and seed for next random result. */
    U32  uValue;

    /* XOR mask for generating the next random value. */
    U32  uXorMask;

    /* This is the number of values which will be generated in the
    /  sequence given the initial value of n. */
    U32  uSequenceLength;

} TRandomFast, *pTRandomFast;

/* Read-only, XOR masks for random # generation. */
static U32 Random_Xor_Masks[31] =
{
    /* First mask is for generating sequences of 3 (2^2 - 1) random numbers,
    /  Second mask is for generating sequences of 7 (2^3 - 1) random numbers,
    /  etc.  Numbers to the right are number of bits of random number sequence.
    /  It generates 2^n - 1 numbers. */
    0x00000003L, 0x00000006L, 0x0000000CL, 0x00000014L,     /* 2,3,4,5 */
    0x00000030L, 0x00000060L, 0x000000B8L, 0x00000110L,     /* 6,7,8,9 */
    0x00000240L, 0x00000500L, 0x00000CA0L, 0x00001B00L,     /* 10,11,12,13 */
    0x00003500L, 0x00006000L, 0x0000B400L, 0x00012000L,     /* 14,15,16,17 */
    0x00020400L, 0x00072000L, 0x00090000L, 0x00140000L,     /* 18,19,20,21 */
    0x00300000L, 0x00400000L, 0x00D80000L, 0x01200000L,     /* 22,23,24,25 */
    0x03880000L, 0x07200000L, 0x09000000L, 0x14000000L,     /* 26,27,28,29 */
    0x32800000L, 0x48000000L, 0xA3000000L                   /* 30,31,32 */

};                              /* Randomizer Xor mask values  CRK 10-29-90 */
/*-------------------------------------------------------------------------*/

TRandomFast gRandomFast = { 0, 0, 0};

/*-*-------------------------------------------------------------------------
/ Function
/   RandomFastInit
/
/ Purpose
/   This initializes the fast random number generator.
/
/ Notes
/   The fast random number generator has some unique qualities:
/
/   1) It generates an exact and repeatable sequence of pseudorandom
/      integers between 1 and 2^n-1, inclusive.  Zero is not generated.
/   2) The same sequence repeats every 2^n-1 generations.
/   3) Each number can be generated extremely rapidly.
/
/   This returns the first random number in the sequence.
/
/ Entry
/   pRandomFast = Pointer to data structure for current state of a fast
/     pseudorandom number generator.
/   n = A value which sets the number of unique values to generate before
/     repeating.  This value can range from 2 to 32, inclusive.  If an
/     invalid value (such as zero) is passed in, 32 is assumed.
/--------------------------------------------*/
void RandomFastInit(pTRandomFast pRandomFast)
{
    S32 n = 32;

    /* The sequence always starts with 1. */
    //    pRandomFast->uValue = 1L;
    pRandomFast->uValue = XTime::GetTimeMSec();

    /* Figure out the sequence length (2^n - 1). */
    pRandomFast->uSequenceLength = (1L << n) - 1L;

    /* Gather the XOR mask value. */
    pRandomFast->uXorMask = Random_Xor_Masks[n - 2];
}
/*-------------------------------------------------------------------------*/

/*-*-------------------------------------------------------------------------
/ Function
/   imRandomFastNext
/
/ Purpose
/   This generates a new pseudorandom number using the fast random number
/   generator.
/
/ Notes
/   The fast random number generator must be initialized before use.
/
/   This is implemented as a macro for the best possible performance.
/
/ Entry
/   pRandomFast = Pointer to data structure for current state of a fast
/     pseudorandom number generator.
/
/ Exit
/   Returns the next pseudorandom number in the sequence.
/--------------------------------------------*/
#define RandomFastNext(_pRandomFast)                                                            \
(                                                                                               \
    ((_pRandomFast)->uValue & 1L)                                                               \
        ? ((_pRandomFast)->uValue = ((_pRandomFast)->uValue >> 1) ^ (_pRandomFast)->uXorMask)   \
        : ((_pRandomFast)->uValue = ((_pRandomFast)->uValue >> 1))                              \
)
/*-------------------------------------------------------------------------*/

/*-*-------------------------------------------------------------------------
/ Function
/   RandomPureHasher
/
/ Purpose
/   This generates a pseudorandom number from a given seed using the high
/   quality random number generator.
/
/ Notes
/   The caller must pass in a seed value.  This value is typically the
/   output of the fast random number generator multiplied by a prime
/   number, but can be any seed which was not derived from the output of
/   this function.
/
/   Please note that this random number generator is really as hasher.
/   It will not work well if you pass its own output in as the next input.
/
/   A given input seed always generates the same pseudorandom output result.
/
/   The feature of this hasher is that the value returned from one seed
/   to the next is highly uncorrelated to the seed value.  High quality
/   multidimensional random numbers can be generated by using a sum of
/   prime multiples of the seed values.
/
/   For example, to generate a vector given three seed values sx, sy and sz,
/   use something like the following:
/     result = imRandomPureHasher(59*sx + 67*sy + 71*sz);
/
/ Entry
/   iSeed = Starting seed value.
/
/ Exit
/   Returns the next pseudorandom value in the sequence.
/--------------------------------------------*/
S32 RandomPureHasher(S32 iSeed)
{
    S32   iResult;

    /* Adapted from "A Recursive Implementation of the Perlin Noise Function,"
    /  by Greg Ward, Graphics Gems II, p. 396. */

    /* First, hash s as a preventive measure.  This helps ensure the first
    /  few numbers are more random when used in conjunction with the fast
    /  random number generator, which starts off with the first 10 or so
    /  numbers all zero in the lowe bits. */
    iSeed = ((iSeed << 13) ^ iSeed) - (iSeed >> 21);

    /* Next, use a third order odd polynomial, better than linear. */
    iResult = (iSeed*(iSeed*iSeed*c3 + c2) + c1) & kRandomPureMax;

    /* DJ14dec94 -- The above wonderful expression always returns odd
    /  numbers, and this is easy to prove.  So we add the seed back to
    /  the result which again randomizes bit zero. */
    iResult += iSeed;

    /* DJ17nov95 -- The above always returns values that are NEVER divisible
    /  evenly by four, so do additional hashing. */
    iResult = ((iResult << 13) ^ iResult) - (iResult >> 21);

    return iResult;
}
/* ------------------------------------------------------------------------------ */
S32 Random(S32 range)
{
    long    aNum;

    /* Fill out gRandomFast if it is uninitialized.
    /  This means seed hasn't been set.  Sequence of numbers will be
    /  the same every time player is run. */
    if (gRandomFast.uValue == 0)
    {
        /* Initialize 32 bit pure random number generator. */
        RandomFastInit(&gRandomFast);
    }

    aNum = RandomFastNext(&gRandomFast);

    /* Use the pure random number generator to hash the result. */
    aNum = RandomPureHasher(aNum * 71L);

    if (range > 0)
    {
        aNum = ( (aNum&0x7FFFFFFF) % range);
    }
    else
        aNum = 0;

    return aNum;
}
/* Button Semaphore (unused) */



XActionScript::~XActionScript()
{
//	m_locals.FreeAll();
	FREEOBJECT(m_baseThread);
	FREEOBJECT(m_thread);
	FREEOBJECT(m_pThis);
	FREEOBJECT(m_pObject);
	FREEOBJECT(m_pWith);
}

//static int nCount=0;
#ifdef _VARCACHE_
XBOOL XActionScript::CacheGetVariable(XXVar&name,XSWFCONTEXT&cnt,XXVar&var)
{
	if(name.nStringType!=STRING_CONST||name.nRefID>=MAX_CACHES) 
			return XFALSE;
	if(m_caches[name.nRefID].pObject==XNULL) return XFALSE;
	
	_VARCACHE&c=m_caches[name.nRefID];
	if(c.pObjectThis==XNULL)
	{
		//(objName.strTxt,cnt.thread,cnt.pThis,cnt.pObject,var);
		return m_pPlayer->GetGlobal(name.strTxt,cnt.thread,cnt.pThis,cnt.pObject,var);
	}
	cnt.varCache=c;
	return c.pObjectThis->GetMemberCache(cnt,name.strTxt,var);	
	//return XTRUE;
}
#endif

static int nDebugCount=0;

XBOOL XActionScript::DoAction(XSWFCONTEXT&context,XXObject*pLocalObj,XBOOL bBlock,XBOOL bInit)
{
//	return 0;
	if(!m_pCodeBuf||!m_nCodeLength) return XFALSE;
#ifdef _ANALY
	/*XXVar var("DoAction");
	if(context.pThis&&context.pThis->m_pObject)
	{
		XXVar idd=context.pThis->m_pObject->character->tag;
		idd.ToString(XFALSE);
		var.ToString(XTRUE);
		//XXVar str("DoAction:");
		//str.StringAdd(var);
		var.AddChar(':');
		var.StringAdd(idd);
		//_ANALYENTER(str.strTxt);
	}*/
	//else
	//{
	//_ANALYENTER(var.strTxt);//"DoAction");
	_ANALYENTER("DoAction");
	//}
#endif
	m_bInit=bInit;

	m_bBlock=bBlock;
	m_pContext=&context;
	XXObject*pNewObj=XNULL;
//	m_pBlock=pBlock;
	m_pLocalObj=pLocalObj;
	if(!pLocalObj&&m_bBlock)
	{
		pNewObj=new XXObject(m_baseThread->m_pRoot);
		m_pLocalObj=pNewObj->RefObject();
	}

	m_pPlayer=m_pContext->pPlayer;
	m_pStack=context.pStack;

//	if(m_thread&&m_thread->m_pObject&&m_thread->m_pObject->character&&m_thread->m_pObject->character->tag==179)
//		int v=0;
//	m_bBlock=bBlock;
//	if(m_pThis&&m_pThis->m_pObject&&
//	   m_pThis->m_pObject->character->tag==161)
//	   int v=0;

	//if(m_thread->m_pObject&&
      // m_thread->m_pObject->character->tag==450)
	   //int v=0;
//	if(m_pThis->m_pObject&&
//	   m_pThis->m_pObject->character->tag==343)
//		int v=0;

	m_pContext->nCallDepth++;
	if(m_pContext->nCallDepth>=0xff)
	{
		m_pContext->nCallDepth--;
		m_pContext->actionOverflow=XTRUE;
//		_ANALYLEAVE();
		return 0;
	}

	SParser parser;
	parser.Attach(m_pCodeBuf, 0);

//	if(m_thread->m_pObject->character->type==98)
//		int v=0;
//	if(m_pThis->m_pObject->character->tag==55)
//		int v=0;
//	if(m_thread->m_pObject->character->tag==13)
//		int v=0;

	m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
	int skipCount = 0,ret=0,bReturn=0;
	//ScriptThread*baseThread=m_pCnt->contexts.GetLast();
	//XSWFPlayer*m_pPlayer=m_pCnt->m_pPlayer;
//	XXStack*m_pStack=m_pCnt->m_pStack;
	int l=(int)m_nCodeLength;
//	if(m_thread->m_pObject->character->tag==1002&&
//	   strcmp(m_thread->m_pObject->name,"laser"))
//		int v=0;
	XBOOL bECMA=m_baseThread->m_pRoot->version>=7;
	if(l<0) l=0;
	//if(l>1000)
	//	int v=0;
//	XU8*pKeep=m_pCodeBuf;
//	XBOOL bString=m_pContext->regObjs[2].IsString();
//	XXVar de=m_pContext->regObjs[2];

	//int sss=m_pStack->m_nSize;

	for (;;) {
		// Handle the action
		if(l&&parser.pos>=l) 
			break;

	//	if(m_pStack->m_nSize<sss)
		//	int v=0;
//		if(!m_thread->m_pObject)
//			int v=0;
		//if(parser.pos>=560)
		//	int v=0;
		//if(bString&&!m_pContext->regObjs[2].IsString())
		//	int v=0;
		//if(parser.pos>90)
		//	int v=0;

		//if(parser.pos>=1520)
		//	int v=0;
	//	if(parser.pos>280)//script!=pKeep)
	//		int v=0;

		int actionCode = parser.GetByte();
		
		if ( !actionCode )
			break;


		// Get the source of the action - set this everytime since the thread could be deleted
//				ScriptThread* baseThread = actionList->actionThread[i];
//		if(l==2210)
//			int v=0;
		int len = 0;
		if ( actionCode & sactionHasLength ) 
			len = parser.GetWord();
		S32 pos = parser.pos + len;
		if ( skipCount > 0 ) {
			// Skip an action because of a wait for frame
			skipCount--;
		} else {
			//ScriptThread* thread = m_pCnt->GetTarget();//context->useTarget ? context->targetThread : baseThread;
			//m_thread=m_pCnt->GetTarget();
			/*if(m_thread->m_pObject==XNULL)
			{

				int v=0;
			}*/
			ScriptThread*thread=m_thread->m_pObject?
								m_thread->m_pObject->thread:XNULL;
			if ( thread ) {
				// Handle actions that use a target
				switch ( actionCode ) {
					// Handle timeline actions
					case sactionGotoFrame: {
						thread->Seek(parser.GetWord(),false,ScriptPlayer::SK_STOP);
						//thread->StopPlay();
					} break;

					case sactionGotoLabel: {
						int frameNum = thread->FindLabel((char*)(parser.script+parser.pos));
						if ( frameNum >= 0 ) 
							thread->Seek(frameNum,false,ScriptPlayer::SK_STOP);
					} break;
					case sactionNextFrame: {
						thread->Seek(thread->GetFrame()+1,false,ScriptPlayer::SK_STOP);
					} break;
					case sactionPrevFrame: {
							thread->Seek(thread->GetFrame()-1,false,ScriptPlayer::SK_STOP);
					} break;
					case sactionPlay: {
						//thread->m_bNoEnter=true;
						thread->Play();
					} break;
					case sactionStop: {
						thread->StopPlay();
					} break;

					case sactionWaitForFrame: {
						// Skip n actions if the frame is not yet loaded
						int frame = parser.GetWord();
						if ( !thread->player->FrameComplete(frame) )
							skipCount = parser.GetByte();
					} break;
				}
			}

			// Handle actions that do not need a target
			switch ( actionCode ) {
				case sactionGetURL: {// Get an URL
					//if ( m_pContext->urlCount < m_pContext->maxURLs ) 
					{
						const char*url=parser.GetStringP();
						const char*target=parser.GetStringP();
						m_pPlayer->AddLoader(url,target);
											  
						//int id=m_pContext->urlCount;
						//m_pContext->actionURLs[id] = parser.GetString();
						//m_pContext->actionTargets[id] = parser.GetString();
						//m_pContext->actionPostData[id] = 0;
						//m_pContext->actionVars[id] = 0;
						//m_pContext->actionObjs[id] = XNULL;
						//m_pContext->urlCount++;
					}
				} break;

				case sactionSetTarget: {
					char* targetName = (char*)parser.script+parser.pos;
					if ( *targetName ) {
						// Set a new target
						//if(strcmp(targetName,"/:TheOneWhoWon")==0)
						//	int v=0;
						//m_pContext->context->useTarget = XTRUE;

						if ( m_baseThread->m_pObject ) {
							// Check to set a target
							SObject*obj=m_pPlayer->FindTarget(m_baseThread->m_pObject, targetName,GetWithObject());
							if(obj&&obj->pObject)
							{
								//m_pContext->context->targetThread = obj->thread;//obj?obj->thread:XNULL;
								//m_pContext->context->useTarget=XTRUE;
								FREEOBJECT(m_thread);
								m_thread=obj->pObject->RefObject();//(XXObjectClip*)(obj?obj->pObject:m_baseThread);
								m_bTell=XTRUE;
							}
							else
							{
								//m_pContext->context->targetThread = 0;
								//m_pContext->context->useTarget=0;
								FREEOBJECT(m_thread);
								m_thread=m_baseThread->RefObject();
								m_bTell=XFALSE;
							}
							//if(!m_thread) m_thread=m_baseThread;
							//m_pContext->thread=m_thread;
							#ifdef _CELANIMATOR
							// Generate an error message
							if ( !context->targetThread ) 
								ShowTargetError(this, baseThread->rootObject, targetName);
							#endif
						} else {
							// Setting a target after the base object has been removed is an error
							//m_pContext->context->targetThread = 0;
							//m_pContext->context->useTarget=0;
							FREEOBJECT(m_thread);
							m_thread=m_baseThread->RefObject();
							m_bTell=XFALSE;
							//m_pCnt->context->useTarget=XFALSE;

							#ifdef _CELANIMATOR
							// Generate an error where the base object is missing
							ShowTargetError(this, 0, targetName);
							#endif
						}
					} else {
						// Clear the set target
						//m_pContext->context->useTarget = XFALSE;
						//m_pContext->context->targetThread = 0;
						FREEOBJECT(m_thread);
						m_thread=m_baseThread->RefObject();
						m_bTell=XFALSE;
						//m_pCnt->pTarget=XNULL;
					}
				} break;

				// Global actions
				#ifdef SOUND
				case sactionStopSounds: {
					m_pPlayer->theSoundMix.RemoveAll();
				} break;
				#endif

				case sactionToggleQuality: {
					//m_pPlayer->highQuality = !m_pPlayer->highQuality;
					//m_pPlayer->autoQuality = XFALSE;
					//m_pPlayer->SetCamera(m_pPlayer->updateLazy);
					m_pPlayer->ControlHighQuality();
				} break;

				// Flash 4 Actions
#ifdef ACTIONSCRIPT
				case sactionAdd:
					if(bECMA)
						m_pStack->AddECMA();
					else
						m_pStack->Add();
					//PushNum(PopNum() + PopNum());
					break;

				case sactionSubtract:
					if(bECMA)
						m_pStack->SubECMA();
					else
						m_pStack->Sub();
					/*{
						XFDOUBLE valueA = PopNum();
						XFDOUBLE valueB = PopNum();
						PushNum(valueB - valueA);
					}*/
					break;

				case sactionMultiply:
					if(bECMA)
						m_pStack->MulECMA();
					else
						m_pStack->Mul();
					//PushNum(PopNum() * PopNum());
					break;

				case sactionDivide:
					/*{
						XFDOUBLE valueA = PopNum();
						XFDOUBLE valueB = PopNum();
						if (valueA) {
							PushNum(valueB / valueA);
						} else {
							Push("#ERROR#");
						}
					}*/
					if(bECMA)
						m_pStack->DivECMA();
					else
						m_pStack->Div();
					break;

				case sactionEquals:
					//Push((PopNum() == PopNum()) ? "1" : "0");
					m_pStack->EQ();
					break;

				case sactionLess:
					/*{
						XFDOUBLE valueA = PopNum();
						XFDOUBLE valueB = PopNum();
						Push((valueB < valueA) ? "1" : "0");
					}*/
					m_pStack->Less();
					break;

				case sactionAnd:
					/*{
						int valueA = (int)PopNum();
						int valueB = (int)PopNum();
						Push((valueA && valueB) ? "1" : "0");
					}*/
					m_pStack->And();
					break;

				case sactionOr:
					/*{
						int valueA = (int)PopNum();
						int valueB = (int)PopNum();
						Push((valueA || valueB) ? "1" : "0");
					}*/
					m_pStack->Or();
					break;

				case sactionNot:
					//Push(PopNum() ? "0" : "1");
					m_pStack->Not();
					break;

				case sactionStringEquals:
					/*{
						char *A = Pop();
						char *B = Pop();
						Push(!strcmp(A, B) ? "1" : "0");
						FreeStr(A);
						FreeStr(B);
					}*/
					m_pStack->StringEQ();
					break;

				case sactionStringLess:
					/*{
						char *A = Pop();
						char *B = Pop();
						Push((strcmp(B, A) < 0) ? "1" : "0");
						FreeStr(A);
						FreeStr(B);
					}*/
					m_pStack->StringLess();
					break;

				case sactionStringLength:
				case sactionMBStringLength:
					{
						BOOL doMultiByte = (actionCode == sactionMBStringLength);
						m_pStack->StringLength(doMultiByte);
						/*char *value = Pop();
						char *result = ConvertIntegerToString(PlayerStringLength(value, doMultiByte));
						Push(result);
						FreeStr(result);
						FreeStr(value);*/
					}
					break;

				case sactionStringAdd:
					{
						/*char *A = Pop();
						char *B = Pop();
						char *result = new char[strlen(A)+strlen(B)+1];
						strcpy(result, B);
						strcat(result, A);
						Push(result);
						delete [] result;
						FreeStr(A);
						FreeStr(B);*/
						m_pStack->StringAdd();
					}
					break;

				case sactionGetTime:
					//PushNum((XFDOUBLE) 
					m_pStack->Push((int)((XTime::GetTimeMSec() - m_pPlayer->mTimerOffset)));
					break;

				case sactionRandomNumber:
					{
						XXVar var;
						m_pStack->Pop(var);

						XINT value = var.ToInt();//PopNum();
						//if(value!=6)
						//	int v=0;
						//value=(int)Random(value);
						//else
						//if(value==4&&l>1000)
						//	int v=0;
						value=(int)Random(value);
						m_pStack->Push(value);//pVar);
						//PushNum((XFDOUBLE) Random((S32) value));
					}
					break;

				case sactionStringExtract:
				case sactionMBStringExtract:
					{
						BOOL doMultiByte = (actionCode == sactionMBStringExtract);
						/*int count = (int)PopNum();
						int index = (int)PopNum()-1;
						char *string = Pop();
						char *result = PlayerStringExtract(string, index, count, doMultiByte);
						Push(result);
						FreeStr(result);
						FreeStr(string);*/
						m_pStack->StringExtract(doMultiByte);
					}
					break;

				case sactionPush:
					{
						//XStream stream(parser.script+parser.pos,len);
						SParser pa(parser.script+parser.pos,len);
						Push(pa);
						//parser.SkipBytes(len);
						/*int type = parser.GetByte();
						if (type == 0) {
							// String literal
							Push(parser.GetStringP());
						} else if (type == 1) {
							// Floating-point literal
							union {
								U32 dw;
								float f;
							} u;
							u.dw = parser.GetDWord();
							PushNum(u.f);
						}*/
						//Push(cnt,parser);
					}
					break;

				case sactionPop:
					{
						//char *result = Pop();
						//FreeStr(result);
						m_pStack->FreePop();
					}
					break;

				case sactionToInteger:
					{
						m_pStack->ToInt();
						//XFDOUBLE value = PopNum();
						//PushNum((float) ((int) value));
					}
					break;

				case sactionJump:
					{
						short offset = (short) parser.GetWord();
						pos += offset;
						FLASHASSERT(pos>0);
					}
					break;

				case sactionIf:
					{
						//XFDOUBLE value = PopNum();
						//XXVar*pVar=m_pStack->Pop();
						short offset = (short) parser.GetWord();
						if (m_pStack->PopLogic()) {
							pos += offset;
							FLASHASSERT(pos>0);
						}
						//delete pVar;
					}
					break;

				case sactionWaitForFrame2:
					//if(m_thread)
					{
						//char *str = Pop();
						XXVar var;
						//XXVar*pVar=
						if(!m_pStack->Pop(var)) break;
						//pVar->ToString();
						int frameNum;
						m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
						ScriptThread *targetThread = m_pPlayer->ResolveFrameNum(var,*m_pContext, &frameNum);
						if (targetThread&&frameNum>=0) {
							// Skip n actions if the frame is not yet loaded
							if (!targetThread->player->FrameComplete(frameNum)) {
								skipCount = parser.GetByte();
							}
						}
						//delete pVar;
						//FreeStr(str);
					}
					break;

				case sactionGotoFrame2:
					{
						int gotoPlay = parser.GetByte();

						//char *str = Pop();
						//XXVar*pVar=
						XXVar var;
						if(!m_pStack->Pop(var)) break;
						//pVar->ToString();
						//if(var.IsString()&&strcmp(var.strTxt,"six_lu")==0)
						//	int v=0;
						int frameNum;
						m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
						ScriptThread* targetThread = m_pPlayer->ResolveFrameNum(var,*m_pContext, &frameNum);
						if (targetThread) {
							if(frameNum>=0)
								targetThread->Seek(frameNum,false,gotoPlay?ScriptPlayer::SK_PLAY:ScriptPlayer::SK_STOP);
							else if (gotoPlay) {
								targetThread->Play();
							}
							else targetThread->StopPlay();
						}
						//delete pVar;
						//FreeStr(str);
					}
					break;

				case sactionCall:
					{
						//char *name = Pop();
						//XXVar*pVar=
						XXVar var;
						if(!m_pStack->Pop(var)) break;
						//if(!pVar) break;
						m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
						m_pPlayer->CallFrame(*m_pContext,CLIPTHREAD(m_thread),&var,pos);
						//delete pVar;
						//FreeStr(name);
					}

					break;

				case sactionGetVariable:
					//GetVariable(thread,
					{
						XXVar var;
						//XXVar*pVar=
						if(m_pStack->Pop(var))//pVar)
						{
							var.ToString(XFALSE);
							//if(XString8::Compare(var.strTxt,"c2")==0)//&&m_thread->m_pObject->character->tag==89)
							//	int v=0;
							XXVar newVar;//*pNew=new XXVar(XOCT_UNDEFINE,XODT_CONST);
							newVar.ToConst(XOCT_UNDEFINE);
							XBOOL bLowCase=NeedLowCase();
							
							m_pContext->InitContextA(this,m_baseThread,m_thread,m_pThis,m_pObject,m_pWith);

							//if(strcmp(var.strTxt,"sofar")==0)
							//	int v=0;
							
							//if(strcmp(var.strTxt,"_root")==XNULL)
							//	int v=0;
							if(!GetLocal(var.strTxt,newVar,bLowCase))
							{
								//SObject*pObject=GetThis()->m_pObject;
#ifdef _VARCACHE_
								/*if(strcmp(var.strTxt,"x")==0)
								{
									nDebugCount++;
									if(nDebugCount==3331)
										int v=0;
									int v=0;
								}*/
								if(CacheGetVariable(var,*m_pContext,newVar))
								{
									//int v=0;
									XXVar vv(0);
									m_pContext->InitContextA(this,m_baseThread,m_thread,m_pThis,m_pObject,m_pWith);
									m_pPlayer->GetVariable(*m_pContext,var.strTxt,vv,m_bInit);
									if(newVar.nType!=vv.nType||
									   newVar.iData32!=vv.iData32||
									   (vv.nType==XODT_FLOAT&&vv.fData!=newVar.fData))
									   int v=0;
								}
								else
#endif
								{
									m_pPlayer->GetVariable(*m_pContext,var.strTxt,newVar,m_bInit);
#ifdef _VARCACHE_				
									Cache(var,m_pContext->varCache);
#endif		
								}

							}
							m_pStack->Push(newVar);

						}
						//else
						//	m_pStack->Push(new XXVar(
					}
					break;

				case sactionSetVariable:
					{
						//char *value = Pop();
						//char *name = Pop();
						//XXVar*pVar=m_pStack->Pop();
						//XXVar*pName=m_pStack->Pop();
						XXVar name,var;
						if(m_pStack->Pop(var)&&m_pStack->Pop(name))
							//pVar&&pName)
						{
							name.ToString(XFALSE);
							//if(name.strTxt[0]<32)
							//	int v=0;
							//if(strcmp(name.strTxt,"x")==0)
							//	int v=0;
							//if(strcmp(name.strTxt,"nextMissionCode")==0)
							//	int v=0;
						//	if(m_thread->m_pObject->character->tag==265&&
						//	parser.pos>=250&&strcmp(pName->strData,"aktiv")==0)
						//		int v=0;//265
							//if(name.strTxt[0]=='i'&&
							//   var.nType==XODT_CONST)
							//   int v=0;
							XBOOL bLowCase=NeedLowCase();
							m_pContext->InitContextA(this,m_baseThread,m_thread,m_pThis,m_pObject,m_pWith);
							if(!SetLocal(name.strTxt,var,bLowCase,XFALSE))
							{
								m_pPlayer->SetVariable(*m_pContext,name,var,XNULL);
#ifdef _VARCACHE_
								Cache(name,m_pContext->varCache);
#endif
							}
							//SetVariable(thread, name, value, XTRUE);
						}
						//SAFEDELETE(pVar);
						//SAFEDELETE(pName);
						//FreeStr(name);
						//FreeStr(value);
					}
					break;

				case sactionSetTarget2: {
					//XXVar*pTarget=m_pStack->Pop();
					XXVar targetVar;
					//char* targetName = Pop();
					if ( m_pStack->Pop(targetVar))//pTarget)//*targetName ) 
					{
						// Set a new target
						//m_pContext->context->useTarget = XTRUE;
						//m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pWith);
						ScriptThread*target=m_pPlayer->ThreadOf(*m_pContext,GetThisObject(XTRUE),&targetVar,XNULL);
						if(target)
						{
						//if(target)
							//m_pContext->context->targetThread=target;
						//m_pCnt->pTarget=target;
							FREEOBJECT(m_thread);
							m_thread=target->rootObject->pObject->RefObject();//(XXObjectClip*)(target?target->rootObject->pObject:m_baseThread);
							//m_pContext->context->useTarget=XTRUE;//target!=XNULL;
							m_bTell=XTRUE;
						}
						else
						{
							//m_pContext->context->useTarget = XFALSE;
							//m_pContext->context->targetThread = 0;
							FREEOBJECT(m_thread);
							m_thread=m_baseThread->RefObject();
							m_bTell=XFALSE;
						}
						//delete pTarget;
					} else {
						// Clear the set target
						//m_pContext->context->useTarget = XFALSE;
						//m_pContext->context->targetThread = 0;
						FREEOBJECT(m_thread);
						m_thread=m_baseThread->RefObject();
						m_bTell=XFALSE;
						//m_pCnt->pTarget=XNULL;
					}
					//m_pCnt->pTarget=m_pCnt->context->targetThread;
					//FreeStr(targetName);
				} break;

				case sactionGetURL2:
					{
						//break;
						U8 method = parser.GetByte();

						//XXVar*pTarget=m_pStack->Pop();
						//XXVar*pURL=m_pStack->Pop();
						XXVar targetVar,url;

						if(m_pStack->Pop(targetVar)&&m_pStack->Pop(url))//pURL) 
						{
							url.ToString(XFALSE);
							if(!url.strTxt[0])
							{
								if(thread)
								{
									ScriptThread*target=m_pPlayer->ThreadOf(*m_pContext,GetThisObject(XTRUE),&targetVar,XNULL);
									if(target)
										m_pPlayer->RemoveSprite(target,XFALSE);
								}
							}
							else
							{
								char*target=XNULL;
								XString8 strPath;
								if(targetVar.IsObject(XXOBJ_CLIP))
								{
									if(targetVar.pObject->m_pObject)
									{
										m_pPlayer->GetTargetPath(targetVar.pObject->m_pObject,targetVar);
										//char*path=m_pPlayer->GetTargetPath(targetVar.pObject->m_pObject);
										//if(path)
										{
											//pTarget->SetString(path);
											target=targetVar.strTxt;//pTarget->strData;
										}
										//((XXObjectClip*)pTarget->pObject)->GetTargetPath(*m_pContext,strPath);
										
									}
								}
								else
								{
									//pTarget->ToString();
									targetVar.ToString(XFALSE);
									target=targetVar.strTxt;//pTarget->strData;
								}
								m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
								m_pPlayer->GetURL2(*m_pContext,CLIPTHREAD(m_thread),url.strTxt,
												 target,
												 method);
							}
						}
						//SAFEDELETE(pURL);
						//SAFEDELETE(pTarget);
					}
					break;

				case sactionGetProperty:
					{
						int index = m_pStack->PopInt();
						//char *target = Pop();
						//XXVar*pVar=m_pStack->Pop();
						XXVar var;
						m_pStack->Pop(var);
						m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
						ScriptThread* targetThread =
							m_pPlayer->ThreadOf(*m_pContext,m_thread->m_pObject,&var,XNULL);
							//thread;
						var.ToConst(XOCT_UNDEFINE);
                       	if (targetThread) {
							//char *value = GetPropertyStr(targetThread, index);
							//Push(value);
							//FreeStr(value);
						
							m_pPlayer->GetPropertyStr(targetThread,index,var);
						} /*else {
							//Push("");
							pVar->ToConst(XOCT_UNDEFINE);
						}*/
						
						m_pStack->Push(var);
						//FreeStr(target);
					}
					break;

#ifdef DRAGBUTTONS
				case sactionStartDrag:
					{
						//XXVar *target = m_pStack->Pop();
						XXVar target;
						m_pStack->Pop(target);

						int bCenter = (int)m_pStack->PopInt();
						int constrainType = (int)m_pStack->PopInt();
						XBOOL bRect=XFALSE;
						SRECT rect;
						//int x1, y1, x2, y2;
						if (constrainType == sdragRectConstraint) {
							// Pop off rectangle constraint
							//y2
							rect.ymax= m_pStack->PopInt()*20;
							bRect=XTRUE;
							rect.xmax= m_pStack->PopInt()*20;
							rect.ymin = m_pStack->PopInt()*20;
							rect.xmin = m_pStack->PopInt()*20;
						}

						m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
						ScriptThread* targetThread = m_pPlayer->ThreadOf(*m_pContext,GetThisObject(XTRUE),&target,CLIPTHREAD(m_thread));
//						SAFEDELETE(target);

						if (targetThread) {
							m_pPlayer->StartDrag(targetThread->rootObject,
											   rect,bCenter,bRect);	
						}
					}
					break;

				case sactionEndDrag:
					m_pPlayer->StopDrag();
					break;
#endif

				case sactionSetProperty:
					{
						//char *value = Pop();
						//XXVar*pVar=m_pStack->Pop();
						XXVar var,targetVar;
						m_pStack->Pop(var);
						int index = (int) m_pStack->PopInt();
						//XXVar*pTarget= m_pStack->Pop();
						if(m_pStack->Pop(targetVar))
						{
							//if(targetVar.strTxt[0]=='g')
							//	int v=0;
							//if(!targetVar.IsString())
							//	int v=0;
							m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
							ScriptThread* targetThread = 
								m_pPlayer->ThreadOf(*m_pContext,m_thread->m_pObject,&targetVar,XNULL);
								//thread;
							
							/*if (target[0]) {
								targetThread = FindTarget(thread ? thread->rootObject : player.rootObject, target);
							}*/
							if(targetThread)
								m_pPlayer->SetPropertyStr(targetThread, index, var);
						}
						//SAFEDELETE(pVar);
						//SAFEDELETE(pTarget);
						//FreeStr(target);
						//FreeStr(value);
					}
					break;

				case sactionTrace:
					{
						//char *value = Pop();
						//XXVar*pVar=m_pStack->Pop();
						XXVar var;
						if(m_pStack->Pop(var))//pVar)
						{
						
#ifdef _WINEMU
						var.ToString(XFALSE);
						this->m_pPlayer->Trace(XString16(var.strTxt));
#endif
//#ifdef _CELANIMATOR
//						TraceString(var.strTxt);//pVar->strData);
//#endif					
						//TRACE0(pVar->strData);
						//delete pVar;
						//FreeStr(value);
						}
					}
					break;

                case sactionCloneSprite:
                    {
                        int             depth = (int) m_pStack->PopInt();
                        //char            *name = Pop();
						//char            *target = Pop();
						//XXVar*pName=m_pStack->Pop();
						//XXVar*pTarget=m_pStack->Pop();
						XXVar name,targetVar;
						m_pStack->Pop(name);
						m_pStack->Pop(targetVar);
						
							//thread;

                        // neg levels are not allowed
                        if (depth < 0) {
                            break;
						}
                        
						//if(pName&&pTarget)

						{
							name.ToString(XFALSE);
							m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
							ScriptThread    *targetThread = m_pPlayer->ThreadOf(*m_pContext,GetThisObject(XFALSE),&targetVar,CLIPTHREAD(m_thread));
							if(targetThread&&targetThread!=targetThread->player)
								targetThread->rootObject->parent->thread->CloneSprite(*m_pContext,
											targetThread, name, depth);
								//m_pPlayer->CloneSprite(targetThread, pName->strData, depth);
						}
						//SAFEDELETE(pTarget);
						//SAFEDELETE(pName);
						//FreeStr(target);
						//FreeStr(name);
                    }
                    break;

                case sactionRemoveSprite:
                    {
						//char            *target = Pop();
						//XXVar*pTarget=m_pStack->Pop();
						XXVar target;
						if(m_pStack->Pop(target))//pTarget)
						{
							m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
							/*ScriptThread    *targetThread = m_pPlayer->ThreadOf(*m_pContext,GetThisObject(XFALSE),&target,XNULL);
                     		
							if (targetThread && targetThread->rootObject->IsCloneObject())
								// remove only duplicate sprites
								m_pPlayer->RemoveSprite(targetThread, XFALSE);*/
							//if(strcmp(target.strTxt,"_root.po14.tpai")==0)
							//	int v=0;
							SObject*pObject=m_pPlayer->ObjectOf(*m_pContext,GetThisObject(XFALSE),&target,XNULL);
							if(pObject&&pObject->IsCloneObject())
							{
								if(pObject->parent&&pObject->parent!=&m_pPlayer->display.root)
								{	        
									pObject->parent->thread->RemoveDepth(pObject->depth);
								}
							}

							//FreeStr(target);
							//delete pTarget;
						}
                    }
                    break;

				case sactionAsciiToChar:
				case sactionMBAsciiToChar:
					{
						BOOL doMultiByte = (actionCode == sactionMBAsciiToChar);
						/*int asciiCode = (int)PopNum();
						char str[3];
						if (doMultiByte && asciiCode > 0xff) {
							str[0] = (char) (asciiCode>>8);
							str[1] = (char) (asciiCode&0xff);
							str[2] = 0;
						} else {
							str[0] = (char)asciiCode;
							str[1] = 0;
						}
						Push(str);*/
						m_pStack->ASCIIToChar(doMultiByte);
					}
					break;

				case sactionCharToAscii:
				case sactionMBCharToAscii:
					{
						BOOL doMultiByte = (actionCode == sactionMBCharToAscii);
						m_pStack->CharToASCII(doMultiByte);
						/*char *str = Pop();
						U16 ch;
						if (doMultiByte && PlayerIsLeadByte(*str)) {
							ch = ((U8)str[0]<<8) | (U8)str[1];
						} else {
							ch = (U8)*str;
						}
						char *result = ConvertIntegerToString((int)ch);
						Push(result);
						FreeStr(result);
						FreeStr(str);*/
					}
					break;
			case sactionCallFunction://	   = 0x3D,
				 
				 CallFunction();
				 break;
			case sactionCallMethod://	   = 0x52,
//				 m_pCnt->pCaller=pCaller;
				 CallMethod();
				 break;
			case sactionConstantPool://	   = 0x88,
				{
					XU16 count=parser.GetWord();
					m_pContext->MakeConstant((XPCTSTR)parser.script+parser.pos,count);
					//m_pCodeBuf[pos+len-1]=0;
					//ConstantPool(cnt,parser);//XStream(m_pCodeBuf+pos,len));
				}
				 break;
			case sactionDefineFunction://  = 0x9B,
				 ret=DefineFunction(parser);//XStream(m_pCodeBuf+pc,m_nCodeLength-pc));
				 if(ret<0) 
					 m_pContext->actionOverflow=XTRUE;//pc=m_nCodeLength+1;
				 else
					 pos+=ret;
					 //pc+=ret;
				 break;
			case sactionDefineLocal://	   = 0x3C,
				 DefineLocal(XTRUE);
				 break;
			case sactionDefineLocal2://	   = 0x41,
				 DefineLocal(XFALSE);
				 break;
			case sactionDelete://		   = 0x3A,
				 //Delete(cnt);
				{
						//XXVar*pName=m_pStack->Pop();
						//XXVar*pObj=m_pStack->Pop();
						XXVar name,obj;
						if(m_pStack->Pop(name)&&m_pStack->Pop(obj))//pName&&pObj)
						{
							if(name.IsString2()&&obj.IsObject())
							{
								//pName->ToString();
								name.ToString(XFALSE);
								//XString8&strName=((XXVarString*)pName)->GetStrData();
								obj.pObject->RemoveMember(name.strTxt);//->strTxt);//strName);
							}
						}
						m_pStack->PushBool(XTRUE);
						//SAFEDELETE(pName);
						//SAFEDELETE(pObj);
				}
				 break;
			case sactionDelete2://		   = 0x3B,
				{
					//XXVar*pName=m_pStack->Pop();
					XXVar name;//->m_pStack->GetLast();
					m_pStack->Last(name);
					//m_pStack->Pop(name);
					if(name.IsString())
					{
//						XString16 str(pName->strData,STRING_ATTACH);
						m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
						m_pPlayer->RemoveVariable(*m_pContext,name.strTxt);//str);
						//str.EmptyNoFree();
					}
					//SAFEDELETE(pName);
				}
				 //Delete2(cnt);
				 break;
			case sactionEnumerate://       = 0x46,
				 Enumerate();
				 break;
			case sactionEquals2://		   = 0x49,
				 m_pStack->EQ2();
				 break;
			case sactionGetMember://	   = 0x4E,
				 GetMember();
				 break;
			case sactionInitArray://	   = 0x42,
				 InitArray();
				 break;
			case sactionInitObject://      = 0x43,
				 InitObject();
				 break;
			case sactionNewMethod://	   = 0x53,
				 NewMethod();
				 break;
			case sactionNewObject://       = 0x40,
				 NewObject();
				 break;
			case sactionSetMember://       = 0x4F,
				 SetMember();
				 break;
			case sactionTargetPath://	   = 0x45,
				 TargetPath();
				 break;
			case sactionWith://			   = 0x94,
				 //pc+=DoWith(cnt,XStream(m_pCodeBuf+pc,m_nCodeLength-pc));
				 pos+=DoWith(parser);
				 break;
			case sactionToNumber://		   = 0x4A,
				 //if(thread&&thread->curFrame==30&&thread->rootObject->character->type==rootChar)
				 //	 int v=0;
				 m_pStack->ToNum();
				 break;
			case sactionToString://		   = 0x4B,
				{
					//XXVar*p=m_pStack->Pop();
					XXVar var;
					m_pStack->Pop(var);
					//if(!p)
					//	m_pStack->Push(XXVar::CreateConst(XOCT_UNDEFINE));
					//else
					{
						var.ToString(XFALSE);
						//XXObjectCreator*pCreator=m_baseThread->GetGlobal();
						//*p=pCreator->CreateString(p->strData);
						m_pStack->Push(var);
					}
					//m_pStack->ToString();
				}
				 break;
			case sactionTypeOf://		   = 0x44,
				 //TypeOf(cnt);
				{
					//XXVar*pVar=m_pStack->Pop();
					XXVar var;
					m_pStack->Pop(var);
					//if(pVar)
					{
					   m_pStack->Push(var.TypeOf());//new XXVar(pVar->TypeOf()));
					}
					//else m_pStack->PushConst(XOCT_UNDEFINE)(XXVar::CreateConst(XOCT_UNDEFINE));
					//SAFEDELETE(pVar);
				}
				 break;
			case sactionAdd2://			   = 0x47,
				 if(bECMA)
					m_pStack->Add2ECMA();
				 else
					m_pStack->Add2();
				 break;
			case sactionLess2://		   = 0x48,
				 //{
					 
				 //}
				 m_pStack->Less2();
				 break;
			case sactionModulo://		   = 0x3F,
				 m_pStack->Mod();
				 break;
			case sactionBitAnd://		   = 0x60,
				 m_pStack->BitAnd();
				 break;
			case sactionBitLShift://	   = 0x63,
				 m_pStack->BitLShift();
				 break;
			case sactionBitOr://		   = 0x61,
				 m_pStack->BitOr();
				 break;
			case sactionBitRShift://	   = 0x64,
				 m_pStack->BitRShift();
				 break;
			case sactionBitURShift://	   = 0x65,
				 m_pStack->BitURShift();
				 break;
			case sactionBitXor://		   = 0x62,
				 m_pStack->BitXor();
				 break;
			case sactionDecrement://	   = 0x51,
				 if(bECMA)
						m_pStack->DecECMA();
				 else
					m_pStack->Dec();
				 break;
			case sactionIncrement://	   = 0x50,
				 if(bECMA)
					m_pStack->IncECMA();
				 else
					m_pStack->Inc();
				 
				 break;
			case sactionPushDulicate://	   = 0x4C,
				{
					//XXVar*pVar=m_pStack->GetLast();
					//if(pVar) m_pStack->Push(new XXVar(*pVar));
					//else m_pStack->Push(XXVar::CreateConst(XOCT_UNDEFINE));
					XXVar var;
					if(m_pStack->Last(var))
						m_pStack->Push(var);
					else
						m_pStack->PushConst(XOCT_UNDEFINE);
				}
				 break;
			case sactionReturn://		   = 0x3E,
				 //pc=m_nCodeLength+1;
				 bReturn=XTRUE;
				 break;
			case sactionStackSwap://	   = 0x4D,
				 m_pStack->SwapStack();
				 break;
			case sactionStoreRegister://   = 0x87,
				 
				{
					XU8 regNum=parser.GetByte();
					//XXVar*pVar=m_pStack->GetLast();
					XXVar var;
					
					if(m_pStack->Last(var))//pVar)
					{
						/*if(regNum==2)
						{
							if(var.IsUndefine()&&m_pContext->regObjs[2].IsString())
								int v=0;
						}*/
						m_pContext->SaveRegister(regNum,var);
						//delete pVar;
					}
				}
				 //SaveRegister(cnt,*(m_pCodeBuf+pc));
				 
				 break;
			case sactionCastOp:
				 //m_pStack->FreePop();
				 //break;
				 {
					//XXVar*pConst=m_pStack->Pop();
					//XXVar*pObj=m_pStack->Pop();
					//XXVar*pVar=XXVar::CreateConst(XOCT_NULL);//(XFALSE);
					XXVar cnt,obj,var;
					m_pStack->Pop(cnt);
					m_pStack->Pop(obj);
					//m_pStack->Pop(var);
					//XXVar var;
					/*if(cnt.IsObject())//pObj&&pConst&&pConst->IsObject())
					{
						if(ObjectOf(obj)&&
						   !obj.IsSysCall()&&
							obj.pObject->IsInstanceOf(cnt.pObject))
							var=obj;
						//pObj->ToObject();
						//XXObject*pObject=ObjectOf(&obj);//pObj);//,GetThisObject(XFALSE));
						//if(pObject&&pObject->IsInstanceOf(cnt.pObject))
						//	var=obj;
					}*/
					m_pStack->Push(cnt);
					//m_pStack->Push(var);
					//SAFEDELETE(pConst);
					//SAFEDELETE(pObj);
				}break;
			case sactionInstanceOf://	   = 0x54,
				 //Instanceof(cnt);

				{
					//XXVar*pConst=m_pStack->Pop();
					//XXVar*pObj=m_pStack->Pop();
					//XXVar*pVar=XXVar::CreateBool(XFALSE);

					XXVar cnt,obj,var;
					m_pStack->Pop(cnt);
					m_pStack->Pop(obj);
					var.ToLogic();
					var=XFALSE;
					//m_pStack->Pop(var);
					//if(pObj&&pConst)//&&pConst->IsObject())
					{
						if(ObjectOf(obj)&&ObjectOf(cnt)&&
						   !obj.IsSysCall()&&!cnt.IsSysCall()&&
						   obj.pObject->IsInstanceOf(cnt.pObject))
						   var=XTRUE;
							//*pVar=XTRUE;
					}
					m_pStack->Push(var);//pVar);
					//SAFEDELETE(pConst);
					//SAFEDELETE(pObj);
				}
				 break;
			case sactionEnumerate2://	   = 0x55,
				 //Enumerate2(cnt);
				{
					 //XXVar*pObj=m_pStack->Pop();
					 XXVar obj;
					 m_pStack->Pop(obj);
					 m_pStack->PushConst(XOCT_NULL);//(XXVar::CreateConst(XOCT_NULL));
					 if(obj.IsObject())//pObj&&pObj->IsObject())
					 {
						m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
						obj.pObject->EnumVariables(*m_pContext);
					 }
					 //SAFEDELETE(pObj);
				}
				 break;
			case sactionStrictEquals://	   = 0x56,
				 m_pStack->StrictEQ();
				 break;
			case sactionGreater://		   = 0x67,
				 m_pStack->Greater();
				 break;
			case sactionStringGreater://   = 0x68,
				 m_pStack->StringGreater();
				 break;
			case sactionDefineFunction2:// = 0x8E,
				{
				 ret=DefineFunction2(parser);//XStream(m_pCodeBuf+pc,m_nCodeLength-pc));
				 if(ret<0) 
					 m_pContext->actionOverflow=XTRUE; 
					 //pc=m_nCodeLength+1;
				 else
					 pos+=ret;
				}
				 break;
			case sactionExtends://		   = 0x69,
//				 Extends(cnt);
				{
					//XXVar*pSuper=m_pStack->Pop();
					//XXVar*pSub=m_pStack->Pop();
					XXVar super,sub;
					m_pStack->Pop(super);
					m_pStack->Pop(sub);
					//if(pSuper&&pSub)
					{	if(super.IsObject()&&sub.IsObject())
						{
							sub.pObject->SetSuper(super.pObject,XTRUE);
							sub.pObject->m_pSuperObj=super.pObject;
							super.pObject->CopyMembers(sub.pObject);
						}
					}
					//SAFEDELETE(pSuper);
					//SAFEDELETE(pSub);
				}
				 break;
			case sactionImplementsOp://    = 0x2C,
				 ImplementsOp();
				 break;
			case sactionTry://			   = 0x8F,
				 DoTry(parser);
				 break;
			case sactionThrow://		   = 0x2A,
				 //DoThrow(cnt);
				 bReturn=2;
				 break;
#endif
			}
		}

		m_pContext->actionCount++;

		if(bReturn) break;

		//if( m_pContext->actionCount > MaxExecActions>>2)
		//	int v=0;

		if (m_pContext->actionCount > MaxExecActions) {
			// So many actions have been executed, something must be
			// wrong... stop all actions processing.
#ifdef _CELANIMATOR
			ExecutionLimitError();
#endif
			m_pContext->actionOverflow = XTRUE;
		}

		if (m_pContext->actionOverflow) {
			FLASHASSERT(XFALSE);
			break;
		}
		FLASHASSERT(pos>=0);
		parser.pos = pos;
	}

	m_pContext->nCallDepth--;

	if(pNewObj)
	{
		pNewObj->FreeObject();	
		//pNewObj->ClearMembers();
		//delete pNewObj;
	}

/*#ifdef _DEBUG
	//startTime=XTime::GetTickCount()-startTime;
	XS32 highE;
	XU32 lowE;
	XTime::GetFrequency(highE,lowE);
	if(tagID>=0&&tagID<0xffff)
	{
		int time=(lowE-low);
//		if(time>20000)
//			int v=0;
		XSWFPlayer::_actionTime[tagID]+=time;
		XSWFPlayer::_actionCount[tagID]++;
	}
#endif*/
//	_ANALYLEAVE();
	return bReturn==2?0:1;
}

void XActionScript::Push(SParser&stream)//XStream&stream)
{
	while(stream.pos<stream.tagEnd)
		{
			XU8 type=stream.GetByte();//0;//parser.GetByte();
			//if(!stream.ReadByte(type)) return;
			//XXVar*m_pVar=XNULL;//new XXVar;
			//XXVar var;
			switch(type)
			{
			case XV_STRING:
				 //string.ReadFromString(pStream);break;
				{

					//var=stream.ReadString();
					m_pStack->Push(stream.GetStringP());//stream.ReadString());
					//XSWFString str;
					//str.ReadFromStream(&stream);
					//XString8 str;
					//stream.ReadString(str,0);
					//m_pVar=new XXVar(str,XTRUE);
					//m_pVar=new XXVarString(str,XTRUE);//(str.DataTo());
					//m_pVar->AttachString(str.DataTo());

				}break;
			case XV_FLOAT:
				{
					//float v=stream.GetFloat();
					float v;
					((XU32&)v)=stream.GetDWord();//GetFloat();
					//if(!stream.ReadFloat(v)) return;
					m_pStack->Push((XFDOUBLE)v);
					//var=v;
					//m_pVar=new XXVar(v);
				}break;
			case XV_DOUBLE:
				{
					double v=stream.GetDouble();
					//if(!stream.ReadDouble(v)) return;
					if(v==(XU32)v)
						//var=(int)v;
						m_pStack->Push((int)v);
						//m_pVar=new XXVar((int)v);
					else
						m_pStack->Push((XFDOUBLE)v);
						//var=(XFDOUBLE)v;
						//m_pVar=new XXVar((XFDOUBLE)v);
				}break;
			case XV_NULL:
				 //m_pVar=XXVar::CreateConst(XOCT_NULL);
				 //var.ToConst(XOCT_NULL);
				 m_pStack->PushConst(XOCT_NULL);
				 break;
			case XV_UNDEFINE:
				 //m_pVar=XXVar::CreateConst(XOCT_UNDEFINE);
				 //var.ToConst(XOCT_UNDEFINE);
				 m_pStack->PushConst(XOCT_UNDEFINE);
				 break;
			case XV_BOOLEAN:
				{
					XU8 v=stream.GetByte();
					//if(!stream.ReadByte(v)) return;
					m_pStack->PushBool(v);
					//var.ToLogic();
					//var=v;
					//m_pVar=new XXVar(v);
				}break;
			case XV_C8:
				 {
					XU8 v=stream.GetByte();
					//if(!stream.ReadByte(v)) return;
					if(v<m_pContext->constants.GetSize())
					{
#ifdef _VARCACHE_
						var.SetConst(m_pContext->constants[v],v);
#else
						m_pStack->Push(m_pContext->constants[v]);
						//var=m_pContext->constants[v];
#endif
						//XString8 str(m_pContext->constants[v]);
						//m_pVar=new XXVar(str,XTRUE);
					}
					else
						m_pStack->Push(v);//Const(XOCT_UNDEFINE);
						//var=v;
						//m_pVar=new XXVar(v);
					//m_pVar->ToLogic();
				}break;
			case XV_C16:
				{
					XU16 v=stream.GetWord();
					//if(!stream.ReadWord(v)) return;
					if(v<m_pContext->constants.GetSize())
					{
#ifdef _VARCACHE_
						var.SetConst(m_pContext->constants[v],v);
#else
						m_pStack->Push(m_pContext->constants[v]);
						//var=m_pContext->constants[v];
#endif
						//XString8 str(m_pContext->constants[v]);
						//m_pVar=new XXVar(str,XTRUE);
					}
					else 
						m_pStack->Push(v);//Const(XOCT_UNDEFINE);
						//var=v;
						//m_pVar=new XXVar(v);
					//m_pVar->ToLogic();
				}break;
			case XV_DWORD:
				{
					XS32 v=stream.GetDWord();
					//if(!stream.ReadDWord(v)) return;
					//if(v==7)
					//	int v=0;
					m_pStack->Push(v);
					//var=v;//(int)v;//new XXVar(v);
					//m_pVar->ToLogic();
				}break;
			
			case XV_REGISTER:
				{
					XU8 v=stream.GetByte();
					//if(!stream.ReadByte(v)) return;
					if(v<m_pContext->regObjs.GetSize())
					{
						//var=m_pContext->regObjs[v];
						m_pStack->Push(m_pContext->regObjs[v]);
						//m_pVar=new XXVar(m_pContext->regObjs[v]);//->REF();
					}
					else
						m_pStack->PushConst(XOCT_UNDEFINE);
						//var=v;
						//m_pVar=new XXVar(v);
				}break;
			}
			//if(m_pVar)
			//m_pStack->Push(var);
	}
}

XActionScript* XActionScript::Clone()
{
	XActionScript*p=new XActionScript(XNULL);
	if(p)
	{
		p->m_pCodeBuf=m_pCodeBuf;
		p->m_nCodeLength=m_nCodeLength;
	}
	return p;
}

XBOOL XActionScript::SetLocal(XPCTSTR strName, XXVar &var,XBOOL bLowCase, XBOOL bAdd)
{
	if(!m_bBlock) return XFALSE;
	XXObject*p=m_pLocalObj;
	while(p)
	{
		if(p->SetMemberLocal(*m_pContext,strName,var,XNULL)) return XTRUE;
		p=p->m_pNext;
	}
	return XFALSE;
	/*XXObject*p=m_pBlock;
	while(p)
	{
		if(!p->IsObject(XXOBJ_FUNCTION)) break;
		XXObject*pBlock=((XXObjectFunction*)p)->GetLocalObj();
		if(pBlock->SetMemberLocal(*m_pContext,strName,var,XNULL))
			return XTRUE;
		p=((XXObjectFunction*)p)->GetBlock();
	}
	if(!m_pLocalObj) return XFALSE;
	return m_pLocalObj->SetMemberLocal(*m_pContext,strName,var,XNULL);*/
    //XXSortToken sort(&m_locals);
 	//XXTOKEN*pToken=sort.TokenOf(strName,bLowCase,bAdd);
 /*	if(pToken==XNULL) 
	{
		XXObject*pThis=GetThis();
		if(pThis&&pThis!=m_thread)
		{
			m_pContext->InitContextA(this,m_thread,pThis,m_pWith);
			if(pThis->SetMemberLocal(*m_pContext,strName,var,XNULL)) return XTRUE;
		}
		return XFALSE;
	}*/
	//if(!pToken) return XFALSE;
 	//pToken->member=var;
 	//return XTRUE;
}

inline XBOOL XActionScript::GetLocal(XPCTSTR strName, XXVar &var,XBOOL bLowCase)
{
	if(!m_bBlock) return XFALSE;
	//if(strcmp(strName,"extraArgs")==0)
	//	int v=0;
	XXObject*p=m_pLocalObj;
	while(p)
	{
		if(p->GetMember(*m_pContext,strName,var,XFALSE)) return XTRUE;
		p=p->m_pNext;
	}
	return XFALSE;
/*	XXObject*p=m_pBlock;
	while(p)
	{
		if(!p->IsObject(XXOBJ_FUNCTION)) break;
		XXObject*pBlock=((XXObjectFunction*)p)->GetLocalObj();
		if(pBlock->GetMember(*m_pContext,strName,var,XFALSE))
			return XTRUE;
		p=((XXObjectFunction*)p)->GetBlock();
	}
	if(!m_pLocalObj) return XFALSE;
	return m_pLocalObj->GetMember(*m_pContext,strName,var,XFALSE);*/
 	/*XXSortToken sort(&m_locals);
 	XXTOKEN*pToken=sort.TokenOf(strName,bLowCase,XFALSE);
 	if(pToken==XNULL) 
	{
		XXObject*pThis=GetThis();
		if(pThis&&pThis!=m_thread)
		{
			m_pContext->InitContext(this,m_thread,pThis,m_pWith);
			if(pThis->GetMember(*m_pContext,strName,var,XTRUE))
				return XTRUE;
		}
		return XFALSE;
	}
	var=pToken->member;
 	return XTRUE;*/
}

//DEL ScriptThread* XActionScript::ThreadOf(, ScriptThread *thread, XXVar *pVar)
//DEL {
//DEL 	if(pVar->IsObject(XXOBJ_CLIP))
//DEL 	{
//DEL 		SObject*p=((XXObjectClip*)pVar)->m_pObject;
//DEL 		if(p) return p->thread;
//DEL 	}
//DEL 	pVar->ToString();
//DEL 	if(pVar->strData[0]_
//DEL 		return m_pCnt->m_pPlayer->FindTarget(thread?thread->rootObject:m_pCnt->
//DEL int index = m_pStack->PopNum();
//DEL 						char *target = Pop();
//DEL 						ScriptThread* targetThread = thread;
//DEL 						
//DEL                         if (target[0]) {
//DEL 							targetThread = FindTarget(thread ? thread->rootObject : player.rootObject, target);
//DEL 						}
//DEL 						if (targetThread) {
//DEL 							char *value = GetPropertyStr(targetThread, index);
//DEL 							Push(value);
//DEL 							FreeStr(value);
//DEL 						} else {
//DEL 							Push("");
//DEL 						}
//DEL }

void XActionScript::CallFunction()//,ScriptThread*thread)
{
	
	XXVar name,args;
	m_pStack->Pop(name);
	//if(name.strTxt&&strcmp(name.strTxt,"Tracking_flash")==0)

	//XXVar*pName=m_pStack->Pop();
/*#ifdef _ANALY
	XString8 str="CallFunction:";
	if(name.IsString())//pName&&pName->nType==XODT_STRING)
	{
		str+=name.strTxt;//strData;		
	}
	_ANALYENTER(str);
#endif*/
	int iNum=m_pStack->PopInt();
	//m_pStack->Pop(args);
	//XXVar*pArgNum=m_pStack->Pop();
	
//	if(pName&&pArgNum)
	{

		XU8 vars[sizeof(XXVar)*32];
		XXVARLIST args(vars,32);//iNum);
		//int iNum=pArgNum->ToInt();
		//pArgNum->ToUInt();
		for(int i=0;i<iNum;i++)
		{
			XXVar arg;
			m_pStack->Pop(arg);
			args.Push(arg);
			//XXVar*pArg=m_pStack->Pop();
			//if(!pArg) break;
			//args.Add(pArg);
		}
		int s=m_pStack->GetSize();
		
		m_pContext->pOwer=XNULL;

		/*if(strcmp(name.strTxt,"findPath")==0)//&&args[1].IsUndefine())
		//	int v=0;
		{
			int v=0;
			//CString str="Close:";
			//TRACE(_T("Close:%d,%d\r\n"),(int)args.m_pData[0].fData,(int)args.m_pData[1].fData);
		}*/

			//turn;
			//int v=0;
		//XXVar bkname=name;
		if(ObjectOf(name))//pObj)
		{
			XXObject*pThis=m_pThis;
			if(m_pContext->pOwer&&!m_pContext->pOwer->IsSuperOf(pThis)) 
				pThis=m_pContext->pOwer;
			m_pContext->InitContextA(this,m_baseThread,m_thread,/*m_pContext->pThis*/pThis,m_pContext->pOwer,XNULL);
			if(name.IsSysCall())
			{
				if(name.pObject)
					name.pObject->SysCallMethod(name.nStringType,m_pContext,args);
			}
			else
			{
				name.pObject->Function(m_pContext,args,m_pContext->pOwer?m_pContext->pOwer:GetCaller());
			}
		}
		
		//if(m_pStack->m_nSize-s<0)
		//	int v=0;

		CheckStack(s);
		args.Release();
		//args.FreeAll();
	}
//	SAFEDELETE(pArgNum);
//	SAFEDELETE(pName);
}

void XActionScript::CallMethod()
{
	//XXVar*pName=m_pStack->Pop();
	//XXVar*pObj=m_pStack->Pop();
	//XXVar*pArgNum=m_pStack->Pop();
	XXVar name,obj;
	m_pStack->Pop(name);
	//if(XString8::Compare(name.strTxt,"num")==0)
	//	int v=0;
	
	m_pStack->Pop(obj);
	int iNum=m_pStack->PopInt();


#ifdef _ANALY
	XString8 str="CallMethod:";
	if(name.nType==XODT_STRING)
		str+=name.strTxt;
    _ANALYENTER(str);
#endif

//	if(pName&&pArgNum&&pObj)//&&pObj->IsObject())
	{
//		if(pName->IsString()&&
//		if(strcmp(name.strTxt,"setPan")==0)
//			int v=0;
		XU8 vars[sizeof(XXVar)*32];
		XXVARLIST args(vars,32);//iNum);
		//int iNum=pArgNum->ToInt();
		for(int i=0;i<iNum;i++)
		{
			//XXVar*pArg=m_pStack->Pop();
			XXVar arg;
			m_pStack->Pop(arg);
			args.Push(arg);
			//if(!pArg) break;
			//args.Add(pArg);
		}
		//pName->ToString();

//		if(XString8(name)=="move")
//			int v=0;
		//pObj->ToObject();
		//FLASHASSERT(pObj->IsObject());
		XBOOL bOK=ToObject(obj);
		XXObject*pObject=bOK?obj.pObject:XNULL;
			//ObjectOf(pObj);//,GetThisObject(XTRUE));
		
		int s=m_pStack->GetSize();
		if(pObject)
		{
			
			//XXObjectThread* thread=m_thread;
		//	FLASHASSERT(strcmp(pName->strData,"showPreGameAd"));
			XXObject*pThis=pObject;
			if(pObject->IsSuperOf(m_pThis))
				pThis=m_pThis;
			m_pContext->InitContextA(this,m_baseThread,m_thread,pThis,pObject,XNULL);
			if(name.IsUndefine())
				pObject->Construct(m_pContext,args,pObject);
			else
			{
				name.ToString(XFALSE);
				 //&pObject->m_strClassName=="Enemy_Type1")
					//int v=0;
				
			//	if(strcmp(name.strTxt,"nextFrame")==0)//&&
  				//	int v=0;
				 //if(strcmp(name.strTxt,"createMovie")==0)					
				//	int v=0;
				 //if(strcmp(name.strTxt,"allocate")!=0)
				//	m_thread->m_pObject&&m_thread->m_pObject->character->tag==227)
				//	int v=0;
				//{
				//	if(m_pStack->m_nSize&&
				//	   strcmp(m_pStack->m_pData[m_pStack->m_nSize-1].strTxt,"onEnterFrame")==0)
				//	int v=0;
				//}
//				   &&pObject->m_strClassName=="Enemy_Type1")
				   //&&m_pThis->m_strClassName!="Player")
//					int v=0;
				//if(strcmp(pName->strData,"addLis")==0)//&&pObject->m_strClassName=="Project"))
				//	int v=0;
			
				pObject->CallMethod(m_pContext,name.strTxt,args,
								   XFALSE);
#ifdef _VARCACHE_
				Cache(name,m_pContext->varCache);
#endif
			}
		}
		CheckStack(s);
		//args.FreeAll();
		args.Release();
	}

//	if(XString8::Compare(name.strTxt,"next")==0)
//		int v=0;
//	SAFEDELETE(pArgNum);
//	SAFEDELETE(pName);
//	SAFEDELETE(pObj);
}

int XActionScript::DefineFunction(SParser &parser)//,ScriptThread*thread)
{
	//XSWFString strName;
	//strName.ReadFromStream(&stream);	
	char*strName=parser.GetStringP();
	XXObjectCreator*pCreator=m_baseThread->m_pRoot->m_pGlobal;
		//m_baseThread->m_pRoot->m_pGlobal;
	XXObjectFunction* pFun;
	if(m_pContext->constants.GetSize())
		pFun=pCreator->CreateFunction(m_pContext->constants[0],m_pContext->constants.GetSize());//,m_pCnt->constants.GetSize());
	else
		pFun=pCreator->CreateFunction(XNULL,0);//,m_pCnt->constants.GetSize());
	if(pFun==XNULL)	return -1;
	int nSize=pFun->Init(m_baseThread, parser,m_pLocalObj);
	if(m_bInit)
		pFun->SetInitFlag();
	if(!strName||!strName[0])//strName.IsEmpty())
		m_pStack->Push(pFun);//new XXVar(pFun));
	else //if(XString8(strName)=="drawThisShape")
		//delete pFun;
	{
		//GetThis()
		//XXObject*p=m_pThis?m_pThis:m_baseThread;
		//if(strcmp(strName,"shoot")==0)
		//	int v=0;
#ifdef _ANALY
		pFun->m_strClassNameT=strName;
#endif
		GetThis()->AddMember(XXVar(strName),XXVar(pFun));
	}
	//else 
	//	FREEOBJECT(pFun);
//	pFun->AttachCode(
	return nSize;
}

void XActionScript::DefineLocal(XBOOL bCnt)
{
	//XXVar*pVar;
	XXVar var,name;
	if(bCnt) m_pStack->Pop(var);//pVar=m_pStack->Pop();
	else	 var.ToConst(XOCT_UNDEFINE);//pVar=XXVar::CreateConst(XOCT_UNDEFINE);
	m_pStack->Pop(name);
	//XXVar*pName=m_pStack->Pop();
   
	//if(pVar&&pName)
	{
		name.ToString(XFALSE);
		//pName->ToString();//GetString(str);

		//if(strcmp(pName->strData,"money")==0)
		//	int v=0;

		if(m_pLocalObj)
		{
			if(m_pLocalObj->IsObject(XXOBJ_CLIP))
			{
				m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
				m_pLocalObj->SetMember(*m_pContext,name,var,XNULL);
			}
			else
				m_pLocalObj->AddMember(name,var,XFALSE);
		}
		else
		{
			GetThis()->AddMember(name,var,XFALSE);
		}
		/*if(m_bBlock)
		{
			XBOOL bLowCase=NeedLowCase();
			SetLocal(pName->strData,*pVar,bLowCase,XTRUE);
		}
		else
		{
			GetThis()->AddMember(pName->strData,*pVar,XFALSE);
			XXObject*pFun=m_pCnt->GetBlock();
			if(pFun)
			{
				//pVar->SetParent(pFun);
				pFun->AddMember(pName->strData,*pVar,XFALSE);//pVar->REF());
			}
		}*/
	}
	//if(bCnt)
	{
	//	SAFEDELETE(pVar);
	}
	//SAFEDELETE(pName);
}

void XActionScript::Enumerate()//,ScriptThread*thread)
{
	//XXVar*pObj=m_pStack->Pop();
	XXVar obj;
	m_pStack->Pop(obj);
	m_pStack->PushConst(XOCT_NULL);//(XXVar::CreateConst(XOCT_NULL));
	//return;
	if(ObjectOf(obj)&&!obj.IsSysCall())//pObject)//pObj&&pObj->IsObject())
	{
		m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
		obj.pObject->EnumVariables(*m_pContext);
	}

}

void XActionScript::GetMember()//,ScriptThread*thread)
{
	//XXVar*pName=m_pStack->Pop();
	//XXVar*pObj=m_pStack->Pop();	

	XXVar name,obj;
	m_pStack->Pop(name);
	m_pStack->Pop(obj);
#ifdef _ANALY
	//XString8 str="GetMember:";
	//if(name.IsString())//pName&&pName->nType==XODT_STRING)
	//{
	//	str+=name.strTxt;//strData;		
	//}
	//_ANALYENTER("GetMember");//str);
#endif
	//if(pName&&pObj)
	{
		//XString8 str;
		//pName->GetString(str);
		//XXVar*pVar=new XXVar;
		XXVar var;
		var.ToConst(XOCT_UNDEFINE);
		//XXObject*pObject=&XXObjectCreator::_global;
		//if(pObj->IsObject())
		//FLASHASSERT(pObj->IsObject());
		if(ToObject(obj))
		{
			XXObject*pObject=obj.pObject;//ObjectOf(pObj);//,GetThisObject(XTRUE));
				//pObj->ToObject();//pObj->pObject;
			if(pObject)
			{
				XBOOL bOK=XFALSE;
				if(name.IsNum2())
				{
					XXVar id(name);
					bOK=pObject->GetMember(id.ToInt(),var,XNULL);//strName);
				}
				if(!bOK)
				{
					name.ToString(XFALSE);
					
					m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
					pObject->GetMember(*m_pContext,name.strTxt,var,XTRUE);
#ifdef _VARCACHE_
					Cache(name,m_pContext->varCache);
#endif
					//if(strcmp(name.strTxt,"victory")==0&&var.fData>=16)
					//	int v=0;
					//if(var.IsUndefine())
					//	int v=0;
					//if(strcmp(name.strTxt,"lvlspw")==0)//&&pVar->iData32)
					//	int v=0;
					//{
						//m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
						//m_pPlayer->
					//	int v=0;
					//} 
					
					//	pVar->iData32=0;
						//m_pStack->GetLast()->iData32=1;
	//					int v=0;
	//				if(strcmp(pName->strData,"htotal")==0&&pVar->fData==0)
	//						int v=0;
					
				}

			}		
		}
		m_pStack->Push(var);
	}

//	SAFEDELETE(pName);
//	SAFEDELETE(pObj);
}

void XActionScript::InitArray()
{
	//XXVar*pNum=m_pStack->Pop();
	int i,iNum=m_pStack->PopInt();
	//if(pNum)
	{
		//pNum->ToInt();
		//XXVARLIST list;
		XXObjectCreator*pCreator=m_baseThread->m_pRoot->m_pGlobal;
		XXObjectArray*pArray=pCreator->CreateArray();
		for(i=0;i<iNum;i++)
		{
			//XXVar*pVar=m_pStack->Pop();
			XXVar var;
			m_pStack->Pop(var);
			//if(!pVar) break;
			if(pArray)
				pArray->m_list.Push(var);//Add(pVar);
			//else
			//	SAFEDELETE(pVar);
			//list.Add(pVar);
		}
		if(pArray)
		{
//			pArray->SetContext(GetThis());
			m_pStack->Push(pArray);//new XXVar(pArray));
		}
		else
			m_pStack->PushConst(XOCT_NULL);//(XXVar::CreateConst(XOCT_NULL));
		//list.FreeAll();
	}
//	SAFEDELETE(pNum);
}

void XActionScript::InitObject()
{
	//XXVar*pNum=m_pStack->Pop();
	int i,iNum=m_pStack->PopInt();
	//if(pNum)
	{
		//pNum->ToInt();
		XXObjectCreator*pCreator=m_baseThread->m_pRoot->m_pGlobal;
		XXObject*pObject=pCreator->CreateObject();
		//pObject->nDebugID=10000;
		//XXVARLIST list;
		for(i=0;i<iNum;i++)
		{
			//XXVar*pVar=m_pStack->Pop();
			//XXVar*pName=m_pStack->Pop();
			XXVar var,name;
			m_pStack->Pop(var);
			m_pStack->Pop(name);
			if(pObject)
			{
				//pName->ToString();
				name.ToString(XFALSE);
				pObject->AddMember(name,var);
				//list.Add(pName);
				//list.Add(pVar);
			}
//			SAFEDELETE(pVar);
//			SAFEDELETE(pName);	
		}
		if(pObject)
			m_pStack->Push(pObject);//new XXVar(pObject));
		else
			m_pStack->PushConst(XOCT_NULL);//(XXVar::CreateConst(XOCT_NULL));
		//list.FreeAll();
	}
//	SAFEDELETE(pNum);
}

void XActionScript::NewObject()
{
	//_ANALYENTER("NewObject");
//	XXVar*pName=m_pStack->Pop();
//	XXVar*pArgs=m_pStack->Pop();
	XXVar name;
	m_pStack->Pop(name);
	int iNum=m_pStack->PopInt();
	//if(pName&&pArgs)
	{
		name.ToString(XFALSE);
		//pName->ToString();
		//if(strcmp(name.strTxt,"MiniMapController")==0)
		//	int v=0;
//		int iNum=pArgs->ToInt();
		XU8 vars[sizeof(XXVar)*32];
		XXVARLIST list(vars,32);//iNum);
		if(iNum>32)
		{
			list.SetSize(iNum);
			list.m_nSize=0;
		}
		for(int i=0;i<iNum;i++)
		{
			//XXVar*pVar=m_pStack->Pop();
			//if(!pVar) break;
			XXVar var;
			m_pStack->Pop(var);
			//delete pVar;
			list.Push(var);//Add(pVar);
		}
		//XXVar var;
		//m_pCnt->m_pPlayer->GetVariable(cnt,GetThisObject(),pName->strData,var);
		XXObject*pObject=XNULL;
		//if(pName->IsObject())
		//	pObject=pName->pObject;
		//else
		{
			XXVar var;
			m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
			if(m_thread->m_pRoot->m_pGlobal->GetMember(*m_pContext,name.strTxt,var,XFALSE)&&
			  var.IsObject())
			  pObject=var.pObject;
			else //if(ObjectOf(name)&&!name.IsSysCall())
			{
				var=name;
				if(ObjectOf(var)&&!var.IsSysCall())
					 pObject=var.pObject;//name.pObject;//ObjectOf(&name);
			}

			

			//ObjectOf(pName);//,GetThisObject(XTRUE));
		}
//		if(strcmp(pName->strData,"SimpleButton")==0)
//			int v=0;
		if(pObject)//var.IsObject())
		{
			pObject->CreateObject(m_pContext,list);
#ifdef _ANALY
			if(!pObject->m_strClassNameT.IsNull())
			{
				//XXVar*pVar=m_pStack->GetLast();
				XXVar var;
				m_pStack->Last(var);
				if(var.IsObject())
					var.pObject->m_strClassNameT=pObject->m_strClassNameT;
			}
			else if(name.IsString2())
			{
				//XXVar*pVar=m_pStack->GetLast();
				XXVar var;
				m_pStack->Last(var);
				if(var.IsObject())
					var.pObject->m_strClassNameT=name;//pName->strData;
			}
#endif
//			pObject->SetContext(GetThis());
		}
		else
		{
			m_pStack->PushConst(XOCT_UNDEFINE);//(XXVar::CreateConst(XOCT_UNDEFINE));
		}
		//XXObject*pCo=m_pCnt->GetThis();//m_pCnt->GetContaint();
		//pCo->NewMethod(pName->strData,m_pCnt,&list,XTRUE);
		//list.FreeAll();
		list.Release();
	}
//	else
//		m_pStack->PushConst(XOCT_UNDEFINE);//(XXVar::CreateConst(XOCT_UNDEFINE));
//	SAFEDELETE(pName);
//	SAFEDELETE(pArgs);
}

void XActionScript::NewMethod()//,ScriptThread*thread)
{
	//XXVar*pName=m_pStack->Pop();
	//XXVar*pObj=m_pStack->Pop();
	//XXVar*pArgs=m_pStack->Pop();
	XXVar name,obj;
	m_pStack->Pop(name);
	m_pStack->Pop(obj);
	int iNum=m_pStack->PopInt();
	//if(pName&&pArgs&&pObj)//&&pObj->IsObject())
	{
		//pName->ToString();
		name.ToString(XFALSE);

		//if(strcmp(name.strTxt,"Game")==0)
		//	int v=0;

		//int iNum=pArgs->ToInt();
		XU8 vars[sizeof(XXVar)*32];
		XXVARLIST list(vars,32);//iNum);
		if(iNum>32)
		{
			list.SetSize(iNum);
			list.m_nSize=0;
		}
		for(int i=0;i<iNum;i++)
		{
			//XXVar*pVar=m_pStack->Pop();
			XXVar var;
			m_pStack->Pop(var);
			list.Push(var);
			//if(!pVar) break;
			//list.Add(pVar);
		}
		if(ObjectOf(obj)&&!obj.IsSysCall())//pCo)
		{
			m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),obj.pObject,m_pWith);
			obj.pObject->NewMethod(name.strTxt,m_pContext,&list,XFALSE);
#ifdef _ANALY
			//XXVar*pVar=m_pStack->GetLast();
			XXVar var;
			m_pStack->Last(var);
			if(var.IsObject())//pVar&&pVar->IsObject())
				var.pObject->m_strClassNameT=name;
#endif
		}
		else
			m_pStack->PushConst(XOCT_UNDEFINE);//(XXVar::CreateConst(XOCT_UNDEFINE));
		//list.FreeAll();
		list.Release();
	}
//	else 

	//else
//		m_pStack->Push(XXVar::CreateConst(XOCT_UNDEFINE));
//	SAFEDELETE(pObj);
//	SAFEDELETE(pName);
//	SAFEDELETE(pArgs);
}

void XActionScript::SetMember()//,ScriptThread*thread)
{
 	//XXVar*pVar=m_pStack->Pop();
	//XXVar*pName=m_pStack->Pop();
	//XXVar*pObj=m_pStack->Pop();
#ifdef _ANALY
	//_ANALYENTER("SetMember");
#endif
	XXVar var,name,obj;
	m_pStack->Pop(var);
	m_pStack->Pop(name);
	m_pStack->Pop(obj);
	//if(pVar&&pName&&pObj)
	{
		//XXObject*pObject=ObjectOf(pObj);//,GetThisObject(XFALSE));
		//XXVar bkobj=obj;
		if(!ToObject(obj)) return;//*pObj);
		XXObject*pObject=obj.pObject;
		if(pObject)
		//if(pObj->IsObject())
		{
			if(name.IsNum2())
			{
				//pName->ToInt();
				name.ToInt();
				pObject->SetMember(name.iData32,var);
			}
			else
			{
				//pName->ToString();
				name.ToString(XFALSE);
				//if(name.strTxt[0]<32)
				//	int v=0;
				///if(strcmp(name.strTxt,"PaiMess")==0)//&&(pVar->iData32==21||pVar->fData==21))//&&
				//	int v=0;
				//if(strcmp(name.strTxt,"level")==0&&var.IsNum()&&var.ToInt()==0)//==0&&
				//	var=2;
					//int v=0;
				  // pObject->m_pObject->character->tag==342)
					//int v=0;
				/*if(strcmp(pName->strData,"startVelocity")==0&&m_pThis->m_strClassName!="Player"&&
					m_pThis->m_strClassName!="Scenery")
					int v=0;
				if(strcmp(pName->strData,"velocity")==0&&m_pThis->m_strClassName!="Player"&&
					m_pThis->m_strClassName!="Scenery")
					int v=0;*/
				//if(pVar->iData32==9001)
				//	int v=0;

#ifdef _ANALY
				if(var.IsObject()&&!var.pObject->m_strClassNameT.IsNull())
				{
					var.pObject->m_strClassNameT=name;
				}
#endif
				m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
				pObject->SetMember(*m_pContext,name,var,XNULL);
#ifdef _VARCACHE_
				Cache(name,m_pContext->varCache);
#endif
			}
		}
	}
//	SAFEDELETE(pVar);
//	SAFEDELETE(pName);
//	SAFEDELETE(pObj);
}

void XActionScript::TargetPath()
{
 //	XXVar*pVar=m_pStack->Pop();
//	XXVar*pName=m_pStack->Pop();
	//XXVar*pObj=m_pStack->Pop();
	XXVar obj;
	m_pStack->Pop(obj);
	//if(pObj)//pVar&&pName&&pObj)
	{

		if(ObjectOf(obj)&&!obj.IsSysCall()&&obj.pObject->m_pObject)//pObject&&pObject->m_pObject)
		{
			XXVar var;
			m_pPlayer->GetTargetPath(obj.pObject->m_pObject,var);
			//XXVar*pVar=new XXVar;
			//pVar->SetString(path);
			m_pStack->Push(var);//pVar);
		}
		else
			m_pStack->PushConst(XOCT_UNDEFINE);//(XXVar::CreateConst(XOCT_UNDEFINE));
	}
//	else
//		m_pStack->Push(XXVar::CreateConst(XOCT_UNDEFINE));
//	//SAFEDELETE(pVar);
	//SAFEDELETE(pName);
//	SAFEDELETE(pObj);
}

int XActionScript::DoWith(SParser &parser)
{
	XU16 nSize=parser.GetWord();
	//if(!stream.ReadWord(nSize)) return 0;
	if(!nSize) return 0;
	//if(m_pCnt->nCallDepth>0xff) return nSize;
	//XXVar*pObj=m_pStack->Pop();
	XXVar obj;
	m_pStack->Pop(obj);
	//if(!pObj) return nSize;
	//else 
	if(obj.IsObject())
	{
		//m_pCnt->withs.Add(pObj->pObject);
		XActionScript actions(parser.script+parser.pos,nSize);
//		actions.Init(stream.ReadAddr(),nSize);
		m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,obj.pObject);
		actions.InitContext(m_baseThread,m_thread,obj.pObject,m_pObject,obj.pObject,this);

		XXObjectCreator*pCreator=m_baseThread->m_pRoot->m_pGlobal;
		XXObject*pLocal=pCreator->CreateObject();
		if(pLocal)
		{
			pLocal->RefObject();
			if(m_bBlock)
				pLocal->m_pNext=m_pLocalObj;
			actions.DoAction(*m_pContext,pLocal,XTRUE);
			pLocal->FreeObject();
		}
		//if(m_bBlock)
		//	actions.DoAction(*m_pContext,m_pLocalObj,m_pBlock,XTRUE);
		//else
		//	actions.DoAction(*m_pContext,XNULL,m_pBlock,XTRUE);
		//m_pCnt->withs.RemoveLast();
	}
	//else nSize=0;//return 0;
//	delete pObj;
	return nSize;
}

int XActionScript::DefineFunction2(SParser &parser)
{
	//XSWFString strName;
	char*strName=parser.GetStringP();
	//strName.ReadFromStream(&stream);
	XXObjectCreator*pCreator=m_baseThread->m_pRoot->m_pGlobal;
	XXObjectFunction* pFun;
	if(m_pContext->constants.GetSize())
		pFun=pCreator->CreateFunction(m_pContext->constants[0],m_pContext->constants.GetSize());//m_pCnt->GetThis());
	else
		pFun=pCreator->CreateFunction(XNULL,0);
	if(pFun==XNULL)	return -1;
	int nSize=pFun->Init2(m_baseThread,parser,m_pLocalObj);
	if(m_bInit)
		pFun->SetInitFlag();
	if(!strName||!strName[0])//strName.IsEmpty())
		m_pStack->Push(pFun);//new XXVar(pFun));
	else
	{
		//if(strcmp(strName,"shoot")==0)
		//	int v=0;
		//XXObject*p=m_pThis?m_pThis:m_baseThread;
#ifdef _ANALY
		pFun->m_strClassNameT=strName;
#endif
		GetThis()->AddMember(XXVar(strName),XXVar(pFun));
	}
//	pFun->AttachCode(
	return nSize;
}

void XActionScript::ImplementsOp()
{
		//XXVar*pNum=m_pStack->Pop();
	//XXVar*pCo=m_pStack->Pop();
	XXVar obj;
	m_pStack->Pop(obj);
	int nCount=m_pStack->PopInt();
	if(obj.IsObject())
	{
		
		XXObjectCreator*pCreator=m_baseThread->m_pRoot->m_pGlobal;
		XXObjectArray*pArray=pCreator->CreateArray();
		if(pArray)
		{
			//pNum->ToInt();
			for(int i=0;i<nCount;i++)
			{
				XXVar var;
				m_pStack->Pop(var);
				//XXVar*pFun=m_pStack->Pop();
				//if(!pFun) break;
				pArray->m_list.Push(var);//Add(pFun);
				//SAFEDELETE(pFun);
			}
			obj.pObject->AddMember(_SYSVAR(implements),XXVar(pArray));
		}
		//list.FreeAll();
	}
//	SAFEDELETE(pCo);
}

enum {catchRegsterFlag=0x04,
	  finallyCatchFlag=0x02,
	  catchBlockFlag  =0x01};

int XActionScript::DoTry(SParser &parser)
{
	XU8 flag=parser.GetByte();
	int nTrySize=parser.GetWord();
	int nCatchSize=parser.GetWord();
	int nFinallySize=parser.GetWord();
	char* catchName=XNULL;
	XU8  nRegister=0;
	if(flag&catchRegsterFlag)
		nRegister=parser.GetByte();
	else
		catchName=parser.GetStringP();
	
	int pos=parser.pos;

	int ret=0;

	XXObjectCreator*pCreator=m_baseThread->m_pRoot->m_pGlobal;
		
	if(nTrySize)
	{
		XActionScript actions(parser.script+pos,nTrySize);
		m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,XNULL);
		actions.InitContext(m_baseThread,m_thread,GetThis(),m_pObject,m_pWith,this);
		XXObject*pLocal=pCreator->CreateObject();
		if(pLocal)
		{
			pLocal->RefObject();
			if(m_bBlock)
				pLocal->m_pNext=m_pLocalObj;
			actions.DoAction(*m_pContext,pLocal,XTRUE);
			pLocal->FreeObject();
		}
		//ret=actions.DoAction(*m_pContext,m_bBlock?m_pLocalObj:XNULL,m_pBlock,XTRUE);
	}
	if(ret&&nCatchSize)
	{
		//XXVar*pVar=m_pStack->Pop();
		XXVar var;
		m_pStack->Pop(var);
		//if(pVar)
		{
			
			XActionScript actions(parser.script+pos+nTrySize,nCatchSize);
			if(flag&catchRegsterFlag)
				m_pContext->SaveRegister(nRegister,var);
			else
				actions.SetLocal(catchName,var,m_baseThread->m_pRoot->NeedLowCase(),XTRUE);
			//delete pVar;
			m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,XNULL);
			actions.InitContext(m_baseThread,m_thread,GetThis(),m_pObject,m_pWith,this);
			XXObject*pLocal=pCreator->CreateObject();
			if(pLocal)
			{
				pLocal->RefObject();
				if(m_bBlock)
					pLocal->m_pNext=m_pLocalObj;
				actions.DoAction(*m_pContext,pLocal,XTRUE);
				pLocal->FreeObject();
			}
			//actions.DoAction(*m_pContext,m_bBlock?m_pLocalObj:XNULL,m_pBlock,XTRUE);
		}
	}
	if(nFinallySize)
	{
		XActionScript actions(parser.script+pos+nTrySize+nCatchSize,nFinallySize);
		m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,XNULL);
		actions.InitContext(m_baseThread,m_thread,GetThis(),m_pObject,m_pWith,this);
		//actions.DoAction(*m_pContext,m_bBlock?m_pLocalObj:XNULL,m_pBlock,XTRUE);
		XXObject*pLocal=pCreator->CreateObject();
		if(pLocal)
		{
			pLocal->RefObject();
			if(m_bBlock)
				pLocal->m_pNext=m_pLocalObj;
			actions.DoAction(*m_pContext,pLocal,XTRUE);
			pLocal->FreeObject();
		}
	}

	return 0;
}

void XActionScript::DoThrow()
{
	
}

void XActionScript::CheckStack(int s)
{
	int s1=m_pStack->GetSize();
	s++;
	if(s1==s) return;
	if(s1<s)
		m_pStack->PushConst(XOCT_UNDEFINE);//(XXVar::CreateConst(XOCT_UNDEFINE));
	else
		while(s1>s)
		{
			m_pStack->FreePop();
			s1--;
		}
}

XBOOL XActionScript::ObjectOf(XXVar&obj)//,SObject*base)//, ScriptThread *pThread)
{
	if(obj.IsObject()) return XTRUE;//pVar->IsObject()) return pVar->pObject;
	else
	{
		XXVar var;
		obj.ToString(XFALSE);
		XBOOL bLowCase=m_baseThread->m_pRoot->NeedLowCase();
		if(GetLocal(obj.strTxt,var,bLowCase))
		{
			if(var.IsObject()) 
			{
				obj=var;
				return XTRUE;
			}
				//return (var.pObject);
		}
		m_pContext->InitContextA(this,m_baseThread,m_thread,GetThis(),m_pObject,m_pWith);
		m_pContext->pOwer=XNULL;
		if(m_pPlayer->GetVariable(*m_pContext,obj.strTxt,var,m_bInit))
		{
#ifdef _VARCACHE_
			Cache(obj,m_pContext->varCache);
#endif
			if(var.IsObject()) //return (var.pObject);
			{
				obj=var;
				return XTRUE;
			}
		}
	}
	return XFALSE;
}
inline XBOOL XActionScript::NeedLowCase()
{
	if(!m_thread) return XFALSE;
	return m_thread->m_pRoot->NeedLowCase();
}

inline XBOOL XActionScript::ToObject(XXVar &var)
{
	XXObjectCreator*p=m_baseThread->m_pRoot->m_pGlobal;
	switch(var.nType)
	{
	case XODT_OBJECT:break;
	case XODT_CONST:
		 //var=p->CreateConst((XU8)var.iData32);
		 return XFALSE;
	case XODT_BOOLEAN:
		 var=p->CreateBool(var.iData32);
		 break;
	case XODT_FLOAT:
		 var=p->CreateNumber(var.fData);
		 break;
	case XODT_INT:
		 var=p->CreateNumber(var.iData32);
		 break;
	case XODT_STRING:
		 var=p->CreateString(var);
		 break;
	}
	return XTRUE;
	//m_nType=XODT_OBJECT;
}
