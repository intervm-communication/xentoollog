#ifndef __XENTOOLLOG_OSDEP_H__
#define __XENTOOLLOG_OSDEP_H__

#include <linux/kernel.h>
#include <linux/bug.h>
#include <linux/errno.h>

/*
 * For these files only, alias kernel-space constructs to their userspace
 * equivalents. In the end, it might be better to replace these with 
 * e.g. osdep_assert and osdep_alloc, or equivalent.
 */
#define assert(condition) BUG_ON(!(condition))
#define malloc(size) kzalloc(size, GFP_KERNEL)

/*
 * Code from the xtl core attempts to save errno; breaking compiling in
 * kernel space. We don't need to save errno, but for compatibility we'll
 * declare a dummy variable for it to save.
 */
#define UNUSED __attribute__ ((unused))
static int errno UNUSED = -ENOMEM;


typedef struct xentoollog_logger_printk  xentoollog_logger_printk;

xentoollog_logger_printk *xtl_createlogger_printk(void);


#endif
