#pragma once

#include "DrawingState/DrawingState.h"

#include <cmath>

class SpecialToolMenu;

class Vector
{
public:
    Vector() : mX(0), mY(0) {}
    Vector(int anX, int anY) : mX(anX), mY(anY) {}
    Vector(const POINT& aPoint) : mX(aPoint.x), mY(aPoint.y) {}

    int GetX() const { return mX; }
    int GetY() const { return mY; }

    int GetLength() const
    {
        return (int)sqrt(double(mX * mX + mY * mY));
    }
    int GetSquareLength() const
    {
        return mX * mX + mY * mY;
    }
    void SetLength(int aLength)
    {
        if (aLength <= 0)
            return;

        int length = (int)sqrt(double(mX * mX + mY * mY));
        mX *= aLength;
        mX /= length;
        mY *= aLength;
        mY /= length;
    }

    Vector& operator+=(const Vector& aVector)
    {
        return operator=(aVector + *this);
    }
    Vector& operator-=(const Vector& aVector)
    {
        return operator=(aVector - *this);
    }
    Vector operator-(const Vector& aVector) const
    {
        return Vector(mX - aVector.mX, mY - aVector.mY);
    }
    Vector operator+(const Vector& aVector) const
    {
        return Vector(mX + aVector.mX, mY + aVector.mY);
    }

    int SquareDistance(const Vector& aVector) const
    {
        return (mX - aVector.mX) * (mX - aVector.mX) + (mY - aVector.mY) * (mY - aVector.mY);
    }

    bool operator!=(const Vector& aVector)
    {
        return mX != aVector.mX || mY != aVector.mY;
    }

private:
    int         mX;
    int         mY;
};


class SmudgingState: public DrawingStateWithCursor
{
public:
    SmudgingState(DrawingWindow& aWindow,
                  HINSTANCE            anInstance,
                  Controller& aController,
                  SpecialToolMenu& aMenu,
                  DrawingAttributes& anAttributes);
    ~SmudgingState();

    virtual LRESULT       ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void          SetColour(COLORREF aColour);
    virtual void          SetThickness(int aThickness);

private:
    LRESULT               Smudge(const Vector& aTarget, const Vector& aWith);
    LRESULT               Smudge1(int anX, int anY);
    LRESULT               Smudge2(int anX, int anY);
    LRESULT               Smudge3(int anX, int anY);
    LRESULT               Smudge4(int anX, int anY);
    LRESULT               Smudge5(int anX, int anY);
    LRESULT               Smudge6(int anX, int anY);
    LRESULT               Smudge7(int anX, int anY);
    LRESULT               Smudge8(int anX, int anY);

    LRESULT               ProcessLButtonDown(int anX, int anY);
    LRESULT               ProcessMouseMove(int anX, int anY);
    LRESULT               ProcessLButtonUp(int anX, int anY);

    void                  Circle(int anX, int anY);

    bool                  mDrawing;
    POINT                 mLastPosition;
};
