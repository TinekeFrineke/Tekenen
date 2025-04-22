#ifndef GENERAL_FONT_H__
#define GENERAL_FONT_H__


#include "Window/Window.h"

class Font
{
public:
                            Font(const std::tstring & mName,
                                 int aSize);
                            Font(const Font & aFont);
  Font &                    operator=(const Font & aFont);

                            ~Font();

  void                      SetName(const std::tstring & aName);
  void                      SetSize(int aSize);
  HFONT                     GetHandle();
  const std::tstring &      GetName() const { return mName; }
  int                       GetSize() const { return mSize; }

private:
  HFONT                     Create();

  std::tstring              mName;
  int                       mSize;
  HFONT                     mFontHandle;
};


inline bool operator==(const Font & aFont1, const Font & aFont2)
{
  return aFont1.GetName() == aFont2.GetName() &&
         aFont1.GetSize() == aFont2.GetSize();
}


#endif // GENERAL_FONT_H__