LOCAL_PATH := $(call my-dir)

PROJ_PATH = $(LOCAL_PATH)/../

#编译GGDom模块
include $(CLEAR_VARS)

LOCAL_MODULE := GGDom

LOCAL_ARM_MODE := arm

#LOCAL_CFLAGS :=-Wl,--no-undefined 
LOCAL_CFLAGS += -include "../StdAfxGGlib.h" \
				-D_SOURCECODE	\
				-D_FULLEDIT		\
				-D_ARGB_PIXEL_  \
				-fshort-wchar	\
				-D$(MY_ANDROID_TV) \
				
				#-D__TOUCHPAD__

LOCAL_C_INCLUDES :=  \
					$(JNI_H_INCLUDE)  \
					$(LOCAL_PATH)      \
					$(LOCAL_PATH)/../GGlibrary      \
					$../../../include  \
					$../../../include/base  \
					$../../../include/base/android  \
					$../../../Resource/include  \
					$../../../dependency/mad/include  \
					$../../../dependency/zlib   \
					$../../../dependency/gdx2d			\
					$../../../dependency/freetype/include \
					$../../../include/dom        \
					$../../../include/flash        \
					$../../../src/flash/AVMSWF     \
					$../../../src/flash/AVMSWF/Inc    \
					$../../../src/flash/core       \
					$../../../src/flash/eval      \
					$../../../src/flash/MMgc         \
					$../../../src/flash/nanojit         \
					$../../../src/flash/pcre            \
					$../../../src/flash/platform           \
					$../../../src/flash/VMPI   
					
			
#LOCAL_LDLIBS := $(MY_LIBRARY)

#GGDom
MY_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/dom/*.cpp)
MY_FILES := $(MY_FILES:$(LOCAL_PATH)/%=%)
# remove "GGAllocator.cpp" from source list
MY_FILES := $(foreach n, $(MY_FILES), $(if $(findstring GGAllocator.cpp,$(n)),,$(n)))
LOCAL_SRC_FILES += \
					$(MY_FILES)\
					/../../../../src/base/android/XSystem.cpp \
					
include $(BUILD_STATIC_LIBRARY)
