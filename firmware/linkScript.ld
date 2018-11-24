OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(startup)
SEARCH_DIR("./stdlib");

MEMORY
{
  ROM  (rx) : ORIGIN = 0x08000000, LENGTH = 64k
  SRAM (rw) : ORIGIN = 0x20000000, LENGTH = 8k
}

SECTIONS
{
  . = ALIGN(4);
  .vectab : ALIGN(4) { support.o(.vectab) }>ROM
  . = ALIGN(4);
  .text : ALIGN(4) { *(.text*) }>ROM
  . = ALIGN(4);
  .rodata : ALIGN(4) { *(.rodata*) }>ROM
  . = ALIGN(4);
  __text_end__ = .;

  . = ALIGN(4);
  .data : ALIGN(4)
  {
  __data_start__ = .;
  *(.data)
  __data_end__ = .;
  }>SRAM AT>ROM

  . = ALIGN(4); 
  __bss_start__ = .;
  .bss : ALIGN(4) { *(.bss) }>SRAM AT>SRAM
  __bss_end__ = .;

  end = .;
}
