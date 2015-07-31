#include "logger.hpp"

#include <iostream>
#include <chrono>
#include <memory>

#include <windows.h>

logger::logger(LogLevel level, bool writeToCout, bool isUnbuffered):
    mLevel(level),
    mWriteToCout(writeToCout),
    mIsUnbuffered(isUnbuffered)
{
}

logger::~logger()
{
    std::lock_guard<std::mutex> lock(mLock);
    if(mWriteToCout)
    {
        std::cout.flush();
    }

    if(mFileStream)
    {
        mFileStream.flush();
    }
}

void logger::setFile(const std::string& filename, bool truncate)
{
    if(mLevel <= logDISABLE)
    {
        return;
    }
    std::ios_base::openmode mode = std::ios_base::out;
    if(truncate)
    {
        mode |= std::ios_base::trunc;
    }
    mFileStream.open(filename, mode);
}

void logger::flush()
{
    OutputDebugString(mStream.str().c_str());
    if(mLevel < 0)
    {
        mStream.str(std::string());
        return;
    }

    if(mWriteToCout)
    {
        std::cout << mStream.str();
        if(mIsUnbuffered)
        {
            std::cout.flush();
        }
    }

    if(mFileStream)
    {
        mFileStream << mStream.str();
        if(mIsUnbuffered)
        {
            mFileStream.flush();
        }
    }

    mStream.str(std::string());
}

log_entry::log_entry(logger& l, logger::LogLevel level):
    mLogger(l),
    mLevel(level)
{
    if(mLevel < 0) return;
    using namespace std::chrono;
    const milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    const seconds s = duration_cast<seconds>(ms);
    const std::time_t t = s.count();
    const std::size_t fractional_seconds = ms.count() % 1000;

    mLogger.lock();
    const char timeFormat[] = "%Y-%m-%d %H:%M:%S";
    const size_t buffSize = 100;
    char buff[buffSize];
    if(0 != std::strftime(buff, buffSize, timeFormat, std::localtime(&t)))
    {
        mLogger.get() << buff;
    }
    else
    {
        size_t dBuffSize = buffSize;
        std::unique_ptr<char[]> dbuff;
        do
        {
            dBuffSize *= 2;
            dbuff.reset(new char[dBuffSize]);
        }
        while(0 == std::strftime(dbuff.get(), dBuffSize, timeFormat, std::localtime(&t)));
        mLogger.get() << dbuff.get();
    }
    mLogger.get() << '.' << fractional_seconds << " ";

    if(level >= 0 && level <= logger::logDEBUG)
    {
        const char* levels[] = {"ERROR","WARNING","INFO","DEBUG"};
        mLogger.get() << levels[level];
    }
    mLogger.get() << ": ";
}

log_entry::~log_entry()
{
    if(mLevel < 0) return;
    mLogger.get() << std::endl;
    mLogger.flush();
    mLogger.unlock();
}
