#ifndef HOOK_H
#define HOOK_H
#include <Android/log.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#define LOG_TAG "Test"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args);
void remove_hook(void *orig_func);
void inline_hook(void *orig_func, void *hook_func);
void  test() __attribute__((constructor));
#endif