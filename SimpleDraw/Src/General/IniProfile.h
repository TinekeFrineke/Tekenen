#ifndef GENERAL_INIPROFILE_H__
#define GENERAL_INIPROFILE_H__


#include "Profile.h"


class IniUserProfile : public UserProfile
{
public:
                      IniUserProfile(const std::tstring & aName)
                      : UserProfile(aName) {}

  virtual void        SetBitmap(const std::tstring & aName) { UserProfile::SetBitmap(aName); }
  virtual void        SetShowFilenames(bool aShow)          { UserProfile::SetShowFilenames(aShow); }

};

class IniProfile : public Profile
{
public:
                      IniProfile(const std::tstring & anIniFile);
                      ~IniProfile();

private:
  void                ReadUser(const std::tstring & aUser);

  std::tstring        mFileName;
};


#endif // GENERAL_INIPROFILE_H__