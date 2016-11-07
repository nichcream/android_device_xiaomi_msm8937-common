# Audio
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/audio/sound_trigger_mixer_paths.xml:system/etc/sound_trigger_mixer_paths.xml \
    $(LOCAL_PATH)/audio/sound_trigger_platform_info.xml:system/etc/sound_trigger_platform_info.xml

# Properties
PRODUCT_PROPERTY_OVERRIDES += \
    audio.dolby.ds2.enabled=true \
    audio.playback.mch.downsample=true \
    audio.safx.pbe.enabled=true \
    ro.qc.sdk.audio.fluencetype=fluence \
    persist.audio.fluence.voicecall=true \
    use.qti.sw.alac.decoder=true \
    use.qti.sw.ape.decoder=true \
    voice.conc.fallbackpath=deep-buffer \
    voice.playback.conc.disabled=true \
    voice.voip.conc.disabled=true
