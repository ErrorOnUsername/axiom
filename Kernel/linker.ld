ENTRY( _start )

SECTIONS
{
	. = 4M;
	_kernel_physical_start = .;
	.boot_text :
	{
		$<TARGET_OBJECTS:boot> ( .multiboot )
		$<TARGET_OBJECTS:boot> ( .text      )
	}

	.boot_rodata :
	{
		$<TARGET_OBJECTS:boot> ( .rodata    )
	}

	.boot_data :
	{
		$<TARGET_OBJECTS:boot> ( .data      )
	}

	.boot_bss :
	{
		$<TARGET_OBJECTS:boot> ( .bss       )
		$<TARGET_OBJECTS:boot> ( COMMON     )
	}

	. = ALIGN( 0x1000 );
	_boot_end = .;

	. += 0xffffffff80000000;
	_kernel_virtual_start = .;

	/*
	 * The weird bit math fuckery in the AT statement is
	 * just aligning it at 0x1000. We do this because it's
	 * placing it directly after the boot segments and we
	 * stil want the newer part to be aligned at 0x1000.
	 *
	 * We don't have to do it for the first one because
	 * _boot_end is set to a pre-aligned address.
	 *
	 * I'm sure there's a better way of doing this, but
	 * this is what I though of at the time.
	 *
	 * Have fun with this mess future me. ;)
	 */

	.text : AT( _boot_end )
	{
		* ( .multiboot )
		* ( .text      )
	}

	. = ALIGN( 0x1000 );

	.rodata : AT( ( LOADADDR( .text ) + SIZEOF( .text ) + 0xfff ) & 0xfffffffffffff000 )
	{
		* ( .rodata    )
	}

	. = ALIGN( 0x1000 );

	.data : AT( ( LOADADDR( .rodata ) + SIZEOF( .rodata ) + 0xfff ) & 0xfffffffffffff000 )
	{
		* ( .data      )
	}

	. = ALIGN( 0x1000 );

	.bss : AT( ( LOADADDR( .data ) + SIZEOF( .data ) + 0xfff ) & 0xfffffffffffff000 )
	{
		* ( COMMON     )
		* ( .bss       )
	}

	_kernel_virtual_end = .;
	_kernel_physical_end = ( LOADADDR( .bss ) + SIZEOF( .bss ) + 0xfff ) & 0xfffffffffffff000;
}
