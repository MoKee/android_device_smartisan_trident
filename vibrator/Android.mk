LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := android.hardware.vibrator@1.3-service.trident
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_RELATIVE_PATH := hw

LOCAL_VENDOR_MODULE := true

LOCAL_INIT_RC := android.hardware.vibrator@1.3-service.trident.rc
LOCAL_VINTF_FRAGMENTS := android.hardware.vibrator@1.3-service.trident.xml

LOCAL_SRC_FILES := \
    service.cpp \
    Vibrator.cpp

LOCAL_CFLAGS := -Wall -Werror

LOCAL_SHARED_LIBRARIES := \
    libhidlbase \
    libhidltransport \
    liblog \
    libutils \
    android.hardware.vibrator@1.0 \
    android.hardware.vibrator@1.1 \
    android.hardware.vibrator@1.2 \
    android.hardware.vibrator@1.3 \

include $(BUILD_EXECUTABLE)
