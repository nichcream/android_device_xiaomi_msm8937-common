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

#include <android-base/logging.h>
#include <linux/msm_mdp.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>

#include "ColorEnhancement.h"

namespace {
constexpr char kFbDevNode[] = "/dev/graphics/fb0";

enum CE_MODES {
	STANDARD = 10,
	VIVID = 11,
	BRIGHT = 12,  // note: our panels don't actually support this
};
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

ColorEnhancement::ColorEnhancement() : mEnabled(false) {
    mInitStatus = setEnabled(false);
}

bool ColorEnhancement::isSupported() {
    return mInitStatus;
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IColorEnhancement follow.
Return<bool> ColorEnhancement::isEnabled() {
    return mEnabled;
}

Return<bool> ColorEnhancement::setEnabled(bool enabled) {
    int fd = open(kFbDevNode, O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        LOG(ERROR) << "failed to open" << kFbDevNode;
        return false;
    }

    unsigned int modeID = enabled ? VIVID : STANDARD;
    if (ioctl(fd, MSMFB_ENHANCE_SET_CE, &modeID) != 0) {
        LOG(ERROR) << "failed to set color enhance";
        return false;
    }

    mEnabled = enabled;

    return true;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
