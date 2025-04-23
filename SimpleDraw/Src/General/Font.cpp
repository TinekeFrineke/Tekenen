
#include "Font.h"

#include <assert.h>


Font::Font(const std::string& aName, int aSize)
    : mName(aName),
    mSize(aSize),
    mFontHandle(0)
{
}


Font::Font(const Font& aFont)
    : mFontHandle(NULL),
    mName(aFont.mName),
    mSize(aFont.mSize)
{
}


Font& Font::operator=(const Font& aFont)
{
    if (&aFont == this)
        return *this;

    if (mName == aFont.mName && mSize == aFont.mSize)
        return *this;

    if (mFontHandle != NULL)
    {
        DeleteObject(mFontHandle);
        mFontHandle = NULL;
    }

    mName = aFont.mName;
    mSize = aFont.mSize;
    return *this;
}


Font::~Font()
{
    if (mFontHandle != NULL)
        DeleteObject(mFontHandle);
}


void Font::SetName(const std::string& aName)
{
    if (mName == aName)
        return;

    if (mFontHandle != NULL)
    {
        DeleteObject(mFontHandle);
        mFontHandle = NULL;
    }

    mName = aName;
}


void Font::SetSize(int aSize)
{
    if (mSize == aSize)
        return;

    if (mFontHandle != NULL)
    {
        DeleteObject(mFontHandle);
        mFontHandle = NULL;
    }

    mSize = aSize;
}


HFONT Font::Create()
{
    if (mName.empty())
        return NULL;

    LOGFONT logfont;
    memset(&logfont, 0, sizeof(LOGFONT));

    assert(mName.length() < 32);
    strcpy(logfont.lfFaceName, mName.c_str());
    logfont.lfHeight = -mSize;

    return CreateFontIndirect(&logfont);
}


HFONT Font::GetHandle()
{
    if (mFontHandle == NULL)
        mFontHandle = Create();

    return mFontHandle;
}
