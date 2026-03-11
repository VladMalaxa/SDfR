//==============================================================
// Filename    : main.cpp
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the Package class implementation.
//==============================================================
#include "Package.h"

// Constructor: Initialize a Package object with sender/receiver info and package weight
Package::Package(std::string sName, std::string sAddr,
                 std::string rName, std::string rAddr,
                 double w)
    : senderName(sName),
      senderAddress(sAddr),
      receiverName(rName),
      receiverAddress(rAddr),
      weight(w) {}

//Function to calculate the cost of package
double Package::calculate_Cost() const {
    return weight * 2.50 + 5.0;
}

// Getter function that returns the sender's name
std::string Package::getSenderName() const {
    return senderName;
}

// Getter function that returns the sender's address
std::string Package::getSenderAddress() const {
    return senderAddress;
}

// Getter function that returns the sender's address
std::string Package::getReceiverName() const {
    return receiverName;
}

// Getter function that returns the receiver's address
std::string Package::getReceiverAddress() const {
    return receiverAddress;
}

// Getter function that returns the package weight
double Package::getWeight() const {
    return weight;
}