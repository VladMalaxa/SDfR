#ifndef TWODAYPACKAGE_H
#define TWODAYPACKAGE_H

#include "Package.h"

class TwoDayPackage : public Package {
public:
    TwoDayPackage(std::string sName, std::string sAddr,
                  std::string rName, std::string rAddr,
                  double w);
};

#endif