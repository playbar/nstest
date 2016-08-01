//
//  module.h
//  BaiWan
//
//  Created by yock on 12-6-4.
//  Copyright (c) 2012年 JQBar. All rights reserved.
//

#ifndef BaiWan_module_h
#define BaiWan_module_h


// handong pay button is unenable if refresh in flash
#define __HDShowPay

// zhangchen when saved that flash change size
//#define __ZCSaveSize

// handong remove flash copy of first launch
#define __HDLOCALNOCOPY


// handong 2012.07.13 解决封神道竖版聊天频道点击无效的bug
#define __HDCANTSHOWCHATLIST

// zhengjl 2012.11.13 增加DrawRect空函数, 以实现高清显示
//#define __AddDrawRect


//using retina screen for game
//#define __USE_RETINA_SCREEN__

// 是否支持语音输入
#define __USING_SPEECH_INPUT__

#endif//BaiWan_module_h
