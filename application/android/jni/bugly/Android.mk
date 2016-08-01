LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


LOCAL_MODULE := libBugly
LOCAL_SRC_FILES :=  libBugly.so
include $(PREBUILT_SHARED_LIBRARY)