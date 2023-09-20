//
// Created by Admin on 2023/9/6.
//

#ifndef UNTITLED_UTIL_H
#define UNTITLED_UTIL_H
#ifdef _WIN32
#include<process.h>
#include<cstdint>
#else
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#endif


namespace hhh {
#ifdef _WIN32
    int GetThreadId();
    int GetFiberId();
#else
    pid_t GetThreadId();
    uint32_t GetFiberId();
#endif

} // hhh

#endif //UNTITLED_UTIL_H
