//==============================================================
// Filename    : TwoDayPackage.h
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the TwoDayPackage class definition,
// which inherits from the Package class.
//==============================================================

#ifndef TWODAYPACKAGE_H
#define TWODAYPACKAGE_H

#include "Package.h"

// TwoDayPackage class declaration, inherits from the Package class
class TwoDayPackage : public Package {
public:

    // Constructor for the TwoDayPackage class
    // Initializes sender and receiver information along with package weight
    TwoDayPackage(std::string sName, std::string sAddr,
                  std::string rName, std::string rAddr,
                  double w);
};

#endif