#ifndef FILEDIALOG_H__
#define FILEDIALOG_H__

#include <map>
#include <vector>

#include "Dialog/Dialog.h"

#include "General/Profile.h"

class DoubleBitmapButton;
class Button;
class FileControl;
class UserButtonBar;


class FileDialog : public Dialog
{
public:
                              FileDialog(Window * aParent, HINSTANCE hInstance,
                                         bool bExistingOnly);
                              ~FileDialog();

  void                        ShowModal();

  std::tstring                GetFilename() const;

private:
  void                        SelectUser(const std::tstring & aUser);

  LRESULT                     OnWindowPosChanged(const WINDOWPOS & aPos);
  LRESULT                     OnButtonClicked(Button * aButton);

  virtual LRESULT             WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  void                        AddButton(int iResource, int iPressedResource, int iID);

  // Index of first displayed item in mEntries array
  int                         mFirstIndex;
  std::vector<std::tstring>   mEntries;
  // First image to show
  std::tstring                mFileMask;

  UserButtonBar *             mButtonBar;
  DoubleBitmapButton *        mOKButton;
  DoubleBitmapButton *        mCloseButton;

  DoubleBitmapButton *        mButtonUp;
  DoubleBitmapButton *        mButtonDown;
  FileControl *               mFileControl;
};


#endif // FILEDIALOG_H__