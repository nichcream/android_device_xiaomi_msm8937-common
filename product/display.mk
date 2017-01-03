# Boot animation
TARGET_SCREEN_HEIGHT := 1280
TARGET_SCREEN_WIDTH := 720

# This device is xhdpi.  However the platform doesn't
# currently contain all of the bitmaps at xhdpi density so
# we do this little trick to fall back to the hdpi version
# if the xhdpi doesn't exist.
PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := xhdpi
# A list of dpis to select prebuilt apk, in precedence order.
PRODUCT_AAPT_PREBUILT_DPI := hdpi

# Ambient display
PRODUCT_PACKAGES += \
    CMDoze

# Dalvik
PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.heapgrowthlimit=192m \
    dalvik.vm.heapmaxfree=8m \
    dalvik.vm.heapminfree=4m \
    dalvik.vm.heapsize=384m \
    dalvik.vm.heapstartsize=16m \
    dalvik.vm.heaptargetutilization=0.75

# HWUI
PRODUCT_PROPERTY_OVERRIDES += \
    ro.hwui.drop_shadow_cache_size=6 \
    ro.hwui.gradient_cache_size=1 \
    ro.hwui.layer_cache_size=48 \
    ro.hwui.path_cache_size=32 \
    ro.hwui.r_buffer_cache_size=8 \
    ro.hwui.text_large_cache_width=2048 \
    ro.hwui.text_large_cache_height=2048 \
    ro.hwui.text_small_cache_width=1024 \
    ro.hwui.text_small_cache_height=1024 \
    ro.hwui.texture_cache_flushrate=0.4 \
    ro.hwui.texture_cache_size=72

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.opengles.aep.xml:system/etc/permissions/android.hardware.opengles.aep.xml

# Properties
PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=196609 \
    ro.sf.lcd_density=320
