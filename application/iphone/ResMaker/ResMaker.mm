//
//  ResMaker.m
//  ResMaker
//
//  Created by JQBar on 11-10-8.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "ResMaker.h"


@implementation ResMaker
@synthesize startButton;
@synthesize srsPathField;
@synthesize dstPathField;
@synthesize msgBoxField;
@synthesize progressIndicator;

- (id)init
{
    self = [super init];
    if (self) {
        pool = [[NSAutoreleasePool alloc] init];
		workThread = [[NSThread alloc] initWithTarget:self selector:@selector(start) object:nil];
		NSDictionary* dict = [[NSDictionary alloc] initWithContentsOfFile:@"default.plist"];
		[srsPathField setStringValue:[dict objectForKey:@"srsPath"]];
		[dstPathField setStringValue:[dict objectForKey:@"dstPath"]];
		[dict release];
    }
    return self;
}

- (void)start
{
	isRunning = YES;
	[progressIndicator setDoubleValue:0.0f];
	[progressIndicator setHidden:NO];
	[startButton setTitle:@"Stop"];
	[msgBoxField setStringValue:@""];
	
	// 打开并读取srs文件
	NSString* srsFilePath = [srsPathField stringValue];
	NSStringEncoding enc1 = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
	NSString* srsTemp = [NSString stringWithContentsOfFile:srsFilePath encoding:enc1 error:nil];
	
	NSFileHandle* srsFileHandle = [NSFileHandle fileHandleForReadingAtPath:srsFilePath];
	
	if(!srsFileHandle)
	{
		[self trace:[NSString stringWithFormat:@"错误:无法打开srs文件(%@)", srsFilePath]];
		[self stop];
		return;
	}
	
	NSData* srsContentData = [srsFileHandle readDataToEndOfFile];
	const void* srsContent = [srsContentData bytes];
	if(!srsContent)
	{
		[self trace:[NSString stringWithFormat:@"错误:srs文件内容为空!"]];
		[self stop];
		[srsFileHandle closeFile];
		return;
	}
	[srsFileHandle closeFile];
	// 读取srs文件完毕
	
	[self setPercent:5];
	
	NSMutableString* strContent = [[NSMutableString alloc] init];
	int srsLine = 0;
	NSMutableArray* srsResList = [[NSMutableArray alloc] init];
	NSMutableArray* srsCmdList = [[NSMutableArray alloc] init];
	BOOL bEcho=NO;
    NSMutableArray* srsCmdLineList = [[NSMutableArray alloc] init];
    NSMutableArray* srsCmdNamelist = [[NSMutableArray alloc] init];
	
	const char* srsContentTmp = [srsTemp cStringUsingEncoding:enc1];
	while (1)
	{
		char c = *srsContentTmp;
		srsContentTmp++;
		switch(c)
		{
			case ';':
			{
				bEcho = YES;
                if([strContent length]!=0)
				{
					NSString* strContentCopy = [[NSString alloc] initWithString:strContent];
					[srsCmdLineList addObject:strContentCopy];
					[strContentCopy release];
				}
				[strContent setString:@""];
			}
				break;
			case '\r':
                break;
            case 0:
            case '\n':
			{
				if([strContent length]!=0)
				{
					NSString* strContentCopy = [[NSString alloc] initWithString:strContent];
                    [srsCmdLineList addObject:strContentCopy];
					[strContentCopy release];
				}
                [strContent setString:@""];
                bEcho=NO;
                switch([srsCmdLineList count])
                {
                    case 0:
                        break;
                    case 1:
                    {
						[self trace:[NSString stringWithFormat:@"错误(行:%d):不合法的命令格式",srsLine]];
                    }
                        break;
                    case 2:
                    {
						NSString* strCmd = [srsCmdLineList objectAtIndex:0];
						strCmd = [strCmd uppercaseString];
						if([strCmd isEqualToString:@"STRING"])
						{
							[self trace:[NSString stringWithFormat:@"错误(行:%d):需要3个参数(实际%d个)",srsLine,[srsCmdLineList count]]];
							break;
						}
						int id = [srsCmdNamelist indexOfObject:[srsCmdLineList objectAtIndex:0]];
                        if(id>0)
                        {
							[self trace:[NSString stringWithFormat:@"错误(行:%d):重复的参数(%@)",srsLine,[srsCmdLineList objectAtIndex:0]]];
							break;
                        }
                        else
						{
							NSString* srsCmdNameCopy = [[NSString alloc] initWithString:[srsCmdLineList objectAtIndex:0]];
							[srsCmdNamelist addObject:srsCmdNameCopy];
							[srsCmdNameCopy release];
						}
						NSString* srsResNameCopy = [[NSString alloc] initWithString:[srsCmdLineList objectAtIndex:0]];
						NSString* srsResPathCopy = [[NSString alloc] initWithString:[srsCmdLineList objectAtIndex:1]];
						[srsResList addObject:srsResNameCopy];
						[srsResList addObject:srsResPathCopy];
						[srsResNameCopy release];
						[srsResPathCopy release];
                    }
                        break;
                    default:
                    {
						NSString* strCmd = [srsCmdLineList objectAtIndex:0];
						strCmd = [strCmd uppercaseString];
						if([strCmd isEqualToString:@"STRING"])
						{
							if([srsCmdLineList count]>=4)
								[self trace:[NSString stringWithFormat:@"警告(行%d):超过3个的参数(实际%d个)被忽略",srsLine,[srsCmdLineList count]]];
							
							NSString* srsCmdSTRINGCopy = [[NSString alloc] initWithString:[srsCmdLineList objectAtIndex:0]];
							NSString* srsCmdNameCopy = [[NSString alloc] initWithString:[srsCmdLineList objectAtIndex:1]];
							NSString* srsCmdValueCopy = [[NSString alloc] initWithString:[srsCmdLineList objectAtIndex:2]];
							[srsCmdList addObject:srsCmdSTRINGCopy];
							[srsCmdList addObject:srsCmdNameCopy];
							[srsCmdList addObject:srsCmdValueCopy];
							[srsCmdSTRINGCopy release];
							[srsCmdNameCopy release];
							[srsCmdValueCopy release];
						}
						else 
							[self trace:[NSString stringWithFormat:@"警告(行%d):超过2个的参数(实际%d个)被忽略",srsLine,[srsCmdLineList count]]];
                    }
                        break;
                }
				
                [srsCmdLineList removeAllObjects];
                srsLine++;
			}
				break;
			case '\\':
			{
				if(!bEcho)
                {
                    c=*srsContentTmp;
					[strContent appendString:[NSString stringWithFormat:@"%c",c]];
                    srsContentTmp++;
                }
			}
				break;
			default:
			{
				if(c<0||c>32)
                {
                    if(!bEcho)
                        [strContent appendString:[NSString stringWithFormat:@"%c",c]];
                }
                else
                {
					if([strContent length]!=0)
					{
						NSString* strContentCopy = [[NSString alloc] initWithString:strContent];
						[srsCmdLineList addObject:strContentCopy];
						[strContentCopy release];
					}
					[strContent setString:@""];
                }
			}
				break;
		}
		
		if(!c) break;
	}
	// srs处理完毕
	
	if([srsResList count]<2)
		[self trace:@"警告:没用可以处理的资源"];
	
	NSMutableString* srsPath = [[NSMutableString alloc] init];
	NSMutableString* strTmp = [[NSMutableString alloc] init];	// 临时字符串
	NSMutableString* srsFileName = [[NSMutableString alloc] init];	// srs文件名
	NSMutableString* rhhPath = [[NSMutableString alloc] init];	// rhh文件路径
	NSMutableString* rppPath = [[NSMutableString alloc] init];	// rpp文件路径
	
	NSString* dstFilePath = [dstPathField stringValue];
	NSRange range = [srsFilePath rangeOfCharacterFromSet:[NSCharacterSet characterSetWithCharactersInString:@"/"] options:NSBackwardsSearch];
	if (range.location!=NSNotFound)
	{
		[srsFileName setString:[srsFilePath substringFromIndex:range.location+1]];
		[srsPath setString:[srsFilePath substringToIndex:range.location]];
	}
	else 
		[srsFileName setString:srsFilePath];
	
	NSRange fileRange = [srsFileName rangeOfCharacterFromSet:[NSCharacterSet characterSetWithCharactersInString:@"."] options:NSBackwardsSearch];
	if(fileRange.location!=NSNotFound)
		[srsFileName setString:[srsFileName substringToIndex:fileRange.location]];
	
	// 给定rhh和rpp的文件路径
	[rhhPath setString:dstFilePath];
	[rhhPath appendString:@"/"];
	[rhhPath appendString:srsFileName];
	[rhhPath appendString:@".rhh"];
	[rppPath setString:dstFilePath];
	[rppPath appendString:@"/"];
	[rppPath appendString:srsFileName];
	[rppPath appendString:@".rpp"];
	
	// #include "xxx.rhh"
	[strContent setString:[NSString stringWithFormat:@"#include \"%@.rhh\"\r\n",srsFileName]];
	// #define _RESCOUNT xx
	int nCount = [srsResList count];
	[strContent appendString:[NSString stringWithFormat:@"#define _RESCOUNT %d\r\n",nCount/2]];
	
	for(int i=0;i<nCount;i+=2)
    {
		[strTmp setString:srsPath];
		[strTmp appendString:@"/"];
		[strTmp appendString:[srsResList objectAtIndex:i+1]];
        //[self trace:[NSString stringWithFormat:@"处理文件:%@",[srsResList objectAtIndex:i+1]]];
		
		NSFileHandle* resFileHandle = [NSFileHandle fileHandleForReadingAtPath:strTmp];
		if(!resFileHandle)
			[self trace:[NSString stringWithFormat:@"错误:不能打开文件%@",[srsResList objectAtIndex:i+1]]];
		else 
		{
			NSData* resData = [resFileHandle readDataToEndOfFile];
			if(![resData bytes])
				[self trace:[NSString stringWithFormat:@"错误:文件(%@)内容为空",[srsResList objectAtIndex:i+1]]];
			else 
			{
				// xxx xxx.xxx
				[strTmp setString:[NSString stringWithFormat:@"//%@\t%@\r\n",[srsResList objectAtIndex:i],[srsResList objectAtIndex:i+1]]];
				[strContent appendString:strTmp];
				
				// static XU8 _resData%s[]={xxx}
				[strTmp setString:[NSString stringWithFormat:@"static XU8 _resData%@[]={", [srsResList objectAtIndex:i]]];
				[strContent appendString:strTmp];
				
				int iPos = 0;		// 当前读取的位置
				int iCount = 0;		// 当前行存放的数据个数
				int fileSize = [resData length];
				const unsigned char* resDataTmp = (const unsigned char*)[resData bytes];
				for(iPos=0;iPos<fileSize;iPos++)
                {
					[strTmp setString:[NSString stringWithFormat:@"0x%x",*resDataTmp++]];
					[strContent appendString:strTmp];
					if(iPos+1<fileSize)
					{
						[strContent appendString:@","];
						if(iCount>=20)
						{
							[strContent appendString:@"\r\n\t"];
							iCount = 0;
						}
						iCount++;
					}
                }
				[strContent appendString:@"};\r\n"];				
			}
			
			[resFileHandle closeFile];
		}
		
		[self setPercent:10+i*90/nCount];
    }
	
	[srsCmdNamelist sortUsingSelector:@selector(caseInsensitiveCompare:)];
	
	nCount = [srsCmdNamelist count];
	if(nCount)
	{
		// Resource MAP list
		[strContent appendString:[NSString stringWithFormat:@"\r\n//Resource MAP list\r\nstruct _RSSMapindex{\r\n\tXPCTSTR strName;\r\n\tXU32 nLength;\r\n\tXU8*pData;};\r\n"]];
		// static _RSSMapindex _rssMapIndex[]={
		[strContent appendString:[NSString stringWithFormat:@"static _RSSMapindex _rssMapIndex[]={\r\n"]];
		
		for(int i=0;i<nCount;i++)
		{
			NSString* cmdName = [srsCmdNamelist objectAtIndex:i];
			[strTmp setString:[NSString stringWithFormat:@"{\"%@\",sizeof(_resData%@),_resData%@},\r\n",cmdName,cmdName, cmdName]];
			[strContent appendString:strTmp];
		}
		[strContent appendString:@"};\r\n"];
	}
	
	nCount = [srsCmdList count];
	NSMutableString* rhhContent = [[NSMutableString alloc] initWithString:@"#if! defined(__XRES_HEADER_)\r\n#define __XRES_HEADER_\r\n#define XIDS_EMPTY 0\r\n"];
	NSMutableString* strDataCnt = [[NSMutableString alloc] initWithString:@"static XPCWSTR _strResString[]={0,\r\n"];
	[strTmp setString:[NSString stringWithFormat:@"#define XIDS_MAX %d\r\n", nCount/3+1]];
	[rhhContent appendString:strTmp];
	
	if (nCount)
	{
		for (int i=0; i<nCount; i+=3)
		{
			NSString* cmdValue = [srsCmdList objectAtIndex:i+2];
			NSString* cmdValueUTF8 = [[NSString alloc] initWithData:[cmdValue dataUsingEncoding:NSUTF32StringEncoding] encoding:NSUTF32StringEncoding];
			NSStringEncoding enc = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
			const char* cmdValueGBP8 = [cmdValue cStringUsingEncoding:enc];
			NSString* cmdValueGB = [NSString stringWithCString:cmdValueGBP8 encoding:enc];
			const char* cmdValueUnicodeP8 = [cmdValueGB cStringUsingEncoding:NSUnicodeStringEncoding];
			[strTmp setString:[NSString stringWithFormat:@"#define %@ %d\r\n", [srsCmdList objectAtIndex:i+1], i/3+1]];
			[rhhContent appendString:strTmp];
			[strTmp setString:[NSString stringWithFormat:@"\t(XPCWSTR)_str%@,\r\n", [srsCmdList objectAtIndex:i+1]]];
			[strDataCnt appendString:strTmp];
			[strTmp setString:[NSString stringWithFormat:@"static XU16 _str%@[]={", [srsCmdList objectAtIndex:i+1]]];
			[strContent appendString:strTmp];
			
			const unsigned short* cmdValueP16 = (const unsigned short*)cmdValueUnicodeP8;
			//const unsigned int* cmdValueP32 = (const unsigned int*)cmdValueP8;
			while (1)
			{
				[strTmp setString:[NSString stringWithFormat:@"(XU16)0x%x", *cmdValueP16]];
				[strContent appendString:strTmp];
				if (!*cmdValueP16)
					break;
				[strContent appendString:@","];
				cmdValueP16++;
			}
			
			[strContent appendString:@"};\r\n"];
		}
	}
	
	[strDataCnt appendString:@"};\r\n"];
	[strContent appendString:strDataCnt];
	[rhhContent appendString:@"#endif"];
	
	[rhhContent writeToFile:rhhPath atomically:NO encoding:NSUTF8StringEncoding error:nil];
	[strContent writeToFile:rppPath atomically:NO encoding:NSUTF8StringEncoding error:nil];
	
//	NSFileHandle* rhhFileHandle = [NSFileHandle fileHandleForWritingAtPath:rhhPath];
//	if(!rhhFileHandle)
//	{
//		[self trace:[NSString stringWithFormat:@"错误:不能打开文件-%@",rhhPath]];
//	}
//	else 
//	{
//		[rhhFileHandle writeData:[rhhContent dataUsingEncoding:NSUTF8StringEncoding]];
//		[rhhFileHandle closeFile];
//	}
//	
//	NSFileHandle* rppFileHandle = [NSFileHandle fileHandleForWritingAtPath:rppPath];
//	if(!rppFileHandle)
//	{
//		[self trace:[NSString stringWithFormat:@"错误:不能打开文件-%@", rppPath]];
//	}
//	else 
//	{
//		[rppFileHandle writeData:[strContent dataUsingEncoding:NSUTF8StringEncoding]];
//		[rppFileHandle closeFile];
//	}
	
	[self stop];
	
	/*
	NSMutableString* strContent = [[NSMutableString alloc] init];
	int srsLine = 0;
	NSMutableArray* srsResList = [[NSMutableArray alloc] init];
	NSMutableArray* srsCmdList = [[NSMutableArray alloc] init];
	BOOL bEcho=NO;
    NSMutableArray* srsCmdLineList = [[NSMutableArray alloc] init];
    NSMutableArray* srsCmdNamelist = [[NSMutableArray alloc] init];
	
	
	[self setPercent:5];
	const char* srsContentTmp = (const char*)srsContent;
	while(1)
	{
		char c = *srsContentTmp;
		srsContentTmp++;
		switch(c)
		{
			case ';':
			{
				bEcho = YES;
                if([strContent length]!=0)
					[srsCmdLineList addObject:strContent];
				[strContent setString:@""];
			}
				break;
			case '\r':
                break;
            case 0:
            case '\n':
			{
				if([strContent length]!=0)
                    [srsCmdLineList addObject:strContent];
                [strContent setString:@""];
                bEcho=NO;
                switch([srsCmdLineList count])
                {
                    case 0:
                        break;
                    case 1:
                    {
						[self trace:[NSString stringWithFormat:@"错误（行：%d）：不合法的命令格式",srsLine]];
                    }
                        break;
                    case 2:
                    {
						NSString* strCmd = [srsCmdLineList objectAtIndex:0];
						strCmd = [strCmd uppercaseString];
						if([strCmd isEqualToString:@"STRING"])
						{
							[self trace:[NSString stringWithFormat:@"错误（行：%d）：需要3个参数（实际%d）个",srsLine,[srsCmdLineList count]]];
							break;
						}
						int id = [srsCmdLineList indexOfObject:[srsCmdLineList objectAtIndex:0]];
                        if(id>0)
                        {
							[self trace:[NSString stringWithFormat:@"错误（行：%d）：重复的参数（%@）",srsLine,[srsCmdLineList objectAtIndex:0]]];
							break;
                        }
                        else
							[srsCmdNamelist addObject:[srsCmdLineList objectAtIndex:0]];
						
						[srsResList addObject:[srsCmdLineList objectAtIndex:0]];
						[srsResList addObject:[srsCmdLineList objectAtIndex:1]];
                    }
                        break;
                    default:
                    {
						NSString* strCmd = [srsCmdLineList objectAtIndex:0];
						strCmd = [strCmd uppercaseString];
						if([strCmd isEqualToString:@"STRING"])
						{
							if([srsCmdLineList count]>=4)
								[self trace:[NSString stringWithFormat:@"警告（行%d）：超过3个参数（实际%d个）被忽略",srsLine,[srsCmdLineList count]]];
							[srsCmdList addObject:[srsCmdLineList objectAtIndex:0]];
							[srsCmdList addObject:[srsCmdLineList objectAtIndex:1]];
							[srsCmdList addObject:[srsCmdLineList objectAtIndex:2]];
						}
						else 
							[self trace:[NSString stringWithFormat:@"警告（行%d）：超过2个参数（实际%d个）被忽略",srsLine,[srsCmdLineList count]]];
                    }
                        break;
                }
				
                [srsCmdLineList removeAllObjects];
                srsLine++;
			}
				break;
			case '\\':
			{
				if(!bEcho)
                {
                    c=*srsContentTmp;
					[strContent appendString:[NSString stringWithFormat:@"%c",c]];
                    srsContentTmp++;
                }
			}
				break;
			default:
			{
				if(c<0||c>32)
                {
                    if(!bEcho)
                        [strContent appendString:[NSString stringWithFormat:@"%c",c]];
                }
                else
                {
					if([strContent length]!=0)
						[srsCmdLineList addObject:strContent];
					[strContent setString:@""];
                }
			}
				break;
		}
		
		if(!c) break;
	}
	// srs处理完毕
	
	if([srsResList count]<2)
		[self trace:@"警告：没用可以处理的资源"];
	
	NSMutableString* srsPath = [[NSMutableString alloc] init];
	NSMutableString* strTmp = [[NSMutableString alloc] init];	// 临时字符串
	NSMutableString* srsFileName = [[NSMutableString alloc] init];	// srs文件名
	NSMutableString* rhhPath = [[NSMutableString alloc] init];	// rhh文件路径
	NSMutableString* rppPath = [[NSMutableString alloc] init];	// rpp文件路径
	
	
	 int id=m_srsPath.lastIndexOf('/');
	 if(id>0)
	 {
	 srsPath=m_srsPath.left(id);
	 srsFileName=m_srsPath.mid(id+1);
	 }
	 else
	 srsFileName=m_srsPath;
	 
	 id=srsFileName.indexOf('.');
	 if(id>0)
	 srsFileName=srsFileName.left(id);
	 
	 // 给定rhh和rpp的文件路径
	 rhhPath=m_dstPath;
	 rppPath=m_dstPath;
	 rhhPath+="/";
	 rhhPath+=srsFileName;
	 rhhPath+=".rhh";
	 rppPath+="/";
	 rppPath+=srsFileName;
	 rppPath+=".rpp";
	
	 
	
	// #include "xxx.rhh"
	[strContent setString:[NSString stringWithFormat:@"#include \"%@.rhh\"",srsFileName]];
	// #define _RESCOUNT xx
	int nCount = [srsResList count];
	[strContent appendString:[NSString stringWithFormat:@"#define _RESCOUNT %d",nCount/2]];
	
	for(int i=0;i<nCount;i+=2)
    {
		[strTmp setString:srsPath];
		[strTmp appendString:@"/"];
		[strTmp appendString:[srsResList objectAtIndex:i+1]];
        [self trace:[NSString stringWithFormat:@"处理文件：%@",[srsResList objectAtIndex:i+1]]];
		
		NSFileHandle* resFileHandle = [NSFileHandle fileHandleForReadingAtPath:strTmp];
		if(!resFileHandle)
			[self trace:[NSString stringWithFormat:@"错误：不能打开文件%@",[srsResList objectAtIndex:i+1]]];
		else 
		{
			NSData* resData = [resFileHandle readDataToEndOfFile];
			if(![resData bytes])
				[self trace:[NSString stringWithFormat:@"错误：文件（%@）内容为空",[srsResList objectAtIndex:i+1]]];
			else 
			{
				// xxx xxx.xxx
				[strTmp setString:[NSString stringWithFormat:@"//%@\t%@",[srsResList objectAtIndex:i],[srsResList objectAtIndex:i+1]]];
				[strContent appendString:strTmp];
				
				// static XU8 _resData%s[]={xxx}
				[strTmp setString:[NSString stringWithFormat:@"static XU8 _resData%@[]={", [srsResList objectAtIndex:i]]];
				[strContent appendString:strTmp];
				
				int iPos = 0;		// 当前读取的位置
				int iCount = 0;		// 当前行存放的数据个数
				int fileSize = [resData length];
				const char* resDataTmp = (const char*)resData;
				for(iPos=0;iPos<fileSize;iPos++)
                {
					[strTmp setString:[NSString stringWithFormat:@"0x%x",*resDataTmp++]];
					[strContent appendString:strTmp];
					if(iPos+1<fileSize)
					{
						[strContent appendString:@","];
						if(iCount>=20)
						{
							[strContent appendString:@"\r\n\t"];
							iCount = 0;
						}
						iCount++;
					}
                }
				[strContent appendString:@"};\r\n"];				
			}
			
			[resFileHandle closeFile];
		}
		
		[self setPercent:10+i*90/nCount];
    }
	
	nCount = [srsCmdNamelist count];
	if(nCount)
	{
		// Resource MAP list
		[strContent appendString:[NSString stringWithFormat:@"\r\n//Resource MAP list\r\nstruct _RSSMapindex{\r\n\tXPCTSTR strName;\r\n\tXU32 nLength;\r\n\tXU8* pData;};\r\n"]];
		// static _RSSMapindex _rssMapIndex[]={
		[strContent appendString:[NSString stringWithFormat:@"static _RSSMapindex _rssMapIndex[]={\r\n"]];
		
		for(int i=0;i<nCount;i++)
		{
			NSString* cmdName = [srsCmdNamelist objectAtIndex:i];
			[strTmp setString:[NSString stringWithFormat:@"{\"%@\",sizeof(_resData%@),_resData%@},\r\n",cmdName,cmdName, cmdName]];
			[strContent appendString:strTmp];
		}
		[strContent appendString:@"};\r\n"];
	}
	
	nCount = [srsCmdList count];
	NSMutableString* rhhContent = [[NSMutableString alloc] initWithString:@"#if! defined(__XRES_HEADER_)\r\n#define __XRES_HEADER_\r\n#define XIDS_EMPTY 0\r\n"];
	NSMutableString* strDataCnt = [[NSMutableString alloc] initWithString:@"static XPCWSTR _strResString[]={0,\r\n"];
	[strTmp setString:[NSString stringWithFormat:@"#define XIDS_MAX %d\r\n", nCount/3+1]];
	[rhhContent appendString:strTmp];
	
	if(nCount)
	{
		for (int i=0; i<nCount;i+=3)
		{
			
//			 QString cmdName;
//			 QTextCodec* textCode=QTextCodec::codecForName("GB2312");
//			 cmdName=textCode->toUnicode(srsCmdList[i+2].toUtf8().constData());
//			 const char* cmdNameP8 = cmdName.toUtf8().constData();
//			 strTmp.sprintf("#define %s %d\r\n",srsCmdList[i+1].toUtf8().constData(),i/3+1);
//			 rhhContent+=strTmp;
//			 strTmp.sprintf("\r(XPCWSTR)_str%s,\r\n",srsCmdList[i+1].toUtf8().constData());
//			 strDataCnt+=strTmp;
//			 strTmp.sprintf("static XU16 _str%s[]={",srsCmdList[i+1].toUtf8().constData());
//			 strContent+=strTmp;
//			 const unsigned short* cmdNameP16 = (const unsigned short*)cmdNameP8;
//			 while(1)
//			 {
//			 strTmp.sprintf("(XU16)0x%x",*cmdNameP16);
//			 strContent+=strTmp;
//			 if(!*cmdNameP16)
//			 break;
//			 strContent+=',';
//			 cmdNameP16++;
//			 }
//			 strContent+="};\r\n";
			 
		}
	}
	
	[strDataCnt appendString:@"};\r\n"];
	[strContent appendString:strDataCnt];
	[rhhContent appendString:@"#endif"];
	
	NSFileHandle* rhhFileHandle = [NSFileHandle fileHandleForWritingAtPath:rhhPath];
	if(!rhhFileHandle)
	{
		[self trace:[NSString stringWithFormat:@"错误：不能打开文件-%@",rhhPath]];
	}
	else 
	{
		[rhhFileHandle writeData:[rhhContent dataUsingEncoding:NSUTF8StringEncoding]];
		[rhhFileHandle closeFile];
	}
	
	NSFileHandle* rppFileHandle = [NSFileHandle fileHandleForWritingAtPath:rppPath];
	if(!rppFileHandle)
	{
		[self trace:[NSString stringWithFormat:@"错误:不能打开文件-%@", rppPath]];
	}
	else 
	{
		[rppFileHandle writeData:[strContent dataUsingEncoding:NSUTF8StringEncoding]];
		[rppFileHandle closeFile];
	}
	
	[self stop];
	*/
}

- (void)stop
{
	isRunning = NO;
	[workThread cancel];
	
	[startButton setTitle:@"Start"];
	[progressIndicator setDoubleValue:0.0f];
	[progressIndicator setHidden:YES];
}

- (void)trace:(NSString*)msg
{
	NSMutableString* msgBoxString = [[NSMutableString alloc] initWithString:[msgBoxField stringValue]];
	[msgBoxString appendString:msg];
	[msgBoxString appendString:@"\r\n"];
	[msgBoxField setStringValue:msgBoxString];
	[msgBoxString release];
}

- (void)setPercent:(int)percent
{
	[progressIndicator setDoubleValue:percent];
}

- (IBAction)clickStartButton:(id)sender
{
	if(!isRunning)
		[self start];
	else 
		[self stop];
}

- (void)dealloc
{
	NSDictionary* dict = [[NSDictionary alloc] initWithObjectsAndKeys:[srsPathField stringValue], @"srsPath",
						  [dstPathField stringValue], @"dstPath", nil];
	[dict writeToFile:@"default.plist" atomically:NO];
	[dict release];
	[pool release];
	[super dealloc];
}

@end
