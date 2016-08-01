LOCAL_PATH:= $(call my-dir)

# We need to build this for both the device (as a shared library)
# and the host (as a static library for tools to use).

common_SRC_FILES := \
	../../../../dependency/libpng/png.c \
	../../../../dependency/libpng/pngerror.c \
	../../../../dependency/libpng/pnggccrd.c \
	../../../../dependency/libpng/pngget.c \
	../../../../dependency/libpng/pngmem.c \
	../../../../dependency/libpng/pngpread.c \
	../../../../dependency/libpng/pngread.c \
	../../../../dependency/libpng/pngrio.c \
	../../../../dependency/libpng/pngrtran.c \
	../../../../dependency/libpng/pngrutil.c \
	../../../../dependency/libpng/pngset.c \
	../../../../dependency/libpng/pngtrans.c \
	../../../../dependency/libpng/pngvcrd.c \
	../../../../dependency/libpng/pngwio.c \
	../../../../dependency/libpng/pngwrite.c \
	../../../../dependency/libpng/pngwtran.c \
	../../../../dependency/libpng/pngwutil.c	\
	../../../../dependency/libpng/BWPngEncode.cpp
common_CFLAGS := ## -fomit-frame-pointer

common_C_INCLUDES += \
			$(JNI_H_INCLUDE)  \
			$(LOCAL_PATH)      \
			$../../../include  \
			$../../../include/base  \
			$../../../include/base/android  \

common_COPY_HEADERS_TO := libpng
common_COPY_HEADERS := \
			$../../../dependency/libpng/png.h \
			$../../../dependency/libpng/pngconf.h \
			$../../../dependency/libpng/pngusr.h

# For the device
# =====================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(common_SRC_FILES)
LOCAL_CFLAGS += $(common_CFLAGS)
LOCAL_C_INCLUDES += $(common_C_INCLUDES) \
	external/zlib
LOCAL_SHARED_LIBRARIES := \
	libz

LOCAL_MODULE:= libpng

LOCAL_COPY_HEADERS_TO := $(common_COPY_HEADERS_TO)
LOCAL_COPY_HEADERS := $(common_COPY_HEADERS)

include $(BUILD_STATIC_LIBRARY)


