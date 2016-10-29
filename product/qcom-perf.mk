# IRQ balance
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/msm_irqbalance.conf:system/vendor/etc/msm_irqbalance.conf

# Properties
PRODUCT_PROPERTY_OVERRIDES += \
    ro.am.reschedule_service=true \
    ro.core_ctl_min_cpu=2 \
    ro.core_ctl_max_cpu=4 \
    ro.vendor.at_library=libqti-at.so \
    ro.vendor.extension_library=libqti-perfd-client.so \
    ro.vendor.gt_library=libqti-gt.so
