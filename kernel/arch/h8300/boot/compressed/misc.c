/*
 * arch/h8300/boot/compressed/misc.c
 *
 * This is a collection of several routines from gzip-1.0.3
 * adapted for Linux.
 *
 * malloc by Hannu Savolainen 1993 and Matthias Urlichs 1994
 *
 * Adapted for h8300 by Yoshinori Sato 2006
 */

#include <asm/uaccess.h>

/*
 * gzip declarations
 */

#define OF(args)  args
#define STATIC static

#undef memset
#undef memcpy
#define memzero(s, n)     memset((s), (0), (n))

extern int _end;
static unsigned long free_mem_ptr;
static unsigned long free_mem_end_ptr;

extern char input_data[];
extern int input_len;
static unsigned char *output;

#define HEAP_SIZE             0x10000

#include "../../../../lib/decompress_inflate.c"

void *memset(void *s, int c, size_t n)
{
	int i;
	char *ss = (char *)s;

	for (i = 0; i < n; i++)
		ss[i] = c;
	return s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
	int i;
	char *d = (char *)dest, *s = (char *)src;

	for (i = 0; i < n; i++)
		d[i] = s[i];
	return dest;
}

static void error(char *x)
{

	while (1)
		;	/* Halt */
}

#define STACK_SIZE (4096)
long user_stack[STACK_SIZE];
long *stack_start = &user_stack[STACK_SIZE];

void decompress_kernel(void)
{
	free_mem_ptr = (unsigned long)&_end;
	free_mem_end_ptr = free_mem_ptr + HEAP_SIZE;

	decompress(input_data, input_len, NULL, NULL, output, NULL, error);
}