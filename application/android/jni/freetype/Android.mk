LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_ARM_NEON := true
endif

LOCAL_SRC_FILES:= \
	../../../../dependency/freetype/src/base/ftbbox.c \
	../../../../dependency/freetype/src/base/ftbitmap.c \
	../../../../dependency/freetype/src/base/ftglyph.c \
	../../../../dependency/freetype/src/base/ftstroke.c \
	../../../../dependency/freetype/src/base/ftxf86.c \
	../../../../dependency/freetype/src/base/ftbase.c \
	../../../../dependency/freetype/src/base/ftsystem.c \
	../../../../dependency/freetype/src/base/ftinit.c \
	../../../../dependency/freetype/src/base/ftgasp.c \
	../../../../dependency/freetype/src/raster/raster.c \
	../../../../dependency/freetype/src/sfnt/sfnt.c \
	../../../../dependency/freetype/src/smooth/smooth.c \
	../../../../dependency/freetype/src/autofit/autofit.c \
	../../../../dependency/freetype/src/truetype/truetype.c \
	../../../../dependency/freetype/src/cff/cff.c \
	../../../../dependency/freetype/src/psnames/psnames.c \
	../../../../dependency/freetype/src/pshinter/pshinter.c

LOCAL_C_INCLUDES += \
	$../../../dependency/freetype/builds 	\
	$../../../dependency/freetype/include

LOCAL_CFLAGS += -W -Wall
LOCAL_CFLAGS += -fPIC -DPIC
LOCAL_CFLAGS += "-DDARWIN_NO_CARBON"
LOCAL_CFLAGS += "-DFT2_BUILD_LIBRARY"
LOCAL_CFLAGS += -O2
LOCAL_CFLAGS += -fshort-wchar

LOCAL_MODULE:= libft2

include $(BUILD_STATIC_LIBRARY)
