//
// Created by qc on 24-10-17.
//

#include "logger.h"
#include "Timestamp.h"
#include<iostream>
//获取日志唯一的示例对象
Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}

//设置日志级别
void Logger::setLogLevel(int Level)
{
   logLevel_ = Level;
}

//写日志 [级别信息] time : msg
void Logger::log(std::string msg)
{
    switch (logLevel_)
    {
        case INFO:
            std::cout<<"[INFO]"<<std::endl;
            break;
        case ERROR:
            std::cout<<"[ERROR]"<<std::endl;
            break;
        case FATAL:
            std::cout<<"[FATAL]"<<std::endl;
            break;
        case DEBUG:
            std::cout<<"[DEBUG]"<<std::endl;
            break;
        default:
            break;
    }
    //打印时间和msg
    std::cout<<Timestamp::now().toString()<<":"<<msg<<std::endl;
}
