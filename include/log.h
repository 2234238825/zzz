//
// Created by Admin on 2023/8/31.
//

#ifndef _LOG_H
#define _LOG_H
#include<string>
#include<cstdint>

#include<iostream>
#include<list>
#include<fstream>
#include<vector>
#include <tuple>
#include <sstream>
#include<map>
#include <functional>
#include <ctime>
#include "singleton.h"

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getSS()
#define HHH_LOG_LEVEL(logger,level) \
    if(logger->getLevel()<=level)   \
         LogEventWrap(shared_ptr<LogEvent>(new LogEvent(logger,level,__FILE__,__LINE__,0,hhh::GetThreadId(),\
                hhh::GetFiberId(),time(0) ) ) ).getSS()
#define HHH_LOG_DEBUG(logger)  HHH_LOG_LEVEL(logger,hhh::LogLevel::DEBUG)
#define HHH_LOG_INFO(logger)  HHH_LOG_LEVEL(logger,hhh::LogLevel::INFO)
#define HHH_LOG_WARN(logger)  HHH_LOG_LEVEL(logger,hhh::LogLevel::WARN)
#define HHH_LOG_ERROR(logger)  HHH_LOG_LEVEL(logger,hhh::LogLevel::ERROR)
#define HHH_LOG_FATAL(logger)  HHH_LOG_LEVEL(logger,hhh::LogLevel::FATAL)
#define HHH_LOG_UNKONW(logger)  HHH_LOG_LEVEL(logger,hhh::LogLevel::UNKONW)

#define HHH_LOG_FMT_LEVEL(logger,level,fmt,...) \
    if(logger->getLevel()<=level)               \
            hhh::LogEventWrap(hhh::LogEvent::ptr(new hhh::LogEvent(logger,level,__FILE__,__LINE__,0,hhh::GetThreadId(),\
                hhh::GetFiberId(),time(0) ))).getEvent()->format(fmt,__VA_ARGS__)

#define HHH_LOG_FMT_DEBUG(logger,fmt,...) HHH_LOG_FMT_LEVEL(logger,hhh::LogLevel::DEBUG,fmt,__VA_ARGS__)
#define HHH_LOG_FMT_INFO(logger,fmt,...) HHH_LOG_FMT_LEVEL(logger,hhh::LogLevel::INFO,fmt,__VA_ARGS__)
#define HHH_LOG_FMT_WARN(logger,fmt,...) HHH_LOG_FMT_LEVEL(logger,hhh::LogLevel::WARN,fmt,__VA_ARGS__)
#define HHH_LOG_FMT_ERROR(logger,fmt,...) HHH_LOG_FMT_LEVEL(logger,hhh::LogLevel::ERROR,fmt,__VA_ARGS__)
#define HHH_LOG_FMT_FATAL(logger,fmt,...) HHH_LOG_FMT_LEVEL(logger,hhh::LogLevel::FATAL,fmt,__VA_ARGS__)

#define HHH_LOG_ROOT()hhh::LoggerMgr::GetInstance()->getRoot()

namespace hhh
{
    class Logger;
    class LogLevel
    {
    public:
        enum Level
        {
            UNKONW = 0,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };
        static const char* ToString(LogLevel::Level level);
    };

    class LogEvent
    {
    public:
        typedef	shared_ptr<LogEvent>ptr;
        LogEvent(shared_ptr<Logger>logger,LogLevel::Level level,
                  const char* file,int32_t line, uint32_t elapse,uint32_t thread_id,uint32_t fiber_id,uint64_t time) ;

        const char* getFile() const { return m_file; }
        int32_t getLine() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId;}
        uint32_t getFiberId() const { return m_fiberId; }
        uint64_t getTime() const { return m_time; }
        string getConent() const{ return m_ss.str(); }
        stringstream &getSS(){return m_ss;}

        void format(const char*fmt,...);
        void format(const char*fmt,va_list al);

        shared_ptr<Logger>getLoggr()const{return m_logger;}
        LogLevel::Level getLevel() const{return m_level;}
    private:
        const char* m_file = nullptr;
        int32_t m_line = 0  ;
        uint32_t m_elapse;
        uint32_t m_threadId = 0;
        uint32_t m_fiberId = 0;
        uint64_t m_time = 0;
        stringstream m_ss;
        shared_ptr<Logger> m_logger;
        LogLevel::Level m_level;
    };

    class LogEventWrap
    {
    public:
        ~LogEventWrap();

        LogEventWrap(LogEvent::ptr e);
        LogEvent::ptr getEvent() const { return m_event;}
        std::stringstream& getSS();
    private:
        LogEvent::ptr m_event;
    };

    class LogFormatter
    {
    public:
        typedef shared_ptr<LogFormatter> ptr;
        LogFormatter(const string& pattern);

        string format(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

    public:
        class FormatItem
        {
        public:

            typedef shared_ptr<FormatItem>ptr;
            FormatItem(const string& fmt = "") {}
            //virtual ~FormatItem() = 0 ;
            virtual void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };

        void init();
    private:
        string m_pattern;
        vector<FormatItem::ptr> m_items;
        bool m_error = false;
    };
/**
 * @日志器有一个级别，日志输出地也有一个级别
 */
    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender>ptr;

        //virtual ~LogAppender();
        virtual void log(std::shared_ptr<Logger>logger, LogLevel::Level level, LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
        LogFormatter::ptr getFormatter()const {return m_formatter;}
    protected:
        LogLevel::Level m_level = LogLevel::DEBUG;
        LogFormatter::ptr m_formatter;

    };
/**
 * @日志器有一个级别，日志输出地也有一个级别
 */
    class Logger : public enable_shared_from_this<Logger>
    {
    public:
        explicit Logger(const std::string& name = "root");
        typedef shared_ptr<Logger> ptr;
        void log(LogLevel::Level level, LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        LogLevel::Level getLevel()const { return m_level; }
        void setLevel(LogLevel::Level val) { m_level = val; }

        const string& getname()const { return m_name; }

    private:
        std::string m_name;
        LogLevel::Level m_level;
        list<LogAppender::ptr> m_appenders;
        LogFormatter::ptr m_formatter;
    };

    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender>ptr;
        void log(shared_ptr<Logger>logger, LogLevel::Level level, LogEvent::ptr event) override;
    };

    class FileLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<FileLogAppender>ptr;

        explicit FileLogAppender(const std::string& filename);
        bool reopen();
        void log(shared_ptr<Logger>logger, LogLevel::Level level, LogEvent::ptr event) override;
    private:
        string m_filename;
        ofstream out;
        ofstream m_filestream;

    };

    class LoggerManager
    {
    public:
        LoggerManager();
        Logger::ptr getLogger(const string &name);
        Logger::ptr getRoot()const{return m_root;}
    private:
        map<string ,Logger::ptr>m_logger;
        Logger::ptr m_root;
    };
    typedef Singleton<LoggerManager> LoggerMgr;

}

#endif //UNTITLED_LOG_H
