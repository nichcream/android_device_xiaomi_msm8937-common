# Copyright (C) 2016 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := MediaBuffer.c
LOCAL_SHARED_LIBRARIES := libstagefright_foundation
LOCAL_MODULE := libshims_ims
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
    android/sensor.cpp \
    gui/SensorManager.cpp

LOCAL_C_INCLUDES := gui
LOCAL_SHARED_LIBRARIES := libgui libutils liblog libbinder libandroid
LOCAL_MODULE := libshim_camera
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
    GraphicBuffer.cpp \
    gui/SensorManager.cpp

LOCAL_C_INCLUDES := gui
LOCAL_SHARED_LIBRARIES := libgui libutils liblog libbinder libandroid
LOCAL_MODULE := libshim_sensors
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := parcel.cpp
LOCAL_SHARED_LIBRARIES := libbinder
LOCAL_MODULE := libshim_parcel
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
    bionic/bionic_time_conversions.cpp \
    bionic/pthread_cond.cpp

LOCAL_SHARED_LIBRARIES := libc
LOCAL_MODULE := libshims_camera
LOCAL_CLANG := false
LOCAL_CXX_STL := none
LOCAL_SANITIZE := never
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
	Camera.cpp \
	CameraMetadata.cpp \
	CaptureResult.cpp \
	CameraParameters2.cpp \
	ICamera.cpp \
	ICameraClient.cpp \
	ICameraService.cpp \
	ICameraServiceListener.cpp \
	ICameraServiceProxy.cpp \
	ICameraRecordingProxy.cpp \
	ICameraRecordingProxyListener.cpp \
	camera2/ICameraDeviceUser.cpp \
	camera2/ICameraDeviceCallbacks.cpp \
	camera2/CaptureRequest.cpp \
	camera2/OutputConfiguration.cpp \
	CameraBase.cpp \
	CameraUtils.cpp \
	VendorTagDescriptor.cpp \
	CameraParameters.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	liblog \
	libbinder \
	libhardware \
	libui \
	libgui \
	libcamera_metadata

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/include \
	system/media/camera/include \
	system/media/private/camera/include

LOCAL_MODULE:= libshim_ims_camera
LOCAL_CLANG := false
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
