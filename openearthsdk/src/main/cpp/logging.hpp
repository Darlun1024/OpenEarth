//
// Created by GXSN-Pro on 2017/11/18.
//

#ifndef OPENEARTH_LOGGING_HPP
#define OPENEARTH_LOGGING_HPP

#include <android/log.h>
#define LOGD(TAG,...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(TAG,...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(TAG,...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(TAG,...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(TAG,...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


#endif //OPENEARTH_LOGGING_HPP
