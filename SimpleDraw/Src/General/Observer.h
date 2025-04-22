#ifndef OBSERVER_H__
#define OBSERVER_H__


class Subject;


//****************************************************************************
// class Observer
//
// class Observer was created together with class Subject. They serve to keep
// the GUI up to date, where the subject is the model and the observer is the
// GUI element.
//****************************************************************************
class Observer
{
public:
  virtual                         ~Observer();

  virtual void                    Update(Subject * aSubject) = 0;
  virtual void                    Destroyed(Subject * aSubject) = 0;

protected:
                                  Observer();

private:
};


#endif // OBSERVER_H__