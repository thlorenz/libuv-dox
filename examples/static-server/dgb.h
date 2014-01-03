#ifndef __dbh_h__
#define __dbh_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifdef NDEBUG
// compile with all debug messages removed
#define debug(M, ...)
#else
// debug("format", arg ..) => fprintf(stderr, "DEBUG %s:%d: format\n", file, line, arg ...)
#define debug(M, ...) fprintf(stderr, "\n\33[34mDEBUG\33[39m " M "\n   \33[90m at %s (%s:%d) \33[39m", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
#endif

// safe readable version of errno
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// log_* work just like debug, but cannot be compiled out
// more predefined macros here: http://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
#define log_err(M, ...) fprintf(stderr,  "\n\33[31mERR\33[39m  " M "\n  \33[90m at %s (%s:%d) \33[94merrno: %s\33[39m", ##__VA_ARGS__, __func__, __FILE__, __LINE__, clean_errno())

#define log_warn(M, ...) fprintf(stderr, "\n\33[91mWARN\33[39m " M "\n  \33[90m at %s (%s:%d) \33[94merrno: %s\33[39m", ##__VA_ARGS__, __func__, __FILE__, __LINE__, clean_errno())

#define log_info(M, ...) fprintf(stderr, "\n\33[32mINFO\33[39m " M "\33[90m at %s (%s:%d) \33[39m", ##__VA_ARGS__, __func__, __FILENAME__, __LINE__)

// enhanced assert? check that A is true, otherwise log error M then jump to error: for cleanup
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// placed in part of fn that shouldn't run, i.e. if/switch branches, prints error followed by jumping to error: cleanup
#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

// same as check, except that error only gets reported when ndebug flag is not set
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif
