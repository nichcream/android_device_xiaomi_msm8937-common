# Camera
PRODUCT_PACKAGES += \
    camera.$(TARGET_BOARD_PLATFORM)

# Properties
PRODUCT_PROPERTY_OVERRIDES += \
    camera.hal1.packagelist=com.skype.raider,com.google.android.talk,com.whatsapp,com.google.android.GoogleCamera \
    media.camera.ts.monotonic=1 \
    persist.camera.gyro.android=1 \
    persist.camera.is_type=1
