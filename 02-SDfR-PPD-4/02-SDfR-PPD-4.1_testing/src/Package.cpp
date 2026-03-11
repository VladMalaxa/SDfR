//==============================================================
// Filename    : Package.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the Package class implementation.
//==============================================================
#include "Package.h"

Package::Package(std::string sName, std::string sAddr,
                 std::string rName, std::string rAddr,
                 double w)
    : senderName(sName),
      senderAddress(sAddr),
      receiverName(rName),
      receiverAddress(rAddr),
      weight(w) {}

double Package::calculate_Cost() const {
    return weight * 2.50 + 5.0;
}

std::string Package::getSenderName() const {
    return senderName;
}

std::string Package::getSenderAddress() const {
    return senderAddress;
}

std::string Package::getReceiverName() const {
    return receiverName;
}

std::string Package::getReceiverAddress() const {
    return receiverAddress;
}

double Package::getWeight() const {
    return weight;
}