#pragma once


#include <string>
#include <vector>

class UserProfile
{
public:
    UserProfile(const std::string& aName);
    virtual ~UserProfile() {}

    std::string GetName() const { return mUserName; }
    std::string GetBitmapName() const { return mBitmapName; }
    bool IsShowFilenames() const { return mShowFilenames; }

protected:
    virtual void SetBitmap(const std::string& aName) { mBitmapName = aName; }
    virtual void SetShowFilenames(bool aShow) { mShowFilenames = aShow; }

private:
    std::string mUserName;
    std::string mBitmapName;
    bool mShowFilenames;
};


class Profile
{
public:
    Profile(/*ID_USER aUser*/);
    virtual ~Profile() = 0;

    std::string        GetWorkDirectory() const { return mCurrentDir; }
    std::string        GetResourceDirectory() const { return GetWorkDirectory() + "\\Src\\Resource"; }
    std::string        GetDataDirectory() const { return GetWorkDirectory() + "\\Data"; }
    std::string        GetStampDirectory() const { return GetDataDirectory() + "\\Stamp"; }
    std::string        GetUserDirectory() const { return GetDataDirectory() + "\\User"; }

    void                GetFontNames(std::vector<std::string>& aNames) const;

    int                 AddProfile(UserProfile* aProfile);
    bool                SetUser(const std::string& aUser);
    bool                SetUser(int aUser);

    int                 NumberOfUsers() const;

    const UserProfile& GetUser() const;
    const UserProfile& GetUser(int anIndex) const;

    static const Profile& GetConstInstance() { return *mInstance; }
    static Profile& GetInstance() { return *mInstance; }

protected:
    UserProfile& GetUser();

private:
    std::string        mCurrentDir;

    static Profile* mInstance;

    typedef std::vector<UserProfile*> UserProfileList;

    UserProfileList     mProfiles;
    int                 mUser;
};


class FastProfile: public Profile
{
public:
    FastProfile();
    virtual             ~FastProfile() {}

private:
};


class FastUserProfile: public UserProfile
{
public:
    FastUserProfile(const std::string& aName) : UserProfile(aName) {}

    void                SetBitmap(const std::string& aName) { UserProfile::SetBitmap(aName); }
    void                SetShowFilenames(bool aShow) { UserProfile::SetShowFilenames(aShow); }
};
