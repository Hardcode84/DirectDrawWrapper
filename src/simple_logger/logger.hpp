#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sstream>
#include <fstream>
#include <mutex>

class logger final
{
public:
    friend class log_entry;
    enum LogLevel
    {
        logDISABLE = -1,
        logERROR = 0,
        logWARNING,
        logINFO,
        logDEBUG
    };

    logger(LogLevel level = logDEBUG, bool writeToCout = true, bool isUnbuffered = false);
    ~logger();

    LogLevel level() const { return mLevel; }

    void setFile(const std::string& filename, bool truncate = true);

    bool isActive() const { return mLevel >= 0 && (mWriteToCout || mFileStream); }

private:
    std::stringstream& get() { return mStream; }
    void lock()   { mLock.lock(); }
    void unlock() { mLock.unlock(); }
    void flush();

    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

    LogLevel mLevel;
    bool mWriteToCout;
    bool mIsUnbuffered;

    std::mutex mLock;
    std::stringstream mStream;
    std::ofstream mFileStream;
};

class log_entry final
{
    logger& mLogger;
    const logger::LogLevel mLevel;
public:
    log_entry(logger& l, logger::LogLevel level);
    ~log_entry();

    std::stringstream& get() { return mLogger.get(); }
};

#define WRITE_LOG(log, lev) if((lev) > (log).level() || !(log).isActive()); \
else log_entry((log),(lev)).get()

template<typename T>
class scoped_helper;

template<>
class scoped_helper<char*>
{
    logger& mLogger;
    const char* mVal;
    const logger::LogLevel mLevel;
public:
    inline scoped_helper(const char* val, logger& log, logger::LogLevel level = logger::logDEBUG):
        mLogger(log),
        mVal(val),
        mLevel(level)
    {
        WRITE_LOG(mLogger, mLevel) << mVal << ": enter";
    }

    inline ~scoped_helper()
    {
        WRITE_LOG(mLogger, mLevel) << mVal << ": exit";
    }
};
template<size_t N>
class scoped_helper<char[N]>
{
    logger& mLogger;
    const char* mVal;
    const logger::LogLevel mLevel;
public:
    inline scoped_helper(const char val[N], logger& log, logger::LogLevel level = logger::logDEBUG):
        mLogger(log),
        mVal(val),
        mLevel(level)
    {
        WRITE_LOG(mLogger, mLevel) << mVal << " enter";
    }

    inline ~scoped_helper()
    {
        WRITE_LOG(mLogger, mLevel) << mVal << " exit";
    }
};

template<>
class scoped_helper<std::string>
{
    logger& mLogger;
    const std::string mVal;
    const logger::LogLevel mLevel;
public:
    inline scoped_helper(const std::string& val, logger& log, logger::LogLevel level = logger::logDEBUG):
        mLogger(log),
        mVal(val),
        mLevel(level)
    {
        WRITE_LOG(mLogger, mLevel) << mVal << ": enter";
    }

    inline ~scoped_helper()
    {
        WRITE_LOG(mLogger, mLevel) << mVal << ": exit";
    }
};

template<typename T>
scoped_helper<T> create_scoped_helper(const T& val, logger& log, logger::LogLevel level = logger::logDEBUG)
{
    return scoped_helper<T>(val, log, level);
}

#define LOG_UNIQ_NAME(line) log_scope_guard_##line
#define LOG_UNIQ_NAME2(line) LOG_UNIQ_NAME(line)
#define LOG_SCOPE(val,log,level) const auto LOG_UNIQ_NAME2(__LINE__) = create_scoped_helper(val,log,level)

#endif // LOGGER_HPP
