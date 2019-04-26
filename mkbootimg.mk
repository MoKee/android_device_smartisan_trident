INSTALLED_RECOVERY_RAMDISK_TARGET := $(PRODUCT_OUT)/prebuilt-ramdisk-recovery.img

INTERNAL_RECOVERYIMAGE_ARGS := \
	--kernel $(INSTALLED_KERNEL_TARGET) \
	--ramdisk $(INSTALLED_RECOVERY_RAMDISK_TARGET)

INTERNAL_RECOVERYIMAGE_FILES := $(filter-out --%,$(INTERNAL_RECOVERYIMAGE_ARGS))

INTERNAL_RECOVERYIMAGE_ARGS += \
	--cmdline "$(INTERNAL_KERNEL_CMDLINE)" \
	--base $(BOARD_KERNEL_BASE) \
	--pagesize $(BOARD_KERNEL_PAGESIZE)

$(INSTALLED_RECOVERY_RAMDISK_TARGET): $(BOARD_PREBUILT_RECOVERY_RAMDISK)
	cp $(BOARD_PREBUILT_RECOVERY_RAMDISK) $@

$(INSTALLED_BOOTIMAGE_TARGET): $(MKBOOTFS) $(MKBOOTIMG) $(MINIGZIP) $(ADBD) \
		$(INSTALLED_RAMDISK_TARGET) \
		$(INTERNAL_RECOVERYIMAGE_FILES) \
		$(recovery_initrc) $(recovery_sepolicy) $(recovery_kernel) \
		$(INSTALLED_2NDBOOTLOADER_TARGET) \
		$(recovery_build_props) $(recovery_resource_deps) \
		$(recovery_fstab) \
		$(RECOVERY_INSTALL_OTA_KEYS) \
		$(INSTALLED_VENDOR_DEFAULT_PROP_TARGET) \
		$(BOARD_RECOVERY_KERNEL_MODULES) \
		$(DEPMOD)
	$(call pretty,"Target boot image from recovery: $@")
	$(call build-recoveryramdisk)
	$(hide) $(MKBOOTIMG) \
		$(INTERNAL_RECOVERYIMAGE_ARGS) \
		$(INTERNAL_MKBOOTIMG_VERSION_ARGS) \
		$(BOARD_MKBOOTIMG_ARGS) \
		--output $@ \
		--id > $(RECOVERYIMAGE_ID_FILE)
	$(hide) $(call assert-max-image-size,$@,$(BOARD_BOOTIMAGE_PARTITION_SIZE))
	@echo ----- Made recovery image: $@ --------
