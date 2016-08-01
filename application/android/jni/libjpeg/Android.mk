LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libjpeg

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_ARM_NEON := true
endif

LOCAL_SRC_FILES := 			\
			../../../../dependency/libjpeg/jerror.c 		\
			../../../../dependency/libjpeg/jmemmgr.c 		\
			../../../../dependency/libjpeg/jcapimin.c 		\
			../../../../dependency/libjpeg/jcparam.c 		\
			../../../../dependency/libjpeg/jdapimin.c 		\
			../../../../dependency/libjpeg/jdhuff.c 		\
			../../../../dependency/libjpeg/jfdctflt.c 		\
			../../../../dependency/libjpeg/jdcoefct.c 		\
			../../../../dependency/libjpeg/jidctint.c 		\
			../../../../dependency/libjpeg/jdcolor.c 		\
			../../../../dependency/libjpeg/jchuff.c 		\
			../../../../dependency/libjpeg/jcphuff.c 		\
			../../../../dependency/libjpeg/jidctfst.c 		\
			../../../../dependency/libjpeg/jutils.c 		\
			../../../../dependency/libjpeg/jidctflt.c 		\
			../../../../dependency/libjpeg/jcmainct.c 		\
			../../../../dependency/libjpeg/jcdctmgr.c 		\
			../../../../dependency/libjpeg/jddctmgr.c 		\
			../../../../dependency/libjpeg/jdatadst.c 		\
			../../../../dependency/libjpeg/jdinput.c 		\
			../../../../dependency/libjpeg/jdtrans.c 		\
			../../../../dependency/libjpeg/jdmaster.c 		\
			../../../../dependency/libjpeg/jdsample.c 		\
			../../../../dependency/libjpeg/jcmarker.c 		\
			../../../../dependency/libjpeg/jcomapi.c 		\
			../../../../dependency/libjpeg/jdmerge.c 		\
			../../../../dependency/libjpeg/jcinit.c 		\
			../../../../dependency/libjpeg/jdapistd.c 		\
			../../../../dependency/libjpeg/jdatasrc.c 		\
			../../../../dependency/libjpeg/jdphuff.c 		\
			../../../../dependency/libjpeg/jfdctfst.c 		\
			../../../../dependency/libjpeg/jccoefct.c 		\
			../../../../dependency/libjpeg/jctrans.c 		\
			../../../../dependency/libjpeg/jccolor.c 		\
			../../../../dependency/libjpeg/jcprepct.c 		\
			../../../../dependency/libjpeg/jidctred.c 		\
			../../../../dependency/libjpeg/jquant2.c 		\
			../../../../dependency/libjpeg/transupp.c 		\
			../../../../dependency/libjpeg/jdmainct.c 		\
			../../../../dependency/libjpeg/jcmaster.c 		\
			../../../../dependency/libjpeg/jquant1.c 		\
			../../../../dependency/libjpeg/jmemansi.c 		\
			../../../../dependency/libjpeg/jcsample.c 		\
			../../../../dependency/libjpeg/jdmarker.c 		\
			../../../../dependency/libjpeg/jcapistd.c 		\
			../../../../dependency/libjpeg/jfdctint.c 		\
			../../../../dependency/libjpeg/jdpostct.c

LOCAL_CFLAGS := 

LOCAL_C_INCLUDES :=  ../../../../dependency/libjpeg

include $(BUILD_STATIC_LIBRARY)
