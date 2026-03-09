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

    virtual ~Package() {}
};

#endif