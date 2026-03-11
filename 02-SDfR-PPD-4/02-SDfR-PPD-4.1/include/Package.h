//==============================================================
// Filename    : Package.h
// Authors     : Vlad Malaxa s2726254, Ricardo Diaz s3681548
// Group       : PPD Group 2
// License     :  N.A. or opensource license like LGPL
// Description : This code consists of the Package class definition, which represents a package to be shipped.
// It contains information about the sender and receiver, as well as the weight of the package.
//==============================================================
#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>

// Base class representing a package
class Package {
protected:
    std::string senderName;
    std::string senderAddress;
    std::string receiverName;
    std::string receiverAddress;
    double weight;

public:
    // Constructor
    // Initializes sender and receiver information and the package weight
    Package(std::string sName, std::string sAddr,
            std::string rName, std::string rAddr,
            double w);

    // Virtual function to calculate the shipping cost, can be overriden by child classes        
    virtual double calculate_Cost() const;

    //getter functions for each attribute 
    std::string getSenderName() const;
    std::string getSenderAddress() const;
    std::string getReceiverName() const;
    std::string getReceiverAddress() const;
    double getWeight() const;

    //Virtual destructor, "virtual" ensures destructors are called in the correct order
    virtual ~Package() {}
};

#endif