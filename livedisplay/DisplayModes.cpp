/*
 * Copyright (C) 2019 The LineageOS Project
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

#define LOG_TAG "vendor.lineage.livedisplay@2.0-service.xiaomi"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <linux/msm_mdp.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>

#include "DisplayModes.h"
#include "PictureAdjustment.h"
#include "Utils.h"

namespace {
constexpr char kEnableSdmDmProperty[] = "vendor.livedisplay.enable_sdm_dm";
constexpr char kFbDevNode[] = "/dev/graphics/fb0";
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

using ::android::base::GetBoolProperty;

#define ARRAY_SIZE(x) (sizeof((x))/sizeof((x)[0]))

DisplayModes::DisplayModes() : mActiveModeId(-1), mInitStatus(false) {
    mFbDevFd = open(kFbDevNode, O_RDWR | O_CLOEXEC);
    if (mFbDevFd < 0) {
        LOG(ERROR) << "failed to open" << kFbDevNode;
        mInitStatus = false;
        return;
    }

    DisplayMode mode = getDefaultDisplayModeInternal();
    mInitStatus = setDisplayMode(mode.id, false);
}

DisplayModes::~DisplayModes() {
    if (mFbDevFd >= 0) {
        close(mFbDevFd);
    }
    mFbDevFd = -1;
}

bool DisplayModes::isSupported() {
    return !GetBoolProperty(kEnableSdmDmProperty, false) && mInitStatus;
}

std::vector<DisplayMode> DisplayModes::getDisplayModesInternal() {
    std::vector<DisplayMode> modes;
    const DisplayMode supportedModes[] = {
        { WARM, "Warm" },
        { NATURE, "Standard"},
        { COOL, "Cool" }
    };

    modes.assign(supportedModes, supportedModes + ARRAY_SIZE(supportedModes));

    return modes;
}

DisplayMode DisplayModes::getDisplayModeById(int32_t id) {
    std::vector<DisplayMode> modes = getDisplayModesInternal();

    for (const DisplayMode& mode : modes) {
        if (mode.id == id) {
            return mode;
        }
    }

    return DisplayMode{-1, ""};
}

DisplayMode DisplayModes::getCurrentDisplayModeInternal() {
    return getDisplayModeById(mActiveModeId);
}

DisplayMode DisplayModes::getDefaultDisplayModeInternal() {
    int32_t id = 0;
    if (Utils::readSavedModeId(&id) != 0 || !isValidDisplayModeId(id)) {
        LOG(INFO) << "Not able to use saved display mode, id=" << id;
        // Default to Nature mode otherwise
        id = NATURE;
    }
    return getDisplayModeById(id);
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
Return<void> DisplayModes::getDisplayModes(getDisplayModes_cb _hidl_cb) {
    _hidl_cb(getDisplayModesInternal());
    return Void();
}

Return<void> DisplayModes::getCurrentDisplayMode(getCurrentDisplayMode_cb _hidl_cb) {
    _hidl_cb(getCurrentDisplayModeInternal());
    return Void();
}

Return<void> DisplayModes::getDefaultDisplayMode(getDefaultDisplayMode_cb _hidl_cb) {
    _hidl_cb(getDefaultDisplayModeInternal());
    return Void();
}

Return<bool> DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    if (!isValidDisplayModeId(modeID)) {
        LOG(ERROR) << "Invalid display mode ID " << modeID;
        return false;
    }

    if (ioctl(mFbDevFd, MSMFB_ENHANCE_SET_GAMMA, &modeID) != 0) {
        LOG(ERROR) << "failed to set enhanced gamma";
        return false;
    }

    mActiveModeId = modeID;

    if (makeDefault && Utils::writeSavedModeId(mActiveModeId) != 0) {
        LOG(ERROR) << "failed to save default mode ID";
        return false;
    }

    return true;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
