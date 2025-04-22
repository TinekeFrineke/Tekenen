#ifndef BITMAPBUTTON_H__
#define BITMAPBUTTON_H__


#include "Button.h"

#include <string>


class BitmapButtonBase : public Button
{
public:
                          BitmapButtonBase(Window * aParent, HINSTANCE hInstance,
                                           bool bDeformAllowed, const std::tstring & aName,
                                           BUTTON_STRATEGY aStrategy);
                          ~BitmapButtonBase();

protected:
  struct BmpInfo
  {
                          BmpInfo() : mHBitmap(NULL), mWidth(0), mHeight(0) {}

    HBITMAP               mHBitmap;
    int                   mWidth;
    int                   mHeight;
  };

  bool                    LoadBitmap(LPCTSTR bitmapname, BmpInfo & anInfo);
  bool                    Show(HDC dc, const BmpInfo & anInfo);
  bool                    IsDeformAllowed() const { return mDeformAllowed; }
  void                    DrawRectangle(HDC aDC, const RECT & aRect, COLORREF aColour);

private:
  bool                    mDeformAllowed;
};


class SingleBitmapButton : public BitmapButtonBase
{
public:
                          SingleBitmapButton(Window * aParent, HINSTANCE hInstance,
                                             bool bDeformAllowed, const std::tstring & aName,
                                             BUTTON_STRATEGY aStrategy = BS_PushButton);
                          ~SingleBitmapButton();

  void                    SetBitmap(int iResource);
  void                    SetSelectedColour(COLORREF aColour);

protected:
  virtual LRESULT         OnPaint();

private:
  BmpInfo                 mBitmap;
  COLORREF                mSelectedColour;
};


class DoubleBitmapButton : public BitmapButtonBase
{
public:
                          DoubleBitmapButton(Window * aParent, HINSTANCE hInstance,
                                             bool bDeformAllowed, const std::tstring & aName,
                                             BUTTON_STRATEGY aStrategy = BS_PushButton);
                          ~DoubleBitmapButton();

  void                    SetBitmap(int iResource, int iPressedResource);

protected:
  virtual LRESULT         OnPaint();

private:
  BmpInfo                 mBitmap;
  BmpInfo                 mPressedBitmap;
};


class VariableBitmapButton : public BitmapButtonBase
{
public:
                          VariableBitmapButton(Window * aParent, HINSTANCE hInstance,
                                               bool bDeformAllowed, const std::tstring & aName,
                                               BUTTON_STRATEGY aStrategy = BS_PushButton);
                          ~VariableBitmapButton();

  void                    SetBitmap(const std::tstring & aBitmap);
  void                    SetSelectedColour(COLORREF aColour);

protected:
  virtual LRESULT         OnPaint();

private:
  BmpInfo                 mBitmap;
  COLORREF                mSelectedColour;
};



#endif // BITMAPBUTTON_H__