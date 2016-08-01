/*
 *  GlobalPortingFunction.cpp
 *  jqbar
 *
 *  Created by lining on 10-10-21.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GlobalPortingFunction.h"
#include "Raster_gl.h"
#include <android/native_window.h>
#include <android/log.h>
#include "GlobalFun.h"
#include "GlesUtil.h"
#include <string.h>
#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <pthread.h>
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
         = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[] =
"attribute vec4 vPosition;\n"
"void main() {\n"
"  gl_Position = vPosition;\n"
"}\n";

static const char gFragmentShader[] =
"precision mediump float;\n"
"void main() {\n"
"  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
"}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                         shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }
    
    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }
    
    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;
GLuint gvPositionHandle;

bool setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
    
    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
         gvPositionHandle);
    
    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return true;
}

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
    0.5f, -0.5f };

void renderFrame() {
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(1.0f, 1.0f, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");
    
    glUseProgram(gProgram);
    checkGlError("glUseProgram");
    
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");
}


EGLDisplay bwdisplay;
EGLConfig bwconfig;
EGLint bwnumConfigs;
EGLint bwformat,w,h;
EGLSurface bwsurface;
EGLContext bwcontext;
EGLSurface bwsurface_copy;
EGLContext bwcontext_copy;
EGLint bwwidth;
EGLint bwheight;
GLfloat bwratio;
ANativeWindow *bwwindow;
void g_initGles()
{
    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
    };
    const EGLint attrs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE, };
    bwwindow=GetWindow();
    bwdisplay=GetDisplay();
    LOGE("Got bwdisplay %p", bwdisplay);
    LOGE("Got bwwindow %p", bwwindow);
    LOGI("Initializing context");
    
    if (bwdisplay  != EGL_NO_DISPLAY) {
        LOGI("%d", 2222);
        
    }
    if (!eglInitialize(bwdisplay, 0, 0)) {
        LOGI("eglInitialize() returned error %d", eglGetError());
        
    }
    eglBindAPI(EGL_OPENGL_ES_API);
    
    if (!eglChooseConfig(bwdisplay, attribs, &bwconfig, 1, &bwnumConfigs)) {
        LOGI("eglChooseConfig() returned error %d", eglGetError());
        g_destroyGL();
        
    }
    
    if (!eglGetConfigAttrib(bwdisplay, bwconfig, EGL_NATIVE_VISUAL_ID, &bwformat)) {
        LOGI("eglGetConfigAttrib() returned error %d", eglGetError());
        g_destroyGL();
        
    }
    
    ANativeWindow_setBuffersGeometry(bwwindow, 0, 0, bwformat);
    LOGI("**********pgc1************");
    if (!(bwsurface_copy = eglCreateWindowSurface(bwdisplay, bwconfig, bwwindow, 0))) {
        LOGI("eglCreateWindowSurface() returned error %d", eglGetError());
//        g_destroyGL();
        
    }else{
        bwsurface = bwsurface_copy;
    }
    
    LOGI("**********pgc2************");
    if (!(bwcontext_copy = eglCreateContext(bwdisplay, bwconfig, EGL_NO_CONTEXT, attrs))) {
        LOGI("eglCreateContext() returned error %d", eglGetError());
//        g_destroyGL();
    }else{
        bwcontext = bwcontext_copy;
    }
    
    if (!eglMakeCurrent(bwdisplay, bwsurface, bwsurface, bwcontext)) {
        LOGI("eglMakeCurrent() returned error %d", eglGetError());
//        g_destroyGL();
        
    }
    
    if (!eglQuerySurface(bwdisplay, bwsurface, EGL_WIDTH, &bwwidth) ||
        !eglQuerySurface(bwdisplay, bwsurface, EGL_HEIGHT, &bwheight)) {
        LOGI("eglQuerySurface() returned error %d", eglGetError());
//        g_destroyGL();
        
    }
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
    
    glHint( GL_GENERATE_MIPMAP_HINT, GL_NICEST );
    
    glHint( GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES, GL_NICEST );
    
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE  );
    glDisable( GL_DITHER );
    glDepthMask( GL_TRUE );
    glDepthFunc( GL_LESS );
    glDepthRangef( 0.0f, 1.0f );
    glClearDepthf( 1.0f );
    glCullFace ( GL_BACK );
    glFrontFace( GL_CCW  );
    glClearStencil( 0 );
    glStencilMask( 0xFFFFFFFF );
    //绘制三角形
//    setupGraphics(1232,800);
//    if (bwdisplay) {
//        renderFrame();
//        if (!eglSwapBuffers(bwdisplay, bwsurface)) {
//            LOGE("eglSwapBuffers() returned error %d", eglGetError());
//            g_destroyGL();
//        }
//        
//    }

}

void g_makeCurrent()
{
    LOGE("g_makeCurrent-->start L:366");
    LOGE("Got bwdisplay %p", bwdisplay);
    LOGE("Got bwsurface %p", bwsurface);
    LOGE("Got bwcontext %p", bwcontext);
    if (!eglMakeCurrent(bwdisplay, bwsurface, bwsurface, bwcontext)) {
        LOGI("eglMakeCurrent() returned error %d", eglGetError());
        g_destroyGL();
    }
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);
    
    glHint( GL_GENERATE_MIPMAP_HINT, GL_NICEST );
    
    glHint( GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES, GL_NICEST );
    
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE  );
    glDisable( GL_DITHER );
    glDepthMask( GL_TRUE );
    glDepthFunc( GL_LESS );
    glDepthRangef( 0.0f, 1.0f );
    glClearDepthf( 1.0f );
    glCullFace ( GL_BACK );
    glFrontFace( GL_CCW  );
    glClearStencil( 0 );
    glStencilMask( 0xFFFFFFFF );
}

void g_freshGles()
{
    LOGE("g_freshGles-->start L:376");
    LOGE("Got bwdisplay %p", bwdisplay);
    LOGE("Got bwcontext %p", bwsurface);
    if (!eglSwapBuffers(bwdisplay, bwsurface)) {
        LOGE("eglSwapBuffers() returned error %d", eglGetError());
        g_destroyGL();

    }
}

void g_destroyGL(){
    LOGE("g_destroyGL-->start  L:376");
    eglMakeCurrent(bwdisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(bwdisplay, bwcontext);
    eglDestroySurface(bwdisplay, bwsurface);
    eglTerminate(bwdisplay);
    
    bwdisplay = EGL_NO_DISPLAY;
    bwsurface = EGL_NO_SURFACE;
    bwcontext = EGL_NO_CONTEXT;
    RasterGL::sharedRasterGL()->mbInit =false;
}