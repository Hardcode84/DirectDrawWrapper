#include "IniFile.hpp"

#include "windows.h"

IniFile::IniFile(const std::string& file):
    mFilename(file)
{
}

std::string IniFile::getString(const std::string& group,
                               const std::string& key,
                               const std::string& def) const
{
    const size_t BuffSize = 100;
    char buff[BuffSize];
    GetPrivateProfileStringA(group.c_str(), key.c_str(), def.c_str(), buff, BuffSize, mFilename.c_str());
    return std::string(buff);
}

int IniFile::getInt(const std::string& group,
                    const std::string& key,
                    int def) const
{
    return GetPrivateProfileIntA(group.c_str(), key.c_str(), def, mFilename.c_str());
}
