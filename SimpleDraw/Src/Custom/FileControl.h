#ifndef CUSTOM_FILECONTROL_H__
#define CUSTOM_FILECONTROL_H__

#include "Window/FrameWindow.h"

#include <vector>

class Button;
class VariableBitmapButton;

class FileControl : public FrameWindow
{
public:
                              FileControl(Window * aParent, HINSTANCE hInstance, bool bExistingOnly, const std::tstring & aName = _T("FileControl"));

  void                        SetFiles(const std::vector<std::tstring> & aFiles);

  void                        ScrollUp();
  void                        ScrollDown();

  std::tstring                GetSelectedFile() const;

  LRESULT                     WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
  void                        UpdateFiles();

  LRESULT                     OnWindowPosChanged(const WINDOWPOS & oPos);
  LRESULT                     OnButtonClicked(Button * aButton);

  bool                        mExistingOnly;
  int                         mIndex;
  std::vector<std::tstring>   mFiles;
  std::vector<VariableBitmapButton *> mCells;
  Button *                    mSelectedButton;
};


#endif // CUSTOM_FILECONTROL_H__