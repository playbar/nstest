LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := BwAudio.cpp

LOCAL_LDLIBS := -lOpenSLES

LOCAL_MODULE := bw23

include $(BUILD_SHARED_LIBRARY)
