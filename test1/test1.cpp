//
// Created by Admin on 2023/9/21.
//
//
// Created by Admin on 2023/9/11.
//
#include <iostream>
#include "../include/log.h"
#include "../include/util.h"
#include <cstdarg>
#include <stdio.h>
using namespace hhh;

int main()
{
    shared_ptr<LogFormatter> fmt(new LogFormatter("%m  heiheihei %d")) ;
    LogAppender::ptr a(new StdoutLogAppender());
    Logger::ptr logger(new Logger());   //在new logger的时候把格式化都加进去了
    logger->addAppender(a);
    logger->setLevel(LogLevel::DEBUG);                     //设定日志器的级别
    a->setFormatter(fmt);


LogEventWrap(shared_ptr<LogEvent>(new LogEvent(logger,LogLevel::DEBUG,__FILE__,__LINE__,0,hhh::GetThreadId(),
                hhh::GetFiberId(),time(0) ) ) ).getSS()<<"(#How i love you#)";


    // LogFormatter::ptr fmt(new LogFormatter("%d%T%m%n"));
   HHH_LOG_FMT_DEBUG(logger,"%s fmt : %s ","dadad","dopppppp");
    return 0;


}
