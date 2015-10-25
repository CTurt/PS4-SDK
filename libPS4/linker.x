OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

PHDRS
{
   code_seg PT_LOAD;
   data_seg PT_LOAD;
}

SECTIONS
{
	/* Code segment */
	. = 0x926200000;
	.text : {
		*(.text.start)
		*(.text*)
	} : code_seg
	.rodata : { *(.rodata) *(.rodata*) } : code_seg

	/* Data segment */
	. = 0x926300000;
	.data : { *(.data) } : data_seg
	.bss  : { *(.bss) }  : data_seg
}
