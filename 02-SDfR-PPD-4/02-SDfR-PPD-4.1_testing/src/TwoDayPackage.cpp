//==============================================================
// Filename    : TwoDayPackage.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the TwoDayPackage class implementation,
// which inherits from the Package class.
//==============================================================

#include "TwoDayPackage.h"

TwoDayPackage::TwoDayPackage(std::string sName, std::string sAddr,
                             std::string rName, std::string rAddr,
                             double w)
    : Package(sName, sAddr, rName, rAddr, w) {}