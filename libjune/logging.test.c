#include "../libjune/logging.h"
#include <stdio.h>

int main(const int argc, const char** argv) {
    const lj_logger_t def = (lj_logger_t) {
        .format = "[$M/$D/$Y $h:$m:$s $l]\t$n",
        .loglevel = LJ_LOG_FATAL,
        .out = stdout,
    };
    lj_log(def, LJ_LOG_DEBUG, "log level checking failure");
    lj_log(def, LJ_LOG_FATAL, "log level checking success (maybe)");

    lj_log(def, LJ_LOG_FATAL, "this should just. work.\n");
    lj_log(def, LJ_LOG_FATAL, "the next one should be an assertion failure\n");

    const lj_logger_t alt = (lj_logger_t) {
        .format = "[$M/$D/$Y $h:$m:$s $l]\t$n $q ",
        .loglevel = LJ_LOG_INFO,
        .out = stdout,
    };
    lj_log(alt, LJ_LOG_FATAL, "format specifier checking failure");

    return 0;
}