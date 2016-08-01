//
//  XWeixin.h
//  BaiWan
//
//  Created by imac1 on 13-12-3.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//

#ifndef __XBWSendMessageContent_h__
#define __XBWSendMessageContent_h__
#include "XMutex.h"
class BWSendMessageContentClass
{
public:
    XMutex m_lock;
    void *parameter1;
    int parameter1Len;
    int parameter1Width;
    int parameter1Height;
    void *parameter2;
    int parameter2Len;
    int parameter2Width;
    int parameter2Height;
    void *parameter3;
    int parameter3Len;
    int parameter3Width;
    int parameter3Height;
    void *parameter4;
    int parameter4Len;
    int parameter4Width;
    int parameter4Height;
    void *parameter5;
    int parameter5Len;
    int parameter5Width;
    int parameter5Height;
    void *parameter6;
    int parameter6Len;
    int parameter6Width;
    int parameter6Height;
    
    BWSendMessageContentClass()
    {
        parameter1=NULL;
        parameter2=NULL;
        parameter3=NULL;
        parameter4=NULL;
        parameter5=NULL;
        parameter6=NULL;
    };
    
    ~BWSendMessageContentClass()
    {
        if(parameter1)
        {
            free(parameter1);
        }
        if(parameter2)
        {
            free(parameter2);
        }
        if(parameter3)
        {
            free(parameter3);
        }
        if(parameter4)
        {
            free(parameter4);
        }
        if(parameter5)
        {
            free(parameter5);
        }
        if(parameter6)
        {
            free(parameter6);
        }
        parameter1 = NULL;
        parameter2 = NULL;
        parameter3 = NULL;
        parameter4 = NULL;
        parameter5 = NULL;
        parameter6 = NULL;
    };
};

#endif
