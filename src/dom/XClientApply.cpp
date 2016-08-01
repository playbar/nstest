#include "StdAfxggdom.h"
#include "XClientApply.h"
#include "XDom.h"
#include "zlib.h"
#include "XDomView.h"
#include "XSystem.h"
#ifdef _SYMBIAN
#include "XThreadPool.h"
#endif


#define _ENCCLIENTID_

#ifdef __APPLE__
#include "IPhoneDeviceData.h"
extern iOSDeviceRelateData g_deviceData;
#endif
#ifdef __ANDROID__
#include "Define.h"
#endif

XClientApply* XClientApply::m_pInstance=XNULL;
//------------------------------------------------------

XPCTSTR XClientApply::_strSetKeys[]=
{
	"Accepts",
	"CID",
	"Colors",
	"Cookie",
	"DATE",
	"Days",
	"DeltaX",
	"DeltaY",
	"Gateway",
	"Help",
	"Home",
	"Host",
	"LocalPath",
	"Port",
	"Profile",
	"Protocol",
	"Retries",
	"Search",
	"Timeout",
	"UA",
	"Version",
	"XID",
	"XTOKEN",
	"XTYPE",

};

static XPCTSTR _strConfig="config";
static XPCTSTR _strFilters="filters";

static XPCTSTR _strProtocol[]={"http","udp"};
static XU8	   _bPassword[]={'J','Q',0xf1,0x80,0x12,0x73,0x90,'a','f','x','p','f',0x12,0xc0};

#ifdef _ENCCLIENTID_

static XU8	   _bPasswordRec[]=		{'+','L','0','9','p','4','8','-','%','&'};
//static XU8	   _bPasswordSend[]=		{'+','L','0','9','p','4','8','-','%','&'};
static XU8	   _bPasswordSend[]=	{'(','^','*','w','k','[','>','?','3','a'};

#define PLAIN_FILE_OPEN_ERROR -1
#define KEY_FILE_OPEN_ERROR -2
#define CIPHER_FILE_OPEN_ERROR -3
#define OK 1&nbsp;&nbsp;&nbsp;

typedef char ElemType;

//初始置换表IP
int IP_Table[64] = 
{
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7,
	56,48,40,32,24,16,8,0,
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6
};

//逆初始置换表IP^-1
int IP_1_Table[64] = 
{
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25,
	32,0,40,8,48,16,56,24
};

//扩充置换表E
int E_Table[48] =
{
	31, 0, 1, 2, 3, 4,
	3, 4, 5, 6, 7, 8,
	7, 8,9,10,11,12,
	11,12,13,14,15,16,
	15,16,17,18,19,20,
	19,20,21,22,23,24,
	23,24,25,26,27,28,
	27,28,29,30,31, 0
};

//置换函数P
int P_Table[32] =
{
	15,6,19,20,28,11,27,16,
	0,14,22,25,4,17,30,9,
	1,7,23,13,31,26,2,8,
	18,12,29,5,21,10,3,24
};

//S盒
int S[8][4][16] =//S1
{
	{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
	},
		//S2
	{
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
	},
		//S3
	{
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
	},
		//S4
	{
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
	},
		//S5
	{
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
	},
		//S6
	{
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
	},
		//S7
	{
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
	},
		//S8
	{
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
};

//置换选择1
int PC_1[56] =
{
	56,48,40,32,24,16,8,
	0,57,49,41,33,25,17,
	9,1,58,50,42,34,26,
	18,10,2,59,51,43,35,
	62,54,46,38,30,22,14,
	6,61,53,45,37,29,21,
	13,5,60,52,44,36,28,
	20,12,4,27,19,11,3
};

//置换选择2
int PC_2[48] = 
{
	13,16,10,23,0,4,2,27,
	14,5,20,9,22,18,11,3,
	25,7,15,6,26,19,12,1,
	40,51,30,36,46,54,29,39,
	50,44,32,47,43,48,38,55,
	33,52,45,41,49,35,28,31
};

//对左移次数的规定
int MOVE_TIMES[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

int ByteToBit(ElemType ch,ElemType bit[8]);
int BitToByte(ElemType bit[8],ElemType *ch);
int Char8ToBit64(ElemType ch[8],ElemType bit[64]);
int Bit64ToChar8(ElemType bit[64],ElemType ch[8]);
int DES_MakeSubKeys(ElemType key[64],ElemType subKeys[16][48]);
int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]);
int DES_PC2_Transform(ElemType key[56], ElemType tempbts[48]);
int DES_ROL(ElemType data[56], int time);
int DES_IP_Transform(ElemType data[64]);
int DES_IP_1_Transform(ElemType data[64]);
int DES_E_Transform(ElemType data[48]);
int DES_P_Transform(ElemType data[32]);
int DES_SBOX(ElemType data[48]);
int DES_XOR(ElemType R[48], ElemType L[48],int count);
int DES_Swap(ElemType left[32],ElemType right[32]);
int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]);
int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48], ElemType plainBlock[8]);

//字节转换成二进制
int ByteToBit(ElemType ch, ElemType bit[8])
{
	int cnt;
	for(cnt = 0;cnt < 8; cnt++)
	{
		*(bit+cnt) = (ch>>cnt)&1;
	}
	return 0;
}

//二进制转换成字节
int BitToByte(ElemType bit[8],ElemType *ch){
	int cnt;
	for(cnt = 0;cnt < 8; cnt++){
		*ch |= *(bit + cnt)<<cnt;
	}
	return 0;
}

//将长度为8的字符串转为二进制位串
int Char8ToBit64(ElemType ch[8],ElemType bit[64]){
	int cnt;
	for(cnt = 0; cnt < 8; cnt++){
		ByteToBit(*(ch+cnt),bit+(cnt<<3));
	}
	return 0;
}

//将二进制位串转为长度为8的字符串
int Bit64ToChar8(ElemType bit[64],ElemType ch[8]){
	int cnt;
	memset(ch,0,8);
	for(cnt = 0; cnt < 8; cnt++){
		BitToByte(bit+(cnt<<3),ch+cnt);
	}
	return 0;
}

//生成子密钥
int DES_MakeSubKeys(ElemType key[64],ElemType subKeys[16][48]){
	ElemType temp[56];
	int cnt;
	DES_PC1_Transform(key,temp);//PC1置换
	for(cnt = 0; cnt < 16; cnt++){//16轮跌代，产生16个子密钥
		DES_ROL(temp,MOVE_TIMES[cnt]);//循环左移
		DES_PC2_Transform(temp,subKeys[cnt]);//PC2置换，产生子密钥
	}
	return 0;
}

//密钥置换1
int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]){
	int cnt;
	for(cnt = 0; cnt < 56; cnt++){
		tempbts[cnt] = key[PC_1[cnt]];
	}
	return 0;
}

//密钥置换2
int DES_PC2_Transform(ElemType key[56], ElemType tempbts[48]){
	int cnt;
	for(cnt = 0; cnt < 48; cnt++){
		tempbts[cnt] = key[PC_2[cnt]];
	}
	return 0;
}

//循环左移
int DES_ROL(ElemType data[56], int time){
	ElemType temp[56];

	//保存将要循环移动到右边的位
	memcpy(temp,data,time);
	memcpy(temp+time,data+28,time);

	//前28位移动
	memcpy(data,data+time,28-time);
	memcpy(data+28-time,temp,time);

	//后28位移动
	memcpy(data+28,data+28+time,28-time);
	memcpy(data+56-time,temp+time,time);

	return 0;
}

//IP置换
int DES_IP_Transform(ElemType data[64]){
	int cnt;
	ElemType temp[64];
	for(cnt = 0; cnt < 64; cnt++){
		temp[cnt] = data[IP_Table[cnt]];
	}
	memcpy(data,temp,64);
	return 0;
}

//IP逆置换
int DES_IP_1_Transform(ElemType data[64]){
	int cnt;
	ElemType temp[64];
	for(cnt = 0; cnt < 64; cnt++){
		temp[cnt] = data[IP_1_Table[cnt]];
	}
	memcpy(data,temp,64);
	return 0;
}

//扩展置换
int DES_E_Transform(ElemType data[48]){
	int cnt;
	ElemType temp[48];
	for(cnt = 0; cnt < 48; cnt++){
		temp[cnt] = data[E_Table[cnt]];
	}
	memcpy(data,temp,48);
	return 0;
}

//P置换
int DES_P_Transform(ElemType data[32]){
	int cnt;
	ElemType temp[32];
	for(cnt = 0; cnt < 32; cnt++){
		temp[cnt] = data[P_Table[cnt]];
	}
	memcpy(data,temp,32);
	return 0;
}

//异或
int DES_XOR(ElemType R[48], ElemType L[48] ,int count){
	int cnt;
	for(cnt = 0; cnt < count; cnt++){
		R[cnt] ^= L[cnt];
	}
	return 0;
}

//S盒置换
int DES_SBOX(ElemType data[48]){
	int cnt;
	int line,row,output;
	int cur1,cur2;
	for(cnt = 0; cnt < 8; cnt++){
		cur1 = cnt*6;
		cur2 = cnt<<2;

		//计算在S盒中的行与列
		line = (data[cur1]<<1) + data[cur1+5];
		row = (data[cur1+1]<<3) + (data[cur1+2]<<2)
			+ (data[cur1+3]<<1) + data[cur1+4];
		output = S[cnt][line][row];

		//化为2进制
		data[cur2] = (output&0X08)>>3;
		data[cur2+1] = (output&0X04)>>2;
		data[cur2+2] = (output&0X02)>>1;
		data[cur2+3] = output&0x01;
	}
	return 0;
}

//交换
int DES_Swap(ElemType left[32], ElemType right[32]){
	ElemType temp[32];
	memcpy(temp,left,32);
	memcpy(left,right,32);
	memcpy(right,temp,32);
	return 0;
}

//加密单个分组
int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]){
	ElemType plainBits[64];
	ElemType copyRight[48];
	int cnt;

	Char8ToBit64(plainBlock,plainBits);
	//初始置换（IP置换）
	DES_IP_Transform(plainBits);

	//16轮迭代
	for(cnt = 0; cnt < 16; cnt++){
		memcpy(copyRight,plainBits+32,32);
		//将右半部分进行扩展置换，从32位扩展到48位
		DES_E_Transform(copyRight);
		//将右半部分与子密钥进行异或操作
		DES_XOR(copyRight,subKeys[cnt],48);
		//异或结果进入S盒，输出32位结果
		DES_SBOX(copyRight);
		//P置换
		DES_P_Transform(copyRight);
		//将明文左半部分与右半部分进行异或
		DES_XOR(plainBits,copyRight,32);
		if(cnt != 15){
			//最终完成左右部的交换
			DES_Swap(plainBits,plainBits+32);
		}
	}
	//逆初始置换（IP^1置换）
	DES_IP_1_Transform(plainBits);
	Bit64ToChar8(plainBits,cipherBlock);
	return 0;
}

//解密单个分组
int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48],ElemType plainBlock[8]){
	ElemType cipherBits[64];
	ElemType copyRight[48];
	int cnt;

	Char8ToBit64(cipherBlock,cipherBits);
	//初始置换（IP置换）
	DES_IP_Transform(cipherBits);

	//16轮迭代
	for(cnt = 15; cnt >= 0; cnt--){
		memcpy(copyRight,cipherBits+32,32);
		//将右半部分进行扩展置换，从32位扩展到48位
		DES_E_Transform(copyRight);
		//将右半部分与子密钥进行异或操作
		DES_XOR(copyRight,subKeys[cnt],48);
		//异或结果进入S盒，输出32位结果
		DES_SBOX(copyRight);
		//P置换
		DES_P_Transform(copyRight);
		//将明文左半部分与右半部分进行异或
		DES_XOR(cipherBits,copyRight,32);
		if(cnt != 0){
			//最终完成左右部的交换
			DES_Swap(cipherBits,cipherBits+32);
		}
	}
	//逆初始置换（IP^1置换）
	DES_IP_1_Transform(cipherBits);
	Bit64ToChar8(cipherBits,plainBlock);
	return 0;
}


void DES_dec(XString8&strCnt,XU8Array&password,XString8&strOut)
{
	int i,count,times = 0;
	ElemType plainBlock[8],cipherBlock[8],keyBlock[8];
	ElemType bKey[64];
	ElemType subKeys[16][48];
	memset(keyBlock,0,8);
	for(i=0;i<password.GetSize();i++)
		keyBlock[i&0x7]+=password[i];
	//将密钥转换为二进制流
	Char8ToBit64(keyBlock,bKey);
	//生成子密钥
	DES_MakeSubKeys(bKey,subKeys);

	//取文件长度
	count=strCnt.GetLength();
	i=0;
	while(i<count)
	{
		int j;
		for(j=0;j<16;j+=2)
		{
			XU8 cr=0;
			XU8 c=((j+i)<strCnt.GetLength())?strCnt[j+i]:0;
			if(c>='a'&&c<='f')
				cr=10+c-'a';
			else if(c>='A'&&c<='F')
				cr=10+c-'A';
			else
				cr=c-'0';
			cr<<=4;
			c=((j+i+1)<strCnt.GetLength())?strCnt[j+i+1]:0;
			if(c>='a'&&c<='f')
				cr+=10+c-'a';
			else if(c>='A'&&c<='F')
				cr+=10+c-'A';
			else
				cr+=c-'0';
			cipherBlock[j>>1]=cr;
		}
		DES_DecryptBlock(cipherBlock,subKeys,plainBlock);
		strOut+=XString8(plainBlock,8);
		i+=16;
	}
}

void DES_enc(XString8&strCnt,XU8Array&password,XString8&strOut)
{
	int i,count;
	ElemType plainBlock[8],cipherBlock[8],keyBlock[8];
	ElemType bKey[64];
	ElemType subKeys[16][48];
	memset(keyBlock,0,8);
	for(i=0;i<password.GetSize();i++)
		keyBlock[i&0x7]+=password[i];
	//memcpy(keyBlock,keyStr,8);
	//将密钥转换为二进制流
	Char8ToBit64(keyBlock,bKey);
	//生成子密钥
	DES_MakeSubKeys(bKey,subKeys);

	i=0,count=strCnt.GetSize();
	while(i<count)
	{
		int j;
		for(j=0;j<8;j++)
		{
			if(j+i<strCnt.GetSize())
				plainBlock[j]=strCnt[j+i];
			else
				plainBlock[j]=0;
		}
		DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
		for(j=0;j<8;j++)
		{
			XU8 c=(XU8)cipherBlock[j];
			XU8 hi=c>>4;
			XU8 lo=c&0xf;
			if(hi>=10)
				strOut+=(char)('A'+hi-10);
			else
				strOut+=(char)('0'+hi);
			if(lo>=10)
				strOut+=(char)('A'+lo-10);
			else
				strOut+=(char)('0'+lo);
		}
		i+=8;
	}

	/*while(!feof(plain)){
	//每次读8个字节，并返回成功读取的字节数
	if((count = fread(plainBlock,sizeof(char),8,plain)) == 8){
	DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
	fwrite(cipherBlock,sizeof(char),8,cipher);
	}
	}
	if(count){
	//填充
	memset(plainBlock + count,'\0',7 - count);
	//最后一个字符保存包括最后一个字符在内的所填充的字符数量
	plainBlock[7] = 8 - count;
	DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
	fwrite(cipherBlock,sizeof(char),8,cipher);
	}*/
}
#endif
bool XClientApply::DecClientID(XString8&strCnt,XString8&strHost)
{
#ifdef _ENCCLIENTID_
	XU8Array pass;
	XString8 strValue=strHost;
	strValue.MakeLower();
	int i;
	bool bSet=false;
	for(i=0;i<m_strDomainList.GetSize();i++)
	{
		XString8& str=m_strDomainList[i];
		int iFind=strValue.Find(str);
		if(iFind<0) continue;
		if(iFind+str.GetLength()==strHost.GetLength())
		{
			bSet=true;
			break;
		}
	}
	if(!bSet) return false;
	int l=(sizeof(_bPasswordRec));
	pass.SetSize(l);
	for(i=0;i<l;i++)
		pass[i]=_bPasswordRec[i]^(20+i);

	strValue=strCnt;
	strCnt.Empty();
	//DES_enc(strValue,pass,strCnt);
	//strValue.Empty();
	DES_dec(strValue,pass,strCnt);

	int iLength=strCnt.GetLength();
	for(i=0;i<iLength;i++)
	{
		char c=strCnt[i];
		if(c=='|') continue;
		if(c<' ') return false;
	}
	/*strValue="100002|23232";//strCnt;
	strCnt.Empty();
	DES_enc(strValue,pass,strCnt);
	strValue.Empty();
	DES_dec(strCnt,pass,strValue);*/

#endif
	return strCnt.Find('|')>0;
	//return true;
}
bool XClientApply::EncClientID(XString8&strCnt)
{
#ifdef _ENCCLIENTID_
	XString8 strClientID;//,strPassword;
	XU8Array outData;
	outData.Append((XU8*)strCnt.GetData(),strCnt.GetLength());
	int id=strCnt.Find('|');
	if(id>0)
	{
		//strCnt.Mid(strPassword,id+1);
		strCnt.Left(strClientID,id);
	}
	else return false;
	//strClientID=strCnt;
	m_nXIDTime=XTime::GetTickCount();
	XString8 strTime,strValue;
	XTime time;
	time.SetCurrentTime();
	strValue.FromInt(time.GetMonth());
	strTime+=strValue;
	strTime+=":";
	strValue.FromInt(time.GetDay());
	strTime+=strValue;
	strTime+=":";
	strValue.FromInt(time.GetHour());
	strTime+=strValue;
	strTime+=":";
	strValue.FromInt(time.GetMinute());
	strTime+=strValue;
	strTime+=":";
	strValue.FromInt(time.GetSecond());
	strTime+=strValue;

	strClientID+="|";
	strClientID+=strTime;
	strClientID+="|";

	outData.Add((XU8)'|');//.Append((XU8*)'|',1);
	outData.Append((XU8*)strTime.GetData(),strTime.GetLength());
	outData.Add((XU8)'|');//outData.Append((XU8*)'|',1);
	//strOut+="|";
	//strOut+=strTime;
	//strOut+="|";



	int i,l=(sizeof(_bPasswordSend));//+strTime.GetLength();
	XU8Array pass;
	pass.SetSize(l);//+strPassword.GetLength()+strTime.GetLength());
	for(i=0;i<l;i++)
		pass[i]=_bPasswordSend[i]^(60-i);
	outData.Append(pass);

	/*XU8 md5Buf[16];
	MD5 md5;
	md5.MakeMD5((char*)outData.GetData(),outData.GetSize(),(char*)md5Buf);

	for(i=0;i<16;i++)
	{
	XU8 c=md5Buf[i];
	XU8 hi=c>>4;
	XU8 lo=c&0xf;
	if(hi>=10)
	strClientID+=(char)('A'+hi-10);
	else
	strClientID+=(char)('0'+hi);
	if(lo>=10)
	strClientID+=(char)('A'+lo-10);
	else
	strClientID+=(char)('0'+lo);
	}*/
	XDomView::GetMD5Of(outData,strClientID);

	strCnt=strClientID;

	/*for(i=0;i<strPassword.GetLength();i++)
	pass[i+l]=(strPassword[i])^(38-i);
	l+=strPassword.GetLength();
	for(i=0;i<strTime.GetLength();i++)
	pass[i+l]=(strTime[i])^(55+i);*/


#endif
	return true;
}

//#ifdef __APPLE__
#if defined(__APPLE__) || defined(__ANDROID__)	// zhengjl 2012.03
bool XClientApply::Enc91UID(XString8& strCnt)
{
	// uid
	XString8 strUID;
	strUID.SetString(strCnt.GetData(), strCnt.GetLength());

	XU8Array outData;
	outData.Append((XU8*)strCnt.GetData(),strCnt.GetLength());
	XString8 strNum;
	int id=m_strXID.Find('|');
	if(id>0)
	{
		m_strXID.Mid(strNum, id+1);
	}
	else 
		return false;

	outData.Add((XU8)'|');
	outData.Append((XU8*)strNum.GetData(), strNum.GetLength());
	outData.Add((XU8)'|');

	m_n91UIDTime=XTime::GetTickCount();
	XString8 strTime,strValue;
	XTime time;
	time.SetCurrentTime();
	strValue.FromInt(time.GetMonth());
	strTime+=strValue;
	strTime+=":";
	strValue.FromInt(time.GetDay());
	strTime+=strValue;
	strTime+=":";
	strValue.FromInt(time.GetHour());
	strTime+=strValue;
	strTime+=":";
	strValue.FromInt(time.GetMinute());
	strTime+=strValue;
	strTime+=":";
	strValue.FromInt(time.GetSecond());
	strTime+=strValue;

	strUID+="|";
	strUID+=strTime;
	strUID+="|";

	outData.Append((XU8*)strTime.GetData(),strTime.GetLength());
	outData.Add((XU8)'|');

	int i,l=(sizeof(_bPasswordSend));
	XU8Array pass;
	pass.SetSize(l);
	for(i=0;i<l;i++)
		pass[i]=_bPasswordSend[i]^(60-i);
	outData.Append(pass);

	XDomView::GetMD5Of(outData,strUID);

	strCnt=strUID;
	return true;
}
#endif


void XClientApply::MakePassword(XU8Array&data,XU8*lastPd)
{
	int i,l=sizeof(_bPassword);
	data.SetSize(l+8);
	XU8*pData=data.GetData();
	for(i=0;i<l;i++)
		pData[i]=_bPassword[i]^(10+i);
	for(i=0;i<8;i++)
	{
		pData[l+i]=lastPd?lastPd[i]:XGlobal::RandInt();
	}
}

int XClientApply::BinEncode(XU8*pData,int length,XU8Array&data)
{
	if(!length) return 0;
	XU8Array password;
	MakePassword(password,NULL);
	int l=password.GetSize();
	data.SetSize(8+l+length);
	//XU32 nHeader=0xfffffff1;
	XU8* gzData=data.GetData();
	XU32* nHeader=(XU32*)gzData;
	nHeader[0]=0xfffffff1;
	nHeader[1]=0;
	gzData+=8;
	//memcpy(gzData,&length,4);
	memcpy(gzData,password.GetData()+l-8,8);
	gzData+=8;

	int len = length;

	unsigned long gzlen = len;

	// Use zlib to compress the data
	compress2((Bytef*)gzData, (unsigned long*)&gzlen,
		(Bytef*)pData, len, 9);

	XU8*pd=password.GetData();
	for(XU32 i=0;i<gzlen;i++)
	{
		gzData[i]=gzData[i]^pd[i%l];
	}

	nHeader[1]=gzlen;


	/*{
	z_stream zstream;

	XGlobal::Memset(&zstream, 0, sizeof(zstream));
	int error = inflateInit(&zstream);

	zstream.next_in = (Bytef*) gzData;
	zstream.avail_in = gzlen;

	int kBufferSize = len;
	//uint8_t *buffer = mmfx_new_array( uint8_t, kBufferSize );

	do {
	zstream.next_out=pData;
	zstream.avail_out=len;
	//zstream.SetNextOut(buffer);
	//zstream.SetAvailOut(kBufferSize);
	error = inflate(&zstream, Z_NO_FLUSH);
	//outStr.AddString((char*)buffer,kBufferSize-zstream.avail_out);
	//tl+=kBufferSize-zstream.avail_out;
	//zstream.InflateWithStatus();
	//m_byteArray.Write(buffer, kBufferSize - zstream.AvailOut());
	} while (error == Z_OK);

	error = inflateEnd(&zstream);
	}*/

	len=gzlen+16;
	return len;
}
int XClientApply::BinDecode(XU8 *pData, int l, XString8 &outStr)
{

	if(l<16) return -1;
	XU32*nHeader=(XU32*)pData;
	if(nHeader[0]!=0xfffffff1) return 0;
	int len=nHeader[1];
	if(l<len+16) return -1;
	pData+=8;
	XU8Array password;
	MakePassword(password,pData);
	pData+=8;
	int ld=password.GetSize();
	XU8*pd=password.GetData();
	for(int i=0;i<len;i++)
	{
		pData[i]=pData[i]^pd[i%ld];
	}
	int tl=0;

	int error = Z_OK;
	{
		// Decompress the data
		z_stream zstream;

		XGlobal::Memset(&zstream, 0, sizeof(zstream));
		error = inflateInit(&zstream);

		zstream.next_in = (Bytef*) pData;
		zstream.avail_in = len;

		const int kBufferSize = 8192;
		Bytef buffer[kBufferSize];
		//uint8_t *buffer = mmfx_new_array( uint8_t, kBufferSize );

		do {
			zstream.next_out=buffer;
			zstream.avail_out=kBufferSize;
			//zstream.SetNextOut(buffer);
			//zstream.SetAvailOut(kBufferSize);
			error = inflate(&zstream, Z_NO_FLUSH);
			if(error<0) return -2;
			outStr.AddString((char*)buffer,kBufferSize-zstream.avail_out);
			tl+=kBufferSize-zstream.avail_out;
			//zstream.InflateWithStatus();
			//m_byteArray.Write(buffer, kBufferSize - zstream.AvailOut());
		} while (error == Z_OK);

		error = inflateEnd(&zstream);
		//mmfx_delete_array( buffer );
		//mmfx_delete_array( gzdata );
	}
	return DEC_OK;

}

void  XClientApply::ResetXID(XString8&strHost)
{
#ifdef _ENCCLIENTID_
	if(XTime::GetTickCount()-m_nXIDTime<600000) return;
	if(m_strXID.IsEmpty()) return;
	XString8 strValue=strHost;
	strValue.MakeLower();
	int i;
	bool bSet=false;
	for(i=0;i<m_strDomainList.GetSize();i++)
	{
		XString8& str=m_strDomainList[i];
		int iFind=strValue.Find(str);
		if(iFind<0) continue;
		if(iFind+str.GetLength()==strHost.GetLength())
		{
			bSet=true;
			break;
		}
	}
	if(!bSet) return;
	XString8 strKey=XEH()[XEH::CLIENT_ID];
	strKey.MakeUpper();
	XString8 strCnt=m_strXID;
#ifdef _ENCCLIENTID_
	EncClientID(strCnt);
#endif
	XSortString8::ESetValue(m_info.strHeaders,strKey,strCnt);
#endif
}

XBOOL XClientApply::SetXID(XString8&strHost,XPCTSTR strValue,bool bEnc)
{
	if(!strValue||!strValue[0]) return XFALSE;
#ifdef _ENCCLIENTID_
	if(bEnc)
	{
		XString8 strCnt=strValue;
		if(!DecClientID(strCnt,strHost)) return XFALSE;
		m_strXID=strCnt;
	}
	else
#endif
		m_strXID=strValue;
	int iLength=m_strXID.GetLength();
	for(int i=0;i<iLength;i++)
	{
		char c=m_strXID[i];
		if(c=='|') continue;
		if(c<' ')
		{
			m_strXID.Empty();
			break;
		}
	}
	XString8 strKey=XEH()[XEH::CLIENT_ID];
	strKey.MakeUpper();
	XString8 strCnt=m_strXID;
	//strCnt.Empty();
#ifdef _ENCCLIENTID_
	EncClientID(strCnt);
#endif
	m_info.strClientID = strCnt;
	//XSortString8::ESetValue(m_info.strHeaders,strKey,strCnt);

	if(bEnc)
	{
		//m_info.m_bModifyToken=XTRUE;
		SaveConfig();
	}
	return XTRUE;
}

XClientApply::~XClientApply()
{
	Release();
}

void XClientApply::Release()
{
#ifdef _SYMBIAN
	XThreadPool *pool = XThreadPool::GetInstance();
	pool->FreeThread(m_pThread);
#else
	m_thread.Stop();
	m_thread.Release();
#endif
	XU32 i;
	if(m_marks.GetSize()>0)
	{
		SaveConfig();
	}
	/*while(m_pClients)
	{
	XClient*pNext=m_pClients->m_pNext;
	delete m_pClients;
	m_pClients=pNext;
	}
	while(m_pFreeClients)
	{
	XCLient*pNext=m_pFreeClient->m_pNext;
	delete m_pFreeClients;
	m_pFreeClients=pNext;
	}*/
	this->ReleaseClient();

	m_lNearest.FreeAll();
	for(i=0;i<m_marks.GetSize();i++)
		m_marks[i]->lMarks.FreeAll();
	m_bModify=XTRUE;

	XDOMFILTER*pFilter=m_pFilters;
	while(pFilter)
	{
		XDOMFILTER*pNext=pFilter->pNext;
		if(pFilter->pRetGO) delete pFilter->pRetGO;
		XPREVFILTER*pp=pFilter->pRevFilter;
		while(pp)
		{
			XPREVFILTER*next=pp->pNext;
			delete pp;
			pp=next;
		}
		delete pFilter;
		pFilter=pNext;
	}
	m_pFilters=XNULL;
	m_marks.FreeAll();

}

void XClientApply::RemoveAllMark()
{
	for(int i=0;i<m_marks.GetSize();i++)
		m_marks[i]->lMarks.FreeAll();
	m_marks.FreeAll();
}

void XClientApply::DropFilters()
{
	XDOMFILTER*pFilter=m_pFilters;
	while(pFilter)
	{
		XDOMFILTER*pNext=pFilter->pNext;
		delete pFilter;
		pFilter=pNext;
	}
	m_pFilters=XNULL;
}

//XClient* _sClient=NULL;

XClient* XClientApply::CreateClient(CONNECTREQ &req,XU32 nID,XU32 nNotifyID,XWindow*pWnd)
{
	XClient*pClient=XNULL;
	//XLock lock(&m_dataLock); // modify hgl
#ifdef _SYMBIAN
	if(!m_dataLock.Lock(XINFINITE))
		return XNULL;
	//==============================I'm a cut-off rule to hide by day and come out at night===============================
#else
	while(true){
		if(m_dataLock.Lock(XNORMALTIME)){
			break;
		}
	}
#endif
	if(m_pFreeClients)
	{
		pClient=m_pFreeClients;
		m_pFreeClients=m_pFreeClients->m_pNext;
		pClient->m_pNext=m_pClients;
		m_pClients=pClient;
	}
	else
	{
		XClient**pp = &m_pClients;
		while( *pp )
		{
			XClient*pc = *pp;
			if(pc->IsFinished()&&!pc->IsUsing())
			{
				if(pClient==XNULL)
				{
					pClient=pc;
				}
				else
				{
					*pp = pc->m_pNext;
					pc->m_pNext=m_pFreeClients;
					m_pFreeClients=pc;
					//delete pc;
					continue;
				}
			}
			pp = &pc->m_pNext;
		}
	}

	if(!pClient)
	{
		pClient=new XClient(this);
		if(pClient)
		{
			pClient->m_pNext=m_pClients;
			m_pClients=pClient;
		}
	}
	if(pClient)
	{
		InitClient(*pClient,nID,nNotifyID,pWnd);
	}
	m_dataLock.UnLock();
	
	/*if(req.URL.m_nProto==XPROTO_SOCKET)
		_sClient = pClient;
	else if(pClient==_sClient)
		int v=0;*/
	return pClient;

}

void XClientApply::InitClient(XClient &client, XU32 nID,XU32 nNotifyID, XWindow *pWnd)
{
	client.SetWindow(pWnd,nID,nNotifyID);
	client.Create(m_info,nID,m_strWorkPath);
	//Modify FILTER
	//client.m_pCurFilter=XNULL;
	client.m_nUserData=0;
	client.m_userData.RemoveAll();//.SetSize(0);
	//client.m_pReferFilter=XNULL;
	client.SetAlive(false);
	client.SetNeedCache(true);
	return;
}


void XClientApply::FromWAP(XString8 &strTxt,XU8 nCharset,DRAWCONTEXT*pDraw)
{
	//return;
	XDom dom;
	dom.Reset(nCharset,XCNT::CTEXT,XCNT::TVND_WAP_WML,pDraw);
	char * pTemp = (char*)strTxt.GetData();
	int ilen = strTxt.GetSize();
	dom.PhaseXML( (XU8*)pTemp, ilen);
	for(XU32 i=dom.m_cardList.GetSize(); i>0; i-- )
	{
		XDomItem*p=dom.m_cardList[i-1];
		//XaVr v(XEAB::CLASS,0);
		XINT nClass=0;
		XString16 strClass;
		XString8  strTmp;
		if(!p->GetAttrib(XEAB::NAME,strTmp))
			continue;
		//XPCTSTR strTmp=dom.GetClass(p);
		//if(strTmp==XNULL) continue;
		//XString8 strt(strTmp,STRING_ATTACH);
		nClass=XEnumConfig::Index(strTmp);
		//strt.EmptyNoFree();
		XDomList list;
		if(nClass==XEnumConfig::UPDATE)
		{
			p->GetList(list,XDOM_A);

		}
		if(nClass==XEnumConfig::CONFIG||nClass==XEnumConfig::UPDATE)
		{
			p->GetList(list,XDOM_SETVAR);
			for(XU32 j=0;j<list.GetSize();j++)
				SetValue(list[j]);
			continue;
		}

		if(!p->GetText(strClass))
			continue;
		if(nClass==XECG::BOOKMARK&&!strClass.IsEmpty())
		{
			AddMarker(strClass,XNULL,XNULL,0);
		}

		p->GetList(list,XDOM_A);

		for(XU32 j=0;j<list.GetSize();j++)
		{
			XDomItem*pa=list[list.GetSize()-j-1];
			XString8 strURL;
			XString16 strTitle;
			if(!pa->GetAttrib(XEAB::HREF,strURL)) continue;
			if(!pa->GetText(strTitle)) continue;

			XINT np=0;
			pa->GetAttrib(XEAB::NAME,np);

			switch(nClass)
			{
			case XECG::NEARBY:
				{
					/*BOOKMARKER*p=new BOOKMARKER;
					p->nPicks=np>>1;
					p->strTitle=strTitle;
					p->strURL=strURL;*/
					XURL url;
					url.SetURL(strURL);
					AddNearby(url,strTitle,np>>1,XFALSE);
					//p->strHost=url.m_strHost;
					//m_lNearest.InsertAt(0,p);
				}
				break;
			case XEnumConfig::BOOKMARK:
				AddMarker(strClass,strTitle,strURL,np>>1);
				break;
			}
		}
	}
	XString8 strPath;
	GetLocalPath(strPath);
	return;
}

XBOOL XClientApply::AddMarker(XPCWSTR strClass, XPCWSTR strTitle, XPCTSTR strURL,XU32 nPicks,XBOOL bFirst)
{
	//MARKCLASS*p=GetClass(strClass);
	//if(p==XNULL) return XFALSE;
	XINT iClass,iMark;
	m_bModify=XTRUE;

	BOOKMARKER*pm=FindMarker(strClass,strTitle,strURL,iClass,iMark);
	MARKCLASS*pc=XNULL;
	if(pm!=XNULL)
	{
		pm->strTitle=strTitle;
		pm->strURL=strURL;
		pc=m_marks[iClass];
		pc->lMarks.RemoveAt(iMark);
	}
	else
	{
		if(iClass<0)
		{
			pc=new MARKCLASS;
			pc->strClass=strClass;
			pc->nPicks=0;
			m_marks.InsertAt(0,pc);
		}
		else
			pc=m_marks[iClass];
		if(strTitle==XNULL) return XFALSE;
		pm=new BOOKMARKER;
		pm->nPicks=0;
		pm->strTitle=strTitle;
		pm->strURL=strURL;
	}
	//pc->lMarks.Add(pm);

	if(bFirst)
		pc->lMarks.InsertAt(0,pm);
	else
		pc->lMarks.Add(pm);
	if(pc->lMarks.GetSize()>MAXBOOKMARKS)
	{
		int id=pc->lMarks.GetSize()-1;
		delete pc->lMarks[id];
		pc->lMarks.RemoveAt(id);
		//pc->lMarks
	}
	if(m_marks.GetSize()>MAXBOOKCLASS)
	{
		int id=m_marks.GetSize()-1;
		m_marks[id]->lMarks.FreeAll();
		delete m_marks[id];
		m_marks.RemoveAt(id);
	}
	return XTRUE;
}

BOOKMARKER* XClientApply::FindMarker(XPCWSTR strClass, XPCWSTR strTitle,
									 XPCTSTR strURL,XINT& iClass,XINT&iMarker)
{
	XU32 i;
	iClass=-1;
	iMarker=-1;
	for( i=0;i<m_marks.GetSize();i++)
	{
		if(!strClass) {iClass=1;break;}
		if(m_marks[i]->strClass==strClass)
		{
			iClass=i;
			break;
		}
	}
	if(iClass<0) return XNULL;
	else if(iClass>=(int)m_marks.GetSize())
	{
		MARKCLASS* pc=new MARKCLASS;
		pc->strClass=XResource::LoadWString(XIDS_BOOKMARK);
		pc->nPicks=0;
		m_marks.InsertAt(0,pc);
	}
	MARKCLASS*p=m_marks[iClass];
	for(i=0;i<p->lMarks.GetSize();i++)
	{
		BOOKMARKER*pm=p->lMarks[i];
		if(pm->strTitle==strTitle||
			pm->strURL==strURL)
		{iMarker=i;return pm;}
	}
	return XNULL;
}

void XClientApply::SetEip( XPCTSTR strEip )
{
	m_info.strEip = strEip;
	XString8 strKey = XEH()[ XEH::EIP ];
	strKey.MakeUpper();
	XSortString8::ESetValue( m_info.strHeaders, strKey, m_info.strEip );
	return;
}

void XClientApply::SetCID(XPCTSTR strCID)
{
	m_info.strChannelID = strCID;
	XString8 strKey=XEH()[XEH::CHANNEL_ID];
	strKey.MakeUpper();
	XSortString8::ESetValue(m_info.strHeaders,strKey,m_info.strChannelID);
    return;
}

void XClientApply::setIDFV( XPCTSTR strIdfv )
{
    m_info.strIDFV = strIdfv;
	XString8 strKey=XEH()[XEH::IDFV];
	strKey.MakeUpper();
	XSortString8::ESetValue(m_info.strHeaders,strKey,m_info.strIDFV);
    return;
}

void XClientApply::setSystemVersion( XPCTSTR strSysVer )
{
    m_info.strSysVer = strSysVer;
	XString8 strKey=XEH()[XEH::SYSVERS];
	strKey.MakeUpper();
	XSortString8::ESetValue(m_info.strHeaders,strKey,m_info.strSysVer);
    return;
}

XBOOL XClientApply::IsEmptyCID()
{
	XBOOL bre = m_info.strChannelID.IsEmpty();
	return bre;
}

XBOOL XClientApply::SetToken(XString8&strToken)
{
	if(m_info.strXToken==strToken) 
		return XFALSE;
	m_info.strXToken=strToken;
	//m_info.m_bModifyToken=XTRUE;
	return XTRUE;
}

void XClientApply::SetMobileType(XString8&strType)
{
	m_info.strMobileType = strType;
	XString8 strKey=XEH()[XEH::MOBILE_TYPE];
	strKey.MakeUpper();
	XSortString8::ESetValue(m_info.strHeaders,strKey,strType);
}

XBOOL XClientApply::SetValue(XDomItem*p)
{
	XString8 strKey,strValue;
	if(!p->GetAttrib(XEAB::NAME,strKey)) return XFALSE;
	if(!p->GetAttrib(XEAB::VALUE,strValue)) return XFALSE;
	XSortConstStr sort(_strSetKeys,sizeof(_strSetKeys)/sizeof(XPCTSTR));

	int id=sort.Index(strKey,XSS_NOCASE);
	//strKey.IndexOfList(_strSetKeys,,
	//XString8::XLS_NOCASE);
	if(id<0) return XFALSE;
	switch(id)
	{
	case SET_CID:
		{
			m_info.strChannelID=strValue;
			XString8 strKey=XEH()[XEH::CHANNEL_ID];
			strKey.MakeUpper();
			XSortString8::ESetValue(m_info.strHeaders,strKey,strValue);
		}break;
	case SET_SEARCH:
		m_strSearch=strValue;
		break;
	case SET_HOME:
		m_strHome=strValue;
		break;
	case SET_HOST:
		m_strHost=strValue;
		break;
	case SET_LOCALPATH:
		m_strLocalPath=strValue;
		break;
	case SET_HELP:
		m_strHelp=strValue;
		break;
	case SET_XTYPE:
#ifdef __ANDROID__
#else
		m_info.strMobileType = strValue;
		{
			//m_strXID=strValue;
			XString8 strKey=XEH()[XEH::MOBILE_TYPE];
			strKey.MakeUpper();
			XSortString8::ESetValue(m_info.strHeaders,strKey,strValue);
		}
#endif
		break;
	case SET_XTOKEN:
		if(m_info.strXToken!=strValue)
		{
			m_info.strXToken=strValue;
			//m_info.m_bModifyToken=XTRUE;
		 }
		break;
	case SET_DELTAX:
		m_nDeltaX=strValue.ToInt();
		if(m_nDeltaX<-50||m_nDeltaX>50)
			m_nDeltaX=0;
		break;
	case SET_DELTAY:
		m_nDeltaY=strValue.ToInt();
		if(m_nDeltaY<-50||m_nDeltaY>50)
			m_nDeltaY=0;
		break;
	case SET_XID:
		{
			SetXID(strValue,strValue,false);
			//m_strXID=strValue;
			//XString8 strKey=XEH()[XEH::CLIENT_ID];
			//strKey.MakeUpper();
			//XSortString8::ESetValue(m_info.strHeaders,strKey,strValue);
		}
		break;
	case SET_DAYS:
		m_strDays=strValue;
		break;
	case SET_PROTOCOL:
		m_info.nProtocol=strValue.Compare(_strProtocol[0],XFALSE)==0?0:1;
		break;
	case SET_RETRIES:
		m_info.nRetries=strValue.ToInt();
		break;
	case SET_TIMEOUT:
		m_info.nTimeout=strValue.ToInt();
		break;
	case SET_PORT:
		m_info.uPort=strValue.ToInt();
		break;
	case SET_GATEWAY:
		m_info.strGateway=strValue;
		break;
	case SET_COLORS:
		break;
	case SET_UA:
		//Init MAC Address
#ifdef __ANDROID__
		if(m_info.strUserAgent.IsEmpty())
		{
			m_info.strUserAgent = BWANDROIDUA;
		}
#elif defined (__APPLE__)
		if(m_info.strUserAgent.IsEmpty())
		{
			m_info.strUserAgent = BWIOSUA;
			//m_info.strUserAgent=strValue;
			//m_info.strUserAgent="AndroidPad;10.0.035;Android2.1;BWTEST/2.5.1";
		}
#else
		if(m_info.strUserAgent.IsEmpty())
		{
			m_info.strUserAgent=strValue;
		}
		else
		{
			int idr=m_info.strUserAgent.ReverseFind('/');
			if(idr<0) m_info.strUserAgent=strValue;
			else
			{
				XString8 strVersion;
				m_info.strUserAgent.Mid(strVersion,idr+1);
				idr=strValue.ReverseFind('/');
				if(idr<0) break;
				XString8 strVersionNew;
				strValue.Mid(strVersionNew,idr+1);
				XString8Array oldv,newv;
				oldv.SplitString(strVersion,'.');
				newv.SplitString(strVersionNew,'.');
				for(int i=0;i<newv.GetSize();i++)
				{
					if(i>=oldv.GetSize())
					{
						m_info.strUserAgent=strValue;
						break;
					}
					int nOld=oldv[i].ToInt();
					int nNew=newv[i].ToInt();
					if(nNew>nOld)
					{
						m_info.strUserAgent=strValue;
						break;
					}
				}
			}			 
		}
#endif
		break;
	case SET_PROFILE:
		m_info.strProfile=strValue;
		break;
	case SET_ACCEPTS:
		XSortString8::ESetValue(m_info.strAccepts,strValue);
		break;
	case SET_VERSION:
		m_nVersion=strValue.ToInt();
		break;
	case SET_DATE:
		m_time.SetTime(strValue);
		break;
	case SET_COOKIE:
		m_bCookie=strValue.Compare("on")==0;
		break;
	}

	return XFALSE;
}

void XClientApply::ResetConfig(XBOOL bDefault,DRAWCONTEXT*pDraw)
{
	m_bCookie=XTRUE;
	m_nVersion=0;
	m_bModify=XTRUE;
	m_time.SetCurrentTime();
	m_time-=(86400<<1);
	m_strLocalPath="/Local";
	XString8 strTxt;
	if(bDefault)
	{
		XResource::LoadText(strTxt,_strConfig);
	}
	else
	{
		XString8 strFile;
		MakeConfigFile(strFile);
		XFile file;
		bool bLoad=false;
		if(file.Open(strFile,XFile::XREAD|XFile::XBINARY))
		{
			int l=file.GetLength();
			if(l>0)
			{
				//strTxt.SetSize(l+1);
				XU8Array data;
				data.SetSize(l);
				file.Read(data.GetData(),l);
				switch(BinDecode(data,l,strTxt))//==DEC_OK)
					//bLoad=true;
				{
					//case DEC_NOCOMPRESS:
					//	strTxt.SetString((char*)data.GetData(),l);
				case DEC_OK:
					bLoad=true;
					break;
				}
			}
			file.Close();
		}
		if(!bLoad)//!XFile::LoadText(strFile,strTxt))
		{
#ifdef __APPLE__
			XResource::LoadText(strTxt, g_deviceData.m_configName);
#else
			XResource::LoadText(strTxt,_strConfig);
#endif
		}
		/*else
		{
		XString8 strOut;
		if(BinDecode((XU8*)strTxt.GetData(),strTxt.GetLength(),strOut)>0)
		strTxt=strOut;
		}*/
	}
	FromWAP(strTxt,XCHARSET_GB2312,pDraw);
	//LOGWHERE();
	if(m_info.strMobileType.IsEmpty())
	{
		if(XSystem::GetClientDesc(m_info.strMobileType))
		{
			if(!m_info.strMobileType.IsEmpty())
			{
				XString8 strKey=XEH()[XEH::MOBILE_TYPE];
				strKey.MakeUpper();
				XSortString8::ESetValue(m_info.strHeaders,strKey,m_info.strMobileType);
			}
		}
	}
	//strTxt.Empty();
	//if(XResource::LoadText(strTxt,_strFilters))
	//	AddFilters(strTxt);
	m_info.nLanguages.EnumAdd(XEnumLan::CHINESE);
	m_info.nCharsets.EnumAdd(XCHARSET_UTF8,80);
	m_info.nCharsets.EnumAdd(XCHARSET_GB2312,80);


	if(!bDefault) return;
	//XString8 strWorkPath=m_strWorkPath;
	m_strFilePath=m_strWorkPath;
#if defined(__APPLE__) || defined(__ANDROID__)
	XString8 strBoot("//_boot.dat");
	m_strFilePath+="//RunFiles";
#else
	XString8 strBoot("\\_boot.dat");
	m_strFilePath+="\\RunFiles";
#endif
	XFile::CreateFolder(m_strFilePath);
	//XString strNew=strPath;
	XString8 strNew=m_strFilePath;
	strNew+=strBoot;
	XFile file;	
	XString8 strContent;
	int length;
	if(file.Open(strNew,XFile::XBINARY|XFile::XREAD))
	{
		int length=file.GetLength();
		if(length<20000)
		{
			strContent.SetSize(length);
			file.Read(strContent,length);
		}
		file.Close();
		m_strBootFile = strNew;
	}
	else
	{
		strNew = m_strWorkPath;
		strNew += strBoot;
		if(file.Open(strNew,XFile::XBINARY|XFile::XREAD))
		{
			//XFile::CreateFolder(
			length=file.GetLength();
			if(length<20000)
			{
				strContent.SetSize(length);
				file.Read(strContent,length);
			}
			file.Close();
			m_strBootFile = strNew;
		}
	}
	if(!strContent.IsEmpty())
	{
		XString8 strMark;
		strContent.Left(strMark,4);
		if(strMark=="BOOT")
			strContent.Mid(m_strBootFile,4,length-4);
	}
	//if(m_info.nLanguages.GetSize()==0) AddLanguage(XEnumLan::CHINESE);
	//if(m_info.nCharsets.GetSize()==0)
	//	AddCharset(XCHARSET_UTF8,80);
	//XString8 strFile=m_strWorkPath;
	//strTxt.Empty();
}

void XClientApply::SaveConfig()
{
	XWAPText wap;
	MakeWAPText(wap);
	XString8 strFile;
	MakeConfigFile(strFile);
	XU8Array data;
	int len=BinEncode((XU8*)wap.GetData(),wap.GetLength(),data);
	if(len<=0) return;
	// Replace the byte array with the compressed data

	//XFile::RemoveFile(strFile);
	XFile file;
	if(file.Open(strFile,XFile::XCREATE|XFile::XWRITE|XFile::XBINARY))
	{
		//file.Write(wap.GetData(),wap.GetLength());
		file.Write(data.GetData(),len);
		file.Close();
	}
}

void XClientApply::MakeWAPText(XWAPText &wap)
{
	XU32 i;
	wap.Create(XTRUE,XCHARSET_UTF8);
	wap.AddToken(XDOM_CARD);//Save Config
	XECG cg;
	wap.AddAttrib(XEAB::NAME,cg[XECG::CONFIG]);
	wap.AddToken(XDOM_P);
	wap.AddToken(XDOM_ANCHOR);
	wap.AddText(cg[XECG::CONFIG]);
	wap.AddToken(XDOM_GO);
	wap.AddAttrib(XEAB::HREF,m_strHost);

	for( i=0;i<sizeof(_strSetKeys)/sizeof(XPCTSTR);i++)
	{
		wap.AddToken(XDOM_SETVAR);
		wap.AddAttrib(XEAB::NAME,_strSetKeys[i]);
		//XString8 str;
		switch(i)
		{
		case SET_SEARCH:
			wap.AddAttrib(XEAB::VALUE,m_strSearch);
			break;
		case SET_CID:
			wap.AddAttrib(XEAB::VALUE,m_info.strChannelID);
			break;
		case SET_HOME:
			//str=m_strHome;
			wap.AddAttrib(XEAB::VALUE,m_strHome);
			break;
		case SET_LOCALPATH:
			wap.AddAttrib(XEAB::VALUE,m_strLocalPath);break;
		case SET_HOST:
			wap.AddAttrib(XEAB::VALUE,m_strHost);break;
		case SET_HELP:wap.AddAttrib(XEAB::VALUE,m_strHelp);break;
		case SET_DAYS:wap.AddAttrib(XEAB::VALUE,m_strDays);break;
		case SET_XTYPE:wap.AddAttrib(XEAB::VALUE,m_info.strMobileType);break;
		case SET_XTOKEN:
			wap.AddAttrib(XEAB::VALUE,m_info.strXToken);break;
			break;
		case SET_DELTAX:
			wap.AddAttrib(XEAB::VALUE,m_nDeltaX);break;
		case SET_DELTAY:
			wap.AddAttrib(XEAB::VALUE,m_nDeltaY);break;
		case SET_XID:wap.AddAttrib(XEAB::VALUE,m_strXID);break;
		case SET_PROTOCOL:
			wap.AddAttrib(XEAB::VALUE,m_info.nProtocol==0?_strProtocol[0]:_strProtocol[1]);
			break;
		case SET_RETRIES:
			wap.AddAttrib(XEAB::VALUE,m_info.nRetries);
			break;
		case SET_TIMEOUT:
			wap.AddAttrib(XEAB::VALUE,m_info.nTimeout);
			break;
		case SET_PORT:
			wap.AddAttrib(XEAB::VALUE,m_info.uPort);break;
		case SET_GATEWAY:
			wap.AddAttrib(XEAB::VALUE,m_info.strGateway);break;
		case SET_UA:
			wap.AddAttrib(XEAB::VALUE,m_info.strUserAgent);break;
		case SET_PROFILE:
			wap.AddAttrib(XEAB::VALUE,m_info.strProfile);break;
		case SET_ACCEPTS:
			wap.AddAttrib(XEAB::VALUE,m_info.strAccepts);break;
			break;
		case  SET_VERSION:
			wap.AddAttrib(XEAB::VALUE,m_nVersion);
			break;
		case SET_DATE:
			wap.AddAttrib(XEAB::VALUE,m_time);
			break;
		case SET_COOKIE:
			wap.AddAttrib(XEAB::VALUE,m_bCookie?"on":"off");break;
		}

		wap.EndToken();
	}
	wap.EndToken();
	wap.EndToken();
	wap.EndToken();
	wap.EndToken();

	wap.AddToken(XDOM_CARD);
	wap.AddAttrib(XEAB::NAME,cg[XECG::NEARBY]);
	wap.AddToken(XDOM_P);
	for(i=0;i<m_lNearest.GetSize();i++)
	{
		wap.AddLink(m_lNearest[i]->strURL,
			XString8(m_lNearest[i]->strTitle),m_lNearest[i]->nPicks);
	}
	wap.EndToken();
	wap.EndToken();

	for(i=0;i<m_marks.GetSize();i++)
	{
		wap.AddToken(XDOM_CARD);
		wap.AddAttrib(XEAB::NAME,cg[XECG::BOOKMARK]);
		wap.AddAttrib(XEAB::TITLE,XString8(m_marks[i]->strClass));
		//if(m_marks[i]->nPicks>0)
		//	wap.AddAttrib(XEAB::ID,m_marks[i]->nPicks);
		for(XU32 j=0;j<m_marks[i]->lMarks.GetSize();j++)
		{
			wap.AddLink(m_marks[i]->lMarks[j]->strURL,
				XString8(m_marks[i]->lMarks[j]->strTitle),
				m_marks[i]->lMarks[j]->nPicks);
		}
		wap.AddToken(XDOM_P);
		wap.EndToken();
		wap.EndToken();
	}


	wap.Final();
}

void XClientApply::MakeConfigFile(XString8 &strFile)
{
	strFile=m_strWorkPath;
	strFile+='/';
	strFile+=_strConfig;
	strFile+=".wml";
	strFile.ConvertToGB();

}

void XClientApply::Click(XURL&url, XPCWSTR strTitle,XBOOL bSameHost)
{
	//int id=AddNearby(url,
	//if(url.m_nProto!=XPROTO_HTTP) return;
	int id=AddNearby(url,strTitle,1,bSameHost);
	if(id>0)
	{
		while(id>0)
		{
			if(m_lNearest[id]->nPicks<=m_lNearest[id-1]->nPicks) break;
			BOOKMARKER*p=m_lNearest[id];
			m_lNearest[id]=m_lNearest[id-1];
			m_lNearest[id-1]=p;
			id--;
			m_bModify=XTRUE;
		}
	}
}


void XClientApply::RemoveAllClick()
{
	m_lNearest.FreeAll();
	m_bModify=XTRUE;
}



int XClientApply::AddNearby(XURL &url, XPCWSTR strTitle, XU32 nPicks,XBOOL bSameHost)
{

	int id=-1;
	for(XU32 i=0;i<m_lNearest.GetSize();i++)
	{
		if(bSameHost)
		{
			if(m_lNearest[i]->strHost!=url.m_strHost) bSameHost=XFALSE;
			else// if(m_lNearest[i]->strURL.Find(".swf"
			{
				int iss=m_lNearest[i]->strURL.ReverseFind('.');
				if(iss>0)
				{
					XString8 strTmp;
					m_lNearest[i]->strURL.Mid(strTmp,iss+1);
					strTmp.MakeLower();
					if(strTmp.Find("swf")==0)
						bSameHost=XFALSE;
					//else
					//	strCnt+="RES@WML";
				}

			}
		}
		if(bSameHost||
			m_lNearest[i]->strTitle==strTitle)
		{
			id=i;
			m_lNearest[i]->strURL.ConvertFrom(url.m_strURL,XCHARSET_GB2312);
			if(strTitle!=XNULL)
				m_lNearest[i]->strTitle=strTitle;
			m_lNearest[i]->nPicks+=nPicks;
			m_bModify=XTRUE;
			return id;
		}
	}
	if(id<0)
	{
		if(strTitle==XNULL) return id;
		m_bModify=XTRUE;
		BOOKMARKER*p=new BOOKMARKER;
		p->nPicks=nPicks;
		p->strHost=url.m_strHost;
		p->strURL.ConvertFrom(url.m_strURL,XCHARSET_GB2312);
		p->strTitle=strTitle;
		m_lNearest.InsertAt(0,p);
		if(m_lNearest.GetSize()>MAXNEARBY)
		{
			delete m_lNearest[m_lNearest.GetSize()-1];
			m_lNearest.RemoveAt(m_lNearest.GetSize()-1);
		}
	}
	return id;
}

void XClientApply::GetLocalPath(XString8 &strPath)
{
	if(m_strLocalPath.Find(":")>=0)
		strPath=m_strLocalPath;
	else
	{
		strPath=m_strWorkPath;
		strPath+=m_strLocalPath;
		XFile::CreateFolder(strPath);
	}
	return;
}



XBOOL XClientApply::Create()
{
#ifdef _SYMBIAN
	XThreadPool *pool = XThreadPool::GetInstance();
	m_pThread=pool->GetThread(this);
	if(m_pThread == XNULL) return XFALSE;
	m_pThread->SetName(_L("ClientApplyThread"));
#else
	m_thread.Create(this,200);
#endif
	return XTRUE;
}

XUINT XClientApply::CallBack(XU32 nID)
{
	if(!m_pClients)
	{
#ifdef _SYMBIAN
		m_pThread->Sleep(100);
#else
		m_thread.Sleep(100);
#endif
		return nID;
	}
	//XLock lock(&m_dataLock); //modify by hgl
	if(!m_dataLock.Lock(XSHORTTIME))
		return nID;
	XClient*pClient=m_pClients;
	while(pClient)
	{
		if(NeedStop())
		{
			break;
		}
		if(!pClient->IsFinished())
		{
			pClient->CallBack(nID);
		}
		pClient=pClient->m_pNext;
	}
	m_dataLock.UnLock();

#ifdef _SYMBIAN
	m_pThread->Sleep(10000);
#else
	m_thread.Sleep(10);
#endif

	return nID;
}

void XClientApply::ReleaseClient()
{
	//XLock lock(&m_dataLock); //modify by hgl
	if(!m_dataLock.Lock(XSHORTTIME))
		return;
	while(m_pClients)
	{
		XClient*pNext=m_pClients->m_pNext;
		delete m_pClients;
		m_pClients=pNext;
	}
	while(m_pFreeClients)
	{
		XClient*pNext=m_pFreeClients->m_pNext;
		delete m_pFreeClients;
		m_pFreeClients=pNext;
	}
	m_dataLock.UnLock();
}

void XClientApply::UpdateClient()
{
	//XLock lock(&m_dataLock); modify by hgl
	if(!m_dataLock.Lock(XSHORTTIME)) return;
	XClient**pp=&m_pClients;
	while(*pp)
	{
		XClient*pc=*pp;
		//if(!pc) break;
		if(pc->IsFinished()&&!pc->IsUsing())
		{
			*pp=pc->m_pNext;
			pc->m_pNext=m_pFreeClients;
			m_pFreeClients=pc;
			//delete pc;
			continue;
		}
		pp=&pc->m_pNext;
	}
	m_dataLock.UnLock();
}

XDOMFILTER* XClientApply::GetFilter(XString8& strURL,XBOOL bFull)
{
	XDOMFILTER*p=m_pFilters;
	//int len=
	while(p)
	{
		int l=(p->bFullMatch||bFull)?0:p->strHost.GetLength();//0:XMIN(strURL.GetLength(),p->strHost.GetLength());
		if( p->strHost.Compare(strURL,XTRUE,l)==0)
			return p;
		p=p->pNext;
	}
	return XNULL;
}

static const char* _strFilterKey[]=
{
	"admode",
	"adurl",
	"advancegc",
	"allowdomain",
	"autocap",
	"autoform",
	"autoframe",
	"autogo",
	"autoheaders",
	"autolocation",
	"autoobject",
	"backcolor",
	"begin",
//#if (__CORE_VERSION__>=0x02075000)
	"blockdiv",
//#endif
	"cachetime",
	"classlock",
	"contentid",
	"dispatchlater",
//#if (__CORE_VERSION__>=0x02070200)
	"dotadjust",
//#endif
	"end",
	"endurl",
	"enufilter",
	"enumode",
//#if (__CORE_VERSION__>=0x02070200)
	"externapi",
//#endif
	"externfun",
	"externfun1",
	"externfun2",
//#if (__CORE_VERSION__>=0x02079000)
	"externfun3",
	"externfun4",
//#endif
	"fheight",
	"font",
	"fwidth",
	"hold",
	"host",
	"imagediv",
	"loadflex",
	"loadingbg",
	"loadscript",
	"loadsrc",
	"match",
	"maxpecent",
	"mode",
	"modifydomain",
	"param",
	"payurl",
	"pluscookies",
	"prevfilter",
	"quality",
	"refesh",
//#if (__CORE_VERSION__>=0x02080000)
	"replacebin",
//#endif
	"replacelib",
	"replaceswz",
	"retgo",
	"showmouse",
	"waitfor",
	//------------------
};
enum
{
	FLT_ADMODE=1,
	FLT_ADURL,
	FLT_ADVANCEGC,
	FLT_ALLOWDOMAIN,
	FLT_AUTOCAP,
	FLT_AUTOFORM,
	FLT_AUTOFRAME,
	FLT_AUTOGO,
	FLT_AUTOHEADERS,
	FLT_AUTOLOCATION,
	FLT_AUTOOBJECT,
	FLT_BACKCOLOR,
	FLT_BEGIN,
//#if (__CORE_VERSION__>=0x02075000)
	FLT_BLOCKDIV,
//#endif
	FLT_CACHE,
	FLT_CLASSLOCK,
	FLT_CONTENTID,
	FLT_DISPATCHLATER,
//#if (__CORE_VERSION__>=0x02070200)
	FLT_DOTADJUST,
//#endif
	FLT_END,
	FLT_ENDURL,
	FLT_ENUFILTER,
	FLT_ENUMODE,
//#if (__CORE_VERSION__>=0x02070200)
	FLT_EXTERNAPI,
//#endif
	FLT_EXTERNFUN,
	FLT_EXTERNFUN1,
	FLT_EXTERNFUN2,
//#if (__CORE_VERSION__>=0x02079000)
	FLT_EXTERNFUN3,
	FLT_EXTERNFUN4,
//#endif
	FLT_FHEIGHT,
	FLT_FONT,
	FLT_FWIDTH,
	FLT_HOLD,
	FLT_HOST,
	FLT_IMAGEDIV,
	FLT_LOADFLEX,
	FLT_LOADINGBG,
	FLT_LOADSCRIPT,
	FLT_LOADSRC,
	FLT_MATCH,
	FLT_MAXPECENT,
	FLT_MODE,
	FLT_MODIFYDOMAIN,
	FLT_PARAM,
	FLT_PAYURL,
	FLT_PLUSCOOKIES,
	FLT_PREVFILTER,
	FLT_QUALITY,
	FLT_REFESH,
//#if (__CORE_VERSION__>=0x02080000)
	FLT_REPLACEBIN,
//#endif
	FLT_REPLACELIB,
	FLT_REPLACESWZ,
	FLT_RETGO,
	FLT_SHOWMOUSE,
	FLT_WAITFOR,
	FLT_VARS=0xffff
};

int XClientApply::GetContent(XPCTSTR strTxt,XString8&txt,XString8&strKey)
{
	int l=0;
	while(strTxt[l])
	{

		if(strTxt[l]=='{')
		{
			l++;
			break;
		}
		strKey+=strTxt[l];
		l++;
	}
	strKey.TrimLeft();
	strKey.TrimRight();
	while(strTxt[l])
	{
		if(strTxt[l]=='}')
		{
			if(strTxt[l+1]=='}')
			{
				txt+='}';
				l+=2;
				continue;
			}
			else {l++;break;}
		}
		else if(strTxt[l]=='{')
		{
			if(strTxt[l+1]=='{')
			{
				txt+='{';
				l+=2;
				continue;
			}
		}
		txt+=strTxt[l];
		l++;
	}
	return l;
}

int XClientApply::GetLine(XPCTSTR strTxt,XString8&strKey,XString8&strValue)
{
	int l=0;
	while(strTxt[l]&&strTxt[l]<=' ') l++;
	while(strTxt[l])
	{
		if(strTxt[l]=='\r'||strTxt[l]=='\n'||strTxt[l]=='=')
		{
			l++;
			break;
		}
		strKey+=strTxt[l];
		//txt+=strTxt[l];
		l++;
	}
	while(strTxt[l])
	{
		if(strTxt[l]=='\r'||strTxt[l]=='\n')
		{
			l++;
			break;
		}
		strValue+=strTxt[l];
		//txt+=strTxt[l];
		l++;
	}
	strKey.TrimLeft();
	strKey.TrimRight();
	strValue.TrimLeft();
	strValue.TrimRight();
	return l;
}

XRETGO* XClientApply::ParserRetGO(XPCTSTR strTxt,int&l)
{
	XString8 txt,strName;
	l=GetContent(strTxt,txt,strName);
	XRETGO*pGo=NULL;
	strTxt=txt;
	for(;;)
	{
		XString8 strKey,strValue;
		int s=GetLine(strTxt,strKey,strValue);
		if(!s||strKey.IsEmpty()) break;
		strTxt+=s;
		XString8Array list;
		list.SplitString(strValue,'\'');
		if(!list.GetSize()) break;
		if(pGo==NULL)
			pGo=new XRETGO;
		if(!pGo) break;
		pGo->strOptions.Add(strKey);
		pGo->strURLs.Add(list[0]);
		if(list.GetSize()>=2)
		{
			pGo->nMethods=XEM::Index(list[1]);
			/*char*p=list[1].GetData();
			if(p[0]=='P'||p[0]=='p')
			pGo->nMethods.Add(XEM::POST);
			else
			pGo->nMethods.Add(XEM::GET);*/
		}
		else pGo->nMethods.Add(XEM::GET);
		if(list.GetSize()>=3)
			pGo->strPosts.Add(list[2]);
		else
			pGo->strPosts.Add("");

	}
	return pGo;
}

int	XClientApply::ParserExtern(XPCTSTR strTxt,XString8&strName,XString8&strValue)
{
	return GetContent(strTxt,strValue,strName);
}


XPREVFILTER* XClientApply::ParserFilter(XPCTSTR strTxt,int&l)
{
	XString8 txt,strKey;
	l=GetContent(strTxt,txt,strKey);
	XPREVFILTER*pFilter=NULL;
	strTxt=txt;
	for(;;)
	{
		XString8 strKey,strValue;
		int s=GetLine(strTxt,strKey,strValue);
		if(!s||strKey.IsEmpty()) break;
		strTxt+=s;
		if(!pFilter)
		{
			pFilter=new XPREVFILTER;
			pFilter->bPost=false;
			pFilter->nMethod=XEM::GET;
			pFilter->nType=XPREVFILTER::_SELF;
			pFilter->nReplace = XPREVFILTER::_FILTER_FULL;
			pFilter->pNext=NULL;
		}
		if(!pFilter) break;
		if(strKey.Compare("method",XTRUE)==0)
		{
			pFilter->nMethod=XEM::Index(strValue);
			//char*p=strValue.GetData();
		}
		else if(strKey.Compare("source",XTRUE)==0)
			pFilter->strHost=strValue;
		else if(strKey.Compare("dest",XTRUE)==0)
			pFilter->strDest=strValue;
		else if(strKey.Compare("post",XTRUE)==0)
			pFilter->strPost=strValue;
		else if(strKey.Compare("target",XTRUE)==0)
		{
			char*p=strValue.GetData();
			switch(p[0])
			{
			case 'p':
			case 'P':
				pFilter->nType=XPREVFILTER::_POPUP;
				break;
			case 'n':
			case 'N':
				pFilter->nType=XPREVFILTER::_NULL;
				break;
			default:
				pFilter->nType=XPREVFILTER::_SELF;
				break;
			}
		}
		else if ( strKey.Compare( "replace", XTRUE) == 0 )
		{
			char *p = strValue.GetData();
			switch(p[0])
			{
			case 'F':
			case 'f':
				pFilter->nReplace = XPREVFILTER::_FILTER_FULL;
				break;
			case 'H':
			case 'h':
				pFilter->nReplace = XPREVFILTER::_FILTER_HALT;
				break;
			default:
				pFilter->nReplace = XPREVFILTER::_NULL;
				break;
			}
		}
		else if(strKey.Compare("reppost",XTRUE)==0)
		{
			char*p=strValue.GetData();
			switch(p[0])
			{
			case 't':
			case 'T':
				pFilter->bPost=true;
				break;
			}
		}
		else if(strKey.Compare("postdest",XTRUE)==0)
		{
			pFilter->strPostDest=strValue;
		}

	}
	return pFilter;
}
void XClientApply::AddFilters(XPCTSTR strTxt,XDomView*pView)
{
	XString8 strWord,strKey;
	XU16 nBegin=0,nSet=0,nMethod=0;
	XDOMFILTER*pFilter=XNULL;
	XBOOL bFullMatch=XTRUE;
	while(*strTxt)
	{
		char c=*strTxt;
		switch(c)
		{
		case '{':
			if(nBegin&&strTxt[1]=='{')
			{
				strWord+='{';
				strTxt++;
				break;
			}
			nBegin=1;
			bFullMatch=XTRUE;
			//strHost.Empty();
			//strBegin.Empty();
			//strEnd.Empty();
			strWord.Empty();
			//strAutoGo.Empty();
			//strAutoFrame.Empty();
			//strAutoLocation.Empty();
			//strParam.Empty();
			//strAutoPost.Empty();
			nSet=0;
			pFilter=XNULL;
			break;
		case '=':
			if(nSet==0)
			{
				strWord.TrimLeft();
				strWord.TrimRight();

				if(!strWord.IsEmpty())
				{
					/*if(strWord=="host") nSet=1;
					else if(strWord=="begin") nSet=2;
					else if(strWord=="end") nSet=3;
					else if(strWord=="autogo") nSet=4;
					else if(strWord=="autoframe") nSet=5;
					else if(strWord=="autoLocation") nSet=6;
					else if(strWord=="param") nSet=7;
					else if(strWord=="autoHeaders") nSet=8;
					//else if(strWord=="drop") nSet=8;*/
					XSortConstStr sort(_strFilterKey,sizeof(_strFilterKey)/sizeof(XPCTSTR));
					//if(strWord=="match")
					//	  int v=0;
					int id=sort.Index(strWord.GetData(),XSS_NOCASE);
					if(id>=0)
						nSet=id+1;
					else
					{
						strKey=strWord;
						nSet=FLT_VARS;
					}
					strWord.Empty();
					if(nSet==FLT_RETGO)
					{
						int l=0;
						XRETGO*pGo=ParserRetGO(strTxt,l);
						strTxt+=l;
						if(pGo)
						{
							if(pFilter)
							{
								if(pFilter->pRetGO)
									delete pFilter->pRetGO;
								pFilter->pRetGO=pGo;
							}
							else
								delete pGo;
						}
					}
					else if(nSet==FLT_EXTERNFUN1)
					{
						XString8 strName,strValue;
						int l=ParserExtern(strTxt+1,strName,strValue);
						strTxt+=(l+1);
						if(!strName.IsEmpty()&&!strValue.IsEmpty()&&pFilter)
						{
							pFilter->strExtFuns.Add(strName);
							pFilter->strExtFuns.Add(strValue);
						}
					}
//#if (__CORE_VERSION__>=0x02079000)
					else if(nSet==FLT_EXTERNFUN3)
					{
						XString8 strName,strValue;
						int l=ParserExtern(strTxt+1,strName,strValue);
						strTxt+=(l+1);
						if(!strName.IsEmpty()&&!strValue.IsEmpty()&&pFilter)
						{
							pFilter->strExtFuns3.Add(strName);
							pFilter->strExtFuns3.Add(strValue);
						}
					}
//#endif
					else if(nSet==FLT_PREVFILTER)
					{
						int l=0;
						XPREVFILTER*p=ParserFilter(strTxt,l);
						strTxt+=l;
						if(p)
						{
							if(!pFilter) delete p;
							else
							{
								//bool bExist=false;
								XPREVFILTER*pExist=NULL;
								XPREVFILTER*pm=pFilter->pRevFilter;
								while(pm)
								{
									if(pm->strHost==p->strHost)
									{
										pExist=pm;
										//bExist=true;
										break;
									}
									pm=pm->pNext;
								}
								if(!pExist)
								{
									p->pNext=pFilter->pRevFilter;
									pFilter->pRevFilter=p;
								}
								else 
								{
									pExist->nMethod=p->nMethod;
									pExist->nType=p->nType;
									pExist->strDest=p->strDest;
									pExist->strPost=p->strPost;
									delete p;
								}
							}
						}
					}
				}
				break;
			}
		default:
			if(nBegin)
			{
				strWord+=c;
				/*switch(nSet)
				{
				case FLT_AUTOHEADERS:
				strAutoLocation+=c;
				break;
				case FLT_PARAM:
				strParam+=c;break;
				case FLT_AUTOLOCATION://auto Location
				strAutoLocation+=c;break;
				default:
				if(c>' ')
				strWord+=c;
				break;
				case FLT_AUTOFRAME:
				strAutoFrame+=c;
				break;
				case FLT_AUTOGO:
				strAutoGo+=c;
				break;
				case FLT_HOST:
				strHost+=c;
				break;
				case FLT_BEGIN:
				strBegin+=c;
				break;
				case FLT_END:
				strEnd+=c;
				break;
				}*/
			}
			break;
			//case '[':
			//case ']':
			//case '\'':
			//case ',':
			if(nBegin&&strTxt[1]==c)
			{
				strWord+=c;
				strTxt++;
			}
			break;
		case '}':
			if(nBegin&&strTxt[1]=='}')
			{
				strTxt++;
				strWord+='}';
				break;
			}
			nBegin=0;
		case '\r':
		case '\n':
			strWord.TrimLeft(' ');
			strWord.TrimRight(' ');
			if(nSet)
			{
				if(nSet==FLT_MATCH)
				{

					char*cc=(char*)strWord.GetData();
					if(cc[0]=='d'||cc[0]=='D')
						bFullMatch=XFALSE;
					strWord.Empty();
					nSet=0;
					break;
				}
				if(nSet==FLT_HOST)
				{
					if(!strWord.IsEmpty())//&&
						//!strBegin.IsEmpty()&&
						//!strEnd.IsEmpty())
					{
						pFilter=GetFilter(strWord,XTRUE);
						if(pFilter==XNULL)
						{
							pFilter=new XDOMFILTER;
							if(!pFilter) return;
							pFilter->strHost=strWord;
							//pFilter->strAutoLocation=strAutoLocation;
							//pFilter->nMethod=nMethod;
							//pFilter->strAutoPost=strAutoPost;
							//pFilter->strHost=strHost;
							pFilter->pNext=m_pFilters;
							//pFilter->nAutoGo=0;
							pFilter->nAutoCap=0;
							pFilter->nAutoObject=1;
							//pFilter->bDrop=XFALSE;
							pFilter->nMethod=XEM::GET;
							pFilter->nMatchID=0;
							pFilter->bModifyDomain=XFALSE;
							pFilter->nFontBase=0;
							pFilter->nFontZoom=0;
							//pFilter->nContentMode=XCNT_MODE_NET;
							//pFilter->nQuality=0;
							pFilter->nAutoForm=0;//XFALSE;
							pFilter->nCacheBitmap=12000;
							pFilter->nCacheImage=4000;
							pFilter->nCacheObject=0xfff;
							pFilter->nCacheMemory=0;
							pFilter->nImgDiv=0;
							pFilter->pRetGO=NULL;
							pFilter->nFlex=0;//false;
							pFilter->bBegin=XTRUE;
							pFilter->nEnd=4;
							pFilter->pRevFilter=NULL;
							pFilter->bLoadScript=XFALSE;
							pFilter->nFrameWidth=0;
							pFilter->nBackColor=0xff000000;
							pFilter->nFrameHeight=0;
//#if (__CORE_VERSION__>=0x02075000)
							pFilter->m_nBlockWidth=0;
							pFilter->m_nBlockHeight=0;
//#endif
//#if (__CORE_VERSION__>=0x02070200)
							pFilter->bEnuFilter=XTRUE;
//#else
//							pFilter->bEnuFilter=XFALSE;
//#endif
//#if (__CORE_VERSION__>=0x02070200)
							pFilter->bExternAPI=XFALSE;
							pFilter->bDotAdjust=XFALSE;
//#endif
							pFilter->bEnuBlendMode=XTRUE;
							pFilter->bFullMatch=bFullMatch;
							pFilter->nADMode=XDOMFILTER::AD_TOP;
							pFilter->bAdvanceGC=XFALSE;
							pFilter->nMaxPecent=0;
							pFilter->bReplaceSWZ=false;
							pFilter->nWaitID=1;
							pFilter->bHold=XFALSE;
							pFilter->bDispatchLater=XFALSE;
							//if(!strAutoGo.IsEmpty())
							//   pFilter->bAutoGo=strAutoGo.ToLogic();
							pFilter->nAutoFrame=1;
							//if(!strAutoFrame.IsEmpty())
							//   pFilter->bAutoFrame=strAutoFrame.ToLogic();
							m_pFilters=pFilter;
						}
						//strHost.Empty();
						//strAutoFrame.Empty();
						//strAutoGo.Empty();
					}
					nSet=0;
					strWord.Empty();
					break;
				}

				if(pFilter)//&&!strAutoLocation.IsEmpty())
				{
					switch(nSet)
					{
					default:
						break;
					case FLT_DISPATCHLATER:
						if(!strWord.IsEmpty())
						{
							pFilter->bDispatchLater=strWord.ToLogic();
							strWord.Empty();
						}break;
//#if (__CORE_VERSION__>=0x02080000)
					case FLT_REPLACEBIN:
						if(!strWord.IsEmpty())
						{
							XString8Array list;
							list.SplitString(strWord,',');
							int i,nc=(list.GetSize()>>1)<<1;
							if(nc>=2)
							{
								for(i=0;i<nc;i+=2)
								{									
									XString8 &str1=list[i];
									XString8 &str2=list[i+1];
									str1.MakeLower();
									str2.MakeLower();
									int len=(str1.GetLength()>>1)<<1;
									if(len==str2.GetLength()&&len>=2)
									{
										int j,nSize=pFilter->repSrc.GetSize();
										pFilter->repSrc.SetSize(nSize+1);
										XU8Array&data=pFilter->repSrc[nSize];
										for(j=0;j<len;j+=2)
										{
											XU8 c=str1[j],v=0;
											if(c>='0'&&c<='9')
												v|=(c-'0')<<4;
											else
												v|=(c-'a'+10)<<4;
											c=str1[j+1];
											if(c>='0'&&c<='9')
												v|=(c-'0');
											else
												v|=(c-'a'+10);
											data.Add(v);
										}
										pFilter->repDst.SetSize(nSize+1);
										XU8Array&data1=pFilter->repDst[nSize];
										for(j=0;j<len;j+=2)
										{
											XU8 c=str2[j],v=0;
											if(c>='0'&&c<='9')
												v|=(c-'0')<<4;
											else
												v|=(c-'a'+10)<<4;
											c=str2[j+1];
											if(c>='0'&&c<='9')
												v|=(c-'0');
											else
												v|=(c-'a'+10);
											data1.Add(v);
										}
									}
								}
							}
						}break;
//#endif
					case FLT_REPLACELIB:
						if(!strWord.IsEmpty())
						{
							int id=strWord.Find(',');
							if(id>0)
							{
								XString8 strSource,strDest;
								strWord.Left(strSource,id);
								strWord.Mid(strDest,id+1);
								strSource.MakeLower();
								pFilter->strReplaceLibs.Add(strSource);
								pFilter->strReplaceLibs.Add(strDest);
							}
						}break;
					case FLT_REPLACESWZ:
						if(!strWord.IsEmpty())
						{
							pFilter->bReplaceSWZ=strWord.ToLogic();
							strWord.Empty();
							//break;
						}break;
//#if (__CORE_VERSION__>=0x02070200)
					case FLT_DOTADJUST:
						if(!strWord.IsEmpty())
						{
							pFilter->bDotAdjust=strWord.ToLogic();
							strWord.Empty();
						}break;
					case FLT_EXTERNAPI:
						if(!strWord.IsEmpty())
						{
							pFilter->bExternAPI=strWord.ToLogic();
							strWord.Empty();
						}break;
//#endif
					case FLT_ENUFILTER:
						if(!strWord.IsEmpty())
						{
							pFilter->bEnuFilter=strWord.ToLogic();
							strWord.Empty();
						}break;
					case FLT_ENUMODE:
						if(!strWord.IsEmpty())
						{
							pFilter->bEnuBlendMode=strWord.ToLogic();
							strWord.Empty();
						}break;
					case FLT_HOLD:
						if(!strWord.IsEmpty())
						{
							pFilter->bHold=strWord.ToLogic();
							strWord.Empty();
						}
						break;
					case FLT_PLUSCOOKIES:
						if(!strWord.IsEmpty())
						{
							if(pView)
							{
								XURL url;
								url.SetURL(pFilter->strHost);
								pView->SetURLCookie(url.m_strHost,strWord,XTime::GetTimeMSec());
							}
							pFilter->strCookies=strWord;
							strWord.Empty();
						}
						break;
					case FLT_WAITFOR:
						if(!strWord.IsEmpty())
						{
							XString8Array list;
							list.SplitString(strWord,',');
							pFilter->strWaitFor=list[0];//strWord;
							if(list.GetSize()>=2)
								pFilter->nWaitID=list[1].ToInt();
							strWord.Empty();
						}break;
					case FLT_SHOWMOUSE:
						if(!strWord.IsEmpty())
						{
							pFilter->bMouseShow=strWord.ToLogic();
							strWord.Empty();
						}break;
					case FLT_VARS:
						if(!strWord.IsEmpty())
						{
							pFilter->strVars.Add(strKey);
							pFilter->strVars.Add(strWord);
						}
						strWord.Empty();
						break;
					case FLT_EXTERNFUN2:
						if(!strWord.IsEmpty())
						{
							XString8 name,value;
							const char*str=strWord.GetData();
							const char*mid=::strchr(str,'\'');
							name.SetString(str,mid-str);
							value.SetString(mid+1,0);
							int i,nc=pFilter->strExtFuns2.GetSize();
							bool bSet=false;
							if(name.Compare("defaultforall",XTRUE,0)==0)
								name.Empty();
							for(i=0;i<nc;i+=2)
							{
								if(pFilter->strExtFuns2[i]==name)
								{
									pFilter->strExtFuns2[i+1]=value;
									bSet=true;
									break;
								}
							}
							if(!bSet)
							{
								pFilter->strExtFuns2.Add(name);
								pFilter->strExtFuns2.Add(value);
							}
							strWord.Empty();
						}break;
//#if (__CORE_VERSION__>=0x02079000)
					case FLT_EXTERNFUN4:
						if(!strWord.IsEmpty())
						{
							const char*str=strWord.GetData();
							const char*mid=::strchr(str,'\'');
							if(mid)
							{
								XString8 name,value;
								name.SetString(str,mid-str);
								value.SetString(mid+1,0);
								int i,nc=pFilter->strExtFuns4.GetSize();
								bool bSet=false;
								if(name.Compare("defaultforall",XTRUE,0)==0)
									name.Empty();
								for(i=0;i<nc;i+=2)
								{
									if(pFilter->strExtFuns4[i]==name)
									{
										pFilter->strExtFuns4[i+1]=value;
										bSet=true;
										break;
									}
								}
								if(!bSet)
								{
									pFilter->strExtFuns4.Add(name);
									pFilter->strExtFuns4.Add(value);
								}
								//strWord.Left(mid-str);
							}
							strWord.Empty();
						}break;
//#endif
					case FLT_EXTERNFUN:
						if(!strWord.IsEmpty())
						{
							const char*str=strWord.GetData();
							const char*mid=::strchr(str,'\'');
							if(mid)
							{
								XString8 name,value;
								name.SetString(str,mid-str);
								value.SetString(mid+1,0);
								int i,nc=pFilter->strExtFuns.GetSize();
								bool bSet=false;
								if(name.Compare("defaultforall",XTRUE,0)==0)
									name.Empty();
								for(i=0;i<nc;i+=2)
								{
									if(pFilter->strExtFuns[i]==name)
									{
										pFilter->strExtFuns[i+1]=value;
										bSet=true;
										break;
									}
								}
								if(!bSet)
								{
									pFilter->strExtFuns.Add(name);
									pFilter->strExtFuns.Add(value);
								}
								//strWord.Left(mid-str);
							}
							strWord.Empty();
						}break;
					case FLT_MAXPECENT:
						if(!strWord.IsEmpty())
						{
							pFilter->nMaxPecent=strWord.ToInt();
							if(pFilter->nMaxPecent<0)
								pFilter->nMaxPecent=0;
							strWord.Empty();
						}

					case FLT_ADVANCEGC:
						if(!strWord.IsEmpty())
						{
							pFilter->bAdvanceGC=strWord.ToLogic();
							strWord.Empty();
						}
						break;
					case FLT_CONTENTID:
						if(!strWord.IsEmpty())
						{
							pFilter->strContentID=strWord;
							strWord.Empty();
						}break;
					case FLT_ENDURL:
						if(!strWord.IsEmpty())
						{
							pFilter->strEndURL=strWord;
							strWord.Empty();
						}break;
					case FLT_ADURL:
						if(!strWord.IsEmpty())
						{
							pFilter->strADURL=strWord;
							strWord.Empty();
						}break;
					case FLT_ADMODE:
						if(!strWord.IsEmpty())
						{
							switch(strWord[0])
							{
							default:
							case '2':pFilter->nADMode=XDOMFILTER::AD_TOP;break;
							case '1':pFilter->nADMode=XDOMFILTER::AD_TOPLEFT;break;
							case '3':pFilter->nADMode=XDOMFILTER::AD_TOPRIGHT;break;
							case '4':pFilter->nADMode=XDOMFILTER::AD_LEFT;break;
							case '5':pFilter->nADMode=XDOMFILTER::AD_CENTER;break;
							case '6':pFilter->nADMode=XDOMFILTER::AD_RIGHT;break;
							case '7':pFilter->nADMode=XDOMFILTER::AD_LEFTBOTTOM;break;
							case '8':pFilter->nADMode=XDOMFILTER::AD_BOTTOM;break;
							case '9':pFilter->nADMode=XDOMFILTER::AD_RIGHTBOTTOM;break;
								/*default:
								case 't':
								case 'T':pFilter->nADMode=XDOMFILTER::AD_TOP;
								break;
								case 'b':
								case 'B':pFilter->nADMode=XDOMFILTER::AD_BOTTOM;
								break;
								case 'r':
								case 'R':pFilter->nADMode=XDOMFILTER::AD_RIGHT;
								break;
								case 'l':
								case 'L':pFilter->nADMode=XDOMFILTER::AD_LEFT;
								break;*/
								//case '0'
							}
							strWord.Empty();
						}break;
					case FLT_FWIDTH:
						if(!strWord.IsEmpty())
						{
							pFilter->nFrameWidth=strWord.ToInt();
							strWord.Empty();
						}break;
					case FLT_REFESH:
						if(!strWord.IsEmpty())
						{
							pFilter->strRefeshURL=strWord;
							strWord.Empty();
						}break;
					case FLT_FHEIGHT:
						if(!strWord.IsEmpty())
						{
							pFilter->nFrameHeight=strWord.ToInt();
							strWord.Empty();
							break;
						}break;
					case FLT_ALLOWDOMAIN:
						if(!strWord.IsEmpty())
						{
							pFilter->strAllows.Add(strWord);
						}break;
					case FLT_IMAGEDIV:
						if(!strWord.IsEmpty())
						{
							pFilter->nImgDiv=strWord.ToInt();
							strWord.Empty();
						}break;
					case FLT_LOADSRC:
						if(!strWord.IsEmpty())
						{
							pFilter->strLoaders.Add(strWord);
							strWord.Empty();
						}break;
					case FLT_LOADINGBG:
						if(!strWord.IsEmpty())
						{
							pFilter->strLoadingBG=strWord;
							strWord.Empty();
						}break;
					case FLT_LOADFLEX:
						if(!strWord.IsEmpty())
						{
							pFilter->nFlex=strWord.ToInt();
							strWord.Empty();
						}break;
					case FLT_LOADSCRIPT:
						if(!strWord.IsEmpty())
						{
							pFilter->bLoadScript=strWord.ToLogic();
							strWord.Empty();
						}break;
					case FLT_MODIFYDOMAIN:
						{
							if(!strWord.IsEmpty())
							{
								pFilter->bModifyDomain=strWord.ToLogic();
								strWord.Empty();
							}
						}break;
					case FLT_AUTOOBJECT:
						if(!strWord.IsEmpty())
						{
							pFilter->nAutoObject=strWord.ToInt();
							strWord.Empty();
						}break;
					case FLT_AUTOCAP:
						if(!strWord.IsEmpty())
						{
							pFilter->nAutoCap=strWord.ToInt();
							break;
						}
						break;
					case FLT_AUTOFORM:
						if(!strWord.IsEmpty())
						{
							int id=strWord.Find('(');
							if(id<0)
								pFilter->nAutoForm=strWord.ToInt();
							else
							{
								int ide=strWord.Find(')');
								XString8 strInt,strDatas;
								strWord.Left(strInt,id);
								pFilter->nAutoForm=strInt.ToInt();
								if(ide>id)
									strWord.Mid(strDatas,id+1,ide-id-1);
								else
									strWord.Mid(strDatas,id+1);
								XString8Array list;
								list.SplitString(strDatas,',');
								int i,nc=list.GetSize();
								for(i=0;i<nc;i++)
								{
									XString8&strValue=list[i];
									id=strValue.Find(':');
									if(id<=0) continue;
									XString8 strKey,strCnt;
									strValue.Left(strKey,id);
									strValue.Mid(strCnt,id+1);
									pFilter->strFormDatas.Add(strKey);
									pFilter->strFormDatas.Add(strCnt);
								}
							}
							strWord.Empty();
						}break;
					case FLT_CLASSLOCK:
						if(!strWord.IsEmpty())
						{
							pFilter->strClassLock.Add(strWord);
							strWord.Empty();
						}break;
					case FLT_CACHE:
						if(!strWord.IsEmpty())
						{
							XString8Array list;
							list.SplitString(strWord,',');
							switch(list.GetSize())
							{
							case 0:
								break;
							default:
							case 4:
								pFilter->nCacheMemory=list[3].ToInt()*1000;
							case 3:
								pFilter->nCacheObject=list[2].ToInt();
							case 2:
								pFilter->nCacheBitmap=list[1].ToInt()*1000;
							case 1:
								pFilter->nCacheImage=list[0].ToInt()*1000;
								break;
							}
							strWord.Empty();
						}
						break;
					case FLT_QUALITY:
						//switch(strWord[0])
						//{
						//default:
						//	pFilter->nQuality=0;
						//	break;
						//case 'h':
						//case 'H':
						//	pFilter->nQuality=3;
						//	break;
						//case 'm':
						//case 'M':
						//	pFilter->nQuality=2;
						//	break;
						//case 'l':
						//case 'L':
						//	pFilter->nQuality=1;
						//	break;
						//}
						strWord.Empty();
						break;
					case FLT_BACKCOLOR:
						if(!strWord.IsEmpty())
						{
							pFilter->nBackColor=strWord.ToInt()|0xff000000;
							break;
						}break;
					case FLT_MODE:
						//switch(strWord[0])
						//{
						//default:
						//	pFilter->nContentMode=XCNT_MODE_LOCAL;
						//	break;
						//case 'n':
						//case 'N':
						//	pFilter->nContentMode=XCNT_MODE_NET;
						//	break;
						//case 'v':
						//case 'V':
						//	pFilter->nContentMode=XCNT_MODE_VIDEO;
						//	break;
						//}
						LOGWHERE();
						strWord.Empty();
						break;
						//case FLT_HOST://Host
						//	pFilter->strHost=strWord;
						//	strWord.Empty();
						break;
					case FLT_AUTOHEADERS:
						//pFilter->bDrop=strAutoGo.ToLogic();
						{
							XString8Array list;
							list.SplitString(strWord,':');
							if(list.GetSize()>=2)
							{
								pFilter->strAutoHeaders.Add(list[0]);
								pFilter->strAutoHeaders.Add(list[1]);
							}
							//pFilter->strAutoHeaders.SplitString(strAutoLocation,';');
							//strAutoLocation.Empty();
							strWord.Empty();
						}
						break;
					case FLT_FONT:
						{
							XString8Array list;
							list.SplitString(strWord,',');
							if(list.GetSize()>=2)
							{
								pFilter->nFontBase=list[0].ToInt()*20;
								pFilter->nFontZoom=list[1].ToInt()*256/100;
							}
							strWord.Empty();
						}break;
					case FLT_PAYURL:
						if(!strWord.IsEmpty())
						{
							pFilter->strPayURL=strWord;
							strWord.Empty();
#ifdef __ANDROID__
                            pView->PostMsg(ANDROID_NOTICE_PAYURL,0, (XU32)pFilter->strPayURL.GetData(),XMS_TOPMOST);

#endif
#ifdef __APPLE__
#ifdef __HDShowPay
#else
							pView->PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_CANPAY, 0,XMS_TOPMOST);
#endif  // __HDShowPay
//#ifdef WEBVIEWPAYMODE
                            pView->PostMsg(XWM_APPLE_MESSAGE, XCMD_NOTICE_PAYURL, (XU32)pFilter->strPayURL.GetData(),XMS_TOPMOST);
//#endif
#endif
						}break;
					case FLT_PARAM:
						if(!strWord.IsEmpty())
						{
							char*s=(char*)strWord.GetData();
							char*sp=strchr(s,'\'');
							if(sp)
							{
								pFilter->strParams.Add(XString8(s,sp-s));
								pFilter->strParams.Add(XString8(sp+1,0));
							}
							else
							{
								pFilter->strParams.Add(XString8(s));
								pFilter->strParams.Add("");
							}
							//pFilter->strParams.Add(strWord);
						}
						//strParam.Empty();
						strWord.Empty();
						break;
//#if (__CORE_VERSION__>=0x02075000)
					case FLT_BLOCKDIV:
						if(!strWord.IsEmpty())
						{
							XString8Array list;
							list.SplitString(strWord,',');
							strWord.Empty();
							if(list.GetSize()>=2)
							{
								pFilter->m_nBlockWidth=list[0].ToInt();
								pFilter->m_nBlockHeight=list[1].ToInt();
							}
						}
						break;

//#endif
					case FLT_BEGIN://Begin
						if(!strWord.IsEmpty())
						{
							pFilter->bBegin=strWord.ToLogic();
							strWord.Empty();
						}
						break;
					case FLT_END:
						if(!strWord.IsEmpty())
						{
							pFilter->nEnd=strWord.ToInt();
							strWord.Empty();
						}break;
					case FLT_AUTOGO://AutoGo
						pFilter->strAutoGo=strWord;//strWord.ToInt();//strAutoGo.ToLogic();
						//strAutoGo.Empty();
						strWord.Empty();
						break;
					case FLT_AUTOFRAME://AutoFrame
						pFilter->nAutoFrame=strWord.ToInt();//strAutoFrame.ToLogic();
						//strAutoFrame.Empty();
						strWord.Empty();
						break;
					case FLT_AUTOLOCATION://auto Location
						{
							XString8Array list;//::SplitString(
							list.SplitString(strWord,'\'');
							if(list.GetSize())
							{
								pFilter->strAutoLocation=list[0];
								if(list.GetSize()>=2)
								{
									pFilter->nMethod=XEM::Index(list[1]);
									//char*str=list[1];
									//if(str[0]=='p'||str[1]=='P')
									//	pFilter->nMethod=XEM::POST;
								}
								if(list.GetSize()>=3)
									pFilter->strAutoPost=list[2];
							}
							strWord.Empty();
							//strAutoLocation.Empty();
						}break;

					}
				}
			}
			strWord.Empty();
			nSet=0;
			break;
		}
		strTxt++;
	}
}

XClientApply::XClientApply()
{
	//m_info.m_bModifyToken=XFALSE;
	m_pInstance=this;
	m_nXIDTime=0;
	m_info.Reset();
	m_pClients=XNULL;
	m_pFreeClients=XNULL;
	m_pFilters=XNULL;
	m_nDeltaX=0;
	m_nDeltaY=0;
	m_strDomainList.Add("jqbar.com");
	m_strDomainList.Add("playbyone.com");
#ifdef _SYMBIAN
	m_pThread=XNULL;
#endif
}
