#
# Copyright (C) 2016 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

CM_PATH := vendor/cm/device/product

# Overlay
DEVICE_PACKAGE_OVERLAYS += \
    $(LOCAL_PATH)/overlay

# Include common product fragments
include $(CM_PATH)/common/ant.mk
include $(CM_PATH)/common/audio.mk
include $(CM_PATH)/common/consumerir.mk
include $(CM_PATH)/common/fingerprint.mk
include $(CM_PATH)/common/gello.mk
include $(CM_PATH)/common/lights.mk
include $(CM_PATH)/common/media.mk
include $(CM_PATH)/common/perf.mk
include $(CM_PATH)/common/snap.mk

# Include QCOM product fragments
include $(CM_PATH)/qcom/audio.mk
include $(CM_PATH)/qcom/cne.mk
include $(CM_PATH)/qcom/display.mk
include $(CM_PATH)/qcom/fm.mk
include $(CM_PATH)/qcom/media.mk
include $(CM_PATH)/qcom/power.mk

# Include device-specific product fragments
include $(LOCAL_PATH)/product/*.mk

# Inherit proprietary files
$(call inherit-product-if-exists, vendor/xiaomi/land/land-vendor.mk)
