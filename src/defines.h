#ifndef DEFINES_H_GUARD
#define DEFINES_H_GUARD

#include <stddef.h>

#define TO_BOOL(x) (!!(x))

#ifndef NULL
#define NULL ((void *) 0) 
#endif /* NULL */

typedef long long		Long;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;
typedef unsigned long long	uLong;
typedef unsigned char		byte;
typedef signed char		sbyte;	
typedef char 			BOOL;


enum {FALSE = 0, TRUE = 1};


#endif /* DEFINES_H_GUARD */
