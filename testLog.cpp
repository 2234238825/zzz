//
// Created by Admin on 2023/9/11.
//
#include <iostream>
#include "include/log.h"
#include "include/util.h"
#include <cstdarg>
#include <stdio.h>
using namespace hhh;

/*
int main()
{
    shared_ptr<LogFormatter> fmt(new LogFormatter("%m  heiheihei %d")) ;
    LogAppender::ptr a(new StdoutLogAppender());
    Logger::ptr logger(new Logger());   //在new logger的时候把格式化都加进去了
    logger->addAppender(a);
    logger->setLevel(LogLevel::DEBUG);                     //设定日志器的级别
    a->setFormatter(fmt);


    */
/*LogEventWrap(shared_ptr<LogEvent>(new LogEvent(logger,LogLevel::DEBUG,__FILE__,__LINE__,0,hhh::GetThreadId(),
                hhh::GetFiberId(),time(0) ) ) ).getSS()<<"(#How i love you#)";
*//*

    // LogFormatter::ptr fmt(new LogFormatter("%d%T%m%n"));
   HHH_LOG_FMT_DEBUG(logger,"%s fmt : %s ","dadad","dopppppp");
    return 0;


}
*/

int main(int argc, char** argv)
{
    hhh::Logger::ptr logger(new Logger);
    logger->addAppender(LogAppender::ptr(new StdoutLogAppender));

    FileLogAppender::ptr file_appender(new ::FileLogAppender("./log.txt"));
    LogFormatter::ptr fmt(new ::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    // file_appender->setLevel(LogLevel::ERROR);

    logger->addAppender(file_appender);


    HHH_LOG_INFO(logger) << "test macro";
    HHH_LOG_ERROR(logger) << "test macro error";
    HHH_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");
    auto l =Singleton<LoggerManager>::GetInstance()->getLogger("xx");
    HHH_LOG_INFO(l) << "xxx";

    return 0;
}