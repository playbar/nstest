LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

TARGET_CPU = arm

# for different version of android
ANDROID_VERSION:=__ANDROID_2_3__
#ANDROID_SOURCE:=/Volumes/Product/Program/android_src
ANDROID_SOURCE:=/Program/androidsrc

ANDROID_TV:=OFF
# android TV
ifeq ($(ANDROID_TV),ON)
MY_ANDROID_TV:=__ANDROID_TV_ON_
else
MY_ANDROID_TV:=__ANDROID_TV_OFF_
endif

#
## android 2.3
#ifeq ($(ANDROID_VERSION),__ANDROID_2_3__)
#MY_ANDROID_SOURCE:=$(ANDROID_SOURCE)/src23
#MY_ANDROID_LIBRARY:=$(ANDROID_SOURCE)/lib23
#endif
#

MY_ANDROID_SOURCE:=$(ANDROID_SOURCE)/src40
MY_ANDROID_LIBRARY:=$(ANDROID_SOURCE)/lib40

#MY_LIBRARY:=-L$(MY_ANDROID_LIBRARY) -lsurfaceflinger_client -lskia -landroid_runtime -lutils -lnativehelper -lz -llog -ldl -lc -lGLESv2 \
#-L$(LOCAL_PATH)/../obj/local/armeabi/ -lGGBase -lGGFlash -lGGDom -liconv -lcharset -lOpenSLES

MY_LIBRARY:=-L$(MY_ANDROID_LIBRARY) -landroid_runtime -lz -llog -ldl -ljnigraphics -lc -lGLESv2 -lEGL -landroid\
-L$(LOCAL_PATH)/../obj/local/armeabi/ -lGGBase -lGGFlash -lGGDom

include $(call all-makefiles-under,$(LOCAL_PATH))

