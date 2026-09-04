
MAKEFLAGS += --silent
QEMU_FLAGS ?=

.PHONY: build
build:
	cmake -B build -G Ninja
	cmake --build build

.PHONY: clean
clean:
	rm -rf build

.PHONY: qemu
qemu:
	qemu-system-x86_64 \
		-s $(QEMU_FLAGS) \
		-M q35,smm=off -smp 1 -m 2G \
		-serial mon:stdio \
		-drive format=raw,file=image/image.hdd

.PHONY: gdb
gdb:
	gdb build/kernel -x gdbinit-k

.PHONY: image
image: limine-binary build
	# Make EFI root.
	mkdir -p image/efiroot/EFI/BOOT/
	cp limine-binary/BOOTX64.EFI image/efiroot/EFI/BOOT/
	mkdir -p image/efiroot/boot/
	cp limine-binary/limine-bios.sys image/efiroot/boot/
	cp limine.conf image/efiroot/boot/
	cp build/kernel image/efiroot/boot/
	
	# Make EFI partition image.
	dd if=/dev/zero of=image/efi.bin bs=1M count=29
	mformat -i image/efi.bin
	mcopy -s -i image/efi.bin `find image/efiroot/ -mindepth 1 -maxdepth 1` ::/
	
	# Create partitions.
	dd if=/dev/zero of=image/image.hdd bs=1M count=32
	sgdisk -a 1 \
		--new=1:34:4129 --change-name=1:BIOS --typecode=1:0xef02 \
		--new=2:4130:-1 --change-name=2:'EFI PART' --typecode=2:0x0700 \
		image/image.hdd
	dd if=image/efi.bin of=image/image.hdd bs=512 seek=4130 count=$$[29*2048] conv=notrunc
	./limine-binary/limine bios-install image/image.hdd

limine-binary:
	wget https://github.com/Limine-Bootloader/Limine/releases/download/v12.7.0/limine-binary.tar.gz
	tar -zxf limine-binary.tar.gz
	$(MAKE) -C limine-binary
