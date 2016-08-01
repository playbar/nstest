LOCAL_PATH := $(call my-dir)

#缂�璇�GGlibrary
include $(CLEAR_VARS)

LOCAL_MODULE := GGlibrary

LOCAL_ARM_MODE := arm

LOCAL_LDFLAGS += -fuse=bfd
LOCAL_LDFLAGS += -fuse-ld=bfd

#LOCAL_CFLAGS :=-Wl,--no-undefined 
LOCAL_CFLAGS += -include "../StdAfxGGlib.h" \
				-DHAVE_PTHREADS \
				-D_SOURCECODE \
				-fshort-wchar \
#				-D$(ANDROID_VERSION)

LOCAL_C_INCLUDES :=  \
					$(JNI_H_INCLUDE)  \
					$(MY_ANDROID_SOURCE)/frameworks/base/include \
					$(MY_ANDROID_SOURCE)/system/core/include         \
					$(MY_ANDROID_SOURCE)/dalvik/libnativehelper/include \
					$(MY_ANDROID_SOURCE)/frameworks/base/core/jni/android/graphics \
					$(MY_ANDROID_SOURCE)/hardware/libhardware/include \
					$(LOCAL_PATH)      \
					$../../../include  \
					$../../../include/base  \
					$../../../include/base/android  \
					$../../../Resource/include  \
					$../../../dependency/mad/include  \
					$../../../dependency/gles/gles  \
					$../../../dependency/zlib   \
					$../../../dependency/gdx2d		\
					$../../../dependency/freetype/include \
					$../../../include/dom        \
					$../../../include/flash        		\
					$../../../src/flash/AVMSWF    \
					$../../../src/flash/AVMSWF/api    \
					$../../../src/flash/AVMSWF/api/flash/accessibility \
					$../../../src/flash/AVMSWF/api/flash/data  \
					$../../../src/flash/AVMSWF/api/flash/desktop \
					$../../../src/flash/AVMSWF/api/flash/display \
					$../../../src/flash/AVMSWF/api/flash/errors \
					$../../../src/flash/AVMSWF/api/flash/events \
					$../../../src/flash/AVMSWF/api/flash/external \
					$../../../src/flash/AVMSWF/api/flash/filesystem \
					$../../../src/flash/AVMSWF/api/flash/filters \
					$../../../src/flash/AVMSWF/api/flash/geom \
					$../../../src/flash/AVMSWF/api/flash/globalization \
					$../../../src/flash/AVMSWF/api/flash/html \
					$../../../src/flash/AVMSWF/api/flash/media \
					$../../../src/flash/AVMSWF/api/flash/net/dns \
					$../../../src/flash/AVMSWF/api/flash/net/drm \
					$../../../src/flash/AVMSWF/api/flash/net \
					$../../../src/flash/AVMSWF/api/flash/printing \
					$../../../src/flash/AVMSWF/api/flash/sampler \
					$../../../src/flash/AVMSWF/api/flash/security \
					$../../../src/flash/AVMSWF/api/flash/sensors \
					$../../../src/flash/AVMSWF/api/flash/system \
					$../../../src/flash/AVMSWF/api/flash/text/engine \
					$../../../src/flash/AVMSWF/api/flash/text/ime \
					$../../../src/flash/AVMSWF/api/flash/text \
					$../../../src/flash/AVMSWF/api/flash/ui \
					$../../../src/flash/AVMSWF/api/flash/utils \
					$../../../src/flash/AVMSWF/api/flash/xml \
					$../../../src/flash/AVMSWF/api/BWLoader \
					$../../../src/flash/AVMSWF/api/adobe/utils \
					$../../../src/flash/AVMSWF/api/JavaScript \
					$../../../src/flash/core       		\
					$../../../src/flash/eval      		\
					$../../../src/flash/MMgc         	\
					$../../../src/flash/nanojit         \
					$../../../src/flash/pcre            \
					$../../../src/flash/platform   		\
					$../../../src/flash/VMPI
					
		
LOCAL_SRC_FILES:= com_jqbar_MobileView.cpp	\
		    	  MobileView.cpp 				\
		    	  MobileMain.cpp 				\
				  com_jqbar_MobileMain.cpp 	\
				  GlobalFun.cpp 				\
		    	  GlesUtil.cpp
# 				  com_jqbar_MobileViewGLES.cpp	\
 				  MobileViewGLES.cpp
#com_jqbar_MobileViewSurface.cpp	\
 MobileViewSurface.cpp		\
renderer.cpp 				\
 
LOCAL_STATIC_LIBRARIES := GGBase
LOCAL_STATIC_LIBRARIES += GGDom
LOCAL_STATIC_LIBRARIES += GGFlash
LOCAL_STATIC_LIBRARIES += png
LOCAL_STATIC_LIBRARIES += ft2
LOCAL_STATIC_LIBRARIES += gif
LOCAL_STATIC_LIBRARIES += jpeg
#LOCAL_STATIC_LIBRARIES += tiff
LOCAL_STATIC_LIBRARIES += iconv
LOCAL_STATIC_LIBRARIES += charset

LOCAL_LDLIBS := $(MY_LIBRARY)
#LOCAL_LDLIBS += -fuse-ld=bfd

#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)

