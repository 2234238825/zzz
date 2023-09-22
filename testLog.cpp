//
// Created by Admin on 2023/9/11.
//
#include <iostream>
#include "include/log.h"
#include "include/util.h"
#include <cstdarg>
#include <stdio.h>
using namespace hhh;



int main(int argc, char** argv)
{


    auto l =Singleton<LoggerManager>::GetInstance()->getLogger("xx");
    HHH_LOG_INFO(l) << "xxx";

    return 0;
}