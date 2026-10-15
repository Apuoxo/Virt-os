CC ?= gcc
OUT ?= out
LIMINE_VERSION ?= v8.7.0
LIMINE_DIR := $(OUT)/limine
ISO_ROOT := $(OUT)/iso-root
KERNEL := $(OUT)/aether-kernel.elf
ISO := $(OUT)/aether-c-kernel-x86_64.iso

.PHONY: all kernel iso clean

all: iso

$(OUT):
	mkdir -p $(OUT)

$(KERNEL): src/kernel.c linker.ld | $(OUT)
	$(CC) -std=c11 -O2 -Wall -Wextra -Werror \
		-ffreestanding -fno-stack-protector -fno-pic -fno-pie \
		-m64 -mno-red-zone -mcmodel=kernel -nostdlib \
		-Wl,-T,linker.ld -Wl,-z,max-page-size=0x1000 \
		-o $@ src/kernel.c

kernel: $(KERNEL)

$(LIMINE_DIR)/limine:
	git clone --depth=1 --branch $(LIMINE_VERSION) https://github.com/limine-bootloader/limine.git $(LIMINE_DIR)
	$(MAKE) -C $(LIMINE_DIR)

iso: $(KERNEL) $(LIMINE_DIR)/limine
	rm -rf $(ISO_ROOT)
	mkdir -p $(ISO_ROOT)/boot
	cp $(KERNEL) $(ISO_ROOT)/boot/kernel.elf
	cp limine.conf $(ISO_ROOT)/limine.conf
	cp $(LIMINE_DIR)/limine-bios.sys $(ISO_ROOT)/
	cp $(LIMINE_DIR)/limine-bios-cd.bin $(ISO_ROOT)/
	cp $(LIMINE_DIR)/limine-uefi-cd.bin $(ISO_ROOT)/
	xorriso -as mkisofs -b limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin -efi-boot-part --efi-boot-image \
		--protective-msdos-label $(ISO_ROOT) -o $(ISO)
	$(LIMINE_DIR)/limine bios-install $(ISO)

clean:
	rm -rf $(OUT)
