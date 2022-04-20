all:
	@echo "  MK     Building bootloader"
	@make -C ./gnu-efi/lib > /dev/null
	@make -C ./gnu-efi/ > /dev/null
	@make -C ./gnu-efi/gnuefi > /dev/null
	@make -C ./gnu-efi/bootloader > /dev/null
	@cp gnu-efi/bootloader/main.efi gnu-efi/x86_64/bootloader
	@make -C ./kernel --no-print-directory -s
	@make -C ./kernel buildimg --no-print-directory -s

clean:
	@make -C ./gnu-efi/lib clean --no-print-directory -s
	@make -C ./gnu-efi/ clean --no-print-directory -s
	@make -C ./gnu-efi/gnuefi clean --no-print-directory -s
	@make -C ./gnu-efi/bootloader clean --no-print-directory -s
	@make -C ./kernel clean --no-print-directory -s

run:
	@make -C ./kernel run --no-print-directory -s