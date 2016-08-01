//
//  MemoryWarn.h
//  BaiWan
//
//  Created by yock on 12-6-12.
//  Copyright (c) 2012年 JQBar. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MemoryWarnObserver.h"

@interface MemoryWarn : NSObject
{
    //NSTimer *_m_ptimer;
    id<MemoryWarnObserver>  observer[64];
    int     _m_nCount;
    bool    _m_bIsProcess;
}

//@property NSTimer *m_timer;

//得到单例
+(MemoryWarn*) Singuler;
//清除单利
+(void) Release;
//设置接受对象
-(void) setObserver:(id<MemoryWarnObserver>)obser;
//消息以处理
-(void) WarnBeProcess;

//启动
-(void) Start;
//停止
-(void) End;

//timer 回调
//- (void)timerFireMethod:(NSTimer*)theTimer;



@end
