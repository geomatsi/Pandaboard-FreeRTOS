/* trace feature for rpmsg/remoteproc */

#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#define ASCII_MASK 0x0000000F

/* trace buffer */

#define TRACE_BUF_SIZE 0x2800
#define TRACE_BUF_BASE 0x9F000000

char *trace_buf = (char *)TRACE_BUF_BASE;

__attribute__ ((section (".tracebuf")))
volatile char buf[TRACE_BUF_SIZE];

/* */

static int printchar(int c)
{
    int pc = 0;

	if (((unsigned int) trace_buf) >= (TRACE_BUF_BASE + TRACE_BUF_SIZE))
		trace_buf = (char *)TRACE_BUF_BASE;

    *trace_buf++ = c;
    pc += 1;

    return pc;
}

static int printstr(const char *string)
{
	int pc = 0;

	for ( ; *string ; ++string) {
		pc += printchar(*string);
	}

	return pc;
}

static int printint(int i, int b, int sign, int letbase)
{
	uint32_t u, mu;
	int t;

	int pc = 0;

	if (i == 0) {
		pc += printchar('0');
		goto done;
	}

	if (sign && b == 10 && i < 0) {
		pc += printchar('-');
		u = -i;
	} else {
		u = i;
	}

	mu = 1;

	while (1) {
		if (u/mu < b)
			break;

		mu *= b;
	}

	while (mu) {
		t = u / mu;

		if( t >= 10 )
			t += letbase - '0' - 10;
		pc += printchar(t + '0');

		u = u % mu;
		mu /= b;
	}

done:
	return pc;
}

static int printvarg(int *varg)
{
	char scr[2];

	char *format = (char *)(*varg++);
	int pc = 0;

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			if (*format == '\0')
				break;

			if (*format == '%')
				goto symbol;

			if( *format == 's' ) {
				char *s = *((char **)varg++);
				pc += printstr(s ? s : "(null)");
				continue;
			}

			if( *format == 'd' ) {
				pc += printint(*varg++, 10, 1, 'a');
				continue;
			}

			if( *format == 'x' ) {
				pc += printint(*varg++, 16, 0, 'a');
				continue;
			}

			if( *format == 'X' ) {
				pc += printint(*varg++, 16, 0, 'A');
				continue;
			}

			if( *format == 'u' ) {
				pc += printint(*varg++, 10, 0, 'a');
				continue;
			}

			if( *format == 'c' ) {
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += printstr(scr);
				continue;
			}
		} else {
		symbol:
			pc += printchar(*format);
		}
	}

	return pc;
}

/* */

void trace_printf(char *msg)
{
	/* TODO: for now, trace will reset when size exceed the buffer size.
	 * we need to keep the history as it is done in bios. */

	unsigned int len;
	len = strlen(msg);

	if ((len + (unsigned int)trace_buf) > (TRACE_BUF_BASE + TRACE_BUF_SIZE))
		trace_buf = (char *)TRACE_BUF_BASE;

	strncpy(trace_buf, msg, len);
	trace_buf += len;
}

void trace_printf_len(char *msg, int len)
{
	/* TODO: for now, trace will reset when size exceed the buffer size.
	 * we need to keep the history as it is done in bios. */

	if ((len + (unsigned int)trace_buf) > (TRACE_BUF_BASE + TRACE_BUF_SIZE))
		trace_buf = (char *)TRACE_BUF_BASE;

	strncpy(trace_buf, msg, len);
	trace_buf += len;
}

void trace_value(unsigned int register_value)
{
	int i,reg;
	char hexa_char;
	char string[10] = "0x00000000";

	reg = register_value;

	for ( i=9 ; i>1 ; i--) {
		hexa_char = (char)(reg & ASCII_MASK);
		if (hexa_char < 10)
			string[i] = hexa_char + 48 ;
		else
			string[i] = hexa_char + 55;
		reg = reg >> 4 ;
	}

	trace_printf(string);
	trace_printf("\n");
}

int trace_append(const char *format, ...)
{
	int *varg = (int *)(&format);

	return printvarg(varg);
}
