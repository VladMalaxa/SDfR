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

class Package {
protected:
    std::string senderName;
    std::string senderAddress;
    std::string receiverName;
    std::string receiverAddress;
    double weight;

public:
    Package(std::string sName, std::string sAddr,
            std::string rName, std::string rAddr,
            double w);

    virtual double calculate_Cost() const;

    std::string getSenderName() const;
    std::string getSenderAddress() const;
    std::string getReceiverName() const;
    std::string getReceiverAddress() const;
    double getWeight() const;

    virtual ~Package() {}
};

#endif