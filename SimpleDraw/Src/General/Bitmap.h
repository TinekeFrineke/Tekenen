#ifndef BITMAP_H__
#define BITMAP_H__


#include <windows.h>
#include <string>

bool SaveBitmap(HDC hDC, HBITMAP hBitmap, const std::wstring & aFilename);

#endif // BITMAP_H__


