#include "HoverButtonBar.h"

#include <assert.h>

#include "Resource/resource.h"

#include "Button/HoverButton.h"
#include "Button/BitmapButton.h"
#include "Control/ButtonBar.h"
#include "General/Profile.h"
#include "Stamping/StampState.h"


HoverButtonBar::HoverButtonBar(Window* aParent, HINSTANCE hInstance,
                               StampState& aState,
                               int aRows, int aColumns)
    : FrameWindow(aParent, hInstance, "HoverButtonBar"),
    mState(aState),
    mRows(aRows),
    mColumns(aColumns),
    mBack(NULL),
    mForward(NULL)
{
    mBack = new SingleBitmapButton(this, hInstance, true, "BackButton");
    mForward = new SingleBitmapButton(this, hInstance, true, "ForwardButton");

    mBack->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\arrowleft.bmp");
    mForward->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\arrowright.bmp");

    for (int row = 0; row < aRows; ++row)
    {
        ButtonBar* buttonbar = new ButtonBar(this, hInstance,
                                             "HoverButtonBarButtonBar",
                                             ButtonBar::LAYOUT_STRATEGY::HorizontalFit);
        mButtonBars.push_back(buttonbar);

        for (int column = 0; column < aColumns; ++column)
            AddButton(buttonbar, new HoverButton(buttonbar, hInstance, mState, 0, 0));
    }
}


void HoverButtonBar::FillButtons()
{

    for (size_t i = 0; i < mButtons.size(); ++i)
    {
        if (i + mCurrentIndex < mFiles.size())
            mButtons[i]->SetFile(mFiles[i + mCurrentIndex]);
        else
            mButtons[i]->SetFile("");

        InvalidateRect(mButtons[i]->GetHwnd(), NULL, false);
    }
}


void HoverButtonBar::ScrollForward()
{
    mCurrentIndex = std::min(mCurrentIndex + mRows * mColumns, int(mFiles.size()) - mRows * mColumns);
    mCurrentIndex = std::max(mCurrentIndex, 0);

    FillButtons();
}


void HoverButtonBar::ScrollBack()
{
    mCurrentIndex -= mRows * mColumns;
    mCurrentIndex = std::max(mCurrentIndex, 0);

    FillButtons();
}


LRESULT HoverButtonBar::OnButtonClicked(Button* aButton)
{
    if (aButton == 0)
        return -1;

    if (aButton == mForward)
    {
        ScrollForward();
        return 0;
    }
    else if (aButton == mBack)
    {
        ScrollBack();
        return 0;
    }

    HoverButton* button = dynamic_cast<HoverButton*>(aButton);
    if (button == NULL)
        return -1;

    HoverWindow* window = button->CreateImage();
    if (window)
        mState.SetHoverWindow(window);

    return 0;
}


LRESULT HoverButtonBar::OnWindowPosChanged(const WINDOWPOS& oPos)
{
    (void)oPos;

    RECT rect;
    GetClientRect(rect);
    int iHeight = (rect.bottom - rect.top) / (int)mButtonBars.size();
    int iWidth = int((rect.right - rect.left) * 0.94);

    mBack->SetWindowPos(NULL, rect.left, rect.top, int((rect.right - rect.left) * 0.03), rect.bottom - rect.top, SWP_SHOWWINDOW);
    mForward->SetWindowPos(NULL, rect.left + int((rect.right - rect.left) * 0.97), rect.top, int((rect.right - rect.left) * 0.03), rect.bottom - rect.top, SWP_SHOWWINDOW);

    for (size_t i = 0; i < mButtonBars.size(); ++i)
        mButtonBars[i]->SetWindowPos(NULL, rect.left + int((rect.right - rect.left) * 0.03), rect.top + iHeight * (int)i,
                                     iWidth, iHeight,
                                     SWP_SHOWWINDOW);

    return 0;
}


LRESULT HoverButtonBar::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_USER_BUTTON_CLICKED:
            return OnButtonClicked((Button*)lParam);

        default:
            return FrameWindow::WindowProc(uMsg, wParam, lParam);
    }
}


void HoverButtonBar::AddButton(ButtonBar* aButtonBar, HoverButton* aButton)
{
    assert(aButtonBar != NULL);
    assert(aButton != NULL);

    aButtonBar->AddButton(aButton, int(mButtons.size()) - 1);
    mButtons.push_back(aButton);
}


void HoverButtonBar::SetFiles(const std::vector<std::string>& aFiles)
{
    mFiles = aFiles;
    mCurrentIndex = 0;

    FillButtons();
}
