//
//  OpenGLView.h
//  Tutorial01
//
//  Created by kesalin@gmail.com on 12-11-24.
//  Copyright (c) 2012年 http://blog.csdn.net/kesalin/. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#import "RNExpandingButtonBar.h"
@class BaiWanViewController_ios;

@protocol OpenGLViewDelegate <NSObject>

-(void)successIntoGame:(NSInteger)index;
-(void)OpneGlenTouch;
@end


@interface OpenGLView : UIView<RNExpandingButtonBarDelegate>
{
    CAEAGLLayer* _eaglLayer;
    EAGLContext* _context;
    GLuint _colorRenderBuffer;
    GLuint _frameBuffer;
    GLuint _depthBuffer;
    
    GLuint _programHandle;
    GLuint _positionSlot;
    
    GLint _backingWidth;
    GLint _backingHeight;
    GLint maxSamplesAllowed;
    
    NSMutableArray* multiTouchArray;
    //UITouch* invalidTouch;
    float scale;
    CGRect _frame;
    
    //缩放
    CGRect srcFrame;
    float beginDistance_x;  //双指起始距离
    float beginDistance_y;
    bool bFirst;
    
    //平移窗口
    CGPoint firstPoint;      //双指平移起始点
    
    CGPoint touchEndPoint;
    NSTimer *timer;
    UIButton *buttonBackButton;
    BaiWanViewController_ios* m_viewController;
}
@property(assign,nonatomic)id <OpenGLViewDelegate>delegate;
@property(nonatomic,strong)  RNExpandingButtonBar *bar;
@property (nonatomic) BOOL initOk;
+(id) SharedGLView;
//+(bool) CheckForExtension:(NSString*)searchName;
-(void) initOpengl;
-(void) swapBuffers;
-(void) makeCurrent;
-(void)destoryBuffers;
- (void) glviewHide:(BOOL) hid;
- (void)setFrame:(CGRect)frame rotation:(BOOL)bRotation;
-(UIButton*) backButton;
//- (UIView*)hitTest:(CGPoint)point withEvent:(UIEvent *)event;
//- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event;
- (id)initWithFrame:(CGRect)frame AndDelegate:(id <OpenGLViewDelegate>)target;
- (void)setViewController:(BaiWanViewController_ios*)viewController;
- (BaiWanViewController_ios*)viewController;

@end
