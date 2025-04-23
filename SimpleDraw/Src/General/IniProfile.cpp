
#include "iniprofile.h"

#include <windows.h>


IniProfile::IniProfile(const std::string& anIniFile)
    : mFileName(anIniFile)
{
    std::string filename = GetWorkDirectory() + "\\" + mFileName;
    DWORD size = 1024;
    TCHAR* section = new TCHAR[1024];
    DWORD result = GetPrivateProfileSection("users", section, size, filename.c_str());
    while (result == size - 2)
    {
        delete[] section;
        size *= 2;
        section = new TCHAR[1024];
        result = GetPrivateProfileSection("users", section, size, filename.c_str());
    }

    TCHAR* cursor = section;
    while (DWORD(cursor - section) < result)
    {
        std::string kvp = cursor;
        size_t pos = kvp.find('=');
        if (pos != std::string::npos)
        {
            ReadUser(kvp.substr(pos + 1));
        }

        cursor += kvp.length() + 1;
    }

    delete[] section;
}


void IniProfile::ReadUser(const std::string& aUser)
{
    if (aUser.length() == 0)
        return;

    std::string filename = GetWorkDirectory() + "\\" + mFileName;

    IniUserProfile* profile = new IniUserProfile(aUser);
    AddProfile(profile);

    TCHAR showfilenames[10];
    GetPrivateProfileString(aUser.c_str(), "ShowFileNames", "false", showfilenames, 10, mFileName.c_str());
    profile->SetShowFilenames(strcmp(showfilenames, "true") == 0);

    TCHAR bitmap[_MAX_PATH];
    GetPrivateProfileString(aUser.c_str(), "BitMap", "", bitmap, _MAX_PATH, filename.c_str());
    profile->SetBitmap(bitmap);
}


IniProfile::~IniProfile()
{
}
