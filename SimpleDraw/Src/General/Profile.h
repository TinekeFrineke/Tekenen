#ifndef PROFILE_H__
#define PROFILE_H__


#include <string>
#include <tchar.h>
#include <vector>

namespace std
{
#ifdef UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif
}


class UserProfile
{
public:
                      UserProfile(const std::tstring & aName);
  virtual             ~UserProfile(){}

  std::tstring        GetName() const               { return mUserName; }
  std::tstring        GetBitmapName() const         { return mBitmapName; }
  bool                IsShowFilenames() const       { return mShowFilenames; }

protected:
  virtual void        SetBitmap(const std::tstring & aName) { mBitmapName = aName; }
  virtual void        SetShowFilenames(bool aShow)          { mShowFilenames = aShow; }

private:
  std::tstring        mUserName;
  std::tstring        mBitmapName;
  bool                mShowFilenames;
};


class Profile
{
public:
                      Profile(/*ID_USER aUser*/);
  virtual             ~Profile() = 0;

  std::tstring        GetWorkDirectory() const      { return mCurrentDir; }
  std::tstring        GetDataDirectory() const      { return GetWorkDirectory() + _T("\\Data"); }
  std::tstring        GetStampDirectory() const     { return GetDataDirectory() + _T("\\Stamp"); }
  std::tstring        GetUserDirectory() const      { return GetDataDirectory() + _T("\\User"); }

  bool                GetShowFilenames() const;
  void                GetFontNames(std::vector<std::tstring> & aNames) const;

  int                 AddProfile(UserProfile * aProfile);
  bool                SetUser(const std::tstring & aUser);
  bool                SetUser(int aUser);

  int                 NumberOfUsers() const;

  const UserProfile & GetUser() const;
  const UserProfile & GetUser(int anIndex) const;

  static const Profile &
                      GetConstInstance() { return *mInstance; }
  static Profile &    GetInstance() { return *mInstance; }

protected:
  UserProfile &       GetUser();

private:
  std::tstring        mCurrentDir;

  static Profile *    mInstance;

  typedef std::vector<UserProfile *> UserProfileList;

  UserProfileList     mProfiles;
  int                 mUser;
};


class FastProfile : public Profile
{
public:
                      FastProfile();
  virtual             ~FastProfile() {}

private:
};


class FastUserProfile : public UserProfile
{
public:
                      FastUserProfile(const std::tstring & aName) : UserProfile(aName) {}

  void                SetBitmap(const std::tstring & aName) { UserProfile::SetBitmap(aName); }
  void                SetShowFilenames(bool aShow)          { UserProfile::SetShowFilenames(aShow); }

};


#endif // PROFILE_H__
