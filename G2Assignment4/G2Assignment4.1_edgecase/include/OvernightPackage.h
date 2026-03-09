//==============================================================
// Filename    : OvernightPackage.h
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the OvernightPackage class definition, which represents a package to be shipped overnight.
// It contains information about the sender and receiver, as well as the weight of the package.
//==============================================================
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