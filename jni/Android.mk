LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MAIN_LOCAL_PATH := $(call my-dir)


LOCAL_MODULE    := demo

LOCAL_SRC_FILES := main.c \
hook.c \

		

LOCAL_LDLIBS := -llog -landroid

include $(BUILD_SHARED_LIBRARY)
