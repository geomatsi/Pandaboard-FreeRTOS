#ifndef __TRACE_H__
#define __TRACE_H__

void trace_printf(char *msg);
void trace_value(unsigned int register_value);
int trace_append(const char *format, ...);

#endif /* __TRACE_H__ */
