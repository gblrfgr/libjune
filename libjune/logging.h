#ifndef LIBJUNE_LOGGING_H
#define LIBJUNE_LOGGING_H


#include "libjune/string.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

typedef enum {
    LJ_LOG_INFO,
    LJ_LOG_DEBUG,
    LJ_LOG_WARN,
    LJ_LOG_ERROR,
    LJ_LOG_FATAL,
} lj_loglevel_t;

const char* LJI_LOGLEVEL_NAMES_K[] = {
    "INFO",
    "DEBUG",
    "WARN",
    "ERROR",
    "FATAL",
};

typedef struct {
    const char* format;
    lj_loglevel_t loglevel;
    const FILE* out;
} lj_logger_t;

lj_logger_t lj_build_logger(const char* format, lj_loglevel_t loglevel, const FILE* out) {
    return (lj_logger_t) {
        .format = format,
        .loglevel = loglevel,
        .out = out,
    };
}

void lj_log(lj_logger_t logger, lj_loglevel_t level, const char* message) {
    if (logger.loglevel > level) {
        return;
    }
    fputs(lji_logger_format(logger.format, message, time.gmtime(), loglevel), logger.out);
}

// example format string: "[%M/%D/%Y %h:%m:%s %l]\t%s"
const char* lji_logger_format(const char* format, const char* message, const struct tm* gmt, lj_loglevel_t level) {
    lj_string_t res = lj_str_new_empty();
    char* cursor = format;
    while (*cursor != '\0') {
        if (*cursor == '\') {
            cursor++;
            assert(*cursor != '\0');
            lj_str_pushback(*cursor, &res);
        } else if (*cursor == '%') {
            cursor++;
            switch (*cursor) {
                case 'M': 
                    lj_str_pushback('0' + (gmt->tm_mon + 1) / 10, &res);
                    lj_str_pushback('0' + (gmt->tm_mon + 1) % 10, &res);
                    break;
                case 'D':
                    lj_str_pushback('0' + (gmt->tm_mday + 1) / 10, &res);
                    lj_str_pushback('0' + (gmt->tm_mday + 1) % 10, &res);
                    break;
                case 'Y':
                    lj_str_pushback('0' + (gmt->tm_year + 1) / 1000, &res);
                    lj_str_pushback('0' + ((gmt->tm_year + 1) / 100) % 10, &res);
                    lj_str_pushback('0' + ((gmt->tm_year + 1) / 10) % 10, &res);
                    lj_str_pushback('0' + (gmt->tm_year + 1) % 10, &res);
                    break;
                case 'h':
                    lj_str_pushback('0' + (gmt->tm_hour + 1) / 10, &res);
                    lj_str_pushback('0' + (gmt->tm_hour + 1) % 10, &res);
                    break;
                case 'm':
                    lj_str_pushback('0' + (gmt->tm_min + 1) / 10, &res);
                    lj_str_pushback('0' + (gmt->tm_min + 1) % 10, &res);
                    break;
                case 's':
                    lj_str_pushback('0' + (gmt->tm_sec + 1) / 10, &res);
                    lj_str_pushback('0' + (gmt->tm_sec + 1) % 10, &res);
                    break;
                case 'l':
                    lj_str_pushstr(LJI_LOGLEVEL_NAMES_K[level], &res);
                    break;
                case 's':
                    lj_str_pushstr(message, &res);
                    break;
                default:
                    assert(false);
        } else {
            lj_str_pushback(*cursor, &res);
        }
        cursor++;
    }
    return lj_str_cstr(res);
}


#endif
