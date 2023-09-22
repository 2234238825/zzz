//
// Created by Admin on 2023/8/31.
//
#include <cstdarg>
#include "../include/log.h"

namespace hhh
{

    const char* LogLevel::ToString(LogLevel::Level level)
    {
        switch (level)
        {
#define	XX(name)\
		case LogLevel::name:\
		return #name;\
		break;
            XX(UNKONW);
            XX(DEBUG);
            XX(INFO);
            XX(WARN);
            XX(ERROR);
            XX(FATAL);
#undef XX
            default:
                return "UNKNOW";
        }

    }
    LogEventWrap::LogEventWrap(LogEvent::ptr e):m_event(e){}

    LogEventWrap::~LogEventWrap()
    {
        m_event->getLoggr()->log(m_event->getLevel(),m_event);
    }

    stringstream &LogEventWrap::getSS() {
        return m_event->getSS();
    }


    class MessageFormatItem : public LogFormatter::FormatItem
    {
    public:
        explicit MessageFormatItem(const string& str = "") {}
        void format(ostream& os, shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getConent();
        }
    };

    class LevelFormatItem : public LogFormatter::FormatItem
    {
    public:
        explicit LevelFormatItem(const string& str = "") {}
        void format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << LogLevel::ToString(level);
        }
    };

    class ElapseFormatItem : public LogFormatter::FormatItem
    {
    public:
        explicit ElapseFormatItem(const string& str = "") {}
        void format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getElapse();
        }
    };


    class ThreadIdFormatItem : public LogFormatter::FormatItem
    {
    public:
        explicit ThreadIdFormatItem(const string& str = "") {}
        void format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getThreadId();
        }
    };

    class FiberIdFormatItem : public LogFormatter::FormatItem
    {
    public:
        explicit FiberIdFormatItem(const string& str = "") {}
        void format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getFiberId();
        }
    };

    class DateTimeFormatItem : public LogFormatter::FormatItem
    {
    private:
        string m_format;
    public:
        explicit DateTimeFormatItem(const string& format = "%Y:%m:%d %H:%M:%S") :m_format(format)
        {
            if(m_format.empty())
            {
                m_format = "%Y-%m-%d %H:%M:%S ";
            }
        }

        void format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            time_t time= event->getTime();
            tm *tm2 = localtime(&time);
            char buf[64];
            strftime(buf,sizeof(buf),m_format.c_str(),tm2);

            os<<buf;

        }
    };

    class FilenameFormatItem : public LogFormatter::FormatItem
    {
    public:
        explicit FilenameFormatItem(const string& str = "") {}
        void format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getFile();
        }
    };

    class LineFormatItem : public LogFormatter::FormatItem
    {
    public:
        LineFormatItem(const string& str = "") {}
        void format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << event->getLine();
        }
    };

    class NewLineFormatItem : public LogFormatter::FormatItem
    {
    public:
        NewLineFormatItem(const string& str = "") {}
        void format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
        {
            os << endl;
        }
    };

    class StringFormatItem:public LogFormatter::FormatItem
    {
    private:
        string m_string;
    public:

        StringFormatItem(const string&str):m_string(str){}
        void format(ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)override
        {
            os << m_string  ;
        }
    };

    LogEvent::LogEvent(shared_ptr<Logger> logger,LogLevel::Level level,const char *file, int32_t line, uint32_t elapse, uint32_t thread_id, uint32_t fiber_id,
                       uint64_t time):m_file(file),m_line(line),m_elapse(elapse),m_threadId(thread_id),
                       m_fiberId(fiber_id),m_time(time),m_logger(logger),m_level(level){

    }

    void LogEvent::format(const char *fmt, ...)
    {
        va_list al;
        va_start(al,fmt);
        format(fmt,al);
        va_end(al);
    }

    void LogEvent::format(const char *fmt, va_list al)
    {
        char*buf = nullptr;
      //  int len  =vsprintf(buf,fmt,al);
        buf="hello"; //这里的buf应该是fmt被解析的内容，然后传入m_ss里面；不想写了呜呜呜
        if(1)
        {
            m_ss<<string(buf,5);
           // free(buf);
        }
    }

    Logger::Logger(const string & name) :m_name(name),m_level(LogLevel::DEBUG)
    {
        m_formatter.reset(new LogFormatter("%d [%p] %m %t %F <%f:%l> %n"));
    }

    void Logger::addAppender(LogAppender::ptr appender)
    {
        if (!appender->getFormatter())
        {
            appender->setFormatter(m_formatter);
        }
        m_appenders.push_back(appender);
    }


    void Logger::delAppender(LogAppender::ptr appender)
    {
        for (auto it = m_appenders.begin();
             it != m_appenders.end();it++)
        {
            if (*it == appender)
            {
                m_appenders.erase(it);
                break;
            }
        }
    }

    void Logger::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            auto self = shared_from_this();
            for (auto& i : m_appenders)
            {

                i->log(self, level, event);
            }
        }
    }



    FileLogAppender::FileLogAppender(const string& filename):m_filename(filename){}

    void FileLogAppender::log(shared_ptr<Logger>logger,LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            m_filestream << m_formatter->format(logger, level, event);
        }
    }

    bool FileLogAppender::reopen()
    {
        if (m_filestream)
        {
            m_filestream.close();
        }
        m_filestream.open(m_filename);
        return !m_filestream;
    }

    void StdoutLogAppender::log(shared_ptr<Logger>logger, LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            cout << m_formatter->format(logger,level,event);
        }
    }

    LogFormatter::LogFormatter(const string& pattern):m_pattern(pattern)
    {
        init();
    }

    string LogFormatter::format(shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
    {
        stringstream ss;
        for (auto& i : m_items)
        {
            i->format(ss, logger,level,event);
        }

        return ss.str();
    }

    void LogFormatter::init() {
        //m_pattern = "%d [%p] <%f:%l> %n"
        vector<tuple<string, string, int> > vec;
        string s;
        for(int i = 0;i < m_pattern.size();i++)
        {
            if(m_pattern[i] != '%')
            {
                s.append(1,m_pattern[i]);
                continue;
            }
            else
            {
                if(!s.empty())
                {
                    vec.emplace_back(make_tuple(s,"",0));
                    s.clear();
                }

                vec.emplace_back(make_tuple(m_pattern.substr(i+1,1),"",1));
                i++;
            }
        }

#define XX(str, C) {#str, [](const string& fmt ) { return shared_ptr<FormatItem>(new C(fmt));}}
        static map<string, function<shared_ptr<FormatItem>(const string& str)> > s_format_items = //这一段应该是智能指针重载了()
        {
                {"m",[](const string& fmt ) { return shared_ptr<FormatItem>(new MessageFormatItem(fmt));}},
                XX(p, LevelFormatItem),             //p:日志级别
                XX(r, ElapseFormatItem),            //r:累计毫秒数
                //		XX(c, NameFormatItem),              //c:日志名称
                XX(t, ThreadIdFormatItem),          //t:线程id
                XX(n, NewLineFormatItem),           //n:换行
                XX(d, DateTimeFormatItem),          //d:时间
                XX(f, FilenameFormatItem),          //f:文件名
                XX(l, LineFormatItem),              //l:行号
                //		XX(T, TabFormatItem),               //T:Tab
                XX(F, FiberIdFormatItem),           //F:协程id
                //		XX(N, ThreadNameFormatItem),        //N:线程名称
#undef XX
         };

        for (auto& i : vec)
        {
            if (get<2>(i) == 0) //如果是0就把第一个数据元素按照字符串格式放入m_items;
            {
                m_items.push_back(FormatItem::ptr(new StringFormatItem(get<0>(i))));
            }
            else   //否则就按照格式化放入；
            {
               // cout<<"debug: "<<get<0>(i)<<endl;
                auto it = s_format_items.find(get<0>(i));
                if (it == s_format_items.end())  //没有找到这个字符就输出格式解析错误
                {
                    m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + get<0>(i) + ">>")));
                    m_error = true;
                }
                else
                {

                    m_items.push_back(it->second(get<1>(i)));
                }
            }

        //    cout << "(" << get<0>(i) << ") - (" << get<1>(i) << ") - (" << get<2>(i) << ")" << endl;
        }
       // cout << m_items.size() << endl;
    }


    LoggerManager::LoggerManager()
    {
        m_root.reset(new Logger);
        m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));

    }
    Logger::ptr LoggerManager::getLogger(const std::string &name)
    {
        auto it = m_logger.find(name);
        return it == m_logger.end()?m_root:it->second;
    }
}

//%m -- 消息体
//%p -- level
//%r -- 启动后的时间
//%c -- 日志名称
//%t -- 线程id
//%n -- 回车换行
//%d -- 时间
//%f -- 文件名
//%l -- 行号