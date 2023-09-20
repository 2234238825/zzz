//
// Created by Admin on 2023/9/6.
//

#include "../include/util.h"

namespace hhh {
#ifdef _WIN32
    int GetThreadId()  //返回windows中的pid，先不写，
    {
        return 121;
    }
    int GetFiberId()
    {
        return 123;
    }
#else
    pid_t GetThreadId()
    {
        return syscall(SYS_gettid);
    }

    pid_t GetFiberId()
    {
        return 0;
    }
#endif
} // hhh