/*
 * xtl_logger_printk.c
 *
 * log message consumer that writes to the kernel ring buffer
 *
 * Copyright (c) 2010 Citrix
 * Copyright (c) 2016 Assured Information Security, Inc.
 * Part of a generic logging interface used by various dom0 userland libraries.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; If not, see <http://www.gnu.org/licenses/>.
 */

#include "xentoollog.h"
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/errno.h>


/**
 * The maximum length of a line to be added to the kernel ring buffer; used to
 * estimate the size of a message buffer.
 */
#ifndef LOG_LINE_MAX
#define LOG_LINE_MAX (1024 - 32)
#endif

/**
 * Core structure for our printk logger.
 */
struct xentoollog_logger_printk {
    xentoollog_logger vtable;
};


/**
 * Translates a XTL loglevel into a kernel loglevel prefix; allowing XTL
 * log-levels to be generated at roughly equivalent kernel log levels.
 *
 * @param level The XTL loglevel to be translated.
 * @param The prefix for the kernel printk.
 */
static const char * xtl_level_to_printk_prefix(xentoollog_level level)
{
    switch(level) {
      case XTL_CRITICAL:
        return KERN_CRIT;
      case XTL_ERROR:
        return KERN_ERR;
      case XTL_WARN:
        return KERN_WARNING;
      case XTL_NOTICE:
        return KERN_NOTICE;
      case XTL_INFO:
      case XTL_PROGRESS:
      case XTL_DETAIL:
        return KERN_INFO;
      case XTL_VERBOSE:
      case XTL_DEBUG:
        return KERN_DEBUG;
      default:
        return KERN_DEFAULT;
    }

    return KERN_DEFAULT;
}

static void printk_vmessage(xentoollog_logger *logger_in,
                                 xentoollog_level level,
                                 int errnoval,
                                 const char *context,
                                 const char *format,
                                 va_list al) {
    char errno_string[16];

    /*
     * We'll need to ensure that we printk() the formatted all message at once--
     * otherwise our message can be interrupted if another kernel process is
     * scheduled onto this CPU. Thus, we'll creatae the formatted string,
     * and then print it with any prefix/suffix attached.
     */
    char *printk_buffer = kmalloc(LOG_LINE_MAX, GFP_KERNEL);
    if(!printk_buffer) {
      printk("WARNING: xentoollog message dropped; insufficient memory!\n");
      return;
    }

    /* Generate the main formatted log message... */
    vsnprintf(printk_buffer, LOG_LINE_MAX, format, al);

    /* If applicable, generate a errno suffix. */
    if(errnoval != 0)
        snprintf(errno_string, sizeof(errno_string), " (%d)", errnoval);
    else
        errno_string[0] = '\0';

    /* ... and print it out wrapped with the provided extra information. */
    printk("%s%s%s%s: %s%s",
        xtl_level_to_printk_prefix(level),
        context ? context : "",
        context ? ": " : "",
        xtl_level_to_string(level),
        printk_buffer,
        errno_string);
}

static void printk_progress(struct xentoollog_logger *logger_in,
                                 const char *context,
                                 const char *doing_what, int percent,
                                 unsigned long done, unsigned long total) {
  /* There's no point printing progress to the kernel buffer, which is
   * inherently non-interactive. */
}

static void printk_destroy(struct xentoollog_logger *logger_in) {
    xentoollog_logger_printk *lg = (void*)logger_in;
    kfree(lg);
}

xentoollog_logger_printk *xtl_createlogger_printk() {
    xentoollog_logger_printk newlogger;
    return XTL_NEW_LOGGER(printk, newlogger);
}

EXPORT_SYMBOL(xtl_createlogger_printk);
EXPORT_SYMBOL(xtl_logger_destroy);
EXPORT_SYMBOL(xtl_logv);
EXPORT_SYMBOL(xtl_log);
EXPORT_SYMBOL(xtl_progress);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Xen Logging facilities for the Kernel");

/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
