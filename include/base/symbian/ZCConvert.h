/*
 * zcconvert.h
 *
 *  Created on: 2009-11-6
 *      Author: Administrator
 */

#if !defined(ZCCONVERT_H_)
#define ZCCONVERT_H_
/**************************************************************
                         ͷ�ļ�

*************************************************************/


//forward declaration
//class CCnvCharacterSetConverter;
//class RFs;

/*
*symbian��ת�����뷽ʽ�����ص�ָ����ʹ�������й���
*�ṹ�Ƚ����ƣ�Ϊ����ϲ�ͬ��ʹ��ϰ�ߡ�
*���������廹�����*/

#include <e32def.h>
class XString16;

class ZCConvert{
public:
	ZCConvert();
	virtual ~ZCConvert();

public:
	///////////////////////////////////////////////////////////////
	//
	//��GBK������ַ�ת����Unicode�����16λ�ַ�
	//
	//�ÿ�ָ����ܷ���ֵ��ʹ�ú����ڽ��ܵ�ָ����delete���������ͷ���ʱ�ռ�
	//gbk to unicode
	static TText16* ConvertToUnicode(const TText8* aSource,int len = -1);
	
	
	///////////////////////////////////////////////////////////////
	//
	//��16λUnicode������ַ�ת��Ϊ16λGBK������ַ���8λ����ʽ����
	//
	//�ÿ�ָ����ܷ���ֵ��ʹ�ú����ڽ��ܵ�ָ����delete���������ͷ���ʱ�ռ�
	//unicode to gbk
	static TText8*  ConvertToGBK(const TText16* aSource);
	
	
	//////////////////////////////////////////////////////////////
	//
	//��8λUTF8ת��Ϊ16λUnicode������ַ�
	//
	//�ÿ�ָ����ܷ���ֵ��ʹ�ú����ڽ��ܵ�ָ����delete���������ͷ���ʱ�ռ�
	//utf-8 to unicode
	static TText* ConvertFromUTF8(const TText8* aSource,int len = -1);
	
	//////////////////////////////////////////////////////////////
	//
	//��8λ��GBKתΪ8λUTF8
	//
	//�ÿ�ָ����ܷ���ֵ��ʹ�ý������ܵ�ָ����delete����
	//gbk to utf-8
	static TText8* ConvertGBKtoUTF8(const TText8* aSource);
	
	//////////////////////////////////////////////////////////////
	//utf-8 to gbk
	static TText8* ConvertUTF8toGBK(const TText8* aSource);

};

#endif /* ZCCONVERT_H_ */
