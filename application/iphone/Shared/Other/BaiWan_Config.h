//
//  config.h
//  BaiWan
//
//  Created by Work on 4/5/12.
//  Copyright (c) 2012 JQBar. All rights reserved.
//


#ifdef BWON
#undef BWON
#endif
#define BWON    1

#ifdef BWOFF
#undef BWOFF
#endif
#define BWOFF   0

// push模式开关
#ifdef BWPUSHMODE
#undef BWPUSHMODE
#endif
// 启动界面loading模式
#ifdef BWLOADINGMODE
#undef BWLOADINGMODE
#endif
// newapp模式开关
#ifdef BWNEWAPPMODE
#undef BWNEWAPPMODE
#endif
// showbar模式开关
#ifdef BWSHOWBARMODE
#undef BWSHOWBARMODE
#endif
// 内存查看模式开关
#ifdef BWMEMORYSHOWMODE
#undef BWMEMORYSHOWMODE
#endif
// onsale模式开关
#ifdef BWONSALEMODE
#undef BWONSALEMODE
#endif
// iap模式开关
#ifdef BWIAPMODE
#undef BWIAPMODE
#endif
// outlogin模式开关
#ifdef BWOUTLOGINMODE
#undef BWOUTLOGINMODE
#endif
// mobiSage模式开关
#ifdef BWMOBISAGE
#undef BWMOBISAGE
#endif

#ifdef BWSINGLE

// push模式开关
#define BWPUSHMODE      BWON
// 启动界面loading模式
#define BWLOADINGMODE   BWON
// newapp模式开关
#define BWNEWAPPMODE    BWON
// showbar模式开关
#define BWSHOWBARMODE   BWON
// 内存查看模式开关
#define BWMEMORYSHOWMODE    BWOFF
// onsale模式开关
#define BWONSALEMODE    BWON
// iap模式开关
#define BWIAPMODE       BWON
// outlogin模式开关 是否包含SDK开关
// mobiSageSDK
#define BWMOBISAGE      BWOFF

// 外部sdk
#if defined(BW91SDKMODE)  // 包含外部的SDK
#define BWTHIRDPARTSDKISON BWON
#define SDKNEEDUSERCENTER  BWON
#define BWLOADINGMODE      BWON

#elif defined(BWUCSDKMODE)
#define BWTHIRDPARTSDKISON BWON
#define SDKNEEDUSERCENTER  BWOFF
#define BWLOADINGMODE      BWON

#elif defined(BWPPSDKMODE)
#define BWTHIRDPARTSDKISON BWON
#define SDKNEEDUSERCENTER  BWON
#define BWLOADINGMODE      BWON

#elif defined(BWTBSDKMODE)
#define BWTHIRDPARTSDKISON BWON
#define SDKNEEDUSERCENTER  BWON
#define BWLOADINGMODE      BWON

#else              // 不包含外部的SDK
#define BWTHIRDPARTSDKISON BWOFF
#define SDKNEEDUSERCENTER  BWOFF
#define BWLOADINGMODE      BWON
#endif

#define BWPLAYNOTFLASHGAME  BWOFF

#elif defined (BWSINGLE_MOBISAGE)

// push模式开关
#define BWPUSHMODE      BWON
// 启动界面loading模式
#define BWLOADINGMODE   BWOFF
// newapp模式开关
#define BWNEWAPPMODE    BWON
// showbar模式开关
#define BWSHOWBARMODE   BWOFF
// 内存查看模式开关
#define BWMEMORYSHOWMODE    BWON
// onsale模式开关
#define BWONSALEMODE    BWOFF
// iap模式开关
#define BWIAPMODE       BWON
// outlogin模式开关
#define BWOUTLOGINMODE  BWOFF
// 外部sdk
// 91SDK
#define BW91SDKMODE     BWOFF
// mobiSageSDK
#define BWMOBISAGE      BWON
// UCSDK
#define BWUCSDKMODE     BWOFF
// PPSDK
#define BWPPSDKMODE BWON

#else//BWSINGLE

// 如果是单品，自己创建#ifdef，下面是非单品的例子
// push模式开关
#define BWPUSHMODE      BWON
// 启动界面loading模式
#define BWLOADINGMODE   BWOFF
// newapp模式开关
#define BWNEWAPPMODE    BWON
// showbar模式开关
#define BWSHOWBARMODE   BWOFF
// 内存查看模式开关
#define BWMEMORYSHOWMODE    BWON
// onsale模式开关
#define BWONSALEMODE    BWOFF
// iap模式开关
#define BWIAPMODE       BWON
// outlogin模式开关
#define BWOUTLOGINMODE  BWOFF
// 外部sdk
// 91SDK
#define BW91SDKMODE     BWOFF
// mobiSageSDK
#define BWMOBISAGE      BWOFF
// UCSDK
#define BWUCSDKMODE     BWOFF
// PPSDK
#define BWPPSDKMODE BWON

#endif//BWSINGLE

// iap相关定义
#if BWIAPMODE
// 存储时用到的KEY
#define BWIAPKEYSTEPID       @"stepid"
#define BWIAPKEYPRODUCTID    @"productid"
#define BWIAPKEYORDERID      @"orderid"
#define BWIAPKEYCODEID       @"codeid"
#define BWIAPKEYDESCRIPTION  @"description"
#define BWIAPKEYCHARGEDATA   @"chargeData"
#define BWIAPKEYCHARGEURLDATA    @"chargeUrlData"
// 反馈用到的error code
#define BWIAPERRORNONE 0
#define BWIAPERRORINVALID -4
#define BWIAPERRORUNSUPPORTED -5
#define BWIAPERRORCANCELLED -6
// 支付流程中的状态
#define BWIAPSTATUSREADY 0
#define BWIAPSTATUSCHECKSTART 1
#define BWIAPSTATUSCHECKFINISH 2
#define BWIAPSTATUSCHECKFAILED 3
#define BWIAPSTATUSCHARGESTART 4
#define BWIAPSTATUSCHARGEFINISH 5
#define BWIAPSTATUSCHARGEFAILED 6
#define BWIAPSTATUSFEEDBACKSTART 7
#define BWIAPSTATUSFEEDBACKFINISH 8
#endif


// 百玩userdefault key
#define BWUSERDEFAULTKEY    @"BaiWan"
// bar or widget key
#define BWMODEKEY           @"BaiWan_Mode"
// showbar key
#define BWSHOWBARKEY        @"BaiWan_ShowBarMode"
// onsale key
#define BWONSALEKEY         @"BaiWan_OnSaleMode"
// webgame key
#define BWWEBGAMEKEY         @"BaiWan_WebGameMode"
// deviceOrientation
#define BWDEVICEORIKEY         @"BaiWan_DeviceOriMode"
// outlogin key
#define BWOUTLOGINKEY       @"BaiWan_OutLogin"
// 鼠标指示key
#define BWPOINTKEY          @"BaiWan_Point"
// 虚拟按键key
#define BWBUTTONKEY         @"BaiWan_Button"



// iPad版本常量
// 默认延迟时间
#define BWDEFAULTTIMEINTERVAL   3.0

// 浏览器工具栏高度
#define BWWEBBARVIEWHEIGHT  44
// 浏览器工具栏选项宽度
//#define BWWEBBARITEMWIDTH   64
#define BWWEBBARITEMWIDTH   64
// 浏览器工具栏选项高度
#define BWWEBBARITEMHEIGHT  BWWEBBARVIEWHEIGHT
// 浏览器更多工具栏高度
#define BWWEBMOREBARVIEWHEIGHT  64
// 浏览器更多工具栏选项宽度
#define BWWEBMOREBARITEMWIDTH   64
// 浏览器更多工具栏选项高度
#define BWWEBMOREBARITEMHEIGHT  64
// 小widget的宽度
#define BWSMALLWIDGETWIDTH  108
// 小widget的高度
#define BWSMALLWIDGETHEIGHT 45
// 小widgetitem的宽度
#define BWSMALLWIDGETITEMWIDTH  45
// 小widgetitem的高度
#define BWSMALLWIDGETITEMHEIGHT 54
// 大widget的宽度
#define BWBIGWIDGETWIDTH    240
// 大widget的高度
#define BWBIGWIDGETHEIGHT   240
// 大widgetitem的宽度
#define BWBIGWIDGETITEMWIDTH    80
// 大widgetitem的高度
#define BWBIGWIDGETITEMHEIGHT   80

#ifdef _DEBUG
#define BWLOG(...)  NSLog(__VA_ARGS__)
#define BWWARNING(...)  NSLog(__VA_ARGS__)
#define BWERROR(...)    NSLog(__VA_ARGS__)
#define BWASSERT(p)
#else
#define BWLOG(...)
#define BWWARNING(...)
#define BWERROR(...)
#define BWASSERT(p)
#endif

#define BWRELEASE(p)    if(p){[p release]; p=nil;}
#define BWCANCEL(p)     if(p){[p cancel]; [p release]; p=nil;}
#define BWINVALIDATE(p) if(p){[p invalidate]; p=nil;}

#ifdef _DEBUG
#define __OFILENAME__ (strrchr(__FILE__,'/')+1)
#define OLOGWHERE() NSLog(@"File: %s, Fun: %s, Line: %d",__OFILENAME__,__FUNCTION__,__LINE__)
#define OLOGWHEREMSG(msg) NSLog(@"File: %s, Fun: %s, Line: %d, msg: %s",__OFILENAME__,__FUNCTION__,__LINE__,msg)
#define OLOGWHEREVAL(val) NSLog(@"File: %s, Fun: %s, Line: %d, val: %d",__OFILENAME__,__FUNCTION__,__LINE__,val)
#else
#define OLOGWHERE()
#define OLOGWHEREMSG( msg )
#define OLOGWHEREVAL( val )
#endif
//IPHONE和IPAD设备方向支持
#define IPAD_LANDSCAPE_IPHONE_LANDSCAPE BWON
#define IPAD_LANDSCAPE_IPHONE_PORTRAIT BWOFF
#define IPAD_PORTRAIT_IPHONE_PORTRAIT BWOFF
#define IPAD_PORTRAIT_IPHONE_LANDSCAPE BWOFF


