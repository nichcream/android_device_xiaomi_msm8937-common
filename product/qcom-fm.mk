# FM
PRODUCT_PACKAGES += \
    FM2 \
    libqcomfm_jni \
    qcom.fmradio

# Init
PRODUCT_PACKAGES += \
    init.qcom.fm.sh

# Properties
PRODUCT_PROPERTY_OVERRIDES += \
    ro.fm.transmitter=false
