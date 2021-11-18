#include <stdarg.h>

#include <AXUtil/Types.h>
#include <Kernel/IO.h>

namespace Kernel {

static void kpf_color_enable()
{
	IO::out8(IO::QEMU_SERIAL_PORT, 0x1b);
	IO::out8(IO::QEMU_SERIAL_PORT, '[');
	IO::out8(IO::QEMU_SERIAL_PORT, '1');
	IO::out8(IO::QEMU_SERIAL_PORT, ';');
	IO::out8(IO::QEMU_SERIAL_PORT, '3');
	IO::out8(IO::QEMU_SERIAL_PORT, '1');
	IO::out8(IO::QEMU_SERIAL_PORT, 'm');
}

static void kpf_color_disable()
{
	IO::out8(IO::QEMU_SERIAL_PORT, 0x1b);
	IO::out8(IO::QEMU_SERIAL_PORT, '[');
	IO::out8(IO::QEMU_SERIAL_PORT, '0');
	IO::out8(IO::QEMU_SERIAL_PORT, 'm');
}

extern "C" {

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
	while(c != '\0') {
		c = fmt[idx];
		switch(c) {
			case '%': {
				c = fmt[++idx];
				switch(c) {
					/* int type */
					case 'i':
					case 'd': {
						if(fmt[idx + 1] == 'l') {
							idx++;

							int64_t value = va_arg(args, int64_t);

							if(value < 0) {
								dbg_putchar('-');
								value *= -1;
							}

							int64_t tmp_value = value / 10;
							uint64_t place = 1;
							while(tmp_value != 0) {
								place *= 10;
								tmp_value /= 10;
							}

							uint8_t current_digit = 0;
							while(place != 0) {
								current_digit = value / place;
								dbg_putchar('0' + current_digit);
								value %= place;
								place /= 10;
							}
						} else {
							int32_t value = va_arg(args, int32_t);

							if(value < 0) {
								dbg_putchar('-');
								value *= -1;
							}

							int32_t tmp_value = value / 10;
							uint32_t place = 1;
							while(tmp_value != 0) {
								place *= 10;
								tmp_value /= 10;
							}

							uint8_t current_digit = 0;
							while(place != 0) {
								current_digit = value / place;
								dbg_putchar('0' + current_digit);
								value %= place;
								place /= 10;
							}
						}

						break;
					}

					/* unsigned int type */
					case 'u': {
						c = fmt[idx + 1];
						if(c == 'l') {
							idx++;

							uint64_t value = va_arg(args, uint64_t);
							uint64_t tmp_value = value / 10;
							uint64_t place = 1;

							while(tmp_value != 0) {
								place *= 10;
								tmp_value /= 10;
							}

							uint8_t current_digit = 0;
							while(place != 0) {
								current_digit = value / place;
								dbg_putchar('0' + current_digit);
								value %= place;
								place /= 10;
							}
						} else if(c == 's') {
							idx++;

							size_t value = va_arg(args, uint64_t);
							size_t tmp_value = value / 10;
							uint64_t place = 1;

							while(tmp_value != 0) {
								place *= 10;
								tmp_value /= 10;
							}

							uint8_t current_digit = 0;
							while(place != 0) {
								current_digit = value / place;
								dbg_putchar('0' + current_digit);
								value %= place;
								place /= 10;
							}
						} else {
							uint32_t value = va_arg(args, uint32_t);
							uint32_t tmp_value = value / 10;
							uint64_t place = 1;

							while(tmp_value != 0) {
								place *= 10;
								tmp_value /= 10;
							}

							uint8_t current_digit = 0;
							while(place != 0) {
								current_digit = value / place;
								dbg_putchar('0' + current_digit);
								value %= place;
								place /= 10;
							}
						}
						break;
					}
					case 'o':
						/* octal notation */
						break;
					case 'x': {
						/* hexadecimal (abcdef) */
						if(fmt[idx + 1] == 'l') {
							c = fmt[++idx];

							uint64_t value = va_arg(args, uint64_t);
							int8_t place = 60;
							dbg_putchar('0');
							dbg_putchar('x');

							while(place >= 0) {
								uint8_t nibble = ((value >> place) & 0xf);
								dbg_putchar(lowercase_hex_values[nibble]);
								place -= 4;
							}
						} else {
							uint32_t value = va_arg(args, uint32_t);
							int8_t place = 28;
							dbg_putchar('0');
							dbg_putchar('x');

							while(place >= 0) {
								uint8_t nibble = ((value >> place) & 0xf);
								dbg_putchar(lowercase_hex_values[nibble]);
								place -= 4;
							}
						}
						break;
					}
					case 'X': {
						/* hexadecimal (ABCDEF) */
						if(fmt[idx + 1] == 'l') {
							c = fmt[++idx];

							uint64_t value = va_arg(args, uint64_t);
							int8_t place = 60;
							dbg_putchar('0');
							dbg_putchar('X');

							while(place >= 0) {
								uint8_t nibble = ((value >> place) & 0xf);
								dbg_putchar(uppercase_hex_values[nibble]);
								place -= 4;
							}
						} else {
							uint32_t value = va_arg(args, uint32_t);
							int8_t place = 28;
							dbg_putchar('0');
							dbg_putchar('X');

							while(place >= 0) {
								uint8_t nibble = ((value >> place) & 0xf);
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
					case 's': {
						const char* argument = va_arg(args, const char*);
						size_t index = 0;
						char current = argument[index];
						while(current != '\0') {
							dbg_putchar(current);
							current = argument[++index];
						}
						break;
					}

					/* void* type */
					case 'p': {
						void* argument = va_arg(args, void*);
						/* FIXME: If we ever end up supporting non-64-bit architectures, this wont work */
						uint64_t value = (uint64_t)argument;
						int8_t place = 60;
						dbg_putchar('0');
						dbg_putchar('x');

						while(place >= 0) {
							uint8_t nibble = ((value >> place) & 0xf);
							dbg_putchar(lowercase_hex_values[nibble]);
							place -= 4;
						}
						break;
					}

					default:
						dbg_putchar('%');
				}
				idx++;
				break;
			}
			default:
				dbg_putchar(c);
				idx++;
		}
	}
	va_end(args);
	kpf_color_disable();
	return 0;
}

}

}
