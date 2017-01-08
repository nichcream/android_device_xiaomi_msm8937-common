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

DEVICE_PATH := device/xiaomi/land
CM_PATH := vendor/cm/config/board

# Define platform before including any common things
include $(DEVICE_PATH)/PlatformConfig.mk

# Inherit common ARM64 board fragments
include $(CM_PATH)/common/arm64/architecture.mk
include $(CM_PATH)/common/arm64/binder.mk

# Inherit common board fragments
include $(CM_PATH)/common/bluetooth.mk
include $(CM_PATH)/common/bootloader.mk
include $(CM_PATH)/common/camera.mk
include $(CM_PATH)/common/clang.mk
include $(CM_PATH)/common/cpusets.mk
include $(CM_PATH)/common/dlmalloc.mk
include $(CM_PATH)/common/gps.mk
include $(CM_PATH)/common/sepolicy.mk

# Inherit QCOM board fragments
include $(CM_PATH)/qcom/bluetooth.mk
include $(CM_PATH)/qcom/bootloader.mk
include $(CM_PATH)/qcom/cne.mk
include $(CM_PATH)/qcom/encryption.mk
include $(CM_PATH)/qcom/fm.mk
include $(CM_PATH)/qcom/gps.mk
include $(CM_PATH)/qcom/per-mgr.mk
include $(CM_PATH)/qcom/platform.mk
include $(CM_PATH)/qcom/power.mk
include $(CM_PATH)/qcom/ril.mk
include $(CM_PATH)/qcom/sepolicy.mk
include $(CM_PATH)/qcom/time.mk

# Inherit Cyanogen board fragments
include $(CM_PATH)/cyanogen/hardware.mk

# Inherit device-specific board fragments
include $(DEVICE_PATH)/board/*.mk

# Inherit the proprietary files
-include vendor/xiaomi/land/BoardConfigVendor.mk
