#include <stdarg.h>

#include <AXUtil/Types.h>
#include <Kernel/IO.h>

static void kpf_color_enable()
{
	IO::out8(IO::QEMU_SERIAL_PORT, 0x1b);
	IO::out8(IO::QEMU_SERIAL_PORT, '[');
	IO::out8(IO::QEMU_SERIAL_PORT, '1');
	IO::out8(IO::QEMU_SERIAL_PORT, ';');
	IO::out8(IO::QEMU_SERIAL_PORT, '3');
	IO::out8(IO::QEMU_SERIAL_PORT, '4');
	IO::out8(IO::QEMU_SERIAL_PORT, 'm');
}

static void kpf_color_disable()
{
	IO::out8(IO::QEMU_SERIAL_PORT, 0x1b);
	IO::out8(IO::QEMU_SERIAL_PORT, '[');
	IO::out8(IO::QEMU_SERIAL_PORT, '0');
	IO::out8(IO::QEMU_SERIAL_PORT, 'm');
}

extern "C"
{

static void dbg_putchar(char c)
{
	IO::out8(IO::QEMU_SERIAL_PORT, c);
}

/*
* FIXME: Actually implement this shit.
*
* For more information on these specifiers,
* read the printf(3) man pages. Link:
* https://man7.org/linux/man-pages/man3/printf.3.html
*/
int k_printf(const char* fmt, ...)
{
	constexpr const char* lowercase_hex_values = "0123456789abcdef";
	constexpr const char* uppercase_hex_values = "0123456789ABCDEF";

	kpf_color_enable();
	size_t idx = 0;
	char c = ':';
	va_list args;
	va_start(args, fmt);
	while(c != '\0')
	{
		c = fmt[idx];
		switch(c)
		{
			case '%':
			{
				c = fmt[++idx];
				switch(c)
				{
					/* int type */
					case 'd':
					case 'i':
						break;

					/* unsigned int type */
					case 'u':
						/* decimal notation */
					case 'o':
						/* octal notation */
						break;
					case 'x':
					{
						/* hexadecimal (abcdef) */
						if(fmt[idx + 1] == 'l')
						{
							c = fmt[++idx];

							uint64_t value = va_arg(args, uint64_t);
							uint8_t place = 60;
							dbg_putchar('0');
							dbg_putchar('x');

							while(place != 0)
							{
								uint8_t nibble = (value & (0xf << place)) >> place;
								dbg_putchar(lowercase_hex_values[nibble]);
								place -= 4;
							}
						}
						else
						{
							uint32_t value = va_arg(args, uint32_t);
							uint8_t place = 28;
							dbg_putchar('0');
							dbg_putchar('x');

							while(place != 0)
							{
								uint8_t nibble = (value & (0xf << place)) >> place;
								dbg_putchar(lowercase_hex_values[nibble]);
								place -= 4;
							}
						}
						break;
					}
					case 'X':
					{
						/* hexadecimal (ABCDEF) */
						if(fmt[idx + 1] == 'l')
						{
							c = fmt[++idx];

							uint64_t value = va_arg(args, uint64_t);
							uint8_t place = 60;
							dbg_putchar('0');
							dbg_putchar('X');

							while(place != 0)
							{
								uint8_t nibble = (value & (0xf << place)) >> place;
								dbg_putchar(uppercase_hex_values[nibble]);
								place -= 4;
							}
						}
						else
						{
							uint32_t value = va_arg(args, uint32_t);
							uint8_t place = 28;
							dbg_putchar('0');
							dbg_putchar('X');

							while(place != 0)
							{
								uint8_t nibble = (value & (0xf << place)) >> place;
								dbg_putchar(uppercase_hex_values[nibble]);
								place -= 4;
							}
						}
						break;
					}

					/* double (e notation) */
					case 'e':
					case 'E':
						/* uses capital 'E' rather than 'e' */
						break;

					/*
					* double (traditional decimal notation)
					* uses dot specifier (.) for precision
					* default precision is 6
					*/
					case 'f':
						/* "[-]inf", "[-]infinity", or "nan" */
					case 'F':
						/* "[-]INF", "[-]INFINITY", or "NAN" */
						break;

					/* double (decimal or e notation) */
					case 'g':
						/* f or e */
					case 'G':
						/* F or E */
						break;

					/* double (hexadecimal notation) */
					case 'a':
						/* 0x and abcdef */
					case 'A':
						/* 0X and ABCDEF */
						break;

					/*
					* no l modifier: unsigned char type
					* l modifier: wint_t
					*/
					case 'c':
						break;

					/*
					* no l modifier: const char* type
					* l modifier: const wint_t* type
					*/
					case 's':
						break;

					/* void* type */
					case 'p':
					{
						void* argument = va_arg(args, void*);
						/* FIXME: If we ever end up supporting non-64-bit architectures, this wont work */
						uint64_t value = (uint64_t)argument;
						uint8_t place = 60;
						dbg_putchar('0');
						dbg_putchar('x');

						while(place != 0)
						{
							uint8_t nibble = (value & (0xf << place)) >> place;
							dbg_putchar(lowercase_hex_values[nibble]);
							place -= 4;
						}
						break;
					}
				}
				idx++;
				break;
			}
			default:
				dbg_putchar(c);
		}
		idx++;
	}
	va_end(args);
	kpf_color_disable();
	return 0;
}

}
