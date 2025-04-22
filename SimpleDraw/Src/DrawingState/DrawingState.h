#ifndef SIMPLEDRAW_DRAWINGSTATE_DRAWINGSTATE_H__
#define SIMPLEDRAW_DRAWINGSTATE_DRAWINGSTATE_H__

#include <windows.h>

#include <string>

#include "General/Controller.h"

class Controller;
class DrawingAttributes;
class DrawingWindow;

class DrawingState
{
public:
                        DrawingState(DrawingWindow & aWindow,
                                     HINSTANCE anInstance,
                                     Controller & aController,
                                     DrawingAttributes & anAttributes);
  virtual               ~DrawingState() {}

  virtual LRESULT       ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

  const DrawingAttributes &
                        GetAttributes() const { return mAttributes; }

protected:
  void                  Store();
  void                  FlushRedo();

  DrawingWindow &       GetWindow()         { return mWindow; }
  DrawingAttributes &   GetAttributes()     { return mAttributes; }
  HINSTANCE             GetInstance() const { return mInstance; }

private:
  // Prohibit copying
                        DrawingState(const DrawingState & aState);
  // Prohibit assignment
  DrawingState &        operator=(const DrawingState & aState);
  DrawingWindow &       mWindow;
  HINSTANCE             mInstance;
  DrawingAttributes &   mAttributes;
  Controller &          mController;
};


class DrawingStateWithCursor : public DrawingState
{
public:
                        DrawingStateWithCursor(DrawingWindow &    aWindow,
                                               HINSTANCE          anInstance,
                                               Controller &       aController,
                                               DrawingAttributes &anAttributes,
                                               int                iCursor);
  virtual               ~DrawingStateWithCursor();

  virtual LRESULT       ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

protected:

  bool                  ProcessMouseMove(int anX, int anY);

private:
  // Prohibit copying
                        DrawingStateWithCursor(const DrawingStateWithCursor & aState);
  // Prohibit assignment
  DrawingStateWithCursor &
                        operator=(const DrawingStateWithCursor & aState);

  HCURSOR               mCursor;
  HCURSOR               mOldCursor;
};


class SimpleDrawingState : public DrawingState
{
public:
                        SimpleDrawingState(DrawingWindow &      aWindow,
                                           HINSTANCE            anInstance,
                                           Controller &         aController,
                                           DrawingAttributes &  anAttributes);
  virtual               ~SimpleDrawingState();

  virtual LRESULT       ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

  virtual void          SetColour(COLORREF aColour);

  virtual void          SetThickness(int aThickness);

private:
  LRESULT               ProcessLButtonDown(int anX, int anY);
  LRESULT               ProcessMouseMove(int anX, int anY);
  LRESULT               ProcessLButtonUp(int anX, int anY);

  bool                  mDrawing;
};

#endif // SIMPLEDRAW_DRAWINGSTATE_DRAWINGSTATE_H__
