# Camera
PRODUCT_PACKAGES += \
    camera.$(TARGET_BOARD_PLATFORM)

# Properties
PRODUCT_PROPERTY_OVERRIDES += \
    camera.hal1.packagelist=com.skype.raider,com.google.android.talk,com.whatsapp,com.google.android.GoogleCamera \
    media.camera.ts.monotonic=0 \
    persist.camera.gyro.android=1 \
    persist.camera.is_type=1 \
    vidc.debug.perf.mode=2 \
    vidc.enc.dcvs.extra-buff-count=2 \
    vidc.enc.disable.pq=true \
    media.stagefright.legacyencoder=true \
    media.stagefright.less-secure=true \
    persist.camera.HAL3.enabled=1 
