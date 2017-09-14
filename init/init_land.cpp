/*
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (c) 2017, The XPerience Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <android-base/properties.h>
#include <android-base/logging.h>
#include <fcntl.h>
#include <stdlib.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/sysinfo.h>

//for cmdline parsing
#include <android-base/file.h>
#include <android-base/strings.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

static std::string board_id;
using android::base::GetProperty;
using android::base::Trim;
using android::init::property_set;
using android::init::import_kernel_cmdline;

//Take care about 3gb ram
int is3GB()
{
    struct sysinfo sys;
    sysinfo(&sys);
    return sys.totalram > 2048ull * 1024 * 1024;
}

//cmdline parser
void import_kernel_cmdline1(bool in_qemu,
                           const std::function<void(const std::string&, const std::string&, bool)>& fn) {
    std::string cmdline;
    android::base::ReadFileToString("/proc/cmdline", &cmdline);

    for (const auto& entry : android::base::Split(android::base::Trim(cmdline), " ")) {
        std::vector<std::string> pieces = android::base::Split(entry, "=");
        if (pieces.size() >= 2) { // lineage's : == 2
            fn(pieces[0], pieces[1], in_qemu);
        }
    }
}


static void import_cmdline(const std::string& key,
        const std::string& value, bool for_emulator __attribute__((unused)))
{
    if (key.empty()) return;

    if (key == "board_id") {
        std::istringstream iss(value);
        std::string token;
        std::getline(iss, token, ':');
        board_id = token;
    }
}

static void init_alarm_boot_properties()
{
    char const *boot_reason_file = "/proc/sys/kernel/boot_reason";
    char const *power_off_alarm_file = "/persist/alarm/powerOffAlarmSet";
    std::string boot_reason;
    std::string power_off_alarm;
    std::string reboot_reason = GetProperty("ro.boot.alarmboot","");

    if (android::base::ReadFileToString(boot_reason_file, &boot_reason)
            && android::base::ReadFileToString(power_off_alarm_file, &power_off_alarm)) {
        /*
         * Setup ro.alarm_boot value to true when it is RTC triggered boot up
         * For existing PMIC chips, the following mapping applies
         * for the value of boot_reason:
         *
         * 0 -> unknown
         * 1 -> hard reset
         * 2 -> sudden momentary power loss (SMPL)
         * 3 -> real time clock (RTC)
         * 4 -> DC charger inserted
         * 5 -> USB charger inserted
         * 6 -> PON1 pin toggled (for secondary PMICs)
         * 7 -> CBLPWR_N pin toggled (for external power supply)
         * 8 -> KPDPWR_N pin toggled (power key pressed)
         */
         if ((Trim(boot_reason) == "3" || reboot_reason == "true")
                 && Trim(power_off_alarm) == "1") {
             property_set("ro.alarm_boot", "true");
         } else {
             property_set("ro.alarm_boot", "false");
         }
    }
}

void read_ramconfig()
{
    if (is3GB()) {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "288m");
        property_set("dalvik.vm.heapsize", "768m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "512k");
        property_set("dalvik.vm.heapmaxfree", "8m");
        //HWUI
        property_set("ro.hwui.texture_cache_size", "72");
        property_set("ro.hwui.layer_cache_size", "48");
        property_set("ro.hwui.r_buffer_cache_size", "8");
        property_set("ro.hwui.path_cache_size", "32");
        property_set("ro.hwui.gradient_cache_size", "1");
        property_set("ro.hwui.drop_shadow_cache_size", "6");
        property_set("ro.hwui.texture_cache_flushrate", "0.4");
        property_set("ro.hwui.text_small_cache_width", "1024");
        property_set("ro.hwui.text_small_cache_height", "1024");
        property_set("ro.hwui.text_large_cache_width", "2048");
        property_set("ro.hwui.text_large_cache_height", "1024");
    } else {
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "192m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapminfree", "2m");
        property_set("dalvik.vm.heapmaxfree", "8m");
        //HWUI
        property_set("ro.hwui.texture_cache_size", "72");
        property_set("ro.hwui.layer_cache_size", "48");
        property_set("ro.hwui.r_buffer_cache_size", "8");
        property_set("ro.hwui.path_cache_size", "32");
        property_set("ro.hwui.gradient_cache_size", "1");
        property_set("ro.hwui.drop_shadow_cache_size", "6");
        property_set("ro.hwui.texture_cache_flushrate", "0.4");
        property_set("ro.hwui.text_small_cache_width", "1024");
        property_set("ro.hwui.text_small_cache_height", "1024");
        property_set("ro.hwui.text_large_cache_width", "2048");
        property_set("ro.hwui.text_large_cache_height", "2048");
    }
}

void variant_properties()
{
    std::string device;

    device = GetProperty("ro.validus.device", "");
    if (device != "land")
        return;

    import_kernel_cmdline1(0, import_cmdline);

    //set board
    property_set("ro.product.wt.boardid", board_id.c_str());


    //Variants
    if (board_id == "S88537AA1") {
        property_set("ro.miui.support_fingerprint", "true");
        property_set("ro.build.display.wtid", "SW_S88537AA1_V079_M20_MP_XM");
    } else if (board_id == "S88537AB1") {
        property_set("ro.miui.support_fingerprint", "true");
        property_set("ro.build.display.wtid", "SW_S88537AB1_V079_M20_MP_XM");
    } else if (board_id == "S88537AC1") {
        property_set("ro.build.display.wtid", "SW_S88537AC1_V079_M20_MP_XM");
        property_set("ro.miui.support_fingerprint", "false");
    } else if (board_id == "S88537BA1") {
        property_set("ro.miui.support_fingerprint", "true");
        property_set("ro.build.display.wtid", "SW_S88537BA1_V079_M20_MP_XM");
        property_set("mm.enable.qcom_parser", "196495");
    } else if (board_id == "S88537CA1") {
        property_set("ro.miui.support_fingerprint", "true");
        property_set("ro.build.display.wtid", "SW_S88537CA1_V079_M20_MP_XM");
        property_set("mm.enable.qcom_parser", "196495");
    } else if (board_id == "S88537EC1") {
        property_set("ro.build.display.wtid", "SW_S88537EC1_V079_M20_MP_XM");
        property_set("ro.miui.support_fingerprint", "false");
        property_set("mm.enable.qcom_parser", "196495");
    }

    if (board_id == "S88537AB1"){
        property_set("ro.product.model", "Redmi 3X");
        property_set("ro.vendor.product.device", "Redmi 3X");
    } else {
        property_set("ro.product.model", "Redmi 3S");
        property_set("ro.vendor.product.device", "Redmi 3S");
    }

}

void vendor_load_properties()
{

    // init
    read_ramconfig();
    init_alarm_boot_properties();
    variant_properties();
}
