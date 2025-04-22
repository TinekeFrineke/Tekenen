#include "subject.h"

#include <algorithm>

#include "observer.h"


//----------------------------------------------------------------------------
Subject::~Subject()
{
  mDestroying = true;

  for (std::list<Observer *>::iterator iter = mObserverList.begin();
       iter != mObserverList.end();
       ++iter)
    (*iter)->Destroyed(this);
}


//----------------------------------------------------------------------------
void Subject::Attach(Observer * anObserver)
{
  if (std::find(mObserverList.begin(), mObserverList.end(), anObserver) !=
      mObserverList.end())
    return;

  mObserverList.push_back(anObserver);
}


//----------------------------------------------------------------------------
void Subject::Detach(Observer * anObserver)
{
  if (mDestroying)
    return;

  std::list<Observer *>::iterator iter = std::find(mObserverList.begin(),
                                                   mObserverList.end(), anObserver);
  if (iter == mObserverList.end())
    return;

  mObserverList.erase(iter);
}


//----------------------------------------------------------------------------
void Subject::Notify()
{
  for (std::list<Observer *>::iterator iter = mObserverList.begin();
       iter != mObserverList.end();
       ++iter)
    (*iter)->Update(this);
}
