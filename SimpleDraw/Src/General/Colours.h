#ifndef GENERAL_COLOURS_H__
#define GENERAL_COLOURS_H__


inline COLORREF GetBackgroundColour(COLORREF aColour)
{
  if (GetRValue(aColour) + GetGValue(aColour) * 2 + GetBValue(aColour) > 225 * 3)
    // Very light colour:
    return RGB(0x8f, 0x8f, 0x8f);
  else
    return RGB(0xff, 0xff, 0xff);
}


#endif //GENERAL_COLOURS_H__