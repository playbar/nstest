//
//  BarSchedular.h
//  BaiWan
//
//  Created by yock on 12-6-13.
//  Copyright (c) 2012年 JQBar. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MemoryWarnObserver.h"
#import "HDToolBarView.h"
#import "HDWidgetView.h"
#import "HDToolBarItem.h"

enum EBarType
{
    EAnother,
    EHtmlBar,       //dom bar
    EVideo,         //flash video bar
    EGame,          //flash game bar
    EWebGame,       //flash net game bar
    EWgt,           //flash wgt bar
    EWgtI           //flash wgt webgeme view bar
};

@interface BarSchedular : NSObject < MemoryWarnObserver>
{
    enum EBarType    m_eBarType;
    bool             m_bIsVisible;
    bool             m_bHaveMsg;
    HDToolBarView    *m_plHtmlBar;      //网页工具栏句柄
    HDToolBarView    *m_plVideoBar;     //视频工具栏句柄
    HDToolBarView    *m_plGameBar;      //单机游戏工具栏句柄 
    HDToolBarView    *m_plWebgameBar;   //网游工具栏句柄
    HDWidgetView     *m_plWgt;          //widget工具栏句柄
    NSTimer          *m_hTimer;         //计时器 
    int              m_nPosCount;       //处理循环次数
    HDToolBarItem    *m_plHtmlRefresh;  //网页刷新按钮句柄
    UIButton         *m_plWebGameRefresh;   //网页游戏刷新按钮
    HDWidget         *m_plWgtRefresh;   //widget中的刷新按钮
}

@property HDToolBarView *plHtmlBar;
@property HDToolBarView *plVideoBar;
@property HDToolBarView *plGameBar;
@property HDToolBarView *plWebgameBar;
@property HDWidgetView  *plWgt;

@property HDToolBarItem *plHtmlRefresh;
@property UIButton      *plWebGameRefresh;
@property HDWidget      *plWgtRefresh;

-(void) changeTo:(enum EBarType)bartype;

-(bool) onMemoryWarn:(int)bytecount;
-(void) onWarnBeProcess;

- (void)timerFireMethod:(NSTimer*)theTimer;

@end
