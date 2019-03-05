/*
 * Copyright (C) 2018-2019 The LineageOS Project
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

#include <android-base/logging.h>
#include <dlfcn.h>

#include "SDMController.h"

#define LOAD_SDM_FUNCTION(name) \
    mFn_##name = loadFunction<disp_api_##name>(mHandle.get(), "disp_api_" #name);

#define CLOSE_SDM_FUNCTION(name) mFn_##name = nullptr;

#define FOR_EACH_FUNCTION(MACRO)          \
    MACRO(init)                           \
    MACRO(deinit)                         \
    MACRO(get_global_color_balance_range) \
    MACRO(set_global_color_balance)       \
    MACRO(get_global_color_balance)       \
    MACRO(get_num_display_modes)          \
    MACRO(get_display_modes)              \
    MACRO(get_active_display_mode)        \
    MACRO(set_active_display_mode)        \
    MACRO(set_default_display_mode)       \
    MACRO(get_default_display_mode)       \
    MACRO(get_global_pa_range)            \
    MACRO(get_global_pa_config)           \
    MACRO(set_global_pa_config)           \
    MACRO(get_feature_version)

#define CONTROLLER_CHECK(function, ...)        \
    do {                                       \
        if (mFn_##function == nullptr) {       \
            return -1;                         \
        }                                      \
        int err = mFn_##function(__VA_ARGS__); \
        return err;                            \
    } while (0)

namespace {
constexpr char kFilename[] = "libsdm-disp-vndapis.so";

template <typename Function>
Function loadFunction(void* handle, const char* name) {
    void* fn = dlsym(handle, name);
    if (fn == nullptr) {
        LOG(ERROR) << "loadFunction -- failed to load function " << name;
    }
    return reinterpret_cast<Function>(fn);
}
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

SDMController::SDMController() {
    std::shared_ptr<void> handle(dlopen(kFilename, RTLD_NOW), [this](void* p) {
        FOR_EACH_FUNCTION(CLOSE_SDM_FUNCTION)
        if (p != nullptr) {
            int err = dlclose(p);
            p = nullptr;
            if (err != 0) {
                LOG(ERROR) << "DLCLOSE failed for " << kFilename;
            }
        }
    });
    if (handle == nullptr) {
        LOG(ERROR) << "DLOPEN failed for " << kFilename << " (" << dlerror() << ")";
        return;
    }
    mHandle = handle;

    FOR_EACH_FUNCTION(LOAD_SDM_FUNCTION)
}

int32_t SDMController::init(uint64_t* hctx, uint32_t flags) {
    CONTROLLER_CHECK(init, hctx, flags);
}

int32_t SDMController::deinit(uint64_t hctx, uint32_t flags) {
    CONTROLLER_CHECK(deinit, hctx, flags);
}

int32_t SDMController::get_global_color_balance_range(uint64_t hctx, uint32_t disp_id,
                                                      void* range) {
    CONTROLLER_CHECK(get_global_color_balance_range, hctx, disp_id, range);
}

int32_t SDMController::set_global_color_balance(uint64_t hctx, uint32_t disp_id, int32_t warmness,
                                                uint32_t flags) {
    CONTROLLER_CHECK(set_global_color_balance, hctx, disp_id, warmness, flags);
}

int32_t SDMController::get_global_color_balance(uint64_t hctx, uint32_t disp_id, int32_t* warmness,
                                                uint32_t* flags) {
    CONTROLLER_CHECK(get_global_color_balance, hctx, disp_id, warmness, flags);
}

int32_t SDMController::get_num_display_modes(uint64_t hctx, uint32_t disp_id, uint32_t mode_type,
                                             int32_t* mode_cnt, uint32_t* flags) {
    CONTROLLER_CHECK(get_num_display_modes, hctx, disp_id, mode_type, mode_cnt, flags);
}

int32_t SDMController::get_display_modes(uint64_t hctx, uint32_t disp_id, uint32_t mode_type,
                                         void* modes, int32_t mode_cnt, uint32_t* flags) {
    CONTROLLER_CHECK(get_display_modes, hctx, disp_id, mode_type, modes, mode_cnt, flags);
}

int32_t SDMController::get_active_display_mode(uint64_t hctx, uint32_t disp_id, int32_t* mode_id,
                                               uint32_t* mask, uint32_t* flags) {
    CONTROLLER_CHECK(get_active_display_mode, hctx, disp_id, mode_id, mask, flags);
}

int32_t SDMController::set_active_display_mode(uint64_t hctx, uint32_t disp_id, int32_t mode_id,
                                               uint32_t flags) {
    CONTROLLER_CHECK(set_active_display_mode, hctx, disp_id, mode_id, flags);
}

int32_t SDMController::set_default_display_mode(uint64_t hctx, uint32_t disp_id, int32_t mode_id,
                                                uint32_t flags) {
    CONTROLLER_CHECK(set_default_display_mode, hctx, disp_id, mode_id, flags);
}

int32_t SDMController::get_default_display_mode(uint64_t hctx, uint32_t disp_id, int32_t* mode_id,
                                                uint32_t* flags) {
    CONTROLLER_CHECK(get_default_display_mode, hctx, disp_id, mode_id, flags);
}

int32_t SDMController::get_global_pa_range(uint64_t hctx, uint32_t disp_id, void* range) {
    CONTROLLER_CHECK(get_global_pa_range, hctx, disp_id, range);
}

int32_t SDMController::get_global_pa_config(uint64_t hctx, uint32_t disp_id, uint32_t* enable,
                                            void* cfg) {
    CONTROLLER_CHECK(get_global_pa_config, hctx, disp_id, enable, cfg);
}

int32_t SDMController::set_global_pa_config(uint64_t hctx, uint32_t disp_id, uint32_t enable,
                                            void* cfg) {
    CONTROLLER_CHECK(set_global_pa_config, hctx, disp_id, enable, cfg);
}

int32_t SDMController::get_feature_version(uint64_t hctx, uint32_t feature_id, void* ver,
                                           uint32_t* flags) {
    CONTROLLER_CHECK(get_feature_version, hctx, feature_id, ver, flags);
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
