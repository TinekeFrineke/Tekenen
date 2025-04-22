#ifndef CONTROLLER_H__
#define CONTROLLER_H__


#include <windows.h>

#include <list>
#include <string>

#include "General.h"

class DrawingWindow;
class GeneralToolMenu;
class MainWindow;
class PickDialog;
class Profile;
class SpecialToolMenu;


class DrawingAttributes
{
public:
                              DrawingAttributes();

  COLORREF                    mColour;
  int                         mThickness;
};


class Controller
{
public:
                              Controller(HINSTANCE hInstance);
                              ~Controller();

  void                        LoadBitmap();
  void                        SaveBitmap();
  void                        ClearWindow();

  void                        SetState(Tekenen::DRAW_STATE aState);

  std::tstring                PickFile(bool bExistingOnly) const;

  const Profile &             GetProfile() const { return *mProfile; }
  DrawingAttributes &         GetAttributes() { return mAttributes; }
  const DrawingAttributes &   GetAttributes() const { return mAttributes; }
    
  static Controller &         GetInstance();

  void                        StoreBitmap(HBITMAP aBitmap);
  void                        FlushRedo();
  bool                        CanUndo() const;
  bool                        CanRedo() const;
  void                        Undo();
  void                        Redo();

private:
  static Controller *         mInstance;

  std::tstring                PickMask();
  std::tstring                GetNewFilename() const;

  HINSTANCE                   mHInstance;

  DrawingAttributes           mAttributes;

  MainWindow *                mMainWindow;
  DrawingWindow *             mDrawingWindow;
  GeneralToolMenu *           mGeneralToolMenu;
  SpecialToolMenu *           mSpecialToolMenu;

  Profile *                   mProfile;
  std::tstring                mName;
  // Undo / redo bitmaps
  std::list<HBITMAP>          mUndoBitmaps;
  std::list<HBITMAP>          mRedoBitmaps;
};


#endif