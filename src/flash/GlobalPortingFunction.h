/*
 *  GlobalPortingFunction.h
 *  BaiWan
 *
 *  Created by lining on 10-10-21.
 *  Modified by handong on 11-05-10.
 *  Copyright 2010 JQBar. All rights reserved.
 *
 */


#ifndef _GLOBAL_PORTING_FUNCTION_H_
#define _GLOBAL_PORTING_FUNCTION_H_

// to ClientView .
// ClientView is UIView.
#ifdef __cplusplus
extern "C" {
#endif
void renderFrame();
bool setupGraphics(int w, int h);
    
#ifdef __cplusplus
}
#endif

#endif
