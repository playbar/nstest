//
//  BarSchedular.m
//  BaiWan
//
//  Created by yock on 12-6-13.
//  Copyright (c) 2012年 JQBar. All rights reserved.
//

#import "BarSchedular.h"

#define Interval 0.5

@interface BarSchedular (PrivateFun)
-(void)BarReply;
@end


@implementation BarSchedular

@synthesize plHtmlBar = m_plHtmlBar;
@synthesize plVideoBar = m_plVideoBar;
@synthesize plGameBar = m_plGameBar;
@synthesize plWebgameBar = m_plWebgameBar;
@synthesize plWgt = m_plWgt;

@synthesize plHtmlRefresh = m_plHtmlRefresh;
@synthesize plWebGameRefresh = m_plWebGameRefresh;
@synthesize plWgtRefresh = m_plWgtRefresh;

-(id)init
{
    self = [super init];
    if(self)
    {
        m_bHaveMsg = false;
        m_bIsVisible = false;
        m_plHtmlBar = nil;
        m_plVideoBar = nil;
        m_plGameBar = nil;
        m_plWebgameBar = nil;
        m_plWgt = nil;
        m_eBarType = EAnother;
//        m_hTimer = [NSTimer scheduledTimerWithTimeInterval:5 
//                                                    target:self 
//                                                  selector:@selector(timerFireMethod:) 
//                                                  userInfo:nil 
//                                                   repeats:NO];
        m_hTimer = nil;
        m_nPosCount = 0;
        
        m_plHtmlRefresh = nil;
        m_plWebGameRefresh = nil;
        m_plWgtRefresh = nil;
    }
    return self;
}

-(void)dealloc
{
    if(m_hTimer != nil)
    {
        [m_hTimer invalidate];
        m_hTimer=nil;
    }
    [super dealloc];
}

-(void) changeTo:(enum EBarType)bartype
{
    m_eBarType = bartype;
    if(m_bHaveMsg)
    {
        m_nPosCount = 0;
        if (m_hTimer!=nil) {
            [m_hTimer fire];
        }
        else 
        {
            m_hTimer = [NSTimer scheduledTimerWithTimeInterval:Interval 
                                                        target:self 
                                                      selector:@selector(timerFireMethod:) 
                                                      userInfo:nil 
                                                       repeats:YES];

        }
    }
}

#define _BilinCount 10

-(void)BarReply
{
    m_nPosCount++;
    switch (m_eBarType) {
        case EHtmlBar:
            m_plHtmlRefresh.alpha = (m_nPosCount%2 ? 0.5 : 1.0);
            [m_plHtmlRefresh setNeedsDisplay];
            break;
        case EWebGame:
            m_plWebGameRefresh.alpha = (m_nPosCount%2 ? 0.5 : 1.0);
            [m_plWebGameRefresh setNeedsDisplay];
            break;
        case EWgt:
            m_plWgt.alpha = (m_nPosCount%2 ? 0.5 : 1.0);
            [m_plWgt setNeedsDisplay];
            break;
        case EWgtI:
            m_plWgtRefresh.alpha = (m_nPosCount%2 ? 0.5 : 1.0);
            [m_plWgtRefresh setNeedsDisplay];
            break;
            
        default:
            break;
    }
    if (m_nPosCount>_BilinCount) 
    {
        m_plHtmlRefresh.alpha = 1.0;
        m_plWebGameRefresh.alpha = 1.0;
        m_plWgt.alpha = 1.0;
        m_plWgtRefresh.alpha = 1.0;
        [m_hTimer invalidate];
        m_hTimer = nil;
    }
}

-(bool) onMemoryWarn:(int)bytecount
{
    if(m_bHaveMsg) return false;
    m_bHaveMsg = true;
    if(m_eBarType == EAnother) return false;
    m_nPosCount = 0;
    m_hTimer = [NSTimer scheduledTimerWithTimeInterval:Interval 
                                                target:self 
                                              selector:@selector(timerFireMethod:) 
                                              userInfo:nil 
                                               repeats:YES];
    return false;
}
-(void) onWarnBeProcess
{
    m_bHaveMsg = false;
    m_plHtmlRefresh.alpha = 1.0;
    m_plWebGameRefresh.alpha = 1.0;
    m_plWgt.alpha = 1.0;
    m_plWgtRefresh.alpha = 1.0;
}

- (void)timerFireMethod:(NSTimer*)theTimer
{
    if(!m_bHaveMsg)
        return;
    [self BarReply];
}

@end
