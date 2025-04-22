#ifndef CUSTOM_PRECENTAGECONTROLLER_H__
#define CUSTOM_PRECENTAGECONTROLLER_H__


#include "General/Subject.h"


class PercentageController : public Subject
{
public:
  virtual void          SetPercentage(int iPercentage) = 0;
  virtual int           GetPercentage() const = 0;

private:
};



#endif // CUSTOM_PRECENTAGECONTROLLER_H__