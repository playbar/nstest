//
//  OpenGLView.m
//  Tutorial01
//
//  Created by kesalin@gmail.com on 12-11-24.
//  Copyright (c) 2012年 http://blog.csdn.net/kesalin/. All rights reserved.
//

#import "OpenGLView.h"
#import "GLESUtils.h"
#import "GlobalPortingFunction.h"
#import "BaiWanViewController_ios.h"

//extern iOSDeviceRelateData g_deviceData;

static OpenGLView *pGLView = 0;

// 使用匿名 category 来声明私有成员
@interface OpenGLView()

- (void)setupLayer;
- (void)setupContext;
- (void)setupProgram;

- (void)setupBuffers;
//- (void)destoryBuffers;
- (void)resizeFormLayer:(CAEAGLLayer *)layer;

- (void)render;

@end

@implementation OpenGLView

+ (Class)layerClass
{
    // 只有 [CAEAGLLayer class] 类型的 layer 才支持在其上描绘 OpenGL 内容。
    return [CAEAGLLayer class];
}

- (void)setupLayer
{
    _eaglLayer = (CAEAGLLayer*) self.layer;
    
    // CALayer 默认是透明的，必须将它设为不透明才能让其可见
    _eaglLayer.opaque = YES;
    
    // 设置描绘属性，在这里设置不维持渲染内容以及颜色格式为 RGBA8
    _eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
}

+ (id) SharedGLView
{
    return pGLView;
}

/*+(bool) CheckForExtension:(NSString*)searchName
{
    NSString *extensionsString = [NSString stringWithCString:glGetString(GL_EXTENSIONS) encoding:NSASCIIStringEncoding];
    NSArray* extensionsNames =  [extensionsString componentsSeparatedBystring:@""];
    return [extensionsNames containsObject:searchName];
}
*/
-(void) initOpengl
{
    [self setupLayer];
    [self setupContext];
    [self setupBuffers];
    self.initOk = YES;
    //[self setupProgram];

}

-(void) swapBuffers
{
    //[EAGLContext setCurrentContext:_context];
    
    //glClearColor(1.0, 1.0, 0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    
    //[self render];
    if([m_viewController isResignActive])return;
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

-(void) makeCurrent
{
	[EAGLContext setCurrentContext:_context];
}

- (void)setupContext
{
    // 指定 OpenGL 渲染 API 的版本，在这里我们使用 OpenGL ES 2.0 
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        exit(1);
    }
    
    // 设置为当前上下文
    if (![EAGLContext setCurrentContext:_context])
    {
        _context = nil;
        NSLog(@"Failed to set current OpenGL context");
        exit(1);
    }
}

- (void)setupBuffers
{
    glGenRenderbuffers(1, &_colorRenderBuffer);
    // 设置为当前 renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    // 为 color renderbuffer 分配存储空间
    if( ![_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer] )
    {
        NSLog(@"failed to call context");
    }
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backingHeight);
    //NSLog(@"cocos2d: surface size: %dx%d", (int)_backingWidth, (int)_backingHeight);
    glGenFramebuffers(1, &_frameBuffer);
    // 设置为当前 framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    // 将 _colorRenderBuffer 装配到 GL_COLOR_ATTACHMENT0 这个装配点上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _colorRenderBuffer);
    
    
    glGenRenderbuffers(1, &_depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    glGetIntegerv(GL_MAX_SAMPLES_APPLE, &maxSamplesAllowed);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, _backingWidth, _backingHeight);
    //glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, maxSamplesAllowed,GL_DEPTH24_STENCIL8_OES, _backingWidth, _backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    
    
}

- (void)destoryBuffers
{
    glDeleteRenderbuffers(1, &_colorRenderBuffer);
    _colorRenderBuffer = 0;
    
    glDeleteRenderbuffers(1, &_depthBuffer);
    _depthBuffer = 0;

    glDeleteFramebuffers(1, &_frameBuffer);
    _frameBuffer = 0;
    
    [EAGLContext setCurrentContext:nil];
    _context = nil;
}

- (void)setupProgram
{
    // Load shaders
    //
    NSString * vertexShaderPath = [[NSBundle mainBundle] pathForResource:@"VertexShader"
                                                                  ofType:@"glsl"];
    NSString * fragmentShaderPath = [[NSBundle mainBundle] pathForResource:@"FragmentShader"
                                                                    ofType:@"glsl"];

    // Create program, attach shaders, compile and link program
    //
    _programHandle = [GLESUtils loadProgram:vertexShaderPath
                 withFragmentShaderFilepath:fragmentShaderPath];
    if (_programHandle == 0) {
        NSLog(@" >> Error: Failed to setup program.");
        return;
    }
    
    glUseProgram(_programHandle);
    
    // Get attribute slot from program
    //
    _positionSlot = glGetAttribLocation(_programHandle, "vPosition");
}

- (void)render
{
    glClearColor(0, 1.0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Setup viewport
    //
    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
    
    GLfloat vertices[] = {
        0.0f,  0.5f, 0.0f, 
        -0.5f, -0.5f, 0.0f,
        0.5f,  -0.5f, 0.0f };
    
    // Load the vertex data
    //
    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, 0, vertices );
    glEnableVertexAttribArray(_positionSlot);
    
    // Draw triangle
    //
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
}

- (id)initWithFrame:(CGRect)frame AndDelegate:(id <OpenGLViewDelegate>)target
{
    _initOk = NO;
    self = [super initWithFrame:frame];
    if (self) {
        pGLView = self;
    }
    multiTouchArray = [[NSMutableArray alloc] init];
   // invalidTouch = [[UITouch alloc] init];
    scale = [[UIScreen mainScreen] scale];
    [[self layer] setContentsScale:scale ];
    srcFrame = _frame = frame;
    g_setGLSize((int)frame.size.width * scale , (int)frame.size.height * scale );
    beginDistance_x = 0;
    beginDistance_y = 0;
    bFirst = true;
    self.delegate=target;
    [self addToolBar];
    
    
    
    
    return self;
}

-(void)addToolBar
{
    UIImage *image = [UIImage imageNamed:@"BWFloatToolButton"];
    UIImage *selectedImage = [UIImage imageNamed:@"BWFloatToolButtonHight.png"];
    UIImage *toggledImage = [UIImage imageNamed:@"BWFloatToolButtonSelect.png"];
    UIImage *toggledSelectedImage = [UIImage imageNamed:@"BWFloatToolButtonSelect.png"];
    CGPoint center = CGPointMake(35.0f, 35.0f);
    
    CGRect buttonFrame = CGRectMake(0, 0,70.0f, 70.0f);
    UIButton *b1 = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [b1 setFrame:buttonFrame];
    [b1 setTitle:@"One" forState:UIControlStateNormal];
    UIButton *b2 = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [b2 setTitle:@"Two" forState:UIControlStateNormal];
    [b2 setFrame:buttonFrame];
    UIButton *b3 = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    [b3 setTitle:@"Three" forState:UIControlStateNormal];
    [b3 setFrame:buttonFrame];
    NSArray *buttons = [NSArray arrayWithObjects:b1, b2, b3, nil];
    _bar = [[RNExpandingButtonBar alloc] initWithImage:image selectedImage:selectedImage toggledImage:toggledImage toggledSelectedImage:toggledSelectedImage buttons:buttons center:center];
    _bar.delegate=self;
    [_bar setHorizontal:YES];
    [_bar setExplode:NO];
    //[[UIApplication sharedApplication].delegate.window addSubview:bar];
    //self.view.frame = CGRectMake(0, 0, 40, 40);
    buttonBackButton=[[UIButton alloc]initWithFrame:CGRectMake(0, 0, 0, 0)];
    [buttonBackButton addTarget:_bar action:@selector(hideButtons) forControlEvents:UIControlEventTouchUpInside];
//    [self addSubview:buttonBackButton];
//    [self addSubview: _bar];
    // [self hideToolBar];
    
}
-(UIButton*) backButton
{
    return buttonBackButton;
}
#pragma mark-RNExpandingButtonBarDelegate
-(void)buttonsShow:(BOOL)isShow;
{
    if (isShow)
    {
        buttonBackButton.frame=CGRectMake(0, 0, self.frame.size.width, self.frame.size.height);
    }
    else
    {
        buttonBackButton.frame=CGRectMake(0, 0, 0, 0);
        
    }
    
}
-(void)toolButtonClick:(NSInteger)index
{
    [self.delegate successIntoGame:index];
}
-(void)BarTouchBegain
{
    [self.delegate OpneGlenTouch];;
}

- (void)dealloc {
    //[invalidTouch release];
	//invalidTouch = nil;
	[multiTouchArray release];
	multiTouchArray = nil;
    [_bar release];
    [buttonBackButton release];
    [self destoryBuffers];
    [super dealloc];
}

- (void)setFrame:(CGRect) frame
{
    _frame = frame;
    //NSLog(@"glview 宽：%f，高：%f", frame.size.width, frame.size.height);
    [super setFrame:frame];
}

- (void)setFrame:(CGRect)frame rotation:(BOOL)bRotation
{
    //if(bRotation)
        srcFrame = frame;
    [self setFrame:frame];
//    [self resizeFormLayer:_eaglLayer];
//    g_setGLSize((int)_frame.size.width * scale , (int)_frame.size.height * scale );
//    g_setViewPos(0, 0, _frame.size.width * scale, _frame.size.height * scale);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];

    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _colorRenderBuffer);
 //   glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
//    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_backingWidth);
//    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backingHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    //glGetIntegerv(GL_MAX_SAMPLES_APPLE, &maxSamplesAllowed);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, _frame.size.width * scale, _frame.size.height * scale);
    //glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, maxSamplesAllowed,GL_DEPTH24_STENCIL8_OES, _backingWidth, _backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    
    g_setGLSize((int)_frame.size.width * scale , (int)_frame.size.height * scale );
    if(!bRotation) return;
    g_setViewPos(0, 0, _frame.size.width * scale, _frame.size.height * scale);
    return;
}

- (void)layoutSubviews
{
//    if(_context)
//        [self resizeFormLayer:_eaglLayer];
/*    [EAGLContext setCurrentContext:_context];
    if( ! [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)_eaglLayer] )
    {
        NSLog(@"failed to call context");
    }
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backingHeight);
    NSLog(@"cocos2d: surface size: %dx%d", (int)_backingWidth, (int)_backingHeight);
    //[self resizeFormLayer:_eaglLayer];
    //[self destoryBuffers];
    //g_setGLSize((int)_frame.size.width * scale , (int)_frame.size.height * scale );
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //[self render];
 */
   // [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void) glviewHide:(BOOL) hid
{
    [self setFrame:srcFrame];
     self.hidden = hid;
    if (hid)
    {
        [buttonBackButton removeFromSuperview];
        [_bar removeFromSuperview];
    }
    else
    {
        if(m_viewController.showBarModeEnable)
        {
            [m_viewController.view addSubview:buttonBackButton];
            [m_viewController.view addSubview:_bar];
        }
    }
   [EAGLContext setCurrentContext:_context];
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)resizeFormLayer:(CAEAGLLayer*)layer
{
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, kEAGLColorFormatRGBA8 , _frame.size.width*scale, _frame.size.height*scale);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    // 将 _colorRenderBuffer 装配到 GL_COLOR_ATTACHMENT0 这个装配点上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
    
//    glGetIntegerv(GL_MAX_SAMPLES_APPLE, &maxSamplesAllowed);
//    glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, maxSamplesAllowed,GL_DEPTH24_STENCIL8_OES, _backingWidth, _backingHeight);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, _frame.size.width*scale, _frame.size.height*scale);
    glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
}
/*
- (UIView*)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    //point.x = point.x * 20;
    //point.y = point.y * 20;
    [super hitTest:point withEvent:event];
    return self;
}

- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event
{
    
    //[super pointInside:point withEvent:event];
    printf("x:%f,y%f\n",point.x,point.y );
    NSLog(@"view pointInside");
    return YES;
}
*/

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self->m_viewController showWidget:false];
	NSArray* eventTouches = [touches allObjects];
    NSArray * touchesArr=[[event allTouches] allObjects];
    //NSLog(@"手指个数%d",[touchesArr count]);
    if ([touchesArr count] == 2)
    {
        //CGPoint p1=[[touchesArr objectAtIndex:0] locationInView:self];
       // CGPoint p2=[[touchesArr objectAtIndex:1] locationInView:self];
        //beginDistance_x = p1.x > p2.x ? p1.x - p2.x : p2.x - p1.x;
        //beginDistance_y = p1.y > p2.y ? p1.y - p2.y : p2.y - p1.y;
        return;
    }
	for (UITouch* touch in eventTouches)
	{
        /*if ([multiTouchArray containsObject:invalidTouch])
        {
            int index = [multiTouchArray indexOfObject:invalidTouch];
            if (index != NSNotFound)
            {
                [multiTouchArray replaceObjectAtIndex:[multiTouchArray indexOfObject:invalidTouch] withObject:touch];
            }
            //CGPoint touchPoint = [touch locationInView:self];
            //int touchPos = [multiTouchArray indexOfObject:touch];
            
            //continue;
        }
        else*/
        [multiTouchArray addObject:touch];
        CGPoint touchPoint = [touch locationInView:self];
        int touchPos = [multiTouchArray indexOfObject:touch];
        float nSale = srcFrame.size.width / _frame.size.width;
        g_beginMouse( (touchPoint.x)*nSale * scale, (touchPoint.y)*nSale * scale, touchPos);
        touchEndPoint = touchPoint;
    }
 
 /*   UITouch* touch = [touches anyObject];
    
    NSUInteger numTaps = [touch tapCount];
    
    if (numTaps < 2) {
        
        [self.nextResponder touchesBegan:touches withEvent:event];
        
    } else {
        
        //[super handleDoubleTap:touch];
        
    }*/
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSArray* eventTouches = [touches allObjects];
    NSArray * touchesArr=[[event allTouches] allObjects];
    int num = [touchesArr count];
    if(num == 2)
    {
        if(!m_viewController.showBarModeEnable)return;
        //if (beginDistance_x == 0 && beginDistance_y == 0)
        //    return;
        CGPoint p1=[[touchesArr objectAtIndex:0] locationInView:self];
        CGPoint p2=[[touchesArr objectAtIndex:1] locationInView:self];
        if (bFirst)
        {
            firstPoint = p1;
            beginDistance_x = p1.x > p2.x ? p1.x - p2.x : p2.x - p1.x;
            beginDistance_y = p1.y > p2.y ? p1.y - p2.y : p2.y - p1.y;
            bFirst = false;
            //NSLog (@"bFirst = false");
            return;
        }
        float cx = p1.x > p2.x ? p1.x - p2.x : p2.x - p1.x;
        float cy = p1.y > p2.y ? p1.y - p2.y : p2.y - p1.y;
        float resX = cx-beginDistance_x;
        float resY = cy-beginDistance_y;
        float res = (resX<0?-resX:resX) < (resY<0?-resY:resY) ? resY : resX;
        float _scale  = 1.0;
        if (res == resX)
            _scale = (_frame.size.width + res)/_frame.size.width;
        else
            _scale = (_frame.size.height + res)/_frame.size.height;
        if ( _scale > 0.95 && _scale < 1.05 )
        {
            if (_frame.size.width == srcFrame.size.width)return;
            _frame.origin.x += p1.x - firstPoint.x;
            _frame.origin.y += p1.y - firstPoint.y;
            if (_frame.origin.x > srcFrame.origin.x)
                _frame.origin.x = srcFrame.origin.x;
            else if (_frame.origin.x < srcFrame.size.width - _frame.size.width)
                _frame.origin.x = srcFrame.size.width - _frame.size.width;
            if (_frame.origin.y > srcFrame.origin.y)
                _frame.origin.y = srcFrame.origin.y;
            else if (_frame.origin.y < srcFrame.size.height - _frame.size.height)
                _frame.origin.y = srcFrame.size.height - _frame.size.height;
            [self setFrame:_frame];
            return;
        }
        firstPoint = p1;
        if (_scale <= 1.0 )
            if (_frame.size.width == srcFrame.size.width)
                return;
        if (_scale > 1.0 )
            if (_frame.size.width == 2*srcFrame.size.width)
                return;
        _frame.size.width = _frame.size.width * _scale;
        _frame.size.height = _frame.size.height * _scale;
        if (_frame.size.width > 2*srcFrame.size.width)
        {
            _frame.size.width = 2*srcFrame.size.width;
            _frame.size.height = 2*srcFrame.size.height;
        }
        if (_frame.size.width < srcFrame.size.width)
        {
            _frame.size.width = srcFrame.size.width;
            _frame.size.height = srcFrame.size.height;
        }
        _frame.origin.x = (srcFrame.size.width - _frame.size.width)/2;
        _frame.origin.y = (srcFrame.size.height - _frame.size.height)/2;
        //if (_scale > 1.1 || _scale < 0.9)
            [self setFrame:_frame];
        //NSLog(@"手指个数%d,p1.x: %f, p2.x: %f",[touchesArr count], p1.x, p2.x);
        return;
    }
    
    
	for (UITouch* touch in eventTouches)
	{
        if ([multiTouchArray containsObject:touch])
		{
			CGPoint touchPoint = [touch locationInView:self];
            int touchPos = [multiTouchArray indexOfObject:touch];
            {
                float nSale = srcFrame.size.width / _frame.size.width;
                g_moveMouse( (touchPoint.x)*nSale * scale, (touchPoint.y)*nSale * scale, touchPos);
                touchEndPoint = touchPoint;
            }
		}
		//else
		//	continue;
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    bFirst = true;
    //NSLog (@"bFirst = true");
    beginDistance_x = 0;
    beginDistance_y = 0;
    NSArray * touchesArr=[[event allTouches] allObjects];
    int num = [touchesArr count];
    if(num == 2)
    {
        [multiTouchArray removeAllObjects];
        return;
    }
    if(num == 3)
    {
       // [m_viewController showWidget:true];
        return;
    }
    NSArray* eventTouches = [touches allObjects];
	for (UITouch* touch in eventTouches)
	{
		if ([multiTouchArray containsObject:touch])
		{
			CGPoint touchPoint = touchEndPoint;//[touch locationInView:self];
            //lastPoint = touchPoint;
			int touchPos = [multiTouchArray indexOfObject:touch];
            {
                printf("x:%f,y%f\n",touchPoint.x,touchPoint.y );
                float nSale = srcFrame.size.width / _frame.size.width;
                g_endMouse( (touchPoint.x)*nSale * scale, (touchPoint.y)*nSale * scale, touchPos);
            }
            /*int index = [multiTouchArray indexOfObject:touch];
            if (index != NSNotFound)
            {
                [multiTouchArray replaceObjectAtIndex:[multiTouchArray indexOfObject:touch] withObject:invalidTouch];
            }*/
        }
		//else
		//	continue;
	}
/*	NSArray* eventTouches = [touches allObjects];

	for (UITouch* touch in eventTouches)
	{
        CGPoint touchPoint = [touch locationInView:self];
        g_endMouse( touchPoint.x, touchPoint.y, (int)touch);
	}

    UITouch* touch = [touches anyObject];
    
    NSUInteger numTaps = [touch tapCount];
    
    if (numTaps < 2) {
        
        [self.nextResponder touchesEnded:touches withEvent:event];
        
    } else {
        
        //[self handleDoubleTap:touch];
        
    }
 */
    [multiTouchArray removeAllObjects];
}

- (void)setViewController:(BaiWanViewController_ios*)viewController
{
    m_viewController = viewController;
}

- (BaiWanViewController_ios*)viewController
{
    return m_viewController;
}
@end
