#include "Settings.hpp"

#include "IniFile.hpp"

Settings::Settings(const std::string &filename)
{
    const IniFile file(filename);
    mMode             = (Settings::Mode)file.getInt("main","mode",(int)modeSW);
    mLogLevel         = (logger::LogLevel)file.getInt("main","log_level",logger::logDEBUG);
    mLogFile          = file.getString("main","log_file","debug.log");
    mAddFrame         = file.getInt("main","add_window_frame", 0);
    mUpdateInterval   = file.getInt("main","force_update_msec", -1);
    mShowDebugInfo    = file.getInt("main","show_debug_info", 0);
    mDrawOnScreen     = file.getInt("main","draw_on_screen", 0);
    mHookGetCursorPos = file.getInt("main","hook_getcursorpos", 0);
}
