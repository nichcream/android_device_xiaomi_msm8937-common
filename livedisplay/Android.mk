# Copyright (C) 2019 The LineageOS Project
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

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := vendor.lineage.livedisplay@2.0-service.xiaomi
LOCAL_INIT_RC := vendor.lineage.livedisplay@2.0-service.xiaomi.rc
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_HEADER_LIBRARIES := generated_kernel_headers

LOCAL_SRC_FILES := \
    AdaptiveBacklight.cpp \
    ColorEnhancement.cpp \
    DisplayModes.cpp \
    DisplayModesSDM.cpp \
    PictureAdjustment.cpp \
    SDMController.cpp \
    Utils.cpp \
    service.cpp

LOCAL_SHARED_LIBRARIES := \
    libbase \
    libbinder \
    libcutils \
    libdl \
    libhidlbase \
    libhidltransport \
    liblog \
    libutils \
    vendor.lineage.livedisplay@2.0

include $(BUILD_EXECUTABLE)
