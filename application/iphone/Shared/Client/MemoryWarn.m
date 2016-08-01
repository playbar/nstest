//
//  MemoryWarn.m
//  BaiWan
//
//  Created by yock on 12-6-12.
//  Copyright (c) 2012年 JQBar. All rights reserved.
//

#import "MemoryWarn.h"
#include <sys/param.h>      
#include <sys/mount.h> 

static MemoryWarn *_memSinguler = NULL;

@interface MemoryWarn (PrivateFun)
-(int) getMemoryFree;
@end

@implementation MemoryWarn

//@synthesize m_timer = _m_ptimer;

//得到单例
+(MemoryWarn*) Singuler
{
    if(_memSinguler == NULL)
    {
        _memSinguler = [MemoryWarn alloc];
        [_memSinguler init];
    }
    return _memSinguler;
}

//清除单利
+(void) Release
{
    if(_memSinguler != NULL)
        [_memSinguler release];
    _memSinguler = NULL;
}

-(id)init
{
    self = [super init];
    if(self)
    {
        _m_nCount = 0;
        _m_bIsProcess = true;
//        _m_ptimer = [NSTimer scheduledTimerWithTimeInterval:20000 
//                                                     target:self 
//                                                   selector:@selector(timerFireMethod:) 
//                                                   userInfo:nil 
//                                                    repeats:NO];
    }
}

-(void)dealloc
{
    [self End];
}

//设置接受对象
-(void) setObserver:(id<MemoryWarnObserver>)obser
{
    observer[_m_nCount] = obser;
    _m_nCount++;
}
//消息以处理
-(void) WarnBeProcess
{
//    [self End];
    _m_bIsProcess = true;
    for(int i=0 ; i<_m_nCount ; i++)
        [observer[i] onWarnBeProcess];
}

//启动
-(void) Start
{
//    if(_m_ptimer == NULL) return;
    _m_bIsProcess = false;
    int free = [self getMemoryFree];
    for(int i=0 ; i<_m_nCount ; i++)
    {
        if([observer[i] onMemoryWarn:free])
        {
            _m_bIsProcess = true;
            break;
        }
    }
    if(_m_bIsProcess)
    {
        for(int i=0 ; i<_m_nCount ; i++)
            [observer[i] onWarnBeProcess];
    }
}
//停止
-(void) End
{
//    if(_m_ptimer != NULL && [_m_ptimer isValid])
//    {
//        [_m_ptimer invalidate];
//    }
//    _m_ptimer = nil;
}

//timer 回调
//- (void)timerFireMethod:(NSTimer*)theTimer
//{
//    if(!_m_bIsProcess)
//    {
//        int free = [self getMemoryFree];
//        for(int i=0 ; i<_m_nCount ; i++)
//        {
//            if([observer[i] onMemoryWarn:free])
//            {
//                _m_bIsProcess = true;
//                break;
//            }
//        }
//        if(_m_bIsProcess)
//        {
//            for(int i=0 ; i<_m_nCount ; i++)
//                [observer[i] onWarnBeProcess];
//        }
//        else {
//            [_m_ptimer fire];
//        }
//    }
//}

-(int) getMemoryFree
{
    struct statfs buf;
    int free = 0;
    if(statfs("/", &buf)>0)
    {
        free = buf.f_bsize * buf.f_bfree / 1024;
    }
    return free;
}

@end
