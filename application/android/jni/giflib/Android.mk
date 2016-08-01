LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_ARM_NEON := true
endif

LOCAL_SRC_FILES := \
	../../../../dependency/giflib/dgif_lib.c \
	../../../../dependency/giflib/gifalloc.c \
	../../../../dependency/giflib/gif_err.c

LOCAL_CFLAGS += -Wno-format  \
				-DHAVE_CONFIG_H
				
LOCAL_C_INCLUDES :=  $../../../dependency/giflib

LOCAL_MODULE:= libgif

include $(BUILD_STATIC_LIBRARY)
