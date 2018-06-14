/*
 * Copyright (C) 2018 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android/hardware/power/1.0/IPower.h>
#include <hardware/hardware.h>
#include <hardware/power.h>
#include <log/log.h>

using android::hardware::power::V1_0::IPower;
using android::hardware::power::V1_0::PowerHint;
using android::hardware::power::V1_0::Feature;

static android::sp<IPower> mPowerHal;

static void power_init(__attribute__((unused)) struct power_module *module)
{
    ALOGI("QCOM power SHIM initializing.");
    mPowerHal = IPower::getService();
    if (mPowerHal == nullptr) {
        ALOGE("Couldn't load PowerHAL module");
    }
}

static void power_hint(__attribute__((unused)) struct power_module *module, power_hint_t hint,
        void *data)
{
    if (mPowerHal == nullptr)
        power_init(module);

    // power_hint_t -> PowerHint
    mPowerHal->powerHint(static_cast<PowerHint>(hint), *(int32_t *)data);
}

static void set_interactive(__attribute__((unused)) struct power_module *module, int on)
{
    if (mPowerHal == nullptr)
        power_init(module);

    // int -> bool
    mPowerHal->setInteractive(on ? true : false);
}

static void set_feature(__attribute__((unused))struct power_module *module, feature_t feature, int state)
{
    if (mPowerHal == nullptr)
        power_init(module);

    // feature_t -> Feature
    // int -> bool
    mPowerHal->setFeature(static_cast<Feature>(feature), state ? true : false);
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct power_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = POWER_MODULE_API_VERSION_0_3,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = POWER_HARDWARE_MODULE_ID,
        .name = "QCOM Power SHIM",
        .author = "Qualcomm",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .powerHint = power_hint,
    .setInteractive = set_interactive,
    .setFeature = set_feature
};
