#ifndef OVERNIGHTPACKAGE_H
#define OVERNIGHTPACKAGE_H

#include "Package.h"

class OvernightPackage : public Package {
public:
    OvernightPackage(std::string sName, std::string sAddr,
                     std::string rName, std::string rAddr,
                     double w);

    double calculate_Cost() const override;
};

#endif