ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = IM;
    .text:
    {
        *(.text)
    }

    .rodata:
    {
        *(.rodata)
    }

    .data:
    {
        *(.data)
    }

    .bss:
    {
        *(COMMON)
        *(.bss)
    }
}