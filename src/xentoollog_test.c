
#include "xentoollog.h"
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/slab.h>
#include <linux/module.h>



static int __init run_test(void)
{
  xentoollog_logger *logger;

  /* Create the logger... */
  logger = (xentoollog_logger *)xtl_createlogger_printk();

  /* And run some simple tests. */
  xtl_log(logger, XTL_NOTICE, 0, "test", "Something happened! %d", 12);
  xtl_log(logger, XTL_CRITICAL, -EPERM, "test", "Something happened! %s", "hello");

  xtl_logger_destroy(logger);

  return -1;
}

module_init(run_test);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Quick test module for the xentoolog module.");
