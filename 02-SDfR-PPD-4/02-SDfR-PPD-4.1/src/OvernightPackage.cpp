//==============================================================
// Filename    : OvernightPackage.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the OvernightPackage class implementation, which represents a package to be shipped overnight.
// It contains information about the sender and receiver, as well as the weight of the package.
//==============================================================
#include "OvernightPackage.h"
#include <string>

// Constructor: Initialize a Package object with sender/receiver info and package weight
// by calling the constructor of the base class (Package).
OvernightPackage::OvernightPackage(std::string sName, std::string sAddr,
                                   std::string rName, std::string rAddr,
                                   double w)
    : Package(sName, sAddr, rName, rAddr, w) {}


// Function to calculate the cost of overnight shipping
// It first calculates the normal package cost using the base class
// method, then adds an extra overnight fee based on the package weight.
double OvernightPackage::calculate_Cost() const {
    return Package::calculate_Cost() + (weight * weight * 1.10);
}