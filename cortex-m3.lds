ENTRY(ResetISR)

MEMORY
{
    TEXT (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    DATA (rwx) : ORIGIN = 0x80000000, LENGTH = 1M
    TRACE (rwx) : ORIGIN = 0x9F000000, LENGTH = 10K
}

SECTIONS
{
	.text :
	{
	  KEEP(*(.isr_vector));
	  * (.text);
	  * (.rodata*);
	} > TEXT

	.data :
	{
      * (vtable)
	  * (.data*);
	} > DATA

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > DATA


	.resource_table :
	{
	  KEEP(*(.resource_table));
	} > DATA

    .tracebuf :
    {
        _tracebuf = .;
    } > TRACE
}
