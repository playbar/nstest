// XXStack.h: interface for the XXStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXSTACK_H__973E050A_A92C_4F09_A866_9915BD76FF28__INCLUDED_)
#define AFX_XXSTACK_H__973E050A_A92C_4F09_A866_9915BD76FF28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXVar.h"

#define CSTACK(VV) while(m_nSize<VV) PushConst(XOCT_UNDEFINE)

#define FUNC2(OPT) inline XU8 OPT()\
		{CSTACK(2);m_nSize--;\
		m_pData[m_nSize-1].OPT(m_pData[m_nSize]);\
		m_pData[m_nSize].Release();return XTRUE;}
#define FUNC1(OPT) inline XU8 OPT() {CSTACK(1);\
		m_pData[m_nSize-1].OPT();return XTRUE;}
#define FUNC1A(OPT,ARG) inline XU8 OPT() {CSTACK(1);\
		m_pData[m_nSize-1].OPT(ARG);return XTRUE;}
/*#define FUNC3(OPT) XU8 OPT(){CSTACK(3);m_nSize-=2;\
		m_pData[m_nSize-1]->OPT(*m_pData[m_nSize],*m_pData[m_nSize+1]);\
		delete m_pData[m_nSize];delete m_pData[m_nSize+1];return XTRUE;}*/
#define FUNC11(OPT) inline XU8 OPT(XBOOL doMultiByte) {CSTACK(1);\
		m_pData[m_nSize-1].OPT(doMultiByte);return XTRUE;}

#define FUNC21(OPT) inline XU8 OPT(XBOOL doMultiByte)\
		{CSTACK(2);m_nSize--;\
		m_pData[m_nSize-1].OPT(m_pData[m_nSize],doMultiByte);\
		m_pData[m_nSize].Release();return XTRUE;}

#define FUNC31(OPT) inline XU8 OPT(XBOOL bMultiByte){CSTACK(3);m_nSize-=2;\
		m_pData[m_nSize-1].OPT(m_pData[m_nSize],m_pData[m_nSize+1],bMultiByte);\
		m_pData[m_nSize].Release();m_pData[m_nSize+1].Release();return XTRUE;}

class XXStack//:public XVector<XXVar*>
{
public:
	/*void RemoveAt(int id)
	{
		if(id>=m_nSize) return;
		m_pData[id].Release();
		if(m_nSize>id+1)
		{
			XGlobal::Memcpy(m_pData+id,m_pData+id+1,(m_nSize-id-1)*sizeof(XXVar));
			XGlobal::Memset(m_pData+m_nSize-1,0,sizeof(XXVar));
		}
		m_nSize--;
	}*/
	int Append(const XXStack& src)
	{
		XUINT nOldSize = m_nSize;
		SetSize(m_nSize + src.m_nSize);
		int i;
		for(i=0;i<src.m_nSize;i++)
			m_pData[nOldSize+i]=src.m_pData[i];
		//XCOPY(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(_XT));
		return nOldSize;
	}
	void RemoveAt(int nIndex, int nCount=1)
	{
		if(nIndex+nCount>=m_nSize)
			nCount=m_nSize-nIndex;
		if(nCount<=0) return;
		//º∆À„“™“∆∂Øµƒ≥§∂»

		int i,nMoveCount = m_nSize - (nIndex + nCount);
		for(i=0;i<nCount;i++)
			m_pData[i+nIndex].Release();

		if (nMoveCount>0)	//“∆∂Ø ˝æ›

		XGlobal::Memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
				nMoveCount * sizeof(XXVar));
		//if(nIndex+nCount<m_nSize)
		XGlobal::Memset(m_pData+nIndex+nMoveCount,0,sizeof(XXVar)*nCount);
		//for(XINT i=nIndex+nCount;i<m_nSize;i++)
		//	m_pData[i
		m_nSize -= nCount;
	}

	void InsertAt(int nIndex,XXVar&var)
	{
		if (nIndex >= m_nSize)
		{
			//≤Â»ÎµƒŒª÷√≥¨π˝ ˝◊È≥§∂»£¨Õÿ’π ˝◊È≥§∂»

			SetSize(nIndex + 1);  // grow so nIndex is valid
		}
		else
		{
			XUINT nOldSize = m_nSize;
			SetSize(m_nSize + 1);
			//“∆∂Ø ˝æ›ƒ⁄»›£¨Ã⁄≥ˆ“™≤Â»Îµƒø◊º“

			XGlobal::Memmove(&m_pData[nIndex+1], &m_pData[nIndex],
				  (nOldSize-nIndex) * sizeof(XXVar));
			XSET(&m_pData[nIndex], 0, sizeof(XXVar));
		}
		//∏¥÷∆ ˝æ›µΩ÷∏∂®Œª÷√
		m_pData[nIndex] = var;
	}
	XXVar& operator[](int i){return m_pData[i];}
	XXVar*m_pData;
	int	  m_nSize,m_nMaxSize,m_bAttach,m_nBufSize;
	XU32 GetSize(){return m_nSize;}
	inline void  SetSize(int newSize)
	{		
		if(newSize>m_nMaxSize)
		{
			m_nMaxSize=newSize<<1;
//			if(m_nMaxSize==6144)
//				int v=0;
			int s=m_nBufSize;
			m_nBufSize=m_nMaxSize*sizeof(XXVar);
			XXVar*pData=(XXVar*)XXVar::AllocData(m_nBufSize);
			if( pData == NULL )
			{
				 LOGWHERE();
				m_nMaxSize = 0;
				m_pData = NULL;
				return;
			}
			m_nMaxSize=m_nBufSize/sizeof(XXVar);
			XGlobal::Memset(pData, 0, sizeof(XXVar) * m_nMaxSize);
			if(m_nSize)
				XGlobal::Memcpy(pData,m_pData,sizeof(XXVar)*m_nSize);
			if(!m_bAttach)
				XXVar::FreeData(m_pData,s);
				//delete (XU8*)m_pData;
			m_pData=pData;
			m_bAttach=XFALSE;
		}
		else if(newSize<m_nSize)
		{
			int i;
			for(i=newSize;i<m_nSize;i++)
				m_pData[i].Release();
		}	
		m_nSize=newSize;
	}
public:
	void Release()
	{
		int i;
		for(i=0;i<m_nSize;i++)
			m_pData[i].Release();		
		m_nSize=0;
	}
	
	inline void PushConst(int e=XOCT_UNDEFINE)
	{
		//XXVar*p=new XXVar;
		//p->ToConst(e);
		//Push(p);
		int i=m_nSize;
		SetSize(m_nSize+1);
		m_pData[i].ToConst(e);
	}
	inline void SwapStack()
	{
		int id=m_nSize-2;
		if(id<0) return;
		XXVar var=m_pData[id];
		m_pData[id]=m_pData[id+1];
		m_pData[id+1]=var;
	}

	FUNC31(StringExtract)

	FUNC1(ToNum)
	FUNC1A(ToString,XFALSE)
	FUNC1(Dec)
	FUNC1(Inc)
	FUNC11(StringLength)
	FUNC1(Not)
	FUNC1(ToInt)
	FUNC11(CharToASCII)
	FUNC11(ASCIIToChar)

	FUNC2(StringGreater)
	FUNC2(StrictEQ)
	FUNC2(Greater)
	FUNC2(BitXor)
	FUNC2(BitRShift)
	FUNC2(BitURShift)
	FUNC2(BitOr)
	FUNC2(BitLShift)
	FUNC2(BitAnd)
	FUNC2(Mod)
	FUNC2(Less2)
	FUNC2(StringLess)
	FUNC2(StringAdd)
	FUNC2(Sub)
	FUNC2(And)
	FUNC2(Or)
	FUNC2(Add)
	FUNC2(Add2)
	FUNC2(EQ2)
	FUNC2(EQ)
	FUNC2(StringEQ)
	FUNC2(Less)
	FUNC2(Div)
	FUNC2(Mul)

	FUNC2(SubECMA)
	FUNC2(AddECMA)
	FUNC2(Add2ECMA)
	FUNC2(DivECMA)
	FUNC2(MulECMA)
	FUNC1(IncECMA)
	FUNC1(DecECMA)
	
	inline XFDOUBLE PopFloat()
	{
		if(m_nSize<=0) return 0;
		m_nSize--;
		XFDOUBLE v=m_pData[m_nSize].ToFloat();		
		return v;
	}
	inline XBOOL PopLogic()
	{
		if(m_nSize<=0) return XFALSE;
		m_nSize--;
		XBOOL v=m_pData[m_nSize].ToLogic();		
		return v;
	}

	inline int PopInt()
	{
		if(m_nSize<=0) return 0;
		m_nSize--;
		int v=m_pData[m_nSize].ToInt();		
		return v;
	}
	

	inline XBOOL Pop(XXVar&var)
	{
		//_ANALYENTER("Pop");
		if(m_nSize<=0) 
		{
			var.ToConst(XOCT_UNDEFINE);
			return XFALSE;
		}
		m_nSize--;
	//	if(m_nSize<=0)
		//	int v=0;
		var=m_pData[m_nSize];
		m_pData[m_nSize].Release();
		return XTRUE;
	}
	inline void FreePop()
	{
		if(m_nSize<=0) return;
		m_nSize--;
		//if(m_nSize<=0)
		//	int v=0;
		m_pData[m_nSize].Release();
		//delete m_pData[m_nSize];
	}
	inline XBOOL Last(XXVar&var)
	{
		if(m_nSize<=0) 
		{
			var.ToConst(XOCT_UNDEFINE);
			return XNULL;
		}
		var=m_pData[m_nSize-1];
		return XTRUE;
	}
	inline void Push(const XXVar&var)
	{
		//_ANALYENTER("Push");
		//int i=m_nSize;
		SetSize(m_nSize+1);
		m_pData[m_nSize-1].Set(var);
		//XVector<XXVar*>::Add(pVar);
	}
	/*inline void PushString(XPTSTR strBuf)
	{
		SetSize(m_nSize+1);
		m_pData[m_nSize-1].AttachString(strBuf);
	}*/

	inline void Push(XPCTSTR strBuf)
	{
		//int i=m_nSize;
		SetSize(m_nSize+1);
		m_pData[m_nSize-1].Set(strBuf);
		//m_pData[i]=strBuf;
	}
/*#ifdef _VARCACHE_
	inline void Push(XPCTSTR strBuf,XU16 iRef)
	{
		int i=m_nSize;
		SetSize(m_nSize+1);
		m_pData[i].SetConst(strBuf,iRef);
	}
#endif*/
	inline void Push(XS32 v)
	{
		SetSize(m_nSize+1);
		//m_pData[m_nSize-1]=v;
		XXVar&var=m_pData[m_nSize-1];
		var.nType=XODT_INT;
		var.iData32=v;
	}
	inline void Push(int v)
	{
		SetSize(m_nSize+1);
		//m_pData[m_nSize-1]=v;
		XXVar&var=m_pData[m_nSize-1];
		var.nType=XODT_INT;
		var.iData32=v;
	}
	inline void Push(XFDOUBLE v)
	{
		SetSize(m_nSize+1);
		//m_pData[m_nSize-1]=v;
		XXVar&var=m_pData[m_nSize-1];
		var.nType=XODT_FLOAT;//XOCT_BOOLEAN;
		var.fData=v;//.iData32=v;
	}
	inline void Push(XXObject*pObj)
	{
		SetSize(m_nSize+1);
		m_pData[m_nSize-1]=pObj;
	}
	
	inline void PushBool(XBOOL v)
	{
		SetSize(m_nSize+1);
		XXVar&var=m_pData[m_nSize-1];
		var.nType=XODT_BOOLEAN;
		var.iData32=v;
		//m_pData[m_nSize-1].ToLogic();//nType=XOCT_BOOLEAN;
		//m_pData[m_nSize-1].iData32=v;
	}
public:
	XXStack(int nSize=64)
	{
		if(nSize<1) nSize=1;
		m_nSize=0;
		m_nMaxSize=nSize;
		m_nBufSize=m_nMaxSize*sizeof(XXVar);
		m_pData=(XXVar*)XXVar::AllocData(m_nBufSize);
		if( m_pData == NULL )
		{
			LOGWHERE();
			m_nMaxSize = 0;
			return;
		}
		m_nMaxSize=m_nBufSize/sizeof(XXVar);
			//new XU8[m_nMaxSize*sizeof(XXVar)];
		memset(m_pData, 0, m_nMaxSize * sizeof(XXVar));
		m_bAttach=XFALSE;
	}
	XXStack(void*pData,int nSize)
	{
		m_nSize=0;
		m_nMaxSize=nSize;
		m_pData=(XXVar*)pData;
		memset(m_pData, 0, m_nMaxSize * sizeof(XXVar));
		m_bAttach=XTRUE;
	}
	//XXStack
	~XXStack()
	{
		Release();
		if(!m_bAttach)
			XXVar::FreeData(m_pData,m_nBufSize);
			//delete m_pData;
		m_pData=XNULL;
	}
};


typedef XXStack XXVARLIST;

#endif // !defined(AFX_XXSTACK_H__973E050A_A92C_4F09_A866_9915BD76FF28__INCLUDED_)
