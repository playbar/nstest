// CRegister.h: interface for the CRegister class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRegister_H__44FD9D23_595F_11D1_B2E3_80C2B91C42E7__INCLUDED_)
#define AFX_CRegister_H__44FD9D23_595F_11D1_B2E3_80C2B91C42E7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#ifndef VGC32_EXT_CLASS
#define VGC32_EXT_CLASS AFX_CLASS_IMPORT
#endif
class VGLineBuffer;
//////////////////////////////////////////////
// 注册表访问类，读写注册表
class CRegister
{
protected:
	LPCSTR m_keyName;//健名
public:
	//设置主键名
	void SetName(LPCSTR name);
	//读入二进制数据
	//写入二进制数据
	//读入结构数据
	void Read(LPCSTR name,LPVOID lpStruct,UINT nSize );
	//写入结构数据
	void Write(LPCSTR name,LPVOID lpStruct,UINT nSize );
	//写入文字数据
	void Write( LPCSTR name, LPCSTR str );
	//读入文字数据
	CString Read( LPCSTR name, LPCSTR sDefault = NULL );
	//写入数字
	void Write( LPCSTR name, int nVal );
	//读如数字
	int Read( LPCSTR name, int nDefault );
	void WriteInt( LPCSTR m_sec, LPCSTR name, int nVal );
	CRegister( LPCSTR keyName );
	virtual ~CRegister();
};

#endif // !defined(AFX_CRegister_H__44FD9D23_595F_11D1_B2E3_80C2B91C42E7__INCLUDED_)
