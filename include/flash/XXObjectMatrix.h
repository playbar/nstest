// XXObjectMatrix.h: interface for the XXObjectMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTMATRIX_H__6949F674_500D_42F4_A20F_133003B5C25F__INCLUDED_)
#define AFX_XXOBJECTMATRIX_H__6949F674_500D_42F4_A20F_133003B5C25F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

const XPCTSTR _strMatrixIndex[]=
{	_SYSNAME(a),_SYSNAME(b),_SYSNAME(c),_SYSNAME(d),_SYSNAME(tx),_SYSNAME(ty)};//_strA,_strB,_strC,_strD,_strTx,_strTy };

class XXObjectMatrix : public XXObject  
{
	XDEC_XOBJECT(XXObjectMatrix);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	/*void aGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void aSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void bGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void bSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void cGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void cSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void dGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void dSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void txGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void txSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void tyGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void tySet(XSWFCONTEXT&cnt,XXVARLIST&list);*/
	void clone(XSWFCONTEXT&cnt,XXVARLIST&list);
	void concat(XSWFCONTEXT&cnt,XXVARLIST&list);
	void createBox(XSWFCONTEXT&cnt,XXVARLIST&list);
	void createGradientBox(XSWFCONTEXT&cnt,XXVARLIST&list);
	void deltaTransformPoint(XSWFCONTEXT&cnt,XXVARLIST&list);
	void identity(XSWFCONTEXT&cnt,XXVARLIST&list);
	void invert(XSWFCONTEXT&cnt,XXVARLIST&list);
	void rotate(XSWFCONTEXT&cnt,XXVARLIST&list);
	void scale(XSWFCONTEXT&cnt,XXVARLIST&list);
	void transformPoint(XSWFCONTEXT&cnt,XXVARLIST&list);
	void translate(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	XXObjectMatrix(ScriptPlayer*pRoot);
	virtual ~XXObjectMatrix();
protected:
	void CreateBox(XXVARLIST&list,XU8 bBox);
	void InitObject( );
	virtual void GetString(XXVar&var);//XString8&str);
	virtual XU32 GetObjectType()
	{
		return XXOBJ_MATRIX;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
public:
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*);
	XFDOUBLE GetIndex(int id);
	void ToMatrix(MATRIX&m){m=matrix;}
	void FromMatrix(MATRIX&m){matrix=m;}
	MATRIX matrix;
	//float a,b,c,d;
	//float tx,ty;
};

#endif // !defined(AFX_XXOBJECTMATRIX_H__6949F674_500D_42F4_A20F_133003B5C25F__INCLUDED_)
