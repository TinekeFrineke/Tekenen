#include "Profile.h"

#include <assert.h>
#include <windows.h>


UserProfile::UserProfile(const std::string& aName)
    : mUserName(aName),
    mShowFilenames(true)
{
}


Profile* Profile::mInstance = 0;


Profile::Profile()
    : mUser(-1)
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


int Profile::AddProfile(UserProfile* aProfile)
{
    mProfiles.push_back(aProfile);
    if (mUser == -1)
        mUser = 0;

    return static_cast<int>(mProfiles.size()) - 1;
}


UserProfile& Profile::GetUser()
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


bool Profile::SetUser(const std::string& aUser)
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


const UserProfile& Profile::GetUser() const
{
    assert(mUser >= 0 && mUser < static_cast<int>(mProfiles.size()));
    return *mProfiles[mUser];
}


const UserProfile& Profile::GetUser(int anIndex) const
{
    assert(anIndex >= 0 && anIndex < static_cast<int>(mProfiles.size()));
    return *mProfiles[anIndex];
}


void Profile::GetFontNames(std::vector<std::string>& aNames) const
{
    static const TCHAR* fontNames[] =
    {
      "Alba",
      "Alba Super",
      "Andy",
      "Arial",
      "Baby Kruffy",
      "Bauhaus 93",
      "Bickley Script",
      "Braggadocio",
      "Chick",
      "Cooper Black",
      "Copperplate",
      "Frosty",
      "Gigi",
      "Imprint MT Shadow",
      "Jokerman",
      "Jokewood",
      "MATTEROFFACT",
      "Papyrus",
      "Porky's",
      "Times New Roman",
    };

    aNames.clear();

    for (int i = 0; i < sizeof(fontNames) / sizeof(fontNames[0]); ++i)
        aNames.push_back(fontNames[i]);
}


FastProfile::FastProfile()
{
    FastUserProfile* profile1 = new FastUserProfile("Natasja");
    profile1->SetBitmap(Profile::GetInstance().GetUserDirectory() + "\\Natasja.bmp");
    AddProfile(profile1);

    FastUserProfile* profile2 = new FastUserProfile("Stijn");
    profile2->SetBitmap(Profile::GetInstance().GetUserDirectory() + "\\Stijn.bmp");
    AddProfile(profile2);

    FastUserProfile* profile3 = new FastUserProfile("Guest");
    profile3->SetBitmap(Profile::GetInstance().GetUserDirectory() + "\\Guest.bmp");
    AddProfile(profile3);
}
