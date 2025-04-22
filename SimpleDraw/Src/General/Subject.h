#ifndef SUBJECT_H__
#define SUBJECT_H__


#include <list>


class Observer;


//****************************************************************************
// class Subject
//
// class Subject was created together with class Observer. They serve to keep
// the GUI up to date, where the subject is the model and the observer is the
// GUI element.
//****************************************************************************
class Subject
{
public:
                                Subject() : mDestroying(false) {}
  virtual                       ~Subject();

  void                          Attach(Observer * anObserver);
  void                          Detach(Observer * anObserver);
  void                          Notify();

private:
  std::list<Observer *>         mObserverList;
  bool                          mDestroying;
};


#endif // SUBJECT_H__