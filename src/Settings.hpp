#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include "simple_logger/logger.hpp"

class Settings
{
public:
    Settings(const std::string& filename);
    enum Mode
    {
        modeClear = 0,
        modeLogger,
        modeSW
    };

    Mode mode() const { return mMode; }
    logger::LogLevel logLevel() const { return mLogLevel; }
    std::string logFile()   const { return mLogFile; }
    bool addWindowFrame()   const { return mAddFrame; }
    int forceUpdateMsec()   const { return mUpdateInterval; }
    bool showDebugInfo()    const { return mShowDebugInfo; }
    bool drawOnScreen()     const { return mDrawOnScreen; }
    bool hookGetCursorPos() const { return mHookGetCursorPos; }

private:
    Mode mMode;
    logger::LogLevel mLogLevel;
    std::string mLogFile;
    bool mAddFrame;
    int mUpdateInterval;
    bool mShowDebugInfo;
    bool mDrawOnScreen;
    bool mHookGetCursorPos;
};

#endif // SETTINGS_HPP
