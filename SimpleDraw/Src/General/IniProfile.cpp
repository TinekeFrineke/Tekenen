
#include "iniprofile.h"

#include <windows.h>


IniProfile::IniProfile(const std::tstring & anIniFile)
: mFileName(anIniFile)
{
  std::tstring filename = GetWorkDirectory() + _T("\\") + mFileName;
  DWORD size = 1024;
  TCHAR * section = new TCHAR[1024];
  DWORD result = GetPrivateProfileSection(_T("users"), section, size, filename.c_str());
  while (result == size - 2)
  {
    delete[] section;
    size *= 2;
    section = new TCHAR[1024];
    result = GetPrivateProfileSection(_T("users"), section, size, filename.c_str());
  }

  TCHAR * cursor = section;
  while (DWORD(cursor - section) < result)
  {
    std::tstring kvp = cursor;
    size_t pos = kvp.find(_T('='));
    if (pos != std::string::npos)
    {
      ReadUser(kvp.substr(pos + 1));
    }

    cursor += kvp.length() + 1;
  }

  delete[] section;
}


void IniProfile::ReadUser(const std::tstring & aUser)
{
  if (aUser.length() == 0)
    return;

  std::tstring filename = GetWorkDirectory() + _T("\\") + mFileName;

  IniUserProfile * profile = new IniUserProfile(aUser);
  AddProfile(profile);

  TCHAR showfilenames[10];
  GetPrivateProfileString(aUser.c_str(), _T("ShowFileNames"), _T("false"), showfilenames, 10, mFileName.c_str());
  profile->SetShowFilenames(_tcscmp(showfilenames, _T("true")) == 0);

  TCHAR bitmap[_MAX_PATH];
  GetPrivateProfileString(aUser.c_str(), _T("BitMap"), _T(""), bitmap, _MAX_PATH, filename.c_str());
  profile->SetBitmap(bitmap);
}


IniProfile::~IniProfile()
{
}
