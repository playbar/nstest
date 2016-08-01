LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#ANDROID_SOURCE:=/Volumes/androidsrc
#ANDROID_SOURCE:=/home/Android

BW_ANDROID_SOURCE:=$(ANDROID_SOURCE)/src22
BW_ANDROID_LIBRARY:=$(ANDROID_SOURCE)/lib22

LOCAL_MODULE := bw22
LOCAL_ARM_MODE := arm


LOCAL_C_INCLUDES := $(LOCAL_PATH) $(JNI_H_INCLUDE)     \
					$(BW_ANDROID_SOURCE)/frameworks/base/include \
					$(BW_ANDROID_SOURCE)/system/core/include \

LOCAL_LDLIBS := -L$(BW_ANDROID_LIBRARY) -lmedia

LOCAL_SRC_FILES := BwAudio.cpp

include $(BUILD_SHARED_LIBRARY)
