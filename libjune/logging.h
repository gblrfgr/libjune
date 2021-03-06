#ifndef LIBJUNE_LOGGING_H
#define LIBJUNE_LOGGING_H


#include "../libjune/silly.h"
#include "../libjune/debug.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
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
    FILE* out;
} lj_logger_t;

lj_string_t lji_logger_format(const char* format, const char* message, const struct tm* gmt, lj_loglevel_t level) {
    lj_string_t res = lj_str_new_empty();
    const char* cursor = format;
    while (*cursor != '\0') {
        if (*cursor == '$') {
            cursor++;
            switch (*cursor) {
                case 'M': 
                    lj_str_pushback(&res, '0' + (gmt->tm_mon + 1) / 10);
                    lj_str_pushback(&res, '0' + (gmt->tm_mon + 1) % 10);
                    break;
                case 'D':
                    lj_str_pushback(&res, '0' + (gmt->tm_mday + 1) / 10);
                    lj_str_pushback(&res, '0' + (gmt->tm_mday + 1) % 10);
                    break;
                case 'Y':
                    lj_str_pushback(&res, '0' + (gmt->tm_year + 1901) / 1000);
                    lj_str_pushback(&res, '0' + ((gmt->tm_year + 1901) / 100) % 10);
                    lj_str_pushback(&res, '0' + ((gmt->tm_year + 1901) / 10) % 10);
                    lj_str_pushback(&res, '0' + (gmt->tm_year + 1901) % 10);
                    break;
                case 'h':
                    lj_str_pushback(&res, '0' + (gmt->tm_hour + 1) / 10);
                    lj_str_pushback(&res, '0' + (gmt->tm_hour + 1) % 10);
                    break;
                case 'm':
                    lj_str_pushback(&res, '0' + (gmt->tm_min + 1) / 10);
                    lj_str_pushback(&res, '0' + (gmt->tm_min + 1) % 10);
                    break;
                case 's':
                    lj_str_pushback(&res, '0' + (gmt->tm_sec + 1) / 10);
                    lj_str_pushback(&res, '0' + (gmt->tm_sec + 1) % 10);
                    break;
                case 'l':
                    lj_str_pushstr(&res, LJI_LOGLEVEL_NAMES_K[level]);
                    break;
                case 'n':
                    lj_str_pushstr(&res, message);
                    break;
                case '$':
                    lj_str_pushback(&res, '$');
                    break;
                default:
                    LJ_ASSERT(false, "invalid format string for logger");
            }
        } else {
            lj_str_pushback(&res, *cursor);
        }
        cursor++;
    }
    return res;
}

void lj_log(lj_logger_t logger, lj_loglevel_t level, const char* message) {
    if (logger.loglevel > level) {
        return;
    }
    const time_t now = time(NULL);
    lj_string_t formatted = lji_logger_format(logger.format, message, gmtime(&now), level);
    fputs(formatted.begin, logger.out);
    fputs("\n", logger.out);
    fflush(logger.out);
    lj_str_destroy(&formatted);
}


#endif
