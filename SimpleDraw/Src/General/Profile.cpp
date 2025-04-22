#include "Profile.h"

#include <assert.h>
#include <windows.h>


UserProfile::UserProfile(const std::tstring & aName)
: mUserName     (aName),
  mShowFilenames(true)
{
}


Profile * Profile::mInstance = 0;


Profile::Profile()
: mUser         (-1)
{
  mInstance = this;
  TCHAR directory[_MAX_PATH];
  GetCurrentDirectory(_MAX_PATH, directory);
  mCurrentDir = directory;
}

Profile::~Profile()
{
  mUser = -1;

  for (size_t i = 0; i < mProfiles.size(); ++i)
    delete mProfiles[i];

  mInstance = NULL;
}


int Profile::AddProfile(UserProfile * aProfile)
{
  mProfiles.push_back(aProfile);
  if (mUser == -1)
    mUser = 0;

  return static_cast<int>(mProfiles.size()) - 1;
}


UserProfile & Profile::GetUser()
{
  assert(mUser != -1 && mUser < static_cast<int>(mProfiles.size()));
  return *mProfiles[mUser];
}


bool Profile::SetUser(int aUser)
{
  if (aUser < 0 || aUser >= static_cast<int>(mProfiles.size()))
    return false;

  mUser = aUser;
  return true;
}


bool Profile::SetUser(const std::tstring & aUser)
{
  for (size_t i = 0; i < mProfiles.size(); ++i)
  {
    if (mProfiles[i]->GetName() != aUser)
      continue;

    mUser = static_cast<int>(i);
    return true;
  }

  return false;
}


int Profile::NumberOfUsers() const
{
  return static_cast<int>(mProfiles.size());
}


const UserProfile & Profile::GetUser() const
{
  assert(mUser >= 0 && mUser < static_cast<int>(mProfiles.size()));
  return *mProfiles[mUser];
}


const UserProfile & Profile::GetUser(int anIndex) const
{
  assert(anIndex >= 0 && anIndex < static_cast<int>(mProfiles.size()));
  return *mProfiles[anIndex];
}


void Profile::GetFontNames(std::vector<std::tstring> & aNames) const
{
  static const TCHAR * fontNames[] =
  {
    _T("Alba"),
    _T("Alba Super"),
    _T("Andy"),
    _T("Arial"),
    _T("Baby Kruffy"),
    _T("Bauhaus 93"),
    _T("Bickley Script"),
    _T("Braggadocio"),
    _T("Chick"),
    _T("Cooper Black"),
    _T("Copperplate"),
    _T("Frosty"),
    _T("Gigi"),
    _T("Imprint MT Shadow"),
    _T("Jokerman"),
    _T("Jokewood"),
    _T("MATTEROFFACT"),
    _T("Papyrus"),
    _T("Porky's"),
    _T("Times New Roman"),
  };

  aNames.clear();

  for (int i = 0; i < sizeof(fontNames) / sizeof(fontNames[0]); ++i)
    aNames.push_back(fontNames[i]);
}


FastProfile::FastProfile()
{
  FastUserProfile * profile1 = new FastUserProfile(_T("Natasja"));
  profile1->SetBitmap(Profile::GetInstance().GetUserDirectory() + _T("\\Natasja.bmp"));
  AddProfile(profile1);

  FastUserProfile * profile2 = new FastUserProfile(_T("Stijn"));
  profile2->SetBitmap(Profile::GetInstance().GetUserDirectory() + _T("\\Stijn.bmp"));
  AddProfile(profile2);

  FastUserProfile * profile3 = new FastUserProfile(_T("Guest"));
  profile3->SetBitmap(Profile::GetInstance().GetUserDirectory() + _T("\\Guest.bmp"));
  AddProfile(profile3);
}
