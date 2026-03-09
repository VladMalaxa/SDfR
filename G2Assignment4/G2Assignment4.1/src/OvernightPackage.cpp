#include "OvernightPackage.h"
#include <string>
OvernightPackage::OvernightPackage(std::string sName, std::string sAddr,
                                   std::string rName, std::string rAddr,
                                   double w)
    : Package(sName, sAddr, rName, rAddr, w) {}

double OvernightPackage::calculate_Cost() const {
    return Package::calculate_Cost() + (weight * weight * 1.10);
}