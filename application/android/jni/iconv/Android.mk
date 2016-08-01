
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := iconv

LOCAL_CFLAGS += \
    -DHAVE_CONFIG_H \
    -DBUILDING_LIBICONV \
    -DIN_LIBRARY  \
    -fshort-wchar
 #  -DLIBICONV_PLUG

LOCAL_C_INCLUDES += \
    $../../../dependency/iconv/include \
    $../../../dependency/libcharset/include

LOCAL_SRC_FILES := \
    ../../../../dependency/iconv/lib/iconv.c

LOCAL_STATIC_LIBRARIES += charset

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

include $(call all-makefiles-under,$(LOCAL_PATH))

