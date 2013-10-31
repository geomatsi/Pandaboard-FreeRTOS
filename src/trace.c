/* trace feature for rpmsg/remoteproc */

#include <string.h>

#define ASCII_MASK 0x0000000F

/* trace buffer */

#define TRACE_BUF_SIZE 0x2800
#define TRACE_BUF_BASE 0x9F000000

char *trace_buf = (char *)TRACE_BUF_BASE;

__attribute__ ((section (".tracebuf")))
volatile char buf[TRACE_BUF_SIZE];

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
