/*
 *  KeyBoardMiddleLayer.cpp
 *  BaiWan
 *
 *  Created by lining on 10-11-5.
 
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#import "KeyBoardMiddleLayer.h"
//#ifdef __IPHONE__
//#import "BaiWanViewController_iPhone.h"
//#import "ClientView_iPhone.h"
//#import "inputViewController_iPhone.h"
//#elif defined(__IPAD__)
#import "BaiWanViewController_ios.h"
#import "inputView_ios.h"
#import "ClientView_ios.h"
//#endif
#include "GlobalPortingFunction.h"
#include "string_comp.h"
#include <assert.h>

KeyBoardMiddleLayer* KeyBoardMiddleLayer::_instance = 0;

KeyBoardMiddleLayer::KeyBoardMiddleLayer(void* controller)
:m_mobileViewController(controller), m_textViewController(0)
{
	
}

KeyBoardMiddleLayer::~KeyBoardMiddleLayer()
{
	
}

bool KeyBoardMiddleLayer::createInstance(void* mobileView)
{
	APPLEASSERT(!_instance);
	_instance = new KeyBoardMiddleLayer(mobileView);
	return _instance != 0;
}

void KeyBoardMiddleLayer::destoryInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = 0;
	}
}

KeyBoardMiddleLayer* KeyBoardMiddleLayer::getInstance()
{
	APPLEASSERT(_instance);
	return _instance;
}

bool KeyBoardMiddleLayer::startInput(const unsigned short* curString, int max_len, unsigned int flag)
{
//#ifdef __IPHONE__
//        BaiWanViewController_iPhone* old_controller = (BaiWanViewController_iPhone*)m_mobileViewController;
////        int mode = [old_controller mode];
//        
//        // at this time. old_controller is current view controller.
//        if (!m_textViewController)
//        {
//            m_textViewController = [[inputViewController_iPhone alloc] initWithNibName:@"inputViewController_iPhone" bundle:nil];
//        }
//        inputViewController_iPhone* inputController = (inputViewController_iPhone*)m_textViewController;
//        inputController.viewController = old_controller;
//        
//        old_controller.inputViewController = inputController;
//    
//        int str_len = gg_wcslen(curString);
//        
//        NSData* data = [[NSData alloc] initWithBytes:curString length:str_len * 2];
//        NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF16LittleEndianStringEncoding];
//        [data release];
//        
//        [inputController setText:str maxLen:max_len property:flag];
//        [str release];
//        
////        [inputController setMode:mode];
//        
////        [old_controller presentModalViewController:inputController animated:NO];
//    [old_controller startEdit];
//    [inputController showKeyBoard];
//    BWLOG(@"x=%f,y=%f,width=%f,height=%f",old_controller.clientView.frame.origin.x,old_controller.clientView.frame.origin.y, old_controller.clientView.frame.size.width,old_controller.clientView.frame.size.height);
//        return true;
//#elif __IPAD__
        BaiWanViewController_ios* old_controller = (BaiWanViewController_ios*)m_mobileViewController;
        
        // at this time. old_controller is current view controller.
        if (!m_textViewController)
        {
            m_textViewController = [[inputView_ios alloc] init];
        }
        inputView_ios* inputView = (inputView_ios*)m_textViewController;
        inputView.viewController = old_controller;
    
        old_controller.inputView = inputView;
    
        int str_len = gg_wcslen(curString);
        
        NSData* data = [[NSData alloc] initWithBytes:curString length:str_len * 2];
        NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF16LittleEndianStringEncoding];
        [data release];
        
        [inputView setText:str maxLen:max_len property:flag];
        [str release];
        
    [old_controller startEdit];
    [inputView showKeyBoard];
        return true;
//#endif
    
    return true;
}

void KeyBoardMiddleLayer::endInput(void* textField)
{
//#ifdef __IPHONE__
//        BaiWanViewController_iPhone* old_controller = (BaiWanViewController_iPhone*)m_mobileViewController;
//        UITextView* t_view = (UITextView*)textField;
//        t_view.text;
//        memset(m_wbuffer, 0, MAX_INPUT_EDIT);
//        int maxLength = [t_view.text maximumLengthOfBytesUsingEncoding:NSUTF16LittleEndianStringEncoding];
//        [t_view.text getCString:(char*)m_wbuffer maxLength:maxLength * 2 encoding:NSUTF16LittleEndianStringEncoding];
//		
//        ClientView_iPhone* clientView = [old_controller clientView];
//        void* mobileView = [clientView mobileView];
//        g_sendInputTextResultToMobileView(mobileView, m_wbuffer);
//#elif defined(__IPAD__)
        BaiWanViewController_ios* old_controller = (BaiWanViewController_ios*)m_mobileViewController;
    
    inputView_ios* inputView = (inputView_ios*)m_textViewController;
    inputView.viewController = old_controller;
    
    old_controller.inputView = inputView;
    
    
        UITextView* t_view = (UITextView*)textField;
        t_view.text;
        memset(m_wbuffer, 0, MAX_INPUT_EDIT);
        int maxLength = [t_view.text maximumLengthOfBytesUsingEncoding:NSUTF16LittleEndianStringEncoding];
        [t_view.text getCString:(char*)m_wbuffer maxLength:maxLength * 2 encoding:NSUTF16LittleEndianStringEncoding];
		
        ClientView_ios* clientView = [old_controller clientView];
        void* mobileView = [clientView mobileView];
        g_sendInputTextResultToMobileView(mobileView, m_wbuffer);
    
    [old_controller finishEdit];
//#endif
}

