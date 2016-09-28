#ifndef __XENTOOLLOG_OSDEP_H__
#define __XENTOOLLOG_OSDEP_H__

#include <stdio.h>
#include <stdarg.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>

#define XTL_STDIOSTREAM_SHOW_PID            001u
#define XTL_STDIOSTREAM_SHOW_DATE           002u
#define XTL_STDIOSTREAM_HIDE_PROGRESS       004u
#define XTL_STDIOSTREAM_PROGRESS_USE_CR     010u /* default is to */
#define XTL_STDIOSTREAM_PROGRESS_NO_CR      020u /* use \r to ttys */

struct xentoollog_logger;
enum xentoolog_level;

typedef enum xentoollog_level xentoollog_level;
typedef struct xentoollog_logger_stdiostream  xentoollog_logger_stdiostream;

xentoollog_logger_stdiostream *xtl_createlogger_stdiostream
        (FILE *f, xentoollog_level min_level, unsigned flags);
    /* may return 0 if malloc fails, in which case error was logged */
    /* destroy on this logger does not close the file */

void xtl_stdiostream_set_minlevel(xentoollog_logger_stdiostream*,
                                  xentoollog_level min_level);
void xtl_stdiostream_adjust_flags(xentoollog_logger_stdiostream*,
                                  unsigned set_flags, unsigned clear_flags);
  /* if set_flags and clear_flags overlap, set_flags takes precedence */

#endif
