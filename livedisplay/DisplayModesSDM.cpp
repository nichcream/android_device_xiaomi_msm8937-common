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

#include "DisplayModesSDM.h"
#include "PictureAdjustment.h"
#include "Utils.h"

namespace {
struct sdm_disp_mode {
    int32_t id;
    int32_t type;
    int32_t len;
    char* name;
    sdm_disp_mode() : id(-1), type(0), len(128) {
        name = new char[128];
    }
    ~sdm_disp_mode() {
        delete name;
    }
};
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

DisplayModesSDM::DisplayModesSDM(const std::shared_ptr<SDMController>& controller, uint64_t cookie)
    : mController(controller), mCookie(cookie) {
}

bool DisplayModesSDM::isSupported() {
    int32_t count = 0;
    uint32_t flags = 0;
    static int supported = -1;

    if (supported >= 0) {
        goto out;
    }

    if (!Utils::checkFeatureVersion(mController.get(), mCookie, FEATURE_VER_SW_SAVEMODES_API)) {
        supported = 0;
        goto out;
    }

    if (mController->get_num_display_modes(mCookie, 0, 0, &count, &flags) != 0) {
        supported = 0;
        goto out;
    }

    supported = (count > 0);
out:
    return supported;
}

std::vector<DisplayMode> DisplayModesSDM::getDisplayModesInternal() {
    std::vector<DisplayMode> modes;
    int32_t count = 0;
    uint32_t flags = 0;

    if (mController->get_num_display_modes(mCookie, 0, 0, &count, &flags) != 0 || count == 0) {
        return modes;
    }

    sdm_disp_mode tmp[count];

    if (mController->get_display_modes(mCookie, 0, 0, tmp, count, &flags) == 0) {
        for (int i = 0; i < count; i++) {
            modes.push_back(DisplayMode{tmp[i].id, std::string(tmp[i].name)});
        }
    }

    return modes;
}

DisplayMode DisplayModesSDM::getDisplayModeById(int32_t id) {
    std::vector<DisplayMode> modes = getDisplayModesInternal();

    for (const DisplayMode& mode : modes) {
        if (mode.id == id) {
            return mode;
        }
    }

    return DisplayMode{-1, ""};
}

DisplayMode DisplayModesSDM::getCurrentDisplayModeInternal() {
    int32_t id = 0;
    uint32_t mask = 0, flags = 0;

    if (mController->get_active_display_mode(mCookie, 0, &id, &mask, &flags) == 0 && id >= 0) {
        return getDisplayModeById(id);
    }

    return DisplayMode{-1, ""};
}

DisplayMode DisplayModesSDM::getDefaultDisplayModeInternal() {
    int32_t id = 0;
    uint32_t flags = 0;

    if (mController->get_default_display_mode(mCookie, 0, &id, &flags) == 0 && id >= 0) {
        return getDisplayModeById(id);
    }

    return DisplayMode{-1, ""};
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
Return<void> DisplayModesSDM::getDisplayModes(getDisplayModes_cb _hidl_cb) {
    _hidl_cb(getDisplayModesInternal());
    return Void();
}

Return<void> DisplayModesSDM::getCurrentDisplayMode(getCurrentDisplayMode_cb _hidl_cb) {
    _hidl_cb(getCurrentDisplayModeInternal());
    return Void();
}

Return<void> DisplayModesSDM::getDefaultDisplayMode(getDefaultDisplayMode_cb _hidl_cb) {
    _hidl_cb(getDefaultDisplayModeInternal());
    return Void();
}

Return<bool> DisplayModesSDM::setDisplayMode(int32_t modeID, bool makeDefault) {
    DisplayMode currentMode = getCurrentDisplayModeInternal();

    if (currentMode.id >= 0 && currentMode.id == modeID) {
        return true;
    }

    DisplayMode mode = getDisplayModeById(modeID);
    if (mode.id < 0) {
        return false;
    }

    if (mController->set_active_display_mode(mCookie, 0, modeID, 0)) {
        return false;
    }

    if (makeDefault && mController->set_default_display_mode(mCookie, 0, modeID, 0)) {
        return false;
    }

    PictureAdjustment::updateDefaultPictureAdjustment();

    return true;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
