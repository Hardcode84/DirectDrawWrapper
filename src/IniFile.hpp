#ifndef INIFILE_HPP
#define INIFILE_HPP

#include <string>

class IniFile
{
    const std::string mFilename;
public:
    IniFile(const std::string& file);

    std::string getString(const std::string& group,
                          const std::string& key,
                          const std::string& def = std::string()) const;

    int getInt(const std::string& group,
               const std::string& key,
               int def = 0) const;
};

#endif // INIFILE_HPP
